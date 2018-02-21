
// CProjectDoc.h : interface of the CProjectDoc class
//


#pragma once

namespace angine
{
	using namespace ang;

	typedef object_wrapper<class project_manager> project_manager_t;


	class property
		: public object
	{
	public:
		property(string _name);
		property(string _name, object value);



	private:
		string _name;
		object _value;

	private:
		virtual~property();
	};

	class project_manager
		: public object
		, public singleton<project_manager_t>
	{
		friend singleton<project_manager_t>;

		project_manager();
		virtual~project_manager();

	public:


	private:
		xml::ixml_document_t m_document;
	};

}


class CProjectDoc : public CDocument
{
protected: // create from serialization only
	CProjectDoc();
	DECLARE_DYNCREATE(CProjectDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
