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
	auto it = m_colorInfo.FindIndex(m_wndCategoryList.GetCurSel());
	if (it.IsValid())
	{
		m_wndColorButton.SetColor(it->Datum().ColorRef());
	}
}


void CSyntaxColorPickerDlg::OnColorPickerChangeColor()
{
	auto it = m_colorInfo.FindIndex(m_wndCategoryList.GetCurSel());
	if (it.IsValid())
	{
		it->Datum().ColorRef(m_wndColorButton.GetColor());
	}
}

BOOL CSyntaxColorPickerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_wndCategoryList.ResetContent();

	if (m_colorInfo.Counter() != 0)
	{
		int i = 0;
		for (auto it = m_colorInfo.Begin(); it.IsValid(); ++it, ++i)
		{
			m_wndCategoryList.InsertString(i, it->KeyValue());
		}
		m_wndCategoryList.SetCurSel(0);

		m_wndColorButton.SetColor(m_colorInfo[0].Datum().ColorRef());
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
