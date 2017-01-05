// DBUtil.h: interface for the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////

/**************************************************************************
 * 类名: CDBUtil
 * 类文件: DBUtil.h, DBUtil.cpp
 * 功能: 数据库操作
 * -----------------------------------------------------------------------
 * 连接库: (无)
 * -----------------------------------------------------------------------
 * 作者: 张大蒙
 * 版本: 1.0
 * 最后修改日期: 
 *************************************************************************/

//temp
//#define _CALL_CoInitialize_

#if !defined(AFX_DBPROCESS_H__2EE0480D_007D_4BE1_8B82_85EB88FFBB00__INCLUDED_)
#define AFX_DBPROCESS_H__2EE0480D_007D_4BE1_8B82_85EB88FFBB00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4146 )

//引入ADO 1.5
#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" no_namespace rename("EOF","adoEOF")

#include <string>
#include <tchar.h>

typedef std::basic_string<TCHAR> TString;

#include "Logger.h"

//如何连接数据库
#define CM_USING_CONNECT_STR	1		// 用连接字符串进行连接
#define CM_USING_SERVER_NAME	2		// 通过ServerName + Database来连接

//欲连接数据库类型
#define DT_OTHER				0		// 其他
#define DT_ORACLE				1		// ORACLE
#define DT_MSSQL				2		// MS SQL Server
#define DT_ASE					3		// Sybase Adptive Server Enterprise
#define DT_ASA					4		// Sybase Adptive Server Anywhere
#define DT_MDB					5		// MS Access MDB
#define DT_DBASE				6		// dBase
#define DT_EXCEL				7		// Excel
#define DT_MYSQL				8		// mySQL
#define DT_TEXT					9		// Text
#define DT_VFOXPRO				10		// Visual Foxpro

//数据类型定义
#define CT_NONE					-1		// 未知类型
#define CT_CHAR					1		// 字符串型
#define CT_INT					2		// 整型或长整型
#define CT_FLOAT				3		// 浮点
#define CT_DOUBLE				4		// 双精度
#define CT_DATETIME				5		// 日期时间型
#define CT_BOOLEAN				6		// 布尔型
#define CT_BLOB					7		// 大二进制数据
/**************************************************************************/

//错误代码定义
#define ERR_SUCCESS				0
#define ERR_RECORDSET_NOT_DEF	-101	// Recordset未定义
#define ERR_DB_NOT_OPEN			-102	// 数据库未连接
#define ERR_CANT_CONNECT_DB		-103	// 无法连接到数据库
#define	ERR_SELECT_ERROR		-104	// 检索数据出错
#define ERR_SQL_EXEC_FAILED		-105	// SQL语句执行失败
#define ERR_INVALID_OBJECT		-106	// 无效的对象
#define	ERR_TABLE_NOT_FOUND		-107	// 表未发现
#define ERR_CANT_ADDNEW			-108	// 无法新增记录
#define ERR_IN_TRANS			-109	// 当前在一个数据处理事务中，不允许其他操作
#define ERR_CANT_DEL_DATA		-110	// 无法删除记录
#define ERR_CANT_INSERT_DATA	-111	// 无法添加记录
#define ERR_CREATE_SAFEARRAY	-112	// 无法创建安全数组
#define ERR_INVALID_LEN			-113	// 无效的长度
#define ERR_NO_RECORD			-114	// RecordSet中没有记录
#define ERR_INVALID_DATATYPE	-115	// 错误的数据类型

#define ERR_UNKONW_ERROR		-999	// 未知错误
/**************************************************************************/

// 常用函数
#define NUMELMS(aa) (sizeof(aa)/sizeof((aa)[0]))

//----The Connection events----------------------------------------------
class CConnEvent : public ConnectionEventsVt
{
private:
      ULONG   m_cRef;
	  BOOL	*m_pIsConnected;
   public:
      CConnEvent(BOOL *pIsConnected = NULL) { m_cRef = 0; m_pIsConnected = pIsConnected; };
      ~CConnEvent() {};
	  void SetStatePtr(BOOL *pIsConnected);


      STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);
      STDMETHODIMP_(ULONG) AddRef(void);
      STDMETHODIMP_(ULONG) Release(void);

      STDMETHODIMP raw_InfoMessage( 
         struct Error *pError,
         EventStatusEnum *adStatus,
         struct _Connection *pConnection);
      
      STDMETHODIMP raw_BeginTransComplete( 
         LONG TransactionLevel,
         struct Error *pError,
         EventStatusEnum *adStatus,
         struct _Connection *pConnection);
      
   STDMETHODIMP raw_CommitTransComplete( 
         struct Error *pError,
         EventStatusEnum *adStatus,
         struct _Connection *pConnection);
      
      STDMETHODIMP raw_RollbackTransComplete( 
         struct Error *pError,
         EventStatusEnum *adStatus,
         struct _Connection *pConnection);
      
      STDMETHODIMP raw_WillExecute( 
         BSTR *Source,
         CursorTypeEnum *CursorType,
         LockTypeEnum *LockType,
         long *Options,
         EventStatusEnum *adStatus,
         struct _Command *pCommand,
         struct _Recordset *pRecordset,
         struct _Connection *pConnection);
      
      STDMETHODIMP raw_ExecuteComplete( 
         LONG RecordsAffected,
         struct Error *pError,
         EventStatusEnum *adStatus,
         struct _Command *pCommand,
         struct _Recordset *pRecordset,
         struct _Connection *pConnection);
      
   STDMETHODIMP raw_WillConnect( 
         BSTR *ConnectionString,
         BSTR *UserID,
         BSTR *Password,
         long *Options,
         EventStatusEnum *adStatus,
         struct _Connection *pConnection);
      
      STDMETHODIMP raw_ConnectComplete( 
         struct Error *pError,
         EventStatusEnum *adStatus,
         struct _Connection *pConnection);
      
      STDMETHODIMP raw_Disconnect( 
         EventStatusEnum *adStatus,
         struct _Connection *pConnection);
};

class CDBUtil  
{
	
public:
	CDBUtil::CDBUtil(BOOL bStartEventMonitor = FALSE);
	virtual ~CDBUtil();

	INT GetBoolean(BOOL &bVal, _RecordsetPtr pRs, INT nCol);
	INT GetBoolean(bool &bVal, _RecordsetPtr pRs, INT nCol);
	INT GetDecimal(DECIMAL &dec, _RecordsetPtr pRs, INT nCol);
	INT GetDecimal(DOUBLE &dblVal, BYTE nScale, _RecordsetPtr pRs, INT nCol);
	INT GetLong(LONG &lRet, _RecordsetPtr pRs, INT nCol);
	INT GetLong(INT &nRet, _RecordsetPtr pRs, INT nCol);
	INT GetLong(WORD &wRet, _RecordsetPtr pRs, INT nCol);
	INT GetLong(DWORD &dwRet, _RecordsetPtr pRs, INT nCol);
	INT GetString(std::string &szRet, _RecordsetPtr pRs, INT nCol);
	INT GetString(std::wstring &wszRet, _RecordsetPtr pRs, INT nCol);
	virtual BOOL Connect();
	BOOL IsWorking() { if (m_bInTransaction) return TRUE; else return m_bExecuting; }
	LONG RollbackTrans();
	LONG CommitTrans();
	LONG BeginTrans();
	BOOL StopConnectionEvent();
	BOOL StartConnectionEvent();
	_variant_t GetSingleValue(LPCTSTR pcszCommand, LONG lCommandType = adCmdText);
	INT GetSingleStringValue(std::basic_string<TCHAR> &szVal, LPCTSTR pDefault, LPCTSTR szCommand, LONG lCommandType = adCmdText);
	LONG GetSingleLongValue(LPCTSTR szCommand, LONG lDefault, LONG lCommandType = adCmdText);
	BOOL GetSingleBoolValue(LPCTSTR szCommand, BOOL bDefault, LONG lCommandType = adCmdText);
	DOUBLE GetSingleDoubleValue(LPCTSTR szCommand, DOUBLE dblDefault, BYTE nScale, LONG lCommandType = adCmdText);	
	DECIMAL GetSingleDecimalValue(LPCTSTR szCommand, DECIMAL decDefault, LONG lCommandType = adCmdText);
	BOOL SetDBType(DWORD dwDBType);													// 设置与连接数据库的类型
	BOOL SetConnectMode(DWORD dwMode);												// 设置数据库连接方式
	BOOL SetConnectStr(LPCTSTR pcszConnectStr);										// 设置数据库连接字符串
	BOOL SetServerName(LPCTSTR pcszServerName);										// 设置数据库服务器名
	BOOL SetDBName(LPCTSTR pcszDBName);												// 设置数据库名
	BOOL SetUserName(LPCTSTR pcszUserName);											// 设置用户名
	BOOL SetUserPassword(LPCTSTR pcszUserPwd);										// 设置用户密码	
	BOOL ConnectToDB(BOOL bLogComError = TRUE);
	BOOL ConnectToDB(LPCTSTR pszConStr,
					 LPCTSTR pszUserId, 
					 LPCTSTR pszUserPwd,
					 BOOL bLogComError = TRUE);											// 连接数据库	
	BOOL IsConnected();																// 是否已连接数据库	
	void Disconnect();																// 断开数据库连接	
	virtual BOOL Init(void) { return TRUE; }
	_RecordsetPtr GetRecordset(const _variant_t & Source, 
							   CursorTypeEnum CursorType = adOpenForwardOnly, 
							   LockTypeEnum LockType = adLockReadOnly, 
							   LONG Options = adCmdText);
	LONG Execute( _bstr_t CommandText,
						   LONG Options = adCmdText | adExecuteNoRecords /*adCmdStoredProc,
															   adAsyncFetch,
															   adExecuteRecord,
															   adExecuteStream*/
						   );
	INT Blob2Variant(char *pBuf, DWORD dwBufLen, _variant_t &Variant);				// 生成Blob的Variant数组，成功返回0	
	INT GetLastErrorCode();															// 获得最后一次错误码	
	LPCTSTR GetLastErrormsg();														// 获得最后一次错误消息
	
