// UI/C3DView.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI/C3DView.h"
//#include <Tempus.h>
#include "ProjectManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AngineEditor::core_view_wnd::core_view_wnd()
{

}

AngineEditor::core_view_wnd::~core_view_wnd()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(AngineEditor::core_view_wnd, ang::platform::windows::window)

void AngineEditor::core_view_wnd::window_proc(ang::platform::events::message_t msg)
{
	if (msg->msg() == ang::platform::events::win_msg_enum::EraseBkgnd)
	{
		msg->result(0);
	}
	else
	{
		window::window_proc(msg);
	}
}



CRenderTargetWnd::CRenderTargetWnd()
{
	m_core_view_wnd = new AngineEditor::core_view_wnd();
	//ang::graphics::idriver_t m_graphic_driver;
	//ang::graphics::isurface_t m_graphic_surface;

	m_core_view_wnd->drawEvent += new ang::platform::events::draw_event(this, &CRenderTargetWnd::on_draw_event);
	m_core_view_wnd->destroyedEvent += new ang::platform::events::destroyed_event([this](ang::objptr, ang::platform::events::imsg_event_args_t) {
		StopRendering();
	});
}

CRenderTargetWnd::~CRenderTargetWnd()
{
	m_core_view_wnd = nullptr;
}

IMPLEMENT_DYNAMIC(CRenderTargetWnd, CWnd)

void CRenderTargetWnd::on_draw_event(ang::objptr, ang::platform::events::idraw_event_args_t)
{
	m_timer.tick([&](double delta, double total) 
	{
		Update((float)delta, (float)total);
		DestroyDriver();
	});
}

bool CRenderTargetWnd::BeginRendering(ang::graphics::idriver_t)
{
	m_timer.reset();
	SetTimer(123, 16, nullptr);
	return true;
}

bool CRenderTargetWnd::StopRendering()
{
	this->KillTimer(123);
	return true;
}

bool CRenderTargetWnd::InitDriver(ang::graphics::idriver_t driver)
{
	m_graphic_driver = driver;
	if (!m_graphic_driver.is_empty())
	{
		m_graphic_surface = m_graphic_driver->create_surface(m_core_view_wnd.get());

	}
	return true;
}

void CRenderTargetWnd::Update(float delta, float total)
{

}

void CRenderTargetWnd::Draw()
{
	m_graphic_driver->bind_frame_buffer(m_graphic_surface->frame_buffer());
	m_graphic_driver->clear(ang::graphics::colors::aero_blue);

	m_graphic_surface->swap_buffers();
}

bool CRenderTargetWnd::DestroyDriver()
{
	StopRendering();
	m_graphic_driver->bind_frame_buffer(nullptr);
	m_graphic_surface = nullptr;
	m_graphic_driver = nullptr;
	return true;
}


BEGIN_MESSAGE_MAP(CRenderTargetWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CRenderTargetWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rc;
	GetClientRect(rc);
	m_core_view_wnd->create(new ang::platform::windows::wnd_create_args{
		"3d_view_wnd","3d view", {0,0,(float)rc.Width(), (float)rc.Height()},
		ang::platform::windows::wnd_style::shown + ang::platform::windows::wnd_style::child,
		GetSafeHwnd(), nullptr
	});
	InitDriver(AngineEditor::CProjectManager::instance()->GraphicDriver());
	return 0;
}

void CRenderTargetWnd::OnSize(UINT nType, int cx, int cy)
{
	if (!m_core_view_wnd.is_empty() && m_core_view_wnd->get_core_view_handle())
	{
		m_core_view_wnd->size({ float(cx), float(cy) });
	}
}

void CRenderTargetWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 123 && m_core_view_wnd->get_core_view_handle())
	{
		::InvalidateRect((HWND)m_core_view_wnd->get_core_view_handle(), NULL, FALSE);
	}
		
}

// C3DView


IMPLEMENT_DYNCREATE(C3DView, CView)

C3DView::C3DView()
{
	m_bIsTrachingPointer = FALSE;
	m_bIsLPointerPressed = FALSE;
	m_bIsRPointerPressed = FALSE;
}

C3DView::~C3DView()
{
}

BEGIN_MESSAGE_MAP(C3DView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEHWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// C3DView drawing

void C3DView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

	//if (!driver.IsPointerEmpty())
	//{
	//	context->ClearRenderTarget({ 0.1f,0.2f,0.5f,1 });


	//	surface->SwapBuffers(nullptr);
	//}
	//

	// TODO: add draw code here
}


// C3DView diagnostics

#ifdef _DEBUG
void C3DView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void C3DView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// C3DView message handlers


int C3DView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rc;
	GetClientRect(rc);
	m_wndRenderTarget.Create(NULL, TEXT(""), WS_VISIBLE | WS_CHILD, rc, this, 12345);

	return 0;
}

void C3DView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_wndRenderTarget.BeginRendering(AngineEditor::CProjectManager::instance()->GraphicDriver());
}

void C3DView::OnDestroy()
{
	m_wndRenderTarget.StopRendering();
	CView::OnDestroy();
}

void C3DView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndRenderTarget.GetSafeHwnd())
	{
		m_wndRenderTarget.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOMOVE);
	}
	
}

BOOL C3DView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void C3DView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bIsRPointerPressed)
		return CView::OnLButtonDown(nFlags, point);;

	CRect rc;
	GetClientRect(&rc);

	if (rc.PtInRect(point))
	{
		m_ptnClickDownPoint = point;
		m_bIsLPointerPressed = TRUE;
		SetCapture();
	}

	CView::OnLButtonDown(nFlags, point);
}

void C3DView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bIsLPointerPressed)
	{
		m_bIsLPointerPressed = FALSE;
		ReleaseCapture();
	}
	CView::OnLButtonUp(nFlags, point);
}

void C3DView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_bIsLPointerPressed)
		return CView::OnRButtonDown(nFlags, point);

	CRect rc;
	GetClientRect(&rc);

	if (rc.PtInRect(point))
	{
		m_ptnClickDownPoint = point;
		m_bIsRPointerPressed = TRUE;
		SetCapture();
	}
	CView::OnRButtonDown(nFlags, point);
}

void C3DView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsRPointerPressed)
	{
		m_bIsRPointerPressed = FALSE;
		ReleaseCapture();
	}

	CView::OnRButtonUp(nFlags, point);
}

void C3DView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMButtonDown(nFlags, point);
}


void C3DView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMButtonUp(nFlags, point);
}


void C3DView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);
}


BOOL C3DView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void C3DView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseHWheel(nFlags, zDelta, pt);
}

void C3DView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void C3DView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
