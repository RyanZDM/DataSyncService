// StrUtil.cpp: implementation of the CStrUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "StrUtil.h"
#include <comutil.h>
#include <shlwapi.h>
#include <algorithm>
#include <stdio.h>
#include <wchar.h>

#if _MSC_VER>1200 
	#pragma comment(lib, "comsuppW.lib")
#else
	#pragma comment(lib, "comsupp.lib")
#endif

#pragma comment(lib, "shlwapi.lib")

#define	COMP_A(s1, s2, flag)	( (flag)?(strstr((LPCSTR)s1, (LPCSTR)s2)):(StrStrIA((LPCSTR)s1, (LPCSTR)s2)) )
#define	COMP_W(s1, s2, flag)	( (flag)?(wcsstr((LPCWSTR)s1, (LPCWSTR)s2)):(StrStrIW((LPCWSTR)s1, (LPCWSTR)s2)) )

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD tagCommonBuf::GetTotalLength()
{
	DWORD dwLen = 0;
	for (DWORD i = 0; i < m_vDatas.size(); i++)
	{
		LPBYTEDATA p = m_vDatas[i];
		if (p)
		{
			dwLen += p->dwLen;
		}	
	}

	return dwLen;
}

BOOL tagCommonBuf::Append(LPBYTE pBuf, DWORD dwLen, BOOL bForceAllocateMemory)
{
	if (!pBuf || (dwLen < 1))
		return FALSE;

	BYTE *pData = pBuf;
	if (bForceAllocateMemory)
	{
		pData = new BYTE[dwLen];
		memcpy(pData, pBuf, dwLen);
	}

	LPBYTEDATA p = new BYTEDATA(pData, dwLen);
	m_vDatas.push_back(p);

	return TRUE;
}

LPBYTE tagCommonBuf::GetWholeData(LPBYTE pBuf, DWORD &dwBufLen, BOOL bAutoRelease)
{
	if (!pBuf)
	{
		dwBufLen = GetTotalLength();
		return pBuf;
	}

	if (bAutoRelease)
		m_vPointersNeedToBeReleased.push_back(pBuf);

	DWORD dwTotalLen = GetTotalLength();
	DWORD dwTotalWritten = 0;
	DWORD dwRemain = dwBufLen - dwTotalWritten;

	for (DWORD i = 0; i < m_vDatas.size(); i++)
	{
		LPBYTEDATA p = m_vDatas[i];
		if (p && p->pData && (p->dwLen > 0))
		{
			DWORD dwBytes = min(dwRemain, p->dwLen);
			memcpy(pBuf + dwTotalWritten, p->pData, dwBytes);
			dwTotalWritten += dwBytes;
			dwRemain -= dwBytes;
		}

		if (dwRemain < 1)
			break;
	}

	dwBufLen = dwTotalWritten;
	return pBuf;
}

LPBYTE tagCommonBuf::GetWholeData(BOOL bAutoRelease)
{
	DWORD dwLen = GetTotalLength();
	if (dwLen < 1)
		return NULL;

	LPBYTE pData = new BYTE[dwLen];
	return GetWholeData(pData, dwLen, bAutoRelease);
}

BOOL tagCommonBuf::Remove(UINT nIndex)
{
	UINT nTotalSize = m_vDatas.size();
	if (nIndex >= nTotalSize)
		return FALSE;

	vector<LPBYTEDATA>::iterator vi = m_vDatas.begin();
	vi++;
	for (UINT i = 0; i < nIndex; i++, vi++) {}

	if (vi != m_vDatas.end())
	{
		m_vDatas.erase(vi);
		return TRUE;
	}
}

BOOL tagCommonBuf::Remove(UINT nMin, UINT nMax)
{
	if (nMax < nMin)
		return FALSE;

	UINT nTotalSize = m_vDatas.size();
	if (nMin >= nTotalSize)
		return FALSE;

	if (nMax >= nTotalSize)
		nMax = nTotalSize - 1;

	UINT nCountToBeDeleted = nMax - nMin + 1;

	UINT i;
	vector<LPBYTEDATA>::iterator viBegin = m_vDatas.begin();
	viBegin++;
	for (i = 0; i < nMin; i++, viBegin++) {}

	vector<LPBYTEDATA>::iterator viEnd = viBegin;
	for (i = 0; i < nCountToBeDeleted; i++, viEnd++)

	m_vDatas.erase(viBegin, viEnd);
	return TRUE;
}

void tagCommonBuf::Clear()
{
	ClearVector(m_vDatas);
	ClearVector(m_vPointersNeedToBeReleased);
}

CStrUtil::CStrUtil()
{
}

