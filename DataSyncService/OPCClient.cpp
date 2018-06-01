// OPCClient.cpp: implementation of the COPCClient class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Constants.h"
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

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
	m_pOPCItemDef->bActive = bActive;
	m_pOPCItemDef->vtRequestedDataType = vtRequestedDataType;

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
	m_hServer = 0;
	if (bInitItemMgtPtr)
	{
		m_pOPCItemDef = (OPCITEMDEF*)CoTaskMemAlloc(sizeof(OPCITEMDEF));
		m_pOPCItemDef->szAccessPath = NULL;
		m_pOPCItemDef->szItemID = NULL;
		m_pOPCItemDef->bActive = FALSE;
		m_pOPCItemDef->hClient = (OPCHANDLE)this;
		m_pOPCItemDef->dwBlobSize = 0;
		m_pOPCItemDef->pBlob = NULL;
		m_pOPCItemDef->vtRequestedDataType = VT_EMPTY;
		m_pOPCItemDef->wReserved = 0;
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

float GetFromVariant(VARIANT &vValue)
{
	switch (vValue.vt)
	{
	case VT_R4:
		return vValue.fltVal;
	case VT_R8:
		return (float)vValue.dblVal;
	case VT_I4:
	case VT_UI4:
		return (float)vValue.lVal;
	case VT_INT:
	case VT_UINT:
		return (float)vValue.intVal;
	case VT_BOOL:
		return (float)(vValue.boolVal ? 1 : 0);		
	case VT_I1:
	case VT_UI1:
		return (float)vValue.bVal;
	case VT_I2:
	case VT_UI2:
		return (float)vValue.iVal;
	case VT_DECIMAL:
		return (float)vValue.lVal;
	default:
		return 0.0f;
	}
}

INT COPCItemDef::UpdateData(CDBUtil *pDB, VARIANT vValue, WORD wQuality, FILETIME *pTimeStamp, map<LPCWSTR, LPWSTR, StrCompare> &mappings)
{
	if (!pTimeStamp || !pDB)
		return E_INVALIDARG;

	_bstr_t bstrVal;
	SYSTEMTIME utcTime;
	SYSTEMTIME localTime;
	
	LPCWSTR pItemID = m_pOPCItemDef->szItemID;
	float fVal = GetFromVariant(vValue);
	if (fVal < 0.0 || fVal > 99999999.00)
	{
		g_Logger.VLog(L"COPCItemDef::UpdateData() Skip the update value for item [%s] because the value [%.2f] is invalid.", pItemID, fVal);
		return -1;
	}

	FileTimeToSystemTime(pTimeStamp, &utcTime);
	SystemTimeToTzSpecificLocalTime(NULL, &utcTime, &localTime);

	const int MAX_LEN = 1000;
	wchar_t wszSQL[MAX_LEN];
	swprintf_s(wszSQL, sizeof(wszSQL) / sizeof(wszSQL[0]), L"Update ItemLatestStatus Set Val='%.2f', LastUpdate=Convert(datetime,'%d-%d-%d %d:%d:%d'),Quality=%d Where ItemID='%s'",
		fVal,
		localTime.wYear,
		localTime.wMonth,
		localTime.wDay,
		localTime.wHour,
		localTime.wMinute,
		localTime.wSecond,
		wQuality,
		mappings[pItemID]);		// Assume the element must be contained in m_ItemMappings

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
			swprintf_s(wszSQL, sizeof(wszSQL) / sizeof(wszSQL[0]), L"Insert Into ItemLatestStatus (ItemID,Val,LastUpdate,Quality) Values ('%s', '%.2f', Convert(datetime, '%d-%d-%d %d:%d:%d'), %d)",
				mappings[pItemID],
				fVal,
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
		// TODO: record the ItemId in log, or support the multiple byte string in log
		g_Logger.VForceLog(_T("COPCItemDef::UpdateData() Failed.\r\n%s.\r\n%s"), wszSQL, pDB->GetLastErrormsg());
		return nError;
	}
}

COPCClient::COPCClient(INT nMinGoodQuality) : m_nMinGoodQuality(nMinGoodQuality)
{
	m_bConnected = FALSE;
	m_pConnectionPointContainer = NULL;
	m_ppUnknown = NULL;
	m_pServer = NULL;
	m_pDataSink = NULL;
	m_pGroup = NULL;
	m_pDB = NULL;
	m_dwCookieDataSink20 = 0;

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

vector<LPWSTR> & COPCClient::GetOPCServerList(CATID catID)
{
	ClearVector(m_vOPCServerList);

	m_hLastHResult = E_FAIL;
	ICatInformation *pCat = NULL;
	__try
	{
		// Get component category manager:		
		if (S_OK != (m_hLastHResult = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_SERVER, IID_ICatInformation, (LPVOID*)&pCat)))
			throw _T("COPCClient::GetOPCServerList(): Failed to call CoCreateInstance for CLSID_StdComponentCategoriesMgr");

		// Enumerate registered components:
		IEnumCLSID *pEnum = NULL;
		CATID catIDs[1];
		catIDs[0] = catID;		// TODO we can add some more category items such as DA 1.0, DA 3.0
		if (S_OK != (m_hLastHResult = pCat->EnumClassesOfCategories(sizeof(catIDs) / sizeof(catIDs[0]), catIDs, 0, NULL, &pEnum)))
			throw _T("COPCClient::GetOPCServerList(): Failed to call EnumClassesOfCategories");

		GUID guid;
		ULONG ulFetched = 0;
		while (S_OK == (m_hLastHResult = pEnum->Next(1, &guid, &ulFetched)))
		{
			wchar_t *pProgID = NULL;
			if (S_OK != (m_hLastHResult = ProgIDFromCLSID(guid, &pProgID)))
			{
				if (pProgID)
					CoTaskMemFree(pProgID);

				throw _T("COPCClient::GetOPCServerList(): Failed to call ProgIDFromCLSID");
			}

			size_t nSize = wcslen(pProgID);
			LPWSTR pBuf = new WCHAR[nSize + 1];
			wcscpy_s(pBuf, nSize + 1, pProgID);
			pBuf[nSize] = L'\0';
			m_vOPCServerList.push_back(pBuf);

			// Release memory
			CoTaskMemFree(pProgID);
		}

		return m_vOPCServerList;
	}
	__finally
	{
		if (pCat)
			pCat->Release();
	}
}

IOPCServer * COPCClient::Connect(LPCOLESTR progID, COSERVERINFO *pCoServerInfo)
{
	if (!progID)
		return NULL;

	CLSID OPCCLSID;
	m_hLastHResult = E_FAIL;
	try
	{
		if (S_OK != (m_hLastHResult = CLSIDFromProgID(progID, &OPCCLSID)))
			throw _T("COPCClient::Connect(): Failed to call CLSIDFromProgID");

		MULTI_QI multiQIs[2];
		memset(multiQIs, 0, sizeof(multiQIs));
		multiQIs[0].pIID = &IID_IOPCServer;
		multiQIs[1].pIID = &IID_IConnectionPointContainer;
		if (S_OK != (m_hLastHResult = CoCreateInstanceEx(OPCCLSID,
			NULL,
			CLSCTX_SERVER,
			pCoServerInfo,
			sizeof(multiQIs) / sizeof(MULTI_QI),
			multiQIs)))
			throw _T("COPCClient::Connect(): Failed to call CoCreateInstanceEx for OPC Server");

		if ((S_OK != multiQIs[0].hr) || !multiQIs[0].pItf)
			throw _T("COPCClient::Connect(): Failed to get pointer to IOPCServer");

		m_pServer = (IOPCServer*)multiQIs[0].pItf;

		if ((S_OK != multiQIs[1].hr) || !multiQIs[1].pItf)
			throw _T("COPCClient::Connect(): Failed to get pointer to IConnectionPointContainer");

		m_pConnectionPointContainer = (IConnectionPointContainer*)multiQIs[1].pItf;

		m_bConnected = TRUE;

		return m_pServer;
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

		throw;
	}
}

void COPCClient::Disconnect()
{
	m_bConnected = FALSE;

	try
	{
		RemoveCallback();
	}
	catch (...) {}

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
	if (!pInfo)
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

	m_pGroup = bNoReleaseOutside ? pInfo : pInfo->Clone();
	m_hLastHResult = m_pServer->AddGroup(m_pGroup->wszName.c_str(),		// Name of the group. The name must be unique among the other groups created by this client. If no name (NULL), the server will generate a unique name.
		m_pGroup->bActive,				// If the Group is to be created as active or not
		m_pGroup->dwRequestedUpdateRate,// Client Specifies the fastest rate at which data changes may be sent to OnDataChange for items in this group. This also indicates the desired accuracy of Cached Data. This is intended only to control the behavior of the interface. How the server deals with the update rate and how often it actually polls the hardware internally is an implementation detail.  
										// Passing 0 indicates the server should use the fastest practical rate.  
										// The rate is specified in milliseconds.
		m_pGroup->hClientGroup,			// Client provided handle for this group. [refer to description of data types, parameters, and structures for more information about this parameter]
		&m_pGroup->lTimeBias,			// Pointer to Long containing the initial TimeBias (in minutes) for the Group.  
										// Pass a NULL Pointer if you wish the group to use the default system TimeBias. 
		(FLOAT*)&m_pGroup->fPercentDeadband,	// The percent change in an item value that will cause a subscription callback for that value to a client. 
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

	if (m_hLastHResult != S_OK)
	{
		delete m_pGroup;
		m_pGroup = NULL;

		CHECK_CONNECT(m_hLastHResult);
		throw _T("COPCClient::AddGroup(): Failed to call IOPCServer.AddGroup.");
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
	m_hLastHResult = m_pServer->CreateGroupEnumerator(dwScope, IID_IEnumString, (LPUNKNOWN*)&pEnum);
	if ((S_OK == m_hLastHResult) && pEnum)
	{
		LPOLESTR pStr;
		ULONG uFetched = 0;
		while (S_OK == (m_hLastHResult = pEnum->Next(1, &pStr, &uFetched)) && pStr)
		{
			vList.push_back(wstring(pStr));
			CoTaskMemFree(pStr);
		}

		pEnum->Release();
		pEnum = NULL;
	}

	return m_hLastHResult;
}

HRESULT COPCClient::RemoveGroup(LPGROUPINFO pGroup)
{
	if (!pGroup)
		return E_INVALIDARG;

	m_hLastHResult = E_FAIL;
	if (!pGroup->pOPCItemMgt)
		return m_hLastHResult;

	// Remove all items belong to this group first
	DWORD dwCount = m_vItems.size();
	if (dwCount > 0)
	{
		DWORD dwIndex = 0;
		OPCHANDLE *phServer = (OPCHANDLE *)CoTaskMemAlloc(sizeof(OPCHANDLE) * dwCount);
		HRESULT *pErrors = NULL;
		for (vector<COPCItemDef*>::iterator vi = m_vItems.begin(); vi != m_vItems.end(); vi++)
		{
			COPCItemDef *pItem = (COPCItemDef*)*vi;
			if (pItem)
			{
				phServer[dwIndex++] = pItem->m_hServer;
			}
		}

		if (S_OK != (m_hLastHResult = pGroup->pOPCItemMgt->RemoveItems(dwCount, phServer, &pErrors)))
		{
			g_Logger.VLog(_T("COPCClient::RemoveAllGroups(): Failed to call IOPCItemMgt.RemoveItems. HRESULT=%x"), m_hLastHResult);
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
	if (S_OK == (m_hLastHResult = m_pServer->RemoveGroup(pGroup->hServerGroup, TRUE)))
	{
		ClearVector(m_vItems);

		pGroup->pOPCItemMgt->Release();
		//m_Contents.erase(pGroup);
	}
	else
	{
		g_Logger.VLog(_T("COPCClient::RemoveAllGroups(): Failed to call IOPCServer.RemoveGroup. HRESULT=%x"), m_hLastHResult);
	}

	return m_hLastHResult;
}

void COPCClient::Clear()
{
	m_pDB = NULL;

	if (m_pDataSink)
	{
		m_pDataSink->Release();
		delete m_pDataSink;
		m_pDataSink = NULL;
	}

	Disconnect();

	ClearVector(m_vOPCServerList);
	ClearVector(m_vItems);
	ClearMap(m_ItemMappings, TRUE);
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

	ClearMap(m_ItemMappings, TRUE);
	TString szErrMsg;
	for (INT i = 0; i < nCount; i++)
	{
		LPITEMINFO pItemInfo = vList[i];
		if (!pItemInfo)
			continue;

		OPCITEMDEF *pItem = (OPCITEMDEF*)CoTaskMemAlloc(sizeof(OPCITEMDEF));

		LPWSTR pName = T2W(pItemInfo->pAddress);
		DWORD dwLen = wcslen(pName);
		pItem->szItemID = (LPWSTR)CoTaskMemAlloc((dwLen + 1) * sizeof(WCHAR));
		lstrcpynW(pItem->szItemID, pName, dwLen + 1);
		pItem->szAccessPath = NULL;
		pItem->bActive = TRUE;
		pItem->dwBlobSize = 0;
		pItem->pBlob = NULL;
		pItem->vtRequestedDataType = pItemInfo->vtRequestedDataType;
		pItem->wReserved = 0;

		if (S_OK == AddItem(new COPCItemDef(pItem), TRUE))
		{
			nRet++;

			// Add ItemId and Address into the mappings
			LPWSTR pAddr = new wchar_t[dwLen + 1];
			lstrcpynW(pAddr, pName, dwLen + 1);

			dwLen = wcslen(pItemInfo->pItemID);
			LPWSTR pItemId = new wchar_t[dwLen + 1];
			lstrcpynW(pItemId, T2W(pItemInfo->pItemID), dwLen + 1);
			m_ItemMappings.insert({ pAddr, pItemId });
		}
		else
		{
			szErrMsg.append(W2T(pItem->szItemID)).append(_T(","));
		}
	}

	if (szErrMsg.size() > 0)
	{
		g_Logger.VForceLog(_T("Failed to add monitor item(s): %s"), szErrMsg.c_str());
	}

	return nCount;
}

HRESULT COPCClient::AddItem(COPCItemDef *pItem, BOOL bNoReleaseOutside)
{
	if (!pItem)
		return E_INVALIDARG;

	if (!m_pGroup || !m_pGroup->pOPCItemMgt)
		return E_INVALID_GROUP_PTR;

	HRESULT hr = E_FAIL;
	COPCItemDef *pActItem = bNoReleaseOutside ? pItem : pItem->Clone();

	HRESULT *pErrors = NULL;
	OPCITEMRESULT *pResults = NULL;
	if (S_OK != (hr = m_pGroup->pOPCItemMgt->AddItems(1, pItem->m_pOPCItemDef, &pResults, &pErrors)) || !pResults)
	{
		g_Logger.VLog(_T("COPCClient::AddItem(): Failed to call IOPCItemMgt.AddItems on %s. HRESULT=%x"), W2CT(pItem->m_pOPCItemDef->szItemID), hr);
		if (!bNoReleaseOutside)
			delete pActItem;
	}
	else
	{
		// Add the pointer to item to vector
		pActItem->m_hServer = pResults[0].hServer;
		m_vItems.push_back(pActItem);

		// Add mapping of monitor item and address
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
	m_hLastHResult = E_FAIL;
	IConnectionPoint *pCP = NULL;
	if (FAILED(m_hLastHResult = m_pConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &pCP)))
	{
		m_dwCookieDataSink20 = 0;
		throw _T("COPCClient::AddCallback: Failed to call IConnectionPointContainer.FindConnectionPoint");
	}

	// If we succeeded to get connection point interface, create
	// our data sink interface and advise server of it:
	// Instantiate a new IKDataSink20:
	if (m_pDataSink)
	{
		m_pDataSink->Release();
		delete m_pDataSink;
	}
	m_pDataSink = new IDataSink20();

	// Add ourselves to its reference count:
	m_pDataSink->AddRef();

	// Advise the server of our data sink:
	if (S_OK != (m_hLastHResult = pCP->Advise(m_pDataSink, &m_dwCookieDataSink20)))
	{
		m_dwCookieDataSink20 = 0;
		pCP->Release();
		throw _T("COPCClient::AddCallback: Failed to call IConnectionPoint.Advise");
	}

	return m_dwCookieDataSink20;
}

void COPCClient::RemoveCallback()
{
	if ((0 == m_dwCookieDataSink20) || !m_pDataSink || !m_pConnectionPointContainer)
	{
		// Invalid, exit
		m_dwCookieDataSink20 = 0;
		if (m_pDataSink)
		{
			m_pDataSink->Release();
			delete m_pDataSink;
			m_pDataSink = NULL;
		}

		return;
	}

	m_hLastHResult = E_FAIL;
	IConnectionPoint *pCP = NULL;
	__try
	{
		if (S_OK != (m_hLastHResult = m_pConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback, &pCP)))
			throw _T("COPCClient::RemoveCallback(): Failed to call IConnectionPointContainer.FindConnectionPoint.");

		m_hLastHResult = pCP->Unadvise(m_dwCookieDataSink20);

		if (S_OK != m_hLastHResult)
			throw _T("COPCClient::RemoveCallback(): Failed to call IConnectionPoint.Unadvise.");
	}
	__finally
	{
		if (pCP)
			pCP->Release();

		// Release memory
		if (m_pDataSink)
		{
			m_pDataSink->Release();
			delete m_pDataSink;
			m_pDataSink = NULL;
		}
	}
}

BOOL COPCClient::IsQualityGood(OPCITEMSTATE &value)
{
	return (value.wQuality >= m_nMinGoodQuality);
}

INT COPCClient::ReadAndUpdateItemValue(const vector<COPCItemDef*> *pvList, BOOL bUpdateDB, OPCITEMSTATE *pState)
{
	if (!pvList || (!bUpdateDB && !pState))
		return E_INVALIDARG;

	if (!m_pGroup || !m_pGroup->pOPCItemMgt)
		return E_INVALID_GROUP_PTR;

	int nCount = 0;

	m_hLastHResult = E_FAIL;
	IOPCSyncIO *pISync = NULL;
	OPCITEMSTATE *pValues = NULL;
	HRESULT *pErrors = NULL;
	TCHAR buf[500];
	TString szFailedItems;
	TString szIgnoredItems;
	try
	{
		if (S_OK != (m_hLastHResult = m_pGroup->pOPCItemMgt->QueryInterface(IID_IOPCSyncIO, (void**)&pISync)))
		{
			CHECK_CONNECT(m_hLastHResult)
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
			
			if ((S_OK == (m_hLastHResult = pISync->Read(OPC_DS_CACHE, 1, &pItem->m_hServer, &pValues, &pErrors))) && pValues)
			{
				if (!IsQualityGood(pValues[0]))
				{
					TCHAR buf[100];
					CStrUtil::_Sprintf(buf, sizeof(buf) / sizeof(buf[0]), _T("%s/%d;"), W2CT(pItem->m_pOPCItemDef->szItemID), pValues[0].wQuality);
					szIgnoredItems.append(buf);
					// TODO: notfiy on UI?
					continue;
				}

				// TODO: May need to convert the readed value by pItem->pInConverter

				if (bUpdateDB)
				{
					int nAffectedRows = pItem->UpdateData(m_pDB, pValues[0].vDataValue, pValues[0].wQuality, &(pValues[0].ftTimeStamp), m_ItemMappings);
					if (nAffectedRows > 0)
					{
						nCount++;
					}
					else
					{
						g_Logger.VForceLog(_T("COPCClient::ReadAndUpdateItemValue() Failed to call COPCItemDef.Updata() on %s, return=%d,"), W2CT(pItem->m_pOPCItemDef->szItemID), nAffectedRows);
					}
				}
				else
				{
					nCount++;
				}
			}
			else
			{
				TString szErr1;
				CStrUtil::FormatMsg(NULL, szErr1, (INT)m_hLastHResult);
				TString szErr2;
				if (pErrors) CStrUtil::FormatMsg(NULL, szErr2, (INT)pErrors[0]);
				_stprintf_s(buf, sizeof(buf) / sizeof(buf[0]), _T("--%s; hr = %x %s Error = %x %s\r\n"), W2T(pItem->m_pOPCItemDef->szItemID), m_hLastHResult, szErr1.c_str(), pErrors ? pErrors[0] : 0, szErr2.c_str());
				szFailedItems.append(buf);
			}
			
			if (pValues)
			{
				CoTaskMemFree(pValues);
				pValues = NULL;
			}

			if (pErrors)
			{
				CoTaskMemFree(pErrors);
				pErrors = NULL;
			}

			CHECK_CONNECT(m_hLastHResult)
		}

		if (szFailedItems.size() > 0)
		{
			// TODO Suppres the log output if keep get the same error
			g_Logger.VForceLog(_T("COPCClient::ReadAndUpdateItemValue() Failed to call IOPCSyncIO.Read() on some monitor items:\r\n%s"), szFailedItems.c_str());
		}

		if (szIgnoredItems.size() > 0)
		{
			g_Logger.VForceLog(_T("COPCClient::ReadAndUpdateItemValue() The quality of following items are not good, ignore it:\r\n%s"), szIgnoredItems.c_str());
		}

		return nCount;
	}
	catch (...)
	{
		if (pValues)
		{
			CoTaskMemFree(pValues);
			pValues = NULL;
		}

		if (pErrors)
		{
			CoTaskMemFree(pErrors);
			pErrors = NULL;
		}

		if (pISync)
		{
			pISync->Release();
			pISync = NULL;
		}

		if (szFailedItems.size() > 0)
		{
			// TODO Suppres the log output if keep get the same error
			g_Logger.VForceLog(_T("COPCClient::ReadAndUpdateItemValue() Failed to call IOPCSyncIO.Read() on some monitor items:\r\n%s"), szFailedItems.c_str());
		}

		if (szIgnoredItems.size() > 0)
		{
			g_Logger.VForceLog(_T("COPCClient::ReadAndUpdateItemValue() The quality of following items are not good, ignore it:\r\n%s"), szIgnoredItems.c_str());
		}

		throw;
	}
}
