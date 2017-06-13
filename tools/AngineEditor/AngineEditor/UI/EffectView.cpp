// UI/EffectView.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/EffectView.h"
#include "UI/XmlFileDoc.h"
#include "UI/AddNewProfileDlg.h"
#include "UI/MFCPropertyGridComboButtonProperty.h"

#if defined _DEBUG
#define NEW ANG_DEBUG_NEW()
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;

CPropertyInfo::CPropertyInfo()
{
	m_pProperty = NULL;
	m_bitsControlsInfo.value = 0;
	m_nPropType = 0;
}

CPropertyInfo::~CPropertyInfo()
{
	m_pProperty = NULL;
	m_bitsControlsInfo.value = 0;
	m_nPropType = 0;
}

CMFCPropertyGridProperty* CPropertyInfo::Property()const
{
	return m_pProperty;
}

void CPropertyInfo::Property(CMFCPropertyGridProperty* prop, uint type)
{
	m_pProperty = prop;
	m_nPropType = type;
}

uint CPropertyInfo::PropertyType()const
{
	return m_nPropType;
}

ang_byte_bits_t CPropertyInfo::ControlsInfo()const
{
	ang_byte_bits_t value;
	value.value = (m_pProperty && m_pProperty->IsEnabled())? m_bitsControlsInfo.value : 0U;
	return value;
}

void CPropertyInfo::ControlsInfo(byte info)
{
	m_bitsControlsInfo.value = info;
}

ang::xml::xml_node_t CPropertyInfo::Data()const
{
	return m_xmlData.get();
}

void CPropertyInfo::Data(ang::xml::xml_node_t data)
{
	m_xmlData = data;
}

void CPropertyInfo::OnPropertyChange(CEffectView* view)
{
	changeEvent(view, this);
}

//void CPropertyInfo::OnCommandUIChange(CEffectView* view, uint uiIdx)
//{
//	if (m_pCommandUIEvent)
//		m_pCommandUIEvent->OnEvent(view, this);
//}

CEffectViewPropertyGridCtrl::CEffectViewPropertyGridCtrl(CEffectView* parent)
	: m_pParentWnd(parent)
{
	
}

void CEffectViewPropertyGridCtrl::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel)
{
	m_pParentWnd->OnChangeSelection(pNewSel, pOldSel);
}


// CEffectView

IMPLEMENT_DYNCREATE(CEffectView, CView)

CEffectView::CEffectView()
	: CView()
	, m_bIsInitialized(FALSE)
	, m_wndProperties(this)
{
	m_bitsEnableControls.value = 0;
	m_pSelectedProperty = NULL;
}

CEffectView::~CEffectView()
{
}

#define  IDC_PROPERTY		0X002001

BEGIN_MESSAGE_MAP(CEffectView, CView)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CEffectView::OnPropertyChanged)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_FILE_SAVE, &CEffectView::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CEffectView::OnUpdateFileSave)
	ON_COMMAND(ID_EFFECTVIEW_ADDNEW, &CEffectView::OnEffectviewAddnew)
	ON_UPDATE_COMMAND_UI(ID_EFFECTVIEW_ADDNEW, &CEffectView::OnUpdateEffectviewAddnew)
	ON_COMMAND(ID_EFFECTVIEW_DOWN, &CEffectView::OnEffectviewDown)
	ON_UPDATE_COMMAND_UI(ID_EFFECTVIEW_DOWN, &CEffectView::OnUpdateEffectviewDown)
	ON_COMMAND(ID_EFFECTVIEW_REMOVE, &CEffectView::OnEffectviewRemove)
	ON_UPDATE_COMMAND_UI(ID_EFFECTVIEW_REMOVE, &CEffectView::OnUpdateEffectviewRemove)
	ON_COMMAND(ID_EFFECTVIEW_UP, &CEffectView::OnEffectviewUp)
	ON_UPDATE_COMMAND_UI(ID_EFFECTVIEW_UP, &CEffectView::OnUpdateEffectviewUp)
	ON_COMMAND(ID_EFFECTVIEW_MODIFY, &CEffectView::OnEffectviewModify)
	ON_UPDATE_COMMAND_UI(ID_EFFECTVIEW_MODIFY, &CEffectView::OnUpdateEffectviewModify)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_EFFECTVIEW_EDITCTRL, &CEffectView::OnUpdateEffectviewEditctrl)
