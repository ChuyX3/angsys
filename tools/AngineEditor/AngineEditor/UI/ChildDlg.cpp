// UI/ChildDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/ChildDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildDlg dialog

IMPLEMENT_DYNAMIC(CChildDlg, CDialogEx)

CChildDlg::CChildDlg(CWnd* pParent /*=NULL*/)
{

}

CChildDlg::~CChildDlg()
{
}

void CChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CChildDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
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

BEGIN_MESSAGE_MAP(CChildDlg, CDialogEx)
END_MESSAGE_MAP()


// CChildDlg message handlers
