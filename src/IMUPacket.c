///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// 2621 Wasco St.                //
// PO Box 1500                   //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#include "ByteOrder.h"
#include "IMUPacket.h"

// Telemetry packet parsing functions
static void DecodeRawGyroPacket(const IMUPacket_t *pPkt, IMUData_t *pData);
static void DecodeRawAccelPacket(const IMUPacket_t *pPkt, IMUData_t *pData);
static void DecodeTimingPacket(const IMUPacket_t *pPkt, IMUData_t *pData);
static void DecodeGyroTempPacket(const IMUPacket_t *pPkt, IMUData_t *pData,
	enum IMUSensorTempIndex_t Index);

// High-speed serial packet parsing functions
static void DecodeHighSpeedRawDataPacket(const IMUPacket_t *pPkt, IMUData_t *pData);
static void DecodeHighSpeedDataPacket(const IMUPacket_t *pPkt, IMUData_t *pData);

// System settings packet parsing functions
static void DecodeSettingsPacket(const IMUPacket_t *pPkt, IMUData_t *pData);
static void DecodeSoftwareVersionPacket(const IMUPacket_t *pPkt, IMUData_t *pData);
static void DecodeHardwareConfigPacket(const IMUPacket_t *pPkt, IMUData_t *pData);
static void DecodeDatesPacket(const IMUPacket_t *pPkt, IMUData_t *pData);
static void DecodeResolutionPacket(const IMUPacket_t *pPkt, IMUData_t *pData);

// Function to create a formed packet's header
static void MakeIMUPacket(IMUPacket_t *pPkt, UInt8 Type, UInt8 Len);


