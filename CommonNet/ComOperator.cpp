
#include <stdio.h>
#include <tchar.h>
#include "ComOperator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComOperator *lpComOperator = NULL;				// Global com operator object
BOOL bHasTaskReturn = FALSE;					// Whether the received operation is finished
BOOL ProcCommReceiveBlock(LPSTR lpBlock, INT nLength );
BOOL CheckReceiveFinished(void);
CCommBuff m_oReceiveBuff;						// The received content
double lfLastReceiveTime = 0.0;					// The last receive time

#ifndef _DISABLE_LOG_
CLogUtil g_Log(_T("ComOperator.Log"));
#endif

void Log(LPCTSTR pMsg, INT nMessageID = 0)
{
#ifndef _DISABLE_LOG_
	g_Log.Log(pMsg, nMessageID);
#endif
}

DWORD FAR PASCAL CommWatchProc( LPSTR lpData )
{
	DWORD		dwEvtMask ;
	NPTTYINFO	npTTYInfo = (NPTTYINFO) lpData ;
	OVERLAPPED	os ;
	INT			nLength ;
	BYTE		abIn[ MAXBLOCK + 1] ;

	memset( &os, 0, sizeof( OVERLAPPED ) ) ;

	os.hEvent = CreateEvent(NULL,
							TRUE,
							FALSE,
							NULL ) ;
	if (os.hEvent == NULL)
	{
		Log(_T("Create event failed in thread"), GetLastError());
		MessageBox( NULL, _T("Create event failed in thread!"), _T("Com operation"), MB_ICONEXCLAMATION | MB_OK ) ;
		return FALSE;
	}

	// We will monitor the event with which a character was received and placed in the input buffer
	if (!SetCommMask( npTTYInfo->idComDev, EV_RXCHAR ))
		return FALSE;

	// Check the connection to the special com port continuously untill the connection is closed
	while ( npTTYInfo->bConnected )
	{
		dwEvtMask = 0 ;

		// Waiting for the coming of the data from the special com port
		WaitCommEvent( npTTYInfo->idComDev, &dwEvtMask, NULL );
		
		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			// Read the data continuously untill the data buffer is empty if there are some char in the data buffer
			do
			{
				if (nLength = lpComOperator->ReadCommBlock( (LPSTR) abIn, MAXBLOCK ))
				{
					*(abIn + nLength) = 0x00;					
					if( !( (nLength == 1) && (*abIn == 0x00) ) )
						// We will do nothing if the data is only 0x00
						// Process the received data
						ProcCommReceiveBlock( (LPSTR) abIn, nLength ) ;
				}
			}
			while ( nLength > 0 ) ;
		}
	}

	// clear the event
	CloseHandle( os.hEvent ) ;

	// clear the thread mark
	npTTYInfo->dwThreadID = 0 ;
	npTTYInfo->hWatchThread = NULL ;

	return( TRUE ) ;

} // end of CommWatchProc()

CComOperator::CComOperator()
{
	lpComOperator = this;			
	m_nCommPort = 1;						// Default port is COM1
	m_dwBaudRate = CBR_19200;				// Default baud rate is 9600 bits/s
	m_bAutoReceiveDataUsingThread = TRUE;	// Receive data automatically
}

CComOperator::~CComOperator()
{

}

BOOL CComOperator::InitTTYInfo()
{

	m_TTYInfo.bPort = m_nCommPort;
	m_TTYInfo.bConnected = FALSE;

	if (m_bAutoReceiveDataUsingThread) {	
		if ( NULL == (m_TTYInfo.osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) )
		{
			Log(_T("InitTTYInfo: CraeteEvent failed"), GetLastError());
			return FALSE;
		}
		
		if ( NULL == (m_TTYInfo.osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) )
		{
			Log(_T("InitTTYInfo: CraeteEvent failed"), GetLastError());
			CloseHandle( m_TTYInfo.osRead.hEvent ) ;
			return FALSE;
		}
	}
	else {
		// We will not create the event if do not receive data automatically
		m_TTYInfo.osRead.hEvent = NULL;
		m_TTYInfo.osWrite.hEvent = NULL;
	}

	memset(&(m_TTYInfo.dcb), 0, sizeof(DCB));
	m_TTYInfo.dcb.DCBlength = sizeof(DCB);
	m_TTYInfo.dcb.BaudRate = m_dwBaudRate;

	return TRUE ;
}

