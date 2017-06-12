
// AngineEditor.h : main header file for the AngineEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


class CAngineEditorApp :
	public CWinAppEx,
	public ang::singleton<CAngineEditorApp>
{
public:
	friend ang::singleton<CAngineEditorApp>;

	CAngineEditorApp();

private:
	HMODULE m_hSciLexerModule;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void AddDocTemplate(CDocTemplate* pTemplate);

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	BOOL OpenDocumentFileEx(LPCTSTR path, LPCTSTR viewType);

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileNewproject();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenproject();
	DECLARE_MESSAGE_MAP()
};

extern CAngineEditorApp theApp;


class VisualStyleHelper
{
public:
	static COLORREF s_clrBackground;
	static COLORREF s_clrCtrlFace;
	static COLORREF s_clrBorderLight;
	static COLORREF s_clrBorderDark;

	static void DetermineCurrentThemeClr()
	{
		CMFCVisualManager* pManager = CMFCVisualManager::GetInstance();

		if (pManager == NULL)
		{
			s_clrBackground = (::GetSysColor(COLOR_3DFACE));
			s_clrCtrlFace = (::GetSysColor(COLOR_BTNFACE));
			s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
			s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
			return;
		}

		CMFCVisualManagerOffice2007* pManager2007 = DYNAMIC_DOWNCAST(CMFCVisualManagerOffice2007, pManager);
		if (pManager2007 != NULL)
		{
			switch (pManager2007->GetStyle())
			{
			default:
			case CMFCVisualManagerOffice2007::Office2007_LunaBlue:
				s_clrBackground = RGB(242, 247, 253);
				s_clrCtrlFace = RGB(193, 215, 245);
				s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
				s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
				break;

			case CMFCVisualManagerOffice2007::Office2007_ObsidianBlack:
				s_clrBackground = RGB(241, 242, 243); 
				s_clrCtrlFace = RGB(225, 226, 229);
				s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
				s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
				break;

			case CMFCVisualManagerOffice2007::Office2007_Silver:
				s_clrBackground = RGB(240, 241, 244); 
				s_clrCtrlFace = RGB(208, 212, 221);
				s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
				s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
				break;

			case CMFCVisualManagerOffice2007::Office2007_Aqua:
				s_clrBackground = RGB(238, 240, 247); 
				s_clrCtrlFace = RGB(212, 218, 233);
				s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
				s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
				break;
			}
		}
		else if (pManager->IsKindOf(RUNTIME_CLASS(CMFCVisualManagerVS2008)))
		{
			s_clrBackground = (::GetSysColor(COLOR_BTNFACE));
			s_clrCtrlFace = (::GetSysColor(COLOR_BTNFACE));
			s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
			s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
		}
		else if (pManager->IsKindOf(RUNTIME_CLASS(CMFCVisualManagerVS2005)))
		{
			s_clrBackground = (::GetSysColor(COLOR_BTNFACE));
			s_clrCtrlFace = (::GetSysColor(COLOR_BTNFACE));
			s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
			s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
		}
		else if (pManager->IsKindOf(RUNTIME_CLASS(CMFCVisualManagerOffice2003)))
		{
			s_clrBackground = RGB(242, 247, 253);
			s_clrCtrlFace = RGB(193, 215, 245);
			s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
			s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
		}
		//else if (pManager->IsKindOf(RUNTIME_CLASS(CMFCVisualManagerOfficeXP)))
		//{
		//	s_clrBackground = (::GetSysColor(COLOR_BTNFACE));
		//}
		//else if (pManager->IsKindOf(RUNTIME_CLASS(CMFCVisualManagerWindows)))
		//{
		//	s_clrBackground = (::GetSysColor(COLOR_BTNFACE));
		//}
		else
		{
			s_clrBackground = (::GetSysColor(COLOR_3DFACE));
			s_clrCtrlFace = (::GetSysColor(COLOR_BTNFACE));
			s_clrBorderLight = (::GetSysColor(COLOR_3DLIGHT));
			s_clrBorderDark = (::GetSysColor(COLOR_3DDKSHADOW));
		}
	}
};