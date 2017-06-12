
// ShaderView.cpp : implementation of the CCodeView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AngineEditor.h"
#endif

#include "CodeDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "CodeView.h"

#include "FontStyleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCodeView

IMPLEMENT_DYNCREATE(CCodeView, CView)

BEGIN_MESSAGE_MAP(CCodeView, CView)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, &CCodeView::OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, &CCodeView::OnEditCut)
	ON_COMMAND(ID_EDIT_FIND, &CCodeView::OnEditFind)
	ON_COMMAND(ID_EDIT_PASTE, &CCodeView::OnEditPaste)
	ON_COMMAND(ID_EDIT_REDO, &CCodeView::OnEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CCodeView::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_REPLACE, &CCodeView::OnEditReplace)
	ON_COMMAND(ID_EDIT_UNDO, &CCodeView::OnEditUndo)
	ON_COMMAND(ID_FILE_SAVE, &CCodeView::OnFileSave)
END_MESSAGE_MAP()

// CCodeView construction/destruction

CCodeView::CCodeView()
{
	// TODO: add construction code here

}

CCodeView::~CCodeView()
{
}

BOOL CCodeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


const char* wordList =
"and and_eq asm auto bitand bitor bool break case "
"catch char class compl const const_cast continue "
"default delete do double dynamic_cast else enum "
"explicit export extern false float for friend goto if "
"inline int long mutable namespace new not not_eq "
"operator or or_eq private protected public register "
"reinterpret_cast return short signed sizeof static "
"static_cast struct switch template this throw true "
"try typedef typeid typename union unsigned using "
"virtual void volatile wchar_t while xor xor_eq "
"float2 float3 float4 float2x2 float3x3 float4x4 "
"vec2 vec3 vec4 mat2 mat3 mat4 sampler2D "
"matrix cbuffer uniform attribute varying ";

const char* wordList0 = "abstract as base break case catch checked continue default delegate do else event explicit extern false finally fixed for foreach goto if implicit in interface internal is lock namespace new null object operator out override params private protected public readonly ref return sealed sizeof stackalloc switch this throw true try typeof unchecked unsafe using virtual while"
	"bool byte char class const decimal double enum float int long sbyte short static string struct uint ulong ushort void"
	"float2 float3 float4 float2x2 float3x3 float4x4 vec2 vec3 vec4 mat2 mat3 mat4 sampler2D matrix cbuffer uniform attribute varying ";

void CCodeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_wndLexerCtrl.Init();
	CString path = GetDocument()->GetPathName();
	m_wndLexerCtrl.LoadFile(path);


	//m_wndLexerCtrl.SetFontName(STYLE_DEFAULT, L"Consoles");

	m_wndLexerCtrl.SendMessage(SCI_SETKEYWORDS, 0, (LPARAM)wordList0);
	//m_wndLexerCtrl.SendMessage(SCI_SETKEYWORDS, 1, (LPARAM)wordList1);
	//m_wndLexerCtrl.SendMessage(SCI_SETKEYWORDS, 2, (LPARAM)wordList2);
	
//	m_wndLexerCtrl.SetForeground(int nStyle, COLORREF crForeground);

//	m_wndLexerCtrl.SetBackground(int nStyle, COLORREF crBackground);

	m_wndLexerCtrl.Refresh();
}

BOOL CCodeView::OnNotify(
	WPARAM wParam,       //@parm not used
	LPARAM lParam,       //@parm pointer to notification structure
	LRESULT* pResult)   //@parm result
{
	NMHDR *phDR;
	phDR = (NMHDR*)lParam;
	// does notification come from my scintilla window?
	if (phDR != NULL && phDR->hwndFrom == m_wndLexerCtrl.m_hWnd)
	{
		SCNotification *pMsg = (SCNotification*)lParam;
		switch (phDR->code)
		{
		case SCN_STYLENEEDED:
			break;
		case SCN_CHARADDED:
			break;
		case SCN_SAVEPOINTREACHED:
			break;
			//       called when the document is changed - mark document modified
		case SCN_SAVEPOINTLEFT:
		{
			CDocument *pDoc = GetDocument();
			pDoc->SetModifiedFlag(TRUE);
		}
		break;
		case SCN_MODIFYATTEMPTRO:
			break;
		case SCN_KEY:
			break;
		case SCN_DOUBLECLICK:
			break;
			//       called when something changes and we want to show new indicator state or brace matching
		case SCN_UPDATEUI:
		{
			m_wndLexerCtrl.UpdateUI();
		}
		break;
		case SCN_MODIFIED:
			break;
		case SCN_MACRORECORD:
			break;
			//       user clicked margin - try folding action
		case SCN_MARGINCLICK:
		{
			m_wndLexerCtrl.DoDefaultFolding(pMsg->margin, pMsg->position);
		}
		break;
		case SCN_NEEDSHOWN:
			break;
		case SCN_PAINTED:
			break;
		case SCN_USERLISTSELECTION:
			break;
		case SCN_URIDROPPED:
			break;
		case SCN_DWELLSTART:
			break;
		case SCN_DWELLEND:
			break;
		}
		return TRUE; // we processed the message
	}
	return CView::OnNotify(wParam, lParam, pResult);
}

