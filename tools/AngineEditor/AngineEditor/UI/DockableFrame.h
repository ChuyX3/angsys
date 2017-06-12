#pragma once

// CResourceEditor

// CResourceView

class CDockableFrame : public CDockablePane
{
	DECLARE_DYNAMIC(CDockableFrame)

public:
	CDockableFrame();
	virtual ~CDockableFrame();

protected:
	CFrameWnd* m_pChildFrame;
	CMFCToolBar m_wndToolBar;

public:
	CFrameWnd *GetChildFrame()const { return m_pChildFrame; }
	void SetChildFrame(CFrameWnd * frm) { 
		m_pChildFrame = frm;
		if (m_pChildFrame->GetSafeHwnd())
		{
			m_pChildFrame->SetParent(this);
			CRect rc;
			GetClientRect(rc);
			m_pChildFrame->SetWindowPos(NULL, 0,0,rc.Width(), rc.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		}		
	}

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	afx_msg void OnDestroy();
	afx_msg void OnClose();
};


