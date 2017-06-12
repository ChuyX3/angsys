// UI/ShaderInputs.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/ShaderPropertiesCtrl.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CShaderPropertyGridCtrl::CShaderPropertyGridCtrl(CShaderPropertiesCtrl* parent)
	: m_pParentWnd(parent)
{

}

void CShaderPropertyGridCtrl::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel)
{
	m_pParentWnd->OnChangeSelection(pNewSel, pOldSel);
}

// CShaderPropertiesCtrl dialog

IMPLEMENT_DYNAMIC(CShaderPropertiesCtrl, CDialogEx)

CShaderPropertiesCtrl::CShaderPropertiesCtrl()
	: m_wndProperties(this)
{
	m_bIsCreated = FALSE;
}

CShaderPropertiesCtrl::~CShaderPropertiesCtrl()
{
}

#define  IDC_EDIT			0X002001
#define  IDC_TREE			0X002002
#define  IDC_PROPERTY		0X002003
#define  IDC_ADDBUTTON		0X002004
#define  IDC_SUBBUTTON		0X002005
#define  IDC_MODBUTTON		0X002006
#define  IDC_UPBUTTON		0X002007
#define  IDC_DOWNBUTTON		0X002008


BEGIN_MESSAGE_MAP(CShaderPropertiesCtrl, CStatic)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CShaderPropertiesCtrl::OnPropertyChanged)
END_MESSAGE_MAP()


// CShaderPropertiesCtrl message handlers

void CShaderPropertiesCtrl::AdjustLayout()
{
	if (!m_bIsCreated)
		return;
	CRect rc;
	GetClientRect(rc);

	int x = rc.Width();
	for (auto i = 4; i >= 0; --i)
	{
		x -= 26;
		m_wndButtons[i].SetWindowPos(NULL, x, 1, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
	}
	m_wndEditCtrl.SetWindowPos(NULL, 0,0, x - 2, 23, SWP_NOACTIVATE | SWP_NOMOVE);
	m_wndProperties.SetWindowPos(NULL, 0, 0, rc.Width() - 2, rc.Height() - 26, SWP_NOACTIVATE | SWP_NOMOVE);
}

int CShaderPropertiesCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	for (int i = 4; i >= 0; --i) 
		m_wndButtons[i].Create(TEXT(""), WS_CHILD | WS_VISIBLE, CRect(CPoint(200 + 31 * i, 1), CSize(26, 23)), this, IDC_ADDBUTTON + i);
	

	//m_wndCombobox.CreateEx(WS_EX_CLIENTEDGE, TEXT("COMBOBOX"), NULL, WS_CHILD | WS_VISIBLE | (m_bIsDropList? CBS_DROPDOWNLIST : CBS_DROPDOWN), CRect(CPoint(1, 1), CSize(98, 22)), this, IDC_COMBOBOX);
	m_wndEditCtrl.CreateEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE , CRect(CPoint(1, 1), CSize(98, 22)), this, IDC_EDIT);
	m_wndProperties.Create(WS_BORDER | WS_CHILD | WS_VISIBLE, CRect(CPoint(1, 24), CSize(98, 22)), this, IDC_PROPERTY);
	

	m_wndProperties.EnableHeaderCtrl(FALSE);
	m_wndProperties.SetVSDotNetLook(TRUE);
	m_wndProperties.EnableDescriptionArea(FALSE);

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

	m_wndEditCtrl.SetFont(&m_font);
	//m_wndTreeCtrl.SetFont(&m_font);
	m_wndProperties.SetFont(&m_font);

	m_bIsCreated = TRUE;

	AdjustLayout();


	m_pGlobalGroup = new CMFCPropertyGridProperty(_T("Properties"));
	m_wndProperties.AddProperty(m_pGlobalGroup);

	CMFCPropertyGridProperty* prop = new CMFCPropertyGridProperty(_T("File ID"), _T("IDS_SOLID_OBJECT_FX"), _T(""));
	prop->AddOption(_T("IDS_SOLID_OBJECT_FX"));
	prop->AddOption(_T("IDS_CHARACTER_FX"));
	prop->AllowEdit(FALSE);
	m_pGlobalGroup->AddSubItem(prop);

	prop = new CMFCPropertyGridProperty(_T("Entry Point"), _T("main"), _T(""));
	prop->AllowEdit(TRUE);
	m_pGlobalGroup->AddSubItem(prop);

	m_pCompileConfigGroup = new CMFCPropertyGridProperty(_T("Compile Config"));
	m_pConstBufferGroup = new CMFCPropertyGridProperty(_T("Constan Buffers"));
	m_pVertexInputGroup = new CMFCPropertyGridProperty(_T("Vertex Inputs"));
	m_pSamplerInputGroup = new CMFCPropertyGridProperty(_T("Samplers"));

	m_pGlobalGroup->AddSubItem(m_pCompileConfigGroup);
	m_pGlobalGroup->AddSubItem(m_pConstBufferGroup);
	m_pGlobalGroup->AddSubItem(m_pVertexInputGroup);
	m_pGlobalGroup->AddSubItem(m_pSamplerInputGroup);

	prop = new CMFCPropertyGridProperty(_T("VSHADER"), _T(""), _T(""));
	prop->AllowEdit(TRUE);
	m_pCompileConfigGroup->AddSubItem(prop);


	prop = new CMFCPropertyGridProperty(_T("Var"), _T(""), _T(""));
	prop->AllowEdit(TRUE);
	m_pConstBufferGroup->AddSubItem(prop);

