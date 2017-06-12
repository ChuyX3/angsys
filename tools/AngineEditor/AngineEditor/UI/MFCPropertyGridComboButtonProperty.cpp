#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/MFCPropertyGridComboButtonProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//Constructor

CMFCPropertyGridComboButtonProperty::CMFCPropertyGridComboButtonProperty(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	: CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_ADD_SUB_BUTTON);

	m_images.Create(16, 15, ILC_MASK | ILC_COLOR8, 0, 0);
	m_images.Add(&bmp, 0XC0C0C0);

	addButtonState = CMFCVisualManager::AFX_BUTTON_STATE::ButtonsIsRegular;
	subButtonState = CMFCVisualManager::AFX_BUTTON_STATE::ButtonsIsRegular;
}


CMFCPropertyGridComboButtonProperty::CMFCPropertyGridComboButtonProperty(const CString& strName,
	const COleVariant& varValue) : CMFCPropertyGridProperty(strName, varValue)
{
	// Load image and create a list from it
	CBitmap bmp;
	bmp.LoadBitmap(IDB_ADD_SUB_BUTTON);

	m_images.Create(16, 15, ILC_MASK | ILC_COLOR8, 0, 0);
	m_images.Add(&bmp, 0XC0C0C0);

	addButtonState = CMFCVisualManager::AFX_BUTTON_STATE::ButtonsIsRegular;
	subButtonState = CMFCVisualManager::AFX_BUTTON_STATE::ButtonsIsRegular;
}



// Change the size of the button
void CMFCPropertyGridComboButtonProperty::AdjustButtonRect()
{
	CMFCPropertyGridProperty::AdjustButtonRect();
	m_rectButton.left -= m_rectButton.Width();
}



//Action to take upon button click

void CMFCPropertyGridComboButtonProperty::OnClickButton(CPoint point)
{
	//BOOL bIsLeft = point.x < m_rectButton.CenterPoint().x;
	//AfxMessageBox(bIsLeft ? _T("Left button clicked") :
	//	_T("Right button clicked"));

//	if (point.x < m_rectButton.left + m_rectButton.Width() / 4)
//	{
		CMFCPropertyGridProperty::OnClickButton(point);
//	}
//	else if(point.x < m_rectButton.left + 2.5 * m_rectButton.Width() / 4)
//	{
//
//	}
//	else
//	{
//		
//	}
	
}

// How to draw the buttons

void CMFCPropertyGridComboButtonProperty::OnDrawButton(CDC* pDC, CRect rectButton)
{
	CMFCToolBarButton button;
	CRect rect = rectButton;
	CMFCVisualManager::AFX_BUTTON_STATE state;
	
	state = addButtonState;

	CMFCVisualManager::GetInstance()->OnFillButtonInterior(pDC, &button, rect, state);

	m_images.Draw(pDC, 0, CPoint(rect.left, rect.top), ILD_NORMAL);

	CMFCVisualManager::GetInstance()->OnDrawButtonBorder(pDC, &button, rect, state);

	

	//for (int i = 0; i < 2; i++)
	//{
	//	CMFCToolBarButton button;
	//	CRect rect = rectButton;
	//	CMFCVisualManager::AFX_BUTTON_STATE state;
	//	if (i == 0)
	//	{
	//		rect.left = rectButton.left + rectButton.Width() / 4;
	//		rect.right = rectButton.left + 2.5 * rectButton.Width() / 4;
	//		state = addButtonState;
	//	}
	//	else
	//	{
	//		rect.left = rectButton.left + 2.5 * rectButton.Width() / 4;
	//		state = subButtonState;
	//	}

	//	CMFCVisualManager::GetInstance()->OnFillButtonInterior(pDC, &button, rect, state);
	//	
	//	m_images.Draw(pDC, i, CPoint(rect.left, rect.top), ILD_NORMAL);

	//	CMFCVisualManager::GetInstance()->OnDrawButtonBorder(pDC, &button, rect, state);

	//}

	rectButton.right = rectButton.left + rectButton.Width() / 4;
	//CMFCPropertyGridProperty::OnDrawButton(pDC, rectButton);
}
