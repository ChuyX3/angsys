#pragma once

#include "UI/MFCFormView.h"

// CAEProjectPropView form view

class CAEProjectPropView : public CMFCFormView
{
	DECLARE_DYNCREATE(CAEProjectPropView)

protected:
	CAEProjectPropView();           // protected constructor used by dynamic creation
	virtual ~CAEProjectPropView();

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bCreated;
	CMFCTabCtrl m_wndTabCtrl;
	CListCtrl m_wndFileList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual void OnInitialUpdate();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


