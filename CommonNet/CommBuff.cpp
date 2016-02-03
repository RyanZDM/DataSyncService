// CommBuff.cpp: implementation of the CCommBuff class.
//
//////////////////////////////////////////////////////////////////////

#include "CommBuff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommBuff::CCommBuff ()
{
	pTopNode = pNowNode = NULL;
	nlAllBufferLength = 0;
	nlBufferCount = 0;
}
CCommBuff::~CCommBuff ()
{
	ClearAllChain();									// �������
}

BOOL CCommBuff::AddToChain(char *pBuff, long nBuffSize)
{
	if(pBuff == NULL)
		return FALSE;

	if(nBuffSize <= 0)
		nBuffSize = (long)(strlen(pBuff));	// ����ַ�������
	if(nBuffSize == 0)
		return FALSE;

	// ����һ������
	LPCOMMBUFF pTemp;
	pTemp = new COMMBUFF;
	if(pTemp == NULL)
		return FALSE;
	memset((void *)pTemp, 0x00, sizeof(COMMBUFF));
	pTemp->pBuff = new unsigned char [nBuffSize+1];
	if(pTemp->pBuff == NULL)
	{
		delete pTemp;
		return FALSE;
	}
	pTemp->nTheBuffSize = nBuffSize;
	memset(pTemp->pBuff, 0x00, nBuffSize+1);
	memcpy(pTemp->pBuff, pBuff, nBuffSize);

	// ��Ӹýڵ�������
	if(pTopNode == NULL)
	{
		pTopNode = pNowNode = pTemp;
		nlAllBufferLength = nBuffSize;
		nlBufferCount = 0;
	}
	else
	{
		pTemp->pPrev = pNowNode;
		pNowNode->pNext = pTemp;
		pNowNode = pTemp;
		nlAllBufferLength += nBuffSize;
	}
	nlBufferCount++;
	return TRUE;
}

LPCOMMBUFF CCommBuff::AddToChain2(char *pBuff, long nBuffSize)
{
	if(pBuff == NULL)
		return NULL;

	if(nBuffSize <= 0)
		nBuffSize = (long)(strlen(pBuff));	// ����ַ�������
	if(nBuffSize == 0)
		return NULL;

	// ����һ������
	LPCOMMBUFF pTemp;
	pTemp = new COMMBUFF;
	if(pTemp == NULL)
		return NULL;
	memset((void *)pTemp, 0x00, sizeof(COMMBUFF));
	pTemp->pBuff = new unsigned char [nBuffSize+1];
	if(pTemp->pBuff == NULL)
	{
		delete pTemp;
		return NULL;
	}
	pTemp->nTheBuffSize = nBuffSize;
	memset(pTemp->pBuff, 0x00, nBuffSize+1);
	memcpy(pTemp->pBuff, pBuff, nBuffSize);

	// ��Ӹýڵ�������
	if(pTopNode == NULL)
	{
		pTopNode = pNowNode = pTemp;
		nlAllBufferLength = nBuffSize;
		nlBufferCount = 0;
	}
	else
	{
		pTemp->pPrev = pNowNode;
		pNowNode->pNext = pTemp;
		pNowNode = pTemp;
		nlAllBufferLength += nBuffSize;
	}
	nlBufferCount++;
	return pTemp;
}

BOOL CCommBuff::ClearAllChain()
{
	LPCOMMBUFF pTemp, pTemp1;
	pTemp = pTopNode;
	while(pTemp != NULL)
	{
		pTemp1 = pTemp->pNext;
		if(pTemp->pBuff != NULL)
			delete[] pTemp->pBuff;		// �ͷ�ָ���ڴ�
		delete pTemp;							// �ͷŽڵ�ṹ
		pTemp = pTemp1;							// ��һ���ڵ�
	}
	pTopNode = pNowNode = NULL;
	nlAllBufferLength = nlBufferCount = 0;
	return TRUE;
}

