// OPCClient.cpp: implementation of the COPCClient class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "OPCClient.h"
#include "opcda_i.c"
#include "ContainerUtil.h"
#include "StrUtil.h"
#include "LogUtil.h"

#include <ComCat.h>
#include <atlconv.h>
#include <Shlwapi.h>
#include <objbase.h>

#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
 
extern CLogUtil g_Logger;

COPCItemDef::COPCItemDef(OPCITEMDEF *pItem) 
{ 
	m_hServer = 0; 
	m_pOPCItemDef = pItem;
	
	if (m_pOPCItemDef)
		m_pOPCItemDef->hClient = (OPCHANDLE)this;
}

COPCItemDef::COPCItemDef(LPCWSTR pAccessPath, LPCWSTR pItemID, BOOL bActive/*= TRUE*/, VARTYPE vtRequestedDataType/*= VT_EMPTY*/)
{
	Init(TRUE);
	m_pOPCItemDef->bActive				= bActive;
	m_pOPCItemDef->vtRequestedDataType	= vtRequestedDataType;
	
	if (pAccessPath)
	{
		int nLen = lstrlenW(pAccessPath);
		m_pOPCItemDef->szAccessPath = (LPWSTR)CoTaskMemAlloc((nLen + 1) * sizeof(wchar_t));
		lstrcpynW(m_pOPCItemDef->szAccessPath, pAccessPath, nLen + 1);
	}
	
	if (pItemID)
	{
		int nLen = lstrlenW(pItemID);
		m_pOPCItemDef->szItemID = (LPWSTR)CoTaskMemAlloc((nLen + 1) * sizeof(wchar_t));
		lstrcpynW(m_pOPCItemDef->szItemID, pItemID, nLen + 1);
	}
}

void COPCItemDef::Init(BOOL bInitItemMgtPtr)
{
	m_hServer					= 0;
	if (bInitItemMgtPtr)
	{
		m_pOPCItemDef = (OPCITEMDEF*)CoTaskMemAlloc(sizeof(OPCITEMDEF));
		m_pOPCItemDef->szAccessPath	= NULL;
		m_pOPCItemDef->szItemID		= NULL;
		m_pOPCItemDef->bActive		= FALSE;
		m_pOPCItemDef->hClient		= (OPCHANDLE)this;
		m_pOPCItemDef->dwBlobSize	= 0;
		m_pOPCItemDef->pBlob		= NULL;
		m_pOPCItemDef->vtRequestedDataType	= VT_EMPTY ;
		m_pOPCItemDef->wReserved	= 0;	
	}
	else
	{
		m_pOPCItemDef = NULL;
	}
}

void COPCItemDef::Clear()
{
	if (m_pOPCItemDef->szAccessPath)
	{
		CoTaskMemFree(m_pOPCItemDef->szAccessPath);
		m_pOPCItemDef->szAccessPath = NULL;
	}
	
	if (m_pOPCItemDef->szItemID)
	{
		CoTaskMemFree(m_pOPCItemDef->szItemID);
		m_pOPCItemDef->szItemID = NULL;
	}
	
	m_pOPCItemDef->dwBlobSize = 0;
	if (m_pOPCItemDef->pBlob)
	{
		delete m_pOPCItemDef->pBlob;
		m_pOPCItemDef->pBlob = NULL;
	}
	
	if (m_pOPCItemDef)
	{
		CoTaskMemFree(m_pOPCItemDef);
		m_pOPCItemDef = NULL;
	}

	Init(FALSE);
}

COPCItemDef* COPCItemDef::Clone()
{
	COPCItemDef *pItem = new COPCItemDef(this->m_pOPCItemDef->szAccessPath, 
										this->m_pOPCItemDef->szItemID, 
										this->m_pOPCItemDef->bActive,
										this->m_pOPCItemDef->vtRequestedDataType);

	return pItem;
}

