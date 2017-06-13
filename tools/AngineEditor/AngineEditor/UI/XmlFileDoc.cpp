// XmlFileDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/XmlFileDoc.h"
//#include <Tempus.h>
//#include <StreamBuffer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace ang;

// CXmlFileDoc

IMPLEMENT_DYNCREATE(CXmlFileDoc, CDocument)

CXmlFileDoc::CXmlFileDoc()
{
	xmlDoc = new ang::xml::xml_document();
}

BOOL CXmlFileDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

void CXmlFileDoc::OnCloseDocument()
{
	CDocument::OnCloseDocument();

	
}

CXmlFileDoc::~CXmlFileDoc()
{
	xmlDoc = nullptr;
}


BEGIN_MESSAGE_MAP(CXmlFileDoc, CDocument)
END_MESSAGE_MAP()


// CXmlFileDoc diagnostics

#ifdef _DEBUG
void CXmlFileDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CXmlFileDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CXmlFileDoc serialization

void CXmlFileDoc::Serialize(CArchive& ar)
{
	
	if (ar.IsLoading())
	{
		//loading code here
		wsize size = ar.GetFile()->GetLength();
		ang::mstring code = ""_sm;
		code->realloc(size, false);
		auto ptr = code->map_buffer(0, size);
		code->unmap_buffer(ptr, ar.GetFile()->Read(ptr, size));

		try {
			xmlDoc->parse(code);
		}
		catch (ang::exception_t e)
		{
			//TRACE(e.What());
		}
	}
	else
	{
		//storing code here
		ang::wstring code;
		try {
			xmlDoc->xml_tree()->xml_print(code, xml::xml_format::fix_entity + xml::xml_format::wrap_text_space);
			mstring mtext = code;
			ar.Write(mtext->cstr().get(), mtext->size());
		}
		catch (ang::exception_t e)
		{
			//TRACE(e.What());
		}
	}
}


// CXmlFileDoc commands
