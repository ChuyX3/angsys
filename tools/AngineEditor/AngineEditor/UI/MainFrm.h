
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "UI/DockableFrame.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	static inline CMainFrame* GetMainFrame() {
		auto app = AfxGetApp();
		if(app->m_pMainWnd == NULL || app->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMainFrame)) == FALSE)
			return NULL;
		return static_cast<CMainFrame*>(app->m_pMainWnd);
	}

	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd*       m_wndOutput;
	CPropertiesWnd    m_wndProperties;
	CList<CBasePane*> m_listDockablePanes;

public: //Properties
	inline CFileView*        GetFileViewPanel() { return &m_wndFileView; }
	inline CClassView*       GetClassViewPanel() { return &m_wndClassView; }
	inline COutputWnd*       GetOutputPanel() { return m_wndOutput; }
	inline CPropertiesWnd*   GetPropertiesPanel() { return &m_wndProperties; }

	virtual BOOL OnCloseMiniFrame(CPaneFrameWnd* /*pWnd*/);
	virtual BOOL OnCmdMsg(UINT id, int code, void *pExtra, AFX_CMDHANDLERINFO* pHandler);
public:
	CDockablePane* CreateDockablePane();
	BOOL RemoveDockablePane(CBasePane*);

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnClosePane(WPARAM, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


