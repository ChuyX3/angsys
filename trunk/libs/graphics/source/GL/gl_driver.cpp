#include "pch.h"
#include "gl/gl_driver.h"

#if OPENGL_FAMILY_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::gl;

gl_driver::gl_driver()
#if OPENGLES_SUPPORT
	: m_gl_display(null)
	, m_gl_config(null)
	, m_gl_context(null)
#else
	: m_native_contxt(null)
	, m_gl_context(null)
	, m_gl_arb_support(null)
#endif
	, m_gl_min_version(0)
	, m_gl_max_version(0)
{
	m_current_index_type = GL_UNSIGNED_INT;
	m_cull_mode = graphics::cull_mode::back;
	m_front_face = graphics::front_face::def;
	m_blend_mode = graphics::blend_mode::disable;
	m_async_worker = core::async::thread::create_dispatcher_thread();
}

void gl_driver::dispose()
{
	close_driver();
	m_async_worker->exit();
	m_async_worker = null;
}

bool gl_driver::init_driver(platform::icore_view_t view, long64 adapter_id)
{
#if OPENGLES_SUPPORT
	// initialize OpenGL ES and EGL

	/*
	* Here specify the attributes of the desired configuration.
	* Below, we select an EGLConfig with at least 8 bits per color
	* component compatible with on-screen windows
	*/
	const EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};
	EGLint format;
	EGLint numConfigs;

	m_gl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(m_gl_display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	eglChooseConfig(m_gl_display, attribs, &m_gl_config, 1, &numConfigs);

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	m_gl_context = eglCreateContext(m_gl_display, m_gl_config, NULL, contextAttribs);

	m_current_surface = new gl_surface(this);
	m_current_surface->create(view);

	if (eglMakeCurrent(m_gl_display, m_current_surface->GLSurface(), m_current_surface->GLSurface(), m_gl_context) == EGL_FALSE) {
	
		close_driver();
		return false;
	}

	m_async_worker->run_async<void>([&](core::async::iasync<void>)
	{
		m_gl_async_context = eglCreateContext(m_gl_display, m_gl_config, m_gl_context, contextAttribs);
		eglMakeCurrent(m_gl_display, m_current_surface->GLSurface(), m_current_surface->GLSurface(), m_gl_async_context);
	})->result(); //waint for completed

	return true;

#else
	HRESULT hr;
	BOOL wglResult;
	m_native_contxt = view->core_context();
	HDC hdc = (HDC)m_native_contxt->core_context_handle();
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
			m_async_worker->run_async<void>([&](core::async::iasync<void>)
			{
				m_gl_async_context = wglCreateContext(hdc);
				wglMakeCurrent(hdc, m_gl_async_context);
			})->result(); //waint for completed
		}
		else
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(prevHglrc);

			m_async_worker->run_async<void>([&](core::async::iasync<void>)
			{
				m_gl_async_context = wglCreateContextAttribsARB(hdc, 0, attribs);
				wglMakeCurrent(hdc, m_gl_async_context);
			})->result(); //waint for completed
		}
	}
	else
	{
		m_async_worker->run_async<void>([&](core::async::iasync<void>)
		{
			m_gl_async_context = wglCreateContext(hdc);
			wglMakeCurrent(hdc, m_gl_async_context);
		})->result(); //waint for completed
	}

	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	//Or better yet, use the GL3 way to get the version number
	glGetIntegerv(GL_MAJOR_VERSION, (int*)&m_gl_min_version);
	glGetIntegerv(GL_MINOR_VERSION, (int*)&m_gl_max_version);

	m_gl_context = hglrc;
	wglMakeCurrent(hdc, m_gl_context);

	wglShareLists(m_gl_context, m_gl_async_context);

	return true;
#endif
}

void gl_driver::close_driver()
{
#if OPENGLES_SUPPORT

#else
	m_async_worker->run_async<void>([&](core::async::iasync<void>)
	{
		wglDeleteContext(m_gl_async_context);
		wglMakeCurrent(NULL, NULL);
	})->result(); //waint for completed
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_gl_context);
#endif
}

