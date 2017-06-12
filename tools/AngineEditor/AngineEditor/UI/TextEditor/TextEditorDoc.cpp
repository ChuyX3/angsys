// DemoEditorDoc.cpp : implementation of the CTextEditorDoc class
//

#include "stdafx.h"
#include "ShaderEditor.h"

#include "TextEditor\TextEditorDoc.h"
#include "TextEditor\TextEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextEditorDoc

IMPLEMENT_DYNCREATE(CTextEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CTextEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CTextEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEditorDoc construction/destruction

CTextEditorDoc::CTextEditorDoc()
{
	// TODO: add one-time construction code here

}

CTextEditorDoc::~CTextEditorDoc()
{
}

BOOL CTextEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTextEditorDoc serialization

void CTextEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTextEditorDoc diagnostics

#ifdef _DEBUG
void CTextEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTextEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextEditorDoc commands

BOOL CTextEditorDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CTextEditorView *pView = (CTextEditorView*)GetView();
	if (pView != NULL)
	{
		if(!pView->Load(lpszPathName)) return FALSE;
	}
	return TRUE;
}

BOOL CTextEditorDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CTextEditorView *pView = (CTextEditorView*)GetView();
	if (pView != NULL)
	{
		if(!pView->Save(lpszPathName)) return FALSE;
	}


	return TRUE;
}



CTextEditorView* CTextEditorDoc::GetView() const
{
	// find the first view - if there are no views
	// we must return NULL

	POSITION pos = GetFirstViewPosition();
	if (pos == NULL)
		return NULL;

	// find the first view that is a CRichEditView

	CView* pView;
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CTextEditorView)))
			return (CTextEditorView*) pView;
	}

	// can't find one--return NULL

	return NULL;
}