END_MESSAGE_MAP()


// CEffectView diagnostics

#ifdef _DEBUG
void CEffectView::AssertValid() const
{
	CView::AssertValid();
}

void CEffectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CEffectView message handlers

void CEffectView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || m_bIsInitialized == FALSE)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, 0, 0 , rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndProperties.SetWindowPos(NULL, rectClient.left, rectClient.top  + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

}

void CEffectView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (!GetDocument()->IsKindOf(RUNTIME_CLASS(CXmlFileDoc)))
		return;

	CXmlFileDoc* doc = (CXmlFileDoc*)GetDocument();

	m_pEffectLibrary = doc->GetXmlRootElement();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EFFECTVIEW_TOOLBAR);
	m_wndToolBar.LoadToolBar(IDR_EFFECTVIEW_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EFFECTVIEW_TOOLBAR_HC : IDR_EFFECTVIEW_TOOLBAR, 0, 0, TRUE /* Locked */);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	CMFCToolBarEditBoxButton myEdtButton(ID_EFFECTVIEW_EDITCTRL, 0, WS_TABSTOP | ES_LEFT | WS_BORDER, 150);
	m_wndToolBar.ReplaceButton(ID_EFFECTVIEW_EDITCTRL, myEdtButton);

	m_wndProperties.Create(WS_BORDER | WS_CHILD | WS_VISIBLE, CRect(CPoint(1, 24), CSize(98, 22)), this, IDC_PROPERTY);
	m_wndProperties.EnableHeaderCtrl(FALSE);
	m_wndProperties.SetVSDotNetLook(TRUE);
	m_wndProperties.EnableDescriptionArea(FALSE);


	m_bIsInitialized = TRUE;

	AdjustLayout();

	FillProperties();
}