graph_driver_type_t gl_driver::graph_driver_type()const
{ 
#if OPENGL_SUPPORT
	return graphics::graph_driver_type::OpenGL;
#else
	return graphics::graph_driver_type::OpenGLES;
#endif
}

ulong gl_driver::min_version()const
{
	return m_gl_min_version;
}

ulong gl_driver::max_version()const
{
	return m_gl_max_version;
}

isurface_t gl_driver::create_surface(platform::icore_view_t view)const
{
	if(m_current_surface.is_empty())
		m_current_surface = new gl_surface(const_cast<gl_driver*>(this));

	if (m_current_surface->core_view() != view.get())
	{
#if OPENGLES_SUPPORT
		eglMakeCurrent(GLDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		m_async_worker->run_async<void>([&](core::async::iasync<void>)
		{
			eglMakeCurrent(GLDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		})->result(); //waint for completed
#endif

		m_current_surface->close();
		m_current_surface->create(view);

#if OPENGLES_SUPPORT
		eglMakeCurrent(GLDisplay(), m_current_surface->GLSurface(), m_current_surface->GLSurface(), GLContext());

		m_async_worker->run_async<void>([&](core::async::iasync<void>)
		{
			eglMakeCurrent(GLDisplay(), m_current_surface->GLSurface(), m_current_surface->GLSurface(), m_gl_async_context);
		})->result(); //waint for completed
#endif
	}

	return m_current_surface.get();
}

ifactory_t gl_driver::get_factory()const
{
	return const_cast<gl_driver*>(this);
}

void gl_driver::viewport(box<float> vp)
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;

	glViewport((uint)vp.left, (uint)vp.top, (uint)vp.width(), (uint)vp.height());
}

box<float> gl_driver::viewport()const
{
	return m_viewport;
}

void gl_driver::cull_mode(cull_mode_t value)
{
	if (m_cull_mode != value)
	{
		driver_locker_t lock = this;
		m_cull_mode = value;
		if (m_cull_mode == cull_mode::none)
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glCullFace(m_cull_mode == cull_mode::front ? GL_FRONT
				: GL_BACK);
			glEnable(GL_CULL_FACE);
		}
	}
}

cull_mode_t gl_driver::cull_mode()const
{
	return m_cull_mode;
}

void gl_driver::front_face(front_face_t value)
{
	if (m_front_face != value)
	{
		glFrontFace(m_front_face == front_face::counter_clockwise ? GL_CCW : GL_CW);
	}
}

front_face_t gl_driver::front_face()const
{
	return m_front_face;
}

void gl_driver::blend_mode(blend_mode_t value)
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	m_blend_mode = value;
	if (value == blend_mode::enable)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}

blend_mode_t gl_driver::blend_mode()const
{
	return m_blend_mode;
}

void gl_driver::clear(color_t color)
{
	float _color[] = {
		(float)color.components.red / 255.0f,
		(float)color.components.green / 255.0f ,
		(float)color.components.blue / 255.0f ,
		(float)color.components.alpha / 255.0f
	};

	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		glClearColor(_color[0], _color[1], _color[2], _color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
}

void gl_driver::clear(maths::float4 const& color)
{
	glClearColor(color.get<0>(), color.get<1>(), color.get<2>(), color.get<3>());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
coord,
line,
triangle,
squere,
poligon
*/
#ifndef GL_QUADS
#define GL_QUADS 0X0005
#endif

#ifndef GL_POLYGON
#define GL_POLYGON 0X0006
#endif

static int s_primitives[] = {
	GL_POINTS,
	GL_LINES,
	GL_TRIANGLES,
	GL_QUADS,
	GL_POLYGON
};

void gl_driver::draw(uint count, primitive_t prim)
{
	if (m_current_frame_buffer.is_empty())
		return;
	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		if (prim != m_primitive)
		{
			m_primitive = prim;
		}
		glDrawArrays(s_primitives[(int)prim.get()], 0, count);
	});
}

