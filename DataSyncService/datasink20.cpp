// **************************************************************************
// datasink20.cpp
//
// Description:
//	Implements the IDataSink20 class and IOPCDataCallback COM interface.
//	Groups in OPC Servers can use the IOPCDataCallback interface to advise us
//	of data change events.
//
//	OPC 2.0 requires that all member functions of the IOPCDataCallback
//	interface be implemented: OnDataChange, OnReadComplete, OnWriteComplete,
//	and OnCancelComplete.
//
//	The IOPCDataCallback interface is used by OPC version 2.0.  OPC version
//	1.0 uses the IAdviseSink interface instead.  If you know that your
//	server(s) will only be using OPC version 1.0, you don't really need this.
//
// **************************************************************************


#include "stdafx.h"
#include "datasink20.h"
#include "OPCClient.h"
#include "LogUtil.h"

#include <atlconv.h>

extern CLogUtil g_Logger;

// **************************************************************************
// IKDataSink20 ()
//
// Description:
//	Constructor.  Reference count is initialized to zero.
//
// Parameters:
//	none
//
// Returns:
//	none
// **************************************************************************
IDataSink20::IDataSink20 () : m_cnRef (0)
{
}

// **************************************************************************
// ~IKDataSink20 ()
//
// Description:
//	Destructor
//
// Parameters:
//	none
//
// Returns:
//	none
// **************************************************************************
IDataSink20::~IDataSink20 ()
{
}

// **************************************************************************
// AddRef ()
//
// Description:
//	This function is called to increment our reference count.  Caller should 
//	increment our reference count each time a new pointer to this interface
//	is created (except when obtained by a call to QueryInterface which will
//  bump the reference count on its behalf). 
//
//	This is one of the 3 member functions all COM interfaces must implement. 
//
// Parameters:
//	none
//
// Returns:
//	STDMETHODIMP_(ULONG) - Reference count resulting from this call.
// **************************************************************************
STDMETHODIMP_(ULONG) IDataSink20::AddRef ()
{
	// Increment the reference count then return value:
	return (++m_cnRef);
}

// **************************************************************************
// Release ()
//
// Description:
//	This function is called to decrement our reference count.  Caller should
//	decrement our reference count just before each pointer to this interface 
//	is destroyed (goes out of scope).  Standard COM practice requires us to
//  self-delete once the refernce count returns to zero.
//
//	This is one of the 3 member functions all COM interfaces must implement.
//
// Parameters:
//	none
//
// Returns:
//	STDMETHODIMP_(ULONG) - Reference count resulting from this call.
// **************************************************************************
STDMETHODIMP_(ULONG) IDataSink20::Release ()
{
	// Decrement reference count and return immediately if not zero:
	if (--m_cnRef != 0)
		return (m_cnRef);
	
	// If we make it here, then the reference count is zero.  We are 
	// therefore obliged to delete ourselves:
	delete this;
	
	// Return our refence count, which is zero now:
	return (0);
}

// **************************************************************************
// QueryInterface ()
//
// Description:
//	This function is called to obtain a pointer to one of the COM interfaces
//	objects of this class support (in this case only IAdviseSink and its base
//	class IUnknown).  As is standard COM practice, our reference count is 
//	incremented upon a successful query.  
//
//  This is one of the 3 member functions all COM interfaces must implement.	
//
// Parameters:
//	REFIID		riid			Requested interface type: IID_IUnknown,
//								  or IID_IAdviseSink.
//	LPVOID		*ppInterface	Pointer to requested interface.
//
// Returns:
//	STDMETHODIMP -
//		S_OK - Query successful, ppInterface set to requested pointer.
//		E_INVALIDARG - One of the arguments was invalid.
//		E_NOINTERFACE - Requested interface is not supported.
// **************************************************************************
STDMETHODIMP IDataSink20::QueryInterface (REFIID iid, LPVOID *ppInterface)
{
	// Validate ppInterface.  Return with "invalid argument" error code if invalid:
	if (ppInterface == NULL)
		return (E_INVALIDARG);
	
	// Standard COM practice requires that we invalidate output arguments
	// if an error is encountered.  Let's assume an error for now and invalidate
	// ppInterface.  We will reset it to a valid interface pointer later if we
	// determine requested ID is valid:
	*ppInterface = NULL;	
	
	// Reset ppInterface if requested interface type is valid:
	if (iid == IID_IUnknown)
		*ppInterface = (IUnknown *) this;
	else if (iid == IID_IOPCDataCallback)
		*ppInterface = (IOPCDataCallback *) this;
	else
	{
		// We have been asked for an interface we don't support.  Return 
		// immediately with "no interface" error code.  ppInterface should
		// still be NULL as required by COM.
		return (E_NOINTERFACE);
	}
	
	// If we made it here, then the query was successful and ppInterface
	// has been set to requested interface pointer.  Standard COM practice
	// requires us to increment our reference count now.
	AddRef ();
	
	// Return with "success" code:
	return (S_OK);
}