void CEffectView::FillProperties()
{
	CMFCPropertyGridProperty *mainGroup = NEW CMFCPropertyGridProperty(TEXT("Effect Library"));
	m_wndProperties.AddProperty(mainGroup);

	CPropertyInfo *info = NEW CPropertyInfo();
	info->Property(mainGroup, MainGroup);
	info->ControlsInfo(DisableButtons);
	info->Data(m_pEffectLibrary.get());
	mainGroup->SetData((DWORD_PTR)(void*)info);
	m_propMap[MainGroup] = mainGroup;

	CMFCPropertyGridProperty* effectProp = NEW CMFCPropertyGridProperty(TEXT("Effect"), TEXT(""), TEXT(""));
	effectProp->AllowEdit(FALSE);
	//effectProp->Enable(FALSE);
	info = NEW CPropertyInfo();
	info->changeEvent += core::delegates::function<void(CObject*, CPropertyInfo*)>(this, &CEffectView::OnCurrentEffectChangeEvent);
	info->Property(effectProp, EffectProp);
	info->ControlsInfo(EffectPropButtons);
	info->Data(nullptr);
	effectProp->SetData((DWORD_PTR)(void*)info);
	m_propMap[EffectProp] = effectProp;
	mainGroup->AddSubItem(effectProp);

	CMFCPropertyGridProperty* profileProp = NEW CMFCPropertyGridProperty(TEXT("Profile"), TEXT(""), TEXT(""));
	profileProp->AllowEdit(FALSE);
	profileProp->Enable(FALSE);
	info = NEW CPropertyInfo();
	info->Property(profileProp, ProfileProp);
	info->ControlsInfo(ProfilePropButtons);
	info->Data(nullptr);
	profileProp->SetData((DWORD_PTR)(void*)info);
	m_propMap[ProfileProp] = profileProp;
	mainGroup->AddSubItem(profileProp);

	CMFCPropertyGridProperty* techniqueProp = NEW CMFCPropertyGridProperty(TEXT("Technique"), TEXT(""), TEXT(""));
	techniqueProp->AllowEdit(FALSE);
	techniqueProp->Enable(FALSE);
	info = NEW CPropertyInfo();
	info->Property(techniqueProp, TechniqueProp);
	info->ControlsInfo(TechniquePropButtons);
	info->Data(nullptr);
	techniqueProp->SetData((DWORD_PTR)(void*)info);
	m_propMap[TechniqueProp] = techniqueProp;
	mainGroup->AddSubItem(techniqueProp);

	CMFCPropertyGridProperty* passProp = NEW CMFCPropertyGridProperty(TEXT("Pass"), TEXT(""), TEXT(""));
	passProp->AllowEdit(FALSE);
	passProp->Enable(FALSE);
	info = NEW CPropertyInfo();
	info->Property(passProp, PassProp);
	info->ControlsInfo(PassPropButtons);
	info->Data(nullptr);
	passProp->SetData((DWORD_PTR)(void*)info);
	m_propMap[PassProp] = passProp;
	mainGroup->AddSubItem(passProp);


	CMFCPropertyGridProperty* vsProp = NEW CMFCPropertyGridProperty(TEXT("Vertex Shader"));
	vsProp->AllowEdit(FALSE);
	vsProp->Enable(FALSE);
	info = NEW CPropertyInfo();
	info->Property(vsProp, VertexShaderGroup);
	info->ControlsInfo(DisableButtons);
	info->Data(nullptr);
	vsProp->SetData((DWORD_PTR)(void*)info);
	m_propMap[VertexShaderGroup] = vsProp;
	mainGroup->AddSubItem(vsProp);
	{
		CMFCPropertyGridProperty* entryProp = NEW CMFCPropertyGridProperty(TEXT("Entry Point"), TEXT(""), TEXT(""));
		entryProp->AllowEdit(TRUE);
		entryProp->Enable(FALSE);
		info = NEW CPropertyInfo();
		info->Property(entryProp, VSEntryProp);
		info->ControlsInfo(VSEntryPropButtons);
		info->Data(nullptr);
		entryProp->SetData((DWORD_PTR)(void*)info);
		m_propMap[VSEntryProp] = entryProp;
		vsProp->AddSubItem(entryProp);
	}

	CMFCPropertyGridProperty* psProp = NEW CMFCPropertyGridProperty(TEXT("Pixel Shader"));
	psProp->AllowEdit(FALSE);
	psProp->Enable(FALSE);
	info = NEW CPropertyInfo();
	info->Property(psProp, PixelShaderGroup);
	info->ControlsInfo(DisableButtons);
	info->Data(nullptr);
	psProp->SetData((DWORD_PTR)(void*)info);
	m_propMap[PixelShaderGroup] = psProp;
	mainGroup->AddSubItem(psProp);
	{
		CMFCPropertyGridProperty* entryProp = NEW CMFCPropertyGridProperty(TEXT("Entry Point"));
		entryProp->AllowEdit(TRUE);
		entryProp->Enable(FALSE);
		info = NEW CPropertyInfo();
		info->Property(entryProp, PSEntryProp);
		info->ControlsInfo(PSEntryPropButtons);
		info->Data(nullptr);
		entryProp->SetData((DWORD_PTR)(void*)info);
		m_propMap[PSEntryProp] = entryProp;
		psProp->AddSubItem(entryProp);
	}

	if (m_pEffectLibrary.is_empty() || !m_pEffectLibrary->xml_has_children())
	{
		SetCurrentEffect(nullptr, true);
		return;
	}

	ang::xml::xml_node_t current;
	for (auto effect = m_pEffectLibrary->xml_children()->begin(); effect.is_valid(); ++effect)
	{
		if (effect->xml_name().as<cwstr_t>() == "effect"_s)
		{
			auto value = effect->xml_attributes()["name"].as<cwstr_t>();
			effectProp->AddOption(value);
			if (current.get() == nullptr)
			{
				current = (ang::xml::xml_node*)effect;
				effectProp->SetValue(value.cstr());
			}
		}
	}

	//SetCurrentEffect((ang::xml::xml_node_t)current);
}


