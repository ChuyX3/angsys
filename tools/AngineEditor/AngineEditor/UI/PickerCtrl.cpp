// PickerCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "afxdialogex.h"
#include "UI/PickerCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace ang;

#define IDC_ADD_BUTTON		1001
#define IDC_SUB_BUTTON		1002
#define IDC_MOD_BUTTON		1003
#define IDC_UP_BUTTON		1004
#define IDC_DOWN_BUTTON		1005
#define IDC_COMBOBOX	1006

// CPickerCtrl dialog

IMPLEMENT_DYNAMIC(CPickerCtrl, CStatic)

CPickerCtrl::CPickerCtrl(UINT buttons, BOOL bIsDropList)
	: m_bIsCreated(FALSE)
	, m_bIsEnable(TRUE)
	, m_nButtons(ALL_BUTTON&buttons)
	, m_bIsDropList(bIsDropList)
{
	
}

CPickerCtrl::~CPickerCtrl()
{
}


void  CPickerCtrl::ModifyItemStyle(int idx, DWORD remove, DWORD add)
{
	switch (idx)
	{
	case 5:
		m_wndCombobox.ModifyStyle(remove, add);
		break;
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		m_wndButtons[idx].ModifyStyle(remove, add);
		break;
	default: break;
	}
	UpdateData(0);
}

CComboBox* CPickerCtrl::GetComboBox()
{
	return &m_wndCombobox;
}

BOOL CPickerCtrl::Enable(BOOL bEnable)
{
	if (m_bIsEnable == bEnable)
		return m_bIsEnable;
	m_bIsEnable = bEnable;
	m_wndButtons[0].EnableWindow(bEnable);
	m_wndButtons[1].EnableWindow(bEnable);
	m_wndButtons[2].EnableWindow(bEnable);
	m_wndButtons[3].EnableWindow(bEnable);
	m_wndButtons[4].EnableWindow(bEnable);
	m_wndCombobox.EnableWindow(bEnable);
	return !m_bIsEnable;
}

void CPickerCtrl::AdjustLayout()
{
	if (!m_bIsCreated)
		return;
	CRect rc;
	GetClientRect(rc);

	int x = rc.Width();
	int buttons = m_nButtons;
	int idx = 4;
	while (idx >= 0)
	{
		if (buttons & 0X01)
		{
			x -= 26;
			m_wndButtons[idx].SetWindowPos(NULL, x, 1, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
		}
		else
		{
			//m_wndButtons[idx].ShowWindow(SW_HIDE);
		}
		buttons = buttons >> 1;
		idx--;
	}
	m_wndCombobox.SetWindowPos(NULL, 1, 1, x - 2, 23, SWP_NOACTIVATE);
}

BEGIN_MESSAGE_MAP(CPickerCtrl, CStatic)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBOBOX, &CPickerCtrl::OnPickerControlSelchange)
	ON_CBN_EDITCHANGE(IDC_COMBOBOX, &CPickerCtrl::OnPickerControlEditchange)
	ON_COMMAND_RANGE(IDC_ADD_BUTTON, IDC_MOD_BUTTON, &CPickerCtrl::OnButtonClicked)
END_MESSAGE_MAP()


// CPickerCtrl message handlers


int CPickerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	int buttons = m_nButtons;
	for (int i = 4; i >= 0; --i) {
		m_wndButtons[i].Create(TEXT(""), WS_CHILD | WS_VISIBLE, CRect(CPoint(200 + 31 * i, 1), CSize(26, 23)), this, IDC_ADD_BUTTON + i);
		if ((buttons & 0X01) == 0)
			m_wndButtons[i].ShowWindow(SW_HIDE);
		buttons = buttons >> 1;
	}
		


	//m_wndCombobox.CreateEx(WS_EX_CLIENTEDGE, TEXT("COMBOBOX"), NULL, WS_CHILD | WS_VISIBLE | (m_bIsDropList? CBS_DROPDOWNLIST : CBS_DROPDOWN), CRect(CPoint(1, 1), CSize(98, 22)), this, IDC_COMBOBOX);
	m_wndCombobox.Create(WS_CHILD | WS_VISIBLE | (m_bIsDropList ? CBS_DROPDOWNLIST : CBS_DROPDOWN), CRect(CPoint(1, 1), CSize(98, 22)), this, IDC_COMBOBOX);
	
	m_wndButtons[0].SetImage(IDB_ADD_BUTTON);
	m_wndButtons[1].SetImage(IDB_SUB_BUTTON);
	m_wndButtons[2].SetImage(IDB_MOD_BUTTON);
	m_wndButtons[3].SetImage(IDB_UP_BUTTON);
	m_wndButtons[4].SetImage(IDB_DOWN_BUTTON);

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
	LOGFONT lfDlgFont = ncm.lfMessageFont;

	m_font.CreateFontIndirect(&lfDlgFont);

	m_wndCombobox.SetFont(&m_font);

	m_bIsCreated = TRUE;

	AdjustLayout();

	return 0;
}


void CPickerCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	AdjustLayout();
}

//void CPickerCtrl::OnOKButton()
//{
//
//	m_wndOKButton.SetFocus();
//
//	UpdateData();
//	CString text;
//	m_wndCombobox.GetWindowTextW(text);
//
//	ang::String str = text;
//	ang::Signed32 idx = m_wndCombobox.FindStringExact(0, text);
//
//	if (idx < 0)
//	{
//		ang::VarArgs args = { &str };
//		addButtonClickedEvent(this, &args);
//	}
//	else
//	{
//		ang::VarArgs args = { &idx, &str };
//		m_wndCombobox.SetCurSel(idx);
//		pickerControlSelChangeEvent(this, &args);
//	}
//}

void CPickerCtrl::OnPickerControlSelchange()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString text = L"";
	ang::wstring str;
	ang::uinterger_t idx = 0_ui;

	ang::var_args_t args = { idx, str };
	idx = m_wndCombobox.GetCurSel();
	if (idx >= 0)
	{
		
		m_wndCombobox.GetLBText(*idx, text);
		str = ang::cwstr_t(text.GetBuffer(), text.GetLength());
	}
		
	pickerControlSelChangeEvent(this, args);
	//m_wndOKButton.SetFocus();
}

void CPickerCtrl::OnPickerControlEditchange()
{
	return;
	// TODO: Add your control notification handler code here
	UpdateData();
	CString text;
	ang::wstring str;
	ang::var_args_t args = { str };
	m_wndCombobox.GetWindowTextW(text);
	str = ang::cwstr_t(text.GetBuffer(), text.GetLength());
	pickerControlTextChangeEvent(this, args);
}

void CPickerCtrl::OnButtonClicked(UINT id)
{
	CString text;
	ang::interger_t idx = int_t(id - IDC_ADD_BUTTON);
	ang::var_args_t args = { idx };
	buttonClickedEvent(this, args);
}

HBRUSH CPickerCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	struct AFX_CTLCOLOR
	{
		HWND hWnd;
		HDC hDC;
		UINT nCtlType;
	}ctrlColor;

	ctrlColor.hDC = pDC->GetSafeHdc();
	ctrlColor.hWnd = pWnd->GetSafeHwnd();
	ctrlColor.nCtlType = nCtlColor;
	return (HBRUSH)GetParent()->SendMessage(WM_CTLCOLOR, (WPARAM)pDC->GetSafeHdc(), (LPARAM)&ctrlColor);
}
