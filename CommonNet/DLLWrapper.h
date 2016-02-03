// DLLWrapper.h: interface for the CDLLWrapper class.
// Description:	Call a method exported by a DLL file
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLWRAPPER_H__03D70CCB_AB5B_419F_A1B7_16062A5E8D0A__INCLUDED_)
#define AFX_DLLWRAPPER_H__03D70CCB_AB5B_419F_A1B7_16062A5E8D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>

#pragma warning (push)
#pragma warning (disable:4700)

class CDLLWrapper  
{
public:
	CDLLWrapper() {
		m_hInst = NULL;
		m_szErrMsg = NULL;
		m_bErrorFlag = FALSE;
	}

	CDLLWrapper(LPCTSTR pszDLLName) {
		m_hInst = NULL;
		m_szErrMsg = NULL;
		m_bErrorFlag = FALSE;
		this->LoadLibrary(pszDLLName);
	}

	virtual ~CDLLWrapper() {
		Release();
	}

	void Release()
	{
		if (m_hInst) {
			try {
				::FreeLibrary(m_hInst);
			}
			catch (...) {
				//
			}
		}

		ClearMemory();
	}

	BOOL GetLastError() {return m_bErrorFlag;}
	LPCTSTR GetLastErrMsg() {return m_szErrMsg;}

	BOOL LoadLibrary(LPCTSTR pszDLLName)
	{
		m_hInst = ::LoadLibrary(pszDLLName);
		m_bErrorFlag = (m_hInst == NULL);
		SetLastError(m_bErrorFlag);
		return !m_bErrorFlag;
	}