void CEffectView::SetCurrentEffect(ang::xml::xml_node_t node, bool enable)
{
	/*CMFCPropertyGridProperty* effectProp = m_propMap[EffectProp];
	CMFCPropertyGridProperty* profileProp = m_propMap[ProfileProp];

	CPropertyInfo* info = (CPropertyInfo*)effectProp->GetData();
	if (info->Data() == node)
		return;

	info->Data(node);

	profileProp->RemoveAllOptions();
	profileProp->ResetOriginalValue();

	if (node)
	{
		effectProp->Enable(TRUE);
		if (!node->xml_has_children())
		{
			SetCurrentProfile(nullptr, true);
			return;
		}
	}
	else
	{
		effectProp->Enable(enable ? TRUE : FALSE);
		SetCurrentProfile(nullptr, false);
		return;
	}
	
	ang::xml::xml_node_t current =  nullptr;

	for (auto profile = node->xml_children()->begin(); profile.is_valid(); ++profile)
	{
		if (profile->xml_name().as<cwstr_t>() == "profile"_s)
		{
			auto value = profile->xml_attributes()["type"];
			profileProp->AddOption(value.as<cwstr_t>());
			if (current.get() == nullptr)
			{
				current = (ang::xml::xml_node_t)profile;
				profileProp->SetValue(value.as<cwstr_t>());
			}
		}
	}
	
	SetCurrentProfile(current);*/
}
//
//void CEffectView::SetCurrentProfile(ang::xml::xml_node_t node, bool enable)
//{
//	CMFCPropertyGridProperty* profileProp = m_propMap[ProfileProp];
//	CMFCPropertyGridProperty* techniqueProp = m_propMap[TechniqueProp];
//
//	CPropertyInfo* info = (CPropertyInfo*)profileProp->GetData();
//	if (info->Data() == node)
//		return;
//
//	info->Data(node);
//
//	techniqueProp->RemoveAllOptions();
//	techniqueProp->ResetOriginalValue();
//
//	if (node)
//	{
//		profileProp->Enable(TRUE);
//		if (!node->xml_has_children())
//		{
//			SetCurrentTechnique(nullptr, true);
//			return;
//		}
//	}
//	else
//	{
//		profileProp->Enable(enable ? TRUE : FALSE);
//		SetCurrentTechnique(nullptr, false);
//		return;
//	}
//
//	ang::xml::xml_node_t current = nullptr;
//
//	for (auto technique = node->xml_children()->begin(); technique.is_valid(); ++technique)
//	{
//		if (ang::StringOperation::CompareWA(technique->xml_name().as<cwstr_t>(), "technique") == 0)
//		{
//			auto value = technique->xml_attributes()["name"];
//			techniqueProp->AddOption(value.as<cwstr_t>());
//			if (current.get() == nullptr)
//			{
//				current = (ang::xml::xml_node_t)technique;
//				techniqueProp->SetValue(value.as<cwstr_t>());
//			}
//		}
//	}
//
//	SetCurrentTechnique(current);
//}
//
//void CEffectView::SetCurrentTechnique(ang::xml::xml_node_t node, bool enable)
//{
//	CMFCPropertyGridProperty* techniqueProp = m_propMap[TechniqueProp];
//	CMFCPropertyGridProperty* passProp = m_propMap[PassProp];
//
//	CPropertyInfo* info = (CPropertyInfo*)techniqueProp->GetData();
//	if (info->Data() == node)
//		return;
//
//	info->Data(node);
//
//	passProp->RemoveAllOptions();
//	passProp->ResetOriginalValue();
//
//	if (node)
//	{
//		techniqueProp->Enable(TRUE);
//		if (!node->xml_has_children())
//		{
//			SetCurrentPass(nullptr, true);
//			return;
//		}
//	}
//	else
//	{
//		techniqueProp->Enable(enable ? TRUE : FALSE);
//		SetCurrentPass(nullptr, false);
//		return;
//	}
//
//	ang::xml::xml_node_t current = nullptr;
//
//	for (auto pass = node->xml_children()->begin(); pass.is_valid(); ++pass)
//	{
//		if (ang::StringOperation::CompareWA(pass->xml_name().as<cwstr_t>(), "pass") == 0)
//		{
//			auto value = pass->xml_attributes()["idx"];
//			passProp->AddOption(value.as<cwstr_t>());
//			if (current.get() == nullptr)
//			{
//				current = (ang::xml::xml_node_t)pass;
//				passProp->SetValue(value.as<cwstr_t>());
//			}
//		}
//	}
//
//	SetCurrentPass(current);
//}
//
//void CEffectView::SetCurrentPass(ang::xml::xml_node_t node, bool enable)
//{
//	CMFCPropertyGridProperty* passProp = m_propMap[PassProp];
//	CMFCPropertyGridProperty* vertexProp = m_propMap[VertexShaderGroup];
//	CMFCPropertyGridProperty* pixelProp = m_propMap[PixelShaderGroup];
//
//
//	CPropertyInfo* info = (CPropertyInfo*)passProp->GetData();
//	if (info->Data() == node)
//		return;
//
//	info->Data(node);
//
//	if (node)
//	{
//		passProp->Enable(TRUE);
//		if (!node->xml_has_children())
//		{
//			SetCurrentVS(nullptr, true);
//			return;
//		}
//	}
//	else
//	{
//		passProp->Enable(enable ? TRUE : FALSE);
//		SetCurrentPS(nullptr, false);
//		return;
//	}
//
//
//}
//
//
//void CEffectView::SetCurrentVS(ang::xml::xml_node_t pass, bool enable)
//{
//
//}
//
//void CEffectView::SetCurrentPS(ang::xml::xml_node_t pass, bool enable)
//{
//
//}


