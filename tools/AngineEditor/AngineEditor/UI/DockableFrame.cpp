// UI/ResourceView.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/DockableFrame.h"
#include "UI/MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CResourceView

IMPLEMENT_DYNAMIC(CDockableFrame, CDockablePane)

CDockableFrame::CDockableFrame()
{
	m_pChildFrame = NULL;
}

CDockableFrame::~CDockableFrame()
{
}

BEGIN_MESSAGE_MAP(CDockableFrame, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CResourceView message handlers

int CDockableFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) != 0)
		return -1;

	return 0;
}

void CDockableFrame::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_pChildFrame->GetSafeHwnd())
	{
		m_pChildFrame->SetWindowPos(nullptr, 0, 0, cx, cy, SWP_NOACTIVATE);
	}
}

void CDockableFrame::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
}


void CDockableFrame::OnDestroy()
{
	if (m_pChildFrame->GetSafeHwnd() && ::IsWindow(m_pChildFrame->GetSafeHwnd()))
	{
		m_pChildFrame->SendMessage(WM_CLOSE);
		m_pChildFrame = NULL;
	}

	((CMainFrame*)AfxGetApp()->m_pMainWnd)->RemoveDockablePane(this);

	CDockablePane::OnDestroy();
	delete this;
	// TODO: Add your message handler code here
}
