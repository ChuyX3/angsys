#pragma once
#include "afxwin.h"
#include "afxbutton.h"
#include "UI/MFCButtonToolBarStyle.h"
#include "UI/MFCComboBoxToolbarStyle.h"


// CPickerCtrl dialog

class CPickerCtrl : 
	public CStatic
{
	DECLARE_DYNAMIC(CPickerCtrl)
public:
	enum Buttons : UINT
	{
		ADD_BUTTON = 16,
		SUB_BUTTON = 8,
		MOD_BUTTON = 4,
		UP_BUTTON = 2,
		DOWN_BUTTON = 1,
		ALL_BUTTON = 31
	};

public:
	CPickerCtrl(UINT buttons = ALL_BUTTON, BOOL bIsDropList = FALSE);   // standard constructor
	virtual ~CPickerCtrl();



protected:
	BOOL m_bIsCreated;
	BOOL m_bIsEnable;
	BOOL m_bIsDropList;
	UINT m_nButtons;
	CFont m_font;
	CComboBox m_wndCombobox;
	CMFCButtonToolBarStyle m_wndButtons[5];

public:
	void ModifyItemStyle(int idx, DWORD remove, DWORD add);
	CComboBox* GetComboBox();

	BOOL Enable(BOOL bEnable = TRUE);

public:
	AngineEditor::mfc_event<CPickerCtrl> buttonClickedEvent;
	AngineEditor::mfc_event<CPickerCtrl> pickerControlSelChangeEvent;
	AngineEditor::mfc_event<CPickerCtrl> pickerControlTextChangeEvent;

	virtual void AdjustLayout();

protected:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnPickerControlSelchange();
	afx_msg void OnPickerControlEditchange();
	afx_msg void OnButtonClicked(UINT id);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
