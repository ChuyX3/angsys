#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxpropertygridctrl.h"
#include "afxbutton.h"
#include "UI/MFCButtonToolBarStyle.h"

class CShaderPropertiesCtrl;

// CShaderPropertiesCtrl dialog
class CShaderPropertyGridCtrl : public CMFCPropertyGridCtrl
{
private:
	CShaderPropertiesCtrl* m_pParentWnd;
public:
	CShaderPropertyGridCtrl(CShaderPropertiesCtrl* parent);

public:
	virtual void OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel);
};


class CShaderPropertiesCtrl : public CStatic
{
	DECLARE_DYNAMIC(CShaderPropertiesCtrl)

	friend CShaderPropertyGridCtrl;

public:
	CShaderPropertiesCtrl();   // standard constructor
	virtual ~CShaderPropertiesCtrl();
	
protected:
	BOOL m_bIsCreated;
	CFont m_font;
	CEdit m_wndEditCtrl;
	CMFCButtonToolBarStyle m_wndButtons[5];
	CTreeCtrl m_wndTreeCtrl;
	CShaderPropertyGridCtrl m_wndProperties;


	CMFCPropertyGridProperty* m_pGlobalGroup;
	CMFCPropertyGridProperty* m_pCompileConfigGroup;
	CMFCPropertyGridProperty* m_pConstBufferGroup;
	CMFCPropertyGridProperty* m_pVertexInputGroup;
	CMFCPropertyGridProperty* m_pSamplerInputGroup;

	void AdjustLayout();
	void OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lParam);
};
