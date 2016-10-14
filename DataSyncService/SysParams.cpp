// SysParams.cpp: implementation of the CSysParams class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Constants.h"
#include "SysParams.h"
#include <time.h>
#include "LogUtil.h"
#include "ContainerUtil.h"
#include "MyDB.h"

extern CLogUtil g_Logger;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysParams::CSysParams()
{
	m_hMutex				= NULL;
	m_lQryInterval			= DEFAULT_QRY_INTERVAL;
	m_bEnableLog			= DEFAULT_LOG_FLAG;
	m_bKeepDbConnection		= DEFAULT_KEEP_DB_CONNECT;

	InitMutex();		
}

CSysParams::~CSysParams()
{
	Unlock();
	if (m_hMutex)
		CloseHandle(m_hMutex);

	g_Logger.ForceLog(_T("The mutex for CSysParams has been released."));
}

void CSysParams::InitMutex()
{
	srand( (unsigned)time( NULL ) );
	INT nRandom = rand();
	TCHAR szName[50];
	wsprintf(szName, _T("_DA_SYS_PARAMTERS_%d"), nRandom);
	m_hMutex = CreateMutex(NULL, FALSE, szName);
	if (m_hMutex)
		g_Logger.VLog(_T("[CSysParams::InitMutex] Mutex has been created: %s"), szName);
	else
		g_Logger.ForceLog(_T("[CSysParams::InitMutex] CreateMutex failed. GetLastError=%d"), GetLastError());
}

BOOL CSysParams::Lock(DWORD dwMilliseconds)
{
	DWORD dwWaitResult = WaitForSingleObject(m_hMutex, dwMilliseconds); 
    switch (dwWaitResult) 
    {
        // The thread got mutex ownership.
        case WAIT_OBJECT_0: 
            return TRUE;
			
        //case WAIT_TIMEOUT:	// Cannot get mutex ownership due to time-out.
        //case WAIT_ABANDONED:	// Got ownership of the abandoned mutex object.
			
		default:
			return FALSE;
    }
}

void CSysParams::Unlock()
{
	if (m_hMutex)
		ReleaseMutex(m_hMutex);
}

INT CSysParams::RefreshSysParams(BOOL bLog)
{
	CMyDB db;
	if (!db.Connect())
		return ERR_CANT_CONNECT_DB;

	INT nRet = RefreshSysParams(db, bLog);
	
	db.Disconnect();

	return nRet;
}

INT CSysParams::RefreshSysParams(CDBUtil &db, BOOL bLog)
{
	USES_CONVERSION;

	INT nRet = ERR_UNKONW_ERROR;
			
	if (Lock(LOCK_WAIT_TIMEOUT))
	{	
		try	
		{
			TCHAR buf[500];
			basic_string<TCHAR> szMsg(_T("Refresh system parameters:"));
			_variant_t value;
			
			value = db.GetSingleValue(_T("SELECT Convert(int,Value) FROM GeneralParams WHERE Category='System' AND Name='QueryInterval'"));
			if ( (db.GetLastErrorCode() == ERR_SUCCESS) && (value.vt == VT_I4) )
			{
				SetQueryInterval( value.lVal );
				if (bLog)
				{
					_stprintf_s(buf, sizeof(buf) / sizeof(buf[0]), _T("\n\tQuery internal: %d ms"), m_lQryInterval);
					szMsg += buf;
				}
			}
			else
			{  
				if (bLog)
				{
					_stprintf_s(buf, sizeof(buf) / sizeof(buf[0]), _T("\n\tQuery internal: use default value %d ms"), DEFAULT_QRY_INTERVAL);
					szMsg += buf;
				}
			}

			value = db.GetSingleValue(_T("SELECT Rtrim(Ltrim(Value)) FROM GeneralParams WHERE Category='System' AND Name='OPCServerProgID'"));
			if ( (db.GetLastErrorCode() == ERR_SUCCESS) && (value.vt == VT_BSTR) )
			{
				LPWSTR pTemp = (BSTR)value.pbstrVal;
				m_wszOPCServerProgID = pTemp;
				if (bLog)
				{
					szMsg += _T("\n\tOPC Server ProgID: ");
					szMsg += pTemp;
				}
			}

			value = db.GetSingleValue(_T("SELECT Rtrim(Ltrim(Value)) FROM GeneralParams WHERE Category='System' AND Name='RemoteMachine'"));
			if ( (db.GetLastErrorCode() == ERR_SUCCESS) && (value.vt == VT_BSTR) )
			{
				LPWSTR pTemp = (BSTR)value.pbstrVal;
				SetRemoteMachine(pTemp);
				if (bLog)
				{
					szMsg += _T("\n\tRemote Machine: ");
					szMsg += W2T(pTemp);
				}
			}

			m_bKeepDbConnection = db.GetSingleBoolValue(_T("SELECT Rtrim(Ltrim(Value)) FROM GeneralParams WHERE Category='System' AND Name='KeepDbConnection'"), m_bKeepDbConnection);			
			if (bLog)
			{
				szMsg += _T("\n\tKeep DB Connection: ");
				szMsg += (m_bKeepDbConnection ? _T("True") : _T("False"));
			}
			
			BOOL bFlag = db.GetSingleBoolValue(_T("SELECT Rtrim(Ltrim(Value)) FROM GeneralParams WHERE Category='System' AND Name='EnableLog'"), m_bEnableLog);
			if (bLog)
			{
				szMsg += _T("\n\tEnable log: ");
				szMsg += (bFlag ? _T("True") : _T("False"));
			}

			EnableLog( bFlag );				
						
			if (bLog)
				g_Logger.ForceLog(szMsg.c_str());
		}
		catch (...) { }
		
		Unlock();
		nRet = ERR_SUCCESS;
	}
	else
	{
		nRet = ERR_LOCK_TIMEOUT;
	}

	return nRet;
}

