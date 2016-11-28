// DBUtil.cpp: implementation of the CDBUtil class.
//
//////////////////////////////////////////////////////////////////////

#include <atlbase.h>
#include "DBUtil.h"
#include "shlwapi.h"
#include <math.h>
#include "StrUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

#pragma warning (disable:4800)
#pragma warning( disable : 4290 )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LPCTSTR DB_TEMPLATE[] = {
							_T("%s")																			// 其他
							,_T("driver={Microsoft ODBC for Oracle};SERVER=%s")									// Oracle
							//,"Provider=SQLNCLI.1;Persist Security Info=False;Data Source=%s;Initial Catalog=%s"
							,_T("Driver={SQL Server};Server=%s;Database=%s")									// MS SQL
							,_T("Driver={SYBASE SYSTEM 11};Srvr=%s")											// ASE
							,_T("ODBC; Driver=Sybase SQL Anywhere 5.0;DefaultDir=%s;Dbf=%s;")					// ASA
							,_T("Driver={Microsoft Access Driver (*.mdb)};Dbq=%s")								// Access
							,_T("Driver={Microsoft dBASE Driver (*.dbf)};DriverID=277;Dbq=%s")					// dBase
							,_T("Driver={Microsoft Excel Driver (*.xls)};DriverId=790;DefaultDir=%s;Dbq=%s")	// Excel
							,_T("Driver={mySQL};Server=%s;Option=16834;Database=%s")							// mySQL（local）
							,_T("Driver={Microsoft Text Driver (*.txt; *.csv)};Dbq=%s;Extensions=asc,csv,tab,txt;Persist Security Info=False")	// Text
							,_T("Driver={Microsoft Visual FoxPro Driver};SourceType=DBC;SourceDB=%s.dbc;Exclusive=No;")		// Visual FoxPro
							};	

CDBUtil::CDBUtil(BOOL bStartEventMonitor)
{
	m_bStartEventMonitor= bStartEventMonitor;
	m_bNeedTrans		= FALSE;
	m_bIsInTrans		= FALSE;
	m_bConnected		= FALSE;
	m_pConnection		= NULL;
	m_nLastErrorCode	= 0;
	m_dwDestDBType		= DT_OTHER;
	m_dwConnectMode		= CM_USING_SERVER_NAME;	
	m_szConnStr			= _T("");
	m_szServerName		= _T("");
	m_szDBName			= _T("");
	m_szLastErrorMsg	= _T("");
	m_dwConnEvt			= 0;
	m_pConnectEvent		= NULL;
	m_bExecuting		= FALSE;
	m_bInTransaction	= FALSE;

#ifdef _CALL_CoInitialize_
    CoInitialize(NULL);
#endif

#ifndef _DISABLE_LOG_
	m_Logger.SetLogFileName(_T("DBUtil.log"));
	m_Logger.SetEnable(TRUE);
#endif

	//InitData();	
}

CDBUtil::~CDBUtil()
{	
	try {StopConnectionEvent();} catch (...) {}
	Disconnect();

#ifdef _CALL_CoInitialize_
    CoUninitialize();
#endif	
}

BOOL CDBUtil::InitData()
{
	try
	{
		if (FAILED(m_pConnection.CreateInstance("ADODB.Connection")))
			throw ERR_CANT_CONNECT_DB;

		if (m_bStartEventMonitor)
			StartConnectionEvent();
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::ConnectToDB"));
		//if (m_pConnection != NULL)
			//m_pConnection.Release();

		SetLastErrorCode(ERR_CANT_CONNECT_DB);
		return FALSE;
	}
	catch (...)
	{
		SetLastErrorCode(ERR_CANT_CONNECT_DB);
		return FALSE;
	}

	return TRUE;
}

BOOL CDBUtil::ConnectToDB(BOOL bLogComError)
{
	return ConnectToDB(m_szConnStr.c_str(), m_szUserId.c_str(), m_szUserPwd.c_str(), bLogComError);
}