BOOL CComOperator::ResetTTYInfo()
{
	if (m_TTYInfo.bConnected)
		CloseConnection() ;

	// Clear the event object
	if (m_TTYInfo.osRead.hEvent)
		CloseHandle( m_TTYInfo.osRead.hEvent ) ;

	if (m_TTYInfo.osWrite.hEvent)
		CloseHandle( m_TTYInfo.osWrite.hEvent ) ;

	if (m_TTYInfo.hPostEvent)
		CloseHandle( m_TTYInfo.hPostEvent ) ;

	memset(&(m_TTYInfo.dcb), 0, sizeof(DCB));
	m_TTYInfo.dcb.DCBlength = sizeof(DCB);

	return TRUE;
}

BOOL CComOperator::SetComPort(INT nPort)
{
	if(nPort < 0 || nPort > 256) {
		Log(_T("The com port number is invalid"));
		return FALSE;
	}
	else
		m_nCommPort = nPort;

	return TRUE;
}

BOOL CComOperator::SetBaudRate(DWORD dwBaudRate)
{
	if(dwBaudRate >= CBR_110 && dwBaudRate <= CBR_256000)
	{
		m_dwBaudRate = dwBaudRate;
		return TRUE;
	}
	else {
		Log(_T("The baud rate is invalid"));
		return FALSE;
	}
} // end of SetBaudRate

