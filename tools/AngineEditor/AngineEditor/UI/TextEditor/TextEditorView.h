// DemoEditorView.h : interface of the CDemoEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


#include "Parser\SyntaxColorSpellChecker.h"

class CTextEditorView : public CCtrlView
{
protected: // create from serialization only
	CTextEditorView();
	DECLARE_DYNCREATE(CTextEditorView)

// Attributes
public:
	CTextEditorDoc* GetDocument();

private:

	CSyntaxColorSpellChecker m_colorizer;
	// some stuff
	CString			m_strOrgSuggest;
	CStringArray	m_tSuggestlist;
	int				m_bChangeAll;
	int				m_nextedComments;
	int				m_bSpellEnabled;
	BOOL			m_bInContextMenu;
	EDITSTREAM		m_es;

	CHARRANGE		m_ContextRange;
	bool			readFile(LPCTSTR  sFileName);
	bool			writeFile(LPCTSTR  sFileName);
public:

	BOOL			Load(LPCTSTR file);
	BOOL			Save(LPCTSTR file);

	void			OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu);
	void			UpdateDynaMenu(CCmdUI* pCmdUI,CStringArray & list,CString & m_strOriginal);

	CRichEditCtrl& GetRichEditCtrl() const
					{ return *(CRichEditCtrl*)this; }




// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	//}}AFX_VIRTUAL

	afx_msg void OnChangeEdit() ;
	afx_msg void OnMsgfilterEdit(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditUndo();


	void OnSuggestUI(CCmdUI* pCmdUI);
	void OnSuggest(UINT id );
	afx_msg void OnAddtodictionnary();
	afx_msg void OnUpdateAddtodictionnary(CCmdUI* pCmdUI);
	afx_msg void OnIgnoralways();
	afx_msg void OnUpdateIgnoralways(CCmdUI* pCmdUI);

// Implementation
public:
	virtual ~CTextEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemoEditorView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DemoEditorView.cpp
inline CDemoEditorDoc* CDemoEditorView::GetDocument()
   { return (CDemoEditorDoc*)m_pDocument; }
#endif

