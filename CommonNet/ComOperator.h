#if !defined(AFX_COMOPERATORSERVER_H__8A40A467_A69A_403D_B959_98C2C7063028__INCLUDED_)
#define AFX_COMOPERATORSERVER_H__8A40A467_A69A_403D_B959_98C2C7063028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>
#include "CommBuff.h"

#ifndef _DISABLE_LOG_
#include <LogUtil.h>
#endif

// Constants
#define MAXPORTS		256			// Max number of port
#define MAXBLOCK		80			// Max received bytes every time


// Flow control
#define FC_DTRDSR		0x01
#define FC_RTSCTS		0x02
#define FC_XONXOFF		0x04

// ASCII definition
#define ASCII_BEL		0x07
#define ASCII_BS		0x08
#define ASCII_LF		0x0A
#define ASCII_CR		0x0D
#define ASCII_XON		0x11
#define ASCII_XOFF		0x13

#define MESSAGE_HEAD			"test"

typedef struct tagTTYINFO
{
	HANDLE		idComDev;			// Device handle
	BYTE		bPort;				// Port number
	BOOL		bConnected;			// Whether connected
	DCB			dcb;				// Device control block
	HANDLE		hPostEvent,			// Handle of the stop event
				hWatchThread,		// Handle of the thread which monitored the com port
				hWatchEvent ;		// The monitor event 
	DWORD		dwThreadID ;		// Id of the thread
	OVERLAPPED	osWrite,			// overlap for write
				osRead ;			// overlap for read
} TTYINFO, NEAR *NPTTYINFO ;

/*
extern DWORD    BaudTable[];			// 串口通讯速率数组
extern DWORD    ParityTable[];			// 奇偶校验数组
extern DWORD    StopBitsTable[];		// 停止位数组
*/

// 串口操作类
class CComOperator
{
public:
	CComOperator();
	virtual ~CComOperator();

	BOOL SetComPort(INT nPort);							// (1~256)
	BOOL SetBaudRate(DWORD dwBaudRate);

	BOOL OpenConnection();
	BOOL CloseConnection();
	BOOL PreSetupConnection(DCB &dcb);
	BOOL SendToComm(LPCTSTR lpszSendData, INT nSendBytes, INT nWaitSeconds=4);

	INT  ReadCommBlock( LPSTR lpszBlock, INT nMaxLength );		// Read data from com port
	BOOL WriteCommBlock( LPSTR lpByte , INT dwBytesToWrite);	// Send data to com port

private:
	BOOL m_bAutoReceiveDataUsingThread;						// Whether to receive the data in buffer automatically in a thread
	TTYINFO m_TTYInfo;										// 
	INT m_nCommPort;
	DWORD m_dwBaudRate;
	BOOL SetupConnection();
	BOOL InitTTYInfo();
	BOOL ResetTTYInfo();
};

DWORD FAR PASCAL CommWatchProc( LPSTR lpData );					// Thread monitored at com port

#endif // !defined(AFX_COMOPERATORSERVER_H__8A40A467_A69A_403D_B959_98C2C7063028__INCLUDED_)