// **************************************************************************
// OnDataChange ()
//
// Description:
//	This method is provided to handle notifications from an OPC Group for
//	exception based (unsolicited) data changes and refreshes.  Data for one
//	or possibly more active items in the group will be provided.
//
// Parameters:
//	DWORD		dwTransID			Zero for normal OnDataChange events, 
//                                    non-zero for Refreshes.
//	OPCHANDLE	hGroup				Client group handle.
//	HRESULT		hrMasterQuality		S_OK if all qualities are GOOD, otherwise S_FALSE.
//	HRESULT		hrMasterError		S_OK if all errors are S_OK, otherwise S_FALSE.
//	DWORD		dwCount				Number of items in the lists that follow.
//	OPCHANDLE	*phClientItems		Item client handles.
//	VARIANT		*pvValues			Item data.
//	WORD		*pwQualities		Item qualities.
//	FILETIME	*pftTimeStamps		Item timestamps.
//	HRESULT		*pErrors			Item errors.
//
// Returns:
//	STDMETHODIMP - 
//		S_OK - Processing of advisement successful.
//		E_INVALIDARG - One of the arguments was invalid.
// **************************************************************************
STDMETHODIMP IDataSink20::OnDataChange (DWORD dwTransID,
						OPCHANDLE hGroup,
						HRESULT hrMasterQuality,
						HRESULT hrMasterError,
						DWORD dwCount,
						OPCHANDLE *phClientItems,
						VARIANT *pvValues,
						WORD *pwQualities,
						FILETIME *pftTimeStamps,
						HRESULT *pErrors)
{
	USES_CONVERSION;
	LPGROUPINFO pGroup = NULL;
	COPCItemDef *pItem = NULL;

	// Initialize a bad item handle counter (used for debugging only):
	DWORD cdwBadItemHandles = 0;

	// Validate arguments.  Return with "invalid argument" error code 
	// if any are invalid:
	if (hGroup					== NULL	||
		dwCount					== 0	||
		phClientItems			== NULL	||
		pvValues				== NULL	||
		pwQualities				== NULL	||
		pftTimeStamps			== NULL	||
		pErrors					== NULL)
		return (E_INVALIDARG);

	// The group argument gives us a pointer to the destination CKGroup:
	pGroup = (LPGROUPINFO) hGroup;

	// Post a "read complete" message for refresh transactions (which will 
	// have a non-zero transaction ID).  No need to load up event log with 
	// "read complete" messages for unsolicited data updates, which could be
	// coming in many times per second.
	if (dwTransID != 0)
	{
		//LogMsg (IDS_ASYNC20_READXACT_COMPLETE, dwTransID, dwCount,
		//		pGroup->GetName (), hrMasterError);
	}

	// Loop over items:
	for (DWORD dwItem = 0; dwItem < dwCount; dwItem++)
	{
		// The client item handles give us pointers to the destination
		// COPCItemDef objects:
		pItem = (COPCItemDef*) phClientItems [dwItem];
		
		// Wrap use of pItem in exception handler in case pointer is bad:
		try
		{
			// Update the item's value, quality, and timestamp:
//			pItem->UpdateData(pvValues[dwItem], pwQualities[dwItem], &pftTimeStamps[dwItem]);
		}
		catch(INT nCode)
		{			
			// The DB execution throw a exception
			switch(nCode)
			{
			case E_INVALIDARG:
			case OPC_E_UNKNOWNITEMID:
			case E_INVALID_DBPTR_FOR_GROUP:
				g_Logger.VForceLog(_T("IDataSink20::OnDataChange: Failed to call COPCItemDef.UpdateDate. Code=%d"), nCode);
				break;
			default:
				;
//				g_Logger.VLog(_T("IDataSink20::OnDataChange: Failed to call COPCItemDef.UpdateDate. Code=%d, Message=%s"), pItem->GetLastErrorCode(), pItem->GetLastErrormsg());
			}
			continue;
		}
		catch (...)
		{
			//m_bExecuting = FALSE;
			//throw SetLastErrorCode((-(LONG)GetLastError()));
			// Must have tried to use a bad pItem pointer.  Bump a bad item 
			// handle count and try to process then next item:
			cdwBadItemHandles++;
			g_Logger.VLog(_T("IDataSink20::OnDataChange: pointer to item [%] is invalid. Total %d invalid pointer to item"), W2CT(pItem->m_pOPCItemDef->szItemID), cdwBadItemHandles);
			continue;
		}
		
		// Issue a TRACE message if item read resulted in an error. This is 
		// for debugging purposes.  You could call GetErrorString() to get
		// meaning of server specific error codes and log them if you wish.
		if (FAILED (pErrors [dwItem]))
		{
			g_Logger.VLog(_T("IDataSink20::OnDataChange: item [%s] failure %x"), W2CT(pItem->m_pOPCItemDef->szItemID), pErrors[dwItem]);
		}
	}

	// Output any non-zero bad item handle counts if debugging:
	if (cdwBadItemHandles)
	{
		g_Logger.VLog(_T("IDataSink20::OnDataChange: Bad item handles received %d"), cdwBadItemHandles);
#ifdef _DEBUG
		//ATL_TRACE (_T("IDataSink20::OnDataChange - bad item handles received %d\r\n"), cdwBadItemHandles);
#endif
	}

	// Return "success" code.  Note this does not mean that there were no 
	// errors reported by the OPC Server, only that we successfully processed
	// the advisement.
	return (S_OK);
}