INT COPCItemDef::UpdateData(CDBUtil *pDB, VARIANT vValue, WORD wQuality, FILETIME *pTimeStamp)
{
	if (!pTimeStamp || !pDB	)
		return E_INVALIDARG;

	_bstr_t bstrVal;
	SYSTEMTIME utcTime;
	SYSTEMTIME localTime;

	CStrUtil::Variant2BSTR(vValue, bstrVal);
	LPCWSTR pItemID = m_pOPCItemDef->szItemID;
	FileTimeToSystemTime(pTimeStamp, &utcTime);
	SystemTimeToTzSpecificLocalTime(NULL, &utcTime, &localTime);

	const int MAX_LEN = 1000;
	wchar_t wszSQL[MAX_LEN];
	swprintf_s(wszSQL, sizeof(wszSQL) / sizeof(wszSQL[0]), L"Update ItemLatestStatus Set Val='%s', LastUpdate=Convert(datetime,'%d-%d-%d %d:%d:%d'),Quality=%d Where ItemID='%s'", 
			(LPCWSTR)bstrVal, 
			localTime.wYear,
			localTime.wMonth,
			localTime.wDay,
			localTime.wHour,
			localTime.wMinute,
			localTime.wSecond,
			wQuality,
			pItemID);

	try
	{
		INT nRet = pDB->Execute(_bstr_t(wszSQL));
		if (nRet > 0)
		{
			// Update successfully.
			return nRet;
		}
		else	
		{
			// No record was affected, need to insert a new record
			swprintf_s(wszSQL, sizeof(wszSQL) / sizeof(wszSQL[0]), L"Insert Into ItemLatestStatus (ItemID,Val,LastUpdate,Quality) Values ('%s', '%s', Convert(datetime, '%d-%d-%d %d:%d:%d'), %d)",
				pItemID, 
				(LPCTSTR)bstrVal,
				localTime.wYear,
				localTime.wMonth,
				localTime.wDay,
				localTime.wHour,
				localTime.wMinute,
				localTime.wSecond,
				wQuality);
			
			return pDB->Execute(_bstr_t(wszSQL));
		}
	}
	catch (INT nError)
	{
		return nError;
	}
}

COPCClient::COPCClient()
{
	m_bConnected				= FALSE;
	m_pConnectionPointContainer	= NULL;
	m_ppUnknown					= NULL;
	m_pServer					= NULL;
	m_pDataSink					= NULL;
	m_pGroup					= NULL;
	m_pDB						= NULL;
	m_dwCookieDataSink20		= 0;

	if (FAILED(CoInitialize(NULL)))
		g_Logger.Log(_T("COPCClient::COPCClient(): Failed to call CoInitialze"));
}

COPCClient::~COPCClient()
{
	Clear();
	CoUninitialize();
}

static const CATID CATID_OPCDAServer10 = 
{ 0x63d5f430, 0xcfe4, 0x11d1, { 0xb2, 0xc8, 0x0, 0x60, 0x8, 0x3b, 0xa1, 0xfb } };
// {63D5F430-CFE4-11d1-B2C8-0060083BA1FB}

static const CATID CATID_OPCDAServer20 = 
{ 0x63d5f432, 0xcfe4, 0x11d1, { 0xb2, 0xc8, 0x0, 0x60, 0x8, 0x3b, 0xa1, 0xfb } };
// {63D5F432-CFE4-11d1-B2C8-0060083BA1FB}

