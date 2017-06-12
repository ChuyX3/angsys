// UI/ProfileViewCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/ProfileViewCtrl.h"
#include "afxdialogex.h"


// CProfileViewCtrl dialog

IMPLEMENT_DYNAMIC(CProfileViewCtrl, CDialogEx)

CProfileViewCtrl::CProfileViewCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EFFECTVIEW_PROFILES, pParent)
{

}

CProfileViewCtrl::~CProfileViewCtrl()
{
}

void CProfileViewCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECTVIEW_PROFILEPICKER_COMBO, m_comboProfilePicker);
	DDX_Control(pDX, IDC_EFFECTVIEW_PROFILEADDNEW_BUTTON, m_btnAddNew);
	DDX_Control(pDX, IDC_EFFECTVIEW_PROFILEREMOVE_BUTTON, m_btnRemove);
}


BEGIN_MESSAGE_MAP(CProfileViewCtrl, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CProfileViewCtrl message handlers

BOOL CProfileViewCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode >= 0 && nCode <= 10)
	{
		GetParent()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	else switch (nCode)
	{
	case -1:
	case 0X100:
	case 0X200:
	case 0X300:
	case 0X400:
	case 0X500:
	case 0X501:
	case 0X601:
	case 0X602:
		GetParent()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		break;
	}
	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CProfileViewCtrl::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rc;
	if (m_comboProfilePicker.GetSafeHwnd() != NULL)
	{
		m_comboProfilePicker.GetClientRect(rc);
		m_comboProfilePicker.SetWindowPos(NULL, 5, 9, cx - 80, rc.Height(), SWP_NOACTIVATE);
	}

	if (m_btnAddNew.GetSafeHwnd() != NULL)
	{
		m_btnAddNew.GetClientRect(rc);
		m_btnAddNew.SetWindowPos(NULL, cx - 70, 8, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}

	if (m_btnRemove.GetSafeHwnd() != NULL)
	{
		m_btnRemove.GetClientRect(rc);
		m_btnRemove.SetWindowPos(NULL, cx - 40, 8, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}
}
