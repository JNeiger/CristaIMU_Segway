/*! \file
	\brief Serial module for the Win32 world

	Serial_W32.c provides a version of the serial routines which are suitable
	for use	with a windows PC.
*/


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



#include <stdio.h>
#include "serial_PS.h"

#if defined(UNICODE) && defined(CreateFile)
# undef CreateFile
# define CreateFile CreateFileA
#endif

static UInt32 OpenWin32Serial(UInt8 chan, UInt32 baud, UInt8 parity, UInt8 data, UInt32 QSize);

/*! Initialize the serial port sub-system*/
void psInitCOMM(void)
{
}// psInitCOMM

/*! Open the platform specific serial port acording to the passed parameters.
	\param chan is the serial port identifier (COM1, COM2, etc).
	\param dir is the direction to open the port (RX_DIR, TX_DIR, BOTH_DIR).
	\param baud is the signalling rate to open the port in bits per second.
	\param parity is the parity type to use (NO_PARITY, ODD_PARITY, EVEN_PARITY).
	\param data is the number of data bits in a tansfer.
	\param flow is the flow control method used (FLOW_NONE, FLOW_SOFT, FLOW_HARD).
	\param QSize is the size of the serial buffer which will be allocated.
	\return The handle of the opened port, or INVALID_HANDLE_VALUE if failed.*/
UInt32 psOpenCOMM(UInt8 chan, UInt8 dir, UInt32 baud, UInt8 parity,
			    UInt8 data, UInt32 flow, UInt32 QSize)
{
	flow;	// compiler warning pacifier for unreferenced
	dir;	// compiler warning pacifier for unreferenced

	return OpenWin32Serial(chan, baud, parity, data, QSize);

}// psOpenCOMM


/*! Close the requested platform specific serial port
	\param Handle is the serial port handle returned from psOpenCOMM(). */
void psCloseCOMM(UInt32 Handle)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		CloseHandle((HANDLE)Handle);

}// psCloseCOMM


/*! Read a byte of data from a platform specific serial port.  This function
	will not block even if the receive buffer is empty
	\param Handle is the serial port handle returned from psOpenCOMM().
	\return the data from the serial port.  If no data -1 is returned. */
SInt16 psReadByteQuick(UInt32 Handle)
{
	UInt8 Data;
	UInt32 Count;

	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
	{
		ReadFile((HANDLE)Handle, &Data, 1, &Count, NULL);
		if(Count == 1)
			return Data;
	}// If port has been opened

	return -1;
}// psReadByteQuick


/*! Read a block of data from the serial port.  This function does not block.
	If the amount of data waiting to be read is less that the amount requested
	then all the available data is read and the actual amount read is returned.
	\param Handle is the serial port handle returned from psOpenCOMM().
	\param pData points to space to receive the array of bytes
	\param Size is the number of bytes to read.
	\return The actual amount of data read.*/
UInt32 psReadBlockQuick(UInt32 Handle, UInt8* pData, UInt32 Size)
{
	UInt32 Count = 0;

	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		ReadFile((HANDLE)Handle, pData, Size, &Count, NULL);

	return Count;

}// psReadBlockQuick



/*! Write a block of data to a platform specific serial port.  This function
	will not block even if the transmit buffer is full, instead the byte(s)
	will be lost.
	\param Handle is the serial port handle returned from psOpenCOMM().
	\param pData points to a buffer of data.
	\param Size is the number of bytes in the buffer to send.
	\return the number of bytes written.*/
UInt32 psWriteBlockQuick(UInt32 Handle, const UInt8* pData, UInt32 Size)
{
	UInt32 Count = 0;

	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
	{
		if(!WriteFile((HANDLE)Handle, pData, Size, &Count, NULL))
			printf("\nTransmit error %u", GetLastError());

	}// If this port has been opened

	return Count;

}// psWriteBlockQuick


/*! Determine if the platform specific serial channel is open.
	\param Handle is the serial port handle returned from psOpenCOMM().
	\return TRUE if the channel is open, else FALSE.*/
