#pragma once
#include "afxpropertygridctrl.h"


class CMFCPropertyGridComboButtonProperty :
	public CMFCPropertyGridProperty

{

public:
	CMFCPropertyGridComboButtonProperty(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);

	CMFCPropertyGridComboButtonProperty(const CString& strName, const COleVariant& varValue);

protected:
	virtual BOOL HasButton() const { return TRUE; }
	virtual void AdjustButtonRect();
	virtual void OnClickButton(CPoint point);
	virtual void OnDrawButton(CDC* pDC, CRect rectButton);

protected:
	CMFCVisualManager::AFX_BUTTON_STATE addButtonState;
	CMFCVisualManager::AFX_BUTTON_STATE subButtonState;
	CImageList m_images;
};
