///////////////////////////////////
// This file provided by:        //
// Cloud Cap Technology, Inc.    //
// PO Box 1500                   //
// 2621 Wasco St.                //
// Hood River, OR, 97031         //
// +1-541-387-2120    (voice)    //
// +1-541-387-2030    (fax)      //
// http://www.cloudcaptech.com   //
///////////////////////////////////

#include "ByteOrder.h"


typedef union
{
	UInt32 Temp;
	float fTemp;
}Uf_t;


typedef union
{
	UInt64 Temp;
	double fTemp;
}U_t;


/*! Validate a floating point number in integer representation.  If the number
    is valid return the floating point result.
	\param Data is the 32-bit integer expression of the floating point number.
	\param pInvalid points to space to receive a flag indicating if the data
		   represent a valid floating point number.  If it does not then
		   pInvalid will be written to a 1.
	\return The floating point value represented by Data.  If the number is
			not valid than 1.0 is returned*/
float FloatValidate(UInt32 Data, BOOL *pInvalid)
{
	Uf_t U;

	// Go to host byte order
	U.Temp = Data;

	if(IsValidFloat(U.Temp))
		return U.fTemp;
	else
	{
		*pInvalid = 1;
		return 1.0f;
	}// If float is not valid

}// FloatValidate


/*! Validate a floating point number in integer representation.  If the number
    is valid return the floating point result.
	\param Data is the 64-bit integer expression of the floating point number.
	\param pInvalid points to space to receive a flag indicating if the data
		   represent a valid floating point number.  If it does not then
		   pInvalid will be written to a 1.
	\return The floating point value represented by Data.  If the number is
			not valid than 1.0 is returned*/
double DoubleValidate(UInt64 Data, BOOL *pInvalid)
{
	U_t U;

	// Go to host byte order
	U.Temp = Data;

	if(IsValidDouble(U.Temp))
		return U.fTemp;
	else
	{
		*pInvalid = 1;
		return 1.0;
	}// If float is not valid

}// DoubleValidate


/*! Convert a string of bytes in network byte order to a single precision
	32-bit floating point number in host byte order.
	\param pData points to a four byte string of data in network byte order
	\param pInvalid points to space to receive a flag indicating if the data
		   represent a valid floating point number.  If it does not then
		   pInvalid will be written to a 1.
	\return The floating point value encoded in the string.  If the number is
			not valid than 1.0 is returned*/
float DataToFloat(const UInt8* pData, BOOL *pInvalid)
{
	return FloatValidate(DataToUInt32(pData), pInvalid);
}// DataToFloat


/*! Convert a string of bytes in little endian byte order to a single precision
	32-bit floating point number in host byte order.
	\param pData points to a four byte string of data in network byte order
	\param pInvalid points to space to receive a flag indicating if the data
		   represent a valid floating point number.  If it does not then
		   pInvalid will be written to a 1.
	\return The floating point value encoded in the string.  If the number is
			not valid than 1.0 is returned*/
float leDataToFloat(const UInt8* pData, BOOL *pInvalid)
{
	return FloatValidate(leDataToUInt32(pData), pInvalid);
}// leDataToFloat


/*! Convert a string of bytes in network byte order to a double precision
	64-bit floating point number in host byte order.  Note that pData must
	point to an aligned address.  If it does not use the function
	DataToDoubleMisaligned().
	\param pData points to a four byte string of data in network byte order
	\param pInvalid points to space to receive a flag indicating if the data
		   represent a valid floating point number.  If it does not then
		   pInvalid will be written to a 1.
	\return The floating point value encoded in the string.*/
double DataToDouble(const UInt8* pData, BOOL* pInvalid)
{
	return DoubleValidate(DataToUInt64(pData), pInvalid);
}// DataToDouble