CStrUtil::~CStrUtil()
{
}

/**************************************************************************
 * Divide a string into several part 
 *		and save it to a vector according to the special separator
 * Parameters:
 *    vRet - to save the several substring
 *    lpszSource - the source string
 *	  lpszSeparator - the separator
 *	  bCaseSensitive - whether it case sensitive
 * Return:
 *    The count of the substring
 * -----------------------------------------------------------------------
 * Note:
 * -----------------------------------------------------------------------
 * Author:
 * Create Date: 
 * Modified Date:
 *************************************************************************/
INT CStrUtil::Str2Vector(std::vector<string> &vRet, LPCSTR lpszSource, LPCSTR lpszSeparator, BOOL bCaseSensitive)
{
	vRet.clear();
	if ( !lpszSource || !lpszSeparator)
		return 0;

	DWORD dwSrcLen = strlen(lpszSource);
	DWORD dwSepLen = strlen(lpszSeparator);

	if ( (0 == dwSrcLen) || (0 == dwSepLen) || (dwSepLen > dwSrcLen) ) {
		vRet.push_back(string(lpszSource));
		return vRet.size();
	}

	char *pBuf = NULL;
	LPCSTR pStart = lpszSource;
	while (pStart) {
		LPCSTR pFound = strstr(pStart, lpszSeparator);		

		if (pFound) {
			// found a matched substring
			DWORD dwSubLen = pFound - pStart;
			pBuf = new char[dwSubLen + 1];
			memcpy(pBuf, pStart, dwSubLen);
			pBuf[dwSubLen] = 0;
			vRet.push_back(string(pBuf));
			
			pStart = (pFound + dwSepLen);
			if (pBuf)
				delete pBuf;
		}
		else {
			// the last substring
			vRet.push_back(string(pStart));
			pStart = NULL;
		}
	}

	return vRet.size();
}

INT CStrUtil::Str2Vector(std::vector<wstring> &vRet, LPCWSTR lpszSource, LPCWSTR lpszSeparator, BOOL bCaseSensitive)
{
	vRet.clear();
	if ( !lpszSource || !lpszSeparator)
		return 0;

	DWORD dwSrcLen = wcslen(lpszSource);
	DWORD dwSepLen = wcslen(lpszSeparator);

	if ( (0 == dwSrcLen) || (0 == dwSepLen) || (dwSepLen > dwSrcLen) ) {
		vRet.push_back(wstring(lpszSource));
		return vRet.size();
	}

	wchar_t *pBuf = NULL;
	LPCWSTR pStart = lpszSource;
	while (pStart) {
		LPCWSTR pFound = wcsstr(pStart, lpszSeparator);		

		if (pFound) {
			// found a matched substring
			DWORD dwSubLen = pFound - pStart;
			pBuf = new wchar_t[dwSubLen + 1];
			memcpy(pBuf, pStart, sizeof(wchar_t) * dwSubLen);
			pBuf[dwSubLen] = 0;
			vRet.push_back(wstring(pBuf));
			
			pStart = (pFound + dwSepLen);
			if (pBuf)
				delete pBuf;
		}
		else {
			// the last substring
			vRet.push_back(wstring(pStart));
			pStart = NULL;
		}
	}

	return vRet.size();
}

string & CStrUtil::Vector2Str(string & str, const std::vector<string> &vList, LPCSTR lpszSeparator)
{
	if (NULL == lpszSeparator)
		lpszSeparator = " ";

	str.clear();
	for (std::vector<string>::const_iterator it = vList.begin(); it != vList.end(); it++)
	{
		str.append(*it).append(lpszSeparator);
	}

	if (str.length() > 0)
		str.erase(str.length() - strlen(lpszSeparator));

	return str;
}

wstring & CStrUtil::Vector2Str(wstring & str, const std::vector<wstring> &vList, LPCWSTR lpszSeparator)
{
	if (NULL == lpszSeparator)
		lpszSeparator = L" ";

	str.clear();
	for (std::vector<wstring>::const_iterator it = vList.begin(); it != vList.end(); it++)
	{
		str.append(*it).append(lpszSeparator);
	}

	if (str.length() > 0)
		str.erase(str.length() - wcslen(lpszSeparator));

	return str;
}

/**************************************************************************
 * Combine the custom message with system error message
 * Parameters:
 *    lpMsg - custom message
 *    nMessageId - the message id which is getted from GetLastError()
 * Return:
 *    The combined message text
 * -----------------------------------------------------------------------
 * Note:
 * -----------------------------------------------------------------------
 * Author:
 * Create Date: 
 * Modified Date:
 *************************************************************************/