/**************************************************************************
 * 使用连接字符串,用户名和密码连接数据库
 * BOOL ConnectToDB(LPCTSTR lpszConStr, LPCTSTR lpszUserId, LPCTSTR lpszUserPwd)
 * 参数:
 *    lpszConStr - 数据库连接字符串
 *    lpszUserId - 用户名
 *    lpszUserPwd - 密码
 * 返回值:
 *    TRUE  - 连接成功
 *    FALSE - 连接失败
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: (无)
 * -----------------------------------------------------------------------
 * 注: 如果失败,可查看最后一次错误代码,及错误信息
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
BOOL CDBUtil::ConnectToDB(LPCTSTR pcszConStr, LPCTSTR pcszUserId, LPCTSTR pcszUserPwd, BOOL bLogComError)
{
	// Note: If we want to connect to database via a DSN, it must be a system DSN because 
	// this is a service application runing background. And the 32 bit "Data Source (ODBC)"
	// program is under the directory "C:\Windows\SysWOW64" on Windowx x64 OS

	//检测输入参数***************************************************/

	//***************************************************************/
	Disconnect();

	try
	{
		try
		{
			m_pConnection = _ConnectionPtr("ADODB.Connection");
		}
		catch (_com_error cComErr1) {
			if (((LONG)cComErr1.Error()) == 0x800401f0)
			{
				// It does means that the CoInitialize must be called first.
				Log(_T("Create _ConnectionPtr failed because we need to call the method CoInitialize first.\nWe will call method CoInitialize here and try again."));
				CoInitialize(NULL);
				m_pConnection = _ConnectionPtr("ADODB.Connection");
			}
			else
			{
				throw cComErr1;
			}
		}

		_bstr_t szConStr(pcszConStr);
		_bstr_t szUserId(pcszUserId);
		_bstr_t szUserPwd(pcszUserPwd);

		m_pConnection->ConnectionTimeout = 10;
		//m_pConnection->KeepConnection = true;

		try
		{
			m_pConnection->Open(szConStr, szUserId, szUserPwd, /*adConnectUnspecified*/adModeUnknown);
		}
		catch (_com_error cComErr2)
		{
			if (((LONG)cComErr2.Error()) == 0x800401f0)
			{
				// It does means that the CoInitialize must be called first.
				Log(_T("_ConnectionPtr->Open failed because we need to call the method CoInitialize first.\nWe will call method CoInitialize here and try again."));
				CoInitialize(NULL);
				m_pConnection->Open(szConStr, szUserId, szUserPwd, /*adConnectUnspecified*/adModeUnknown);
			}
			else
			{
				throw cComErr2;
			}	
		}
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::ConnectToDB"), bLogComError);

		SetLastErrorCode(ERR_CANT_CONNECT_DB);
		return (FALSE);
	}
	catch (...)
	{
		SetLastErrorCode(ERR_CANT_CONNECT_DB);
		return (FALSE);
	}
	
	LONG lState = m_pConnection->State;
	m_bConnected = ( ((lState & adStateOpen) == adStateOpen) || 
					 ((lState & adStateExecuting) == adStateExecuting) || 
					 ((lState & adStateFetching) == adStateFetching) 
				   );
	
	return (m_bConnected);	
}

/**************************************************************************
 * 设置最后一次出错代码
 * INT SetLastErrorCode(INT nLastErrorCode)
 * 参数:
 *    nLastErrorCode - 出错代码
 * 返回值:
 *    nLastErrorCode
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 几乎其他所有模块
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
INT CDBUtil::SetLastErrorCode(INT nLastErrorCode)
{
	m_nLastErrorCode = nLastErrorCode;
	return m_nLastErrorCode;
}


/**************************************************************************
 * 获得最后一次出错代码
 * INT GetLastErrorCode()
 * 参数:
 *   
 * 返回值:
 *   最后一次的出错代码
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
INT CDBUtil::GetLastErrorCode()
{
	return m_nLastErrorCode;
}

/**************************************************************************
 * 设置最后一次出错信息
 * void SetLastErrorMsg(TString szErrMsg)
 * 参数:
 *    szErrMsg - 出错信息
 * 返回值:
 *    无
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 几乎其他所有模块
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
void CDBUtil::SetLastErrorMsg(TString szErrMsg)
{
	m_szLastErrorMsg = szErrMsg;
}

/**************************************************************************
 * 判断当前是否已经连接数据库
 * BOOL IsConnected()
 * 参数:
 *    
 * 返回值:
 *    TRUE  - 已经连接
 *    FALSE - 尚未连接
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
BOOL CDBUtil::IsConnected()
{
	//return m_bConnected;
	if ( (m_pConnection != NULL) && m_bConnected )
	{
//		try
//		{
//			_variant_t value = GetSingleValue(_T("SELECT GetDate()"));
//			if ( (GetLastErrorCode() == ERR_SUCCESS) && (value.vt == VT_DATE) )
//				m_bConnected = TRUE;
//			else
//				m_bConnected = FALSE;
//		}
//		catch (...) {
//			m_bConnected = FALSE;
//		}

		return (m_bConnected);
//		LONG lState = m_pConnection->GetState();	//State;
//		return ( ((lState & adStateOpen) == adStateOpen) || 
//				 ((lState & adStateExecuting) == adStateExecuting) || 
//				 ((lState & adStateFetching) == adStateFetching) 
//			    );
	}
	else
		m_bConnected = FALSE;

	return m_bConnected;
}

/**************************************************************************
 * 关闭当前数据库连接
 * void Disconnect()
 * 参数:
 *    
 * 返回值:
 *    
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
void CDBUtil::Disconnect()
{
	m_bConnected = FALSE;

	if (!m_pConnection)
		return;

	try
	{
		if (m_pConnection->State)
		{
			m_pConnection->Close();
		}		
	}
	catch (...) {}
	
	try
	{
		m_pConnection.Release();		
	}
	catch (...) {}

	m_pConnection = NULL;
}

/**************************************************************************
 * 获得最后一次出错信息的指针
 * const char* GetLastErrormsg()
 * 参数:
 *    
 * 返回值:
 *    最后一次错误信息
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
LPCTSTR CDBUtil::GetLastErrormsg()
{	
	return m_szLastErrorMsg.c_str();
}

/**************************************************************************
 * 如果异常的类型是_com_error，将错误描述提取出来
 * void CDBUtil::IndicateComErrorException(_com_error&		cComErr,
 *										   _ConnectionPtr&	pConn,
 *										   LPCTSTR		pcszIdentifierTag)
 * 参数:
 *    cComErr - 异常
 *    pConn - 当前连接
 *    pszIdentifierTag - 出错的位置
 * 返回值:
 *    (无)
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 几乎所有模块
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
void CDBUtil::IndicateComErrorException(_com_error&		cComErr,
										   _ConnectionPtr&	pConn,
										   LPCTSTR		pcszIdentifierTag,
										   BOOL			bLogComError)
{
	const int MAX_LEN = 512;
	TCHAR szMsg[MAX_LEN] = { _T('\0') };

	//生成_com_error错误消息
	TString szFormatedMsg;
	CStrUtil::FormatMsg(NULL, szFormatedMsg, (INT)cComErr.Error());
	_stprintf_s((LPTSTR)szMsg, MAX_LEN, _T("[CDBUtil] _com_error caught in '%s', hr = 0x%08X, Message=%s"), pcszIdentifierTag, cComErr.Error(), szFormatedMsg.c_str());

	m_szLastErrorMsg = szMsg;
	IErrorInfo *pErr = cComErr.ErrorInfo();
	if (pErr != NULL)
	{
		BSTR	bstrDescription;
		BSTR	bstrSource;

		pErr->GetDescription(&bstrDescription);
		pErr->GetSource(&bstrSource);
		pErr->Release();

		_stprintf_s(szMsg, MAX_LEN, _T("\tFrom _com_error:\n\t- Description: '%s'; \n\t- Source: '%s'"), W2CT(_bstr_t(bstrDescription)), W2CT(_bstr_t(bstrSource)));

		::SysFreeString(bstrDescription);
		::SysFreeString(bstrSource);

		m_szLastErrorMsg += szMsg;
	}

	//生成Connection错误消息
	Errors		*pErrors;
	Error		*pError;

	if (pConn != NULL)
	{
		pConn->get_Errors(&pErrors);
		if (pErrors != NULL)
		{
			LONG lCount = pErrors->GetCount();

			for ( LONG lIndx = 0 ; lIndx < lCount ; lIndx++ )
			{
				pError = pErrors->Item[lIndx];
				LONG lNative = pError->GetNativeError();
				LONG lNumber = pError->GetNumber();

				// Check the returned SQLState and change value of m_bConnected
				LPCTSTR pState = (LPCTSTR)pError->GetSQLState();
				CheckSQLState(pState);

				_stprintf_s(szMsg, MAX_LEN, _T("\tFrom _Connection (%u):\n\t- Description: '%s'; \n\t- Source: '%s', \n\t- SQLState: '%s'"),
												lIndx + 1, W2CT(pError->GetDescription()), W2CT(pError->GetSource()), pState);
				pError->Release();
				m_szLastErrorMsg += _T("\n");
				m_szLastErrorMsg += szMsg;
			}

			pErrors->Release();
		}
	}

	// If the connection is broken, release resource
	if (!IsConnected())
	{
		Disconnect();
	}

	if (bLogComError)
		Log(m_szLastErrorMsg.c_str());
}

/**************************************************************************
 * 设置数据库服务器名
 * BOOL SetServerName(LPCTSTR pcszServerName)
 * 参数:
 *    lpcszServerName - 数据库服务器名
 * 返回值:
 *    TRUE  - 成功
 *    FALSE - 失败
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: (无)
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
BOOL CDBUtil::SetServerName(LPCTSTR pcszServerName)
{
	if (!pcszServerName || (m_dwConnectMode != CM_USING_SERVER_NAME))
		return FALSE;

	m_szServerName = pcszServerName;

	return GenerateConnectStr();
}

/**************************************************************************
* 设置数据库连接字符串
* BOOL SetConnectStr(LPCTSTR pcszConnectStr)
* 参数:
*    lpcszConnectStr - 数据库连接字符串
* 返回值:
*    TRUE  - 成功
*    FALSE - 失败
* -----------------------------------------------------------------------
* 调用: 
* 被调用: (无)
* -----------------------------------------------------------------------
* 注: 
* -----------------------------------------------------------------------
* 作者: 张大蒙
* 修改日期: 2004.05.11
*************************************************************************/
BOOL CDBUtil::SetConnectStr(LPCTSTR pcszConnectStr)
{
	if (!pcszConnectStr || (m_dwConnectMode != CM_USING_CONNECT_STR))
		return FALSE;

	m_szConnStr = pcszConnectStr;

	return TRUE;
}

