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

namespace coffe
{
	namespace graphics
	{
		namespace gl
		{
			coffe_object(gl_driver);
			coffe_object(gl_surface);
			coffe_object(gl_frame_buffer);
			coffe_object(gl_texture);
			coffe_object(gl_shaders);

			typedef struct _model
			{
				buffers::iindex_buffer_t indices;
				buffers::ivertex_buffer_t vertices;
			}model_t, *model_ptr_t;

			template<typename... Ts>
			void gl_debug_print(int level, cstr_t format, Ts... args)
			{
				switch (level)
				{
				case 2:
					coffe_debug_output_error(format, args...);
					break;
				case 1:
					coffe_debug_output_warning(format, args...);
					break;
				default: //0
					coffe_debug_output_info(format, args...);
					break;
				}
			}

		}
	}

	template<> struct is_inherited_from<graphics::gl::gl_driver, bean> : true_type {};
	template<> struct is_inherited_from<graphics::gl::gl_driver, intf> : true_type {};

}

#include "gl/gl_buffers.h" 
#include "gl/gl_shaders.h"
#include "gl/gl_textures.h"

namespace coffe
{
	namespace graphics
	{
		namespace gl
		{


			class gl_frame_buffer
				: public smart<gl_frame_buffer, system_object, iframe_buffer, resources::iresource>
			{
				weak_ptr<gl_driver> m_parent_driver;
				string m_resource_sid;
				graphics::size<uint> m_dimentions;
				GLuint m_gl_frame_buffer;
				collections::vector<gl_texture_t> m_gl_color_textures;
				gl_texture_t m_gl_depth_stencil_texture;

			public: //internal
				inline GLuint GLFrameBUfferObject()const { return m_gl_frame_buffer; }
				inline GLuint GLColorTexture(windex idx)const { return !m_gl_color_textures.is_empty() && idx < m_gl_color_textures->counter() ? m_gl_color_textures[idx]->GLResource() : (GLuint)-1; }
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
				: public smart<gl_surface, system_object, isurface>
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
				void update() override;
				void swap_buffers(bool syncronize) override;
				iframe_buffer_t frame_buffer()const override;
				collections::ienum_ptr<collections::tuple<scenes::icamera_t, iframe_buffer_t>> cameras()const override {
					return null;
				}

			private:
				void on_display_size_changed_event(objptr, coffe::platform::events::idisplay_info_event_args_t);

			private:
				virtual~gl_surface();
			};

			
			class gl_driver
				: public smart<gl_driver, system_object, idriver, ifactory>
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
				HGLRC m_gl_async_context;
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
				mutable core::async::thread_t m_async_worker;

			public:
				gl_driver();

			public: //overrides
				void dispose()override;
				idriver_t driver()const override { return const_cast<gl_driver*>(this); }
				void set_file_system(core::files::ifile_system_t)override;
				optional<buffers::ivertex_buffer> create_vertex_buffer(buffers::buffer_usage_t usage, vector<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t init_data, string sid = null)const override;
				optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				optional<textures::itexture> create_texture(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				optional<textures::itexture> create_texture(unknown_t tex_handle, string sid = null)const override;
				optional<textures::itexture> load_texture(text::string file, textures::tex_type_t, string sid = null)const override;
				optional<textures::itexture> load_texture(array_view<text::string> files, textures::tex_type_t, string sid = null)const override;
				optional<iframe_buffer> create_frame_buffer(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, size<uint> dimentions, string sid = null)const override;
				optional<iframe_buffer> create_frame_buffer(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil_format = null, string sid = null)const override;
				optional<effects::ishaders> compile_shaders(wstring vertex_shader, wstring pixel_shader, string sid = null)const override;
				optional<effects::ishaders> compile_shaders(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid = null)const override;

				core::async::iasync_optional<buffers::ivertex_buffer> create_vertex_buffer_async(buffers::buffer_usage_t usage, vector<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t init_data, string sid = null)const override;
				core::async::iasync_optional<buffers::iindex_buffer> create_index_buffer_async(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				core::async::iasync_optional<textures::itexture> create_texture_async(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				core::async::iasync_optional<textures::itexture> create_texture_async(unknown_t tex_handle, string sid = null)const override;
				core::async::iasync_optional<textures::itexture> load_texture_async(text::string file, textures::tex_type_t, string sid = null)const override;
				core::async::iasync_optional<textures::itexture> load_texture_async(array_view<text::string> files, textures::tex_type_t, string sid = null)const override;
				core::async::iasync_optional<iframe_buffer> create_frame_buffer_async(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, size<uint> dimentions, string sid = null)const override;
				core::async::iasync_optional<iframe_buffer> create_frame_buffer_async(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil_format = null, string sid = null)const override;
				core::async::iasync_optional<effects::ishaders> compile_shaders_async(wstring vertex_shader, wstring pixel_shader, string sid = null)const override;
				core::async::iasync_optional<effects::ishaders> compile_shaders_async(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid = null)const override;

				graph_driver_type_t graph_driver_type()const override;
				ulong min_version()const override;
				ulong max_version()const override;
				isurface_t create_surface(platform::icore_view_t)const override;
				ifactory_t get_factory()const override;

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
				inline auto GLContextAsync()const { return m_gl_async_context; }

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
