// OPCClient.h: interface for the COPCClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPCCLIENT_H__A8E88EB6_644E_4B53_BFAC_1D2EB71A2CCB__INCLUDED_)
#define AFX_OPCCLIENT_H__A8E88EB6_644E_4B53_BFAC_1D2EB71A2CCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>
#include <string>
#include <vector>
#include <map>
#include "DBUtil.h"

#include "opcda.h"
#include "opccomn.h"
#include "opcerror.h"
#include "datasink20.h"

using namespace std;

#define E_INVALID_GROUP_PTR			-9997
#define E_CONNECTION_BROKE			-9998
#define E_INVALID_DBPTR_FOR_GROUP	-9999
#define CHECK_CONNECT(hr) if ( (0x800706ba == hr) || (0x800706bf == hr) ) { m_bConnected=FALSE; m_hLastHResult=hr; throw E_CONNECTION_BROKE; }

typedef struct tagGroupInfo 
{
	wstring		wszName;
	BOOL		bActive;
	DWORD		dwRequestedUpdateRate;
	OPCHANDLE	hClientGroup;
	LONG		lTimeBias;
	double		fPercentDeadband;
	DWORD		dwLCID;
	OPCHANDLE	hServerGroup;
	DWORD		dwRevisedUpdateRate;
//	REFIID		riid;
	IOPCItemMgt*	pOPCItemMgt;

	tagGroupInfo()
	{
		Init();
	}

	tagGroupInfo(LPCWSTR pName):wszName(pName)
	{
		Init();
	}

	tagGroupInfo* Clone()
	{
		tagGroupInfo* pGroup = new tagGroupInfo();
		pGroup->wszName				= wszName;
		pGroup->bActive					= bActive;
		pGroup->dwRequestedUpdateRate	= dwRequestedUpdateRate;
		pGroup->hClientGroup			= (OPCHANDLE)pGroup;
		pGroup->lTimeBias				= lTimeBias;
		pGroup->fPercentDeadband		= fPercentDeadband;
		pGroup->dwLCID					= dwLCID;
		pGroup->hServerGroup			= hServerGroup;
		pGroup->pOPCItemMgt				= pOPCItemMgt;

		return pGroup;
	}

	void Init()
	{
		bActive					= TRUE;
		dwRequestedUpdateRate	= 500;
		hClientGroup			= (OPCHANDLE)this;
		lTimeBias				= 0;
		fPercentDeadband		= 0.0f;
		dwLCID					= 0;
		hServerGroup			= 0;	// Not defined
		dwRevisedUpdateRate		= 0;
		//riid					= IID_IOPCItemMgt;
		pOPCItemMgt				= NULL;
	}
} GROUPINFO, *LPGROUPINFO;

typedef struct tagItemInfo 
{
	LPTSTR	pItemID;
	LPTSTR	pAddress;
	LPTSTR	pDisplayName;
	VARTYPE vtRequestedDataType;
	BOOL	bNeedAccumulate;
	LPTSTR	pInConverter;
	LPTSTR	pOutConverter;
	TCHAR	chStatus;

	tagItemInfo()
	{
		pItemID = NULL;
		pAddress = NULL;
		pDisplayName = NULL;
		vtRequestedDataType = VT_I4;
		bNeedAccumulate = FALSE;
		pInConverter = NULL;
		pOutConverter = NULL;
		chStatus = _T('');
	}

	~tagItemInfo()
	{
		Clear();
	}

	void Clear()
	{
		if (pItemID)
		{
			delete pItemID;
			pItemID = NULL;
		}

		if (pAddress)
		{
			delete pAddress;
			pAddress = NULL;
		}

		if (pDisplayName)
		{
			delete pDisplayName;
			pDisplayName = NULL;
		}

		if (pInConverter)
		{
			delete pInConverter;
			pInConverter = NULL;
		}

		if (pOutConverter)
		{
			delete pOutConverter;
			pOutConverter = NULL;
		}
	}
}ITEMINFO, *LPITEMINFO;

class COPCItemDef
{ 
public:
	INT UpdateData(CDBUtil *pDB, VARIANT vValue, WORD wQuality, FILETIME *pTimeStamp);
	COPCItemDef* Clone();
	operator OPCITEMDEF*() { return m_pOPCItemDef; }
	COPCItemDef() { Init(TRUE); }
	COPCItemDef(OPCITEMDEF *pItem) ;
	COPCItemDef(LPCWSTR pAccessPath, LPCWSTR pItemID, BOOL bActive = TRUE, VARTYPE vtRequestedDataType = VT_EMPTY);

	~COPCItemDef() { Clear(); }

	void Clear();

public:
	OPCITEMDEF	*m_pOPCItemDef;
	OPCHANDLE	m_hServer;

private:
	void Init(BOOL bInitItemMgtPtr = TRUE);
};

class COPCClient  
{
public:
	HRESULT GetLastHResult() { return m_hLastHResult; }
	void SetDBPtr(CDBUtil *pDB) { m_pDB = pDB; }
	CDBUtil* GetDBPtr() { return m_pDB; }
	BOOL IsConnected() { return m_bConnected; }
	INT ReadAndUpdateItemValue(const vector<COPCItemDef*> *pvList, BOOL bUpdateDB = TRUE, OPCITEMSTATE *pState = NULL);
	vector<COPCItemDef*>* GetItems() { return &m_vItems; }
	LPGROUPINFO GetGroup() { return m_pGroup; }
	void RemoveCallback();
	DWORD AddCallback();
	INT AddItems(const vector<LPITEMINFO> &vList);
	HRESULT AddItem(COPCItemDef *pItem, BOOL bNoReleaseOutside = TRUE);
	HRESULT RemoveGroup(LPGROUPINFO pGroup);
	HRESULT EnumerateGroups(vector<wstring> &vList, OPCENUMSCOPE dwScope = OPC_ENUM_PRIVATE_CONNECTIONS);
	LPGROUPINFO AddGroup(LPGROUPINFO pInfo, BOOL bNoReleaseOutside = TRUE);
	void Disconnect();
	IOPCServer * Connect(LPCOLESTR progID, COSERVERINFO *pCoServerInfo = NULL);
	vector<LPWSTR> & GetOPCServerList(CATID catID);
	vector<LPWSTR> & CurrentOPCServerList() { return m_vOPCServerList; }

	COPCClient();
	virtual ~COPCClient();
	void Clear();

private:
	BOOL					m_bConnected;
	IConnectionPointContainer * m_pConnectionPointContainer;
	IOPCServer				*m_pServer;
	IUnknown				*m_ppUnknown;
	IDataSink20				*m_pDataSink;
	LPGROUPINFO				m_pGroup;

	DWORD					m_dwCookieDataSink20;
	vector<LPWSTR>			m_vOPCServerList;
	vector<COPCItemDef*>	m_vItems;
	CDBUtil					*m_pDB;
	HRESULT					m_hLastHResult;
};

#endif // !defined(AFX_OPCCLIENT_H__A8E88EB6_644E_4B53_BFAC_1D2EB71A2CCB__INCLUDED_)