/*! Convert a string of bytes in little endian byte order to a double precision
	64-bit floating point number in host byte order.  Note that pData must
	point to an aligned address.  If it does not use the function
	DataToDoubleMisaligned().
	\param pData points to a four byte string of data in network byte order
	\param pInvalid points to space to receive a flag indicating if the data
		   represent a valid floating point number.  If it does not then
		   pInvalid will be written to a 1.
	\return The floating point value encoded in the string.*/
double leDataToDouble(const UInt8* pData, BOOL* pInvalid)
{
	return DoubleValidate(leDataToUInt64(pData), pInvalid);
}// leDataToDouble


/*! Convert a 32-bit single precision floating point number in host byte order
	into a string of bytes in network byte order.  This function can be
	used even if pData points to an unaligned address.
	\param pData points to space to receive a four byte string of data in
		   network byte order
	\param Value is the floating point data to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 FloatToData(UInt8* pData, float Value)
{
	Uf_t U;

	// Convert to integer representation
	U.fTemp = Value;

	return UInt32ToData(pData, U.Temp);	// Go to host byte order

}// FloatToData


/*! Convert a 32-bit single precision floating point number in host byte order
	into a string of bytes in little endian byte order.  This function can be
	used even if pData points to an unaligned address.  NOTE: Works on little-endian target only.
	\param pData points to space to receive a four byte string of data in
		   network byte order
	\param Value is the floating point data to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 FloatToleData(UInt8* pData, float Value)
{
	Uf_t U;

	// Convert to integer representation
	U.fTemp = Value;

	return UInt32ToleData(pData, U.Temp);	// Go to host byte order

}// FloatToleData


/*! Convert a 64-bit single precision floating point number in host byte order
	into a string of bytes in network byte order.  This function can be
	used even if pData points to an unaligned address.
	\param pData points to space to receive a eight byte string of data in
		   network byte order
	\param Value is the floating point data to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 DoubleToData(UInt8* pData, double Value)
{
	U_t U;

	// Convert to integer representation
	U.fTemp = Value;

	return UInt64ToData(pData, U.Temp);	// Go to host byte order

}// DoubleToData


/*! Convert a 64-bit single precision floating point number in host byte order
	into a string of bytes in little endian byte order.  This function can be
	used even if pData points to an unaligned address.  NOTE: Works on little-endian target only.
	\param pData points to space to receive a eight byte string of data in
		   network byte order
	\param Value is the floating point data to be encoded.
	\return Number of bytes required to encode Value.*/
UInt32 DoubleToleData(UInt8* pData, double Value)
{
	U_t U;

	// Convert to integer representation
	U.fTemp = Value;

	return UInt64ToleData(pData, U.Temp);	// Go to host byte order

}// DoubleToleData


/*! Check the passed 32-bit field to determine if it represents a valid single
	precision floating point number.  There are four cases, infinity, NaN,
	Denormalized, and normalized.  Only the last case is valid
	\param FloatCandidate is the 32-bit single precision floating point candidate.
	\return TRUE if the floating point candidate represents a valid number.*/
BOOL IsValidFloat(UInt32 FloatCandidate)
{
	UInt32 Mantissa;
	UInt32 Exponent;

	// Mantissa occupies the least significant 23 bits
	Mantissa = (UInt32)(FloatCandidate & 0x007FFFFF);

	// Exponent occupies the next 8 bits
	Exponent = (UInt32)((0x7F800000 & FloatCandidate) >> 23);

	// Floating point numbers have four states:
	// 1) Inifinity
	// 2) Nan
	// 3) De-normalized
	// 4) Normalized.
	// For our purposes only state 4 is valid.  We can determine if we are in
	//   this state by looking at the exponent and the mantissa.  We don't need
	//   to worry about sign bit for this test

	if(Exponent == 0xFF)	// NaN or Infinity
		return FALSE;
	else if(Exponent == 0)
	{
		if(Mantissa)
			return FALSE;	// De-normalized number
		else
			return TRUE;	// Zero
	}// If exponent is zero

	return TRUE;

}// IsValidFloat