/**************************************************************************
 * 设置登录用户名
 * BOOL SetUserName(LPCTSTR pcszUserName)
 * 参数:
 *    lpcszUserName - 用户名
 * 返回值:
 *    TRUE  - 成功
 *    FALSE - 失败
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: (无)
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
BOOL CDBUtil::SetUserName(LPCTSTR pcszUserName)
{
	if (!pcszUserName)
		return FALSE;

	m_szUserId = pcszUserName;

	return TRUE;
}

/**************************************************************************
 * 设置登录密码
 * BOOL SetUserPassword(LPCTSTR pcszUserPwd)
 * 参数:
 *    lpcszUserPwd - 登录密码
 * 返回值:
 *    TRUE  - 成功
 *    FALSE - 失败
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: (无)
 * -----------------------------------------------------------------------
 * 注: 
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
BOOL CDBUtil::SetUserPassword(LPCTSTR pcszUserPwd)
{
	if (!pcszUserPwd)
		return FALSE;

	m_szUserPwd = pcszUserPwd;

	return TRUE;
}

/**************************************************************************
* 设置欲连接数据库的类型
* BOOL SetDBType(DWORD dwDBType)
* 参数:
*    dwDBType - 数据库的类型
*				DT_OTHER				0		//其他
*				DT_ORACLE				1		//ORACLE
*				DT_MSSQL				2		//MS SQL Server
*				DT_ASE					3		//Sybase Adptive Server Enterprise
*				DT_ASA					4		//Sybase Adptive Server Anywhere
*				DT_MDB					5		//MS Access MDB
* 返回值:
*    TRUE  - 成功
*    FALSE - 失败
* -----------------------------------------------------------------------
* 调用: 
* 被调用: (无)
* -----------------------------------------------------------------------
* 注: 
* -----------------------------------------------------------------------
* 作者: 张大蒙
* 修改日期: 2004.05.19
*************************************************************************/
BOOL CDBUtil::SetDBType(DWORD dwDBType)
{
	DWORD dwDBTemplateCount = NUMELMS(DB_TEMPLATE);
	if (dwDBType < 0 || dwDBType >= dwDBTemplateCount)
		return FALSE;

	m_dwDestDBType = dwDBType;

	switch(m_dwDestDBType) {
	case DT_ASA:
	case DT_MDB:
	case DT_DBASE:
	case DT_EXCEL:
	case DT_TEXT:
	case DT_VFOXPRO:
		//这些数据库不是网络数据库，不启动事务
		m_bNeedTrans = FALSE;
		break;
	
	default:
		m_bNeedTrans = TRUE;
		break;

	}
	return TRUE;
}

