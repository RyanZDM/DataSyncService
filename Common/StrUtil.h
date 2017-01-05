// StrUtil.h: interface for the CStrUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRUTIL_H__A947E2F9_CDC3_4218_BE1F_E149751A9548__INCLUDED_)
#define AFX_STRUTIL_H__A947E2F9_CDC3_4218_BE1F_E149751A9548__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#include <wtypes.h>
#include <vector>
#include <windows.h>
#include <string>
#include <COMUTIL.H>
#include <tchar.h>

#include "ContainerUtil.h"

using namespace std;

typedef struct tagByteData 
{
	LPBYTE pData;
	DWORD dwLen;
	tagByteData() {pData = NULL; dwLen = 0;}
	tagByteData(LPBYTE p, DWORD len) {pData = p; dwLen = len;}
	~tagByteData() {Clear();}
	void Clear()
	{
		if (pData)
		{
			delete pData; 
			pData = NULL;
		}
	}
}BYTEDATA, *LPBYTEDATA;

typedef struct tagCommonBuf 
{
	vector<LPBYTEDATA> m_vDatas;
	vector<LPBYTE> m_vPointersNeedToBeReleased;
	tagCommonBuf() {}
	~tagCommonBuf() {Clear();}

	inline DWORD GetTotalLength();
	inline BOOL Append(LPBYTE pBuf, DWORD dwLen, BOOL bForceAllocateMemory = FALSE);
	inline LPBYTE GetWholeData(LPBYTE pBuf, DWORD &dwBufLen, BOOL bAutoRelease = FALSE);
	inline LPBYTE GetWholeData(BOOL bAutoRelease = FALSE);
	inline BOOL Remove(UINT nIndex);
	inline BOOL Remove(UINT nMin, UINT nMax);
	inline void Clear();
}COMMON_BUF, *LPCOMMON_BUF;			// A data buffer which can allocate/destroy memory itself.

class CStrUtil  
{
public:
	static WORD Str2Word(const char *pcszHex /*char[4]*/, INT base);
	static WORD Str2Word(const char *pcszHex, WORD wLen, INT base);
	static char Str2Char(const char *pcszHex /*char[1/2]*/, INT nLen/*1 or 2*/, INT base);
	static char* ConvertDateStr(char szNew[20], const char *pszOld /*[14]*/);
	static char* Int2HexStr(char *pHexStr, DWORD dwBufLen, INT nHexCode, DWORD dwHexStrLen);
	static INT Str2Int(const char* pStr, INT base, WORD wLen = 4);

