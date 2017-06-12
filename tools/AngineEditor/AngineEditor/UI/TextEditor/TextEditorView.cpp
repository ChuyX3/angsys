// DemoEditorView.cpp : implementation of the CTextEditorView class
//

#include "stdafx.h"
#include "ShaderEditor.h"

#include "TextEditor\TextEditorDoc.h"
#include "TextEditor\TextEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextEditorView

IMPLEMENT_DYNCREATE(CTextEditorView, CCtrlView)

BEGIN_MESSAGE_MAP(CTextEditorView, CCtrlView)
	//{{AFX_MSG_MAP(CTextEditorView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChangeEdit)
	ON_NOTIFY_REFLECT(EN_MSGFILTER, OnMsgfilterEdit)
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CCtrlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CCtrlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CCtrlView::OnFilePrintPreview)



//	ON_COMMAND_RANGE(ID_SUGGEST_START,ID_SUGGEST_END,OnSuggest)
//	ON_UPDATE_COMMAND_UI_RANGE(ID_SUGGEST_START,ID_SUGGEST_END,OnSuggestUI)
//	ON_COMMAND(ID_ADDTODICTIONNARY, OnAddtodictionnary)
//	ON_UPDATE_COMMAND_UI(ID_ADDTODICTIONNARY, OnUpdateAddtodictionnary)
//	ON_COMMAND(ID_IGNORALWAYS, OnIgnoralways)
//	ON_UPDATE_COMMAND_UI(ID_IGNORALWAYS, OnUpdateIgnoralways)



	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEditorView construction/destruction

CTextEditorView::CTextEditorView()  : CCtrlView(_T("RICHEDIT"), AFX_WS_DEFAULT_VIEW |	WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |	ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL | ES_SELECTIONBAR)
{
		m_bChangeAll = TRUE;
		m_bInContextMenu = FALSE;


}

CTextEditorView::~CTextEditorView()
{
}

BOOL CTextEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CCtrlView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTextEditorView drawing

void CTextEditorView::OnDraw(CDC* pDC)
{
	CTextEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTextEditorView printing

BOOL CTextEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTextEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTextEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTextEditorView diagnostics

#ifdef _DEBUG
void CTextEditorView::AssertValid() const
{
	CCtrlView::AssertValid();
}

void CTextEditorView::Dump(CDumpContext& dc) const
{
	CCtrlView::Dump(dc);
}

CTextEditorDoc* CTextEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTextEditorDoc)));
	return (CTextEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextEditorView message handlers





static DWORD CALLBACK readFunction(DWORD dwCookie,
							 LPBYTE lpBuf,	//the buffer to fill
							 LONG nCount,	//the no. of bytes to read
							 LONG* nRead) // no. of bytes read
{
	CFile* fp = (CFile *)dwCookie;
	*nRead = fp->Read(lpBuf,nCount);
	return 0;
}

static DWORD CALLBACK writeFunction(DWORD dwCookie,
							 LPBYTE lpBuf,	//the buffer to fill
							 LONG nCount,	//the no. of bytes to read
							 LONG* nRead) // no. of bytes read
{
	CFile *pFile = (CFile*)dwCookie;
	try
	{
		pFile->Write(lpBuf, nCount);
	}
	catch(CFileException* pEX)
	{
		pEX;
		*nRead = 0;
		return 1;
	};

	*nRead = nCount;
	return 0;
}


// -------------------------------------------------------------------------------------
/*! 
 *
 * \param sFileName   
 *
 * \return void   
 */
bool CTextEditorView::writeFile(LPCTSTR  sFileName)
{
	CString oldfile(sFileName);
	oldfile += ".Bak";

	::MoveFileEx(sFileName,oldfile,MOVEFILE_REPLACE_EXISTING);

	try
	{
		CFile file(sFileName,CFile::modeCreate | CFile::modeWrite|CFile::typeBinary);
		m_es.dwCookie = (DWORD)&file;
		m_es.pfnCallback = writeFunction;
		GetRichEditCtrl().StreamOut(SF_TEXT,m_es);
		file.Close();
		GetRichEditCtrl().SetModify(FALSE);
	}
	catch(...) 
	{
		return false;
	}
	return true;

}







// -------------------------------------------------------------------------------------
/*! 
 *
 * \param sFileName   
 *
 * \return void   
 */
bool CTextEditorView::readFile(LPCTSTR  sFileName)
{

	try
	{

	//set redraw to false to reduce flicker, and to speed things up
	GetRichEditCtrl().LimitText(0);
	
	CFile file(sFileName,CFile::modeRead|CFile::typeBinary);
	m_es.dwCookie = (DWORD)&file;
	m_es.pfnCallback = readFunction;
	GetRichEditCtrl().StreamIn(SF_TEXT,m_es);
	file.Close();
	GetRichEditCtrl().SetModify(FALSE);
	}
	catch(...) 
	{
		return false;
	}
	return true;




}






// -------------------------------------------------------------------------------------
/*! 
 *
 * \param file   
 *
 * \return void   
 */
BOOL CTextEditorView::Load(LPCTSTR file)
{
	GetRichEditCtrl().SetRedraw(FALSE); // avoid flash screen
	bool ret = readFile(file);
	m_colorizer.ColorizeAll();	// color all the file 
	GetRichEditCtrl().SetRedraw(TRUE);
	GetRichEditCtrl().RedrawWindow();

	return ret;

}


// -------------------------------------------------------------------------------------
/*! 
 *
 * \param file   
 *
 * \return void   
 */
BOOL CTextEditorView::Save(LPCTSTR file)
{
	
	return writeFile(file);
}









//--------------------------------------------------------------------------------------------
//
//
//
//
void CTextEditorView::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu)
{



	ASSERT(pMenu != NULL);
	// check the enabled state of various menu items

	CCmdUI state;
	state.m_pMenu = pMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pMenu->m_hMenu)
		state.m_pParentMenu = pMenu;    // parent == child for tracking popup
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = GetTopLevelParent();
			// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		state.m_nID = pMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
			OnInitMenuPopup(state.m_pSubMenu,0,0);
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, 1 && state.m_nID < 0xF000);
		}

		// adjust for menu deletions and additions
		UINT nCount = pMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