/**************************************************************************
* 设置数据库连接方式
* BOOL SetConnectMode(DWORD dwMode)
* 参数:
*    dwMode - 数据库连接方式
*			CM_USING_CONNECT_STR	1		// 用连接字符串进行连接
*			CM_USING_SERVER_NAME	2		// 通过ServerName + Database来连接
* 返回值:
*    TRUE  - 成功
*    FALSE - 失败
* -----------------------------------------------------------------------
* 调用: 
* 被调用: (无)
* -----------------------------------------------------------------------
* 注: 
* -----------------------------------------------------------------------
* 作者: 张大蒙
* 修改日期: 2004.05.19
*************************************************************************/
BOOL CDBUtil::SetConnectMode(DWORD dwMode)
{
	if (dwMode < 1 || dwMode > 2)
		return FALSE;

	m_dwConnectMode = dwMode;

	return TRUE;
}

/**************************************************************************
* 设置数据库名
* BOOL SetDBName(LPCTSTR pcszDBName)
* 参数:
*    lpcszDBName - 数据库名
* 返回值:
*    TRUE  - 成功
*    FALSE - 失败
* -----------------------------------------------------------------------
* 调用: 
* 被调用: (无)
* -----------------------------------------------------------------------
* 注: 
* -----------------------------------------------------------------------
* 作者: 张大蒙
* 修改日期: 2004.05.19
*************************************************************************/
BOOL CDBUtil::SetDBName(LPCTSTR pcszDBName)
{
	if (!pcszDBName || (m_dwConnectMode != CM_USING_SERVER_NAME))
		return FALSE;

	m_szDBName = pcszDBName;

	return GenerateConnectStr();
}

/**************************************************************************
* 根据m_szServerName和m_szDBName合成连接字符串
* BOOL GenerateConnectStr()
* 参数:
*    无
* 返回值:
*    TRUE  - 成功
*    FALSE - 失败
* -----------------------------------------------------------------------
* 调用: 
* 被调用: (无)
* -----------------------------------------------------------------------
* 注: 
* -----------------------------------------------------------------------
* 作者: 张大蒙
* 修改日期: 2004.05.19
*************************************************************************/
BOOL CDBUtil::GenerateConnectStr(void)
{
	if (m_dwConnectMode == CM_USING_CONNECT_STR)
		return FALSE;

	TCHAR szMsg[256];

	switch (m_dwDestDBType)
	{
	case DT_ORACLE:
	case DT_ASE:
	case DT_MSSQL:
	case DT_MYSQL:
	case DT_ASA:
		// 只需要指定ServerName，或需要指定ServerName + DBName，或者是:目录名+DBName
		// 但是参数的顺序相同
		_stprintf_s(szMsg, DB_TEMPLATE[m_dwDestDBType], m_szServerName.c_str(), m_szDBName.c_str());
		m_szConnStr = szMsg;
		break;

	case DT_MDB:
	case DT_DBASE:
	case DT_TEXT:
	case DT_VFOXPRO:
		// 只需要指定DBName
		_stprintf_s(szMsg, DB_TEMPLATE[m_dwDestDBType], m_szDBName.c_str());
		m_szConnStr = szMsg;
		break;

	default:
		break;		
	}
	
	return TRUE;
}

/**************************************************************************
 * 将Blob数据转换为Variant类型的数据
 * INT Blob2Variant(char *pBuf, DWORD dwBufLen, _variant_t &Variant) throw (INT)
 * 参数:
 *    pBuf - 数据缓冲区指针
 *    dwBufLen - 数据缓冲区长度
 *    Variant - 将转换后的数据保存在这里
 * 返回值:
 *    数据的长度
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 
 * -----------------------------------------------------------------------
 * 注: 如果失败,可查看最后一次错误代码,及错误信息
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2004.02.19
 *************************************************************************/
INT CDBUtil::Blob2Variant(char *pBuf, DWORD dwBufLen, _variant_t &Variant) throw (INT)
{
	INT nRet = 0;

	try
	{
		if (pBuf == NULL)
			throw ERR_INVALID_OBJECT;
		if (dwBufLen < 1)
			throw ERR_INVALID_LEN;
		
		char			*pTmp= pBuf;
		SAFEARRAY		*psa = NULL;
		SAFEARRAYBOUND	rgsabound[1];
		
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = dwBufLen;
		if ( NULL == (psa = SafeArrayCreate(VT_UI1, 1, rgsabound)) )		
			throw ERR_CREATE_SAFEARRAY;
		
		for (LONG i = 0; i < dwBufLen; i++)
			SafeArrayPutElement (psa, &i, pTmp++);

		Variant.vt = VT_ARRAY | VT_UI1;
		Variant.parray = psa;
		
	}
	catch(INT nErrorCode)
	{
		nRet = nErrorCode;
		throw;
	}
	
	return nRet;
}

