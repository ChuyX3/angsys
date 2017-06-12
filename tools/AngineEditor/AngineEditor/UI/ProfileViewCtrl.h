#pragma once
#include "afxwin.h"


// CProfileViewCtrl dialog

class CProfileViewCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CProfileViewCtrl)

public:
	CProfileViewCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProfileViewCtrl();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECTVIEW_PROFILES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CComboBox m_comboProfilePicker;
	CButton m_btnAddNew;
	CButton m_btnRemove;
};
