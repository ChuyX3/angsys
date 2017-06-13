// UI\SintaxColorPickerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI\SyntaxColorPickerDlg.h"
#include "afxdialogex.h"


// CSyntaxColorPickerDlg dialog

IMPLEMENT_DYNAMIC(CSyntaxColorPickerDlg, CDialogEx)

CSyntaxColorPickerDlg::CSyntaxColorPickerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CSyntaxColorPickerDlg::~CSyntaxColorPickerDlg()
{
}

void CSyntaxColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CATEGORY_LIST, m_wndCategoryList);
	DDX_Control(pDX, IDC_COLORBUTTON, m_wndColorButton);
}


BEGIN_MESSAGE_MAP(CSyntaxColorPickerDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_CATEGORY_LIST, &CSyntaxColorPickerDlg::OnCategorySelChange)
	ON_BN_CLICKED(IDC_COLORBUTTON, &CSyntaxColorPickerDlg::OnColorPickerChangeColor)
END_MESSAGE_MAP()

// CSyntaxColorPickerDlg message handlers


void CSyntaxColorPickerDlg::OnCategorySelChange()
{
	auto idx = m_wndCategoryList.GetCurSel();
	if (!m_colorInfo.is_empty() && m_colorInfo->counter() > idx)
		m_wndColorButton.SetColor(m_colorInfo[idx].value().code);
}


void CSyntaxColorPickerDlg::OnColorPickerChangeColor()
{
	auto idx = m_wndCategoryList.GetCurSel();
	if (!m_colorInfo.is_empty() && m_colorInfo->counter() > idx)
		m_colorInfo[idx].value().code = m_wndColorButton.GetColor();
}

BOOL CSyntaxColorPickerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_wndCategoryList.ResetContent();
	ang::static_array<ang::collections::pair<ang::string, ang::graphics::color>> view = m_colorInfo;

	int i = 0;
	for (auto& pair : view)
	{
		auto text = ang::interop::string_cast<CString>(pair.key_value());
		m_wndCategoryList.InsertString(i++, text);
	}
	m_wndCategoryList.SetCurSel(0);
	m_wndColorButton.SetColor(m_colorInfo.get() ? m_colorInfo[0].value().code : 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
