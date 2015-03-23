/*! \file
	\brief Basic intrinsic variable types

	This file is used to define new types that are accessible by all files in
	the system.  Most of these types are familiar contractions that simply
	save typing, and make the code clearer
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
#ifndef _TYPES_H
#define _TYPES_H

// These definitions used for systems where chars are 8bits, shorts are
//   16bits, longs are 32bits, and long long are 64 bits.

// Word size (generally 32 bits)
typedef unsigned long		 Word_t;

typedef char				  SInt8;  //!< 8 bits signed
typedef unsigned char		  UInt8;  //!< 8 bits unsigned
typedef volatile SInt8		 VSInt8;  //!< 8 bits volatile signed
typedef volatile UInt8		 VUInt8;  //!< 8 bits volatile unsigned
typedef const SInt8			 CSInt8;  //!< 8 bits constant signed
typedef const UInt8			 CUInt8;  //!< 8 bits constant unsigned
typedef const VSInt8		CVSInt8;  //!< 8 bits constant volatile signed
typedef const VUInt8		CVUInt8;  //!< 8 bits constant volatile unsigned

typedef short				  SInt16; //!< 16 bits signed
typedef unsigned short		  UInt16; //!< 16 bits unsigned
typedef volatile SInt16		 VSInt16; //!< 16 bits volatile signed
typedef volatile UInt16		 VUInt16; //!< 16 bits volatile unsigned
typedef const SInt16		 CSInt16; //!< 16 bits constant signed
typedef const UInt16		 CUInt16; //!< 16 bits constant unsigned
typedef const VSInt16		CVSInt16; //!< 16 bits constant volatile signed
typedef const VUInt16		CVUInt16; //!< 16 bits constant volatile unsigned

typedef long				 SInt32;  //!< 32 bits signed
typedef unsigned long		 UInt32;  //!< 32 bits unsigned
typedef volatile SInt32		VSInt32;  //!< 32 bits volatile
typedef volatile UInt32		VUInt32;  //!< 32 bits volatile unsigned
typedef const SInt32		CSInt32;  //!< 32 bits constant signed
typedef const UInt32	 	CUInt32;  //!< 32 bits constant unsigned
typedef const VSInt32	   CVSInt32;  //!< 32 bits constant volatile
typedef const VUInt32 	   CVUInt32;  //!< 32 bits constant volatile unsigned

#ifdef WIN32
#include <WTypes.h>
typedef LONGLONG			 SInt64; //!< 64 bits signed
//typedef ULONGLONG			 UInt64; //!< 64 bits unsigned
typedef LONGLONG			 UInt64; //!< 64 bits signed (cause MS is dumb!)
#else
typedef long long			 SInt64; //!< 64 bits signed
typedef unsigned long long	 UInt64; //!< 64 bits unsigned
#endif

typedef volatile SInt64		VSInt64; //!< 64 bits volatile signed
typedef volatile UInt64		VUInt64; //!< 64 bits volatile unsigned
typedef const SInt64		CSInt64; //!< 64 bits constant signed
typedef const UInt64		CUInt64; //!< 64 bits constant unsigned
typedef const VSInt64	   CVSInt64; //!< 64 bits constant volatile signed
typedef const VUInt64	   CVUInt64; //!< 64 bits constant volatile unsigned

#ifndef WIN32
typedef Word_t				   BOOL; //!< 32-bit boolean
#endif	// WIN32

// To accomodate people who don't like capital letters
typedef UInt8 uint8;				 //!< 8 bits unsigned
typedef UInt16 uint16;				 //!< 16 bits unsigned
typedef UInt32 uint32;				 //!< 32 bits unsigned
typedef float float32;				 //!< 32 bit floating point
typedef double float64;				 //!< 64 bit floating point
typedef SInt8 sint8;				 //!< 8 bits signed
typedef SInt16 sint16;				 //!< 16 bits signed
typedef SInt32 sint32;				 //!< 32 bits signed

#ifndef WIN32
typedef UInt32 DWORD;
typedef UInt16 WORD;
typedef UInt8 BYTE;
#endif

// Mask constants
#define BIT0  0x80000000	//!< Bit 0 in 32-bit PowerPC, most significant bit
#define BIT1  0x40000000	//!< Bit 1 in 32-bit PowerPC
#define BIT2  0x20000000	//!< Bit 2 in 32-bit PowerPC
#define BIT3  0x10000000	//!< Bit 3 in 32-bit PowerPC
#define BIT4  0x08000000	//!< Bit 4 in 32-bit PowerPC
#define BIT5  0x04000000	//!< Bit 5 in 32-bit PowerPC
#define BIT6  0x02000000	//!< Bit 6 in 32-bit PowerPC
#define BIT7  0x01000000	//!< Bit 7 in 32-bit PowerPC
#define BIT8  0x00800000	//!< Bit 8 in 32-bit PowerPC
#define BIT9  0x00400000	//!< Bit 9 in 32-bit PowerPC
#define BIT10 0x00200000	//!< Bit 10 in 32-bit PowerPC
#define BIT11 0x00100000	//!< Bit 11 in 32-bit PowerPC
#define BIT12 0x00080000	//!< Bit 12 in 32-bit PowerPC
#define BIT13 0x00040000	//!< Bit 13 in 32-bit PowerPC
#define BIT14 0x00020000	//!< Bit 14 in 32-bit PowerPC
#define BIT15 0x00010000	//!< Bit 15 in 32-bit PowerPC
#define BIT16 0x00008000	//!< Bit 16 in 32-bit PowerPC
#define BIT17 0x00004000	//!< Bit 17 in 32-bit PowerPC
#define BIT18 0x00002000	//!< Bit 18 in 32-bit PowerPC
#define BIT19 0x00001000	//!< Bit 19 in 32-bit PowerPC
#define BIT20 0x00000800	//!< Bit 20 in 32-bit PowerPC
#define BIT21 0x00000400	//!< Bit 21 in 32-bit PowerPC
#define BIT22 0x00000200	//!< Bit 22 in 32-bit PowerPC
#define BIT23 0x00000100	//!< Bit 23 in 32-bit PowerPC
#define BIT24 0x00000080	//!< Bit 24 in 32-bit PowerPC
#define BIT25 0x00000040	//!< Bit 25 in 32-bit PowerPC
#define BIT26 0x00000020	//!< Bit 26 in 32-bit PowerPC
#define BIT27 0x00000010	//!< Bit 27 in 32-bit PowerPC
#define BIT28 0x00000008	//!< Bit 28 in 32-bit PowerPC
#define BIT29 0x00000004	//!< Bit 29 in 32-bit PowerPC
#define BIT30 0x00000002	//!< Bit 30 in 32-bit PowerPC
#define BIT31 0x00000001	//!< Bit 31 in 32-bit PowerPC, least significant bit

#ifndef TRUE
    #define TRUE 1			//!< You don't really need documentation for this... do you?
#endif

#ifndef FALSE
    #define FALSE 0			//!< You don't really need documentation for this... do you?
#endif

#ifndef NULL
    #define NULL ((Word_t)0)//!< You don't really need documentation for this... do you?
#endif

#endif //!_TYPES_H