_RecordsetPtr CDBUtil::GetRecordset(const _variant_t & Source, CursorTypeEnum CursorType, LockTypeEnum LockType, LONG Options)
{
	try
	{
		m_bExecuting = TRUE;

		//如果没有连接，则用缺省的成员变量连接
		if (!IsConnected())
			if (!Connect())
				throw ERR_CANT_CONNECT_DB;
		
		_RecordsetPtr pRs("ADODB.Recordset");
		if (S_OK == pRs->Open(Source, _variant_t((IDispatch *)m_pConnection,TRUE)/*m_pConnection.GetInterfacePtr()*/, CursorType, LockType, Options))
		{
			m_bExecuting = FALSE;
			return pRs;
		}

		SetLastErrorCode(0);
	}
	catch (_com_error cComErr)
	{		
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::GetRecordset"));
		SetLastErrorCode(ERR_SELECT_ERROR);
	}
	catch (INT error_code)
	{
		SetLastErrorCode(error_code);
	}
	catch (...)
	{
		SetLastErrorCode(-GetLastError());
	}

	m_bExecuting = FALSE;
	return NULL;
}

/**************************************************************************
 * 执行SQL语句
 * _RecordsetPtr Execute( _bstr_t CommandText, VARIANT * RecordsAffected, LONG Options )
 * 参数:
 *    lpcszSQL - SQL语句
 * 返回值:
 *    结果集可能为空(如果是Update, delete 或insert操作)
 * -----------------------------------------------------------------------
 * 调用: 
 * 被调用: 
 * -----------------------------------------------------------------------
 * 注: 如果失败,可查看最后一次错误代码,及错误信息
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 修改日期: 2007.01.17
 *************************************************************************/
 LONG CDBUtil::Execute( _bstr_t CommandText, LONG Options ) throw (INT)
{
	if (!CommandText || (CommandText.length() <= 0))
		throw SetLastErrorCode(ERR_INVALID_OBJECT);
	
	try
	{
		m_bExecuting = TRUE;

		//如果没有连接，则用缺省的成员变量连接
		if (!IsConnected())
			if (!Connect())
				throw ERR_CANT_CONNECT_DB;
			
		m_pConnection->Errors->Clear();
		VARIANT vRecordsAffected;
		vRecordsAffected.vt = VT_I4;
		m_pConnection->Execute(CommandText, &vRecordsAffected, Options);	// the records set is read-only
		if (m_pConnection->Errors->Count > 0)
		{
			SetLastErrorMsg((LPCTSTR)m_pConnection->Errors->GetItem(_variant_t((LONG)0))->Description);
			throw ERR_CANT_DEL_DATA;
		}
	
		SetLastErrorCode(0);
		m_bExecuting = FALSE;
		return vRecordsAffected.lVal;
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::Execute"));
		m_bExecuting = FALSE;
		throw SetLastErrorCode(ERR_SQL_EXEC_FAILED);
	}
	catch(INT error_code)
	{			
		m_bExecuting = FALSE;
		throw SetLastErrorCode(error_code);
	}
	catch (...)
	{
		m_bExecuting = FALSE;
		throw SetLastErrorCode((-(LONG)GetLastError()));
	}
}

_variant_t CDBUtil::GetSingleValue(LPCTSTR szCommand, LONG lCommandType)
{
	// Only get the value of first column at first row
	SetLastErrorCode(ERR_NO_RECORD);

	_variant_t vRetVal;		
	try
	{	
		// m_bWorking = TRUE; Is not needed because the var was changed in method GetRecordset

		_RecordsetPtr pRs = GetRecordset(_bstr_t(szCommand), adOpenForwardOnly, adLockReadOnly, adCmdText);
		if (pRs != NULL)
		{
			if (!(pRs->adoEOF))
			{
				vRetVal = pRs->Fields->GetItem(_variant_t((long)0))->GetValue();
				SetLastErrorCode(0);
			}

			pRs->Close();
			pRs.Release();			
		}
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::GetSingleValue"));
		SetLastErrorCode(ERR_SELECT_ERROR);
	}
	catch(INT error_code)
	{		
		SetLastErrorCode(error_code);
	}
	catch(...)
	{
		SetLastErrorCode(ERR_UNKONW_ERROR);
	}

	return vRetVal;
}

BOOL CDBUtil::GetSingleBoolValue(LPCTSTR szCommand, BOOL bDefault, LONG lCommandType)
{
	_variant_t vRetVal = GetSingleValue(szCommand, lCommandType);
	if (ERR_SUCCESS == m_nLastErrorCode)
	{
		switch (vRetVal.vt)
		{
		case VT_BOOL:
			return vRetVal.boolVal;
		case VT_BSTR:
			{
				char* pTemp = _com_util::ConvertBSTRToString((BSTR)vRetVal.pbstrVal);
				BOOL bFlag = ((strcmp(pTemp, "1") == 0) || (_stricmp(pTemp, "true") == 0) || (_stricmp(pTemp, "yes") == 0));
				delete[] pTemp;
				return bFlag;
			}
		default:
			SetLastErrorCode(ERR_INVALID_DATATYPE);
			break;
		}
	}

	return bDefault;
}