	template <typename TRet>
		void RunFromDLL0(TRet *pRet, LPCTSTR pszFuncName)
		{
			typedef TRet (WINAPI *FUNC) ();
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc();
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

		void RunFromDLL0(LPCTSTR pszFuncName)
		{

			typedef void (WINAPI *FUNC) ();
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc();
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1>
		void RunFromDLL1(TRet *pRet,
						 LPCTSTR pszFuncName,
						 TParam1 param1
						 )
		{
			typedef TRet (WINAPI *FUNC) (TParam1);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1>
		void RunFromDLL1(LPCTSTR pszFuncName,
						 TParam1 param1
						 )
		{

			typedef void (WINAPI *FUNC) (TParam1);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1, typename TParam2>
		void RunFromDLL2(TRet *pRet,
						 LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2
						 ) 
		{
			typedef TRet (WINAPI *FUNC) (TParam1, TParam2);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1, param2);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1, typename TParam2>
		void RunFromDLL2(LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2
						 ) 
		{
			typedef void (WINAPI *FUNC) (TParam1, TParam2);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1, param2);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1, typename TParam2, typename TParam3>
		void RunFromDLL3(TRet *pRet,
						 LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3
						 ) 
		{
			typedef TRet (WINAPI *FUNC) (TParam1, TParam2, TParam3);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1, param2, param3);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1, typename TParam2, typename TParam3>
		void RunFromDLL3(LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3
						 ) 
		{
			typedef void (WINAPI *FUNC) (TParam1, TParam2, TParam3);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1, param2, param3);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1, typename TParam2, typename TParam3, typename TParam4>
		void RunFromDLL4(TRet *pRet,
						 LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4
						 ) 
		{
			typedef TRet (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1, param2, param3, param4);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1, typename TParam2, typename TParam3, typename TParam4>
		void RunFromDLL4(LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4
						 ) 
		{
			typedef void (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1, param2, param3, param4);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5>
		void RunFromDLL5(TRet *pRet,
						 LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5
						 ) 
		{
			typedef TRet (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1, param2, param3, param4, param5);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5>
		void RunFromDLL5(LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5
						 ) 
		{
			typedef void (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1, param2, param3, param4, param5);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5, typename TParam6>
		void RunFromDLL6(TRet *pRet,
						 LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5,
						 TParam6 param6
						 ) 
		{
			typedef TRet (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5, TParam6);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1, param2, param3, param4, param5, param6);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5, typename TParam6>
		void RunFromDLL6(LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5,
						 TParam6 param6
						 ) 
		{
			typedef void (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5, TParam6);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1, param2, param3, param4, param5, param6);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5, typename TParam6, typename TParam7>
		void RunFromDLL7(TRet *pRet,
						 LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5,
						 TParam6 param6,
						 TParam7 param7
						 ) 
		{
			typedef TRet (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5, TParam6, TParam7);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1, param2, param3, param4, param5, param6, param7);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5, typename TParam6, typename TParam7>
		void RunFromDLL7(LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5,
						 TParam6 param6,
						 TParam7 param7
						 ) 
		{
			typedef void (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5, TParam6, TParam7);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1, param2, param3, param4, param5, param6, param7);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5, typename TParam6, typename TParam7, typename TParam8>
		void RunFromDLL8(TRet *pRet,
						 LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5,
						 TParam6 param6,
						 TParam7 param7,
						 TParam8 param8
						 ) 
		{
			typedef TRet (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5, TParam6, TParam7, TParam8);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1, param2, param3, param4, param5, param6, param7, param8);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5, typename TParam6, typename TParam7, typename TParam8>
		void RunFromDLL8(LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5,
						 TParam6 param6,
						 TParam7 param7,
						 TParam8 param8
						 ) 
		{
			typedef void (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5, TParam6, TParam7, TParam8);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1, param2, param3, param4, param5, param6, param7, param8);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TRet, typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5, typename TParam6, typename TParam7, typename TParam8, typename TParam9>
		void RunFromDLL9(TRet *pRet,
						 LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5,
						 TParam6 param6,
						 TParam7 param7,
						 TParam8 param8,
						 TParam9 param9
						 ) 
		{
			typedef TRet (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5, TParam6, TParam7, TParam8, TParam9);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					*pRet = pFunc(param1, param2, param3, param4, param5, param6, param7, param8, param9);
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

	template <typename TParam1, typename TParam2, typename TParam3, typename TParam4, typename TParam5, typename TParam6, typename TParam7, typename TParam8, typename TParam9>
		void RunFromDLL9(LPCTSTR pszFuncName, 
						 TParam1 param1, 
						 TParam2 param2, 
						 TParam3 param3,
						 TParam4 param4,
						 TParam5 param5,
						 TParam6 param6,
						 TParam7 param7,
						 TParam8 param8,
						 TParam9 param9
						 ) 
		{
			typedef void (WINAPI *FUNC) (TParam1, TParam2, TParam3, TParam4, TParam5, TParam6, TParam7, TParam8, TParam9);
			if (m_hInst && pszFuncName) {
				FUNC pFunc = (FUNC)GetProcAddress(m_hInst, pszFuncName);
				if (pFunc) {
					SetLastError(FALSE);
					pFunc(param1, param2, param3, param4, param5, param6, param7, param8, param9);
					return;
				}
			}
			else
			{
				SetLastError(TRUE);
			}
		}

private:
	//CStrUtil m_StrUtil;
	HMODULE m_hInst;
	LPTSTR m_szErrMsg;

	BOOL m_bErrorFlag;

	void ClearMemory() {
		if (m_szErrMsg) {
			delete [] m_szErrMsg;
			m_szErrMsg = NULL;
		}
	}

	void SetLastError(BOOL bFlag) {
		if (bFlag) 
			SetLastErrMsg(::GetLastError());
		
		m_bErrorFlag = bFlag;
	}

	void SetLastErrMsg(LPCTSTR pszMsg) {
		ClearMemory();

		INT nSize = lstrlen(pszMsg) + 1;
		if (nSize > 0) {
			m_szErrMsg = new TCHAR[nSize];
			memcpy(m_szErrMsg, pszMsg, sizeof(TCHAR) * nSize);
		}
	}

	void SetLastErrMsg(INT nCode) {
		FormatMsg(_T(""), nCode);
	}	

	LPCTSTR FormatMsg(LPCTSTR lpMsg, INT nMessageId)
	{
		ClearMemory();

		LPVOID lpMsgBuf = NULL;
		if ( 0 != FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
			0, 
			nMessageId, 
			0,
			(LPTSTR)&lpMsgBuf,
			0,
			NULL) ) 
		{
			INT nActLen = lstrlen(lpMsg) + lstrlen((LPTSTR)lpMsgBuf) + 3;
			m_szErrMsg = new TCHAR[nActLen];

			//memset(m_szErrMsg, 0, sizeof(TCHAR)*nActLen);
			wsprintf(m_szErrMsg, _T("%s\r\n%s"), lpMsg, (LPTSTR)lpMsgBuf);				

			LocalFree(lpMsgBuf);
		}

		return m_szErrMsg;
	}

};

#pragma warning (pop)

#endif // !defined(AFX_DLLWRAPPER_H__03D70CCB_AB5B_419F_A1B7_16062A5E8D0A__INCLUDED_)
