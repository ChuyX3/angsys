#include "stdafx.h"
#include "AngineEditor.h"
#include "AngineEditorDocManager.h"
#include "UI/NewProjectDlg.h"
#include "UI/EffectView.h"

//#include "ProjectDocTemplate.h"

using namespace ang;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CAngineEditorDocManager::CAngineEditorDocManager()
{
}

CAngineEditorDocManager::~CAngineEditorDocManager()
{
	//delete CProjectDocTemplate::Instance();
}

void CAngineEditorDocManager::OnFileNew()
{
	CDocManager::OnFileNew();
}

void CAngineEditorDocManager::OnFileOpen()
{
	CDocManager::OnFileOpen();
}

class CDocTemplateDummy : public CDocTemplate
{
private:
	CDocTemplateDummy() : CDocTemplate(NULL,NULL,NULL,NULL){}
	~CDocTemplateDummy() {}
public:
	inline CRuntimeClass* GetFrameRuntimeClass()const { return m_pFrameClass; }
	inline CRuntimeClass* GetViewRuntimeClass()const { return m_pViewClass; }
	inline CRuntimeClass* GetDocRuntimeClass()const { return m_pDocClass; }
};

CDocument* CAngineEditorDocManager::OpenDocumentFileEx(LPCTSTR lpszFileName, LPCTSTR fileType)
{
	//OpenDocumentFile(lpszFileName, TRUE);
	// find the highest confidence
	POSITION pos = m_templateList.GetHeadPosition();
	CDocTemplate::Confidence bestMatch = CDocTemplate::noAttempt;
	CDocTemplate* pBestTemplate = NULL;
	CDocument* pOpenDocument = NULL;

	if (fileType == "effect_library"_s)
	{
		while (pos != NULL)
		{
			CDocTemplateDummy* pTemplate = (CDocTemplateDummy*)m_templateList.GetNext(pos);
			ASSERT_KINDOF(CDocTemplate, pTemplate);
		
			if (pTemplate->GetViewRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CEffectView)))
			{
				pTemplate->MatchDocType(lpszFileName, pOpenDocument);
				pBestTemplate = pTemplate;
				break;
			}
		}
	}
	else if (fileType == "model_file"_s)
	{
		pBestTemplate = NULL;
	}
	else if (fileType == "shader_file"_s)
	{
		return OpenDocumentFile(lpszFileName, TRUE);
	}
	else
	{
		return OpenDocumentFile(lpszFileName, TRUE);
	}

	if (pOpenDocument != NULL)
	{
		POSITION posOpenDoc = pOpenDocument->GetFirstViewPosition();
		if (posOpenDoc != NULL)
		{
			CView* pView = pOpenDocument->GetNextView(posOpenDoc); // get first one
			ASSERT_VALID(pView);
			CFrameWnd* pFrame = pView->GetParentFrame();

			if (pFrame == NULL)
				TRACE(traceAppMsg, 0, "Error: Can not find a frame for document to activate.\n");
			else
			{
				pFrame->ActivateFrame();

				if (pFrame->GetParent() != NULL)
				{
					/*CFrameWnd* pAppFrame;
					if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
					{
						ASSERT_KINDOF(CFrameWnd, pAppFrame);
						pAppFrame->ActivateFrame();
					}*/
				}
			}
		}
		else
			TRACE(traceAppMsg, 0, "Error: Can not find a view for document to activate.\n");

		return pOpenDocument;
	}

	if (pBestTemplate == NULL)
	{
		return NULL;
	}
	return pBestTemplate->OpenDocumentFile(lpszFileName, TRUE, TRUE);
}


void CAngineEditorDocManager::OnProjectNew()
{
	CNewProjectDlg dlg;
	INT_PTR result = dlg.DoModal();
	if (result == IDOK)
	{
		if (m_templateList.IsEmpty())
		{
			TRACE(traceAppMsg, 0, "Error: no document templates registered with CWinApp.\n");
			AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
			return;
		}

		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetHead();
		ASSERT(pTemplate != NULL);
		ASSERT_KINDOF(CDocTemplate, pTemplate);

		if (CreatingProjectTemplate(dlg.GetProjectFolder() + dlg.GetProjectName(), dlg.GetProjectName()) == FALSE)
		{
			return;
		}

		pTemplate->OpenDocumentFile(dlg.GetProjectFolder()  + dlg.GetProjectName() + _T("\\") + dlg.GetProjectName() + L".aep", FALSE);
		// if returns NULL, the user has already been alerted
	}

}

void CAngineEditorDocManager::OnProjectOpen()
{

}

BOOL CAngineEditorDocManager::CreatingProjectTemplate(CString path, CString name)
{
	using namespace ang;
	using namespace ang::xml;

	xml::xml_document_t xml_doc = new xml::xml_document();
	core::files::input_text_file_t project_template = new core::files::input_text_file(L".\\templates\\project_template.xml"_s);
	try
	{
		xml_doc->load(project_template);
		project_template = null;
	}
	catch (exception_t)
	{
		return FALSE;
	}

	CreateDirectoryW(path, NULL);
	xml::xml_builder_t builder = xml_doc->xml_tree()->as<xml::xml_builder>();
	if (!builder.is_empty())
	{
		builder->move_to(builder->find_first("project_name"));
		builder->value(cwstr_t(name.GetBuffer(), name.GetLength()));
		//builder->move_to(builder->find_first("project_dir"));
		//builder->value(cwstr_t(path.GetBuffer(), path.GetLength()));
		auto it = builder->find_first("filters");

		if (it.is_valid())
		{
			xml::xml_tree_t finder = new xml::xml_tree((xml::xml_node*)it);
			for (auto item = finder->begin(); item.is_valid(); ++item)
			{
				auto name = item->xml_name().as<cwstr_t>();
				if (name == "folder"_s)
				{
					auto pathTemplate = item->xml_attributes()["path"].as<cwstr_t>();
					if (pathTemplate.get())
					{
						wstring newPath = cwstr_t(path.GetBuffer(), path.GetLength());
						newPath << "\\" << pathTemplate;
						CreateDirectoryW((cwstr_t)newPath, NULL);
					}
				}
				else if (name == "file"_s)
				{
					auto pathTemplate = item->xml_attributes()["path"].as<cwstr_t>();
					auto fileName = item->xml_value().as<cwstr_t>();
					if (pathTemplate.get())
					{
						wstring newPath = cwstr_t(path.GetBuffer(), path.GetLength());
						newPath << "\\"_s << pathTemplate;
						wstring tempPath = ".\\templates\\"_s;
						tempPath += fileName;
						CopyFileW((cwstr_t)tempPath, (cwstr_t)newPath, TRUE);
					}
				}
			}
		}
		
	}


	wstring newPath = cwstr_t(path.GetBuffer(), path.GetLength());
	newPath << "\\"_s << cwstr_t(name.GetBuffer(), name.GetLength()) << ".aep"_s;
	core::files::output_text_file_t project_file = new core::files::output_text_file(newPath, text::encoding::utf_8);
	xml_doc->save(project_file);

	return TRUE;
}