long CCommBuff::GetContentCount()
{
	return nlBufferCount;
}

long CCommBuff::GetAllContentLength()
{
	return nlAllBufferLength;
}

long CCommBuff::GetAllContent(char *pBuff)
{
	// ���BufferΪNULL, Ϊ��û�������С����
	if(pBuff == NULL || nlAllBufferLength == 0)
		return nlAllBufferLength;

	long nlPlace=0;
	LPCOMMBUFF pTemp = pTopNode;

	// �����������нڵ�������Ŀ���ڴ�
	while(pTemp != NULL)
	{
		memcpy((void *)(pBuff+nlPlace), pTemp->pBuff, pTemp->nTheBuffSize);
		nlPlace += pTemp->nTheBuffSize;
		pTemp = pTemp->pNext;
	}
	return nlPlace;
}

long CCommBuff::GetContentLimit(char *pBuff, long nMaxBytes)
{
	// ���BufferΪNULL, Ϊ��û�������С����
	if(pBuff == NULL || nlAllBufferLength == 0)
		return nlAllBufferLength;

	long nlPlace=0;
	long nLessBytes = 0;
	LPCOMMBUFF pTemp = pTopNode;

	// �����������нڵ�������Ŀ���ڴ�
	while(pTemp != NULL)
	{
		nLessBytes = nMaxBytes - nlPlace;
		if(nLessBytes < pTemp->nTheBuffSize)	// ���ʣ��ռ�С
		{
			if(nLessBytes > 0)
				memcpy((void *)(pBuff+nlPlace), pTemp->pBuff, nLessBytes);
			nlPlace += nLessBytes;
			break;
		}
		memcpy((void *)(pBuff+nlPlace), pTemp->pBuff, pTemp->nTheBuffSize);
		nlPlace += pTemp->nTheBuffSize;
		pTemp = pTemp->pNext;
	}
	return nlPlace;
}

LPCOMMBUFF CCommBuff::IsInIt(char *pFindStr, BOOL bCaseCompare, INT nStrLength)
{
	LPCOMMBUFF pResult = NULL;

	if(pFindStr == NULL)
		return pResult;

	INT nCompareResult = 0;
	LPCOMMBUFF pTemp;
	pTemp = pTopNode;
	while(pTemp != NULL)
	{
		if(bCaseCompare)
		{
			if(nStrLength == 0)
				nCompareResult = strcmp((char *)pTemp->pBuff, pFindStr);
			else
				nCompareResult = memcmp((char *)pTemp->pBuff, pFindStr, nStrLength);
		}
		else
			nCompareResult = _stricmp((char *)pTemp->pBuff, pFindStr);
		if( nCompareResult == 0)
		{
			pResult = pTemp;
			break;
		}
		pTemp = pTemp->pNext;			// ��һ���ڵ�
	}
	return pResult;

}

BOOL CCommBuff::RemoveItem(LPCOMMBUFF pItem)
{
	if(pItem == NULL)
		return FALSE;

	BOOL bFindIt = FALSE;
	// �����Ƿ��и���,����������ڴ����
	LPCOMMBUFF pTemp = pTopNode;
	while(pTemp != NULL)
	{
		if(pTemp == pItem)
		{
			bFindIt = TRUE;
			break;
		}
		pTemp = pTemp->pNext;
	}
	// �����������ڵ�
	if(!bFindIt)
		return FALSE;

	// �޸ĵ�ǰ�ڵ�
	if(pNowNode == pItem)
		pNowNode = pItem->pPrev;

	if(pItem->pPrev == NULL)	// û�и��ڵ�
		pTopNode = pItem->pNext;
	else
		pItem->pPrev->pNext = pItem->pNext;
	if(pItem->pNext != NULL)
		pItem->pNext->pPrev = pItem->pPrev;

	nlAllBufferLength -= pItem->nTheBuffSize;
	nlBufferCount--;

	delete pItem;
	return TRUE;
}