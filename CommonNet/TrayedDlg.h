#if !defined(AFX_TRAYEDDLG_H__364F5F81_B414_4C24_A570_62238213CB85__INCLUDED_)
#define AFX_TRAYEDDLG_H__364F5F81_B414_4C24_A570_62238213CB85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define _WIN32_IE 0x0500
//#define _WIN32_IE 0x0600

#define DEFAULT_TIP_INFO	_T("You can active window by double clicking left mouse button here.")

const UINT WM_TRAY_ACTIVE	= WM_USER + 9999;
// TrayedDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrayedDlg dialog

class CTrayedDlg : public CDialog
{
// Construction
public:
	CTrayedDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

	void Hide();
	void Show();

// Dialog Data
	//{{AFX_DATA(CTrayedDlg)
	//enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayedDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrayedDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnActiveWindow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRestore(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void ShowTipMsg(LPCTSTR pMsg, DWORD dwInfoFlag = NIIF_INFO, HICON hIcon = NULL);
	USHORT	m_uActivateStyle;	// WM_LBUTTONDBLCLK, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP
	USHORT	m_uPopupMenuStyle;	// WM_LBUTTONDBLCLK, WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP
	NOTIFYICONDATA m_NotifyData;

protected:
	HMENU	m_hPopupMenu;
private:
	UINT	m_uTrayID;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAYEDDLG_H__364F5F81_B414_4C24_A570_62238213CB85__INCLUDED_)