/*! Check the passed 64-bit field to determine if it represents a valid double
	precision floating point number.  There are four cases, infinity, NaN,
	Denormalized, and normalized.  Only the last case is valid
	\param DoubleCandidate is the 64-bit double precision floating point candidate.
	\return TRUE if the floating point candidate represents a valid number.*/
BOOL IsValidDouble(UInt64 DoubleCandidate)
{
	UInt64 Mantissa;
	UInt32 Exponent;

	// Mantissa occupies the least significant 52 bits
	#ifdef WIN32
	Mantissa = (UInt64)(DoubleCandidate & 0x000FFFFFFFFFFFFF);
	#else
	Mantissa = (UInt64)(DoubleCandidate & 0x000FFFFFFFFFFFFFULL);
	#endif

	// Exponent occupies the next 11 bits
	#ifdef WIN32
	Exponent = (UInt32)((0x7FF0000000000000 & DoubleCandidate) >> 52);
	#else
	Exponent = (UInt32)((0x7FF0000000000000ULL & DoubleCandidate) >> 52);
	#endif

	// Floating point numbers have four states:
	// 1) Inifinity
	// 2) Nan
	// 3) De-normalized
	// 4) Normalized.
	// For our purposes only state 4 is valid.  We can determine if we are in
	//   this state by looking at the exponent and the mantissa.  We don't need
	//   to worry about sign bit for this test

	if(Exponent == 0x7FF)	// NaN or Infinity
		return FALSE;
	else if(Exponent == 0)
	{
		if(Mantissa)
			return FALSE;	// De-normalized number
		else
			return TRUE;	// Zero
	}// If exponent is zero

	return TRUE;
}// IsValidDouble


#ifdef __MWERKS__

/*! Convert a floating point value to integer using the round to nearest method.
 *  this is actually a hardware implementation that uses the fctiw instruction
 *  of the PowerPC.  Round to nearest is accomplished because the FPSCR is
 *  setup that way in InitUSIU().*/
asm SInt32 RoundToInteger(register double Value)
{
	nofralloc				// no stack frame
	fctiw    fp0,Value		// convert data in register to int, round to nearest, place result in volatile f0
	stwu     rsp,-16(rsp)	// move the stack pointer 16 bytes down.  We only do this in case an interrupt fires
	stfd     fp0,8(rsp)		// store the double precision f0 on the stack space
	lwz      r3,12(rsp)		// read bottom 4 bytes of the store into return register three
	addi     rsp,rsp,16		// restore the stack pointer
	blr						// branch back to caller
}// RoundToInteger

#else

/*! Return a correctly rounded integer result, instead of a truncated result
	\param Value is the floating point value to be rounded to an integer.
		   This number must fit within a signed 32-bit integer.
	\return The correctly rounded integer.*/
SInt32 RoundToInteger(double Value)
{
	// The code below assumes the default casting behavior is to round
	//   toward zero, truncating the fractional part
	if (Value >= 0)
		return (SInt32)(Value + 0.5);
	else
		return (SInt32)(Value - 0.5);

}// RoundToInteger

#endif


/*! Return a correctly rounded integer result, instead of a truncated result
 *  the result returned is limited to the bounds of a signed 32-bit integer
 *  (-2147483648 to 2147483647)
 *  \param Value is the floating point value to be rounded to an integer.
 *  \return The correctly rounded integer.*/
SInt32 RoundToSInt32(double Value)
{
	if(Value < -2147483647.5)
		return 0x80000000;
	else if(Value > 2147483646.5)
		return 0x7FFFFFFF;
	else
		return RoundToInteger(Value);
}// RoundToSInt32


/*! Return a correctly rounded integer result, instead of a truncated result
 *  the result returned is limited to the bounds of a signed 16-bit integer
 *  (-32768 to 32767)
 *  \param Value is the floating point value to be rounded to an integer.
 *  \return The correctly rounded integer.*/