vector<wstring> * COPCClient::GetOPCServerList(CATID catID)
{
	m_vOPCServerList.clear();

	HRESULT hr;
	try
	{
		// Get component category manager:
		ICatInformation *pCat = NULL;
		if (S_OK != (hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_SERVER, IID_ICatInformation, (LPVOID*)&pCat)))
			throw _T("COPCClient::GetOPCServerList(): Failed to call CoCreateInstance for CLSID_StdComponentCategoriesMgr");

		// Enumerate registered components:
		IEnumCLSID *pEnum = NULL;
		CATID catIDs[1];
		catIDs[0] = catID;		// TODO we can add some more category items such as DA 1.0, DA 3.0
		if (S_OK != (hr = pCat->EnumClassesOfCategories(sizeof(catIDs)/sizeof(catIDs[0]), catIDs, 0, NULL, &pEnum)))
			throw _T("COPCClient::GetOPCServerList(): Failed to call EnumClassesOfCategories");

		GUID guid;
		ULONG ulFetched = 0;
		while (S_OK == (hr = pEnum->Next(1, &guid, &ulFetched)))
		{
			wchar_t *wszProgID = NULL;
			if (S_OK != (hr = ProgIDFromCLSID(guid, &wszProgID)))
				throw _T("COPCClient::GetOPCServerList(): Failed to call ProgIDFromCLSID");

			m_vOPCServerList.push_back(wszProgID);

			// Release memory
			CoTaskMemFree(wszProgID);
		}
	}
	catch (LPCTSTR pMsg) 
	{
		g_Logger.VLog(_T("%s. HRESULT=%x"), pMsg, hr);
		return NULL;
	}
	catch (...) {
		g_Logger.VLog(_T("COPCClient::GetOPCServerList(): unspecfied error occurred. HRESULT=%x"), hr);
		return NULL;
	}

	return &m_vOPCServerList;
}

IOPCServer * COPCClient::Connect(LPCOLESTR progID, COSERVERINFO *pCoServerInfo)
{
	if (!progID)
		return NULL;

	CLSID OPCCLSID;
	HRESULT hr = E_FAIL;
	try
	{	
		if (S_OK != (hr = CLSIDFromProgID(progID, &OPCCLSID)))
			throw _T("COPCClient::Connect(): Failed to call CLSIDFromProgID");

		MULTI_QI multiQIs[2];
		memset(multiQIs, 0, sizeof(multiQIs));
		multiQIs[0].pIID = &IID_IOPCServer;
		multiQIs[1].pIID = &IID_IConnectionPointContainer;
		if (S_OK != (hr = CoCreateInstanceEx(OPCCLSID, 
											NULL, 
											CLSCTX_SERVER, 
											pCoServerInfo, 
											sizeof(multiQIs)/sizeof(MULTI_QI),
											multiQIs)))
 			throw _T("COPCClient::Connect(): Failed to call CoCreateInstanceEx for OPC Server");

		if ((S_OK != multiQIs[0].hr) || !multiQIs[0].pItf)
			throw _T("COPCClient::Connect(): Failed to get pointer to IOPCServer");

		m_pServer = (IOPCServer*)multiQIs[0].pItf;

		if ((S_OK != multiQIs[1].hr) || !multiQIs[1].pItf)
			throw _T("COPCClient::Connect(): Failed to get pointer to IConnectionPointContainer");

		m_pConnectionPointContainer = (IConnectionPointContainer*)multiQIs[1].pItf;

		m_bConnected = TRUE;
	}
	catch (LPCTSTR pMsg)
	{
		if (m_pConnectionPointContainer)
		{
			m_pConnectionPointContainer->Release();
			m_pConnectionPointContainer = NULL;
		}

		if (m_pServer)
		{
			m_pServer->Release();
			m_pServer = NULL;
		}

		if (m_ppUnknown)
		{
			m_ppUnknown->Release();
			m_ppUnknown = NULL;
		}

		g_Logger.VLog(_T("%s. HRESULT=%x"), pMsg, hr);
		return NULL;
	}
	catch (...) 
	{
		if (m_pConnectionPointContainer)
		{
			m_pConnectionPointContainer->Release();
			m_pConnectionPointContainer = NULL;
		}

		if (m_pServer)
		{
			m_pServer->Release();
			m_pServer = NULL;
		}

		if (m_ppUnknown)
		{
			m_ppUnknown->Release();
			m_ppUnknown = NULL;
		}
		g_Logger.VLog(_T("COPCClient::Connect(): unspecfied error occurred. HRESULT=%x"), hr);
		return NULL;
	}

	return m_pServer;
}