void CCodeView::OnDraw(CDC*)
{

}

void CCodeView::Serialize(CArchive& ar)
{
	if (!m_wndLexerCtrl.GetSafeHwnd())
		return;

	if (ar.IsStoring())
	{
		m_wndLexerCtrl.SaveFileDoc(ar);
	}
	else
	{
		m_wndLexerCtrl.LoadFileDoc(ar);
	}
}

void CCodeView::OnDestroy()
{
   CView::OnDestroy();
}


void CCodeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CCodeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCodeView diagnostics

#ifdef _DEBUG
void CCodeView::AssertValid() const
{
	CView::AssertValid();
}

void CCodeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCodeDoc* CCodeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCodeDoc)));
	return (CCodeDoc*)m_pDocument;
}
#endif //_DEBUG


// CCodeView message handlers


int CCodeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndLexerCtrl.Create(WS_CHILD | WS_VISIBLE, CRect(0,0,100,100), this);

	return 0;
}


void CCodeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		m_wndLexerCtrl.MoveWindow(0, 0, cx, cy);
	}
}


void CCodeView::OnEditCopy()
{
	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		m_wndLexerCtrl.Copy();
	}
}


void CCodeView::OnEditCut()
{
	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		m_wndLexerCtrl.Cut();
	}
}


void CCodeView::OnEditFind()
{
	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		//m_wndLexerCtrl.Find();
	}
}


void CCodeView::OnEditPaste()
{
	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		m_wndLexerCtrl.Paste();
	}
}


void CCodeView::OnEditRedo()
{
	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		m_wndLexerCtrl.Redo();
	}
}


void CCodeView::OnEditUndo()
{
	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		m_wndLexerCtrl.Undo();
	}
}

void CCodeView::OnEditSelectAll()
{
	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		m_wndLexerCtrl.SelectAll();
	}
}


void CCodeView::OnEditReplace()
{
	if (m_wndLexerCtrl.GetSafeHwnd())
	{
		//m_wndLexerCtrl.ReplaceAll();
	}
}


void CCodeView::OnFileSave()
{
	/*CSyntaxColorPickerDlg dlg;
	auto info = m_wndLexerCtrl.GetColorInfo();
	ang::Array<ang::Storage::Pair<ang::String, ang::Drawing::Color>> colorInfo = {
		{ "0???", info[0] },
		{ "Comment Type 1", info[1] },
		{ "Comment Type 2", info[2] },
		{ "3???", info[3] },
		{ "4???", info[4] },
		{ "Reserved Words", info[5] },
		{ "Strings", info[6] },
		{ "7???", info[7] },
		{ "8???", info[8] },
		{ "Proprocesors", info[9] },
		{ "Operators", info[10] },
		{ "Text", info[11] },
		{ "12???", info[12] },
	};
	dlg.SetColorInfo(ang::Move(colorInfo));

	if(dlg.DoModal() == IDOK)
	{
		colorInfo = dlg.GetColorInfo();
		m_wndLexerCtrl.SetColorInfo({
			{ colorInfo[0].Datum() },
			{ colorInfo[1].Datum() },
			{ colorInfo[2].Datum() },
			{ colorInfo[3].Datum() },
			{ colorInfo[4].Datum() },
			{ colorInfo[5].Datum() },
			{ colorInfo[6].Datum() },
			{ colorInfo[7].Datum() },
			{ colorInfo[8].Datum() },
			{ colorInfo[9].Datum() },
			{ colorInfo[10].Datum() },
			{ colorInfo[11].Datum() },
			{ colorInfo[12].Datum() },
		});
	}*/

	CFontStyleDlg dlg;
	auto styleIdx = m_wndLexerCtrl.GetCurrentStyle();

	
	CString font = m_wndLexerCtrl.GetFontName(styleIdx);
	dlg.SetFontInfo(font, 
		m_wndLexerCtrl.GetForeground(styleIdx),
		m_wndLexerCtrl.GetBackground(styleIdx),
		m_wndLexerCtrl.GetFontHeight(styleIdx),
		m_wndLexerCtrl.GetBold(styleIdx),
		m_wndLexerCtrl.GetItalic(styleIdx),
		FALSE,
		m_wndLexerCtrl.GetUnderline(styleIdx)
		);


	if (dlg.DoModal() == IDOK)
	{
		auto style = dlg.GetFontInfo();
		m_wndLexerCtrl.SetFontName(styleIdx, style.strFontName.Data());	
		m_wndLexerCtrl.SetFontHeight(styleIdx, style.nFontSize);
		m_wndLexerCtrl.SetBold(styleIdx, style.bBold);
		m_wndLexerCtrl.SetItalic(styleIdx, style.bItalic);
		m_wndLexerCtrl.SetForeground(styleIdx, style.nForegroundColor.ColorRef());
		m_wndLexerCtrl.SetBackground(styleIdx, style.nBackgroundColor.ColorRef());
		//m_wndLexerCtrl.SetUnderline(styleIdx, style.bUnderline);
	}

	// TODO: Add your command handler code here
}
