// MyDB.cpp: implementation of the CMyDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Constants.h"
#include "ProfileOperator.h"
#include "MyDB.h"
using namespace ATL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyDB::CMyDB()
{
}

CMyDB::~CMyDB()
{

}

BOOL CMyDB::Connect(BOOL bLogComError)
{
	if (IsConnected())
		return TRUE;	

	SetLogFileName(LOG_FILENAME);

	try
	{
		try
		{
			SetDBType(DT_MSSQL);
			SetConnectMode(CM_USING_CONNECT_STR);

			TCHAR szBuf[500];
			DWORD dwLen = sizeof(szBuf) / sizeof(szBuf[0]);
			CProfileOperator po;

			po.GetString(_T("Database"), _T("ConnectionString"), DefaultConnectString, szBuf, dwLen);
			SetConnectStr(szBuf);

			po.GetString(_T("Database"), _T("UserName"), _T("sa"), szBuf, dwLen);
			SetUserName(szBuf);

			po.GetString(_T("Database"), _T("Password"), NULL, szBuf, dwLen);
			SetUserPassword(szBuf);	// TODO: dencrypt the password
		}
		catch (...) {
		}
		
		return ConnectToDB(bLogComError);
	}
	catch (...)
	{
		return FALSE;
	}
}

INT CMyDB::RemoveAllItems()
{
	try
	{
		if (!IsConnected())
			Connect();

		if (IsConnected())
		{
			return Execute(_bstr_t("Delete ItemLatestStatus"));
		}
		else
		{
			return ERR_CANT_CONNECT_DB;
		}
	}
	catch (INT nCode)
	{
		return nCode;
	}
}
