#pragma once

namespace AngineEditor
{
	using namespace ang;

	class core_view_wnd : public platform::windows::window
	{
	public:
		core_view_wnd();
		void window_proc(platform::events::message_t) override;

		ANG_DECLARE_INTERFACE();
	private:
		virtual~core_view_wnd();
	};
}

// C3DView view

class CRenderTargetWnd
	: public CWnd
{
	DECLARE_DYNAMIC(CRenderTargetWnd)
private:
	ang::platform::windows::window_t m_core_view_wnd;
	ang::graphics::idriver_t m_graphic_driver;
	ang::graphics::isurface_t m_graphic_surface;

public:
	CRenderTargetWnd();
	virtual ~CRenderTargetWnd();


	bool BeginRendering(ang::graphics::idriver_t);
	bool StopRendering();

	bool InitDriver(ang::graphics::idriver_t);
	void Update(float, float);
	void Draw();
	bool DestroyDriver();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

class C3DView : public CView
{
	DECLARE_DYNCREATE(C3DView)

protected:
	C3DView();           // protected constructor used by dynamic creation
	virtual ~C3DView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	CRenderTargetWnd m_wndRenderTarget;

	CPoint m_ptnClickDownPoint;
	BOOL m_bIsLPointerPressed;
	BOOL m_bIsRPointerPressed;
	BOOL m_bIsTrachingPointer;

public:


protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};


