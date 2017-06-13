#pragma once
#include "afxwin.h"
#include "afxcolorbutton.h"


// CSyntaxColorPickerDlg dialog

class CSyntaxColorPickerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSyntaxColorPickerDlg)

public:
	CSyntaxColorPickerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSyntaxColorPickerDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	CListBox m_wndCategoryList;
	CMFCColorButton m_wndColorButton;
	ang::collections::vector<ang::collections::pair<ang::string, ang::graphics::color>> m_colorInfo;

public:
	void SetColorInfo(ang::static_array<ang::collections::pair<ang::string, ang::graphics::color>> colors) {
		m_colorInfo = nullptr;
		for (ang::collections::pair<ang::string, ang::graphics::color>& pair : colors)
		{
			m_colorInfo += pair;
		}
	}

	ang::static_array<ang::collections::pair<ang::string, ang::graphics::color>> GetColorInfo()const {
		return m_colorInfo;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCategorySelChange();
	afx_msg void OnColorPickerChangeColor();

	
};
