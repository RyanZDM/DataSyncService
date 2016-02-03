#pragma once

#include <vector>
using namespace std;

typedef basic_string<TCHAR> TString;

typedef struct tagTABPAGE_INFO
{
	LONG		lIndex;
	CDialog*	pDlg;
	TString		szTitle;
	TString		szImage;

	tagTABPAGE_INFO() {lIndex = -1; pDlg = NULL; szTitle = _T("");}
	tagTABPAGE_INFO(LONG index, CDialog* p, LPCTSTR pszTitle, LPCTSTR pszImage)
	{
		lIndex = index;
		pDlg = p;
		szTitle = (pszTitle)?pszTitle:_T("");
		szImage = (pszImage)?pszImage:_T("");
	}

	~tagTABPAGE_INFO() {if (pDlg) delete pDlg;}

} TABPAGE_INFO, *PTABPAGE_INFO;

// CTabCtrlEx
#define	DEFAULT_L_THICKNESS	5		// Default thickness of left border
#define	DEFAULT_R_THICKNESS	5		// Default thickness of right border
#define	DEFAULT_T_THICKNESS	5		// Default thickness of top border
#define	DEFAULT_B_THICKNESS	5		// Default thickness of bottom border


class CTabCtrlEx : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlEx)
public:
	vector<PTABPAGE_INFO> m_vCtrls;		// Contain all reference to tab pages included in this container

public:	
	// Show/Hide a tab page and return its pointer, return NULL if not found
	CDialog* Show(DWORD dwIndex, BOOL bFlag);
	// Enable/Diable a tab page and return its pointer, return NULL if not found
	CDialog* Enable(DWORD dwIndex, BOOL bFlag);
	// Add tag page(s) to this container using dialog ID
	LONG AddTabPage(int nIndex, DWORD dwDiagID, LPCTSTR pszTitle, LPCTSTR pszImage = NULL);
	LONG AddTabPages(int nIndex, const DWORD *pdwDiagIDArray, const char** paTitles, const char** paImages = NULL);
	LONG AddTabPages(int nIndex, const vector<DWORD> vDiagIDs, const vector<const char*> vTitles, const vector<const char*> vImages);
	// Change control size
	void SetLeftBorderThickness(UINT uSize)		{m_uLeftThickness = uSize;}
	void SetRightBorderThickness(UINT uSize)	{m_uRightThickness = uSize;}
	void SetTopBorderThickness(UINT uSize)		{m_uTopThickness = uSize;}
	void SetBottomBorderThickness(UINT uSize)	{m_uBottomThickness = uSize;}
	void SetBorderThickness(UINT uLeft, UINT uTop, UINT uRight, UINT uBottom)	{m_uLeftThickness = uLeft;
																				 m_uRightThickness = uRight;
																				 m_uTopThickness = uTop;
																				 m_uBottomThickness = uBottom;}

	CTabCtrlEx();
	virtual ~CTabCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()

private:
	UINT m_uLeftThickness;			// Thickness of left border
	UINT m_uRightThickness;			// Thickness of right border
	UINT m_uTopThickness;			// Thickness of top border
	UINT m_uBottomThickness;		// Thickness of bottom border
	UINT m_uLTabOffset;				// Offset when tabs appear at the left side of the control
	UINT m_uRTabOffset;				// Offset when tabs appear at the right side of the control
	UINT m_uTTabOffset;				// Offset when tabs appear at the top side of the control
	UINT m_uBTabOffset;				// Offset when tabs appear at the bottom side of the control
	void Clear();
	BOOL GetTabOffset();			// Get the offset according to position of tab
	void ResizePage(UINT uIndex);	// Resize a tab page according to the size of this TabControl
	void ResizePages();				// Resize all tab pages according to the size of this TabControl, will be called when added a new tab page or resized TabControl
	
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