// -----------------------------------------------------------------------------------------
//  
//  
//  
//  
//  
//  


void CTextEditorView::UpdateDynaMenu(CCmdUI* pCmdUI,CStringArray & list,CString & m_strOriginal)
{

	CMenu* pMenu = pCmdUI->m_pMenu;
	if (m_strOriginal.IsEmpty() && pMenu != NULL)	
	{
		pMenu->GetMenuString(pCmdUI->m_nID, m_strOriginal, MF_BYCOMMAND);
	}


	if (list.GetSize() == 0)
	{
		// no MRU files
		if (!m_strOriginal.IsEmpty()) 	pCmdUI->SetText(m_strOriginal);
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCmdUI->m_pMenu == NULL)
		return;

	for (int iMRU = 0; iMRU < list.GetSize(); iMRU++)
		pCmdUI->m_pMenu->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

	CString strName;
	CString strTemp;
	for (iMRU = 0; iMRU < list.GetSize(); iMRU++)
	{

		if(iMRU >= list.GetSize() ) break;

		strName = list[iMRU];

		// double up any '&' characters so they are not underlined
		LPCTSTR lpszSrc = strName;
		LPTSTR lpszDest = strTemp.GetBuffer(strName.GetLength()*2);
		while (*lpszSrc != 0)
		{
			if (*lpszSrc == '&')
				*lpszDest++ = '&';
			if (_istlead(*lpszSrc))
				*lpszDest++ = *lpszSrc++;
			*lpszDest++ = *lpszSrc++;
		}
		*lpszDest = 0;
		strTemp.ReleaseBuffer();

		int flg = 0;
		if(iMRU && (iMRU%16) == 0 )  flg = MFT_MENUBARBREAK;


		pCmdUI->m_pMenu->InsertMenu(pCmdUI->m_nIndex++,
			MF_STRING | MF_BYPOSITION | flg, pCmdUI->m_nID++,
			strTemp);
	}

	// update end menu count
	pCmdUI->m_nIndex--; // point to last menu added
	pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

	pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
//  
//		SPELLER IMPLEMNTATION
//  
//  
//  

void CTextEditorView::OnSuggest(UINT id )
{

	int idx= id -ID_SUGGEST_START;
	m_colorizer.ReplaceWord(m_ContextRange,m_tSuggestlist[idx]); 
	




}

// -----------------------------------------------------------------------------------------
//  
//  
//  
//  
//  
//  
void CTextEditorView::OnSuggestUI(CCmdUI* pCmdUI)
{


	pCmdUI->Enable(FALSE);
	if(!m_bSpellEnabled) return;

	m_colorizer.SuggestWords(m_tSuggestlist,m_ContextRange);

	if(!m_tSuggestlist.GetSize() || pCmdUI->m_pSubMenu)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);

		UpdateDynaMenu(pCmdUI,m_tSuggestlist,m_strOrgSuggest);
	}
	
}







// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnAddtodictionnary() 
{
	m_colorizer.AddToDictionnary(m_ContextRange);
}


// -------------------------------------------------------------------------------------
/*! 
 *
 * \param pCmdUI   
 *
 * \return void   
 */
void CTextEditorView::OnUpdateAddtodictionnary(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(FALSE);
	if(!m_bSpellEnabled) return;
	CString text ;
	text.Format("Add to dictionary  \"%s\"", m_colorizer.GetWord(m_ContextRange));
	pCmdUI->SetText(text);
	pCmdUI->Enable(TRUE);
}


// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnIgnoralways() 
{
	m_colorizer.IgnoreAlways(m_ContextRange);
	
}


// -------------------------------------------------------------------------------------
/*! 
 *
 * \param pCmdUI   
 *
 * \return void   
 */
void CTextEditorView::OnUpdateIgnoralways(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(FALSE);
	if(!m_bSpellEnabled) return;

	CString text;
	text.Format("Ignore Always \"%s\"", m_colorizer.GetWord(m_ContextRange));
	pCmdUI->SetText(text);
	pCmdUI->Enable(TRUE);
	
}


