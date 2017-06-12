#include "stdafx.h"
#include "ProjectDocTemplate.h"
#include "UI/AEProjectPropFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CProjectDocTemplate, CSingleDocTemplate)

CProjectDocTemplate* CProjectDocTemplate::instance = NULL;

CProjectDocTemplate* CProjectDocTemplate::Instance()
{
	return instance;
}

CProjectDocTemplate::CProjectDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
	CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
	: CSingleDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{
	instance = this;
	m_hMenuShared = NULL;
	m_hAccelTable = NULL;
}


CProjectDocTemplate::~CProjectDocTemplate()
{
	if (m_pOnlyDoc)
	{
		m_pOnlyDoc->m_bAutoDelete = TRUE;
		m_pOnlyDoc->OnCloseDocument();
	}
	instance = NULL;
}


void CProjectDocTemplate::LoadTemplate()
{
	CDocTemplate::LoadTemplate();

	if (m_nIDResource != 0 && m_hMenuShared == NULL)
	{
		HINSTANCE hInst = AfxFindResourceHandle(
			MAKEINTRESOURCE(m_nIDResource), RT_MENU);
		m_hMenuShared = ::LoadMenuW(hInst, MAKEINTRESOURCEW(m_nIDResource));
		m_hAccelTable =
			::LoadAcceleratorsW(hInst, MAKEINTRESOURCEW(m_nIDResource));
	}

#ifdef _DEBUG
	// warnings about missing components (don't bother with accelerators)
	if (m_hMenuShared == NULL)
		TRACE(traceAppMsg, 0, "Warning: no shared menu for document template #%d.\n",
			m_nIDResource);
#endif //_DEBUG
}

void CProjectDocTemplate::AddDocument(CDocument* pDoc)
{
	CSingleDocTemplate::AddDocument(pDoc);
}

void CProjectDocTemplate::RemoveDocument(CDocument* pDoc)
{
	CSingleDocTemplate::RemoveDocument(pDoc);
}

POSITION CProjectDocTemplate::GetFirstDocPosition() const
{
	return CSingleDocTemplate::GetFirstDocPosition();
}

CDocument* CProjectDocTemplate::GetNextDoc(POSITION& rPos) const
{
	return CSingleDocTemplate::GetNextDoc(rPos);
}


CDocument* CProjectDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	return OpenDocumentFile(lpszPathName, TRUE, bMakeVisible);
}

//__declspec(dllimport) BOOL g_bRemoveFromMRU;

CDocument* CProjectDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible)
{
	CDocument* pDocument = NULL;
	CFrameWnd* pFrame = NULL;
	BOOL bWasModified = FALSE;

	if (m_pOnlyDoc != NULL)
	{
		// already have a document - reinit it
		pDocument = m_pOnlyDoc;
		if (!pDocument->SaveModified())
		{
			// set a flag to indicate that the document being opened should not
			// be removed from the MRU list, if it was being opened from there
			//g_bRemoveFromMRU = FALSE;
			return NULL;        // leave the original one
		}
	}
	else
	{
		// create a new document
		pDocument = CreateNewDocument();
		pDocument->m_bAutoDelete = FALSE;
	}

	if (pDocument == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}

	CWaitCursor wait;

	// open an existing document
	bWasModified = pDocument->IsModified();
	pDocument->SetModifiedFlag(FALSE);  // not dirty for open

	if (!pDocument->OnOpenDocument(lpszPathName))
	{
		// user has been alerted to what failed in OnOpenDocument
		TRACE(traceAppMsg, 0, "CDocument::OnOpenDocument returned FALSE.\n");
		if (!pDocument->IsModified())
		{
			// original document is untouched
			pDocument->SetModifiedFlag(bWasModified);
		}
		else
		{
			// we corrupted the original document
			SetDefaultTitle(pDocument);

			if (!pDocument->OnNewDocument())
			{
				TRACE(traceAppMsg, 0, "Error: OnNewDocument failed after trying "
					"to open a document - trying to continue.\n");
				// assume we can continue
			}
		}
		return NULL;        // open failed
	}
	pDocument->SetPathName(lpszPathName, bAddToMRU);
	pDocument->OnDocumentEvent(CDocument::onAfterOpenDocument);
	
	if (bMakeVisible)
	{
		if (CAEProjectPropFrame::Instance() == NULL)
		{
			// create frame - set as main document frame
			BOOL bAutoDelete = pDocument->m_bAutoDelete;
			pDocument->m_bAutoDelete = FALSE;
			// don't destroy if something goes wrong
			pFrame = CreateNewFrame(pDocument, NULL);
			pDocument->m_bAutoDelete = bAutoDelete;
			if (pFrame == NULL)
			{
				AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
				delete pDocument;       // explicit delete on error
				return NULL;
			}		
		}
		else
		{
			pFrame = CAEProjectPropFrame::Instance();
		}
		InitialUpdateFrame(pFrame, pDocument, TRUE);
	}

	return pDocument;
}

void CProjectDocTemplate::SetDefaultTitle(CDocument* pDocument)
{
	CSingleDocTemplate::SetDefaultTitle(pDocument);
}

BOOL CProjectDocTemplate::ShowSEProjectProperties(BOOL show)
{
	CFrameWnd *pFrame = CAEProjectPropFrame::Instance();
	if (pFrame != NULL && IsWindow(pFrame->m_hWnd))
	{
		if (!show)
		{
			pFrame->SendMessage(WM_CLOSE);
			return TRUE;
		}
		else
		{
			InitialUpdateFrame(pFrame, m_pOnlyDoc, TRUE);
		}
	}
	else if (show && m_pOnlyDoc != NULL)
	{

		// create frame - set as main document frame
		BOOL bAutoDelete = m_pOnlyDoc->m_bAutoDelete;
		m_pOnlyDoc->m_bAutoDelete = FALSE;
		// don't destroy if something goes wrong
		pFrame = CreateNewFrame(m_pOnlyDoc, NULL);
		m_pOnlyDoc->m_bAutoDelete = bAutoDelete;
		InitialUpdateFrame(pFrame, m_pOnlyDoc, TRUE);
		return TRUE;
	}
	return FALSE;
}

