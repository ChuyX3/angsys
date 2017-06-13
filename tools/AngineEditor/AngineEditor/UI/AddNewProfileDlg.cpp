// UI\AddNewProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI\AddNewProfileDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAddNewProfileDlg dialog

IMPLEMENT_DYNAMIC(CAddNewProfileDlg, CDialogEx)

CAddNewProfileDlg::CAddNewProfileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDNEWPROFILEDLG, pParent)
	, m_strTargetSelected()
	, m_strSourceSelected()
{

}

CAddNewProfileDlg::~CAddNewProfileDlg()
{
}

void CAddNewProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDNEWPROFILE_TARGET_COMBO, m_wndTargetProfileCombo);
	DDX_Control(pDX, IDC_ADDNEWPROFILE_COPYFROM_COMBO, m_wndCopyFromProfileCombo);
}


BEGIN_MESSAGE_MAP(CAddNewProfileDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddNewProfileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddNewProfileDlg message handlers


void CAddNewProfileDlg::OnBnClickedOk()
{
	auto idx = m_wndTargetProfileCombo.GetCurSel();
	if (idx >= 0)
		m_strTargetSelected = targetList[idx];

	idx = m_wndCopyFromProfileCombo.GetCurSel();
	if (idx >= 0)
		m_strSourceSelected = sourceList[idx];
	CDialogEx::OnOK();
}

INT_PTR CAddNewProfileDlg::DoModalArgs(ang::static_array<ang::wstring> target, ang::static_array<ang::wstring> source) {
	targetList = target;
	sourceList = source;
	return DoModal();
}


BOOL CAddNewProfileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ang::static_array<ang::wstring> target_view = targetList;
	for (ang::wstring& value : target_view) {
		m_wndTargetProfileCombo.AddString((ang::cwstr_t)value);
	}

	ang::static_array<ang::wstring> source_view = sourceList;
	for (ang::wstring& value : source_view) {
		m_wndCopyFromProfileCombo.AddString((ang::cwstr_t)value);
	}

	m_wndTargetProfileCombo.SetCurSel(0);
	m_wndCopyFromProfileCombo.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
