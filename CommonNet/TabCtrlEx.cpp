// TabCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "TabTest.h"
#include "TabCtrlEx.h"


// CTabCtrlEx

IMPLEMENT_DYNAMIC(CTabCtrlEx, CTabCtrl)

CTabCtrlEx::CTabCtrlEx()
{
	m_uLTabOffset = 0;
	m_uRTabOffset = 0;
	m_uTTabOffset = 0;
	m_uBTabOffset = 0;
	m_uLeftThickness	= DEFAULT_L_THICKNESS;
	m_uRightThickness	= DEFAULT_R_THICKNESS;
	m_uTopThickness		= DEFAULT_T_THICKNESS;
	m_uBottomThickness	= DEFAULT_B_THICKNESS;
}

CTabCtrlEx::~CTabCtrlEx()
{
	Clear();
}

void CTabCtrlEx::Clear()
{
	try
	{
		vector<PTABPAGE_INFO>::iterator it;
		for (it = m_vCtrls.begin(); it != m_vCtrls.end(); it++)
		{
			PTABPAGE_INFO p = (PTABPAGE_INFO)*it;
			if (p)
			{
				delete p;
				*it = (PTABPAGE_INFO)NULL;
			}
		}
	}
	catch (...) {}

	m_vCtrls.clear();
}


BEGIN_MESSAGE_MAP(CTabCtrlEx, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabCtrlEx::OnTcnSelchange)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CTabCtrlEx message handlers

LONG CTabCtrlEx::AddTabPage(int nIndex, DWORD dwDiagID, LPCTSTR pszTitle, LPCTSTR pszImage)
{
	CDialog *pDlg = new CDialog();
	pDlg->Create(dwDiagID, this);
	LONG lRet = this->InsertItem(nIndex, (pszTitle)?pszTitle:_T(""));
	if (lRet >= 0)
	{
		PTABPAGE_INFO pInfo = new TABPAGE_INFO(nIndex, pDlg, pszTitle, pszImage);
		m_vCtrls.push_back(pInfo);

		if (lRet == 0)
			GetTabOffset();				// Calculate offset of tab if the tab is the first

		ResizePage(lRet);
		if (lRet == 0)
			pDlg->ShowWindow(SW_SHOW);	// Show if it is the first tab
	}

	return lRet;
}

void CTabCtrlEx::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nIndex = this->GetCurSel();
	if (nIndex >= 0)
	{
		for (int i = 0; i < m_vCtrls.size(); i++)
		{
			PTABPAGE_INFO p = m_vCtrls[i];
			if (p->lIndex != nIndex)
			{
				if (p->pDlg) 
					p->pDlg->ShowWindow(SW_HIDE);	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE)
			}
			else
			{
				if (p->pDlg) 
					p->pDlg->ShowWindow(SW_SHOW);	//SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
			}
		}
	}

	*pResult = 0;
}

void CTabCtrlEx::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	ResizePages();
}

BOOL CTabCtrlEx::GetTabOffset()
{
	if (m_vCtrls.size() < 1)
		return FALSE;

	m_uLTabOffset = 0;
	m_uRTabOffset = 0;
	m_uTTabOffset = 0;
	m_uBTabOffset = 0;

	DWORD dwStyle = GetStyle();//GetExtendedStyle();
	BOOL bVertical = (TCS_VERTICAL == (dwStyle & TCS_VERTICAL));
	BOOL bBottom = (TCS_BOTTOM == (dwStyle & TCS_BOTTOM));
	int nRowCount = GetRowCount();

	RECT rect;
	GetItemRect(0, &rect);

	if (!bVertical)
	{
		if (bBottom)		// Tabs appear at the bottom of the control
		{
			m_uBTabOffset = (rect.bottom - rect.top) * nRowCount;
		}
		else				// Tabs appear at the top of the control
		{
			m_uTTabOffset = (rect.bottom - rect.top) * nRowCount;
		}
	}
	else
	{
		if (bBottom)		// Tabs appear at the right side of the control, with tab text displayed vertically
		{
			m_uRTabOffset = (rect.right - rect.left) * nRowCount;
		}
		else				// Tabs appear at the left side of the control, with tab text displayed vertically
		{
			m_uLTabOffset = (rect.right - rect.left) * nRowCount;
		}
	}

	return TRUE;
}

void CTabCtrlEx::ResizePage(UINT uIndex)
{
	if (uIndex >= m_vCtrls.size())
		return;

	PTABPAGE_INFO p = m_vCtrls[uIndex];
	if ( p && (p->pDlg) )
	{
		RECT rect;
		GetClientRect(&rect);		// rect.top and rect.x should always be 0
		LONG lNewX		= m_uLTabOffset + m_uLeftThickness;
		LONG lNewY		= m_uTTabOffset + m_uTopThickness;
		LONG lNewWidth = rect.right - m_uLeftThickness - m_uRightThickness - m_uLTabOffset - m_uRTabOffset;
		LONG lNewHeigh = rect.bottom - m_uTopThickness - m_uBottomThickness - m_uTTabOffset - m_uBTabOffset;

		p->pDlg->MoveWindow(lNewX, lNewY, lNewWidth, lNewHeigh);
	}
}

void CTabCtrlEx::ResizePages()
{
	GetTabOffset();
	for (int i = 0; i < m_vCtrls.size(); i++)
	{
		ResizePage(i);
	}
}