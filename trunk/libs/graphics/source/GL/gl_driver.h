#pragma once

#include "graphic_object.h"

#if OPENGL_FAMILY_SUPPORT

#if OPENGL_SUPPORT
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GLU.h>
#elif OPENGLES_SUPPORT
#include <EGL/egl.h>
#include<GLES3/gl3.h>
#endif

namespace ang
{
	namespace graphics
	{
		namespace gl
		{
			ang_declare_object(gl_driver);
			ang_declare_object(gl_surface);
			ang_declare_object(gl_frame_buffer);
			ang_declare_object(gl_texture);
			ang_declare_object(gl_shaders);
			ang_declare_object(gl_shader_code);
			ang_declare_object(gl_index_buffer);
			ang_declare_object(gl_vertex_buffer);

			template<typename... Ts>
			void gl_debug_print(int level, castr_t format, Ts... args)
			{
				text::fast_string<text::encoding::ascii, memory::unmanaged_allocator> formater;

				switch (level)
				{
				case 2:
					formater = "error: "_sv;
					formater.append_format(format, args...);
					printf(formater.cstr());
					break;
				case 1:
					formater = "warning: "_sv;
					formater.append_format(format, args...);
					printf(formater.cstr());
					break;
				default: //0
					formater = "info: "_sv;
					formater.append_format(format, args...);
					printf(formater.cstr());
					break;
				}
			}
		}
	}
}

namespace std {
	template<> struct is_base_of<ang::object, ang::graphics::gl::gl_driver> : true_type { };
	template<> struct is_base_of<ang::iintf, ang::graphics::gl::gl_driver> : true_type { };
}

#include "gl/gl_buffers.h" 
#include "gl/gl_shaders.h"
#include "gl/gl_textures.h"

namespace ang
{
	namespace graphics
	{
		namespace gl
		{
			class gl_frame_buffer
				: public graphic<gl_frame_buffer
				, iid("ang::graphics::gl::gl_frame_buffer")
				, iframe_buffer
				, resources::iresource>
			{
				weak_ptr<gl_driver> m_parent_driver;
				string m_resource_sid;
				graphics::size<uint> m_dimentions;
				GLuint m_gl_frame_buffer;
				collections::vector<gl_texture_t> m_gl_color_textures;
				gl_texture_t m_gl_depth_stencil_texture;

			public: //internal
				inline GLuint GLFrameBufferObject()const { return m_gl_frame_buffer; }
				inline GLuint GLColorTexture(windex idx)const { return !m_gl_color_textures.is_empty() && idx < m_gl_color_textures.size() ? m_gl_color_textures[idx]->GLResource() : (GLuint)-1; }
				inline GLuint GLDepthStencilTexture()const { return !m_gl_depth_stencil_texture.is_empty() ? m_gl_depth_stencil_texture->GLResource() : (GLuint)-1; }

			public:
				gl_frame_buffer(gl_driver*);

				bool create(gl_surface_t);
				bool create(array_view<gl_texture_t> textures, gl_texture_t depth_stencil, string sid = null);
				bool create(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, string sid = null);
				bool update(gl_texture_t resource);
				bool close();

			protected: //override
				void dispose()override;

			public: //overrides	
				resources::iresource_t resource()const override;
				graphics::size<uint> dimentions()const override;
				bool is_stereo()const override;
				uint color_buffer_count()const override;
				textures::itexture_t color_buffer(windex)const;
				textures::itexture_t depth_stencil_buffer()const override;
				bool try_resize(size<uint>) override;

				resources::resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				intfptr fast_cast(resources::resource_type_t) override;
			};