BOOL CSysParams::SetRemoteMachine(LPWSTR pName)
{
	StrTrimW(pName, L" ");
	m_wszRemoteMachine = pName;

	return TRUE;
}

BOOL CSysParams::SetQueryInterval(long interval)
{
	m_lQryInterval = interval;
	return TRUE;
}

BOOL CSysParams::EnableLog(BOOL bFlag)
{
	m_bEnableLog = bFlag;
	g_Logger.SetEnable(bFlag);
	return TRUE;
}

BOOL CSysParams::SetOPCServerProgID(LPCWSTR pName)
{
	m_wszOPCServerProgID = pName;
	return TRUE;
}

INT CSysParams::GetItemList(vector<LPITEMINFO>& vList)
{
 	ClearVector(&vList);

	INT nRet = ERR_UNKONW_ERROR;
	CMyDB db;
	if (!db.Connect())
		return ERR_CANT_CONNECT_DB;
	
	USES_CONVERSION;	// for W2A or A2W
	
	if (Lock(LOCK_WAIT_TIMEOUT))
	{
		try
		{		
			LPCTSTR pSQL = _T("SELECT ItemID, Address, DisplayName, NeedAccumulate, InConverter, OutConverter, DataType From MonitorItem WHERE Status='A'");
			_RecordsetPtr pRS = db.GetRecordset(_variant_t(pSQL));
			if (pRS)
			{
				DWORD dwBufLen;
				while (!pRS->adoEOF)
				{
					LPITEMINFO pItem = new ITEMINFO();
					GetStringValue(pRS, 0, &(pItem->pItemID), dwBufLen);
					GetStringValue(pRS, 1, &(pItem->pAddress), dwBufLen);
					GetStringValue(pRS, 2, &(pItem->pDisplayName), dwBufLen);
					GetStringValue(pRS, 4, &(pItem->pInConverter), dwBufLen);
					GetStringValue(pRS, 5, &(pItem->pOutConverter), dwBufLen);

					_variant_t vRetVal = pRS->Fields->GetItem(_variant_t((long)6))->GetValue();
					if (VT_I4 == vRetVal.vt)
					{
						pItem->vtRequestedDataType = vRetVal.lVal;
					}

					vRetVal = pRS->Fields->GetItem(_variant_t((long)3))->GetValue();
					if (VT_BOOL == vRetVal.vt)
					{
						pItem->bNeedAccumulate = vRetVal.boolVal;
					}

					pItem->chStatus = _T('A');

					vList.push_back(pItem);

					pRS->MoveNext();
				}
				
				pRS->Close();
				pRS.Release();		
			}
			else
			{
				nRet = db.GetLastErrorCode();
				throw nRet;
			}
		}
		catch (...) {
			Unlock();
			nRet;
		}
		
		Unlock();
		nRet = vList.size();
	}
	else
	{
		nRet = ERR_LOCK_TIMEOUT;
	}
	
	db.Disconnect();
	return nRet;
}

BOOL CSysParams::GetStringValue(_RecordsetPtr pRS, LONG lIndex, LPTSTR *ppBuf, DWORD &dwLen)
{
	_variant_t vRetVal = pRS->Fields->GetItem(_variant_t(lIndex))->GetValue();

	if (VT_NULL == vRetVal.vt)
	{
		dwLen = 0;
		*ppBuf = NULL;
		return TRUE;
	}

	if (VT_BSTR != vRetVal.vt)
		return FALSE;
		
	dwLen = 0;
	LPCTSTR itemId = W2CT((BSTR)vRetVal.pbstrVal);
	size_t len = _tcslen(itemId) + 1;
	*ppBuf = new TCHAR[len];
	_tcscpy_s(*ppBuf, len, itemId);
	
	return TRUE;
}