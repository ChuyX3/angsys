#pragma once



// CNewProjectDlg dialog

class CNewProjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewProjectDlg)

public:
	CNewProjectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewProjectDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWPROJECTDLG };
#endif

protected:
	CString m_strProjectName;
	CString m_strProjectFolder;
	CListCtrl m_wndProjectList;

public:
	const CString& GetProjectName()const;
	const CString& GetProjectFolder()const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBrowseFolder();
};