string & CStrUtil::FormatMsg(LPCSTR lpMsg, string &szFormatedMsg, INT nMessageId)
{
	szFormatedMsg.clear();

	LPVOID lpMsgBuf = NULL;
	if ( 0 != FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
								0, 
								nMessageId, 
								0,
								(LPSTR)&lpMsgBuf,
								0,
								NULL) ) 
	{
		if (lpMsg)
			szFormatedMsg.append(lpMsg, strlen(lpMsg));
		//szFormatedMsg.append("\r\n", 2);
		szFormatedMsg.append((LPCSTR)lpMsgBuf, strlen((LPCSTR)lpMsgBuf));

		LocalFree(lpMsgBuf);
	}
			
	return szFormatedMsg;
}

wstring & CStrUtil::FormatMsg(LPCWSTR lpMsg, wstring &szFormatedMsg, INT nMessageId)
{
	szFormatedMsg.clear();

	LPVOID lpMsgBuf = NULL;
	if ( 0 != FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
								0, 
								nMessageId, 
								0,
								(LPWSTR)&lpMsgBuf,
								0,
								NULL) ) 
	{
		if (lpMsg)
			szFormatedMsg.append(lpMsg, wcslen(lpMsg));
		//szFormatedMsg.append(L"\r\n", 2);
		szFormatedMsg.append((LPCWSTR)lpMsgBuf, wcslen((LPCWSTR)lpMsgBuf));

		LocalFree(lpMsgBuf);
	}
			
	return szFormatedMsg;
}

//*****************************************************************************
//
// WideStrToMultiStr()
//
//*****************************************************************************

string & CStrUtil::WideStrToMultiStr (PWCHAR WideStr, string &szMultiStr, UINT nCharSet)
{
	szMultiStr.clear();

    ULONG nBytes;
    PCHAR MultiStr = NULL;

    // Get the length of the converted string
    //
    nBytes = WideCharToMultiByte(nCharSet,
								 0,
								 WideStr,
								 -1,
								 NULL,
								 0,
								 NULL,
								 NULL);

    if (nBytes > 0)
	{
		// Allocate space to hold the converted string
		//
		MultiStr = new CHAR[nBytes];
		if (MultiStr != NULL)
		{
			// Convert the string
			//
			nBytes = WideCharToMultiByte(nCharSet,
				0,
				WideStr,
				-1,
				MultiStr,
				nBytes,
				NULL,
				NULL);
			
			if (nBytes > 0)
			{				
				szMultiStr.append(MultiStr, nBytes);
			}

			delete[] MultiStr;
		}
	}
	
    return szMultiStr;
}

string & CStrUtil::ToLower(string &str)
{
	transform(str.begin(), str.end(), str.begin(), tolower);

	return str;
}

wstring & CStrUtil::ToLower(wstring &str)
{
	transform(str.begin(), str.end(), str.begin(), towlower);

	return str;
}

string & CStrUtil::ToUpper(string &str)
{
	transform(str.begin(), str.end(), str.begin(), toupper);

	return str;
}

wstring & CStrUtil::ToUpper(wstring &str)
{
	transform(str.begin(), str.end(), str.begin(), towupper);

	return str;
}

string & CStrUtil::Str2Hex(const unsigned char *pszMsg, string &szHexMsg, INT nLen, char chSeparator)
{
	szHexMsg.clear();

	if (!pszMsg)
		return szHexMsg;

	if (nLen <= 0)	
		nLen = strlen((const char*)pszMsg);
	
	char hex[3] = {0};
	INT end = nLen - 1;
	for (INT i = 0; i < nLen; i++)
	{
		sprintf_s(hex, 3, "%02X", *(pszMsg+i));
		szHexMsg += hex;
		if ( (i < end) && (chSeparator != 0) )
			szHexMsg += chSeparator;		
	}

	return szHexMsg;
}

string & CStrUtil::Str2Hex(const wchar_t *pwszMsg, string &szHexMsg, INT nLen, char chSeparator)
{
	szHexMsg.clear();

	if (!pwszMsg)
		return szHexMsg;

	if (nLen <= 0)
		nLen = wcslen(pwszMsg) * (sizeof (wchar_t) / sizeof(char));

	return Str2Hex((const unsigned char*)pwszMsg, szHexMsg, nLen, chSeparator);
}

LPSTR CStrUtil::_Sprintf(LPSTR pszMsg, size_t sizeInBytes, LPCSTR pcszFormatStr, ...)
{
	try
	{
		va_list args;		
		va_start (args, pcszFormatStr);
		sprintf_s(pszMsg, sizeInBytes, pcszFormatStr, args);
		va_end (args);
	}
	catch (...) {
	}

	return pszMsg;
}

