// DESUtil.cpp: implementation of the DESUtil class.
//
//////////////////////////////////////////////////////////////////////

#include <memory.h>
#include <windows.H>
#include "DES.h"
#include "DESUtil.h"
//#include <iostream>
#include <fstream.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef void (*PDESKEY)(unsigned char *, short);
typedef void (*PDES)(unsigned char *, unsigned char *);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDESUtil::CDESUtil()
{
	const static unsigned char m_chDefaultKey[24] = {
					0x34, 0x13, 0x51, 0x01, 0x11, 0x32, 0x43, 0x25,
					0x12, 0x45, 0x76, 0x34, 0x62, 0x18, 0x35, 0x67,
					0x57, 0x38, 0x33, 0x16, 0x19, 0x90, 0x49, 0x87};

	memcpy(m_chKey, m_chDefaultKey, 24);
	m_chLevel = EN0;

}

CDESUtil::~CDESUtil()
{

}

void CDESUtil::SetKey2(unsigned char Key[8])
{
	memcpy(&m_chKey[8], Key, 8);
}

void CDESUtil::SetKey3(unsigned char Key[8])
{
	memcpy(&m_chKey[16], Key, 8);
}

void CDESUtil::SetLevel(char chLevel)
{	
	m_chLevel = (chLevel == 1)?1:3;
}

unsigned char * CDESUtil::Encrypt(unsigned char *Dest, unsigned char *Src, int nLen)
{
	return DESOper(Dest, Src, nLen, EN0);
}

unsigned char * CDESUtil::Decrypt(unsigned char *Dest, unsigned char *Src, int nLen)
{
	return DESOper(Dest, Src, nLen, DE1);
}

unsigned char * CDESUtil::DESOper(unsigned char *Dest, unsigned char *Src, int nLen, short nMode)
{
	if (!Src || !Dest || (nLen < 1))
		return NULL;

	nMode = (nMode == EN0)?EN0:DE1;

	PDESKEY pDesKey;
	PDES pDES;
	if (m_chLevel = 1)
	{
		pDesKey = deskey;
		pDES = des;
	}
	else
	{
		pDesKey = des3key;
		pDES = Ddes;
	}

	pDesKey(m_chKey, nMode);
	int counter = nLen / 8;
	if ( (nLen % 8) > 0 )
		counter++;

	unsigned char *pDest = Dest;
	for (int i = 0; i < counter; i++, Src+=8, pDest+=8)
	{
		pDES(Src, pDest);
	}

	return Dest;
}

int CDESUtil::FileDESOper(LPCTSTR pszDest, LPCTSTR pszSrc, short nMode)
{
	nMode = (nMode == EN0)?EN0:DE1;

	PDESKEY pDesKey;
	PDES pDES;
	if (m_chLevel = 1)
	{
		pDesKey = deskey;
		pDES = des;
	}
	else
	{
		pDesKey = des3key;
		pDES = Ddes;
	}

	pDesKey(m_chKey, nMode);

	{
		ifstream inFile(pszSrc, ios::in || ios::binary);
		if (!inFile)
			return ERR_OPEN_SRC_FILE;

		ofstream outFile(pszDest, ios::out || ios::trunc || ios::binary);
		if (!outFile)
			return ERR_OPEN_DEST_FILE;

		unsigned char Buf[8];
		while (inFile)
		{
			inFile.read(Buf, 8);
			pDES(Buf, Buf);
			outFile.write(Buf, 8);
		}

		inFile.close();
		outFile.close();		
	}

	return 0;
}

int CDESUtil::FileEncrypt(LPCTSTR pszDest, LPCTSTR pszSrc)
{
	return FileDESOper(pszDest, pszSrc, EN0);
}

int CDESUtil::FileDecrypt(LPCTSTR pszDest, LPCTSTR pszSrc)
{
	return FileDESOper(pszDest, pszSrc, DE1);
}
