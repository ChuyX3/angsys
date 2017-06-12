#pragma once



// CMFCFormView form view

class CMFCFormView : public CFormView
{
	DECLARE_DYNAMIC(CMFCFormView)

protected:
	CMFCFormView(UINT id);           // protected constructor used by dynamic creation
	virtual ~CMFCFormView();

protected:
	COLORREF m_clrBackground;
	CBrush m_brBackground;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	virtual void OnInitialUpdate();
};