/*! Decodes an incoming packet from an IMU and stores all data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeIMUPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	switch (pPkt->type)
	{
	case RAWGYRO_IMU_MSG:         DecodeRawGyroPacket(pPkt, pData);                  break;
	case RAWACCEL_IMU_MSG:        DecodeRawAccelPacket(pPkt, pData);                 break;
	case TIMING_IMU_MSG:          DecodeTimingPacket(pPkt, pData);                   break;
	case RAWGYROTEMPX_IMU_MSG:    DecodeGyroTempPacket(pPkt, pData, GYROX_TEMP_IDX); break;
	case RAWGYROTEMPY_IMU_MSG:    DecodeGyroTempPacket(pPkt, pData, GYROY_TEMP_IDX); break;
	case RAWGYROTEMPZ_IMU_MSG:    DecodeGyroTempPacket(pPkt, pData, GYROZ_TEMP_IDX); break;
	case SETTINGS_IMU_MSG:        DecodeSettingsPacket(pPkt, pData);                 break;
	case SWVERSION_IMU_MSG:       DecodeSoftwareVersionPacket(pPkt, pData);          break;
	case SERIALNUMCONFIG_IMU_MSG: DecodeHardwareConfigPacket(pPkt, pData);           break;
	case MFRCALDATE_IMU_MSG:      DecodeDatesPacket(pPkt, pData);                    break;
	case RESOLUTION_IMU_MSG:      DecodeResolutionPacket(pPkt, pData);               break;
	case HS_RAW_IMU_MSG:          DecodeHighSpeedRawDataPacket(pPkt, pData);         break;
	case HS_SERIAL_IMU_MSG:       DecodeHighSpeedDataPacket(pPkt, pData);            break;
	default:                                                                         break;
	}

}// DecodeIMUPacket


/*! Decodes an incoming raw gyro packet from an IMU and stores the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeRawGyroPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;

	pData->SensorsVolts[GYROX_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_GYROVOLTS; i += 2;
	pData->SensorsVolts[GYROY_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_GYROVOLTS; i += 2;
	pData->SensorsVolts[GYROZ_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_GYROVOLTS; i += 2;
	pData->SequenceNumber = pPkt->data[i++];

}// DecodeRawGyroPacket


/*! Decodes an incoming raw accelerometer data packet from an IMU and stores
 *  the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeRawAccelPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;

	pData->SensorsVolts[ACCELX_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_VOLTS; i += 2;
	pData->SensorsVolts[ACCELY_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_VOLTS; i += 2;
	pData->SensorsVolts[ACCELZ_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_VOLTS; i += 2;
	pData->SequenceNumber = pPkt->data[i++];

}// DecodeRawAccelPacket


/*! Decodes an incoming raw accelerometer data packet from an IMU and stores
 *  the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeTimingPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;

	pData->TimeSincePPS   = DataToUInt32(&pPkt->data[i]) / 10000.0; i += 4;
	pData->PPSCount       = pPkt->data[i++];
	pData->SequenceNumber = pPkt->data[i++];
	pData->ClockError     = DataToSInt16(&pPkt->data[i]); i += 2;

}// DecodeRawAccelPacket


/*! Decodes an incoming gyro temperature telemetry packet from an IMU and
 *  stores the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeGyroTempPacket(const IMUPacket_t *pPkt, IMUData_t *pData,
	enum IMUSensorTempIndex_t Index)
{
	BOOL Invalid;

	pData->GyroTempVolts[Index] = DataToFloat(pPkt->data, &Invalid);

}// DecodeGyroTempPacket


/*! Decodes an incoming output settings packet from an IMU and stores
 *  the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeSettingsPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;

	pData->OutputDevice    = pPkt->data[i++];
	pData->OutputMode      = pPkt->data[i++];
	pData->OversampleRatio = DataToUInt16(&pPkt->data[i]); i += 2;
	pData->OutputRate      = 1.0e6 / DataToUInt32(&pPkt->data[i]);

}// DecodeSettingsPacket


/*! Decodes an incoming sensor resolution data packet from an IMU and stores
 *  the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeResolutionPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	BOOL Invalid;
	UInt8 i = 0;

	pData->GyroRange  = DataToFloat(&pPkt->data[i], &Invalid); i += 4;
	pData->AccelRange = DataToFloat(&pPkt->data[4], &Invalid); i += 4;

}// DecodeResolutionPacket


/*! Decodes an incoming hardware configuration packet from an IMU and stores
 *  the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeHardwareConfigPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;

	pData->SerialNumber  = DataToUInt16(&pPkt->data[i]); i += 2;
	pData->EepromVersion = pPkt->data[i++];
	pData->HwRevMajor    = pPkt->data[i++];
	pData->HwRevMinor    = pPkt->data[i++];
	pData->AccelConfig   = pPkt->data[i++];
	pData->GyroConfig    = pPkt->data[i++];
	pData->ConfigBits    = pPkt->data[i++];

}// DecodeHardwareConfigPacket



/*! Decodes an incoming software version data packet from an IMU and stores
 *  the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeSoftwareVersionPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;
	UInt8 temp;

	pData->MajorVersion = pPkt->data[i++];
	pData->MinorVersion = pPkt->data[i++];
	pData->SubVersion = pPkt->data[i++];

	temp = pPkt->data[i++];
	pData->PatchNumber = (temp >> 1) & 0x3F;
	pData->Released = (temp & 0x1);
	pData->EnhancedProcessor = (temp >> 7);

	pData->VersionMonth = pPkt->data[i++];
	pData->VersionDay = pPkt->data[i++];
	pData->VersionYear = DataToUInt16(&pPkt->data[i]); i += 2;

}// DecodeHardwareConfigPacket


/*! Decodes an incoming manufacture and calibration date packet from an IMU
 *  and stores the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeDatesPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;

	pData->BuildMonth = pPkt->data[i++];
	pData->BuildDay = pPkt->data[i++];
	pData->BuildYear = DataToUInt16(&pPkt->data[i]); i += 2;

	pData->CalMonth = pPkt->data[i++];
	pData->CalDay = pPkt->data[i++];
	pData->CalYear = DataToUInt16(&pPkt->data[i]); i += 2;

}// FormDatesPacket


/*! Decodes an incoming high-speed raw telemetry data packet from an IMU
 *  and stores the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeHighSpeedRawDataPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;

	// Raw sensor data
	pData->SensorsVolts[GYROX_IDX]  = DataToUInt16(&pPkt->data[i]) * AD16_TO_GYROVOLTS; i += 2;
	pData->SensorsVolts[GYROY_IDX]  = DataToUInt16(&pPkt->data[i]) * AD16_TO_GYROVOLTS; i += 2;
	pData->SensorsVolts[GYROZ_IDX]  = DataToUInt16(&pPkt->data[i]) * AD16_TO_GYROVOLTS; i += 2;
	pData->SensorsVolts[ACCELX_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_VOLTS; i += 2;
	pData->SensorsVolts[ACCELY_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_VOLTS; i += 2;
	pData->SensorsVolts[ACCELZ_IDX] = DataToUInt16(&pPkt->data[i]) * AD16_TO_VOLTS; i += 2;

	// Packet sequence number
	pData->SequenceNumber = pPkt->data[i];

}// DecodeHighSpeedRawDataPacket


/*! Decodes an incoming high-speed converted telemetry data packet from an IMU
 *  and stores the data locally.
 *  \param pPkt A pointer to the received packet meant to be decoded.
 *  \param pData The data container in which to store the IMU data. */