BOOL CComOperator::OpenConnection()
{
	TCHAR	szPort[15];
	BOOL	bRetVal ;

	HANDLE	hCommWatchThread ;
	DWORD	dwThreadID ;
	COMMTIMEOUTS	CommTimeOuts ;

	InitTTYInfo();

	// Set the com port
	_stprintf(szPort, _T("COM%d"), m_TTYInfo.bPort) ;

	// Open the special com port
	if ( INVALID_HANDLE_VALUE == (m_TTYInfo.idComDev = CreateFile( szPort, 
																   GENERIC_READ | GENERIC_WRITE,
																   0,
																   NULL,
																   OPEN_EXISTING,
																   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// overlapped I/O
																   NULL )) )
	{
		return FALSE;
	}

	// get any early notifications
	SetCommMask( m_TTYInfo.idComDev, EV_RXCHAR ) ;
	
	// set the buffer：4096 bytes
	SetupComm( m_TTYInfo.idComDev, 4096, 4096 ) ;
	
	// clear the original content in the buffer 
	PurgeComm( m_TTYInfo.idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
	
	// setup the overlapped I/O property
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
	// CBR_9600 is approximately 1byte/ms. For our purposes, allow
	// double the expected time per character for a fudge factor.
	CommTimeOuts.WriteTotalTimeoutMultiplier = 2 * CBR_9600 / m_TTYInfo.dcb.BaudRate ;
	CommTimeOuts.WriteTotalTimeoutConstant = 0 ;
	SetCommTimeouts( m_TTYInfo.idComDev, &CommTimeOuts ) ;
	
	// set the com port parameters
	bRetVal = SetupConnection();

	if (bRetVal)
	{
		m_TTYInfo.bConnected = TRUE ;

		if (m_bAutoReceiveDataUsingThread) 
		{
			// Create a secondary thread
			// to watch for an event.
			
			if (NULL == (hCommWatchThread = CreateThread( (LPSECURITY_ATTRIBUTES) NULL,
				0,
				(LPTHREAD_START_ROUTINE) CommWatchProc,
				(LPVOID)&m_TTYInfo,
				0, 
				&dwThreadID )) )
			{
				m_TTYInfo.bConnected = FALSE ;
				CloseHandle( m_TTYInfo.idComDev ) ;
				bRetVal = FALSE ;
			}
			else
			{
				m_TTYInfo.dwThreadID = dwThreadID ;
				m_TTYInfo.hWatchThread = hCommWatchThread ;
				
				// assert DTR
				EscapeCommFunction( m_TTYInfo.idComDev, SETDTR ) ;
			}
		}
		else {
			m_TTYInfo.dwThreadID = 0;
			m_TTYInfo.hWatchThread = 0;
		}
	}
	else
	{
		m_TTYInfo.bConnected = FALSE ;
		CloseHandle( m_TTYInfo.idComDev ) ;
	}

	return bRetVal;
} 

BOOL CComOperator::SetupConnection()
{
	BOOL       bRetVal ;
	DCB        dcb ;

	dcb.DCBlength = sizeof( DCB ) ;

	GetCommState( m_TTYInfo.idComDev, &dcb ) ;

	dcb.BaudRate = m_dwBaudRate;
		
	if (!PreSetupConnection(dcb))
		return FALSE;

	memcpy(&(m_TTYInfo.dcb), &dcb, sizeof(DCB));

 /*
	// setup software flow control
	BYTE       bSet ;
	bSet = (BYTE) ((FLOWCTRL( npTTYInfo ) & FC_XONXOFF) != 0) ;

	dcb.fInX = dcb.fOutX = bSet ;
	dcb.XonChar = ASCII_XON ;
	dcb.XoffChar = ASCII_XOFF ;
	dcb.XonLim = 100 ;
	dcb.XoffLim = 100 ;
*/

	bRetVal = SetCommState( m_TTYInfo.idComDev, &dcb ) ;

	return bRetVal;

} // end of SetupConnection()

BOOL CComOperator::CloseConnection()
{
	// set connected flag to FALSE
	m_TTYInfo.bConnected = FALSE ;

	// disable event notification and wait for thread to halt
	SetCommMask( m_TTYInfo.idComDev, 0 ) ;

	// block until thread has been halted
//	while(THREADID(npTTYInfo) != 0)
//		Sleep(100);

	INT i=0;
	while(m_TTYInfo.dwThreadID != 0 && i<20)		// Waiting for the end of the thread within 2 seconds
	{
		i++;
		Sleep(100);
	}

	// drop DTR
	EscapeCommFunction( m_TTYInfo.idComDev, CLRDTR ) ;

	// purge any outstanding reads/writes and close device handle
	PurgeComm( m_TTYInfo.idComDev, PURGE_TXABORT | PURGE_RXABORT |
								   PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
	CloseHandle( m_TTYInfo.idComDev ) ;

	return ( TRUE ) ;
} // end of CloseConnection()

INT CComOperator::ReadCommBlock( LPSTR lpszBlock, INT nMaxLength )
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength;

	// only try to read number of bytes in queue
	ClearCommError( m_TTYInfo.idComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

	if (dwLength > 0)
	{
		fReadStat = ReadFile( m_TTYInfo.idComDev, lpszBlock,
							dwLength, &dwLength, &(m_TTYInfo.osRead) ) ;
		if (!fReadStat)  
		{
			if (GetLastError() == ERROR_IO_PENDING) 
			{
				OutputDebugString("\n\rIO Pending");
				// We have to wait for read to complete.
				// This function will timeout according to the
				// CommTimeOuts.ReadTotalTimeoutConstant variable
				// Every time it times out, check for port errors
				
				if (!GetOverlappedResult( m_TTYInfo.idComDev,
										  &(m_TTYInfo.osRead), 
										  &dwLength, 
										  TRUE )) 
				{
					// an error occurred, write error message to log
					Log(_T("Error read data from com port"), GetLastError());
				}	
			}			
			else
			{
				// some other error occurred
				dwLength = 0 ;
				ClearCommError(m_TTYInfo.idComDev, &dwErrorFlags, &ComStat ) ;
			}
		}
	}

	return ( dwLength ) ;

} // end of ReadCommBlock()

BOOL CComOperator::WriteCommBlock( LPSTR lpByte , INT dwBytesToWrite)
{
	BOOL	fWriteStat ;
	DWORD	dwBytesWritten ;
	DWORD	dwErrorFlags;
	DWORD	dwBytesSent=0;
	COMSTAT	ComStat;
	TCHAR	szError[128] ;

	fWriteStat = WriteFile( m_TTYInfo.idComDev, lpByte, dwBytesToWrite,
							&dwBytesWritten, &(m_TTYInfo.osWrite) ) ;

	// Note that normally the code will not execute the following
	// because the driver caches write operations. Small I/O requests
	// (up to several thousand bytes) will normally be accepted
	// immediately and WriteFile will return true even though an
	// overlapped operation was specified

	if (!fWriteStat)
	{
		// We should wait for the completion of the write operation
		// so we know if it worked or not
		
		// This is only one way to do this. It might be beneficial to
		// place the write operation in a separate thread
		// so that blocking on completion will not negatively
		// affect the responsiveness of the UI
		
		// If the write takes too long to complete, this
		// function will timeout according to the
		// CommTimeOuts.WriteTotalTimeoutMultiplier variable.
		// This code logs the timeout but does not retry
		// the write.
		
		if (GetLastError() == ERROR_IO_PENDING) 
		{
			if (!GetOverlappedResult(m_TTYInfo.idComDev,
									 &(m_TTYInfo.osWrite),
									 &dwBytesWritten,
									 TRUE) ) 
			{
				// an error occurred, write error message to log
				Log(_T("Error write data to com port"), GetLastError());
			}
			
			dwBytesSent += dwBytesWritten;
			
			if( dwBytesSent != dwBytesToWrite )
				_stprintf(szError, _T("Probable Write Timeout: Total of %ld bytes sent"), dwBytesSent);				
			else
				_stprintf(szError, _T("%ld bytes written"), dwBytesSent);
		}	
		else
		{
			// some other error occurred
			ClearCommError(m_TTYInfo.idComDev, &dwErrorFlags, &ComStat ) ;
			return FALSE;
		}
	}

	return TRUE;

} // end of WriteCommBlock()

BOOL CComOperator::PreSetupConnection(DCB &dcb)
{
	dcb.fBinary = TRUE ;
	dcb.fParity = TRUE ;

	// TODO: Add some other codes here

	return TRUE;
}

#define MAX_SEND_BYTES		80

BOOL CComOperator::SendToComm(LPCTSTR lpszSendData, INT nSendBytes, INT nWaitSeconds)
{
	BOOL bSendSuccess = TRUE;
	INT i;
	
	INT nSendNum=0, nLastBytes=0;
	char *pTemp = NULL;

	if(lpszSendData == NULL || nSendBytes <= 0 ) {
		Log(_T("SendToComm: invalid parameters"));
		return FALSE;
	}

/* ryan	
	// 打包编码
	long nBagLength = 0;
	char *pBagBuff = BagEncodeMessage(lpszSendData, nSendBytes, &nBagLength);
	if(pBagBuff == NULL)
		return FALSE;

	// 计算发送次数，每次发送MAX_SEND_BYTES
	nSendNum = nBagLength / MAX_SEND_BYTES;
	nLastBytes = nBagLength % MAX_SEND_BYTES;

	// 编码, 循环块数量
	for(i=0; i<nSendNum; i++)
	{
		pTemp = pBagBuff+i*MAX_SEND_BYTES;
		if(!WriteCommBlock(pTemp, MAX_SEND_BYTES))
		{
			bSendSuccess = FALSE;
			break;
		}
	}

	// 最后块编码
	if( bSendSuccess && (nLastBytes > 0) )
	{
		pTemp = pBagBuff+nSendNum*MAX_SEND_BYTES;
		if(!WriteCommBlock(pTemp, nLastBytes))
			bSendSuccess = FALSE;
	}

	delete[] pBagBuff;		// 释放内存
*/	return bSendSuccess;
}

BOOL ProcCommReceiveBlock(LPSTR lpBlock, INT nLength )
{
	// The data will be clear if the duration between the current received and the last is larger than 10s
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	double lfNowTime, lfBlankTime;
	SystemTimeToVariantTime(&systime, &lfNowTime);
	lfBlankTime = (double)10.0 / 24.0 / 3600.0;			// 10s
	if( (lfNowTime - lfLastReceiveTime) > lfBlankTime )
		m_oReceiveBuff.ClearAllChain ();

	// Modify the last received time
	lfLastReceiveTime = lfNowTime;

	// Add the data to link
	m_oReceiveBuff.AddToChain (lpBlock, nLength);

	CheckReceiveFinished();

	return TRUE;
} // end of ProcCommReceiveBlock()

BOOL CheckReceiveFinished(void)
{
	long nAllReceivedBuffLength = 0;
	char *pAllReceivedBuff = NULL;
	TCHAR szMsg[100];

	// TODO
	// Check the header info
	
	// TODO
	// return FALSE if the data hasn't finished

	//---------------------------------------------
	// The data has arrived completely
	//---------------------------------------------
	nAllReceivedBuffLength = m_oReceiveBuff.GetAllContent (NULL);
	pAllReceivedBuff = new char [nAllReceivedBuffLength + 1];
	if(pAllReceivedBuff == NULL)
	{
		Log(_T("The memory is not enough"));
		return FALSE;
	}
	memset(pAllReceivedBuff, 0x00, nAllReceivedBuffLength + 1);
	nAllReceivedBuffLength = m_oReceiveBuff.GetAllContent (pAllReceivedBuff);
	_stprintf(szMsg, _T("The data has received, data length is %d bytes"), nAllReceivedBuffLength);
	Log(szMsg);

	// TODO
	// analyse the data and do a special process

	m_oReceiveBuff.ClearAllChain ();		// clear all data	
	delete[] pAllReceivedBuff;

	return TRUE;
}