void gl_driver::draw_indexed(uint count, primitive_t prim)
{
	if (m_current_frame_buffer.is_empty())
		return;

	core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
		if (prim != m_primitive)
		{
			m_primitive = prim;
		}
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Model->GetIndexBuffer());			// task 4 draw elements
		glDrawElements(s_primitives[(int)prim.get()], count, m_current_index_type, 0);	// draw elements
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	});

	//In task 4 Draw all triangles of model

}

void gl_driver::bind_frame_buffer(iframe_buffer_t ifb)
{
	gl_frame_buffer_t fb = interface_cast<gl_frame_buffer>(ifb.get());

	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	m_current_frame_buffer = fb;
	if (m_current_frame_buffer.get() == null)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_current_frame_buffer->GLFrameBufferObject());
	}
}

void gl_driver::bind_shaders(effects::ishaders_t sh)
{
	gl_shaders_t shaders = interface_cast<gl_shaders>(sh.get());

	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	if (shaders.get() == m_current_technique.get())
		return;
	m_current_technique = shaders;
	if (m_current_technique.get() == null)
	{
		glUseProgram(0);
	}
	else
	{
		glUseProgram(shaders->GLProgram());
		//TODO: bind attributes, uniforms, textures
	}
}

void gl_driver::bind_texture(textures::itexture_t _tex, windex idx)
{
	gl_texture_t tex = interface_cast<gl_texture>(_tex.get());
	if (tex.is_empty())
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			glActiveTexture(GL_TEXTURE0 + idx);
			glBindTexture(GL_TEXTURE_2D, 0);
			//TODO: glUniform1i(0, idx);
		});
	}
	else
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			tex->use_texture(this, idx);
		});
	}
}

void gl_driver::bind_index_buffer(buffers::iindex_buffer_t buff)
{
	gl_index_buffer_t buffer = interface_cast<gl_index_buffer>(buff.get());
	if (buffer.is_empty())
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		});

	}
	else
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GLBuffer());
			m_current_index_type = buffer->GLIndexType();
		});
	}
}

void gl_driver::bind_vertex_buffer(buffers::ivertex_buffer_t buff)
{
	gl_vertex_buffer_t buffer = interface_cast<gl_vertex_buffer>(buff.get());
	if (buffer.is_empty())
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		});
	}
	else
	{
		core::async::scope_locker<core::async::mutex_t>::lock(m_mutex, [&]() {
			glBindBuffer(GL_ARRAY_BUFFER, buffer->GLBuffer());
		});
	}
}

core::async::idispatcher_t gl_driver::dispatcher()const
{
	return m_async_worker.get();
}


bool gl_driver::lock()const
{
	if (has_thread_access())
		return true;

	if (!m_mutex.lock())
		return false;
#if OPENGLES_SUPPORT
	eglMakeCurrent(GLDisplay(), m_current_surface->GLSurface(), m_current_surface->GLSurface(), GLContext());
#else
	wglMakeCurrent(m_current_surface->GLSurface(), m_gl_context);
#endif
}

bool gl_driver::unlock()const
{
	if (has_thread_access())
		return true;

	return m_mutex.unlock();
#if OPENGLES_SUPPORT
	//eglMakeCurrent(null, null, null, null); no release for optimization
#else
	//wglMakeCurrent(null, null); no release for optimization
#endif
}

///////////////////////////////////////////////////////////////////////////////////////

void gl_driver::set_file_system(core::files::ifile_system_t fs)
{
	m_mutex.lock();
	m_fs = fs;
	m_mutex.unlock();
}

optional<buffers::ivertex_buffer> gl_driver::create_vertex_buffer(buffers::buffer_usage_t usage, array_view<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t buff, string sid)const
{
	array_view<byte> init_data = buff.is_empty() ? to_array((byte*)null, (byte*)null) : to_array((byte*)buff->buffer_ptr(), (byte*)buff->buffer_ptr() + buff->buffer_size());

	gl_vertex_buffer_t buffer = new gl_vertex_buffer();
	if (!buffer->create(
		const_cast<gl_driver*>(this),
		usage,
		vertex_desc,
		vertex_count,
		init_data,
		sid
	))
		return error_code::unknown;
	return buffer.get();
}

