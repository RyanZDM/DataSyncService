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

BOOL CMyDB::Connect()
{
	// Note: If we want to connect to database via a ODBC,
	// The ODBC must be a system system DSN because this is a service application runing background.
	SetLogFileName(LOG_FILENAME);

	try
	{
		// Get db parameters
		TString szConnectionStr;
		TString szUserName;
		TString szPassword;
		
		try
		{
			SetDBType(DT_MSSQL);
			SetConnectMode(CM_USING_CONNECT_STR);

			TCHAR szBuf[200];
			DWORD dwLen = sizeof(szBuf) / sizeof(szBuf[0]);
			CProfileOperator po;

			po.GetString(_T("Database"), _T("ConnectionString"), DefaultConnectString, szBuf, dwLen);
			SetConnectStr(szBuf);

			po.GetString(_T("Database"), _T("UserName"), _T("sa"), szBuf, dwLen);
			SetUserName(szBuf);

			po.GetString(_T("Database"), _T("Password"), _T(""), szBuf, dwLen);
			SetUserPassword(szBuf);	// TODO: dencrypt the password
		}
		catch (...) {
		}
		
		return ConnectToDB();
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
