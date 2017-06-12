#pragma once


// CStackPanelCtrl

typedef struct tagSTACKPANELINFO
{
	CString strDisplayText;
	BOOL bIsVisible;
	int nMinWidth;
	BOOL bIsWidthAutoAdjust;
	CWnd *wndPanel;
}STACKPANELINFO, *LPSTACKPANELINFO;


class CStackPanelCtrl : public CWnd
{
	DECLARE_DYNAMIC(CStackPanelCtrl)

public:
	CStackPanelCtrl();
	virtual ~CStackPanelCtrl();

protected:
	BOOL m_bIsCreated;
	CSize m_szCtrlSize;
	CSize m_szCurrentSize;
	CPoint m_pnScrollPos;
	CList<LPSTACKPANELINFO> m_listPanles;
	
public:
	int GetCtrlHeight(void)const;
	int GetCtrlWidth(void)const;
	void SetCtrlWidth(int newSize);

	void AddPanel(LPCTSTR displayText, int minWidth,  BOOL widthAutoAdjust, CWnd *wndPanel);
	void ShowPanel(int idx, BOOL show);

public: //overrides
	virtual BOOL Create(CWnd * pParentWnd, RECT const& rect,
		UINT nStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		UINT nID = 0);

	virtual BOOL CreateEx(CWnd * pParentWnd, RECT const& rect,
		UINT nPaneStyle = 0, UINT nStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		UINT nID = 0);

	virtual void AdjustLayout();

protected:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreate);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
};


