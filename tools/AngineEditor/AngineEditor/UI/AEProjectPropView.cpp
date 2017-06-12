// SEProjectPropView.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/AEProjectPropView.h"
#include "ProjectManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAEProjectPropView

IMPLEMENT_DYNCREATE(CAEProjectPropView, CMFCFormView)

CAEProjectPropView::CAEProjectPropView()
	: CMFCFormView(IDD_PROJECT_PROPERTIES)
	, m_bCreated(FALSE)
{

}

CAEProjectPropView::~CAEProjectPropView()
{
}

void CAEProjectPropView::DoDataExchange(CDataExchange* pDX)
{
	CMFCFormView::DoDataExchange(pDX);
}

void CAEProjectPropView::OnInitialUpdate()
{
	CMFCFormView::OnInitialUpdate();
	CRect rc;
	GetClientRect(rc);
	rc.DeflateRect(5, 5, 5, 5);
	m_wndTabCtrl.Create(CMFCTabCtrl::Style::STYLE_3D_ONENOTE, rc, this,-1, CMFCBaseTabCtrl::LOCATION_TOP);

	m_wndFileList.Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS, rc, &m_wndTabCtrl, 100);

	m_wndTabCtrl.AddTab(&m_wndFileList, TEXT("Files"));

	m_wndFileList.InsertColumn(0, TEXT("File ID"), LVCFMT_LEFT, 200);
	m_wndFileList.InsertColumn(1, TEXT("File Name"), LVCFMT_LEFT, 200);
	m_wndFileList.InsertColumn(2, TEXT("Full File Name"), LVCFMT_LEFT, 200);

	ang::Storage::BTree<ang::AString, ang::String> &fileMap = AngineEditor::CProjectManager::Instance()->FileMap();

	int i = 0;
	for (auto it = fileMap.Begin(); it.IsValid(); ++it, ++i)
	{
		ang::String key = it->KeyValue();
		m_wndFileList.InsertItem(i, TEXT(""));
		m_wndFileList.SetItemText(i, 1, key.Data());
		m_wndFileList.SetItemText(i, 2, it->Datum().Data());
	}

	m_bCreated = TRUE;
}


BEGIN_MESSAGE_MAP(CAEProjectPropView, CMFCFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAEProjectPropView diagnostics

#ifdef _DEBUG
void CAEProjectPropView::AssertValid() const
{
	CMFCFormView::AssertValid();
}

void CAEProjectPropView::Dump(CDumpContext& dc) const
{
	CMFCFormView::Dump(dc);
}

#endif //_DEBUG

// CAEProjectPropView message handlers

void CAEProjectPropView::OnSize(UINT nType, int cx, int cy)
{
	CMFCFormView::OnSize(nType, cx, cy);

	if (m_bCreated)
	{
		m_wndTabCtrl.SetWindowPos(NULL, 0, 0, cx - 10, cy - 10, SWP_NOACTIVATE | SWP_NOMOVE);
		CRect rc;
		m_wndFileList.GetClientRect(rc);
		m_wndFileList.SetColumnWidth(1,(rc.Width() - 200) / 3);
		m_wndFileList.SetColumnWidth(2, 2*(rc.Width() - 200) / 3);
	}
}