SInt16 RoundToSInt16(double Value)
{
	if(Value < -32767.5)
		return -32768;
	else if(Value > 32766.5)
		return 32767;
	else
		return (SInt16)RoundToInteger(Value);
}// RoundToSInt16


/*! Return a correctly rounded integer result, instead of a truncated result
 *  the result returned is limited to the bounds of a signed 8-bit integer
 *  (-128 to 127)
 *  \param Value is the floating point value to be rounded to an integer.
 *  \return The correctly rounded integer.*/
SInt8 RoundToSInt8(double Value)
{
	if(Value < -127.5)
		return -128;
	else if(Value > 126.5)
		return 127;
	else
		return (SInt8)RoundToInteger(Value);
}// RoundToSInt8


/*! Return a correctly rounded integer result, instead of a truncated result
 *  the result returned is limited to the bounds of a unsigned 32-bit integer
 *  (0 to 4294967295)
 *  \param Value is the floating point value to be rounded to an integer.
 *  \return The correctly rounded integer.*/
UInt32 RoundToUInt32(double Value)
{
	if(Value < 0.5)
		return 0;
	else if(Value > 4294967294.5)
		return 0xFFFFFFFF;
	else
		return (UInt32)(Value + 0.5);	// Can't do hardware cast from float to int if more than 2^31

}// RoundToUInt32


/*! Return a correctly rounded integer result, instead of a truncated result
 *  the result returned is limited to the bounds of a unsigned 24-bit integer
 *  (0 to 16777215)
 *  \param Value is the floating point value to be rounded to an integer.
 *  \return The correctly rounded integer.*/
UInt32 RoundToUInt24(double Value)
{
	if(Value < 0.5)
		return 0;
	else if(Value > 16777214.5)
		return 16777215;
	else
		return (UInt32)RoundToInteger(Value);
}// RoundToUInt24


/*! Return a correctly rounded integer result, instead of a truncated result
 *  the result returned is limited to the bounds of a unsigned 16-bit integer
 *  (0 to 65535)
 *  \param Value is the floating point value to be rounded to an integer.
 *  \return The correctly rounded integer.*/
UInt16 RoundToUInt16(double Value)
{
	if(Value < 0.5)
		return 0;
	else if(Value > 65534.5)
		return 65535;
	else
		return (UInt16)RoundToInteger(Value);
}// RoundToUInt16


/*! Return a correctly rounded integer result, instead of a truncated result
 *  the result returned is limited to the bounds of a unsigned 8-bit integer
 *  (0 to 255)
 *  \param Value is the floating point value to be rounded to an integer.
 *  \return The correctly rounded integer.*/
UInt8  RoundToUInt8(double Value)
{
	if(Value < 0.5)
		return 0;
	else if(Value > 254.5)
		return 255;
	else
		return (UInt8)RoundToInteger(Value);
}// RoundToUInt8



/*! Convert a 32-bit IEEE-754 floating point value to 16-bits.  Do this by
 *  limiting the exponent to 6 bits (from 8) and the mantissa to 9 bits
 *  (from 23).  Underflows will be returned as zeros and overflows will
 *  be returned with the largest 6-bit exponent.
 *  \param Data is the 32-bit floating point data to convert.
 *  \return The 16-bit representation.*/
