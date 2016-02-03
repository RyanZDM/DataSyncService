// SysParams.h: interface for the CSysParams class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSPARAMS_H__640CAA53_1956_4B69_9D2A_B65090686F16__INCLUDED_)
#define AFX_SYSPARAMS_H__640CAA53_1956_4B69_9D2A_B65090686F16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
using namespace std;

#include "OPCClient.h"

#define LOCK_WAIT_TIMEOUT		1000			// 1s

#define ERR_SUCCESS				0
#define ERR_LOCK_TIMEOUT		-200

#define DEFAULT_QRY_INTERVAL		60000		// 60 seconds
#define DEFAULT_LOG_FLAG			true	// false
#define DEFAULT_ITEMS_EACHTIME		100

class CSysParams  
{
public:	
	INT GetItemList(vector<LPITEMINFO>& vList);
	BOOL EnableLog(bool bFlag);
	BOOL SetRemoteMachine(LPWSTR pName);
	BOOL SetQueryInterval(long interval);
	BOOL SetOPCServerProgID(LPCWSTR pName);
	INT RefreshSysParams(BOOL bLog = FALSE);
	INT RefreshSysParams(CDBUtil &db, BOOL bLog = FALSE);
	CSysParams();
	virtual ~CSysParams();

	LPCWSTR	GetRemoteMachine() { return m_wszRemoteMachine.c_str(); }
	LPCWSTR GetOPCServerProgID() { return m_wszOPCServerProgID.c_str(); }
	long GetQueryInterval() { return m_lQryInterval; }
	bool IsLogEnabled() { return m_bEnableLog; }

// 消息轮询时间，查询命令发送间隔，记录日志，监控目录名称，
private:
	BOOL GetStringValue(_RecordsetPtr pRS, LONG lIndex, LPTSTR *ppBuf, DWORD &dwLen);
	HANDLE	m_hMutex;
	void	Unlock();
	BOOL	Lock(DWORD dwMilliseconds);
	void	InitMutex();
	long	m_lQryInterval;		// second
	bool	m_bEnableLog;
	wstring	m_wszOPCServerProgID;
	wstring	m_wszRemoteMachine;
};

#endif // !defined(AFX_SYSPARAMS_H__640CAA53_1956_4B69_9D2A_B65090686F16__INCLUDED_)