INT CDBUtil::GetSingleStringValue(std::basic_string<TCHAR> &szVal, LPCTSTR pDefault, LPCTSTR szCommand, LONG lCommandType)
{
	szVal = pDefault ? pDefault : _T("");
	_variant_t vRetVal = GetSingleValue(szCommand, lCommandType);
	if (ERR_SUCCESS == m_nLastErrorCode)
	{
		switch (vRetVal.vt)
		{
		case VT_BSTR:
			{
				SetLastErrorCode(0);
				LPCTSTR pTemp = W2CT((BSTR)vRetVal.pbstrVal);
				szVal = pTemp;
				return szVal.size();
			}
		default:
			SetLastErrorCode(ERR_INVALID_DATATYPE);
			break;
		}
	}

	return m_nLastErrorCode;
}

LONG CDBUtil::GetSingleLongValue(LPCTSTR szCommand, LONG lDefault, LONG lCommandType)
{
	_variant_t vRetVal = GetSingleValue(szCommand, lCommandType);
	if (ERR_SUCCESS == m_nLastErrorCode)
	{
		switch (vRetVal.vt)
		{
		case VT_I4:
			return vRetVal.lVal;
		case VT_UI4:
			return vRetVal.ulVal;
		case VT_BSTR:
			{
				char* pTemp = _com_util::ConvertBSTRToString((BSTR)vRetVal.pbstrVal);
				LONG lRet = atol(pTemp);
				delete[] pTemp;
				return lRet;
			}
		default:
			SetLastErrorCode(ERR_INVALID_DATATYPE);
			break;
		}
	}

	return lDefault;
}

DOUBLE CDBUtil::GetSingleDoubleValue(LPCTSTR szCommand, DOUBLE dblDefault, BYTE nScale, LONG lCommandType)
{
	DOUBLE dblVal = dblDefault;	
	DECIMAL decDefault;
	CStrUtil::Double2Decimal(decDefault, dblVal);

	DECIMAL dec = GetSingleDecimalValue(szCommand, decDefault, lCommandType);
	if (ERR_SUCCESS == m_nLastErrorCode)
	{
		dec.scale = nScale;
		CStrUtil::Decimal2Doule(dblVal, dec);
	}
	
	return dblVal;
}

DECIMAL CDBUtil::GetSingleDecimalValue(LPCTSTR szCommand, DECIMAL decDefault, LONG lCommandType)
{
	_variant_t vRetVal = GetSingleValue(szCommand, lCommandType);
	if (ERR_SUCCESS == m_nLastErrorCode)
	{
		switch (vRetVal.vt)
		{
		case VT_DECIMAL:
			return (DECIMAL)vRetVal;
		default:
			SetLastErrorCode(ERR_INVALID_DATATYPE);
			break;
		}
	}

	return decDefault;
}

BOOL CDBUtil::StartConnectionEvent()
{
	BOOL bResult = FALSE;
	HRESULT hr;   
	IConnectionPointContainer	*pCPC = NULL;
	IConnectionPoint			*pCP = NULL;
	IUnknown					*pUnk = NULL;
	
	try
	{
		if (m_pConnection)
		{
			//Stop event
			
			// Start using the Connection events
			hr = m_pConnection->QueryInterface(__uuidof(IConnectionPointContainer), (void **)&pCPC);
			if (FAILED(hr)) throw -1;
			
			hr = pCPC->FindConnectionPoint(__uuidof(ConnectionEvents), &pCP);
			pCPC->Release();
			if (FAILED(hr)) throw -1;
			
			m_pConnectEvent = new CConnEvent(&m_bConnected);
			hr = m_pConnectEvent->QueryInterface(__uuidof(IUnknown), (void **) &pUnk);
			if (FAILED(hr)) throw -1;
			
			hr = pCP->Advise(pUnk, &m_dwConnEvt);
			pCP->Release();
			if (FAILED(hr)) throw -1;
			
			bResult = TRUE;
		}
	}
	catch (...) {
	}
	
	return bResult;
}

BOOL CDBUtil::StopConnectionEvent()
{
	BOOL bResult = FALSE;
	HRESULT hr;   
	IConnectionPointContainer	*pCPC = NULL;
	IConnectionPoint				*pCP = NULL;
	IUnknown						*pUnk = NULL;
	
	try
	{
		if ( (NULL != m_pConnection) && (0 != m_dwConnEvt) )
		{
			hr = m_pConnection->QueryInterface(__uuidof(IConnectionPointContainer), (void **) &pCPC);
			if (FAILED(hr)) throw -1;
			
			hr = pCPC->FindConnectionPoint(__uuidof(ConnectionEvents), &pCP);
			pCPC->Release();
			if (FAILED(hr)) throw -1;
			
			hr = pCP->Unadvise( m_dwConnEvt );
			pCP->Release();
			if (FAILED(hr)) throw -1;

			m_dwConnEvt = 0;
			delete m_pConnectEvent;
			m_pConnectEvent = NULL;

			bResult = TRUE;
		}
	}
	catch (...) {
	}
	
	return bResult;
}

LONG CDBUtil::BeginTrans()
{
	if ((NULL != m_pConnection) && IsConnected())
	{
		BOOL bRet = m_pConnection->BeginTrans();
		if (bRet)
			m_bInTransaction = TRUE;

		return bRet;
	}
	else
	{
		return ERR_DB_NOT_OPEN;
	}
}

LONG CDBUtil::CommitTrans()
{
	m_bInTransaction = FALSE;

	if ((NULL != m_pConnection) && IsConnected())
		return m_pConnection->CommitTrans();
	else
		return ERR_DB_NOT_OPEN;
}