void COPCClient::Disconnect()
{
	m_bConnected = FALSE;

	if (!RemoveCallback())
	{
		// Sometimes the callback cannot be removed, so need to release memeory by hand
		if (m_pDataSink)
		{
			m_pDataSink->Release();
			delete m_pDataSink;
			m_pDataSink = NULL;
		}
	}

	if (m_pConnectionPointContainer)
	{
		m_pConnectionPointContainer->Release();
		m_pConnectionPointContainer = NULL;
	}

	if (m_pServer)
	{		
//		RemoveAllGroups();
		
		m_pServer->Release();
		m_pServer = NULL;
	}

	if (m_pGroup)
	{
		delete m_pGroup;
		m_pGroup = NULL;
	}

	if (m_ppUnknown)
	{
		m_ppUnknown->Release();
		m_ppUnknown = NULL;
	}
}

LPGROUPINFO COPCClient::AddGroup(LPGROUPINFO pInfo, BOOL bNoReleaseOutside) 
{
	if(!pInfo)
		return NULL;

	// TODO Check if the group name is duplicated or not
// 	if (FindGroupInMap(pInfo->wszName.c_str()))
// 		return NULL;

	if (m_pGroup)
	{
		// Remove group
		delete m_pGroup;
		m_pGroup = NULL;
	}
	
	m_pGroup = bNoReleaseOutside?pInfo:pInfo->Clone();
	HRESULT hr = m_pServer->AddGroup(m_pGroup->wszName.c_str(),		// Name of the group. The name must be unique among the other groups created by this client. If no name (NULL), the server will generate a unique name.
									m_pGroup->bActive,				// If the Group is to be created as active or not
									m_pGroup->dwRequestedUpdateRate,// Client Specifies the fastest rate at which data changes may be sent to OnDataChange for items in this group. This also indicates the desired accuracy of Cached Data. This is intended only to control the behavior of the interface. How the server deals with the update rate and how often it actually polls the hardware internally is an implementation detail.  
																	// Passing 0 indicates the server should use the fastest practical rate.  
																	// The rate is specified in milliseconds.
									m_pGroup->hClientGroup,			// Client provided handle for this group. [refer to description of data types, parameters, and structures for more information about this parameter]
									&m_pGroup->lTimeBias,			// Pointer to Long containing the initial TimeBias (in minutes) for the Group.  
																	// Pass a NULL Pointer if you wish the group to use the default system TimeBias. 
									&m_pGroup->fPercentDeadband,	// The percent change in an item value that will cause a subscription callback for that value to a client. 
																	// This parameter only applies to items in the group that have dwEUType of Analog. 
																	// A NULL pointer is equivalent to 0.0.
									m_pGroup->dwLCID,				// The language to be used by the server when returning values (including EU enumeration's) as text for operations on this group.  
																	// This could also include such things as alarm or status conditions or digital contact states.
									&m_pGroup->hServerGroup,		// Place to store the unique server generated handle to the newly created group. The client will use the server provided handle for many of the subsequent functions that the client requests the server to perform on the group.
									&m_pGroup->dwRevisedUpdateRate,	// The server returns the value it will actually use for the UpdateRate which may differ from the RequestedUpdateRate.
																	// Note that this may also be slower than the rate at which the server is internally obtaining the data and updating the cache. In general the server should 'round up' the requested rate to the next available supported rate. 
																	// The rate is specified in milliseconds.  
																	// Server returns HRESULT of OPC_S_UNSUPPORTEDRATE  when it returns a value in revisedUpdateRate that is different than RequestedUpdateRate
									IID_IOPCItemMgt,//pInfo->riid,	// The type of interface desired (e.g. IID_IOPCItemMgt)
									(LPUNKNOWN*)&(m_pGroup->pOPCItemMgt)// Where to store the returned interface pointer. NULL is returned for any FAILED HRESULT.
								);

	if (S_OK == hr)
	{
		//
	}
	else
	{
		delete m_pGroup;
		m_pGroup = NULL;
		g_Logger.VLog(_T("COPCClient::AddGroup(): Failed to call IOPCServer.AddGroup. HRESULT=%x"), hr);

		CHECK_CONNECT(hr);
	}

	return m_pGroup;
}

