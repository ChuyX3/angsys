#pragma once
#include "afxwin.h"


// CAddNewProfileDlg dialog

class CAddNewProfileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddNewProfileDlg)

public:
	CAddNewProfileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddNewProfileDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDNEWPROFILEDLG };
#endif

protected:
	ang::wstring m_strTargetSelected;
	ang::wstring m_strSourceSelected;
	ang::collections::vector<ang::wstring> targetList, sourceList;

	CComboBox m_wndTargetProfileCombo;
	CComboBox m_wndCopyFromProfileCombo;

public:
	INT_PTR DoModalArgs(ang::static_array<ang::wstring > targetList, ang::static_array<ang::wstring> sourceList);
	ang::wstring GetTargetSelected()const { return m_strTargetSelected; }
	ang::wstring GetSourceSelected()const { return m_strSourceSelected; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
