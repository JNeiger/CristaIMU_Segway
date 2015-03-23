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

#include "CRC16.h"

// Table for cached 4-bit CRC-16 lookups
static const UInt16 crctable[16] = {
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400 };

/*! Calculates a 16-bit cyclic redundancy check on blocks of 8-bit data of
	arbitrary length.
	\param pData points to a buffer of data.
	\param len is the size of the buffer in bytes.
	\return The 16-bit crc of the buffer.*/
UInt16 CRC16(const UInt8 *pData, UInt16 len)
{
	UInt16 crc = 0;		// CRC-16 accumulator

	// While there are more bytes to encode
	while (len--)
	{
		// CRC the lower 4 bits
		crc = (crc >> 4) ^ crctable[((crc ^ (*pData & 0xF)) & 0xF)];

		// Now run the upper 4 bits
		crc = (crc >> 4) ^ crctable[((crc ^ (*pData >> 4)) & 0xF)];

		// Move on to the next element
		pData++;
	}

	// Return the cumulative CRC-16 value
	return crc;

}// CRC16


/*! Calculate the new 16-bit cyclic redundancy check that results from adding
	a byte to an existing crc.
	\param byte is the new byte.
	\param crc is the existing crc value.
	\return The new crc value*/
UInt16 CRC16OneByte(UInt8 byte, UInt16 crc)
{
	// CRC the lower 4 bits
	crc = (crc >> 4) ^ crctable[((crc ^ (byte & 0xF)) & 0xF)];

	// Now run the upper 4 bits
	crc = (crc >> 4) ^ crctable[((crc ^ (byte >> 4)) & 0xF)];

	// Now return the updated CRC value
	return crc;

}// CRC16OneByte
