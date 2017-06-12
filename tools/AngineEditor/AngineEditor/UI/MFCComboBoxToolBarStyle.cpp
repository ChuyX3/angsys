// MFCComboBoxToolBarStyle.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/MFCComboBoxToolBarStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCComboBoxToolBarStyle

IMPLEMENT_DYNAMIC(CMFCComboBoxToolBarStyle, CComboBox)

CMFCComboBoxToolBarStyle::CMFCComboBoxToolBarStyle()
{

}

CMFCComboBoxToolBarStyle::~CMFCComboBoxToolBarStyle()
{
}


BEGIN_MESSAGE_MAP(CMFCComboBoxToolBarStyle, CComboBox)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()



// CMFCComboBoxToolBarStyle message handlers




void CMFCComboBoxToolBarStyle::OnNcPaint()
{
	CDC* pDC;
	

	CRect rw;
	GetWindowRect(rw);
	ScreenToClient(rw);
	pDC = GetDCEx(NULL, DCX_WINDOW | DCX_INTERSECTRGN);


	CMFCToolBarComboBoxButton button;
	CMFCVisualManager::GetInstance()->OnDrawComboBorder(pDC, rw, FALSE, FALSE, FALSE, &button);

}
