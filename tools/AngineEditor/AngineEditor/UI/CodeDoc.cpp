// UI\CodeDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI\CodeDoc.h"


// CCodeDoc

IMPLEMENT_DYNCREATE(CCodeDoc, CDocument)

CCodeDoc::CCodeDoc()
{
}

BOOL CCodeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCodeDoc::~CCodeDoc()
{
}


BEGIN_MESSAGE_MAP(CCodeDoc, CDocument)
END_MESSAGE_MAP()


// CCodeDoc diagnostics

#ifdef _DEBUG
void CCodeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CCodeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CCodeDoc serialization

void CCodeDoc::Serialize(CArchive& ar)
{
	auto pos = this->GetFirstViewPosition();
	auto view = this->GetNextView(pos);
	if(view)
		view->Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CCodeDoc commands
