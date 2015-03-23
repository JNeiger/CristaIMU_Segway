///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// PO Box 1500                   //
// 2621 Wasco St.                //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// vaglient@gorge.net (e-mail)   //
// http://www.gorge.net/cloudcap //
///////////////////////////////////

#ifndef DIGICONNECTME
#include <memory.h>
#endif

#include "ByteOrder.h"

// Little endian version of the byte order routines

/*! Convert a string of bytes in network byte order to a unsigned 16-bit
	integer in host byte order.
	\param pData points to a two byte string of data in network byte order
	\return The 16-bit unsigned integer encoded in the string.*/
UInt16 DataToUInt16(const UInt8* pData)
{
	// In the little endian targets we do things the hard way
	return (UInt16)(pData[0] << 8) | (pData[1]);
}//DataToUInt16

/*! Convert a string of bytes in network byte order to a signed 16-bit
	integer in host byte order.
	\param pData points to a two byte string of data in network byte order
	\return The 16-bit signed integer encoded in the string.*/
SInt16 DataToSInt16(const UInt8* pData)
{
	// In the little endian targets we do things the hard way
	return (SInt16)(pData[0] << 8) | (pData[1]);
}//DataToSInt16


/*! Convert a string of bytes in network byte order into an unsigned 24-bit
	integer in host byte order.  Since not all hosts support an intrinsic
	24-bit type the data is returned as 32-bits
	\param pData points to space to receive a three byte string of data in
		   network byte order
	\return Returns a 32-bit integer (high byte always zero) */
UInt32 DataToUInt24(const UInt8* pData)
{
	UInt32 tmp32;

	tmp32=((UInt32)(pData[0]))<<16;
	tmp32|=((UInt32)(pData[1]))<<8;
	tmp32|=((UInt32)(pData[2]));

	return tmp32;
}// DataToUInt24


/*! Convert a string of bytes in network byte order to a unsigned 32-bit
	integer in host byte order.
	\param pData points to a four byte string of data in network byte order
	\return The 32-bit unsigned integer encoded in the string.*/
UInt32 DataToUInt32(const UInt8* pData)
{
	// In the little endian targets we do things the hard way
	return (UInt32)((pData[0] << 24) | (pData[1] << 16) | (pData[2] <<8) | pData[3]);
}//DataToUInt32


/*! Convert a string of bytes in network byte order to a signed 32-bit
	integer in host byte order.
	\param pData points to a four byte string of data in network byte order
	\return The 32-bit signed integer encoded in the string.*/
SInt32 DataToSInt32(const UInt8* pData)
{
	// In the little endian targets we do things the hard way
	return (SInt32)((pData[0] << 24) | (pData[1] << 16) | (pData[2] <<8) | pData[3]);
}//DataToSInt32


/*! Convert a string of bytes in network byte order to a unsigned 64-bit
	integer in host byte order.
	\param pData points to a eight byte string of data in network byte order
	\return The 64-bit unsigned integer encoded in the string.*/
UInt64 DataToUInt64(const UInt8* pData)
{
	UInt64 Return;

 	Return = DataToUInt32(pData);
	Return = Return<<32;		              // MSWord
	Return |= DataToUInt32(&(pData[4]));	// LSWord

	return Return;

}//DataToUInt64


/*! Convert a string of bytes in network byte order to a unsigned 64-bit
	integer in host byte order.
	\param pData points to a eight byte string of data in network byte order
	\return The 64-bit unsigned integer encoded in the string.*/
SInt64 DataToSInt64(const UInt8* pData)
{
	SInt64 Return;

	Return = DataToUInt32(pData);
	Return = Return<<32;		              // MSWord
	Return |= DataToUInt32(&(pData[4]));	// LSWord

	return Return;

}//DataToSInt64


