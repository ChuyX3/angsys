// SEProjectDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/AEProjectDoc.h"
#include "ProjectManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAEProjectDoc

IMPLEMENT_DYNCREATE(CAEProjectDoc, CDocument)

CAEProjectDoc::CAEProjectDoc()
{
	AngineEditor::CProjectManager::Instance();
}

BOOL CAEProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CAEProjectDoc::~CAEProjectDoc()
{
	AngineEditor::CProjectManager::ReleaseInstance();
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
	auto pm = AngineEditor::CProjectManager::Instance();
	if (ar.IsLoading())
	{
		// loading code
		ang::IntfPtr<ang::IBuffer> buffer = ang::Buffer::NewBuffer((ang::WSizeT)ar.GetFile()->GetLength() + 1);
		ar.GetFile()->Read(buffer->BufferPtr(), buffer->BufferSize() - 1);
		ang::MString code;
		code.Set(buffer.Get());
		pm->LoadDocument(code);
	
	}
	else
	{
		//storing code here
		ang::String text;
		ang::MString mtext;
		try {
			pm->SaveDocument(text);
			mtext = text;
			ar.Write(mtext.Data(), mtext.Length());
		}
		catch (ang::Exception const& e)
		{
			TRACE(e.What());
		}
	}
}
#endif


// CAEProjectDoc commands
