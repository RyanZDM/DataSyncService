// DBConnectPool.h: interface for the CDBConnectPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCONNECTPOOL_H__60365550_AD3B_4F93_9DB7_480010A5D4CF__INCLUDED_)
#define AFX_DBCONNECTPOOL_H__60365550_AD3B_4F93_9DB7_480010A5D4CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DBUtil.h>
#include <ContainerUtil.h>
#include <LogUtil.h>

#define MAX_CONNECTIONS_IN_POOL	20

class CDBPool;

class CDBConnectPool  
{
public:
	CDBConnectPool();
	virtual ~CDBConnectPool();

	BOOL AddDataSource(LPCTSTR pcszDataSourceName, LPCTSTR pcszConStr, LPCTSTR pcszUserId, LPCTSTR pcszUserPwd);
	BOOL RemoveDataSource(LPCTSTR pcszDataSourceName);
	CDBUtil *GetConnectionPtr(LPCTSTR pcszDataSourceName);

private:
	vector<CDBPool*>	m_vConnectionPool;		// Connection pool

};

class CDBPool
{
public:
	CDBPool(LPCTSTR pcszName, LPCTSTR pcszConStr, LPCTSTR pcszUserId, LPCTSTR pcszUserPwd, DWORD dwMaxConnections = MAX_CONNECTIONS_IN_POOL)
	{
		m_szName			= pcszName;
		m_szConStr			= pcszConStr;
		m_szUserID			= pcszUserId;
		m_szPwd				= pcszUserPwd;
		m_dwMaxConnections	= dwMaxConnections;
	}

	~CDBPool() { Clear(); }

	void Clear() { ClearVector(m_vConnections); }
	BOOL Equal(LPCTSTR pcszName) { return ( pcszName && (lstrcmpi(pcszName, m_szName.c_str()) == 0) );}
	void SetMaxConnections(DWORD dwMaxConnections) { m_dwMaxConnections = dwMaxConnections; }
	DWORD GetMaxConnections() { return m_dwMaxConnections; }
	DWORD GetTotalConnections() { return m_vConnections.size(); }

	CDBUtil *GetConnectionPtr();

private:
	CDBUtil *NewConnection();

private:
	vector<CDBUtil*>	m_vConnections;			// Connection pool
	TString				m_szName;				// Pool name
	TString				m_szConStr;				// DB connection string
	TString				m_szUserID;				// User ID for connecting to DB
	TString				m_szPwd;				// Password for connecting to DB
	DWORD				m_dwMaxConnections;		// Maximum connections can be hold by pool
};


#endif // !defined(AFX_DBCONNECTPOOL_H__60365550_AD3B_4F93_9DB7_480010A5D4CF__INCLUDED_)
