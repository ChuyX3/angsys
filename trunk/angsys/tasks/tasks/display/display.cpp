#include "pch.h"
#include "dsplay.h"
#include <stdio.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;

typedef struct class_regist_args
{
	wstring class_name;
	uint class_style;
	WNDPROC wndproc;
	uint menu_name;
	pointer hicon;
	pointer hbrush;
	pointer hcursor;

}class_regist_args_t, *class_regist_args_ptr_t;

wstring regist_wnd_class(class_regist_args_ptr_t args)
{
	WNDCLASSEXW wcex;

	if (GetClassInfoExW(GetModuleHandle(NULL), (cwstr_t)args->class_name, &wcex))
	{
		return args->class_name.get();
	}

	wcex.cbSize = sizeof(WNDCLASSEXW);

	wcex.style = args->class_style;
	wcex.lpfnWndProc = args->wndproc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = (HICON)args->hicon;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINGRAPH3D));
	wcex.hCursor = args->hcursor ? (HCURSOR)args->hcursor : LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)args->hbrush ? (HBRUSH)args->hbrush : (HBRUSH)GetStockObject(BLACK_BRUSH);// (HBRUSH)GetStockObject(BLACK_BRUSH); //(HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName = args->menu_name ? MAKEINTRESOURCEW(args->menu_name) : NULL;
	wcex.lpszClassName = (cwstr_t)args->class_name;
	wcex.hIconSm = NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (RegisterClassExW(&wcex) == 0)
		return null;
	return args->class_name.get();
}

LRESULT WINAPI window::WndProc(HWND hwnd, UINT msg, WPARAM wprm, LPARAM lprm)
{
	window_t wnd = null;

	if (msg == WM_CREATE && lprm != 0)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lprm;
		wnd = (window*)pcs->lpCreateParams;
		if (!wnd.is_empty())
		{
			wnd->add_ref();
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)wnd.get());
		}
	}
	else
	{
		wnd = (window*)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	}
	
	if (!wnd.is_empty())
	{	
		return (LRESULT)wnd->wndproc(msg, (pointer)wprm, (pointer)lprm);
	}

	return DefWindowProcW(hwnd, msg, wprm, lprm);
}

window::window()
	: handle(null)
{
	
}

window::window(wstring title, wstring name, rect<float> size, HWND parent)
{
	create(title, name, size, parent);
}

window::~window()
{

}

point<float> window::position()const {
	POINT ptn = {0 , 0};
	ClientToScreen(handle, &ptn);
	return{ (float)ptn.x, (float)ptn.y };
}

void window::position(point<float> p)const
{
	POINT ptn = { (int)p.x, (int)p.y };
	ClientToScreen(handle, &ptn);
	SetWindowPos(handle, null, ptn.x, ptn.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
}

size<float> window::layout()const
{
	RECT rc;
	GetClientRect(handle, &rc);
	return{ (float)rc.right, (float)rc.bottom };
}

void window::layout(size<float> sz)const
{
	SetWindowPos(handle, null, 0, 0, (int)sz.width, (int)sz.height, SWP_NOMOVE | SWP_NOACTIVATE);
}

HWND window::hwnd()const { return handle; }
HDC window::hdc()const { return null; }

int window::run(wstring title, wstring name, rect<float> size, HWND parent)
{
	if (!create(title.get(), name.get(), size, parent))
		return -1;
	add_ref();
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (WM_QUIT == msg.message) {
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			draw_event(this, this);
		}

	} while (msg.message != WM_QUIT);
	release();
	return 0;
}

bool window::create(wstring title, wstring name, rect<float> size, HWND parent)
{
	if (IsWindow(handle))
		return false;

	HMODULE instance = GetModuleHandleA(NULL);
	WNDCLASSW wndclass = { 0 };
	class_regist_args_t class_arg = { name
		, CS_OWNDC | CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW , &window::WndProc, 0, null, null, null};

	wstring class_name = regist_wnd_class(&class_arg);

	if (class_name.is_empty())
	{
		class_arg.class_name = "ang::windows::window"_s;
		class_name = regist_wnd_class(&class_arg);
	}

	handle = CreateWindowExW(0, (cwstr_t)class_name
		, (cwstr_t)title, (uint)(WS_OVERLAPPEDWINDOW)
		, size.left, size.top, size.width(), size.height()
		, (HWND)parent, null
		, instance, this);

	if (handle == null)
	{
		HRESULT hr = GetLastError();
		printf("Error:%d\n", hr);
		UnregisterClassW((cwstr_t)class_name, instance);
		return false;
	}

	ShowWindow(handle, SW_SHOW);

	return true;
}

bool window::close()
{
	if (!IsWindow(handle))
		return false;
	SendMessageW(handle, WM_CLOSE, 0, 0);
	return true;
}

bool window::destroy()
{
	if (!IsWindow(handle))
		return false;
	SetWindowLongPtrW(handle, GWLP_USERDATA, (LONG_PTR)null);
	release(); // remove reference
	return true;
}

pointer window::wndproc(uint msg, pointer wprm, pointer lprm)
{
	switch (msg)
	{
	case WM_CREATE:
		create_event(this, this);
		break;
	case WM_MOUSEMOVE:
		
		break;
	case WM_ERASEBKGND:
		
		return 0;
	case WM_PAINT:
		ValidateRect(handle, null);
		draw_event(this, this);
		return 0;
	case WM_DESTROY:
		destroy_event(this, this);
		PostQuitMessage(0);
		destroy();
		break;
	}
	return (pointer)DefWindowProc(handle, msg, (WPARAM)wprm, (LPARAM)lprm);
}


