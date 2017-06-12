
// CntrItem.h : interface of the CAngineEditorCntrItem class
//

#pragma once

class CShaderDoc;
class CCodeView;

class CAngineEditorCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CAngineEditorCntrItem)

// Constructors
public:
	CAngineEditorCntrItem(REOBJECT* preo = NULL, CShaderDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer

// Attributes
public:
	CShaderDoc* GetDocument()
		{ return reinterpret_cast<CShaderDoc*>(CRichEditCntrItem::GetDocument()); }
	CCodeView* GetActiveView()
		{ return reinterpret_cast<CCodeView*>(CRichEditCntrItem::GetActiveView()); }

// Implementation
public:
	~CAngineEditorCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