void CEffectView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	AdjustLayout();
}


void CEffectView::OnChangeSelection(CMFCPropertyGridProperty* pNewSel, CMFCPropertyGridProperty* pOldSel)
{
	m_pSelectedProperty = pNewSel;
	if (!pNewSel)
	{
		m_bitsEnableControls.value = 0;
		return;
	}

	CPropertyInfo* info = (CPropertyInfo*)(void*)pNewSel->GetData();
	if (info)
	{
		m_bitsEnableControls.value = info->ControlsInfo().value;
	}
	else
	{
		m_bitsEnableControls.value = 0;
	}
}

LRESULT CEffectView::OnPropertyChanged(WPARAM wparam, LPARAM lParam)
{
	CMFCPropertyGridProperty* prop = (CMFCPropertyGridProperty*)lParam;

	if (prop == nullptr || prop->GetData() == 0)
		return 0;

	CPropertyInfo* info = (CPropertyInfo*)(void*)prop->GetData();
	info->OnPropertyChange(this);

	return 0;
}

void CEffectView::OnFileSave()
{
	if (GetDocument())
	{
		GetDocument()->SaveModified();
	}
}


class CMFCToolBarEditBoxButtonEx : public CMFCToolBarEditBoxButton
{
public:
	CMFCToolBarEditBoxButtonEx() : CMFCToolBarEditBoxButton() {}
	CMFCToolBarEditBoxButtonEx(UINT uiID, int iImage, DWORD dwStyle = ES_AUTOHSCROLL, int iWidth = 0) : CMFCToolBarEditBoxButton(uiID, iImage, dwStyle, iWidth) {}
	virtual ~CMFCToolBarEditBoxButtonEx() {}
	inline CString GetContent()const { return m_strContents; }
};


