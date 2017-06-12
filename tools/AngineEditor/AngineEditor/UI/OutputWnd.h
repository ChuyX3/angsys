
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public CEdit
{
// Construction
public:
	COutputList();

// Implementation
public:
	virtual ~COutputList();
	void AddString(LPCTSTR text);

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd :
	public CDockablePane,
	public ang::singleton<COutputWnd>
{
	friend ang::singleton<COutputWnd>;
// Construction
public:
	COutputWnd();

	void UpdateFonts();

// Attributes
protected:
	CMFCTabCtrl	m_wndTabs;

	COutputList m_wndOutputBuild;
	COutputList m_wndOutputDebug;
	COutputList m_wndOutputFind;

public:
	void WriteBuildWindow(LPCTSTR text);
	void WriteDebugWindow(LPCTSTR text);
	void WriteFindWindow(LPCTSTR text);

	void AdjustHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