	static DECIMAL& Double2Decimal(DECIMAL &dec, double dblVal);
	static double Decimal2Doule(double &dblVal, const DECIMAL &dec);
	static DECIMAL& Str2Decimal(DECIMAL &dec, const char *pStr);
	static DECIMAL& Str2Decimal(DECIMAL &dec, const wchar_t *pStr);
	static string& Str2Decimal(string &str, const DECIMAL &dec);
	static wstring& Str2Decimal(wstring &wstr, const DECIMAL &dec);
	static _bstr_t& Variant2BSTR(const VARIANT &var, _bstr_t &bstrRet);
	static INT FindByte(const LPBYTE pbData, DWORD dwLen, BYTE chData);																			// Find position of the specified byte data in a byte buffer
	static INT FindBytes(const LPBYTE pbDataSrc, DWORD dwSrcLen, const LPBYTE pbDataDest, DWORD dwDestLen);
	static BOOL CompareBytes(const LPBYTE pbData1, const LPBYTE pbData2, DWORD dwLen);															// Check if the left [dwLen] bytes are the same for source and dest buffer
	static BOOL StartWith(const char *pData1, const char *pData2, DWORD dwLen, BOOL bCaseSensitive = TRUE);
	static BOOL StartWith(const wchar_t *pData1, const wchar_t *pData2, DWORD dwLen, BOOL bCaseSensitive = TRUE);
	static LPBYTE ParsePseudoHex(DWORD &dwDataLen, LPCSTR pcszData, LPCSTR pcszPrefix, LPCSTR pcszSuffix, LPCSTR pcszSeparator);				// Convert all the hex string in string buffer into hex data 
	static LPSTR FindKeyword(LPCSTR pcszSrc, LPCSTR pcszKeyword, const vector<char> *pvExcludeChar = NULL, BOOL bCaseSensitive = FALSE);		// Find specified keyword in a string, return pointer to that keyword if found
	static LPWSTR FindKeyword(LPCWSTR pcwszSrc, LPCWSTR pcwszKeyword, const vector<wchar_t> *pExcludeChar = NULL, BOOL bCaseSensitive = FALSE);	// Find specified keyword in a string, return pointer to that keyword if found
	static INT ReplaceAll(string &szDest, LPCSTR pcszSrc, LPCSTR pcszOld, LPCSTR pcszNew, BOOL bCaseSensitive = FALSE);							// Replace all matched keyword with a new string value
	static INT ReplaceAll(wstring &wszDest, LPCWSTR pcwszSrc, LPCWSTR pcwszOld, LPCWSTR pcwszNew, BOOL bCaseSensitive = FALSE);					// Replace all matched keyword with a new string value
	static BOOL GetKeyValue(string &szVal, LPCSTR pcszSource, LPCSTR pcszKey, LPCSTR pcszSeparator, LPCSTR pcszDefalut = "");					// Find the specified keyword and return the string value behind "[keyword]="
	static BOOL GetKeyValue(wstring &wszVal, LPCWSTR pcwszSource, LPCWSTR pcwszKey, LPCWSTR pcwszSeparator, LPCWSTR pcwszDefalut = L"");		// Find the specified keyword and return the string value behind "[keyword]="
	static string & BSTR2Str(const BSTR &bstrMsg, string &szMsg);																				// Convert a BSTR to sting
	static LPSTR _Sprintf(LPSTR pszMsg, size_t sizeInBytes, LPCSTR pcszFormatStr, ...);																				// Similar with sprintf
	static LPWSTR _Sprintf(LPWSTR pszMsg, size_t sizeInWords, LPCWSTR pcszFormatStr, ...);																			// Similar with sprintf
	static string & Str2Hex(const unsigned char *pszMsg, string &szHexMsg, INT nLen = 0, char chSeparator = ',');								// Convert byte data to hex string
	static string & Str2Hex(const wchar_t *pwszMsg, string &szHexMsg, INT nLen = 0, char chSeparator = ',');									// Convert byte data to hex string
	static string & ToUpper(string &str);
	static wstring & ToUpper(wstring &str);
	static string & ToLower(string &str);
	static wstring & ToLower(wstring &str);
	static string & WideStrToMultiStr (PWCHAR WideStr, string &szMultiStr, UINT nCharSet = CP_ACP);
	static string & FormatMsg(LPCSTR lpMsg, string &szFormatedMsg, INT nMessageId);																// Combine the custom message with system error message
	static wstring & FormatMsg(LPCWSTR lpMsg, wstring &szFormatedMsg, INT nMessageId);															// Combine the custom message with system error message
	static INT Str2Vector(std::vector<string> &vRet, LPCSTR lpszSource, LPCSTR lpszSeparator, BOOL bCaseSensitive = FALSE);						// Split a string according to specified separator and fill it into a vector
	static INT Str2Vector(std::vector<wstring> &vRet, LPCWSTR lpszSource, LPCWSTR lpszSeparator, BOOL bCaseSensitive = FALSE);					// Split a string according to specified separator and fill it into a vector
	static string & Vector2Str(string & str, const std::vector<string> &vList, LPCSTR lpszSeparator);											// Combine all string in vector to a single string
	static wstring & Vector2Str(wstring & str, const std::vector<wstring> &vList, LPCWSTR lpszSeparator);
	static INT GetProgramFromCommandString(LPCTSTR pcszCmdStr, LPTSTR szBuf, DWORD dwLen);														// Extract the program from a command string
	
	CStrUtil();
	virtual ~CStrUtil();
};

#endif // !defined(AFX_STRUTIL_H__A947E2F9_CDC3_4218_BE1F_E149751A9548__INCLUDED_)