// -------------------------------------------------------------------------------------
/*!
 *
 * \param nFlags   
 * \param point   
 *
 * \return void   
 */
void CTextEditorView::OnRButtonDown(UINT nFlags, CPoint point) 
{

	CCtrlView::OnRButtonDown(nFlags, point);
	m_bInContextMenu = TRUE;
	
	GetRichEditCtrl().ClientToScreen(&point);
	m_bSpellEnabled = m_colorizer.GetSpellSelFromPos(point.x,point.y,m_ContextRange);


	CMenu menu;
	menu.LoadMenu(IDR_CTX);
	CMenu *pop = menu.GetSubMenu(0);
	OnInitMenuPopup(pop,0,0);
	pop->TrackPopupMenu(TPM_CENTERALIGN,point.x, point.y,this);
	m_bInContextMenu = FALSE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//						
//							COLORIZER IMP
//
//
//
//






// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnDestroy() 
{
	m_colorizer.Terminate();
	CCtrlView::OnDestroy();
}


// -------------------------------------------------------------------------------------
/*! 
 *
 * \param pCmdUI   
 *
 * \return void   
 */
void CTextEditorView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetRichEditCtrl().CanUndo());
	
}


// -------------------------------------------------------------------------------------
/*! 
 *
 * \param pCmdUI   
 *
 * \return void   
 */
void CTextEditorView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
}
// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnEditCopy()
{
	GetRichEditCtrl().Copy();

}




// -------------------------------------------------------------------------------------
/*! 
 *
 * \param pCmdUI   
 *
 * \return void   
 */
void CTextEditorView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetRichEditCtrl().CanPaste());
	
}



// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnEditPaste()
{
	GetRichEditCtrl().Paste();
	m_colorizer.ColorizeAll();
}


// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnEditCut()
{
	GetRichEditCtrl().Cut();
	m_colorizer.ColorizeAll();
}


// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnEditUndo()
{
	GetRichEditCtrl().Undo();
	m_colorizer.ColorizeAll();
}


// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnEditClear()
{
	GetRichEditCtrl().Clear();
	m_colorizer.ColorizeAll();

}


// -------------------------------------------------------------------------------------
/*! 
 *
 * \param pCmdUI   
 *
 * \return void   
 */
void CTextEditorView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	
	
}

BOOL CTextEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_bInContextMenu)
	{

		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW ));
		return TRUE;
	}
	else
	{
		return CCtrlView::OnSetCursor(pWnd, nHitTest, message);
	}
}


//
//
//					INIT AND NOTIFICATION
//
//




// -------------------------------------------------------------------------------------
/*! 
 *
 * \param pNMHDR   
 * \param pResult   
 *
 * \return void   
 */
void CTextEditorView::OnMsgfilterEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	MSGFILTER *pMsgFilter = reinterpret_cast<MSGFILTER *>(pNMHDR);
	if(pMsgFilter->msg == WM_CHAR)
	{
		switch(pMsgFilter->wParam)		
		{
			case '*':
			case '/':
				{
					m_bChangeAll = true;
					break;// '*' comment could affect all the file
				}
					
		}
		CHARRANGE cr;
		GetRichEditCtrl().GetSel(cr);
		if(cr.cpMax -cr.cpMin)// when we remove, it could remove '*/' or '/*'
		{
			m_bChangeAll = true;
		}
	}
	if(pMsgFilter->msg == WM_KEYDOWN)
	{
		switch(pMsgFilter->wParam)		
		{
				case VK_BACK:
				case VK_DELETE:
				case VK_RETURN:
					m_bChangeAll = true;
					break;
		}
	}
	if(m_bChangeAll)
	{
		CHARRANGE cr;
		GetRichEditCtrl().GetSel(cr);
		m_nextedComments  = m_colorizer.AnalyseMultiCommentLine(GetRichEditCtrl().LineFromChar(cr.cpMin));
	}
	*pResult = 0;
}





// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return void   
 */
void CTextEditorView::OnChangeEdit() 
{
	CHARRANGE cr;
	GetRichEditCtrl().GetSel(cr);
	int line = GetRichEditCtrl().LineFromChar(cr.cpMin);
	int nextedComment = m_colorizer.ColorizeLine(line,false);
	if(m_bChangeAll && m_nextedComments != nextedComment)
	{
		m_colorizer.ColorizeAll();
		TRACE(" Multi-Comments Changed %d \n",line);
	}
	GetRichEditCtrl().SendMessage(EM_SCROLLCARET,0,0);
	m_bChangeAll = false;
}








// -------------------------------------------------------------------------------------
/*! 
 *
 * \param lpCreateStruct   
 *
 * \return int   
 */
int CTextEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CCtrlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// init to reicieve changes
	long mask = GetRichEditCtrl().GetEventMask();
	GetRichEditCtrl().SetEventMask(mask | ENM_KEYEVENTS | ENM_CHANGE);


	GetRichEditCtrl().SetOptions(ECOOP_OR   ,ECO_AUTOHSCROLL   );

	
	m_colorizer.Initialize(&GetRichEditCtrl());
	
	return 0;
}