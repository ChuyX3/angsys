// UI\NewProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "NewProjectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewProjectDlg dialog

IMPLEMENT_DYNAMIC(CNewProjectDlg, CDialogEx)

CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWPROJECTDLG, pParent)
	, m_strProjectName(_T("new_project_1"))
	, m_strProjectFolder(_T(""))
{
	TCHAR userPath[MAX_PATH];
	if (SHGetSpecialFolderPath(m_hWnd, userPath, CSIDL_MYDOCUMENTS, FALSE))
	{
		m_strProjectFolder = userPath;
	//	m_strProjectFolder += L"\\AngineEditor\\";
	}
}

CNewProjectDlg::~CNewProjectDlg()
{
}

void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NP_EDITNAME, m_strProjectName);
	DDX_Text(pDX, IDC_NP_EDITPATH, m_strProjectFolder);
	DDX_Control(pDX, IDC_NP_LISTCTRL, m_wndProjectList);

}


BOOL CNewProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

const CString& CNewProjectDlg::GetProjectName()const { return m_strProjectName; }
const CString& CNewProjectDlg::GetProjectFolder()const { return m_strProjectFolder; }

BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PN_BTNBROWSE, &CNewProjectDlg::OnBnClickedBrowseFolder)
END_MESSAGE_MAP()


// CNewProjectDlg message handlers


void CNewProjectDlg::OnBnClickedOk()
{
	UpdateData();
	CDialogEx::OnOK();
}

void CNewProjectDlg::OnBnClickedBrowseFolder()
{
	CFolderPickerDialog fileDlg;
	UpdateData();
	CString title;
	GetWindowText(title);
	fileDlg.m_ofn.lpstrTitle = title;
	fileDlg.m_ofn.lpstrInitialDir = m_strProjectFolder;
	if (fileDlg.DoModal() == IDOK)
	{
		m_strProjectFolder = fileDlg.m_ofn.lpstrFile;
		m_strProjectFolder += L"\\";
		UpdateData(FALSE);
	}
}
