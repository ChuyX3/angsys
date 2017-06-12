#pragma once


// CMFCButtonToolBarStyle

class CMFCButtonToolBarStyle : public CMFCButton
{
	DECLARE_DYNAMIC(CMFCButtonToolBarStyle)

public:
	CMFCButtonToolBarStyle();
	virtual ~CMFCButtonToolBarStyle();

protected:
	virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);

protected:
	DECLARE_MESSAGE_MAP()
};


