// XmlFileDoc.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/XmlFileDoc.h"
#include <Tempus.h>
#include <StreamBuffer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXmlFileDoc

IMPLEMENT_DYNCREATE(CXmlFileDoc, CDocument)

CXmlFileDoc::CXmlFileDoc()
{
	xmlDoc = new ang::xml::XmlDocument();
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

		ang::IntfPtr<ang::IBuffer> buffer = ang::Buffer::NewBuffer((ang::WSizeT)ar.GetFile()->GetLength() + 1);
		ar.GetFile()->Read(buffer->BufferPtr(), buffer->BufferSize() - 1);
		ang::MString code;
		code.Set(buffer.Get());
		try {
			xmlDoc->Load(code);
		}
		catch (ang::Exception const& e)
		{
			TRACE(e.What());
		}
	}
	else
	{
		//storing code here
		ang::Streams::TextBufferOutputStream stream;
		stream.IsAutoAllocable(true);
		stream.EnableTextFormating(false);
		stream.TextEncoding(ang::Text::TextEncoding::Utf8);
		try {
			ang::Core::Tempus::DeltaTime delta;
			delta.Reset();
			xmlDoc->Save(stream);
			//mtext = text;
			delta.Update();
			ar.Write(stream.BufferInterface()->BufferPtr(), stream.Position());
		}
		catch (ang::Exception const& e)
		{
			TRACE(e.What());
		}
	}
}


// CXmlFileDoc commands
