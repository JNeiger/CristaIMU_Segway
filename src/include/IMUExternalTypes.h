#ifndef IMUEXTERNALTYPES_H
#define IMUEXTERNALTYPES_H

#include "Types.h"

enum IMUSensorIndex_t
{
	GYROX_IDX,                             //!< X gyro sensor data array index
	GYROY_IDX,                             //!< Y gyro sensor data array index
	GYROZ_IDX,                             //!< Z gyro sensor data array index
	ACCELX_IDX,                            //!< X accel sensor data array index
	ACCELY_IDX,                            //!< Y accel sensor data array index
	ACCELZ_IDX,                            //!< Z accel sensor data array index
	N_SENSOR_IDX                           //!< Number of sensor data array indices
};

enum IMUSensorTempIndex_t
{
	GYROX_TEMP_IDX,                        //!< X gyro temperature array index
	GYROY_TEMP_IDX,                        //!< Y gyro temperature array index
	GYROZ_TEMP_IDX,                        //!< Z gyro temperature array index
	N_TEMP_IDX,                            //!< Number of temperature array indices
	ACCELX_TEMP_IDX = GYROY_TEMP_IDX,      //!< X accel temperature array index (using Y gyro)
	ACCELY_TEMP_IDX = GYROX_TEMP_IDX,      //!< Y accel temperature array index (using X gyro)
	ACCELZ_TEMP_IDX = GYROY_TEMP_IDX       //!< Z accel temperature array index (using Y gyro)
};

typedef struct
{
	// Sensor data
	float  SensorsVolts[N_SENSOR_IDX];     //!< Sensor analog output raw voltages
	float  SensorsConverted[N_SENSOR_IDX]; //!< Sensor readings in engineering units
	float  GyroTempVolts[N_TEMP_IDX];      //!< Gyro temperature analog output voltages

	// Build and calibration dates
	UInt8  BuildMonth;                     //!< Month of manufacture
	UInt8  BuildDay;                       //!< Day of manufacture
	UInt16 BuildYear;                      //!< Year of manufacture
	UInt8  CalMonth;                       //!< Month of calibration
	UInt8  CalDay;                         //!< Day of calibration
	UInt16 CalYear;                        //!< Year of calibration

	// Hardware configuration
	UInt16 SerialNumber;                   //!< Sensor head serial number
	UInt8  EepromVersion;                  //!< EEPROM layout version number
	UInt8  HwRevMajor;                     //!< Major hardware revision number (x in x.y)
	UInt8  HwRevMinor;                     //!< Minor hardware revision number (y in x.y)
	UInt8  AccelConfig;                    //!< Accelerometer configuration number
	UInt8  GyroConfig;                     //!< Gyro configuration number
	UInt8  ConfigBits;                     //!< System configuration bits

	// Software version information
	UInt8  MajorVersion;                   //!< IMU firmware major version
	UInt8  MinorVersion;                   //!< IMU firmware minor version
	UInt8  SubVersion;                     //!< IMU firmware subversion
	UInt8  PatchNumber;                    //!< IMU firmware patch number
	BOOL   Released;                       //!< IMU firmware release flag
	UInt8  VersionMonth;                   //!< Month of IMU firmware release
	UInt8  VersionDay;                     //!< Day of IMU firmware release
	UInt16 VersionYear;                    //!< Year of IMU firmware release

	// Processor version
	BOOL EnhancedProcessor;                //!< TRUE if the IMU has an enhanced processor, otherwise FALSE

	// Sensor ranges
	float GyroRange;                       //!< Gyro max range in degrees per second
	float AccelRange;                      //!< Accelerometer max range in meters per second squared

	// Data rate numbers
	float  OutputRate;                     //!< Current IMU output rate, in Hz
	UInt16 OversampleRatio;                //!< Number of oversamples that compose one data reading

	// Output data device target flags
	enum IMUOutputDevice_t
	{
		OUTPUT_DEVICE_SERIAL   = 0x01,     //!< Set if packets should be sent via RS-232
		OUTPUT_DEVICE_CAN      = 0x02      //!< Set if packets should be sent via CANbus
	} OutputDevice;

	// Output data mode flags
	enum IMUOutputMode_t
	{
		OUTPUT_MODE_CONVERTED  = 0x01,     //!< Set if engineering data should be sent
		OUTPUT_MODE_RAW        = 0x02,     //!< Set if raw data should be sent
		OUTPUT_MODE_HS_RAW     = 0x04      //!< Set if high speed raw data ONLY should be sent
	} OutputMode;

	// PPS Data
	float  TimeSincePPS;                   //!< Milliseconds since last PPS
	UInt8  PPSCount;                       //!< Number of PPS signals captured
	SInt16 ClockError;                     //!< Difference between PPS and clock seconds

	// Packet sequence number
	UInt8 SequenceNumber;                  //!< Sequence number of the last round of telemetry packets

} IMUData_t;

// Defines to convert A/D counts to volts
#define AD16_TO_VOLTS		(0.0000625)		// Vref / (maximum A/D counts + 1)
#define VOLTS_TO_GYROVOLTS	(1.0 / 0.8085)	// Cancels out the gyro voltage divider
#define AD16_TO_GYROVOLTS	(AD16_TO_VOLTS * VOLTS_TO_GYROVOLTS)

#endif // IMUEXTERNALTYPES_H
