#include "pch.h"
#include "gl/gl_driver.h"

#if OPENGL_FAMILY_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::gl;


gl_surface::gl_surface(gl_driver* driver)
	: m_parent_driver(driver)
	, m_gl_surface(NULL)
	, m_gl_display(NULL)
	, m_gl_frame_buffer(null)
{
}

gl_surface::~gl_surface()
{
	
}

void gl_surface::dispose()
{
	//close();
	m_parent_driver = null;
}


bool gl_surface::create(platform::icore_view_t view)
{
	gl_driver_t driver = m_parent_driver.lock();
	m_view = view;

#if OPENGLES_SUPPORT
	EGLint format;
	m_gl_display = driver->GLDisplay();

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(GLDisplay(), driver->GLConfig(), EGL_NATIVE_VISUAL_ID, &format);

	auto window = reinterpret_cast<ANativeWindow*>(view->core_view_handle());

	ANativeWindow_setBuffersGeometry(window, 0, 0, format);
	m_gl_surface = eglCreateWindowSurface(GLDisplay(), driver->GLConfig(), window, NULL);

	int w, h;
	eglQuerySurface(GLDisplay(), GLSurface(), EGL_WIDTH, &w);
	eglQuerySurface(GLDisplay(), GLSurface(), EGL_HEIGHT, &h);
	m_current_size = size<uint>(w, h);
#else
	m_surface = m_view->core_context();
	m_gl_surface = (HDC)m_surface->core_context_handle();
#endif
	view->dispatcher()->notify(new ang::platform::events::display_size_change_event(this, &gl_surface::on_display_size_changed_event));
}

bool gl_surface::update(platform::icore_view_t view, graphics::size<uint> size)
{
	return true;
}

bool gl_surface::close()
{
#if OPENGLES_SUPPORT
	if (GLSurface() != EGL_NO_SURFACE) {
		eglDestroySurface(GLDisplay(), GLSurface());
	}
	m_gl_surface = EGL_NO_SURFACE;
#else
	m_gl_surface = null;

#endif
	m_view = null;
	return true;
}

bool gl_surface::swap_buffers(bool syncronize)
{
#if OPENGLES_SUPPORT
	eglSwapBuffers(GLDisplay(), GLSurface());
#else
	SwapBuffers(m_gl_surface);
#endif
	return true;
}

iframe_buffer_t gl_surface::frame_buffer()const
{
	if (m_gl_frame_buffer.is_empty()) {
		m_gl_frame_buffer = new gl_frame_buffer(m_parent_driver.lock());
		m_gl_frame_buffer->create(const_cast<gl_surface*>(this));
	}
	return m_gl_frame_buffer.get();
}

void gl_surface::on_display_size_changed_event(objptr, ang::platform::events::idisplay_info_event_args_t args)
{
#if OPENGLES_SUPPORT
	int w, h;
	eglQuerySurface(GLDisplay(), GLSurface(), EGL_WIDTH, &w);
	eglQuerySurface(GLDisplay(), GLSurface(), EGL_HEIGHT, &h);
	m_current_size = size<uint>(w, h);
#else
	auto sf = args->display_info().display_scale_factor;
	auto sz = args->display_info().display_resolution;
	m_current_size = size<uint>((uint)(sz.width * sz.width), (uint)(sz.height  * sz.height));
#endif
		
	if (m_gl_frame_buffer.is_empty()) {
		m_gl_frame_buffer = new gl_frame_buffer(m_parent_driver.lock());
		m_gl_frame_buffer->create(const_cast<gl_surface*>(this));
	}
	else {
		m_gl_frame_buffer->close();
		m_gl_frame_buffer->create(const_cast<gl_surface*>(this));
	}
}


#endif
