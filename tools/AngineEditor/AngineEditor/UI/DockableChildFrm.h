#pragma once


// CDockableChildFrame

class CDockableChildFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CDockableChildFrame)

public:
	CDockableChildFrame();
	virtual ~CDockableChildFrame();

public:
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext);

protected:
	CDockablePane* m_pParentPane;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	
};