LPWSTR CStrUtil::_Sprintf(LPWSTR pszMsg, size_t sizeInWords, LPCWSTR pcszFormatStr, ...)
{
	try
	{
		va_list args;		
		va_start (args, pcszFormatStr);
		vswprintf_s(pszMsg, sizeInWords, pcszFormatStr, args);
		va_end (args);
	}
	catch (...) {
	}

	return pszMsg;
}

/************************************************************************/
/* Description: Convert a BSTR to char * and save it to string object   */
/* Note:																*/
/*	1. We can get the wchar_t* directly by conversion (const wchar_t*)	*/
/************************************************************************/
string & CStrUtil::BSTR2Str(const BSTR &bstrMsg, string &szMsg)
{
	char* pTemp = _com_util::ConvertBSTRToString(bstrMsg);
	szMsg = pTemp;
	delete[] pTemp;

	return szMsg;
}

BOOL CStrUtil::GetKeyValue(string &szVal, LPCSTR pcszSource, LPCSTR pcszKey, LPCSTR pcszSeparator, LPCSTR pcszDefalut)
{
	vector<string> vVal;
	szVal.clear();
	INT nCount = Str2Vector(vVal, pcszSource, pcszSeparator);
	if (nCount > 0)
	{
		vector<string>::iterator vi;
		for (vi = vVal.begin(); vi != vVal.end(); vi++)
		{
			LPCSTR pVal = ((string)(*vi)).c_str();
			LPSTR pFound = (LPSTR)strchr(pVal, '=');
			if (pFound != NULL)
			{
				string szKey("");
				LPSTR pTemp = NULL;
				for (pTemp = (LPSTR)pVal; pTemp != pFound; pTemp++)
					szKey += *pTemp;

				if (_stricmp(pcszKey, szKey.c_str()) == 0)		// Found the key
				{
					pTemp++;			// Pointer to value
					szVal = pTemp;
					return TRUE;
				}
			}
		}
	}

	szVal = pcszDefalut;
	return FALSE;
}

BOOL CStrUtil::GetKeyValue(wstring &wszVal, LPCWSTR pcwszSource, LPCWSTR pcwszKey, LPCWSTR pcwszSeparator, LPCWSTR pcwszDefalut)
{
	vector<wstring> vVal;
	wszVal.clear();
	INT nCount = Str2Vector(vVal, pcwszSource, pcwszSeparator);
	if (nCount > 0)
	{
		vector<wstring>::iterator vi;
		for (vi = vVal.begin(); vi != vVal.end(); vi++)
		{
			LPCWSTR pVal = ((wstring)(*vi)).c_str();
			LPWSTR pFound = (LPWSTR)wcschr(pVal, L'=');
			if (pFound != NULL)
			{
				LPWSTR pTemp = NULL;
				wstring szKey(L"");
				for (pTemp = (LPWSTR)pVal; pTemp != pFound; pTemp++)
					szKey += *pTemp;

				if (_wcsicmp(pcwszKey, szKey.c_str()) == 0)		// Found the key
				{
					pTemp++;			// Pointer to value
					wszVal = pTemp;
					return TRUE;
				}
			}
		}
	}

	wszVal = pcwszDefalut;
	return FALSE;
}

INT CStrUtil::ReplaceAll(string &szDest, LPCSTR pcszSrc, LPCSTR pcszOld, LPCSTR pcszNew, BOOL bCaseSensitive)
{
	if (!pcszSrc || !pcszOld || !pcszNew)
		return -1;

	INT nCount = 0;
	string szTemp;
	DWORD dwOldLen = strlen(pcszOld);
	LPCSTR pStart = pcszSrc;
	LPCSTR pFound = NULL;
	while ( NULL != (pFound = COMP_A(pStart, pcszOld, bCaseSensitive)) )
	{
		nCount++;
		for (LPCSTR p = pStart; p != pFound; p++)
			szTemp += *p;

		szTemp += pcszNew;
		pStart = (pFound + dwOldLen);
	}

	if (nCount == 0)
	{
		// Do nothing if the address of dest buffer and source string are the same
		if (szDest.c_str() != pcszSrc)
			szDest = pcszSrc;
	}
	else
	{
		szTemp += pStart;		// The remaining string
		szDest = szTemp.c_str();
	}

	return nCount;
}

