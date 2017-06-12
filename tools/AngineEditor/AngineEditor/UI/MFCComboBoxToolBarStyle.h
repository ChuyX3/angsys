#pragma once


// CMFCComboBoxToolBarStyle

class CMFCComboBoxToolBarStyle : public CComboBox
{
	DECLARE_DYNAMIC(CMFCComboBoxToolBarStyle)

public:
	CMFCComboBoxToolBarStyle();
	virtual ~CMFCComboBoxToolBarStyle();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNcPaint();
};


