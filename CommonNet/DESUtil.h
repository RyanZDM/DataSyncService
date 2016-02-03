// DESUtil.h: interface for the DESUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESUTIL_H__3895A56F_B8A1_4715_B501_7DE6BF699565__INCLUDED_)
#define AFX_DESUTIL_H__3895A56F_B8A1_4715_B501_7DE6BF699565__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>

#define	ERR_OPEN_SRC_FILE	-1;
#define ERR_OPEN_DEST_FILE	-2;

class CDESUtil  
{
public:
	int FileDecrypt(LPCTSTR pszDest, LPCTSTR pszSrc);
	int FileEncrypt(LPCTSTR pszDest, LPCTSTR pszSrc);
	unsigned char * Decrypt(unsigned char *Dest, unsigned char *Src, int nLen);
	void SetLevel(char chLevel);
	unsigned char * Encrypt(unsigned char *Dest, unsigned char *Src, int nLen);
	void SetKey3(unsigned char Key[8]);
	void SetKey2(unsigned char Key[8]);
	CDESUtil();
	virtual ~CDESUtil();

private:
	int FileDESOper(LPCTSTR pszDest, LPCTSTR pszSrc, short nMode);
	unsigned char * DESOper(unsigned char *Dest, unsigned char *Src, int  nLen, short nMode);
	char m_chLevel;
	unsigned char m_chKey[24];
	const static unsigned char m_chDefaultKey[24];
};

#endif // !defined(AFX_DESUTIL_H__3895A56F_B8A1_4715_B501_7DE6BF699565__INCLUDED_)
