#pragma once
#include "afxwin.h"
class CAngineEditorDocManager :
	public CDocManager
{
public:
	CAngineEditorDocManager();
	virtual ~CAngineEditorDocManager();

public:
	virtual CDocument* OpenDocumentFileEx(LPCTSTR lpszFileName, LPCTSTR fileType);
	virtual void OnFileNew();
	virtual void OnFileOpen();

	virtual void OnProjectNew();
	virtual void OnProjectOpen();

protected:
	virtual BOOL CreatingProjectTemplate(CString path, CString name);
};

