// UI/MFCButtonToolBarStyle.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/MFCButtonToolBarStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCButtonToolBarStyle

IMPLEMENT_DYNAMIC(CMFCButtonToolBarStyle, CMFCButton)

CMFCButtonToolBarStyle::CMFCButtonToolBarStyle()
{

}

CMFCButtonToolBarStyle::~CMFCButtonToolBarStyle()
{
}

void CMFCButtonToolBarStyle::OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState)
{
	//CMFCToolBarButton button;
	CMFCToolBarComboBoxButton button;


	CMFCVisualManager::AFX_BUTTON_STATE state;
	if (m_bPushed)
	{
		state = CMFCVisualManager::AFX_BUTTON_STATE::ButtonsIsHighlighted;
		button.m_nStyle = TBBS_PRESSED;

		

		if (StrCmpA(CMFCVisualManager::GetInstance()->GetRuntimeClass()->m_lpszClassName
			, RUNTIME_CLASS(CMFCVisualManager)->m_lpszClassName) == 0)
		{
			state = CMFCVisualManager::AFX_BUTTON_STATE::ButtonsIsPressed;
		}
	}
	else if (m_bHover)
	{
		state = CMFCVisualManager::AFX_BUTTON_STATE::ButtonsIsHighlighted;
		
	}
	else
	{
		state = CMFCVisualManager::AFX_BUTTON_STATE::ButtonsIsRegular;
	
	}
	pDC->FillSolidRect(rectClient, VisualStyleHelper::s_clrCtrlFace);
	CMFCVisualManager::GetInstance()->OnFillButtonInterior(pDC, &button, rectClient, state);
	CMFCVisualManager::GetInstance()->OnDrawButtonBorder(pDC, &button, rectClient, state);
}


BEGIN_MESSAGE_MAP(CMFCButtonToolBarStyle, CMFCButton)
END_MESSAGE_MAP()

// CMFCButtonToolBarStyle message handlers


