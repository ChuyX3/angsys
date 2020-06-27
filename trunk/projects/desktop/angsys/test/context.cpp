#include "pch.h"
#include "test.h"

using namespace ang;
using namespace ang::graphics;

gl_context::gl_context(platform::icore_view_t wnd)
	: m_gl_context(null)
	//, m_gl_async_context(null)
	, m_hdc(null)
	, m_wnd(null)
	, m_async_worker(core::async::thread::create_dispatcher_thread())
{
	init(wnd);
}

gl_context::~gl_context()
{

}

void gl_context::dispose()
{
	//m_async_worker->run_async<void>([&](core::async::iasync<void>)
	//{
	//	wglDeleteContext(m_gl_async_context);
	//	wglMakeCurrent(NULL, NULL);
	//})->result(); //waint for completed

	m_async_worker->exit();
	m_async_worker = null;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_gl_context);
	m_gl_context = null;
	//m_gl_async_context = null;

}

void gl_context::init(platform::icore_view_t wnd)
{
	m_wnd = wnd;

	HRESULT hr;
	BOOL wglResult;
	HDC hdc = ::GetDC((HWND)m_wnd->core_view_handle());
	HGLRC hglrc;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, nPixelFormat, &pfd);
	hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
	GLenum err = glewInit();

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	
	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		HGLRC prevHglrc = hglrc;
		hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
		if (!hglrc)
		{
			hglrc = prevHglrc;
			//m_gl_async_context = wglCreateContext(hdc);
		}
		else
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(prevHglrc);
			//m_gl_async_context = wglCreateContextAttribsARB(hdc, 0, attribs);
		}
	}
	else
	{
		//m_gl_async_context = wglCreateContext(hdc);
	}

	//Checking GL version
	const GLubyte* GLVersionString = glGetString(GL_VERSION);

	//Or better yet, use the GL3 way to get the version number
//	glGetIntegerv(GL_MAJOR_VERSION, (int*)&m_gl_min_version);
//	glGetIntegerv(GL_MINOR_VERSION, (int*)&m_gl_max_version);

	m_gl_context = hglrc;
	//wglShareLists(m_gl_context, m_gl_async_context);

	//m_async_worker->run_async<void>([&](core::async::iasync<void>)
	//{
	//	wglMakeCurrent(hdc, m_gl_async_context);
	//})->result(); //waint for completed

	wglMakeCurrent(NULL, NULL);
	ReleaseDC((HWND)m_wnd->core_view_handle(), hdc);
}

bool gl_context::lock()
{
	m_mutex.lock();

	if (m_hdc == null)
	{
		m_hdc = ::GetDC((HWND)m_wnd->core_view_handle());
		wglMakeCurrent(m_hdc, m_gl_context);
	}
	return true;
}

void gl_context::present()
{
	if (m_hdc != null)
	{
		SwapBuffers(m_hdc);
	}
}

bool gl_context::unlock()
{
	if (m_hdc == null)
		return false;
	wglMakeCurrent(NULL, NULL);
	ReleaseDC((HWND)m_wnd->core_view_handle(), m_hdc);
	m_hdc = null;
	m_mutex.unlock();
	return true;
}