optional<buffers::iindex_buffer> gl_driver::create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t buff, string sid)const
{
	array_view<byte> init_data = buff.is_empty() ? to_array((byte*)null, (byte*)null) : to_array((byte*)buff->buffer_ptr(), (byte*)buff->buffer_ptr() + buff->buffer_size());
	gl_index_buffer_t buffer = new gl_index_buffer();
	if (!buffer->create(
		const_cast<gl_driver*>(this),
		usage,
		index_type,
		index_count,
		init_data,
		sid
	))
		return error_code::unknown;
	return buffer.get();
}

optional<textures::itexture> gl_driver::create_texture(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, graphics::size3d<uint> dimentions, string sid)const
{
	gl_texture_t text = new gl_texture();
	if (!text->create(
		const_cast<gl_driver*>(this),
		type,
		color_format,
		usage,
		flags,
		dimentions,
		sid
	))
		return error_code::unknown;
	return text.get();
}

optional<textures::itexture> gl_driver::create_texture(unknown_t tex_handle, string sid)const
{
	if (tex_handle == null)
		return error_code::invalid_param;
	uint resource = reinterpret_cast<uint>(tex_handle);	
	gl_texture_t text = new gl_texture();
	if (!text->attach(resource, const_cast<gl_driver*>(this), sid))
		return error_code::unknown;
	return text.get();
}

optional<textures::itexture> gl_driver::load_texture(text::string file, textures::tex_type_t type, string sid)const
{
	gl_texture_t tex = new gl_texture();
	if (!tex->load(const_cast<gl_driver*>(this), file, type, sid))
		return error_code::unknown;
	return tex.get();
}

optional<textures::itexture> gl_driver::load_texture(array_view<text::string> files, textures::tex_type_t type, string sid)const
{
	return error_code::unsupported;
}

optional<iframe_buffer> gl_driver::create_frame_buffer(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, bool is_serio, string sid)const
{
	gl_frame_buffer_t buffer = new gl_frame_buffer(const_cast<gl_driver*>(this));
	if (!buffer->create(
		color_format,
		depth_stencil_format,
		dimentions,
		sid
	))
		return error_code::unknown;
	return buffer.get();
}

optional<iframe_buffer> gl_driver::create_frame_buffer(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil, string sid)const
{
	gl_frame_buffer_t buffer = new gl_frame_buffer(const_cast<gl_driver*>(this));
	vector<gl_texture_t> textures;
	for (textures::itexture_t tex : color_tex)
		textures += interface_cast<gl_texture>(color_tex.get());
	if (!buffer->create(
		(array_view<gl_texture_t>)textures,
		interface_cast<gl_texture>(depth_stencil.get()),
		sid
	))
		return error_code::unknown;
	return buffer.get();
}

optional<effects::ishaders> gl_driver::compile_shaders(string vertex_shader, string pixel_shader, string sid)const
{
	error err;
	gl_shaders_t shaders = new gl_shaders();
	if ((err = shaders->load(
		const_cast<gl_driver*>(this),
		vertex_shader,
		pixel_shader,
		sid)).code() != error_code::success)
		return err;
	return shaders.get();
}

optional<effects::ishaders> gl_driver::compile_shaders(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid)const
{
	error err;
	gl_shaders_t shaders = new gl_shaders();
	if ((err = shaders->load(
		const_cast<gl_driver*>(this),
		vertex_shader,
		pixel_shader,
		sid)).code() != error_code::success)
		return err;
	return shaders.get();
}

core::async::iasync_op<buffers::ivertex_buffer> gl_driver::create_vertex_buffer_async(
	buffers::buffer_usage_t usage,
	array_view<reflect::attribute_desc> vertex_desc_,
	wsize vertex_count,
	ibuffer_t init_data,
	string sid)const
{
	smart<reflect::attribute_desc[]> vertex_desc = vertex_desc_;
	return create_task<optional<buffers::ivertex_buffer>>([=](core::async::iasync_op<buffers::ivertex_buffer>, gl_driver_t driver)
	{
		return driver->create_vertex_buffer(usage, vertex_desc, vertex_count, init_data, sid);
	});
}

