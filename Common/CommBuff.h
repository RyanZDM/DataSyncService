// CommBuff.h: interface for the CCommBuff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMBUFF_H__63C0BBD0_10DE_4845_A17B_C2117268C157__INCLUDED_)
#define AFX_COMMBUFF_H__63C0BBD0_10DE_4845_A17B_C2117268C157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>

// 通用内存链表类
typedef struct _COMM_BUFF
{
	_COMM_BUFF *pPrev;		// 前一个链表指针地址(用于回搠)
	_COMM_BUFF *pNext;		// 后一个链表指针地址(用于遍历)
	unsigned char *pBuff;	// 存储区
	INT nTheBuffSize;		// 存储长度
}COMMBUFF, *LPCOMMBUFF;

class CCommBuff
{
public:
	CCommBuff();
	~CCommBuff();
	BOOL AddToChain(char *pBuff, long nBuffSize=0);			// 添加节点
	LPCOMMBUFF AddToChain2(char *pBuff, long nBuffSize=0);	// 添加节点
	BOOL ClearAllChain();									// 清除所有链表节点
	long GetContentCount();									// 获得内容个数
	long GetAllContentLength();								// 获得所有内容大小
	long GetAllContent(char *pBuff);						// 获得所有内容
	long GetContentLimit(char *pBuff, long nMaxBytes);		// 获得限制长度的内容
	LPCOMMBUFF IsInIt(char *pFindStr, BOOL bCaseCompare=TRUE, INT nStrLength=0);	// 查找是否有查找的字符串项
	BOOL RemoveItem(LPCOMMBUFF pItem);						// 删除该项
private:
	LPCOMMBUFF pTopNode, pNowNode;
	long nlAllBufferLength;									// 全部内存长度
	long nlBufferCount;										// 内存链表节点个数
};

#endif // !defined(AFX_COMMBUFF_H__63C0BBD0_10DE_4845_A17B_C2117268C157__INCLUDED_)
