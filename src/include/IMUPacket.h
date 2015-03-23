/*! \file
	\brief Packet processing functions for the IMU.

	The functions in this file allow the user to both (a) process incoming
	messages from the Crista IMU and (b) form outbound messages destined for
	the IMU.
*/

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
#ifndef IMUPACKET_H
#define IMUPACKET_H

#include "IMUExternalTypes.h"

enum IMUMessageTypes
{
	RAWGYRO_IMU_MSG,					//!< Raw gyro data
	RESERVED0_IMU_MSG,					//!< Reserved
	RAWACCEL_IMU_MSG,					//!< Raw accelerometer data
	TIMING_IMU_MSG,						//!< Timing data
	RESOLUTION_IMU_MSG,					//!< Resolution
	RESUNITS_GYRO_IMU_MSG,				//!< Gyro data in resolution units
	RESUNITS_ACCEL_IMU_MSG,				//!< Accelerometer data in resolution units
	SET_SETTINGS_IMU_MSG,				//!< Set settings IMU message
	SETTINGS_IMU_MSG,					//!< Return settings IMU message
	MFRCALDATE_IMU_MSG,					//!< Return manufacture and calibration dates
	SERIALNUMCONFIG_IMU_MSG,			//!< Return serial number and configuration data
	SWVERSION_IMU_MSG,					//!< Set/return software versions message
	BOARDREFERENCE_IMU_MSG,				//!< Processor board reference data (crystal freq and A/D ref)
	REQ_CONFIG_IMU_MSG,					//!< Request all configuration and settings messages
	RESERVED2_IMU_MSG,					//!< Reserved
	RESERVED3_IMU_MSG,					//!< Reserved
	RESERVED4_IMU_MSG,					//!< Reserved
	RESERVED5_IMU_MSG,					//!< Reserved
	RESERVED6_IMU_MSG,					//!< Reserved
	RESERVED7_IMU_MSG,					//!< Reserved
	RESERVED8_IMU_MSG,					//!< Reserved
	REQ_CALPARAM_IMU_MSG,				//!< Request calibration parameter
	CALPARAM_IMU_MSG,					//!< Calibration parameter
	RAWGYROTEMPX_IMU_MSG,				//!< Raw X gyro temp, volts, float32be
	RAWGYROTEMPY_IMU_MSG,				//!< Raw Y gyro temp, volts, float32be
	RAWGYROTEMPZ_IMU_MSG,				//!< Raw Z gyro temp, volts, float32be
	RESERVED9_IMU_MSG,					//!< Reserved
	SENSORHEAD_CRC_STATUS_IMU_MSG,		//!< Return sensor head CRC status (pass/fail).
	RESERVED10_IMU_MSG,					//!< Reserved
	RESERVED11_IMU_MSG,					//!< Reserved
	GYRO_STDEV_IMU_MSG,					//!< Gyro oversample std. deviation message
	ACCEL_STDEV_IMU_MSG,				//!< Accelerometer oversample std. deviation message
	GYRO_MINIMUM_IMU_MSG,				//!< Gyro minimum oversample value message
	ACCEL_MINIMUM_IMU_MSG,				//!< Accelerometer minimum oversample value message
	GYRO_MAXIMUM_IMU_MSG,				//!< Gyro maximum oversample value message
	ACCEL_MAXIMUM_IMU_MSG,				//!< Accelerometer maximum oversample value message

    HS_RAWGYROTEMP_IMU_MSG=0xFD,        //!< High speed raw gyro temperatures packet.
	HS_RAW_IMU_MSG,						//!< High speed raw data serial packet.
	HS_SERIAL_IMU_MSG   				//!< High speed converted data serial packet
};

#define MAX_PAYLOAD_BYTES 18

#define SYNC_BYTE0 0x55
#define SYNC_BYTE1 0xAA

//!< Applicable states for the serial packet parsing state machine
enum SerialPktState_t
{
	SERIAL_STATE_SYNC0,
	SERIAL_STATE_SYNC1,
	SERIAL_STATE_MESSAGE_TYPE,
	SERIAL_STATE_LEN,
	SERIAL_STATE_DATA
};

//!< Structure for holding a CAN and/or serial packet
typedef struct
{
	UInt8 sync0;
	UInt8 sync1;
	UInt8 type;							//!< CAN message ID
	UInt8 len;							//!< Payload length
	UInt8 data[MAX_PAYLOAD_BYTES + 2];	//!< Message payload
	UInt8 state;						//!< Receive state machine status
	UInt8 i;							//!< Payload receive byte index
} IMUPacket_t;

void DecodeIMUPacket(const IMUPacket_t *pPkt, IMUData_t *pData);

// System settings packets
void FormSettingsPacket(IMUPacket_t *pPkt, const IMUData_t *pData);
void FormConfigurationRequestPacket(IMUPacket_t *pPkt, const IMUData_t *pData);

// Calibration parameter packets
void FormCalibrationParameterPacket(IMUPacket_t *pPkt, const IMUData_t *pData,
									UInt8 pNumber, double pParam);
void FormCalibrationParameterRequestPacket(IMUPacket_t *pPkt, UInt8 pNumber);

#endif // IMUPACKET_H