// **************************************************************************
// OnReadComplete ()
//
// Description:
//	This method is provided to handle notifications from an OPC Group on 
//	completion of AsyncIO2 reads of one or more items.
//
// Parameters:
//	DWORD		dwTransID,		Transaction ID returned by the server when
//								  the read was initiated.
//	OPCHANDLE	hGroup,			Client group handle.
//	HRESULT		hrMasterQuality	S_OK if all qualities are GOOD, otherwise S_FALSE.
//	HRESULT		hrMasterError	S_OK if all errors are S_OK, otherwise S_FALSE.
//	DWORD		dwCount			Number of items in the lists that follow.
//								  This may not be the same number as passed in
//								  read request if the server encountered errors.
//	OPCHANDLE	*phClientItems	Item client handles.
//	VARIANT		*pvValues		Item data.
//	WORD		*pwQualities	Item qualities.
//	FILETIME	*pftTimeStamps	Item timestamps.
//	HRESULT		*pErrors		Item errors.
//
// Returns:
//	STDMETHODIMP -
//		S_OK - Processing of advisement successful.
//		E_INVALIDARG - One of the arguments was invalid.
// **************************************************************************
STDMETHODIMP IDataSink20::OnReadComplete (DWORD dwTransID,
							OPCHANDLE hGroup,
							HRESULT hrMasterQuality,
							HRESULT hrMasterError,
							DWORD dwCount,
							OPCHANDLE *phClientItems,
							VARIANT *pvValues,
							WORD *pwQualities,
							FILETIME *pftTimeStamps,
							HRESULT *pErrors)
{
	USES_CONVERSION;
	LPGROUPINFO pGroup = NULL;
	COPCItemDef *pItem = NULL;

	// Initialize a bad item handle counter (used for debugging only):
	DWORD cdwBadItemHandles = 0;

	// Validate arguments.  Return with "invalid argument" error code 
	// if any are invalid:
	if (hGroup					== NULL	||
		dwCount					== 0	||
		phClientItems			== NULL	||
		pvValues				== NULL	||
		pwQualities				== NULL	||
		pftTimeStamps			== NULL	||
		pErrors					== NULL)
		return (E_INVALIDARG);

	// The group argument gives us a pointer to the destination CKGroup:
	pGroup = (LPGROUPINFO) hGroup;

	// Post a "read complete" message:
//	LogMsg (IDS_ASYNC20_READXACT_COMPLETE, dwTransID, dwCount,
//			pGroup->GetName (), hrMasterError);

	// Loop over items:
	for (DWORD dwItem = 0; dwItem < dwCount; dwItem++)
	{
		// The client item handles give us pointers to the destination 
		// COPCItemDef objects:
		pItem = (COPCItemDef *) phClientItems [dwItem];
		
		// Wrap use of COPCItemDef in exception handler in case pointer is bad:
		try
		{
			// Update the item's value, quality, and timestamp:
//			pItem->UpdateData(pvValues[dwItem], pwQualities[dwItem], &pftTimeStamps[dwItem]);
		}
		catch(INT nCode)
		{			
			// The DB execution throw a exception
			switch(nCode)
			{
			case E_INVALIDARG:
			case OPC_E_UNKNOWNITEMID:
			case E_INVALID_DBPTR_FOR_GROUP:
				g_Logger.VForceLog(_T("IDataSink20::OnDataChange: Failed to call COPCItemDef.UpdateDate. Code=%d"), nCode);
				break;
			default:
				;
//				g_Logger.VLog(_T("IDataSink20::OnDataChange: Failed to call COPCItemDef.UpdateDate. Code=%d, Message=%s"), pItem->GetLastErrorCode(), pItem->GetLastErrormsg());
			}
			continue;
		}
		catch (...)
		{
			//m_bExecuting = FALSE;
			//throw SetLastErrorCode((-(LONG)GetLastError()));
			// Must have tried to use a bad pItem pointer.  Bump a bad item 
			// handle count and try to process then next item:
			cdwBadItemHandles++;
			g_Logger.VLog(_T("IDataSink20::OnDataChange: pointer to item [%] is invalid. Total %d invalid pointer to item"), W2CT(pItem->m_pOPCItemDef->szItemID), cdwBadItemHandles);
			continue;
		}
		
		// Issue a TRACE message if item read resulted in an error. This is 
		// for debugging purposes.  You could call GetErrorString() to get
		// meaning of server specific error codes and log them if you wish.
		if (FAILED (pErrors [dwItem]))
		{
			g_Logger.VLog(_T("IDataSink20::OnReadComplete: item [%s] failure %x"), W2CT(pItem->m_pOPCItemDef->szItemID), pErrors[dwItem]);
		}
	}

	// Output any non-zero bad item handle counts if debugging:
	if (cdwBadItemHandles)
	{
		g_Logger.VLog(_T("IDataSink20::OnReadComplete: Bad item handles received %d"), cdwBadItemHandles);
#ifdef _DEBUG
		//TRACE (_T("IDataSink20:OnReadComplete - bad item handles received %d\r\n"), cdwBadItemHandles);
#endif
	}

	// Return "success" code.  Note this does not mean that there were no 
	// errors reported by the OPC Server, only that we successfully processed
	// the advisement.
	return (S_OK);
	}