	_ConnectionPtr m_pConnection;													// 数据库连接指针

protected:
	DWORD		m_dwConnectMode;													// 连接方式：1 用连接字符串进行连接 2 通过ServerName + Database来连接	
	DWORD		m_dwDestDBType;														// 欲连接数据库类型：1 Oracle 2 MS SQL 3 Sybase ASE 4 Sybase ASA 5 Access MDB
	TString		m_szServerName;														// 数据库网络名
	TString		m_szDBName;															// 数据库名
	TString		m_szUserPwd;														// 连接用户
	TString		m_szUserId;															// 连接密码
	TString		m_szConnStr;														// 数据库连接字符串

private:
	log4cplus::Logger	m_Logger;
	void CheckSQLState(LPCTSTR pState);
	BOOL m_bStartEventMonitor;
	CConnEvent	*m_pConnectEvent;
	DWORD		m_dwConnEvt;
	TString		m_szLastErrorMsg;													// 最后一次出错信息
	INT			m_nLastErrorCode;													// 最后一次错误代码
	BOOL		m_bConnected;														// 指示当前是否已经连接数据库
	BOOL		m_bIsInTrans;														// 指示当前是否在一个事务中
	BOOL		m_bNeedTrans;														// 指示当前数据库是否需要使用事务
	BOOL		m_bExecuting;														// 是否正在执行操作
	BOOL		m_bInTransaction;													// 是否在一个事务中
	
	INT SetLastErrorCode(INT nLastErrorCode);
	BOOL InitData();																// 初始化
	void SetLastErrorMsg(TString szErrMsg);											// 设置最后一次错误消息
	void IndicateComErrorException(_com_error&		cComErr,
								   _ConnectionPtr&	pConn,
								   LPCTSTR			pcszIdentifierTag,				// 生成数据库操作的错误信息
								   BOOL bLogComError = TRUE);				
	
	BOOL GenerateConnectStr(void);													// 根据m_szServerName和m_szDBName合成连接字符串
};

#endif // !defined(AFX_DBPROCESS_H__2EE0480D_007D_4BE1_8B82_85EB88FFBB00__INCLUDED_)
