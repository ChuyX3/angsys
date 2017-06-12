#pragma once
#include "afxwin.h"
#include "afxcolorbutton.h"
#include "afxfontcombobox.h"
#include "LexerCtrl.h"


// CFontStyleDlg dialog
class CFontStyleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFontStyleDlg)

public:
	CFontStyleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFontStyleDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTSTYLE_DLG };
#endif

protected:
	BOOL m_bInitialized;
	CComboBox m_wndStylePicker;
	CMFCFontComboBox m_wndFontPicker;
	CMFCColorButton m_wndForegroundColor;
	CMFCColorButton m_wndBackgroundColor;
	CStatic m_wndSampleText;
	CString m_strFontName;
	COLORREF m_nForegroundColor;
	COLORREF m_nBackgroundColor;
	int m_nFontSize;
	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bStrikeout;
	BOOL m_bUnderline;
	CString m_strCustomName;
	CFont m_sampleTextFont;
	//ang::Storage::

protected:
	void UpdateSampleTextFont();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual BOOL OnInitDialog();

	FontStyle GetFontInfo() {
		if(m_bInitialized)UpdateData(TRUE);
		return{ m_strFontName,m_nForegroundColor,m_nBackgroundColor,m_nFontSize,(bool)m_bBold,(bool)m_bItalic,(bool)m_bStrikeout,(bool)m_bUnderline };
	}

	void SetFontInfo(FontStyle style) {
		m_strFontName = style.strFontName.Data();
		m_nForegroundColor = style.nForegroundColor.ColorRef();
		m_nBackgroundColor = style.nBackgroundColor.ColorRef();
		m_nFontSize = style.nFontSize;
		m_bBold = style.bBold;
		m_bItalic = style.bItalic;
		m_bStrikeout = style.bStrikeout;
		m_bUnderline = style.bUnderline;
		if(m_bInitialized)UpdateData(FALSE);
	}

	void SetFontInfo(
		LPCTSTR fontName, 
		COLORREF foregroundColor, 
		COLORREF backgroundColor, 
		int fontSize, 
		BOOL bold, 
		BOOL italic,
		BOOL strikeout, 
		BOOL underline)
	{
		m_strFontName = fontName;
		m_nForegroundColor = foregroundColor;
		m_nBackgroundColor = backgroundColor;
		m_nFontSize = fontSize;
		m_bBold = bold;
		m_bItalic = italic;
		m_bStrikeout = strikeout;
		m_bUnderline = underline;

		if (m_bInitialized)UpdateData(FALSE);
	}

protected:
	DECLARE_MESSAGE_MAP()	
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBoldCheck();
	afx_msg void OnBnClickedStrikeoutCheck();
	afx_msg void OnBnClickedItalicCheck();
	afx_msg void OnBnClickedUnderlineCheck();
	afx_msg void OnBnClickedForegroundcolorpickerButton();
	afx_msg void OnBnClickedBackgroundcolorpickerButton();
	afx_msg void OnDeltaposFontsizeSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeFontsizeEdit();
	afx_msg void OnCbnSelchangeTextfontCombo();
};

