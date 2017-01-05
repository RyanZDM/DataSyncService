// CommBuff.h: interface for the CCommBuff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMBUFF_H__63C0BBD0_10DE_4845_A17B_C2117268C157__INCLUDED_)
#define AFX_COMMBUFF_H__63C0BBD0_10DE_4845_A17B_C2117268C157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>

// ͨ���ڴ�������
typedef struct _COMM_BUFF
{
	_COMM_BUFF *pPrev;		// ǰһ������ָ���ַ(���ڻ���)
	_COMM_BUFF *pNext;		// ��һ������ָ���ַ(���ڱ���)
	unsigned char *pBuff;	// �洢��
	INT nTheBuffSize;		// �洢����
}COMMBUFF, *LPCOMMBUFF;

class CCommBuff
{
public:
	CCommBuff();
	~CCommBuff();
	BOOL AddToChain(char *pBuff, long nBuffSize=0);			// ��ӽڵ�
	LPCOMMBUFF AddToChain2(char *pBuff, long nBuffSize=0);	// ��ӽڵ�
	BOOL ClearAllChain();									// �����������ڵ�
	long GetContentCount();									// ������ݸ���
	long GetAllContentLength();								// ����������ݴ�С
	long GetAllContent(char *pBuff);						// �����������
	long GetContentLimit(char *pBuff, long nMaxBytes);		// ������Ƴ��ȵ�����
	LPCOMMBUFF IsInIt(char *pFindStr, BOOL bCaseCompare=TRUE, INT nStrLength=0);	// �����Ƿ��в��ҵ��ַ�����
	BOOL RemoveItem(LPCOMMBUFF pItem);						// ɾ������
private:
	LPCOMMBUFF pTopNode, pNowNode;
	long nlAllBufferLength;									// ȫ���ڴ泤��
	long nlBufferCount;										// �ڴ�����ڵ����
};

#endif // !defined(AFX_COMMBUFF_H__63C0BBD0_10DE_4845_A17B_C2117268C157__INCLUDED_)