// **************************************************************************
// OnWriteComplete ()
//
// Description:
//	This method is provided to handle notifications from an OPC Group on 
//	completion of AsyncIO2 writes to one or more items.
//
// Parameters:
//	DWORD		dwTransID		Transaction ID returned by the server when the 
//								  write was initiated.
//	OPCHANDLE	hGroup			Client group handle.
//	HRESULT		hrMasterError	S_OK if all errors are S_OK, otherwise S_FALSE
//	DWORD		dwCount			Number of items in the lists that follow.
//								  This may not be the same number as passed in
//								  read request if the server encountered errors.
//	OPCHANDLE	*phClientItems	Item client handles.
//	HRESULT		*pErrors		Item errors	.
//
// Returns:
//	STDMETHODIMP -
//		S_OK - Processing of advisement successful.
//		E_INVALIDARG - One of the arguments was invalid.
// **************************************************************************
STDMETHODIMP IDataSink20::OnWriteComplete (DWORD dwTransID,
							OPCHANDLE hGroup,
							HRESULT hrMasterError,
							DWORD dwCount,
							OPCHANDLE *phClientItems,
							HRESULT *pErrors)
{
	USES_CONVERSION;
	LPGROUPINFO pGroup = NULL;
	COPCItemDef *pItem = NULL;

	// Initialize a bad item handle counter (used for debugging only):
	DWORD cdwBadItemHandles = 0;

	// Validate arguments.  Return with "invalid argument" error code 
	// if any are invalid:
	if (hGroup					== NULL	||
		dwCount					== 0	||
		phClientItems			== NULL	||
		pErrors					== NULL)
		return (E_INVALIDARG);

	// The group argument gives us a pointer to the destination CKGroup:
	pGroup = (LPGROUPINFO) hGroup;

	// Post a "write complete" message:
//	LogMsg (IDS_ASYNC20_WRITE_COMPLETE, dwTransID, dwCount,
//		pGroup->GetName (), hrMasterError);

	// Loop over items:
	for (DWORD dwItem = 0; dwItem < dwCount; dwItem++)
	{
		// Post "failed write" messages:
		if (pErrors && FAILED (pErrors [dwItem]))
		{
			// Wrap use of pItem in exception handler in case pointer is bad:
			try
			{
				// The client item handles give us pointers to the destination 
				// COPCItemDef objects:
				pItem = (COPCItemDef *) phClientItems [dwItem];

				// Post the message.  You could call GetErrorString() to get
				// meaning of server specific error codes if you wish.
				g_Logger.VLog(_T("IDataSink20::OnWriteComplete: Failed to write item [%s]. TransID=%d, HRESULT=%x"), W2CT(pItem->m_pOPCItemDef->szItemID), dwTransID, pErrors[dwItem]);
			}
			catch (...)
			{
				// Must have tried to use a bad COPCItemDef pointer.  Bump a bad item 
				// handle count and try to process then next item:
				cdwBadItemHandles++;
				g_Logger.VLog(_T("IDataSink20::OnWriteComplete: Got a invalid pointer to item [%s]."), W2CT(pItem->m_pOPCItemDef->szItemID));
				continue;
			}
		}
	}

	// Output any non-zero bad item handle counts if debugging:
	if (cdwBadItemHandles)
	{
		g_Logger.VLog(_T("IDataSink20::OnWriteComplete: Bad item handles received %d"), cdwBadItemHandles);
#ifdef _DEBUG
		//TRACE (_T("IDataSink20::OnWriteComplete: - bad item handles received %d\r\n"), cdwBadItemHandles);
#endif
	}

	// Return "success" code.  Note this does not mean that there were no 
	// errors reported by the OPC Server, only that we successfully processed
	// the advisement.
	return (S_OK);
}

// **************************************************************************
// OnCancelComplete ()
//
// Description:
//	This method is provided to handle notifications from an OPC Group on
//	successful completion of an AsyncIO2 cancel.  This should not get called
//	if cancel request fails.  We do not need to do anything here except 
//	acknowledge receipt of notification by returning S_OK.
//
// Parameters:
//	DWORD		dwTransID		Transaction ID provided by use when the Read,
//								  Write, or Refresh was initiated.
//	OPCHANDLE	hGroup			Group handle.
//
// Returns:
//	STDMETHODIMP - S_OK
// **************************************************************************
STDMETHODIMP IDataSink20::OnCancelComplete (DWORD dwTransID, OPCHANDLE hGroup)
{
	//TRACE (_T("OTC: OnCancelComplete20\r\n"));
	return (S_OK);
}