HRESULT COPCClient::EnumerateGroups(vector<wstring> &vList, OPCENUMSCOPE dwScope)
{
	if (!m_pServer)
		return -1;

	USES_CONVERSION;
	vList.clear();
	IEnumString *pEnum = NULL;
	HRESULT hr = m_pServer->CreateGroupEnumerator(dwScope, IID_IEnumString, (LPUNKNOWN*)&pEnum);
	if ((S_OK == hr) && pEnum)
	{
		LPOLESTR pStr;
		ULONG uFetched = 0;
		while (S_OK == (hr = pEnum->Next(1, &pStr, &uFetched)) && pStr)
		{
			vList.push_back(wstring(pStr));
			CoTaskMemFree(pStr);
		}

		pEnum->Release();
		pEnum = NULL;
	}

	return hr;
}

HRESULT COPCClient::RemoveGroup(LPGROUPINFO pGroup)
{
	if (!pGroup)
		return E_INVALIDARG;

	HRESULT hr = E_FAIL;
	if (pGroup->pOPCItemMgt)
	{
		// Remove all items belong to this group first
		DWORD dwCount = m_vItems.size();
		if (dwCount > 0)
		{
			DWORD dwIndex = 0;
			OPCHANDLE *phServer = (OPCHANDLE *)CoTaskMemAlloc(sizeof(OPCHANDLE) * dwCount);
			HRESULT *pErrors	= NULL;
			for (vector<COPCItemDef*>::iterator vi = m_vItems.begin(); vi != m_vItems.end(); vi++)
			{
				COPCItemDef *pItem = (COPCItemDef*)*vi;
				if (pItem)
				{
					phServer[dwIndex++] = pItem->m_hServer;
				}
			}
			
			if (S_OK != (hr = pGroup->pOPCItemMgt->RemoveItems(dwCount, phServer, &pErrors)))
			{
				g_Logger.VLog(_T("COPCClient::RemoveAllGroups(): Failed to call IOPCItemMgt.RemoveItems. HRESULT=%x"), hr);
			}
			else
			{
				for (DWORD i = 0; i < dwCount; i++)
				{
					if (FAILED(pErrors[i]))
					{
						g_Logger.VLog(_T("COPCClient::RemoveAllGroups(): IOPCItemMgt.RemoveItems() succeed, but the item #%d cannot be removed. HRESULT=%x"), i, pErrors[i]);
					}
				}
			}
			
			CoTaskMemFree(phServer);
			CoTaskMemFree(pErrors);
		}		// end if (dwCount > 0)				
		
		// Then remove group
		if (S_OK == (hr = m_pServer->RemoveGroup(pGroup->hServerGroup, TRUE)))
		{
			ClearVector(m_vItems);
			
			pGroup->pOPCItemMgt->Release();
			//m_Contents.erase(pGroup);
		}
		else
		{
			g_Logger.VLog(_T("COPCClient::RemoveAllGroups(): Failed to call IOPCServer.RemoveGroup. HRESULT=%x"), hr);
		}
	}		// end if (pGroup && pGroup->pOPCItemMgt)
/*
	else						// Remove all private groups belong to this client
	{
		vector<wstring> vList;
		hr = EnumerateGroups(vList, OPC_ENUM_PRIVATE_CONNECTIONS);
		if (SUCCEEDED(hr))
		{		
			if (vList.size() > 0)
			{
				for (int i = 0; i < vList.size(); i++)
				{
					IOPCGroupStateMgt *pIGroupStateMgt = NULL;
					if (S_OK == (hr = m_pServer->GetGroupByName(vList[i].c_str(), IID_IOPCGroupStateMgt, (LPUNKNOWN*)&pIGroupStateMgt)) && pIGroupStateMgt)
					{
						GROUPINFO group;
						LPWSTR pszName = NULL;
						if (S_OK == (hr = pIGroupStateMgt->GetState(&group.dwRequestedUpdateRate,
																	&group.bActive, 
																	&pszName, 
																	&group.lTimeBias, 
																	&group.fPercentDeadband, 
																	&group.dwLCID, 
																	&group.hClientGroup, 
																	&group.hServerGroup)))
						{
							// TODO Remove all items belong to this group first

							if (S_OK != (hr = m_pServer->RemoveGroup(group.hServerGroup, TRUE)))
							{
								g_Logger.VLog(_T("COPCClient::RemoveAllGroups(): Failed to call IOPCServer.RemoveGroup. HRESULT=%x"), hr);
							}
							
							CoTaskMemFree(pszName);
						}
						
						pIGroupStateMgt->Release();
					}			
				}	// end for

				Clear();		// Clear the vector m_vGroups since we already removed all private groups belong to this client
			}	// end if (vList.size() > 0)
		}	// end if (SUCCEEDED(hr))
	}
*/
	return hr;
}

