
#include "stdafx.h"
#include "Resource.h"

#include "mainfrm.h"
#include "FileView.h"

#include "AngineEditor.h"
#include "ProjectDocTemplate.h"
#include "ProjectManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
{
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES, &CFileView::OnUpdateProperties)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
//	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView(ang::xml::xml_document_t xmlDoc)
{
	using namespace ang;
	using namespace ang::xml;

	if (xmlDoc == nullptr)
		return;
	IntfPtr<XmlFinder> finder = new XmlFinder(xmlDoc->XmlRoot());

	auto project_name_pos = finder->FindFirst("project_name");

	HTREEITEM hRoot = m_wndFileView.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
		project_name_pos->XmlValue().As<CWStr>(), 0, 0, 0U, 0U, (LPARAM)(xml_node_t)project_name_pos, nullptr, nullptr);

	auto files_iterator = finder->FindFirst("filters");
	if (files_iterator->XmlChildren() != nullptr)
	{
		for (auto files_element = files_iterator->XmlChildren()->Begin(); files_element.IsValid(); ++files_element)
		{
			if (files_element->XmlIsTypeOf(ang::xml::XmlType::Element))
			{
				AddElement((xml::xml_node_t)files_element, hRoot);
			}
		}
	}
}

void CFileView::AddElement(ang::xml::xml_node_t node, HTREEITEM parent)
{
	using namespace ang;
	using namespace ang::xml;

	if (StringOperation::CompareW(node->XmlName().As<CWStr>(), L"folder") == 0)
	{
		xml::XmlValue name = node->XmlAttributeFinder()["name"];
		if (!name.IsEmpty())
		{
			HTREEITEM thisItme = m_wndFileView.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
				name.As<CWStr>(), 0, 0, 0U, 0U, (LPARAM)node, parent, nullptr);

			if (node->XmlChildren() != nullptr)
			{
				for (auto element = node->XmlChildren()->Begin(); element.IsValid(); ++element)
				{
					if (element->XmlIsTypeOf(XmlType::Element))
					{
						AddElement((ang::xml::xml_node_t)element, thisItme);
					}
				}
			}
			
		}
	}
	else if (StringOperation::CompareW(node->XmlName().As<CWStr>(), L"file") == 0)
	{
		xml::XmlValue name = node->XmlValue();
		if (!name.IsEmpty())
			m_wndFileView.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM,
				name.As<CWStr>(), 2, 2, 0U, 0U, (LPARAM)node, parent, nullptr);
	}
		
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();


	HTREEITEM hSelectedItem;
	hSelectedItem = pWndTree->GetSelectedItem();
	TVITEM tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = hSelectedItem;
	pWndTree->GetItem(&tvItem);
	auto node = reinterpret_cast<ang::xml::xml_node_t>(tvItem.lParam);

	if (node != nullptr && ang::StringOperation::CompareW(node->XmlName().As<ang::CWStr>(), L"file") == 0)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
		return;
	}
	//else if (node != nullptr && ang::StringOperation::CompareW(node->XmlName().As<ang::CWStr>(), L"folder") == 0)
	//{
	//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
	//	return;
	//}

	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	auto docTemp = CProjectDocTemplate::Instance();

	if (docTemp != NULL)
	{
		docTemp->ShowSEProjectProperties();
	}

}

void CFileView::OnUpdateProperties(CCmdUI *pCmdUI)
{
	//POSITION GetFirstDocPosition() const;
	//virtual CDocument* GetNextDoc(POSITION& rPos) const;

	if (CProjectDocTemplate::Instance() != NULL)
	{
		auto pos = CProjectDocTemplate::Instance()->GetFirstDocPosition();
		auto doc = CProjectDocTemplate::Instance()->GetNextDoc(pos);
		if (doc != nullptr)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
		
	}
	pCmdUI->Enable(FALSE);
	
	
	// TODO: Add your command update UI handler code here
}


void CFileView::OnFileOpen()
{
	// TODO: Add your command handler code here

	HTREEITEM hSelectedItem;
	hSelectedItem = m_wndFileView.GetSelectedItem();
	TVITEM tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = hSelectedItem;
	m_wndFileView.GetItem(&tvItem);
	auto node = reinterpret_cast<ang::xml::xml_node_t>(tvItem.lParam);

	if (node != nullptr && ang::StringOperation::CompareW(node->XmlName().As<ang::CWStr>(), L"file") == 0)
	{
		ang::String path = node->XmlAttributeFinder()["path"].As<ang::CWStr>();
		ang::String type = node->XmlAttributeFinder()["type"].As<ang::CWStr>();
		auto pos = path.Find("$(project_dir)");
		if (pos != ang::Index(-1))
		{
			path.Replace(AngineEditor::CProjectManager::Instance()->ProjectPath(), pos, pos + 14);
			
		}
		CAngineEditorApp::Instance()->OpenDocumentFileEx(path, type);
	}	
}

void CFileView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CFileView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CFileView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