BOOL psIsCOMMOpen(UInt32 Handle)
{
	if(((HANDLE)Handle) == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;

}// psIsCOMMOpen


/*! Change the baud rate of an open platform specific serial port.  The
	channel will be	temporarily disabled while the baud rate is changed.
	This may result	in lost data words.
	\param Handle is the serial port handle returned from psOpenCOMM().
	\param baud	is the new signalling rate of the port in bits per second. */
void psChangeBaud(UInt32 Handle, UInt32 baud)
{
	DCB dcb;

	// Fill dcb structure
	if (GetCommState((HANDLE)Handle, &dcb))
	{
		// update serial comm parameters
		dcb.BaudRate = baud;			// Set baud rate

		// Now use the new parameters
		SetCommState((HANDLE)Handle, &dcb);

	}// If got com state data

}// psChangeBaud


/*! Return the amount of data in the serial receive queue.
	\param Handle is the serial port handle returned from psOpenCOMM().
	\return The amount of data in the receive queue.*/
UInt32 psRxQHolding(UInt32 Handle)
{
	// Warning pacifier
	Handle;

	return 0;
}// psRxQHolding


/*! Return the status of the platform specific carrier detect line
	\param Handle is the serial port handle returned from psOpenCOMM().
	\return TRUE if the carrier detect line is active*/
BOOL psIsCarrierDetectActive(UInt32 Handle)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
	{
		DWORD dwModemStatus;
   		//BOOL  fCTS, fDSR, fRING, fRLSD;

		if (!GetCommModemStatus((HANDLE)Handle, &dwModemStatus))
			return FALSE; // Error in GetCommModemStatus;

		//fCTS = MS_CTS_ON & dwModemStatus;
		//fDSR = MS_DSR_ON & dwModemStatus;
		//fRING = MS_RING_ON & dwModemStatus;

		// Note that the term RLSD (Receive Line Signal Detect) is commonly
		//   referred to as the CD (Carrier Detect) line.
		return MS_RLSD_ON & dwModemStatus;
	}
	else
		return FALSE;

}// psIsCarrierDetectActive


/*! Set the DTR line of the platform specific serial port active
	\param Handle is the serial port handle returned from psOpenCOMM().*/
void psSetDTRActive(UInt32 Handle)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		EscapeCommFunction((HANDLE)Handle, SETDTR);

}// psSetDTRActive


/*! Set the DTR line of the platform specific serial port inactive
	\param Handle is the serial port handle returned from psOpenCOMM().*/
void psSetDTRInactive(UInt32 Handle)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		EscapeCommFunction((HANDLE)Handle, CLRDTR);

}// psSetDTRInactive


/*! Set the output line of the platform specific serial port to break
	\param Handle is the serial port handle returned from psOpenCOMM().*/
void psAssertBreak(UInt32 Handle)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		EscapeCommFunction((HANDLE)Handle, SETBREAK);
}// psAssertBreak


/*! Clear a break condition set by psAssertBreak()
	\param Handle is the serial port handle returned from psOpenCOMM().*/
void psClearBreak(UInt32 Handle)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		EscapeCommFunction((HANDLE)Handle, CLRBREAK);
}// psClearBreak


/*! Remove all data from the serial receive queue
	\param Handle is the serial port handle returned from psOpenCOMM().*/
void psPurgeRxQ(UInt32 Handle)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		PurgeComm((HANDLE)Handle, PURGE_RXABORT|PURGE_RXCLEAR);
}// psPurgeRxQ


/*! Remove all data from the serial transmit queue
	\param Handle is the serial port handle returned from psOpenCOMM().*/
void psPurgeTxQ(UInt32 Handle)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		PurgeComm((HANDLE)Handle, PURGE_TXABORT|PURGE_TXCLEAR);
}// psPurgeTxQ


/*! Open a serial port on a Win32 machine.
	\param chan	is the COM port number of the desired serial port.  Use the
		   defined values given in serial.h.
	\param baud is the baud rate of the port in bits per second.
	\param QSize is the size in bytes of the transmit and receive queue to be
		   used for the interrupt driven serial port.  If this parameter is 0
		   then the serial port will be configured to operate in a polled
		   fashion.  If this parameter is not zero then transmit and receive
		   queues will be dynamically allocated, the serial port will be
		   configured to operate in an interrupt fashion, and the interrupt
		   system must be correctly initialized before calling this function.
	\return The handle to the opened serial port, or -1 if the port was not opened.*/
