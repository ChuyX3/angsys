#pragma once

#include "graphic_object.h"

#if DIRECTX9_SUPPORT

#include <d3d9.h>
//#include <d3dcompiler.h>
//#include <d2d1_3.h>
//#include <dwrite_3.h>

#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "dwrite.lib")
//#pragma comment(lib, "d2d1.lib")

#include <ang/inline/com_wrapper.hpp>

namespace ang
{
	namespace graphics
	{

		namespace d3d9
		{
			ang_declare_object(d3d9_driver);
			ang_declare_object(d3d9_surface);
			ang_declare_object(d3d9_frame_buffer);
			ang_declare_object(d3d9_texture);
			ang_declare_object(d3d9_shaders);
			ang_declare_object(d3d9_shader_code);
			ang_declare_object(d3d9_index_buffer);
			ang_declare_object(d3d9_vertex_buffer);

#if DIRECTX11_VRX_SUPPORT
			ang_declare_object(holographic_driver);
			ang_declare_object(holographic_scene);
			ang_declare_object(holographic_camera);
#endif

			template<typename... Ts>
			void d3d9_debug_print(int level, castr_t format, Ts... args)
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
	template<> struct is_base_of<ang::object, ang::graphics::d3d9::d3d9_driver> : true_type { };
	template<> struct is_base_of<ang::iintf, ang::graphics::d3d9::d3d9_driver> : true_type { };
}

#include "d3d9/d3d9_buffers.h" 
#include "d3d9/d3d9_shaders.h"
#include "d3d9/d3d9_textures.h"

namespace ang
{
	namespace graphics
	{
		namespace d3d9
		{

			class d3d9_frame_buffer
				: public graphic<d3d9_frame_buffer
				, iid("ang::graphics::d3d9::d3d9_frame_buffer")
				, iframe_buffer
				, resources::iresource>
			{
				weak_ptr<d3d9_driver> m_parent_driver;
				string m_resource_sid;
				graphics::size<uint> m_dimentions;
			
			public:
				d3d9_frame_buffer(d3d9_driver*);

				bool create(d3d9_surface_t);
				bool create(array_view<d3d9_texture_t> textures, d3d9_texture_t depth_stencil, string sid = null);
				bool create(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, bool stereo = false, string sid = null);
				bool update(d3d9_texture_t resource);
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

			class d3d9_surface final
				: public graphic<d3d9_surface
				, iid("ang::graphics::d3d9::d3d9_surface")
				, isurface>
			{
				bool m_need_update;
				graphics::size<uint> m_pending_size;
				platform::icore_view_t m_view;
				graphics::size<uint> m_current_size;
				weak_ptr<d3d9_driver> m_parent_driver;
				//com_wrapper<IDXGISwapChain1> m_dxgi_swap_chain;
				d3d9_frame_buffer_t m_d3d_frame_buffer;

			public: //internal
				graphics::size<uint> const& surface_size()const { return m_current_size; }
			
			public:
				d3d9_surface(d3d9_driver*);

				bool create(platform::icore_view_t);
				bool update(platform::icore_view_t, graphics::size<uint> size);
				bool close();

			protected: //override
				void dispose()override;

			public: //overrides
				collections::ienum_ptr<collections::tuple<scenes::itransform_t, iframe_buffer_t>> update() override;
				bool swap_buffers(bool syncronize) override;
				iframe_buffer_t frame_buffer()const override;

			private:
				void on_display_size_changed_event(objptr, ang::platform::events::idisplay_info_event_args_t);

			};

			class d3d9_driver
				: public graphic<d3d9_driver
				, iid("ang::graphics::d3d9::d3d9_driver")
				, idriver
				, ifactory>
			{
			private:
				ulong m_max_version;
				ulong m_min_version;
				//D3D_FEATURE_LEVEL m_feature_level;
				com_wrapper<IDirect3D9Ex> m_d3d9;
				com_wrapper<IDirect3DDevice9Ex> m_d3d9_device;
				//com_wrapper<IDXGIAdapter3> m_dxgi_adapter;
				//com_wrapper<IDXGIFactory2> m_dxgi_factory;
				//com_wrapper<ID3D11Device2> m_d3d_device;
				//com_wrapper<ID3D11DeviceContext2> m_d3d_context;
				//com_wrapper<ID3D11BlendState> m_d3d_blend_state;
				//collections::stack_array<com_wrapper<ID3D11SamplerState>, 3> m_d3d_samplers;

				d3d9_frame_buffer_t m_current_frame_buffer;
				d3d9_shaders_t m_current_technique;
				primitive_t m_primitive;

				graphics::box<float> m_viewport;
				graphics::cull_mode_t m_cull_mode;
				graphics::front_face_t m_front_face;
				graphics::blend_mode_t m_blend_mode;

				mutable core::async::mutex_t m_mutex;
				weak_ptr<core::files::ifile_system> m_fs;
				mutable core::async::idispatcher_t m_async_worker;

				astring m_vs_model;
				astring m_ps_model;

				collections::hash_map<string, d3d9_shader_code_t> m_compiled_shaders;

			public:
				d3d9_driver();

			protected: //override
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

				idriver_t driver()const override { return const_cast<d3d9_driver*>(this); }
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

				optional<d3d9_shader_code_t> compile_shader(string, shader_type_t const&);
				optional<d3d9_shader_code_t> compile_shader(effects::shader_info_t const&, shader_type_t const&);

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

				virtual bool init_driver(platform::icore_view_t wnd, long64 adapter_id);

			protected:
				virtual void close_driver();

			public: //intenal
				template<typename T>
				core::async::iasync<T> create_task(function<T(core::async::iasync<T>, d3d9_driver_t)> func)const {
					d3d9_driver_t auto_save = const_cast<d3d9_driver*>(this);
					return m_async_worker->run_async<T>([auto_save, func](core::async::iasync<T> async) {
						return func(async, auto_save);
					});
				}
				inline core::files::ifile_system* get_file_system()const {
					auto fs = m_fs.lock();
					return fs.is_empty() ? core::files::ifile_system::instance().get() : fs.get();
				}
				//inline ID3D11Device2* D3D11Device()const { return m_d3d_device.get(); }
				//inline ID3D11DeviceContext2* D3D11Context()const { return m_d3d_context.get(); }
				//inline IDXGIFactory2* DXGIFactory()const { return m_dxgi_factory.get(); }
				//inline ID3D11SamplerState* D3D11SamplerState(windex i)const { return m_d3d_samplers[i].get(); }
				inline d3d9_frame_buffer_t current_frame_buffer()const { return m_current_frame_buffer.get(); }


				inline castr_t vs_model()const {
					return m_vs_model;
				}
				inline castr_t ps_model()const {
					return m_ps_model;
				}
			};
		}
	}
}

//ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11Buffer);
//ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11SamplerState);
//ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11RenderTargetView);
//ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11ShaderResourceView);

#endif
