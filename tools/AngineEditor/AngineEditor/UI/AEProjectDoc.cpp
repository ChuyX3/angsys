// SEProjectDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/AEProjectDoc.h"
#include "ProjectManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace ang;

// CAEProjectDoc

IMPLEMENT_DYNCREATE(CAEProjectDoc, CDocument)

CAEProjectDoc::CAEProjectDoc()
{
	AngineEditor::CProjectManager::instance();
}

BOOL CAEProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CAEProjectDoc::~CAEProjectDoc()
{
	AngineEditor::CProjectManager::release_instance();
}


BEGIN_MESSAGE_MAP(CAEProjectDoc, CDocument)
END_MESSAGE_MAP()


// CAEProjectDoc diagnostics

#ifdef _DEBUG
void CAEProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CAEProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CAEProjectDoc serialization

void CAEProjectDoc::Serialize(CArchive& ar)
{
	auto pm = AngineEditor::CProjectManager::instance();
	if (ar.IsLoading())
	{
		// loading code
		ang::mstring code = ""_s;
		wsize size = ar.GetFile()->GetLength();
		code->realloc(size + 1, false);
		auto ptr = code->map_buffer(0, size);
		code->unmap_buffer(ptr, ar.GetFile()->Read(ptr, size));
		pm->LoadDocument((ang::cmstr_t)code);
	
	}
	else
	{
		//storing code here
		ang::wstring text;
		ang::mstring mtext;
		try {
			pm->SaveDocument(text);
			mtext = text;
			ar.Write(mtext->cstr(), mtext->length());
		}
		catch (ang::exception_t e)
		{
			auto w = ang::move(e->what());
			TRACE(w->cstr().get());
		}
	}
}
#endif


// CAEProjectDoc commands
