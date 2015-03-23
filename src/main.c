// Example application for demonstrating IMU packet processing.

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

#define RAD_TO_DEG  57.295779513

#include "IMUPacket.h"
#include "CRC16.h"
#include "ImuSerial.h"
#include "Serial_PS.h"
#include <stdio.h>
#include <math.h>

#include "CalcAngle.h"

int main(int argc, char *argv[])
{
	IMUPacket_t Pkt;     // IMU packet storage
	IMUData_t IMU;       // Current IMU state data
	SInt16 Byte;         // Current incoming byte
	BOOL Waiting = TRUE; // Flag to wait for configuration data
	float lastTime = 0;  // PPS time from the last packet (for dT measurement)

	float angle;

	// Open the serial port on COM1
	UInt32 Handle = psOpenCOMM(0, BOTH_DIR, 115200, PARITY_NONE, 8, FLOW_NONE, 1024);

	initKFilter();

	// Loop forever
	while (TRUE)
	{
		// While there are bytes to be read from the serial port
		while ((Byte = psReadByteQuick(Handle)) >= 0)
		{
			// If this byte has completed a packet
			if (LookForIMUPacketInByte((Byte & 0xFF), &Pkt))
			{
				// Decode the data contained in this packet
				DecodeIMUPacket(&Pkt, &IMU);

				// If we're waiting for configuration data
				if (Waiting)
				{
					// If this packet contains the sensor ranges, we're done waiting
					if (Pkt.type == RESOLUTION_IMU_MSG)
					{
						printf("   gx[d/s]   gy[d/s]   gz[d/s] ax[m/s/s] ay[m/s/s] az[m/s/s] dT[ms]\n");
						Waiting = FALSE;
					}
					else // Otherwise, keep asking the IMU for its configuration data
					{
						FormConfigurationRequestPacket(&Pkt, &IMU);
						psWriteBlockQuick(Handle, (UInt8 *)&Pkt, Pkt.len + 6);
					}
				}
				else if (Pkt.type == HS_SERIAL_IMU_MSG) // If high-speed (converted) telemetry
				{
					// Print the data to the screen in tidy columns
					printf("%10.2f:%10.2f:%10.2f:%10.2f:%10.2f:%10.2f",
						IMU.SensorsConverted[GYROX_IDX],
						IMU.SensorsConverted[GYROY_IDX],
						IMU.SensorsConverted[GYROZ_IDX],
						IMU.SensorsConverted[ACCELX_IDX],
						IMU.SensorsConverted[ACCELY_IDX],
						IMU.SensorsConverted[ACCELZ_IDX]);

					angle = getAngle(atan(IMU.SensorsConverted[ACCELY_IDX] / IMU.SensorsConverted[ACCELZ_IDX]) * RAD_TO_DEG, IMU.SensorsConverted[GYROX_IDX], 0.02);

					printf("%10.2f", angle);
					
					// Only print the time delta if we've got a good "last time" reading
					if (lastTime > 0)
						printf("%10.1f\n", IMU.TimeSincePPS - lastTime);
					else
						printf("%10.1f\n", 0);

					// Store the current timestamp as the new previous timestamp
					lastTime = IMU.TimeSincePPS;
				}
			}
		}

		// Give the rest of the system time to do its thing
		Sleep(1);
	}
}
