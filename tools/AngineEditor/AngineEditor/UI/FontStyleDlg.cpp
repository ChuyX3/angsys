// UI\FontStyleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI\FontStyleDlg.h"
#include "afxdialogex.h"


// CFontStyleDlg dialog

IMPLEMENT_DYNAMIC(CFontStyleDlg, CDialogEx)

CFontStyleDlg::CFontStyleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEXTSTYLE_DLG, pParent)
	, m_bInitialized(FALSE)
	, m_nFontSize(0)
	, m_bBold(FALSE)
	, m_bItalic(FALSE)
	, m_bStrikeout(FALSE)
	, m_bUnderline(FALSE)
	, m_strCustomName(_T(""))
{

}

CFontStyleDlg::~CFontStyleDlg()
{
}

void CFontStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STYLEPICKER_COMBO, m_wndStylePicker);
	DDX_Control(pDX, IDC_TEXTFONT_COMBO, m_wndFontPicker);
	DDX_Text(pDX, IDC_FONTSIZE_EDIT, m_nFontSize);
	DDX_Check(pDX, IDC_BOLD_CHECK, m_bBold);
	DDX_Check(pDX, IDC_ITALIC_CHECK, m_bItalic);
	DDX_Check(pDX, IDC_STRIKEOUT_CHECK, m_bStrikeout);
	DDX_Check(pDX, IDC_UNDERLINE_CHECK, m_bUnderline);
	DDX_Control(pDX, IDC_FOREGROUNDCOLORPICKER_BUTTON, m_wndForegroundColor);
	DDX_Control(pDX, IDC_BACKGROUNDCOLORPICKER_BUTTON, m_wndBackgroundColor);
	DDX_Text(pDX, IDC_CUSTOM_EDIT, m_strCustomName);
	DDX_Control(pDX, IDC_SAMPLE_STATIC, m_wndSampleText);
}


BEGIN_MESSAGE_MAP(CFontStyleDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFontStyleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFontStyleDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BOLD_CHECK, &CFontStyleDlg::OnBnClickedBoldCheck)
	ON_BN_CLICKED(IDC_STRIKEOUT_CHECK, &CFontStyleDlg::OnBnClickedStrikeoutCheck)
	ON_BN_CLICKED(IDC_ITALIC_CHECK, &CFontStyleDlg::OnBnClickedItalicCheck)
	ON_BN_CLICKED(IDC_UNDERLINE_CHECK, &CFontStyleDlg::OnBnClickedUnderlineCheck)
	ON_BN_CLICKED(IDC_FOREGROUNDCOLORPICKER_BUTTON, &CFontStyleDlg::OnBnClickedForegroundcolorpickerButton)
	ON_BN_CLICKED(IDC_BACKGROUNDCOLORPICKER_BUTTON, &CFontStyleDlg::OnBnClickedBackgroundcolorpickerButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FONTSIZE_SPIN, &CFontStyleDlg::OnDeltaposFontsizeSpin)
	ON_EN_CHANGE(IDC_FONTSIZE_EDIT, &CFontStyleDlg::OnEnChangeFontsizeEdit)
	ON_CBN_SELCHANGE(IDC_TEXTFONT_COMBO, &CFontStyleDlg::OnCbnSelchangeTextfontCombo)
END_MESSAGE_MAP()


// CFontStyleDlg message handlers


BOOL CFontStyleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_bInitialized = TRUE;

	m_wndFontPicker.SelectFont(m_strFontName);
	m_wndForegroundColor.SetColor(m_nForegroundColor);
	m_wndBackgroundColor.SetColor(m_nBackgroundColor);

	return TRUE;
}


void CFontStyleDlg::OnBnClickedOk()
{
	UpdateData();
	m_strFontName = m_wndFontPicker.GetSelFont()->m_strName;

	m_nForegroundColor = m_wndForegroundColor.GetColor();
	m_nBackgroundColor = m_wndBackgroundColor.GetColor();

	m_bInitialized = FALSE;
	CDialogEx::OnOK();
}


void CFontStyleDlg::OnBnClickedCancel()
{
	m_bInitialized = FALSE;
	CDialogEx::OnCancel();
}


void CFontStyleDlg::UpdateSampleTextFont()
{
	if (m_wndSampleText.GetSafeHwnd())
	{
		UpdateData();

		if(m_sampleTextFont.GetSafeHandle())
			m_sampleTextFont.DeleteObject();
		auto style = GetFontInfo();
		LOGFONTW lf;
		memset(&lf, 0, sizeof(lf));
		// The negative is to allow for leading
		lf.lfHeight = -(abs(static_cast<int>(m_nFontSize + 0.5)));
		lf.lfWeight = m_bBold ? FW_BOLD : FW_NORMAL;
		lf.lfItalic = m_bItalic;
		lf.lfCharSet = ANSI_CHARSET;
		lf.lfQuality = ANTIALIASED_QUALITY;
		ang::StringOperation::CopyW(lf.lfFaceName, 32, m_strFontName.GetBuffer());

		m_sampleTextFont.CreateFontIndirectW(&lf);

		m_wndSampleText.SetFont(&m_sampleTextFont);

	}
}

void CFontStyleDlg::OnBnClickedBoldCheck()
{
	UpdateSampleTextFont();
}


void CFontStyleDlg::OnBnClickedStrikeoutCheck()
{
	UpdateSampleTextFont();
}


void CFontStyleDlg::OnBnClickedItalicCheck()
{
	UpdateSampleTextFont();
}


void CFontStyleDlg::OnBnClickedUnderlineCheck()
{
	UpdateSampleTextFont();
}


void CFontStyleDlg::OnBnClickedForegroundcolorpickerButton()
{
	if (m_wndSampleText.GetSafeHwnd())
	{
		m_wndSampleText.Invalidate();
	}
}


void CFontStyleDlg::OnBnClickedBackgroundcolorpickerButton()
{
	if (m_wndSampleText.GetSafeHwnd())
	{
		m_wndSampleText.Invalidate();
	}
}


void CFontStyleDlg::OnDeltaposFontsizeSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	m_nFontSize += pNMUpDown->iDelta;

	UpdateData(FALSE);

	*pResult = 0;
}


void CFontStyleDlg::OnEnChangeFontsizeEdit()
{
	UpdateSampleTextFont();
}


void CFontStyleDlg::OnCbnSelchangeTextfontCombo()
{
	UpdateSampleTextFont();
}
