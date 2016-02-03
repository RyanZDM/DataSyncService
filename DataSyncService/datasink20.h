// **************************************************************************
// datasink20.h
//
// Description:
//	Defines the IDataSink20 class.
//
// **************************************************************************


#ifndef _DATASINK20_H
#define _DATASINK20_H

#include "opcda.h"
#include "opccomn.h"

// **************************************************************************
class IDataSink20 : public IOPCDataCallback
	{
	public:
		IDataSink20 ();
		~IDataSink20 ();

		// IUnknown Methods
		STDMETHODIMP         QueryInterface (REFIID iid, LPVOID *ppInterface);
		STDMETHODIMP_(ULONG) AddRef ();
		STDMETHODIMP_(ULONG) Release ();

		// IOPCDataCallback Methods 
		STDMETHODIMP OnDataChange (		// OnDataChange notifications
			DWORD dwTransID,			// 0 for normal OnDataChange events, non-zero for Refreshes
			OPCHANDLE hGroup,			// client group handle
			HRESULT hrMasterQuality,	// S_OK if all qualities are GOOD, otherwise S_FALSE
			HRESULT hrMasterError,		// S_OK if all errors are S_OK, otherwise S_FALSE
			DWORD dwCount,				// number of items in the lists that follow
			OPCHANDLE *phClientItems,	// item client handles
			VARIANT *pvValues,			// item data
			WORD *pwQualities,			// item qualities
			FILETIME *pftTimeStamps,	// item timestamps
			HRESULT *pErrors);			// item errors	

		STDMETHODIMP OnReadComplete (	// OnReadComplete notifications
			DWORD dwTransID,			// Transaction ID returned by the server when the read was initiated
			OPCHANDLE hGroup,			// client group handle
			HRESULT hrMasterQuality,	// S_OK if all qualities are GOOD, otherwise S_FALSE
			HRESULT hrMasterError,		// S_OK if all errors are S_OK, otherwise S_FALSE
			DWORD dwCount,				// number of items in the lists that follow
			OPCHANDLE *phClientItems,	// item client handles
			VARIANT *pvValues,			// item data
			WORD *pwQualities,			// item qualities
			FILETIME *pftTimeStamps,	// item timestamps
			HRESULT *pErrors);			// item errors	

		STDMETHODIMP OnWriteComplete (	// OnWriteComplete notifications
			DWORD dwTransID,			// Transaction ID returned by the server when the write was initiated
			OPCHANDLE hGroup,			// client group handle
			HRESULT hrMasterError,		// S_OK if all errors are S_OK, otherwise S_FALSE
			DWORD dwCount,				// number of items in the lists that follow
			OPCHANDLE *phClientItems,	// item client handles
			HRESULT *pErrors);			// item errors	

		STDMETHODIMP OnCancelComplete (	// OnCancelComplete notifications
			DWORD dwTransID,			// Transaction ID provided by the client when the read/write/refresh was initiated
			OPCHANDLE hGroup);

	private:
		DWORD m_cnRef;
	};


#endif // _DATASINK20_H
