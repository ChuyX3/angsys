
// ProjectPropertiesView.cpp : implementation of the CProjectPropertiesView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Angine Editor.h"
#endif

#include "CProjectDoc.h"
#include "ProjectPropertiesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProjectPropertiesView

IMPLEMENT_DYNCREATE(CProjectPropertiesView, CView)

BEGIN_MESSAGE_MAP(CProjectPropertiesView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CProjectPropertiesView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CProjectPropertiesView construction/destruction

CProjectPropertiesView::CProjectPropertiesView()
{
	// TODO: add construction code here

}

CProjectPropertiesView::~CProjectPropertiesView()
{
}

BOOL CProjectPropertiesView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CProjectPropertiesView drawing

void CProjectPropertiesView::OnDraw(CDC* /*pDC*/)
{
	CProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CProjectPropertiesView printing


void CProjectPropertiesView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CProjectPropertiesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CProjectPropertiesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CProjectPropertiesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CProjectPropertiesView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CProjectPropertiesView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CProjectPropertiesView diagnostics

#ifdef _DEBUG
void CProjectPropertiesView::AssertValid() const
{
	CView::AssertValid();
}

void CProjectPropertiesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProjectDoc* CProjectPropertiesView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProjectDoc)));
	return (CProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CProjectPropertiesView message handlers