INT CStrUtil::ReplaceAll(wstring &wszDest, LPCWSTR pcwszSrc, LPCWSTR pcwszOld, LPCWSTR pcwszNew, BOOL bCaseSensitive)
{
	if (!pcwszSrc || !pcwszOld || !pcwszNew)
		return -1;

	INT nCount = 0;
	wstring wszTemp;
	DWORD dwOldLen = lstrlenW(pcwszOld);
	LPCWSTR pStart = pcwszSrc;
	LPCWSTR pFound = NULL;
	while ( NULL != (pFound = COMP_W(pStart, pcwszOld, bCaseSensitive)) )
	{
		nCount++;
		for (LPCWSTR p = pStart; p != pFound; p++)
			wszTemp += *p;

		wszTemp += pcwszNew;
		pStart = (pFound + dwOldLen);
	}

	if (nCount == 0)
	{
		// Do nothing if the address of dest buffer and source string are the same
		if (wszDest.c_str() != pcwszSrc)
			wszDest = pcwszSrc;
	}
	else
	{
		wszTemp += pStart;		// The remaining string
		wszDest = wszTemp.c_str();
	}

	return nCount;
}

LPSTR CStrUtil::FindKeyword(LPCSTR pcszSrc, LPCSTR pcszKeyword, const vector<char> *pvExcludeChar, BOOL bCaseSensitive)
{
	if (!pcszSrc || !pcszKeyword)
		return NULL;

	LPSTR pFound = NULL;
	LPCSTR pStart = pcszSrc;
	DWORD dwSrcLen = strlen(pcszSrc);
	DWORD dwKeywordLen = strlen(pcszKeyword);
	while ( NULL != (pFound = (LPSTR)COMP_A(pStart, pcszKeyword, bCaseSensitive)) )	
	{
		if ( !pvExcludeChar || (pvExcludeChar->size() == 0) )		// If no exclude chars specified, return immediately
			return pFound;

		BOOL bLeftMatched = FALSE;
		BOOL bRightMatched = FALSE;
		if (pFound > pStart)		// The keyword is not at the begining
		{			
			char ch = *(pFound - 1);
			bLeftMatched = ( pvExcludeChar->end() != find(pvExcludeChar->begin(), pvExcludeChar->end(), ch) );			
		}
		else						// The keyword is at the begining
		{
			bLeftMatched = TRUE;	
		}

		if (bLeftMatched)
		{
			if ( (pFound - pcszSrc + dwKeywordLen) < dwSrcLen )		// The keyword is not at the end of string
			{
				char ch = *(pFound + dwKeywordLen);
				bRightMatched = ( pvExcludeChar->end() != find(pvExcludeChar->begin(), pvExcludeChar->end(), ch) );
			}
			else												// The keyword is at the end of string
			{
				bRightMatched = TRUE;
			}
		}

		if (bLeftMatched && bRightMatched)
			return pFound;
		
		pStart = pFound + dwKeywordLen;
	}

	return pFound;
}

LPWSTR CStrUtil::FindKeyword(LPCWSTR pcwszSrc, LPCWSTR pcwszKeyword, const vector<wchar_t> *pvExcludeChar, BOOL bCaseSensitive)
{
	if (!pcwszSrc || !pcwszKeyword)
		return NULL;

	LPWSTR pFound = NULL;
	LPCWSTR pStart = pcwszSrc;
	DWORD dwSrcLen = lstrlenW(pcwszSrc);
	DWORD dwKeywordLen = lstrlenW(pcwszKeyword);
	while ( NULL != (pFound = (LPWSTR)COMP_W(pStart, pcwszKeyword, bCaseSensitive)) )	
	{
		if ( !pvExcludeChar || (pvExcludeChar->size() == 0) )		// If no execlue chars specified, return immediately
			return pFound;

		BOOL bLeftMatched = FALSE;
		BOOL bRightMatched = FALSE;
		if (pFound > pStart)		// The keyword is not at the begining
		{			
			wchar_t ch = *(pFound - 1);
			bLeftMatched = ( pvExcludeChar->end() != find(pvExcludeChar->begin(), pvExcludeChar->end(), ch) );			
		}
		else						// The keyword is at the begining
		{
			bLeftMatched = TRUE;	
		}

		if (bLeftMatched)
		{
			if ( (pFound - pcwszSrc + dwKeywordLen) < dwSrcLen )		// The keyword is not at the end of string
			{
				wchar_t ch = *(pFound + dwKeywordLen);
				bRightMatched = ( pvExcludeChar->end() != find(pvExcludeChar->begin(), pvExcludeChar->end(), ch) );
			}
			else												// The keyword is at the end of string
			{
				bRightMatched = TRUE;
			}
		}

		if (bLeftMatched && bRightMatched)
			return pFound;
		
		pStart = pFound + dwKeywordLen;
	}

	return pFound;
}

