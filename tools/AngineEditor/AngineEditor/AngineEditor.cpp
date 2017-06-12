
// AngineEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "AngineEditor.h"
#include "UI/MainFrm.h"

#include "UI/AEProjectPropFrm.h"
#include "UI/AEProjectPropView.h"
#include "UI/AEProjectDoc.h"

#include "UI/ChildFrm.h"
#include "UI/DockableChildFrm.h"
#include "UI/CodeDoc.h"
#include "UI/CodeView.h"

#include "UI/XmlFileDoc.h"
#include "UI/EffectView.h"

#include "UI/C3DView.h"

#include "ProjectDocTemplate.h"
#include "AngineEditorDocManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAngineEditorApp

BEGIN_MESSAGE_MAP(CAngineEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CAngineEditorApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CAngineEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CAngineEditorApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CAngineEditorApp::OnFileNew)
	ON_COMMAND(ID_FILE_NEWPROJECT, &CAngineEditorApp::OnFileNewproject)
	ON_COMMAND(ID_FILE_OPEN, &CAngineEditorApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPENPROJECT, &CAngineEditorApp::OnFileOpenproject)
END_MESSAGE_MAP()


// CAngineEditorApp construction

CAngineEditorApp::CAngineEditorApp()
	: m_hSciLexerModule(NULL)
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("AngineEditor.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CAngineEditorApp object

CAngineEditorApp theApp;

COLORREF VisualStyleHelper::s_clrBackground = 0;
COLORREF VisualStyleHelper::s_clrCtrlFace = 0;
COLORREF VisualStyleHelper::s_clrBorderLight = 0;
COLORREF VisualStyleHelper::s_clrBorderDark = 0;
// CAngineEditorApp initialization

HMODULE g_hSciLexerLib = NULL;

BOOL CAngineEditorApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	m_hSciLexerModule = LoadLibrary(TEXT("SciLexer.dll"));
	if (m_hSciLexerModule == NULL)
		return false;

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CProjectDocTemplate* pSolDocTemplate;
	pSolDocTemplate = new CProjectDocTemplate(IDR_AEPROJECT_TYPE,
		RUNTIME_CLASS(CAEProjectDoc),
		RUNTIME_CLASS(CAEProjectPropFrame), // custom MDI child frame
		RUNTIME_CLASS(C3DView));
	if (!pSolDocTemplate)
		return FALSE;
	pSolDocTemplate->SetContainerInfo(IDR_AEPROJECT_TYPE_CNTR_IP);
	AddDocTemplate(pSolDocTemplate);

	CMultiDocTemplate* pDocTemplate = new CMultiDocTemplate(IDR_HLSLFILE_TYPE,
		RUNTIME_CLASS(CCodeDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCodeView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetContainerInfo(IDR_AEPROJECT_TYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_GLSLFILE_TYPE,
		RUNTIME_CLASS(CCodeDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCodeView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetContainerInfo(IDR_AEPROJECT_TYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_FXFILE_TYPE,
		RUNTIME_CLASS(CCodeDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCodeView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetContainerInfo(IDR_AEPROJECT_TYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	pDocTemplate = new CMultiDocTemplate(IDR_EFFECTFILE_TYPE,
		RUNTIME_CLASS(CXmlFileDoc),
		RUNTIME_CLASS(CDockableChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CEffectView));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetContainerInfo(IDR_AEPROJECT_TYPE_CNTR_IP);
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (cmdInfo.m_nShellCommand != CCommandLineInfo::FileNew)if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	VisualStyleHelper::DetermineCurrentThemeClr();

	return TRUE;
}

int CAngineEditorApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);
	
	if (m_hSciLexerModule != NULL)
		FreeLibrary(m_hSciLexerModule);
	m_hSciLexerModule = NULL;
	return CWinAppEx::ExitInstance();
}

// CAngineEditorApp message handlers

void CAngineEditorApp::AddDocTemplate(CDocTemplate* pTemplate)
{
	if (m_pDocManager == NULL)
		m_pDocManager = new CAngineEditorDocManager;
	m_pDocManager->AddDocTemplate(pTemplate);
}

BOOL CAngineEditorApp::OpenDocumentFileEx(LPCTSTR path, LPCTSTR viewType)
{
	if (m_pDocManager == NULL)
		return FALSE;
	((CAngineEditorDocManager*)m_pDocManager)->OpenDocumentFileEx(path, viewType);

	return FALSE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CAngineEditorApp::OnAppAbout()
{

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
	
}

// CAngineEditorApp customization load/save methods

void CAngineEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CAngineEditorApp::LoadCustomState()
{
}

void CAngineEditorApp::SaveCustomState()
{
}

// CAngineEditorApp message handlers


void CAngineEditorApp::OnFileNew()
{
	if (m_pDocManager != NULL)
		((CAngineEditorDocManager*)m_pDocManager)->OnFileNew();
}


void CAngineEditorApp::OnFileNewproject()
{
	if (m_pDocManager != NULL)
		((CAngineEditorDocManager*)m_pDocManager)->OnProjectNew();
}


void CAngineEditorApp::OnFileOpen()
{
	if (m_pDocManager != NULL)
		((CAngineEditorDocManager*)m_pDocManager)->OnFileOpen();
}


void CAngineEditorApp::OnFileOpenproject()
{
	if (m_pDocManager != NULL)
		((CAngineEditorDocManager*)m_pDocManager)->OnProjectOpen();
		
}
