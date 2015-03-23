/*! \file
	\brief Nibble-based CRC-16 computation functions.
*/

///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// PO Box 1500                   //
// No. 8 Fourth St.              //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#ifndef CRC16_H
#define CRC16_H

#include "Types.h"

UInt16 CRC16(const UInt8* pBuf, UInt16 len);
UInt16 CRC16OneByte(UInt8 Byte, UInt16 crc);

#endif // CRC16_H
