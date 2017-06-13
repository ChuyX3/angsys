#pragma once
#include "afxcmn.h"

#include "ProjectManager.h"
#include "afxpropertygridctrl.h"
#include "afxbutton.h"

// CEffectView form view
class CEffectView;

// CShaderPropertiesCtrl dialog
class CEffectViewPropertyGridCtrl : public CMFCPropertyGridCtrl
{
private:
	CEffectView* m_pParentWnd;
public:
	CEffectViewPropertyGridCtrl(CEffectView* parent);

public:
	virtual void OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel);
};

class CPropertyInfo;

class CPropertyInfo
{
public:
	CPropertyInfo();
	~CPropertyInfo();

public:
	CMFCPropertyGridProperty* Property()const;
	void Property(CMFCPropertyGridProperty*, uint type);
	uint PropertyType()const;

	ang_byte_bits_t ControlsInfo()const;
	void ControlsInfo(byte);

	ang::xml::xml_node_t Data()const;
	void Data(ang::xml::xml_node_t);

	void OnPropertyChange(CEffectView*);
	//void OnCommandUIChange(CEffectView*, uint uiIdx);

	ang::core::delegates::listener<void(CObject*, CPropertyInfo*)> changeEvent;

private:
	uint m_nPropType;
	CMFCPropertyGridProperty* m_pProperty;
	ang_byte_bits_t m_bitsControlsInfo;
	ang::xml::xml_node_t m_xmlData;
};

ANG_REGISTER_RUNTIME_TYPENAME(CPropertyInfo)

class CEffectView : 
	public CView
{
	DECLARE_DYNCREATE(CEffectView)
	friend CEffectViewPropertyGridCtrl;
protected:
	CEffectView();           // protected constructor used by dynamic creation
	virtual ~CEffectView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	enum PropertyMapKey : uint
	{
		MainGroup = 0,
		EffectProp,
		ProfileProp,
		TechniqueProp,
		PassProp,
		VertexShaderGroup,
		VSFileProp,
		VSEntryProp,
		VSConfigGroup,
		VSConfigProp,
		VSConstBufferGroup,
		VSConstBufferProp,
		VSInputGroup,
		VSInputProp,
		PixelShaderGroup,
		PSFileProp,
		PSEntryProp,
		PSConfigGroup,
		PSConfigProp,
		PSConstBufferGroup,
		PSConstBufferProp,
		PSSamplerGroup,
		PSSamplerProp,

		MaxSize
	};

	enum ProperyButtons : byte
	{
		DisableButtons = 0,
		EffectPropButtons = 0X3F,
		ProfilePropButtons = 0X36,
		TechniquePropButtons = 0X3F,
		PassPropButtons = 0X36,
		VSFilePropButtons = 0X02,
		VSEntryPropButtons = 0X09,
		VSConfigGroupButtons = 0X03,
		VSConfigPropButtons =  0X3D,
		VSConstBufferGroupButtons = 0X03,
		VSConstBufferStructPropButtons = 0X3F,
		VSConstBufferVarPropButtons = 0X3D,
		VSInputGroupButtons = 0X03,
		VSInputPropButtons = 0X3D,
		PSFilePropButtons = 0X02,
		PSEntryPropButtons = 0X09,
		PSConfigGroupButtons = 0X03,
		PSConfigPropButtons = 0X3D,
		PSConstBufferGroupButtons = 0X03,
		PSConstBufferStructPropButtons = 0X3F,
		PSConstBufferVarPropButtons = 0X3D,
		PSSamplerGroupButtons = 0X03,
		PSSamplerPropButtons = 0X3D,
	};

	ang::xml::xml_node_t m_pEffectLibrary;
	CMFCToolBar m_wndToolBar;
	CEffectViewPropertyGridCtrl m_wndProperties;
	COLORREF m_clrBackground;
	CBrush m_brBackground;
	ang_byte_bits_t m_bitsEnableControls;


	ang::collections::map<uint,CMFCPropertyGridProperty*> m_propMap;
	CMFCPropertyGridProperty* m_pSelectedProperty;

	void OnCurrentEffectChangeEvent(CObject* caller, CPropertyInfo* args);

protected:
	BOOL m_bIsInitialized;
	void AdjustLayout();

	void FillProperties();
	void SetCurrentEffect(ang::xml::xml_node_t effect, bool enable = true);
	void SetCurrentProfile(ang::xml::xml_node_t profile, bool enable = true);
	void SetCurrentTechnique(ang::xml::xml_node_t technique, bool enable = true);
	void SetCurrentPass(ang::xml::xml_node_t pass, bool enable = true);
	void SetCurrentVS(ang::xml::xml_node_t pass, bool enable = true);
	void SetCurrentPS(ang::xml::xml_node_t pass, bool enable = true);

	void OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC*) {};
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lParam);
	afx_msg void OnFileSave();
	afx_msg void OnEffectviewAddnew();	
	afx_msg void OnEffectviewRemove();
	afx_msg void OnEffectviewModify();
	afx_msg void OnEffectviewUp();	
	afx_msg void OnEffectviewDown();

	afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectviewAddnew(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectviewRemove(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectviewModify(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectviewUp(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEffectviewDown(CCmdUI *pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateEffectviewEditctrl(CCmdUI *pCmdUI);
};