void COPCClient::Clear()
{
	m_pDB = NULL;
	Disconnect();

	ClearVector(m_vItems);
}

INT COPCClient::AddItems(const vector<LPITEMINFO> &vList)
{
	if (!m_pGroup)
		return E_INVALID_GROUP_PTR;

	INT nCount = vList.size();
	if (nCount < 1)
		return 0;

	USES_CONVERSION;

	int nRet = 0;
	HRESULT hr = E_FAIL;
	HRESULT *pErrors = NULL;
	OPCITEMRESULT *pResults = NULL;	
	for (INT i = 0; i < nCount; i++)
	{
		LPITEMINFO pItemInfo = vList[i];
		if (!pItemInfo)
			continue;

		OPCITEMDEF *pItem = (OPCITEMDEF*)CoTaskMemAlloc(sizeof(OPCITEMDEF));
		
		LPWSTR pName = T2W(pItemInfo->pItemID);
		DWORD dwLen = wcslen(pName);
		pItem->szItemID				= (LPWSTR)CoTaskMemAlloc((dwLen + 1) * sizeof (WCHAR));
		lstrcpynW(pItem->szItemID, pName, dwLen + 1);
		pItem->szAccessPath			= NULL;
		pItem->bActive				= TRUE;
		pItem->dwBlobSize			= 0;
		pItem->pBlob				= NULL;
		pItem->vtRequestedDataType	= pItemInfo->vtRequestedDataType;
		pItem->wReserved				= 0;

		if (S_OK == AddItem(new COPCItemDef(pItem), TRUE))
			nRet++;
	}

	return nCount;
/*
	hr = pGroup->pOPCItemMgt->AddItems(nCount, pItemArray, &pResults, &pErrors);
	if (S_OK == hr)				// The operation succeeded, all items were added.
	{
		g_Logger.VLog(L"COPCClient::AddItems(): Successfully added %d items to group [%s].", nCount, pGroup->wszName.c_str());
	}
	else if (S_FALSE == hr)		// The operation completed with one or more errors. Refer to individual error returns for failure analysis.
	{
	}
	else
	{
		nRet = -1;
		for (int i = 0; i < nCount; i++)
		{
			delete ppItemDefs[i];
			ppItemDefs[i] = NULL;
		}

		delete ppItemDefs;

		g_Logger.VForceLog(_T("COPCClient::AddItems(): Failed to call IOPCItemMgt.AddItems. HRESULT=%x"), hr);
	}

	if ( (S_OK == hr) || (S_FALSE == hr) )
	{
		// Got the pointer to vector in m_Contents
		vector<COPCItemDef*> *pvItems = NULL;
		OPCContentsMap::const_iterator mi = m_Contents.find(pGroup);
		if (mi != m_Contents.end())
		{
			pvItems = (vector<COPCItemDef*>*)mi->second;
		}

		if (!pvItems)
		{
			pvItems = new vector<COPCItemDef*>();
			m_Contents[pGroup] = pvItems;
		}

		for (int i = 0; i < nCount; i++)
		{
			if (pErrors && SUCCEEDED(pErrors[i]))
			{
				nCount++;
				ppItemDefs[i]->SetGroup(pGroup);
				pvItems->push_back(ppItemDefs[i]);
			}
			else
			{
				// We will release the memory for that item
				// Needn't to release pItemArry because COPCItemDef will handle that
				delete ppItemDefs[i];
				ppItemDefs[i] = NULL;
				g_Logger.VForceLog(L"Failed to add item [%s] to group [%s]", pItemArray[i].szItemID, pGroup->wszName.c_str());
			}
		}
	}

	if (pResults)
		CoTaskMemFree (pResults);
	
	if (pErrors)
		CoTaskMemFree (pErrors);

	return nRet;
*/
}

