#include "pch.h"
#include "ang/platform/win32/windows.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::platform;
using namespace ang::platform::windows;

//#if defined _DEBUG
//#define new ANG_DEBUG_NEW()
//#endif

typedef struct _hdc
{
	HWND hWnd;
	HDC hDC;
	PAINTSTRUCT ps;
}*hdc_t;

graphics::device_context::device_context(window_t wnd)
	: handle(null)
{
	create(wnd);
}

graphics::device_context::~device_context()
{
	close();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::device_context);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::device_context, object, icore_context);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::device_context, object, icore_context);

pointer graphics::device_context::core_context_handle()const
{
	return get_HDC();
}

bool graphics::device_context::bind_graphic_native_surface(pointer)
{
	return false;
}

bool graphics::device_context::is_valid()const
{
	return handle != null;
}

bool graphics::device_context::create(pointer args)
{
	if (args == null)
		return false;

	window_t wnd = reinterpret_cast<window*>(args);

	if (!IsWindow((HWND)wnd->core_view_handle()))
		return false;

	hdc_t dc = new _hdc();
	if (dc != null)
	{
		memset(dc, 0, sizeof(_hdc));
		dc->hWnd = (HWND)wnd->core_view_handle();
		dc->hDC = GetDC((HWND)dc->hWnd);
		if (!dc->hDC)
		{
			delete dc;
			dc = null;
		}
	}
	handle = dc;
	return handle ? true : false;
}

bool graphics::device_context::close()
{
	if (!handle)
		return false;
	ReleaseDC(hdc_t(handle)->hWnd, hdc_t(handle)->hDC);
	delete hdc_t(handle);
	handle = null;
	return true;
}

pointer graphics::device_context::get_HDC()const
{
	if (is_valid())
		return hdc_t(handle)->hDC;
	return null;
}

graphics::point<float> graphics::device_context::move_to(graphics::point<float> ptn)
{
	POINT p;
	if (MoveToEx((HDC)get_HDC(), (int)ptn.x, (int)ptn.y, &p))
		return graphics::point<float>((float)p.x, (float)p.y);
	return graphics::point<float>(0, 0);
}

graphics::point<float> graphics::device_context::current_position()const
{
	POINT p;
	if (GetCurrentPositionEx((HDC)get_HDC(), &p))
		return graphics::point<float>((float)p.x, (float)p.y);
	return graphics::point<float>(0, 0);
}

graphics::point<float> graphics::device_context::draw_line_to(graphics::point<float> p)
{
	if (::LineTo((HDC)get_HDC(), (int)p.x, (int)p.y))
		return p;
	return current_position();
}

void graphics::device_context::draw_primitive(primitive_t, const collections::ienum<graphics::point<float>>* ptns, bool solid)
{
	if (ptns == null)
		return;

	if (!solid)
	{
		bool first = true;
		for (auto it = ptns->begin(); it.current() != null; ++it)
		{
			graphics::point<float> ptn = *it;
			if (first)
			{
				first = false;
				move_to(*it);
			}
			else
				draw_line_to(*it);
		}
		draw_line_to(*ptns->begin());
	}
	//else
	//{

	//}
}

///////////////////////////////////////////////////////

graphics::paint_dc::paint_dc(window_t wnd)
	: device_context(wnd)
{
}

graphics::paint_dc::~paint_dc()
{
	close();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::paint_dc);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::paint_dc, device_context);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::paint_dc, device_context);

bool graphics::paint_dc::create(pointer args)
{
	if (!args)
		return false;

	window_t wnd = reinterpret_cast<window*>(args);
	if (!IsWindow((HWND)wnd->core_view_handle()))
		return false;

	hdc_t dc = new _hdc();
	if (dc != null)
	{
		memset(dc, 0, sizeof(_hdc));

		dc->hWnd = (HWND)wnd->core_view_handle();
		BeginPaint((HWND)dc->hWnd, &dc->ps);
		dc->hDC = dc->ps.hdc;

		ValidateRect(dc->hWnd, &dc->ps.rcPaint);
	}
	handle = dc;
	return handle ? true : false;
}

bool graphics::paint_dc::close()
{
	if (!handle)
		return false;
	EndPaint(hdc_t(handle)->hWnd, &hdc_t(handle)->ps);
	delete hdc_t(handle);
	handle = null;
	
	return true;
}

graphics::rect<float> graphics::paint_dc::clip_area()const
{
	if (!is_valid())
		return graphics::rect<float>();

	return graphics::rect<float>((float)hdc_t(handle)->ps.rcPaint.left
		, (float)hdc_t(handle)->ps.rcPaint.top
		, (float)hdc_t(handle)->ps.rcPaint.right
		, (float)hdc_t(handle)->ps.rcPaint.bottom);
}