LONG CDBUtil::RollbackTrans()
{
	m_bInTransaction = FALSE;

	if ((NULL != m_pConnection) && IsConnected())
		return m_pConnection->RollbackTrans();
	else
		return ERR_DB_NOT_OPEN;
}

void CDBUtil::CheckSQLState(LPCTSTR pState)
{		
	if (!_tcscmp(pState, _T("08S01"))		// 通讯链接失败
		|| !_tcscmp(pState, _T("01002"))	// 断开连接错误
		|| !_tcscmp(pState, _T("08001"))	// 无法连接到数据源
		|| !_tcscmp(pState, _T("08003"))	// 连接未打开
		|| !_tcscmp(pState, _T("08004"))	// 数据源拒绝建立连接
		|| !_tcscmp(pState, _T("08007"))	// 在执行事务的过程中连接失败
		)
	{
		m_bConnected = FALSE;
	}
	
#ifndef _DISABLE_LOG_
	if (!_tcscmp(pState, _T("08S01"))		// 通讯链接失败
		|| !_tcscmp(pState, _T("01002"))	// 断开连接错误
		|| !_tcscmp(pState, _T("08007"))	// 在执行事务的过程中连接失败
		)
	{
		m_Logger.VLog(_T("[CDBUtil::CheckSQLState] The connection to database is broken, SQLState=%s"), pState);
	}
#endif

}

BOOL CDBUtil::Connect()
{
	// Note: This is a virtual method for the class derived from CDBUtil
	//		 CDBUtil will call this method automatically when it found currently no connection to DB during executing SQL scrpt
	//		 The derived class should do some initialize for DB connection.

	return ConnectToDB();
}

INT CDBUtil::GetLong(INT &nRet, _RecordsetPtr pRs, INT nCol)
{
	LONG lRet;
	INT nActionRet = GetLong(lRet, pRs, nCol);

	nRet = (INT)lRet;

	return nActionRet;
}

INT CDBUtil::GetLong(WORD &wRet, _RecordsetPtr pRs, INT nCol)
{
	LONG lRet;
	INT nActionRet = GetLong(lRet, pRs, nCol);

	wRet = (WORD)lRet;

	return nActionRet;
}

INT CDBUtil::GetLong(DWORD &dwRet, _RecordsetPtr pRs, INT nCol)
{
	LONG lRet;
	INT nActionRet = GetLong(lRet, pRs, nCol);

	dwRet = (DWORD)lRet;

	return nActionRet;
}

INT CDBUtil::GetLong(LONG &lRet, _RecordsetPtr pRs, INT nCol)
{
	try
	{	
		if (NULL == pRs)
			throw ERR_INVALID_OBJECT;

		if (nCol < 0)
			throw ERR_INVALID_LEN;

		if (pRs->adoEOF)
			throw ERR_NO_RECORD;

		_variant_t vRetVal = pRs->Fields->GetItem(_variant_t((long)nCol))->GetValue();
		lRet = vRetVal.lVal;

		SetLastErrorCode(0);
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::GetSingleValue"));
		SetLastErrorCode(ERR_SELECT_ERROR);
	}
	catch(INT error_code)
	{		
		SetLastErrorCode(error_code);
	}
	catch(...)
	{
		SetLastErrorCode(ERR_UNKONW_ERROR);
	}

	return m_nLastErrorCode;
}

INT CDBUtil::GetString(std::string &szRet, _RecordsetPtr pRs, INT nCol)
{
	try
	{	
		if (NULL == pRs)
			throw ERR_INVALID_OBJECT;

		if (nCol < 0)
			throw ERR_INVALID_LEN;

		if (pRs->adoEOF)
			throw ERR_NO_RECORD;

		_variant_t vRetVal = pRs->Fields->GetItem(_variant_t((long)nCol))->GetValue();
		if (vRetVal.vt != VT_BSTR)
			throw ERR_INVALID_DATATYPE;

		char* pTemp = _com_util::ConvertBSTRToString((BSTR)vRetVal.pbstrVal);
		szRet = pTemp;
		delete[] pTemp;

		SetLastErrorCode(0);
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::GetSingleValue"));
		SetLastErrorCode(ERR_SELECT_ERROR);
	}
	catch(INT error_code)
	{		
		SetLastErrorCode(error_code);
	}
	catch(...)
	{
		SetLastErrorCode(ERR_UNKONW_ERROR);
	}

	return m_nLastErrorCode;
}

INT CDBUtil::GetString(std::wstring &wszRet, _RecordsetPtr pRs, INT nCol)
{
	try
	{	
		if (NULL == pRs)
			throw ERR_INVALID_OBJECT;

		if (nCol < 0)
			throw ERR_INVALID_LEN;

		if (pRs->adoEOF)
			throw ERR_NO_RECORD;

		_variant_t vRetVal = pRs->Fields->GetItem(_variant_t((long)nCol))->GetValue();
		if (vRetVal.vt != VT_BSTR)
			throw ERR_INVALID_DATATYPE;

		USES_CONVERSION;
		wszRet = OLE2W((BSTR)vRetVal.pbstrVal);

		SetLastErrorCode(0);
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::GetSingleValue"));
		SetLastErrorCode(ERR_SELECT_ERROR);
	}
	catch(INT error_code)
	{		
		SetLastErrorCode(error_code);
	}
	catch(...)
	{
		SetLastErrorCode(ERR_UNKONW_ERROR);
	}

	return m_nLastErrorCode;
}

