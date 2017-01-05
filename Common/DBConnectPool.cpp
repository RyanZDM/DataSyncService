// DBConnectPool.cpp: implementation of the CDBConnectPool class.
//
//////////////////////////////////////////////////////////////////////

#include "DBConnectPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBConnectPool::CDBConnectPool()
{

}

CDBConnectPool::~CDBConnectPool()
{

}

CDBUtil* CDBConnectPool::GetConnectionPtr(LPCTSTR pcszDataSourceName)
{
	if (!pcszDataSourceName || (_tcslen(pcszDataSourceName) < 1))
		return NULL;

	for (vector<CDBPool*>::iterator vi = m_vConnectionPool.begin(); vi != m_vConnectionPool.end(); vi++)
	{
		CDBPool *pPool = *vi;
		if (pPool && pPool->Equal(pcszDataSourceName))
			return pPool->GetConnectionPtr(); 
	}

	return NULL;
}

BOOL CDBConnectPool::AddDataSource(LPCTSTR pcszDataSourceName, LPCTSTR pcszConStr, LPCTSTR pcszUserId, LPCTSTR pcszUserPwd)
{
	if (!pcszDataSourceName || (_tcslen(pcszDataSourceName) < 1))
		return FALSE;

	for (vector<CDBPool*>::iterator vi = m_vConnectionPool.begin(); vi != m_vConnectionPool.end(); vi++)
	{
		CDBPool *pPool = *vi;
		if (pPool && pPool->Equal(pcszDataSourceName))
			return FALSE; 
	}

	CDBPool *pPool = new CDBPool(pcszDataSourceName, pcszConStr, pcszUserId, pcszUserPwd);
	m_vConnectionPool.push_back(pPool);

	return TRUE;
}

BOOL CDBConnectPool::RemoveDataSource(LPCTSTR pcszDataSourceName)
{
	if (!pcszDataSourceName || (_tcslen(pcszDataSourceName) < 1))
		return FALSE;

	for (vector<CDBPool*>::iterator vi = m_vConnectionPool.begin(); vi != m_vConnectionPool.end(); vi++)
	{
		CDBPool *pPool = *vi;
		if (pPool && pPool->Equal(pcszDataSourceName))
		{
			delete pPool;
			m_vConnectionPool.erase(vi);
			return TRUE;
		}
	}

	return FALSE; 
}

CDBUtil* CDBPool::GetConnectionPtr()
{
	DWORD dwConnectionCount = 0;
	for (vector<CDBUtil*>::iterator vi = m_vConnections.begin(); vi != m_vConnections.end(); vi++)
	{
		CDBUtil *pDB = *vi;
		if (pDB)
		{
			dwConnectionCount++;

			if (!pDB->IsWorking())
				return pDB;
		}
	}

	// Create a new connection and append it to vector
	return (dwConnectionCount <= m_dwMaxConnections)?NewConnection():NULL;
}

CDBUtil* CDBPool::NewConnection()
{
	CDBUtil *pDB = new CDBUtil();
	if (pDB)
	{
		pDB->SetConnectStr(m_szConStr.c_str());
		pDB->SetUserName(m_szUserID.c_str());
		pDB->SetUserPassword(m_szPwd.c_str());
		if (pDB->ConnectToDB())
		{
			// Check if there is NULL pointer in vector
			for (vector<CDBUtil*>::iterator vi = m_vConnections.begin(); vi != m_vConnections.end(); vi++)
			{
				CDBUtil *p = *vi;
				if (!pDB)	// Found a NULL pointer, replace it
				{
					*vi = pDB;
					return pDB;
				}
			}

			// Not found, append to vector
			m_vConnections.push_back(pDB);
		}
		else
		{
			delete pDB;
			pDB = NULL;
		}
	}

	return pDB;
}