HRESULT COPCClient::AddItem(COPCItemDef *pItem, BOOL bNoReleaseOutside)
{
	if (!pItem)
		return E_INVALIDARG;

	if (!m_pGroup || !m_pGroup->pOPCItemMgt)
		return E_INVALID_GROUP_PTR;

	HRESULT hr = E_FAIL;
	COPCItemDef *pActItem = bNoReleaseOutside?pItem:pItem->Clone();

	HRESULT *pErrors = NULL;
	OPCITEMRESULT *pResults = NULL;
	if (S_OK != (hr = m_pGroup->pOPCItemMgt->AddItems(1, pItem->m_pOPCItemDef, &pResults, &pErrors)) || !pResults)
	{
		g_Logger.VLog(_T("COPCClient::AddItem(): Failed to call IOPCItemMgt.AddItems. HRESULT=%x"), hr);
		if (!bNoReleaseOutside)	
			delete pActItem; 
	}
	else
	{
		// Add the pointer to item to vector
		pActItem->m_hServer = pResults[0].hServer;
		m_vItems.push_back(pActItem);
	}

	if (pResults)
		CoTaskMemFree(pResults);

	if (pErrors)
		CoTaskMemFree(pErrors);

	CHECK_CONNECT(hr)

	return hr;
}

DWORD COPCClient::AddCallback()
{
	if (!m_pConnectionPointContainer)
		return 0;

	if (m_dwCookieDataSink20 > 0)
		RemoveCallback();
	
	// Get connection point (IID_IOPCDataCallback interface):
	HRESULT hr = E_FAIL;
	IConnectionPoint *pCP = NULL;
	try
	{
		//if (FAILED(hr = m_pConnectionPointContainer->FindConnectionPoint (IID_IOPCDataCallback, &pCP)))
			throw _T("COPCClient::AddCallback: Failed to call IConnectionPointContainer.FindConnectionPoint");
		
		// If we succeeded to get connection point interface, create
		// our data sink interface and advise server of it:
		// Instantiate a new IKDataSink20:
		if (!m_pDataSink)
		{
			m_pDataSink->Release();
			delete m_pDataSink;
		}
		
		m_pDataSink = new IDataSink20();
		
		// Add ourselves to its reference count:
		m_pDataSink->AddRef ();
		
		// Advise the server of our data sink:
		if (S_OK != (hr = pCP->Advise(m_pDataSink, &m_dwCookieDataSink20)))
		{
			throw _T("COPCClient::AddCallback: Failed to call IConnectionPoint.Advise");
		}
		
		// We are done with the IID_IOPCDataCallback, so release
		// (remove us from its reference count):
		pCP->Release ();
	}
	catch (LPCTSTR pMsg)
	{
		g_Logger.VLog(_T("%s. HRESULT=%x"), pMsg, hr);
		m_dwCookieDataSink20 = 0;
		if (m_pDataSink)
		{
			m_pDataSink->Release();
			delete m_pDataSink;
			m_pDataSink = NULL;
		}
	}
	catch (...) 
	{
		g_Logger.VLog(_T("COPCClient::Connect(): unspecfied error occurred. HRESULT=%x"), hr);
		m_dwCookieDataSink20 = 0;
		if (m_pDataSink)
		{
			m_pDataSink->Release();
			delete m_pDataSink;
			m_pDataSink = NULL;
		}
	}

	return m_dwCookieDataSink20;
}

