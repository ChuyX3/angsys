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
	ang::String m_strTargetSelected;
	ang::String m_strSourceSelected;
	ang::Storage::List<ang::String> targetList, sourceList;

	CComboBox m_wndTargetProfileCombo;
	CComboBox m_wndCopyFromProfileCombo;

public:
	INT_PTR DoModalArgs(ang::Storage::List<ang::String > targetList, ang::Storage::List<ang::String> sourceList);
	ang::String GetTargetSelected()const { return m_strTargetSelected; }
	ang::String GetSourceSelected()const { return m_strSourceSelected; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
