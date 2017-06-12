#pragma once
class CProjectDocTemplate :
	public CSingleDocTemplate
{
	static CProjectDocTemplate* instance;
public:
	static CProjectDocTemplate* Instance();

	CProjectDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~CProjectDocTemplate();
	DECLARE_DYNAMIC(CProjectDocTemplate)

	// Menu and accel table for MDI Child windows of this type
	HMENU m_hMenuShared;
	HACCEL m_hAccelTable;

	virtual void LoadTemplate();
	virtual void AddDocument(CDocument* pDoc);
	virtual void RemoveDocument(CDocument* pDoc);
	virtual POSITION GetFirstDocPosition() const;
	virtual CDocument* GetNextDoc(POSITION& rPos) const;
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bAddToMRU, BOOL bMakeVisible);
	virtual void SetDefaultTitle(CDocument* pDocument);

	BOOL ShowSEProjectProperties(BOOL show = TRUE);
};

