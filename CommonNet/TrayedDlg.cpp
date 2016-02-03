// TrayedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TrayedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ARRAYSIZE(A) (sizeof(A)/sizeof(A[0]))
/////////////////////////////////////////////////////////////////////////////
// CTrayedDlg dialog


CTrayedDlg::CTrayedDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CTrayedDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_uActivateStyle	= WM_LBUTTONDBLCLK;
	m_uPopupMenuStyle	= WM_RBUTTONUP;
	m_hPopupMenu		= NULL;

	srand( (unsigned)time( NULL ) );
	m_uTrayID = (UINT)(((double) rand() / (double) RAND_MAX) * 20000 + 10000);
}


void CTrayedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrayedDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrayedDlg, CDialog)
	//{{AFX_MSG_MAP(CTrayedDlg)
	ON_WM_SIZE()
	ON_MESSAGE(WM_TRAY_ACTIVE, OnActiveWindow)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrayedDlg message handlers

void CTrayedDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (SIZE_MINIMIZED == nType)
	{
		Hide();
	}
}

LRESULT CTrayedDlg::OnActiveWindow(WPARAM wParam, LPARAM lParam)
{
	USHORT uStyle = (USHORT)lParam;
	if ( ((m_uActivateStyle & uStyle) == m_uActivateStyle) || (0x0405 == uStyle) )
	{
		Show();	
		return 1L;
	}
	else if ( (m_uPopupMenuStyle & uStyle) == m_uPopupMenuStyle )
	{
		// Check if need to pop up a menu
		if (m_hPopupMenu)
		{
			CPoint point;
			GetCursorPos(&point);
			::TrackPopupMenu(m_hPopupMenu, TPM_LEFTBUTTON | TPM_RIGHTALIGN/*TPM_RIGHTALIGN | TPM_BOTTOMALIGN*/, point.x, point.y, 0, this->GetSafeHwnd(), NULL);
		}
		//TrackPopupMenu
	}
	else if (0x0404 == uStyle)	// Close the tip message window
	{
		if (!this->IsIconic())
		{
			Shell_NotifyIcon(NIM_DELETE, &m_NotifyData);
		}
	}
	 	
	return 0L;
}

void CTrayedDlg::Hide()
{
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyData);
	Shell_NotifyIcon(NIM_ADD, &m_NotifyData);
	this->ShowWindow(SW_HIDE);
}

void CTrayedDlg::Show()
{
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyData);
	this->ShowWindow(SW_SHOWNORMAL);
	this->ActivateTopParent();
}

BOOL CTrayedDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_NotifyData.cbSize		= sizeof(NOTIFYICONDATA);
	m_NotifyData.hWnd		= this->GetSafeHwnd();
	m_NotifyData.uID		= m_uTrayID;
	m_NotifyData.hIcon		= NULL;
	m_NotifyData.uCallbackMessage	= WM_TRAY_ACTIVE;
	m_NotifyData.uFlags		= NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;	
	m_NotifyData.dwInfoFlags= NIIF_INFO;

	CString szTitle;
	this->GetWindowText(szTitle);
	_tcsncpy(m_NotifyData.szTip, szTitle, ARRAYSIZE(m_NotifyData.szTip));
	_tcsncpy(m_NotifyData.szInfoTitle, szTitle, ARRAYSIZE(m_NotifyData.szInfoTitle));
	_tcsncpy(m_NotifyData.szInfo, DEFAULT_TIP_INFO, ARRAYSIZE(m_NotifyData.szInfo));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#include <strsafe.h>
void CTrayedDlg::ShowTipMsg(LPCTSTR pMsg, DWORD dwInfoFlag, HICON hNewIcon)
{
	// Record the original value for restore
	UINT uOldFlags		= m_NotifyData.uFlags;
	DWORD dwOldInfoFlags= m_NotifyData.dwInfoFlags;
	CString szOldInfo	= m_NotifyData.szInfo;

	m_NotifyData.uFlags			= m_NotifyData.uFlags | NIF_INFO;
	m_NotifyData.dwInfoFlags	= dwInfoFlag;
	if (hNewIcon)
		m_NotifyData.hIcon = hNewIcon;

	_tcsncpy(m_NotifyData.szInfo, pMsg, ARRAYSIZE(m_NotifyData.szInfo));
	if (this->IsIconic())
	{
		Shell_NotifyIcon(NIM_MODIFY, &m_NotifyData);
	}
	else
	{
		Shell_NotifyIcon(NIM_DELETE, &m_NotifyData);
		Shell_NotifyIcon(NIM_ADD, &m_NotifyData);		
	}

	//Sleep(100);

	// Restore the original value for next hide/show
	m_NotifyData.uFlags		= uOldFlags;
	m_NotifyData.dwInfoFlags= dwOldInfoFlags;

	_tcsncpy(m_NotifyData.szInfo, (LPCTSTR)szOldInfo, ARRAYSIZE(m_NotifyData.szInfo));
}

BOOL CTrayedDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	try
	{
		Shell_NotifyIcon(NIM_DELETE, &m_NotifyData);
		if (m_hPopupMenu)
		{
			::DestroyMenu(m_hPopupMenu);
			m_hPopupMenu = NULL;
		}

	}
	catch (...)
	{
		//
	}
	
	return CDialog::DestroyWindow();
}
