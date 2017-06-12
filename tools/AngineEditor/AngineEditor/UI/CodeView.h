
// ShaderView.h : interface of the CCodeView class
//

#include "UI/LexerCtrl.h"

#pragma once

class CCodeView : public CView
{
protected: // create from serialization only
	CCodeView();
	DECLARE_DYNCREATE(CCodeView)

// Attributes
public:
	CCodeDoc* GetDocument() const;

protected:
	CLexerCtrl m_wndLexerCtrl;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CCodeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Serialize(CArchive& ar);

protected:
	virtual void OnDraw(CDC*);

// Generated message map functions
protected:
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditFind();
	afx_msg void OnEditPaste();
	afx_msg void OnEditRedo();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditReplace();
	afx_msg void OnEditUndo();
	afx_msg void OnFileSave();
};

#ifndef _DEBUG  // debug version in ShaderView.cpp
inline CCodeDoc* CCodeView::GetDocument() const
   { return reinterpret_cast<CCodeDoc*>(m_pDocument); }
#endif

