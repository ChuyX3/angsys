// UI\SEProjectPropFrm.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/AEProjectPropFrm.h"
#include "ProjectDocTemplate.h"
#include <afxdatarecovery.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAEProjectPropFrame

IMPLEMENT_DYNCREATE(CAEProjectPropFrame, CMDIChildWndEx)

CAEProjectPropFrame* CAEProjectPropFrame::instance = NULL;

CAEProjectPropFrame* CAEProjectPropFrame::Instance()
{
	return instance;
}

CAEProjectPropFrame::CAEProjectPropFrame()
{
	instance = this;
}

CAEProjectPropFrame::~CAEProjectPropFrame()
{
	instance = nullptr;
}


BEGIN_MESSAGE_MAP(CAEProjectPropFrame, CMDIChildWndEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
/*

void CAEProjectPropFrame::InitialUpdateFrame(CDocument* pDoc, BOOL bMakeVisible)
{
	// if the frame does not have an active view, set to first pane
	CView* pView = NULL;
	if (GetActiveView() == NULL)
	{
		CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			pView = (CView*)pWnd;
			SetActiveView(pView, FALSE);
		}
	}

	if (bMakeVisible)
	{
		// send initial update to all views (and other controls) in the frame
		SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);

		// give view a chance to save the focus (CFormView needs this)
		if (pView != NULL)
			pView->OnActivateFrame(WA_INACTIVE, this);

		// finally, activate the frame
		// (send the default show command unless the main desktop window)
		int nCmdShow = -1;      // default
		CWinApp* pApp = AfxGetApp();
		if (pApp != NULL && pApp->m_pMainWnd == this)
		{
			nCmdShow = pApp->m_nCmdShow; // use the parameter from WinMain
			pApp->m_nCmdShow = -1; // set to default after first time
		}
		ActivateFrame(nCmdShow);
		if (pView != NULL)
			pView->OnActivateView(TRUE, pView, pView);
	}

	// update frame counts and frame title (may already have been visible)
	if (pDoc != NULL)
		pDoc->UpdateFrameCounts();
	OnUpdateFrameTitle(TRUE);
}
*/

BOOL CAEProjectPropFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
	CWnd* pParentWnd, CCreateContext* pContext)
{
	// only do this once
	ASSERT_VALID_IDR(nIDResource);
	ASSERT(m_nIDHelp == 0 || m_nIDHelp == nIDResource);

	m_nIDHelp = nIDResource;    // ID for help context (+HID_BASE_RESOURCE)

								// parent must be MDI Frame (or NULL for default)
	ASSERT(pParentWnd == NULL || pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
	// will be a child of MDIClient
	ASSERT(!(dwDefaultStyle & WS_POPUP));
	dwDefaultStyle |= WS_CHILD;

	// if available - get MDI child menus from doc template
	CProjectDocTemplate* pTemplate;
	if (pContext != NULL &&
		(pTemplate = (CProjectDocTemplate*)pContext->m_pNewDocTemplate) != NULL)
	{
		ASSERT_KINDOF(CProjectDocTemplate, pTemplate);
		// get shared menu from doc template
		m_hMenuShared = pTemplate->m_hMenuShared;
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
		(CMDIFrameWnd*)pParentWnd, pContext))
	{
		return FALSE;   // will self destruct on failure normally
	}

	// it worked !
	return TRUE;
}

// CAEProjectPropFrame message handlers




void CAEProjectPropFrame::OnClose()
{
	//// Note: only queries the active document
	CDocument* pDocument = GetActiveDocument();
	if (pDocument != NULL)
	{
		pDocument->SaveModified();
	}
	//// then destroy the window
//	ShowWindow(SW_HIDE);
	DestroyWindow();
//	CMDIChildWndEx::OnClose();
}
