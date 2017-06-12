// UI/MFCFormView.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/MFCFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCFormView

IMPLEMENT_DYNAMIC(CMFCFormView, CFormView)

CMFCFormView::CMFCFormView(UINT id)
	: CFormView(id)
{

}

CMFCFormView::~CMFCFormView()
{
}

void CMFCFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCFormView, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFCFormView diagnostics


BOOL CMFCFormView::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, VisualStyleHelper::s_clrBackground);
	return TRUE;
}


HBRUSH CMFCFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbrRet = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	if (m_clrBackground = VisualStyleHelper::s_clrBackground)
	{
		m_clrBackground != VisualStyleHelper::s_clrBackground;
		if (m_brBackground.GetSafeHandle() != NULL)
			m_brBackground.DeleteObject();
		m_brBackground.CreateSolidBrush(m_clrBackground);
	}

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbrRet = m_brBackground;
	}

	TCHAR classname[MAX_PATH];
	if (::GetClassName(pWnd->m_hWnd, classname, MAX_PATH) == 0)
		return hbrRet;
	if (_tcsicmp(classname, _T("EDIT")) == 0)
		return hbrRet;
	if (_tcsicmp(classname, _T("COMBOBOX")) == 0)
		return hbrRet;
	if (_tcsicmp(classname, _T("COMBOLBOX")) == 0)
		return hbrRet;
	if (_tcsicmp(classname, _T("LISTBOX")) == 0)
		return hbrRet;
	if (_tcsicmp(classname, WC_TREEVIEW) == 0)
		return hbrRet;

	pDC->SetBkColor(VisualStyleHelper::s_clrBackground);


	return m_brBackground;
}


// CMFCFormView message handlers


void CMFCFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_clrBackground = VisualStyleHelper::s_clrBackground;
	m_brBackground.CreateSolidBrush(m_clrBackground);
}