//	m_pGlobalGroup->RemoveSubItem(, FALSE);

	return 0;
}


void CShaderPropertiesCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	AdjustLayout();
}

LRESULT CShaderPropertiesCtrl::OnPropertyChanged(WPARAM wparam, LPARAM lParam)
{
	return 0;
}

void CShaderPropertiesCtrl::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel)
{
	if (pNewSel)
	{
		if(pNewSel == m_pGlobalGroup)
		{
			m_wndButtons[0].EnableWindow(FALSE);
			m_wndButtons[1].EnableWindow(FALSE);
			m_wndButtons[2].EnableWindow(FALSE);
			m_wndButtons[3].EnableWindow(FALSE);
			m_wndButtons[4].EnableWindow(FALSE);
			m_wndEditCtrl.EnableWindow(FALSE);
		}
		else if (pNewSel == m_pCompileConfigGroup)
		{
			m_wndButtons[0].EnableWindow(TRUE);
			m_wndButtons[1].EnableWindow(FALSE);
			m_wndButtons[2].EnableWindow(FALSE);
			m_wndButtons[3].EnableWindow(FALSE);
			m_wndButtons[4].EnableWindow(FALSE);
			m_wndEditCtrl.EnableWindow(TRUE);

		}
		else if (pNewSel == m_pConstBufferGroup)
		{

		}
		else if (pNewSel == m_pVertexInputGroup)
		{

		}
		else if (pNewSel == m_pSamplerInputGroup)
		{

		}
		else
		{
			if (m_pCompileConfigGroup && m_pCompileConfigGroup->GetSubItemsCount() > 0)
			{
				for (auto i = 0; i < m_pCompileConfigGroup->GetSubItemsCount(); ++i)
				{
					if (pNewSel == m_pCompileConfigGroup->GetSubItem(i))
					{
						m_wndButtons[0].EnableWindow(FALSE);
						m_wndButtons[1].EnableWindow(TRUE);
						m_wndButtons[2].EnableWindow(TRUE);
						m_wndButtons[3].EnableWindow(TRUE);
						m_wndButtons[4].EnableWindow(TRUE);
						m_wndEditCtrl.EnableWindow(TRUE);
					}
				}
			}
		}

	}
	else
	{		
		m_wndButtons[0].EnableWindow(FALSE);
		m_wndButtons[1].EnableWindow(FALSE);
		m_wndButtons[2].EnableWindow(FALSE);
		m_wndButtons[3].EnableWindow(FALSE);
		m_wndButtons[4].EnableWindow(FALSE);
		m_wndEditCtrl.EnableWindow(FALSE);
	}
	
}