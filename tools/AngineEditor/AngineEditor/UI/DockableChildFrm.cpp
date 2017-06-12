// UI/DockPaneChildFrm.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/DockableChildFrm.h"
#include "UI/DockableFrame.h"
#include "UI/MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDockableChildFrame

IMPLEMENT_DYNCREATE(CDockableChildFrame, CFrameWndEx)

CDockableChildFrame::CDockableChildFrame()
{
	m_pParentPane = NULL;
}

CDockableChildFrame::~CDockableChildFrame()
{

}



BEGIN_MESSAGE_MAP(CDockableChildFrame, CFrameWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CDockableChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	BOOL bRedraw = IsWindowVisible() && (GetStyle() & WS_MAXIMIZE) == 0;

	CString strTitle1;

	if (bRedraw)
	{
		GetWindowText(strTitle1);
	}

	CFrameWndEx::OnUpdateFrameTitle(bAddToTitle);

	if(m_pParentPane && !strTitle1.IsEmpty())
		m_pParentPane->SetWindowTextW(strTitle1);

}


// CDockableChildFrame message handlers

BOOL CDockableChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
	CWnd* pParentWnd, CCreateContext* pContext)
{
	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

								// parent must be MDI Frame (or NULL for default)

	// will be a child of MDIClient
	ASSERT(!(dwDefaultStyle & WS_POPUP));
	dwDefaultStyle |= WS_CHILD;

	CMainFrame* mainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CDockableFrame* dock = (CDockableFrame*)mainFrame->CreateDockablePane();
	dock->SetChildFrame(this);
	m_pParentPane = dock;
	pParentWnd = m_pParentPane;

	// if available - get MDI child menus from doc template
	CMultiDocTemplate* pTemplate;
	if (pContext != NULL &&
		(pTemplate = (CMultiDocTemplate*)pContext->m_pNewDocTemplate) != NULL)
	{
		ASSERT_KINDOF(CMultiDocTemplate, pTemplate);
		// get shared menu from doc template
		//m_hMenuShared = pTemplate->m_hMenuShared;
		m_hAccelTable = pTemplate->m_hAccelTable;
	}
	else
	{
		TRACE(traceAppMsg, 0, "Warning: no shared menu/acceltable for MDI Child window.\n");
		// if this happens, programmer must load these manually
	}

	CString strFullString, strTitle;
	if (strFullString.LoadString(nIDResource))
		AfxExtractSubString(strTitle, strFullString, 0);    // first sub-string

	ASSERT(m_hWnd == NULL);
	if (!Create(GetIconWndClass(dwDefaultStyle, nIDResource),
		strTitle, dwDefaultStyle, rectDefault,
		pParentWnd, NULL, 0, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	// it worked !
	return TRUE;
}


int CDockableChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	ModifyStyle(WS_OVERLAPPEDWINDOW, 0, 0);
	CRect rc;
	m_pParentPane->GetClientRect(&rc);
	SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOACTIVATE);
	return 0;
}