core::async::iasync_op<buffers::iindex_buffer> gl_driver::create_index_buffer_async(
	buffers::buffer_usage_t usage,
	reflect::var_type_t index_type,
	wsize index_count,
	ibuffer_t init_data,
	string sid)const
{
	return create_task<optional<buffers::iindex_buffer>>([=](core::async::iasync_op<buffers::iindex_buffer>, gl_driver_t driver)
	{
		return driver->create_index_buffer(usage, index_type, index_count, init_data, sid);
	});
}

core::async::iasync_op<textures::itexture> gl_driver::create_texture_async(
	textures::tex_type_t type,
	textures::tex_format_t color_format,
	buffers::buffer_usage_t usage,
	buffers::buffer_bind_flag_t flags,
	size3d<uint> dimentions,
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_op<textures::itexture>, gl_driver_t driver)
	{
		return driver->create_texture(type, color_format, usage, flags, dimentions, sid);
	});
}

core::async::iasync_op<textures::itexture> gl_driver::create_texture_async(
	unknown_t tex_handle,
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_op<textures::itexture>, gl_driver_t driver)
	{
		return driver->create_texture(tex_handle, sid);
	});
}

core::async::iasync_op<textures::itexture> gl_driver::load_texture_async(
	text::string file,
	textures::tex_type_t type,
	string sid)const
{
	return create_task<optional<textures::itexture>>([=](core::async::iasync_op<textures::itexture>, gl_driver_t driver)
	{
		return driver->load_texture(file, type, sid);
	});
}

core::async::iasync_op<textures::itexture> gl_driver::load_texture_async(
	array_view<text::string> files_,
	textures::tex_type_t type,
	string sid)const
{
	array<string> files = files_;
	return create_task<optional<textures::itexture>>([=](core::async::iasync_op<textures::itexture>, gl_driver_t driver)
	{
		return driver->load_texture(files, type, sid);
	});
}

core::async::iasync_op<iframe_buffer> gl_driver::create_frame_buffer_async(
	array_view<textures::tex_format_t> color_format_,
	textures::tex_format_t depth_stencil_format,
	size<uint> dimentions,
	bool is_stereo,
	string sid)const
{
	array<textures::tex_format_t> color_format = color_format_;
	return create_task<optional<iframe_buffer>>([=](core::async::iasync_op<iframe_buffer>, gl_driver_t driver)
	{
		return driver->create_frame_buffer(color_format, depth_stencil_format, dimentions, is_stereo, sid);
	});
}

core::async::iasync_op<iframe_buffer> gl_driver::create_frame_buffer_async(
	array_view<textures::itexture_t> color_tex_,
	textures::itexture_t depth_stencil_format,
	string sid)const
{
	array<textures::itexture_t> color_tex = color_tex_;
	return create_task<optional<iframe_buffer>>([=](core::async::iasync_op<iframe_buffer>, gl_driver_t driver)
	{
		return driver->create_frame_buffer(color_tex, depth_stencil_format, sid);
	});
}

core::async::iasync_op<effects::ishaders> gl_driver::compile_shaders_async(
	string vertex_shader,
	string pixel_shader,
	string sid)const
{
	return create_task<optional<effects::ishaders>>([=](core::async::iasync_op<effects::ishaders>, gl_driver_t driver)
	{
		return driver->compile_shaders(vertex_shader, pixel_shader, sid);
	});
}

core::async::iasync_op<effects::ishaders> gl_driver::compile_shaders_async(
	effects::shader_info_t const& vertex_shader,
	effects::shader_info_t const& pixel_shader,
	string sid)const
{
	return create_task<optional<effects::ishaders>>([=](core::async::iasync_op<effects::ishaders>, gl_driver_t driver)
	{
		return driver->compile_shaders(vertex_shader, pixel_shader, sid);
	});
}




#endif