void DecodeHighSpeedDataPacket(const IMUPacket_t *pPkt, IMUData_t *pData)
{
	UInt8 i = 0;
	float GyroRes  = (2.0 * pData->GyroRange) / 65535.0;
	float AccelRes = (2.0 * pData->AccelRange * 9.81) / 65535.0;

	// Raw sensor data
	pData->SensorsConverted[GYROX_IDX]  = DataToSInt16(&pPkt->data[i]) * GyroRes;  i += 2;
	pData->SensorsConverted[GYROY_IDX]  = DataToSInt16(&pPkt->data[i]) * GyroRes;  i += 2;
	pData->SensorsConverted[GYROZ_IDX]  = DataToSInt16(&pPkt->data[i]) * GyroRes;  i += 2;
	pData->SensorsConverted[ACCELX_IDX] = DataToSInt16(&pPkt->data[i]) * AccelRes; i += 2;
	pData->SensorsConverted[ACCELY_IDX] = DataToSInt16(&pPkt->data[i]) * AccelRes; i += 2;
	pData->SensorsConverted[ACCELZ_IDX] = DataToSInt16(&pPkt->data[i]) * AccelRes; i += 2;

	// PPS data
	pData->TimeSincePPS = (double)DataToUInt32(&pPkt->data[i]) / 10000.0; i += 4;
	pData->PPSCount = pPkt->data[i++];

	// Packet sequence number
	pData->SequenceNumber = pPkt->data[i];

}// DecodeHighSpeedDataPacket


void FormSettingsPacket(IMUPacket_t *pPkt, const IMUData_t *pData)
{
	UInt8 i = 0;

	pPkt->data[i++] = pData->OutputDevice;
	pPkt->data[i++] = pData->OutputMode;

	i += UInt16ToData(&pPkt->data[i], pData->OversampleRatio);

	i += UInt32ToData(&pPkt->data[i], 1.0e6 / pData->OutputRate);

	pPkt->len = i;
	pPkt->type = SET_SETTINGS_IMU_MSG;

	MakeIMUPacket(pPkt, SET_SETTINGS_IMU_MSG, i);

}// FormSettingsPacket

void FormConfigurationRequestPacket(IMUPacket_t *pPkt, const IMUData_t *pData)
{
	MakeIMUPacket(pPkt, REQ_CONFIG_IMU_MSG, 1);

}// FormConfigurationRequestPacket

void FormCalibrationParameterPacket(IMUPacket_t *pPkt, const IMUData_t *pData,
									UInt8 Number, double Param)
{
	UInt8 i = 0;

	pPkt->data[i++] = Number;

	i += FloatToData(&pPkt->data[i], Param);

	MakeIMUPacket(pPkt, CALPARAM_IMU_MSG, i);

}// FormCalibrationParameterPacket


void FormCalibrationParameterRequestPacket(IMUPacket_t *pPkt, UInt8 Number)
{
	UInt8 i = 0;

	pPkt->data[i++] = Number;

	MakeIMUPacket(pPkt, REQ_CALPARAM_IMU_MSG, i);

}// FormCalibrationParameterRequestPacket


void MakeIMUPacket(IMUPacket_t *pPkt, UInt8 Type, UInt8 Len)
{
	pPkt->sync0 = SYNC_BYTE0;
	pPkt->sync1 = SYNC_BYTE1;
	pPkt->type  = Type;
	pPkt->len   = Len;

	UInt16ToData(&pPkt->data[Len], CRC16((UInt8 *)pPkt, Len + 4));

}// MakeIMUPacket