			class gl_surface final
				: public graphic<gl_surface
				, iid("ang::graphics::gl::gl_surface")
				, isurface>
			{
				bool m_need_update;
				graphics::size<uint> m_pending_size;
				platform::icore_view_t m_view;
				graphics::size<uint> m_current_size;
				weak_ptr<gl_driver> m_parent_driver;
#if OPENGLES_SUPPORT
				EGLSurface m_gl_surface;
				EGLDisplay m_gl_display;
#else
				icore_context_t m_surface;
				HDC m_gl_surface;
				uint m_gl_display;//dummy
				//HGLRC m_gl_context;
#endif

				mutable gl_frame_buffer_t m_gl_frame_buffer;

			public: //internal
				graphics::size<uint> const& surface_size()const { return m_current_size; }
				inline auto GLSurface()const { return m_gl_surface; }
				inline auto GLDisplay()const { return m_gl_display; }
				inline platform::icore_view* core_view()const { return m_view.get(); }

			public:
				gl_surface(gl_driver*);

				bool create(platform::icore_view_t);
				bool update(platform::icore_view_t, graphics::size<uint> size);
				bool close();

			protected: //override
				void dispose()override;

			public: //overrides
				bool swap_buffers(bool syncronize) override;
				iframe_buffer_t frame_buffer()const override;
				collections::ienum_ptr<collections::tuple<scenes::itransform_t, iframe_buffer_t>> update()override {
					return null;
				}

			private:
				void on_display_size_changed_event(objptr, ang::platform::events::idisplay_info_event_args_t);

			private:
				virtual~gl_surface();
			};

			
			class gl_driver
				: public graphic<gl_driver
				, iid("ang::graphics::gl::gl_driver")
				, idriver
				, ifactory>
			{
			private:
#if OPENGLES_SUPPORT
				EGLDisplay m_gl_display;
				EGLConfig m_gl_config;
				EGLContext m_gl_context;
				EGLContext m_gl_async_context;
#else
				icore_context_t m_native_contxt;
				HGLRC m_gl_context;
				//HGLRC m_gl_async_context;
				bool m_gl_arb_support;
#endif
				ulong m_gl_max_version;
				ulong m_gl_min_version;

				mutable gl_surface_t m_current_surface;

				int m_current_index_type;
				gl_frame_buffer_t m_current_frame_buffer;
				gl_shaders_t m_current_technique;
				primitive_t m_primitive;

				graphics::box<float> m_viewport;
				graphics::cull_mode_t m_cull_mode;
				graphics::front_face_t m_front_face;
				graphics::blend_mode_t m_blend_mode;

				core::async::mutex_t m_mutex;
				core::files::ifile_system_t m_fs;
				mutable core::async::thread_t m_main_thread;
				mutable core::async::idispatcher_t m_async_worker;

			public:
				gl_driver();

			protected: //overrides
				void dispose()override;

			public: //overrides
				graph_driver_type_t graph_driver_type()const override;
				bool matrix_transpose_needed()const override;
				bool vr_extension_support()const override;
				ulong min_version()const override;
				ulong max_version()const override;
				isurface_t create_surface(platform::icore_view_t)const override;
				ifactory_t get_factory()const override;

				using ifactory::create_index_buffer;
				using ifactory::create_vertex_buffer;

				idriver_t driver()const override { return const_cast<gl_driver*>(this); }
				void set_file_system(core::files::ifile_system_t)override;
				optional<buffers::ivertex_buffer> create_vertex_buffer(buffers::buffer_usage_t usage, array_view<reflect::attribute_desc_t> vertex_desc, wsize vertex_count, ibuffer_t vertex_data, string sid = null)const override;
				optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				optional<textures::itexture> create_texture(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				optional<textures::itexture> create_texture(unknown_t tex_handle, string sid = null)const override;
				optional<textures::itexture> load_texture(text::string file, textures::tex_type_t, string sid = null)const override;
				optional<textures::itexture> load_texture(array_view<text::string> files, textures::tex_type_t, string sid = null)const override;
				optional<iframe_buffer> create_frame_buffer(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, size<uint> dimentions, bool is_stereo = false, string sid = null)const override;
				optional<iframe_buffer> create_frame_buffer(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil_format = null, string sid = null)const override;
				optional<effects::ishaders> compile_shaders(string vertex_shader, string pixel_shader, string sid = null)const override;
				optional<effects::ishaders> compile_shaders(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid = null)const override;

				core::async::iasync_op<buffers::ivertex_buffer> create_vertex_buffer_async(buffers::buffer_usage_t usage, array_view<reflect::attribute_desc_t> vertex_desc, wsize vertex_count, ibuffer_t vertex_data, string sid = null)const override;
				core::async::iasync_op<buffers::iindex_buffer> create_index_buffer_async(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				core::async::iasync_op<textures::itexture> create_texture_async(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				core::async::iasync_op<textures::itexture> create_texture_async(unknown_t tex_handle, string sid = null)const override;
				core::async::iasync_op<textures::itexture> load_texture_async(text::string file, textures::tex_type_t, string sid = null)const override;
				core::async::iasync_op<textures::itexture> load_texture_async(array_view<text::string> files, textures::tex_type_t, string sid = null)const override;
				core::async::iasync_op<iframe_buffer> create_frame_buffer_async(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, size<uint> dimentions, bool is_stereo = false, string sid = null)const override;
				core::async::iasync_op<iframe_buffer> create_frame_buffer_async(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil_format = null, string sid = null)const override;
				core::async::iasync_op<effects::ishaders> compile_shaders_async(string vertex_shader, string pixel_shader, string sid = null)const override;
				core::async::iasync_op<effects::ishaders> compile_shaders_async(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid = null)const override;	

				void viewport(box<float> vp) override;
				box<float> viewport()const override;

				void cull_mode(cull_mode_t) override;
				cull_mode_t cull_mode()const override;

				void front_face(front_face_t) override;
				front_face_t front_face()const override;

				void blend_mode(blend_mode_t) override;
				blend_mode_t blend_mode()const override;

				void bind_frame_buffer(iframe_buffer_t) override;
				void bind_shaders(effects::ishaders_t) override;
				void bind_texture(textures::itexture_t, windex idx) override;
				void bind_index_buffer(buffers::iindex_buffer_t) override;
				void bind_vertex_buffer(buffers::ivertex_buffer_t) override;

				void clear(maths::float4 const&) override;
				void clear(color_t) override;
				void draw(uint count, primitive_t) override;
				void draw_indexed(uint count, primitive_t) override;
				
				core::async::idispatcher_t dispatcher()const override;

				bool lock()const override;
				bool unlock()const override;
				bool has_thread_access()const { return m_async_worker->has_thread_access(); }

				virtual bool init_driver(platform::icore_view_t wnd, long64 adapter_id);

			protected:
				virtual void close_driver();

			public: //intenal

#if OPENGLES_SUPPORT
				inline auto GLDisplay()const { return m_gl_display; };
				inline auto GLConfig()const { return m_gl_config; };
#endif
				inline auto GLContext()const { return m_gl_context; }
				//inline auto GLContextAsync()const { return m_gl_async_context; }

				inline gl_frame_buffer_t current_frame_buffer()const { return m_current_frame_buffer.get(); }
				template<typename T>
				core::async::iasync<T> create_task(function<T(core::async::iasync<T>, gl_driver_t)> func)const {
					gl_driver_t auto_save = const_cast<gl_driver*>(this);
					return m_async_worker->run_async<T>([auto_save, func](core::async::iasync<T> async) {
						return func(async, auto_save);
					});
				}
				inline core::files::ifile_system* get_file_system()const { return m_fs.is_empty() ? core::files::ifile_system::instance().get() : m_fs.get(); }
				
			};
		}
	}
}


#endif