BOOL COPCClient::RemoveCallback()
{
	if ( (0 == m_dwCookieDataSink20) || !m_pDataSink || !m_pConnectionPointContainer)
	{
		// Invalid, exit
		m_dwCookieDataSink20 = 0;
		if (m_pDataSink)
		{
			m_pDataSink->Release();
			delete m_pDataSink;
			m_pDataSink = NULL;
		}

		return TRUE;
	}

	HRESULT hr = E_FAIL;
	IConnectionPoint *pCP = NULL;
	try
	{
		if (S_OK != (hr = m_pConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &pCP)))
			throw _T("COPCClient::RemoveCallback(): Failed to call IConnectionPointContainer.FindConnectionPoint");

		hr = pCP->Unadvise(m_dwCookieDataSink20);
		pCP->Release();
		if (S_OK != hr)
			throw _T("COPCClient::RemoveCallback(): Failed to call IConnectionPoint.Unadvise");

		// Release memory
		if (m_pDataSink)
		{
			m_pDataSink->Release();
			delete m_pDataSink;
			m_pDataSink = NULL;
		}
	}
	catch (LPCTSTR pMsg)
	{
		g_Logger.VLog(_T("%s. HRESULT=%x"), pMsg, hr);
		return FALSE;
	}
	catch (...) 
	{
		g_Logger.VLog(_T("COPCClient::RemoveCallback(): unspecfied error occurred. HRESULT=%x"), hr);
		return FALSE;
	}

	return TRUE;
}

INT COPCClient::ReadAndUpdateItemValue(const vector<COPCItemDef*> *pvList, BOOL bUpdateDB, OPCITEMSTATE *pState)
{
	if (!pvList || (!bUpdateDB && !pState))
		return E_INVALIDARG;

	if (!m_pGroup || !m_pGroup->pOPCItemMgt)
		return E_INVALID_GROUP_PTR;

	int nCount = 0;

	HRESULT hr = NULL;
	IOPCSyncIO *pISync = NULL;
	if (S_OK != (hr = m_pGroup->pOPCItemMgt->QueryInterface(IID_IOPCSyncIO, (void**)&pISync)))
	{
		CHECK_CONNECT(hr)
		throw _T("COPCClient::ReadAndUpdateItemValue(): Failed to call IOPCItemMgt.QueryInterface for IID_IOPCSyncIO");
	}
	
	for (vector<COPCItemDef*>::const_iterator vi = pvList->begin(); vi != pvList->end(); vi++)
	{
		COPCItemDef *pItem = (COPCItemDef*)*vi;
		if (!pItem)
		{
			g_Logger.ForceLog(_T("COPCClient::ReadAndUpdateItemValue() Failed to get pointer to COPCItemDef from vector."));
			continue;
		}

		OPCITEMSTATE *pValues	= NULL;
		HRESULT *pErrors = NULL;
		if (S_OK == (hr = pISync->Read(OPC_DS_CACHE, 1, &pItem->m_hServer, &pValues, &pErrors)))
		{
			// TODO: May need to convert the readed value by pItem->pInConverter

			if (bUpdateDB)
			{
				int nAffectedRows = pItem->UpdateData(m_pDB, pValues[0].vDataValue, pValues[0].wQuality, &(pValues[0].ftTimeStamp));
				if (nAffectedRows > 0)
				{
					nCount++;
				}
				else
				{
					g_Logger.VForceLog(_T("COPCClient::ReadAndUpdateItemValue() Failed to call COPCItemDef.Updata(), return=%d,"), nAffectedRows);
				}
			}
			else
			{
				nCount++;
			}
		}
		else
		{
			g_Logger.VForceLog(_T("COPCClient::ReadAndUpdateItemValue() Failed to call IOPCSyncIO.Read(), hr=%x, error=%x"), hr, pErrors?pErrors[0]:0);
		}

		if (pValues)
			CoTaskMemFree (pValues);	

		if (pErrors)
			CoTaskMemFree (pErrors);

		CHECK_CONNECT(hr)
	}
	
	pISync->Release();
	pISync = NULL;

	return nCount;
}