void CEffectView::OnEffectviewAddnew()
{
	if (m_pSelectedProperty == NULL)
		return;

	CPropertyInfo* info = (CPropertyInfo*)(void*) m_pSelectedProperty->GetData();
	if (info == NULL)
		return;

	//ang::xml::XmlBuilder* builder = ((CXmlFileDoc*)GetDocument())->GetXmlBuilder();

	//switch (info->PropertyType())
	//{
	//case EffectProp: {		
	//	CMFCToolBarEditBoxButtonEx* button = (CMFCToolBarEditBoxButtonEx*)m_wndToolBar.GetButton(0);
	//	CString text = button->GetContent();
	//	if (text.IsEmpty())
	//		return;

	//	auto find_it = m_pEffectLibrary->xml_children()->FindFirst("effect");
	//	while (find_it.is_valid())
	//	{
	//		if (ang::StringOperation::CompareW(find_it->xml_attributes()["name"].as<cwstr_t>(), text) == 0)
	//			return;
	//		find_it = m_pEffectLibrary->xml_children()->FindNext("effect", find_it);
	//	}

	//	ang::TinyString* value = NEW_ARGS(text) ang::TinyString();

	//	auto it = builder->Find(*m_pEffectLibrary);
	//	builder->MoveTo(it);

	//	builder->beginElement("effect");
	//	builder->Attribute("name", value);
	//	auto current = builder->XmlCurrent();
	//	builder->EndElement();

	//	GetDocument()->SetModifiedFlag();

	//	m_pSelectedProperty->AddOption(text);
	//	m_pSelectedProperty->SetValue(text);

	//	SetCurrentEffect(current);

	//} break;
	//case ProfileProp:break;
	//case TechniqueProp:break;
	//case PassProp:break;

	//case VSFileProp:break;
	//case VSEntryProp:break;

	//case VSConfigProp:break;

	//case VSConstBufferProp:break;

	//case VSInputProp:break;

	//case PSFileProp:break;
	//case PSEntryProp:break;

	//case PSConfigProp:break;

	//case PSConstBufferProp:break;

	//case PSSamplerProp:break;
	//}

}

void CEffectView::OnEffectviewRemove()
{

}

void CEffectView::OnEffectviewModify()
{

}

void CEffectView::OnEffectviewUp()
{

}

void CEffectView::OnEffectviewDown()
{

}

void CEffectView::OnUpdateFileSave(CCmdUI *pCmdUI)
{
	if (GetDocument())
	{
		pCmdUI->Enable(GetDocument()->IsModified());
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CEffectView::OnUpdateEffectviewEditctrl(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(m_bitsEnableControls.Bit0);
}

void CEffectView::OnUpdateEffectviewAddnew(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(m_bitsEnableControls.Bit1);
}

void CEffectView::OnUpdateEffectviewRemove(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(m_bitsEnableControls.Bit2);
}

void CEffectView::OnUpdateEffectviewModify(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(m_bitsEnableControls.Bit3);
}

void CEffectView::OnUpdateEffectviewUp(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(m_bitsEnableControls.Bit4);
}

void CEffectView::OnUpdateEffectviewDown(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(m_bitsEnableControls.Bit5);
}


void CEffectView::OnDestroy()
{
	for (index i = 0; i < MaxSize; ++i)
	{
		auto prop = m_propMap[i];
		if (prop && prop->GetData())
		{
			CPropertyInfo* info = (CPropertyInfo*)(void*)prop->GetData();
			delete info;
		}
	}

	CView::OnDestroy();
}

void CEffectView::OnCurrentEffectChangeEvent(CObject* caller, CPropertyInfo* args)
{
	/*CPropertyInfo* info = reinterpret_cast<CPropertyInfo*>(args);
	if (info == nullptr)
		return;

	if (info->PropertyType() != EffectProp)
		return;

	if (info->Data())
	{
		auto parent = info->Data()->XmlParent();
		auto find_it = parent->xml_children()->FindFirst("effect");
		CString text = info->Property()->GetValue();
		while (find_it.is_valid())
		{
			if (ang::StringOperation::CompareW(find_it->xml_attributes()["name"].as<cwstr_t>(), text) == 0)
				break;
			find_it = parent->xml_children()->FindNext("effect", find_it);
		}
		if (find_it.is_valid())
			SetCurrentEffect((ang::xml::xml_node_t)find_it);

	}
*/
	
}