LPBYTE CStrUtil::ParsePseudoHex(DWORD &dwDataLen, LPCSTR pcszData, LPCSTR pcszPrefix, LPCSTR pcszSuffix, LPCSTR pcszSeparator)
{
	dwDataLen = 0;
	if (!pcszData || !pcszPrefix || !pcszSuffix || !pcszSeparator)
		return NULL;

	DWORD dwSrcLen = strlen(pcszData);
	if (dwSrcLen < 1)
		return  NULL;

	DWORD dwPrefixLen = strlen(pcszPrefix);
	if (dwPrefixLen < 1)
		return NULL;

	DWORD dwSuffixLen = strlen(pcszSuffix);
	if (dwSuffixLen < 1)
		return NULL;

	DWORD dwSeparatorLen = strlen(pcszSeparator);
	if (dwSeparatorLen < 1)
		return NULL;

	if (strcmp(pcszPrefix, pcszSuffix) == 0)
		return NULL;

	char *pPrefix = NULL;
	char *pSuffix = NULL;
	const char *pStart = pcszData;
	COMMON_BUF buf;
	while ( (pPrefix = (char*)strstr(pStart, pcszPrefix)) && (pSuffix = strstr(pPrefix, pcszSuffix)) )
	{
		// Get the data before prefix
		DWORD dwLen = (pPrefix - pStart);
		char *pBuf = new char[dwLen];
		memcpy(pBuf, pStart, dwLen);
		buf.Append((LPBYTE)pBuf, dwLen);

		pStart = pSuffix + dwSuffixLen;		// The start position for next search

		// Parse hex string
		pPrefix += dwPrefixLen;
		DWORD dwHexStrLen = (pSuffix - pPrefix);
		if (dwHexStrLen > 0)
		{
			char *pHexStr = new char[dwHexStrLen + 1];
			memcpy(pHexStr, pPrefix, dwHexStrLen);
			pHexStr[dwHexStrLen] = '\0';

			vector<string> vRet;
			if (CStrUtil::Str2Vector(vRet, pHexStr, pcszSeparator) > 0)
			{
				for (UINT i = 0; i < vRet.size(); i++)
				{					
					if (vRet[i].length() > 0)
					{
						BYTE *p = new BYTE[1];
						p[0] = (BYTE)strtol(vRet[i].c_str(), NULL, 16);
						buf.Append(p, 1);
					}
				}
			}

			delete pHexStr;
		}
		
	}

	if (pStart)		// There are still some data after pseudo hex string
	{		
		buf.Append((LPBYTE)_strdup(pStart), strlen(pStart));
	}

	dwDataLen = buf.GetTotalLength();
	return buf.GetWholeData(FALSE);
}

