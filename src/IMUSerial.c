
#include "CRC16.h"
#include "IMUSerial.h"

static BOOL ValidateReceivedPacket(IMUPacket_t *pPkt);


/*! Processes a byte in a serial data stream looking for an IMU packet.
	\param Byte The current byte in the data stream to process.
	\param pPkt A container to hold the packet's data. This container MUST
	be persistent between calls to this function in order for the state machine
	to function properly.
	\return TRUE if a packet has been found, otherwise FALSE. */
BOOL LookForIMUPacketInByte(UInt8 Byte, IMUPacket_t *pPkt)
{
	switch (pPkt->state)
	{
	default:
		pPkt->state = SERIAL_STATE_SYNC0;

	// Sync byte 0
	case SERIAL_STATE_SYNC0:
		// If the sync byte matches, reset the state machine and increment the state
		if (Byte == SYNC_BYTE0)
		{
			pPkt->sync0 = Byte;
			pPkt->i = 0;
			pPkt->state++;
		}
		break;

	// Sync byte 1
	case SERIAL_STATE_SYNC1:
		// If the second sync byte matches, move on; otherwise reset the state machine
		if (Byte == SYNC_BYTE1)
		{
			pPkt->sync1 = Byte;
			pPkt->state++;
		}
		else
			pPkt->state--;
		break;

	// Message ID
	case SERIAL_STATE_MESSAGE_TYPE:
		// Copy in the message ID and go to the next state
		pPkt->type = Byte;
		pPkt->state++;
		break;

	// Payload length
	case SERIAL_STATE_LEN:
		// Copy in the payload length and go to the next state
		if (pPkt->type != HS_RAW_IMU_MSG)
		{
			pPkt->len = Byte;
			pPkt->state++;
			break;
		}
		// HS_RAW_IMU_MSG has no length byte and is 13 bytes long
		else
			pPkt->len = 13;

	// Message payload
	case SERIAL_STATE_DATA:
		// Get the next payload data byte
		pPkt->data[pPkt->i++] = Byte;

		// If we've got the whole packet, return TRUE if the CRC is right
		if (pPkt->i == pPkt->len + 2)
		{
			pPkt->state = SERIAL_STATE_SYNC0;
			return ValidateReceivedPacket(pPkt);
		}

		break;
	}

	return FALSE;

}// LookForIMUPacketInByte


/*! Checks the received CRC value of an RS-232 packet against the computed
	value to ensure that the payload data are intact.
	\param pPkt A pointer to the packet to be validated.
	\return TRUE if the CRC values match up, otherwise FALSE. */
static BOOL ValidateReceivedPacket(IMUPacket_t *pPkt)
{
	UInt16 crcPacket, crcCalc = 0;
	UInt8  i;

	// Glue the CRC value MSB and LSB together in a temporary
	crcPacket  = (UInt16)pPkt->data[pPkt->len] << 8;
	crcPacket |= (UInt16)pPkt->data[pPkt->len + 1];

	// Compute the expected CRC value
	if (pPkt->type != HS_RAW_IMU_MSG)
		crcCalc = CRC16((UInt8 *)pPkt, pPkt->len + 4);
	else
	{
		// HS_RAW_IMU_MSG omits the length byte
		crcCalc = CRC16((UInt8 *)pPkt, 3);

		//  Now do the message payload
		for (i = 0; i < 13; i++)
			crcCalc = CRC16OneByte(pPkt->data[i], crcCalc);
	}

	// Finally, return TRUE if the two values match, or FALSE if not
	return (crcPacket == crcCalc);

}// ValidateReceivedPacket