UInt16 FloatToFloat16(float Data)
{
	Uf_t U;
	UInt32 Sign;
	UInt32 Mantissa;
	UInt32 UExponent;
	SInt32 Exponent;

	// Get the floating point value in integer format
	U.fTemp = Data;

	// Sign is the first bit
	Sign = (U.Temp & 0x80000000);

	// Mantissa occupies the least significant 23 bits
	Mantissa = (UInt32)(U.Temp & 0x007FFFFF);

	// Exponent occupies the next 8 bits
	UExponent = (UInt32)((0x7F800000 & U.Temp) >> 23);

	// If mantissa and exponent are zero means a number of zero
	if((Mantissa == 0) && (UExponent == 0))
		return 0;

	// By definition a normalized mantissa is left-aligned with an implicit
	//   leading 1 (according to IEEE-754).  We are going to drop the right
	//   most 14 bits (we only have a 9 bit mantissa).
	Mantissa = Mantissa >> 14;

	// The exponent is 8 bits and is biased by 127.  I.e. an exponent of 0x7F
	//   represents zero.  0x7E represents -1, 0x80 represents +1.  We remove
	//   this bias
	Exponent = (SInt32)(UExponent - 127);

	// Now we re-bias to fit within our 6-bit definition.  That definition uses
	//   a bias of 31.  Here is where we get into trouble, if we cannot get the
	//   exponent to fit with this range that implies the data cannot be fit.
	//   There are two possibilities: the number is too big, the number is too
	//   small.  If too small occurs, then we just send zero.  If too big occurs
	//   we send the largest possible exponent.
	if(Exponent < -31)
		return 0;		// Exactly zero
	else if(Exponent > 31)
		Exponent = 31;	// Biggest possible number, but not an Inf.

	// Add in the 6-bit bias (0...63, so bias is 31)
	UExponent = (UInt32)(Exponent + 31);

	// Build the number move sign bit from 0 to 16
	U.Temp = Sign >> 16;

	// Add in the exponent, leave 9 bits for the mantissa
	U.Temp |= (UExponent << 9);

	// Finally add in the Mantissa
	U.Temp |= Mantissa;

	// return the 16-bit floating point representation
	return (UInt16)U.Temp;

}// FloatToFloat16



/*! Convert a 16-bit floating point value to 32-bit IEEE-754 floating point
 *  Do this by expanding the exponent to 8 bits (from 6) and the mantissa to
 *  23 bits (from 9)
 *  \param Data is the 16-bit floating point data to convert.
 *  \return The 32-bit IEEE-754 representation.*/
float Float16ToFloat(UInt16 Data)
{
	Uf_t U;
	UInt32 Sign;
	UInt32 UExponent;
	SInt32 Exponent;
	UInt32 Mantissa;

	// Sign bit, move from location 16 to 0
	Sign = (UInt32)((Data & 0x8000)<<16);

	// biased exponent, uses six bits from 1 to 6
	UExponent = (UInt32)((Data & 0x7E00)>>9);

	// Calculate the unbiased exponent
	Exponent = (SInt32)(UExponent - 31);

	// Mantissa, least 9 bits, increase to 23 bits
	Mantissa = (UInt32)((Data & 0x03FF)<<14);

	// Re-bias the exponent, use 127 (8-bit bias)
	UExponent = (UInt32)(Exponent + 127);

	// Shift it up into its correct location
	UExponent = UExponent << 23;

	// Put it all together
	Sign |= UExponent;
	Sign |= Mantissa;

	// Copy to floating point storage
	U.Temp = Sign;

	return U.fTemp;

}// Float16ToFloat



#ifdef FLOAT16_TEST

#include <stdio.h>
#include "univdata.h"
void TestFloat16(void)
{
	int i;
	float DataIn, DataOut;
	printf("\n\n");
	DataIn = PIf;
	for(i = 0; i < 10; i++)
	{
		DataOut = Float16ToFloat(FloatToFloat16(DataIn));
		printf("%0.10f %0.10f\n", DataIn, DataOut);
		DataIn *= 10.0f;
	}

	printf("\n\n");

	DataIn = PIf;
	for(i = 0; i < 10; i++)
	{
		DataOut = Float16ToFloat(FloatToFloat16(DataIn));
		printf("%0.10f %0.10f\n", DataIn, DataOut);
		DataIn /= 10.0f;
	}

	printf("\n\n");
}

#endif