BOOL CStrUtil::CompareBytes(const LPBYTE pbData1, const LPBYTE pbData2, DWORD dwLen)
{
	if (dwLen < 1)
		return TRUE;

	if (!pbData1 || !pbData2)
		return FALSE;

	for (DWORD i = 0; i < dwLen; i++)
	{
		try
		{
			if (pbData1[i] != pbData2[i])
				return FALSE;
		}
		catch (...)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CStrUtil::StartWith(const char *pData1, const char *pData2, DWORD dwLen, BOOL bCaseSensitive)
{
	if (dwLen < 1)
		return TRUE;

	if (!pData1 || !pData2)
		return FALSE;

	LPCSTR pFound = NULL;
	if (bCaseSensitive)
	{
		pFound = StrStrIA(pData1,  pData2);
	}
	else
	{
		pFound = strstr(pData1, pData2);
	}

	return (pFound && (pFound == pData1));
}

BOOL CStrUtil::StartWith(const wchar_t *pData1, const wchar_t *pData2, DWORD dwLen, BOOL bCaseSensitive)
{
	if (dwLen < 1)
		return TRUE;

	if (!pData1 || !pData2)
		return FALSE;

	LPCWSTR pFound = NULL;
	if (bCaseSensitive)
	{
		pFound = StrStrIW(pData1,  pData2);
	}
	else
	{
		pFound = wcsstr(pData1, pData2);
	}

	return (pFound && (pFound == pData1));
}

INT CStrUtil::FindByte(const LPBYTE pbData, DWORD dwLen, BYTE chData)
{
	if (!pbData || dwLen < 1)
		return -1;

	for (DWORD i = 0; i < dwLen; i++)
	{
		try
		{
			if (chData == pbData[i])
				return i;
		}
		catch (...)
		{
			return -1;
		}
	}

	return -1;
}

INT CStrUtil::FindBytes(const LPBYTE pbDataSrc, DWORD dwSrcLen, const LPBYTE pbDataDest, DWORD dwDestLen)
{
	INT nRet = -1;

	if ( (dwDestLen < 1) || (dwSrcLen < 1) || (dwSrcLen < dwDestLen) )
		return nRet;

	if (!pbDataSrc  || !pbDataDest)
		return nRet;

	BOOL bMatch = FALSE;
	DWORD dwBoundary = dwSrcLen - dwDestLen + 1;
	for (DWORD i = 0; i < dwBoundary; i++)
	{
		try
		{
			bMatch = TRUE;
			for (DWORD dest = 0; dest < dwDestLen; dest++)
			{
				if (pbDataSrc[i+dest] != pbDataDest[dest])
				{
					bMatch = FALSE;
					break;
				}
			}

			if (bMatch)
			{
				nRet = i;
				break;
			}
		}
		catch (...)
		{
			// Do nothing
		}
	}

	return nRet;
}

_bstr_t& CStrUtil::Variant2BSTR(const VARIANT &var, _bstr_t &bstrRet)
{
	const INT MAX_LEN = 1000;
	TCHAR szBuf[MAX_LEN];

	switch(var.vt)
	{
	case VT_UI1:
		_stprintf_s(szBuf, MAX_LEN, _T("%d"),  var.bVal);
		bstrRet = szBuf;
		break;
	case VT_I2:
		_stprintf_s(szBuf, MAX_LEN, _T("%d"),  var.iVal);
		bstrRet = szBuf;
		break;
	case VT_I4:
		_stprintf_s(szBuf, MAX_LEN, _T("%d"),  var.lVal);
		bstrRet = szBuf;
		break;
	case VT_R4:
		_stprintf_s(szBuf, MAX_LEN, _T("%f"),  var.fltVal);
		bstrRet = szBuf;
	case VT_R8:
		_stprintf_s(szBuf, MAX_LEN, _T("%f"),  var.dblVal);
		bstrRet = szBuf;
		break;
	case VT_CY:
		{
			double dblVal = (double)var.cyVal.int64;
			_stprintf_s(szBuf, MAX_LEN, _T("%0.4f"),  dblVal / 10000.0f);
			bstrRet = szBuf;
		}
        break;
	case VT_BSTR:
		bstrRet = var.bstrVal;
		break;
	case VT_DATE:
		{
			FILETIME fileTime;
			SYSTEMTIME systemTime;
			time_t curTime = (long)var.date;			
			
			// Convert time_t to FILETime and then to SYTEMTIME
			LONGLONG llTime;
			llTime = Int32x32To64(curTime, 10000000) + 116444736000000000;
			fileTime.dwLowDateTime = (DWORD)llTime;
			fileTime.dwHighDateTime = (DWORD)(llTime >> 32);
			FileTimeToSystemTime(&fileTime, &systemTime);
			
			_stprintf_s(szBuf, MAX_LEN,
						_T("%d-%d-%d %d:%d:%d"), 
						systemTime.wYear,
						systemTime.wMonth,
						systemTime.wDay,
						systemTime.wHour,
						systemTime.wMinute,
						systemTime.wSecond);
			bstrRet = szBuf;
		}
		break;
	case VT_BOOL:
		_stprintf_s(szBuf, MAX_LEN, _T("%d"),  var.boolVal);
		bstrRet = szBuf;
		break;
	case VT_EMPTY:
	case VT_NULL:
		bstrRet = _T("");
		break;
	default: 
		bstrRet = _T("");
		break;
	}

    return bstrRet;
}

INT CStrUtil::Str2Int(const char* pStr, INT base, WORD wLen)
{
	char data[5] = {0};
	strncpy_s(data, 5, pStr, min(4, wLen));
	return (INT)strtol(data, NULL, base);
}

char* CStrUtil::Int2HexStr(char *pHexStr, DWORD dwBufLen, INT nHexCode, DWORD dwHexStrLen)
{
	memset(pHexStr, 0, dwBufLen);

	if (dwBufLen <= dwHexStrLen)
		return pHexStr;		// Just return a empty buffer

	char szTemplate[5];
	sprintf_s(szTemplate, 5, "%%0%dX", dwHexStrLen);
	sprintf_s(pHexStr, 5, szTemplate, nHexCode);

	return pHexStr;
}

char* CStrUtil::ConvertDateStr(char szNew[20], const char *pszOld /*[14]*/)
{
	if (!szNew )
		return NULL;

	if (pszOld && (strlen(pszOld) < 14))
	{
		return NULL;
	}

	if (pszOld)
	{
		szNew[0] = pszOld[0];
		szNew[1] = pszOld[1];
		szNew[2] = pszOld[2];
		szNew[3] = pszOld[3];
		szNew[4] = '-';			// year end
		szNew[5] = pszOld[4];
		szNew[6] = pszOld[5];
		szNew[7] = '-';			// month end
		szNew[8] = pszOld[6];
		szNew[9] = pszOld[7];
		szNew[10] = 0x20;		// day end
		szNew[11] = pszOld[8];
		szNew[12] = pszOld[9];
		szNew[13] = ':';		// hour end
		szNew[14] = pszOld[10];
		szNew[15] = pszOld[11];
		szNew[16] = ':';		// minute end
		szNew[17] = pszOld[12];
		szNew[18] = pszOld[13];
		szNew[19] = 0x00;		// end
	}
	else		// It does means no data, set it to current date time
	{
		SYSTEMTIME now;
		GetLocalTime(&now);
		sprintf_s(szNew, 20, "%04d-%02d-%02d %02d:%02d:%02d", now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond);
	}

	return szNew;
}

DECIMAL& CStrUtil::Double2Decimal(DECIMAL &dec, double dblVal)
{
	DECIMAL_SETZERO(dec);

	dec.sign = (dblVal < 0)?DECIMAL_NEG:0;

	for (INT i = 0; i < dec.scale; i++)
	{
		dblVal *= 10;
	}

	dec.Lo32 = (ULONG)dblVal;

	return dec;
}

double CStrUtil::Decimal2Doule(double &dblVal, const DECIMAL &dec)
{
	dblVal = dec.Lo32;
	for (INT i = 0; i < dec.scale; i++)
	{
		dblVal /= 10;
	}

	if (DECIMAL_NEG == dec.sign)
		dblVal *= -1;

	return dblVal;
}

DECIMAL& CStrUtil::Str2Decimal(DECIMAL &dec, const char *pStr)
{
	DECIMAL_SETZERO(dec);

	if (!pStr)		// Set decimal to zero
		return dec;

	double dblVal = (atof(pStr));
	dec.sign = (dblVal < 0)?DECIMAL_NEG:0;

	for (INT i = 0; i < dec.scale; i++)
	{
		dblVal *= 10;
	}

	dec.Lo32 = (ULONG)dblVal;

	return dec;
}

DECIMAL& CStrUtil::Str2Decimal(DECIMAL &dec, const wchar_t *pStr)
{
	DECIMAL_SETZERO(dec);

	if (!pStr)		// Set decimal to zero
		return dec;

	wchar_t *pStop = NULL;
	double dblVal = (wcstod(pStr, &pStop));
	dec.sign = (dblVal < 0)?DECIMAL_NEG:0;

	for (INT i = 0; i < dec.scale; i++)
	{
		dblVal *= 10;
	}

	dec.Lo32 = (ULONG)dblVal;

	return dec;
}

string& CStrUtil::Str2Decimal(string &str, const DECIMAL &dec)
{
	double dblVal = dec.Lo32;
	for (INT i = 0; i < dec.scale; i++)
	{
		dblVal /= 10;
	}

	if (DECIMAL_NEG == dec.sign)
		dblVal *= -1;

	char szBuf[100];
	sprintf_s(szBuf, 100, "%f", dblVal);

	str = szBuf;

	return str;
}

wstring& CStrUtil::Str2Decimal(wstring &wstr, const DECIMAL &dec)
{
	double dblVal = dec.Lo32;
	for (INT i = 0; i < dec.scale; i++)
	{
		dblVal /= 10;
	}

	if (DECIMAL_NEG == dec.sign)
		dblVal *= -1;

	wchar_t szBuf[100];
	swprintf_s(szBuf, 100, L"%f", dblVal);

	wstr = szBuf;

	return wstr;
}

WORD CStrUtil::Str2Word(const char *pcszHex /*char[4]*/, INT base)
{
	return Str2Word(pcszHex, 4, base);
}

WORD CStrUtil::Str2Word(const char *pcszHex, WORD wLen, INT base)
{
	if (!pcszHex)
		return 0;
	
	char data[5] = {0};
	strncpy_s(data, 5, pcszHex, min(4, wLen));
	return (WORD)strtol(data, NULL, base);
}

char CStrUtil::Str2Char(const char *pcszHex /*char[1/2]*/, INT nLen/*1 or 2*/, INT base)
{
	if (nLen > 2)
		nLen = 2;
	
	char data[3] = {0};
	strncpy_s(data, 3, pcszHex, nLen);
	return (char)strtol(data, NULL, base);
}