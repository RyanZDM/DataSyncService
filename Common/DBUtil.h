// DBUtil.h: interface for the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////

/**************************************************************************
 * ����: CDBUtil
 * ���ļ�: DBUtil.h, DBUtil.cpp
 * ����: ���ݿ����
 * -----------------------------------------------------------------------
 * ���ӿ�: (��)
 * -----------------------------------------------------------------------
 * ����: �Ŵ���
 * �汾: 1.0
 * ����޸�����: 
 *************************************************************************/

//temp
//#define _CALL_CoInitialize_

#if !defined(AFX_DBPROCESS_H__2EE0480D_007D_4BE1_8B82_85EB88FFBB00__INCLUDED_)
#define AFX_DBPROCESS_H__2EE0480D_007D_4BE1_8B82_85EB88FFBB00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4146 )

//����ADO 1.5
#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" no_namespace rename("EOF","adoEOF")

#include <string>
#include <tchar.h>

typedef std::basic_string<TCHAR> TString;

#include "Logger.h"

//����������ݿ�
#define CM_USING_CONNECT_STR	1		// �������ַ�����������
#define CM_USING_SERVER_NAME	2		// ͨ��ServerName + Database������

//���������ݿ�����
#define DT_OTHER				0		// ����
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

//�������Ͷ���
#define CT_NONE					-1		// δ֪����
#define CT_CHAR					1		// �ַ�����
#define CT_INT					2		// ���ͻ�����
#define CT_FLOAT				3		// ����
#define CT_DOUBLE				4		// ˫����
#define CT_DATETIME				5		// ����ʱ����
#define CT_BOOLEAN				6		// ������
#define CT_BLOB					7		// �����������
/**************************************************************************/

//������붨��
#define ERR_SUCCESS				0
#define ERR_RECORDSET_NOT_DEF	-101	// Recordsetδ����
#define ERR_DB_NOT_OPEN			-102	// ���ݿ�δ����
#define ERR_CANT_CONNECT_DB		-103	// �޷����ӵ����ݿ�
#define	ERR_SELECT_ERROR		-104	// �������ݳ���
#define ERR_SQL_EXEC_FAILED		-105	// SQL���ִ��ʧ��
#define ERR_INVALID_OBJECT		-106	// ��Ч�Ķ���
#define	ERR_TABLE_NOT_FOUND		-107	// ��δ����
#define ERR_CANT_ADDNEW			-108	// �޷�������¼
#define ERR_IN_TRANS			-109	// ��ǰ��һ�����ݴ��������У���������������
#define ERR_CANT_DEL_DATA		-110	// �޷�ɾ����¼
#define ERR_CANT_INSERT_DATA	-111	// �޷���Ӽ�¼
#define ERR_CREATE_SAFEARRAY	-112	// �޷�������ȫ����
#define ERR_INVALID_LEN			-113	// ��Ч�ĳ���
#define ERR_NO_RECORD			-114	// RecordSet��û�м�¼
#define ERR_INVALID_DATATYPE	-115	// �������������

#define ERR_UNKONW_ERROR		-999	// δ֪����
/**************************************************************************/

// ���ú���
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
	BOOL SetDBType(DWORD dwDBType);													// �������������ݿ������
	BOOL SetConnectMode(DWORD dwMode);												// �������ݿ����ӷ�ʽ
	BOOL SetConnectStr(LPCTSTR pcszConnectStr);										// �������ݿ������ַ���
	BOOL SetServerName(LPCTSTR pcszServerName);										// �������ݿ��������
	BOOL SetDBName(LPCTSTR pcszDBName);												// �������ݿ���
	BOOL SetUserName(LPCTSTR pcszUserName);											// �����û���
	BOOL SetUserPassword(LPCTSTR pcszUserPwd);										// �����û�����	
	BOOL ConnectToDB(BOOL bLogComError = TRUE);
	BOOL ConnectToDB(LPCTSTR pszConStr,
					 LPCTSTR pszUserId, 
					 LPCTSTR pszUserPwd,
					 BOOL bLogComError = TRUE);											// �������ݿ�	
	BOOL IsConnected();																// �Ƿ����������ݿ�	
	void Disconnect();																// �Ͽ����ݿ�����	
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
	INT Blob2Variant(char *pBuf, DWORD dwBufLen, _variant_t &Variant);				// ����Blob��Variant���飬�ɹ�����0	
	INT GetLastErrorCode();															// ������һ�δ�����	
	LPCTSTR GetLastErrormsg();														// ������һ�δ�����Ϣ
	
	_ConnectionPtr m_pConnection;													// ���ݿ�����ָ��

protected:
	DWORD		m_dwConnectMode;													// ���ӷ�ʽ��1 �������ַ����������� 2 ͨ��ServerName + Database������	
	DWORD		m_dwDestDBType;														// ���������ݿ����ͣ�1 Oracle 2 MS SQL 3 Sybase ASE 4 Sybase ASA 5 Access MDB
	TString		m_szServerName;														// ���ݿ�������
	TString		m_szDBName;															// ���ݿ���
	TString		m_szUserPwd;														// �����û�
	TString		m_szUserId;															// ��������
	TString		m_szConnStr;														// ���ݿ������ַ���

private:
	log4cplus::Logger	m_Logger;
	void CheckSQLState(LPCTSTR pState);
	BOOL m_bStartEventMonitor;
	CConnEvent	*m_pConnectEvent;
	DWORD		m_dwConnEvt;
	TString		m_szLastErrorMsg;													// ���һ�γ�����Ϣ
	INT			m_nLastErrorCode;													// ���һ�δ������
	BOOL		m_bConnected;														// ָʾ��ǰ�Ƿ��Ѿ��������ݿ�
	BOOL		m_bIsInTrans;														// ָʾ��ǰ�Ƿ���һ��������
	BOOL		m_bNeedTrans;														// ָʾ��ǰ���ݿ��Ƿ���Ҫʹ������
	BOOL		m_bExecuting;														// �Ƿ�����ִ�в���
	BOOL		m_bInTransaction;													// �Ƿ���һ��������
	
	INT SetLastErrorCode(INT nLastErrorCode);
	BOOL InitData();																// ��ʼ��
	void SetLastErrorMsg(TString szErrMsg);											// �������һ�δ�����Ϣ
	void IndicateComErrorException(_com_error&		cComErr,
								   _ConnectionPtr&	pConn,
								   LPCTSTR			pcszIdentifierTag,				// �������ݿ�����Ĵ�����Ϣ
								   BOOL bLogComError = TRUE);				
	
	BOOL GenerateConnectStr(void);													// ����m_szServerName��m_szDBName�ϳ������ַ���
};

#endif // !defined(AFX_DBPROCESS_H__2EE0480D_007D_4BE1_8B82_85EB88FFBB00__INCLUDED_)
