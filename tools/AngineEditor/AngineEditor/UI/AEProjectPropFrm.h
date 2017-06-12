#pragma once


// CAEProjectPropFrame

class CAEProjectPropFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CAEProjectPropFrame)
	static CAEProjectPropFrame* instance;

public:
	static CAEProjectPropFrame* Instance();

	CAEProjectPropFrame();
	virtual ~CAEProjectPropFrame();

	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
		CWnd* pParentWnd, CCreateContext* pContext = NULL);
//	void InitialUpdateFrame(CDocument* pDoc, BOOL bMakeVisible);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};