INT CDBUtil::GetDecimal(DOUBLE &dblVal, BYTE nScale, _RecordsetPtr pRs, INT nCol)
{
	DECIMAL dec;
	dec.scale = nScale;

	INT nRet = GetDecimal(dec, pRs, nCol);
	CStrUtil::Decimal2Doule(dblVal, dec);
	
	return nRet;
}

INT CDBUtil::GetDecimal(DECIMAL &dec, _RecordsetPtr pRs, INT nCol)
{
	try
	{	
		if (NULL == pRs)
			throw ERR_INVALID_OBJECT;

		if (nCol < 0)
			throw ERR_INVALID_LEN;

		if (pRs->adoEOF)
			throw ERR_NO_RECORD;

		_variant_t vRetVal = pRs->Fields->GetItem(_variant_t((long)nCol))->GetValue();
		if (vRetVal.vt != VT_DECIMAL)
			throw ERR_INVALID_DATATYPE;

		dec	= (DECIMAL)vRetVal;

		SetLastErrorCode(0);
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::GetSingleValue"));
		SetLastErrorCode(ERR_SELECT_ERROR);
	}
	catch(INT error_code)
	{		
		SetLastErrorCode(error_code);
	}
	catch(...)
	{
		SetLastErrorCode(ERR_UNKONW_ERROR);
	}

	return m_nLastErrorCode;
}

INT CDBUtil::GetBoolean(BOOL &bVal, _RecordsetPtr pRs, INT nCol)
{
	INT nRet;
	bool bRet;
	
	if (ERR_SUCCESS == (nRet = GetBoolean(bRet, pRs, nCol)))
	{
		bVal = (BOOL)bRet;
	}
	
	return nRet;
}

INT CDBUtil::GetBoolean(bool &bVal, _RecordsetPtr pRs, INT nCol)
{
	try
	{	
		if (NULL == pRs)
			throw ERR_INVALID_OBJECT;

		if (nCol < 0)
			throw ERR_INVALID_LEN;

		if (pRs->adoEOF)
			throw ERR_NO_RECORD;

		_variant_t vRetVal = pRs->Fields->GetItem(_variant_t((long)nCol))->GetValue();
		if (vRetVal.vt != VT_BOOL)
			throw ERR_INVALID_DATATYPE;

		bVal = (bool)vRetVal.boolVal;

		SetLastErrorCode(ERR_SUCCESS);
	}
	catch (_com_error cComErr)
	{
		IndicateComErrorException(cComErr, m_pConnection, _T("CDBUtil::GetSingleValue"));
		SetLastErrorCode(ERR_SELECT_ERROR);
	}
	catch(INT error_code)
	{		
		SetLastErrorCode(error_code);
	}
	catch(...)
	{
		SetLastErrorCode(ERR_UNKONW_ERROR);
	}

	return m_nLastErrorCode;
}

//-----Implement QueryInterface, AddRef, and Release---------------------

STDMETHODIMP CConnEvent::QueryInterface(REFIID riid, void ** ppv)

{
	*ppv = NULL;
	if (riid == __uuidof(IUnknown) ||
		riid == __uuidof(ConnectionEventsVt)) *ppv = this;
	if (*ppv == NULL)
		return ResultFromScode(E_NOINTERFACE);
	AddRef();
	return NOERROR;
}
STDMETHODIMP_(ULONG) CConnEvent::AddRef()
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CConnEvent::Release()
{
	if (0 != --m_cRef) return m_cRef;
	delete this;
	return 0;
}

STDMETHODIMP CConnEvent::raw_InfoMessage(
struct Error *pError,
	EventStatusEnum *adStatus,
struct _Connection *pConnection)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_BeginTransComplete(
	LONG TransactionLevel,
struct Error *pError,
	EventStatusEnum *adStatus,
struct _Connection *pConnection)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_CommitTransComplete(
struct Error *pError,
	EventStatusEnum *adStatus,
struct _Connection *pConnection)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_RollbackTransComplete(
struct Error *pError,
	EventStatusEnum *adStatus,
struct _Connection *pConnection)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_WillExecute(
	BSTR *Source,
	CursorTypeEnum *CursorType,
	LockTypeEnum *LockType,
	long *Options,
	EventStatusEnum *adStatus,
struct _Command *pCommand,
struct _Recordset *pRecordset,
struct _Connection *pConnection)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_ExecuteComplete(
	LONG RecordsAffected,
struct Error *pError,
	EventStatusEnum *adStatus,
struct _Command *pCommand,
struct _Recordset *pRecordset,
struct _Connection *pConnection)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_WillConnect(
	BSTR *ConnectionString,
	BSTR *UserID,
	BSTR *Password,
	long *Options,
	EventStatusEnum *adStatus,
struct _Connection *pConnection)
{
	*adStatus = adStatusUnwantedEvent;
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_ConnectComplete(
struct Error *pError,
	EventStatusEnum *adStatus,
struct _Connection *pConnection)
{
	//*adStatus = adStatusUnwantedEvent;
	if (m_pIsConnected)
		*m_pIsConnected = TRUE;

	int a = 0;
	return S_OK;
}

STDMETHODIMP CConnEvent::raw_Disconnect(
	EventStatusEnum *adStatus,
struct _Connection *pConnection)
{
	//*adStatus = adStatusUnwantedEvent;
	if (m_pIsConnected)
		*m_pIsConnected = FALSE;

	int a = 0;
	return S_OK;
}

void CConnEvent::SetStatePtr(BOOL *pIsConnected)
{
	m_pIsConnected = pIsConnected;
}