UInt32 OpenWin32Serial(UInt8 chan, UInt32 baud, UInt8 parity, UInt8 data, UInt32 QSize)
{
	char sPort[20];
	DCB dcb;
	COMMTIMEOUTS CommTimeouts;
	HANDLE Handle;

	// create a string like "\\.\COM1, leave space for \\.\COM256"
	//   Note that we used zero based channel numbers
	#ifdef OLD_COMPILER
	sprintf(sPort, "\\\\.\\COM%d", chan+1);
	#else
	sprintf_s(sPort, 20, "\\\\.\\COM%d", chan+1);
	#endif

	// Open port
	Handle = CreateFile(sPort,
		GENERIC_READ | GENERIC_WRITE,
		0,             // comm devices must be opened w/exclusive-access
		NULL,          // no security attributes
		OPEN_EXISTING, // comm devices must use OPEN_EXISTING
		0,             // not overlapped I/O
		NULL);         // hTemplate must be NULL for comm devices

	if(Handle == INVALID_HANDLE_VALUE)
	{
		return (UInt32)INVALID_HANDLE_VALUE;
	}// if the port failed to open

	// Set buffer size
    if (!SetupComm(Handle, QSize, QSize))
	{
		CloseHandle(Handle);
        return (UInt32)INVALID_HANDLE_VALUE;
	}// failure!, close and get out

	// Configure port, start by filling dcb structure
	if (!GetCommState(Handle, &dcb))
	{
		CloseHandle(Handle);
        return (UInt32)INVALID_HANDLE_VALUE;
	}// failure!, close and get out

	// update serial comm parameters
	dcb.DCBlength = sizeof(DCB);	// Set structure length
	dcb.BaudRate = baud;			// Set baud rate
	dcb.fBinary = TRUE;				// Binary mode
	dcb.fOutxCtsFlow = FALSE;		// No CTS flow control
	dcb.fOutxDsrFlow = FALSE;		// No DSR flow control
	dcb.fDtrControl = DTR_CONTROL_DISABLE;	// No DTR activity
	dcb.fDsrSensitivity = FALSE;	// DSR is don't care
	dcb.fOutX = FALSE;				// No XOff/XOn output flow control
	dcb.fInX = FALSE;				// No XOff/XOn input flow control
	dcb.fNull = FALSE;				// Null characters OK
	dcb.fRtsControl = FALSE;		// No RTS flow control
	dcb.fAbortOnError = FALSE;		// Do not abort on errors
	dcb.ByteSize = (BYTE)data;		// Set number of bits (8)
	dcb.StopBits = ONESTOPBIT;		// Set number of stop bits (1)
	dcb.XoffChar = ~dcb.XonChar;	// Make sure XOn and XOff are different

	switch(parity)	// Setup the parity
	{
	default: dcb.Parity = NOPARITY; break;
	case ODD_PARITY: dcb.Parity = ODDPARITY; break;
	case EVEN_PARITY: dcb.Parity = EVENPARITY; break;
	}

	// Now use the new parameters
	if (!SetCommState(Handle, &dcb))
	{
		CloseHandle(Handle);
        return (UInt32)INVALID_HANDLE_VALUE;
	}// failure!, close and get out

	// Fill comm timeout structure
	if (!GetCommTimeouts(Handle, &CommTimeouts))
	{
		CloseHandle(Handle);
        return (UInt32)INVALID_HANDLE_VALUE;
	}// failure!, close and get out

	// Set timeouts for immediate timeout, i.e. when reading from the port,
	//   only read what is in the buffer, then return without waiting for
	//   more data
    CommTimeouts.ReadIntervalTimeout = MAXDWORD;
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.ReadTotalTimeoutConstant = 0;

	// Write data to buffer immediately, don't wait for data to be moved
	//   out the communications channel
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;

	// use the new comm timeout values
	if (!SetCommTimeouts(Handle, &CommTimeouts))
	{
		CloseHandle(Handle);
        return (UInt32)INVALID_HANDLE_VALUE;
	}// failure!, close and get out

	// Track data for this channel

	return (UInt32)Handle;

}// OpenWin32Serial


/*! Check to see if a string is in the serial port receive buffer.  The string
 *  to look for is given as a NULL-terminated ASCII string.  This function
 *  starts at the oldest data in the receive buffer and moves forward matching
 *  bytes until there are not bytes left in the buffer or the string has been
 *  successfully matched.  If a match occurs the the receive buffer will be
 *  emptied up to and including the string bytes.
 *  \param Handle is the serial port handle returned from psOpenCOMM().
 *  \param pString points to a NULL-terminated string to search for.
 *  \return TRUE if the string is found, else FALSE.*/
BOOL psCheckForResponse(UInt32 Handle, const char *pString)
{
	if(((HANDLE)Handle) != INVALID_HANDLE_VALUE)
		return psCheckForMultipleResponse(Handle, pString, 1);
	else
		return FALSE;

}// CheckForResponse


/*! Check to see if a string appears in the serial port receive buffer multiple
 *  times.  The string to look for is given as a NULL-terminated ASCII string.
 *  This function starts at the oldest data in the receive buffer and moves
 *  forward matching bytes until there are not bytes left in the buffer or the
 *  string has been successfully matched the required number of times.  If the
 *  function succeeds teh buffer is emptied up to and including the last
 *  matched string.
 *  \param Handle is the serial port handle returned from psOpenCOMM().
 *  \param pString points to a NULL-terminated string to search for.
 *  \param NumResponse is the number of responses to find.
 *  \return TRUE if the string is found, else FALSE.*/
BOOL psCheckForMultipleResponse(UInt32 Handle, const char *pString, UInt32 NumResponse)
{
	if(((HANDLE)Handle) == INVALID_HANDLE_VALUE)
		return FALSE;

	if(NumResponse > 0)
	{
		// Room for lots of data
		UInt8 Buffer[1024];

		UInt32 Data = psReadBlockQuick(Handle, Buffer, 1024);

		if(Data)
		{
			const char* pResult = (const char*)Buffer;
			UInt32 Num = 0;

			// Make sure we are null terminated
			if(Data >= 1024)
				Buffer[1023] = 0;
			else
				Buffer[Data] = 0;

			while(1)
			{
				// See if the result appears
				pResult = strstr(pResult, pString);

				if(pResult)
				{
					// Count number of reponses
					Num++;

					if(Num == NumResponse)
						return TRUE;
					else
						pResult++; // Go to next possible response

				}// If we found result
				else
					return FALSE;

			}// While still responses to find

		}// If we got data

	}// If we have data to look for

	return FALSE;

}// CheckForMultipleResponse
