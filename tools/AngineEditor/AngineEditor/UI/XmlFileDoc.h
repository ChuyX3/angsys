#pragma once

// CXmlFileDoc document

class CXmlFileDoc : public CDocument
{
	DECLARE_DYNCREATE(CXmlFileDoc)

public:
	CXmlFileDoc();
	virtual ~CXmlFileDoc();
	virtual void Serialize(CArchive& ar);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ang::xml::xml_document_t xmlDoc;

public:
	inline ang::xml::xml_node_t GetXmlRootElement()const {
		return xmlDoc->xml_root();
	}

	inline ang::xml::xml_builder_t GetXmlBuilder()const {
		return xmlDoc->xml_tree()->as<ang::xml::xml_builder>();
	}

protected:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();

	DECLARE_MESSAGE_MAP()
};