/*! Convert a unsigned 16-bit integer in host byte order into a string of bytes
	in network byte order.
	\param pData points to space to receive a two byte string of data in
		   network byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 UInt16ToData(UInt8* pData, UInt16 Value)
{
	// In the little endian targets we do things the hard way
	pData[0] = (UInt8)(Value>>8);	// MSByte
	pData[1] = (UInt8)(Value);		// LSByte

	return (UInt32)sizeof(Value);

}// UInt16ToData


/*! Convert a signed 16-bit integer in host byte order into a string of bytes
	in network byte order.
	\param pData points to space to receive a two byte string of data in
		   network byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 SInt16ToData(UInt8* pData, SInt16 Value)
{
	// In the little endian targets we do things the hard way
	pData[0] = (UInt8)(Value>>8);	// MSByte
	pData[1] = (UInt8)(Value);		// LSByte

	return (UInt32)sizeof(Value);

}// SInt16ToData


/*! Convert a unsigned 24-bit integer in host byte order into a string of bytes
	in network byte order.
	\param pData points to space to receive a three byte string of data in
		   network byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 UInt24ToData(UInt8* pData, UInt32 Value)
{
	// In the little endian targets we do things the hard way
	pData[0]=(UInt8)((Value>>16)&0xFF);	// MSByte
	pData[1]=(UInt8)((Value>>8)&0xFF);
	pData[2]=(UInt8)(Value & 0xFF);		// LSByte

	return 3;

}// UInt24ToData


/*! Convert a unsigned 32-bit integer in host byte order into a string of bytes
	in network byte order.
	\param pData points to space to receive a four byte string of data in
		   network byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 UInt32ToData(UInt8* pData, UInt32 Value)
{
	// In the little endian targets we do things the hard way
	pData[0] = (UInt8)(Value>>24);	// MSByte
	pData[1] = (UInt8)(Value>>16);
	pData[2] = (UInt8)(Value>>8);
	pData[3] = (UInt8)(Value);		// LSByte

	return (UInt32)sizeof(Value);

}// UInt32ToData


/*! Convert a signed 32-bit integer in host byte order into a string of bytes
	in network byte order.
	\param pData points to space to receive a four byte string of data in
		   network byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 SInt32ToData(UInt8* pData, SInt32 Value)
{
	// In the little endian targets we do things the hard way
	pData[0] = (UInt8)(Value>>24);	// MSByte
	pData[1] = (UInt8)(Value>>16);
	pData[2] = (UInt8)(Value>>8);
	pData[3] = (UInt8)(Value);		// LSByte

	return (UInt32)sizeof(Value);

}// SInt32ToData


/*! Convert a unsigned 64-bit integer in host byte order into a string of bytes
	in network byte order.
	\param pData points to space to receive a eight byte string of data in
		   network byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 UInt64ToData(UInt8* pData, UInt64 Value)
{
	// In the little endian targets we do things the hard way

	// This code is not yet tested
	pData[0] = (UInt8)(Value>>56);	// MSByte
	pData[1] = (UInt8)(Value>>48);
	pData[2] = (UInt8)(Value>>40);
	pData[3] = (UInt8)(Value>>32);
	pData[4] = (UInt8)(Value>>24);
	pData[5] = (UInt8)(Value>>16);
	pData[6] = (UInt8)(Value>>8);
	pData[7] = (UInt8)(Value);		// LSByte

	return (UInt32)sizeof(Value);

}// UInt64ToData


/*! Convert a signed 64-bit integer in host byte order into a string of bytes
	in network byte order.
	\param pData points to space to receive a eight byte string of data in
		   network byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 SInt64ToData(UInt8* pData, SInt64 Value)
{
	// In the little endian targets we do things the hard way

	// This code is not yet tested
	pData[0] = (UInt8)(Value>>56);	// MSByte
	pData[1] = (UInt8)(Value>>48);
	pData[2] = (UInt8)(Value>>40);
	pData[3] = (UInt8)(Value>>32);
	pData[4] = (UInt8)(Value>>24);
	pData[5] = (UInt8)(Value>>16);
	pData[6] = (UInt8)(Value>>8);
	pData[7] = (UInt8)(Value);		// LSByte

	return (UInt32)sizeof(Value);

}// SInt64ToData


//Routines for encoding little endian data (needed by Novatel simulator)

/*! Convert a unsigned 16-bit integer in host byte order into a string of bytes
	in little endian byte order.  NOTE: Works on little-endian target only.
	\param pData points to space to receive a two byte string of data in
		   little endian byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 UInt16ToleData(UInt8* pData, UInt16 Value)
{
	// Network byte order is BIG_ENDIAN, so we assume it is faster to perform
	//   a potentially misaligned access than it is to try and do multiple byte
	//   accesses and then assemble the result in a register.
	*((UInt16*)pData) = Value;

	return (UInt32)sizeof(Value);

}// UInt16ToleData

/*! Convert a unsigned 32-bit integer in host byte order into a string of bytes
	in little endian byte order.  NOTE: Works on little-endian target only.
	\param pData points to space to receive a four byte string of data in
		   little endian byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 UInt32ToleData(UInt8* pData, UInt32 Value)
{
	// Network byte order is BIG_ENDIAN, so we assume it is faster to perform
	//   a potentially misaligned access than it is to try and do multiple byte
	//   accesses and then assemble the result in a register.
	*((UInt32*)pData) = Value;

	return (UInt32)sizeof(Value);

}// UInt32ToData

/*! Convert a signed 32-bit integer in host byte order into a string of bytes
	in little endian byte order.  NOTE: Works on little-endian target only.
	\param pData points to space to receive a four byte string of data in
		   little endian byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 SInt32ToleData(UInt8* pData, SInt32 Value)
{
	// Network byte order is BIG_ENDIAN, so we assume it is faster to perform
	//   a potentially misaligned access than it is to try and do multiple byte
	//   accesses and then assemble the result in a register.
	*((SInt32*)pData) = Value;

	return (UInt32)sizeof(Value);

}// SInt32ToData


/*! Convert a unsigned 64-bit integer in host byte order into a string of bytes
	in little endian byte order.  NOTE: Works on little-endian target only.
	\param pData points to space to receive a eight byte string of data in
		   little endian byte order
	\param Value is the integer to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 UInt64ToleData(UInt8* pData, UInt64 Value)
{
	// Network byte order is BIG_ENDIAN, so we assume it is faster to perform
	//   a potentially misaligned access than it is to try and do multiple byte
	//   accesses and then assemble the result in a register.
	*((UInt64*)pData) = Value;

	return (UInt32)sizeof(Value);

}// UInt64ToleData


/*! Convert a string of bytes in little endian order to a unsigned 32-bit
	integer in host byte order.
	\param pData points to a four byte string of data in little endian byte order
	\return The 32-bit unsigned integer encoded in the string.*/
UInt32 leDataToUInt32(const UInt8* pData)
{
	// In this case host and string byte order are the same so just do an
	//   un-aligned memory access
	return *((UInt32*)pData);

}//leDataToUInt32

/*! Convert a string of bytes in little endian order to signed 32-bit
	integer in host byte order.
	\param pData points to a four byte string of data in little endian byte order
	\return The 32-bit signed integer encoded in the string.*/
SInt32 leDataToSInt32(const UInt8* pData)
{
	// In this case host and string byte order are the same so just do an
	//   un-aligned memory access
	return *((SInt32*)pData);

}//leDataToSInt32

/*! Convert a string of bytes in little endian order to unsigned 64-bit
	integer in host byte order.
	\param pData points to a four byte string of data in little endian byte order
	\return The 32-bit signed integer encoded in the string.*/
UInt64 leDataToUInt64(const UInt8* pData)
{
	// In this case host and string byte order are the same so just do an
	//   un-aligned memory access
	return *((UInt64*)pData);

} //leDataToUInt64

