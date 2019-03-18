#pragma once

#include <ang/graphics/graphics.h>
#include <ang/collections/hash_map.h>

#if DIRECTX11_SUPPORT

#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <d2d1_3.h>
#include <dwrite_3.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include <ang/inline/com_wrapper.hpp>

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			ang_object(d3d11_driver);
			ang_object(d3d11_surface);
			ang_object(d3d11_frame_buffer);
			ang_object(d3d11_texture);
			ang_object(d3d11_shaders);

			typedef struct _model
			{
				buffers::iindex_buffer_t indices;
				buffers::ivertex_buffer_t vertices;
			}model_t, *model_ptr_t;

			template<typename... Ts>
			void d3d11_debug_print(int level, cstr_t format, Ts... args)
			{
				switch (level)
				{
				case 2:
					ang_debug_output_error(format, args...);
					break;
				case 1:
					ang_debug_output_warning(format, args...);
					break;
				default: //0
					ang_debug_output_info(format, args...);
					break;
				}
			}
		}
	}

	template<> struct is_inherited_from<graphics::d3d11::d3d11_driver, object> : true_type {};
	template<> struct is_inherited_from<graphics::d3d11::d3d11_driver, intf> : true_type {};
	
}

#include "d3d11/d3d11_buffers.h" 
#include "d3d11/d3d11_shaders.h"
#include "d3d11/d3d11_textures.h"

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{

			class d3d11_frame_buffer
				: public smart<d3d11_frame_buffer, iframe_buffer, resources::iresource>
			{
				weak_ptr<d3d11_driver> m_parent_driver;
				string m_resource_sid;
				graphics::size<uint> m_dimentions;
				collections::vector<com_wrapper<ID3D11RenderTargetView>> m_d3d_render_targets;
				collections::vector<d3d11_texture_t> m_d3d_color_textures;
				d3d11_texture_t m_d3d_depth_stencil_texture;
				com_wrapper<ID3D11DepthStencilView> m_d3d_depth_stencil;
				com_wrapper<ID3D11DepthStencilState> m_d3d_depth_stencil_state;

			public: //internal
				inline ID3D11RenderTargetView* D3DRenderTargetView(index idx)const { return m_d3d_render_targets.is_empty() ? null : m_d3d_render_targets[idx].get(); }
				inline ID3D11ShaderResourceView* D3DShaderResourceView(index idx)const { return m_d3d_color_textures.is_empty() ? null : m_d3d_color_textures[idx]->D3D11ShaderResourceView(); }
				inline ID3D11DepthStencilView* D3DDepthStencilView()const { return m_d3d_depth_stencil.get(); }
				inline ID3D11DepthStencilState* D3DDepthStencilState()const { return m_d3d_depth_stencil_state.get(); }

			public:
				d3d11_frame_buffer(d3d11_driver*);

				ANG_DECLARE_INTERFACE();
				void clear()override;
				bool create(d3d11_surface_t);
				bool create(array_view<d3d11_texture_t> textures, d3d11_texture_t depth_stencil, string sid = null);
				bool create(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, string sid = null);
				bool update(d3d11_texture_t resource);
				bool close();

			public: //overrides
				resources::iresource_t resource()const override;
				graphics::size<uint> dimentions()const override;
				uint color_buffer_count()const override;
				textures::itexture_t color_buffer(index)const;
				textures::itexture_t depth_stencil_buffer()const override;
				bool try_resize(size<uint>) override;

				resources::resource_type_t resource_type()const override;
				string resource_sid()const override;
				void resource_sid(cstr_t) override;
				effects::ieffect_t to_effect() override;
				effects::ishaders_t to_shaders() override;
				textures::itexture_t to_texture() override;
				iframe_buffer_t to_frame_buffer() override;
				buffers::iindex_buffer_t to_index_buffer() override;
				buffers::ivertex_buffer_t to_vertex_buffer() override;

			private:
				virtual~d3d11_frame_buffer();
			};

			class d3d11_surface final
				: public smart<d3d11_surface, isurface>
			{
				bool m_need_update;
				graphics::size<uint> m_pending_size;
				platform::icore_view_t m_view;
				graphics::size<uint> m_current_size;
				weak_ptr<d3d11_driver> m_parent_driver;
				com_wrapper<IDXGISwapChain1> m_dxgi_swap_chain;
				d3d11_frame_buffer_t m_d3d_frame_buffer;

			public: //internal
				graphics::size<uint> const& surface_size()const { return m_current_size; }
				inline IDXGISwapChain1* DXGISwapChain()const { return m_dxgi_swap_chain.get(); }

			public:
				d3d11_surface(d3d11_driver*);

				ANG_DECLARE_INTERFACE();

				bool create(platform::icore_view_t);
				bool update(platform::icore_view_t, graphics::size<uint> size);
				bool close();

			public: //overrides
				void update() override;
				void swap_buffers(bool syncronize) override;
				iframe_buffer_t frame_buffer()const override;
				collections::ienum_ptr<collections::tuple<scenes::icamera_t, iframe_buffer_t>> cameras()const override {
					return null;
				}

			private:
				void on_display_size_changed_event(objptr, ang::platform::events::idisplay_info_event_args_t);

			private:
				virtual~d3d11_surface();
			};

			class d3d11_driver
				: public smart<d3d11_driver, idriver, ifactory>
			{
			private:		
				D3D_FEATURE_LEVEL m_feature_level;
				com_wrapper<IDXGIAdapter3> m_dxgi_adapter;
				com_wrapper<IDXGIFactory2> m_dxgi_factory;
				com_wrapper<ID3D11Device2> m_d3d_device;
				com_wrapper<ID3D11DeviceContext2> m_d3d_context;
				com_wrapper<ID3D11BlendState> m_d3d_blend_state;

				d3d11_frame_buffer_t m_current_frame_buffer;
				d3d11_shaders_t m_current_technique;
				primitive_t m_primitive;

				graphics::box<float> m_viewport;
				graphics::cull_mode_t m_cull_mode;
				graphics::front_face_t m_front_face;
				graphics::blend_mode_t m_blend_mode;

				core::async::mutex_ptr_t m_mutex;
				core::files::ifile_system_t m_fs;
				mutable core::async::thread_t m_async_worker;

			public:
				d3d11_driver();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				void clear()override;
				graph_driver_type_t graph_driver_type()const override;
				isurface_t create_surface(platform::icore_view_t)const override;
				ifactory_t get_factory()const override;

				idriver_t driver()const override { return const_cast<d3d11_driver*>(this); }
				void set_file_system(core::files::ifile_system_t)override;
				buffers::ivertex_buffer_t create_vertex_buffer(buffers::buffer_usage_t usage, vector<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t init_data, string sid = null)const override;
				buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				textures::itexture_t create_texture(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				textures::itexture_t create_texture(unknown_t tex_handle, string sid = null)const override;
				textures::itexture_t load_texture(text::string file, textures::tex_type_t, string sid = null)const override;
				textures::itexture_t load_texture(array_view<text::string> files,textures::tex_type_t, string sid = null)const override;
				iframe_buffer_t create_frame_buffer(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, size<uint> dimentions, string sid = null)const override;
				iframe_buffer_t create_frame_buffer(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil_format = null, string sid = null)const override;
				effects::ishaders_t compile_shaders(wstring vertex_shader, wstring pixel_shader, string sid = null, string_ptr_t log = null)const override;
				effects::ishaders_t compile_shaders(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid = null, string_ptr_t log = null)const override;

				core::async::iasync<buffers::ivertex_buffer_t> create_vertex_buffer_async(buffers::buffer_usage_t usage, vector<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t init_data, string sid = null)const override;
				core::async::iasync<buffers::iindex_buffer_t> create_index_buffer_async(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				core::async::iasync<textures::itexture_t> create_texture_async(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				core::async::iasync<textures::itexture_t> create_texture_async(unknown_t tex_handle, string sid = null)const override;
				core::async::iasync<textures::itexture_t> load_texture_async(text::string file, textures::tex_type_t, string sid = null)const override;
				core::async::iasync<textures::itexture_t> load_texture_async(array_view<text::string> files, textures::tex_type_t, string sid = null)const override;
				core::async::iasync<iframe_buffer_t> create_frame_buffer_async(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, size<uint> dimentions, string sid = null)const override;
				core::async::iasync<iframe_buffer_t> create_frame_buffer_async(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil_format = null, string sid = null)const override;
				core::async::iasync<effects::ishaders_t> compile_shaders_async(wstring vertex_shader, wstring pixel_shader, string sid = null)const override;
				core::async::iasync<effects::ishaders_t> compile_shaders_async(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid = null)const override;


				void d3d11_driver::viewport(box<float> vp) override;
				box<float> d3d11_driver::viewport()const override;

				void cull_mode(cull_mode_t) override;
				cull_mode_t cull_mode()const override;

				void front_face(front_face_t) override;
				front_face_t front_face()const override;

				void blend_mode(blend_mode_t) override;
				blend_mode_t blend_mode()const override;

				void bind_frame_buffer(iframe_buffer_t) override;
				void bind_shaders(effects::ishaders_t) override;
				void bind_texture(textures::itexture_t, index idx) override;
				void bind_index_buffer(buffers::iindex_buffer_t) override;
				void bind_vertex_buffer(buffers::ivertex_buffer_t) override;

				void clear(maths::float4 const&) override;
				void clear(color_t) override;
				void draw(uint count, primitive_t) override;
				void draw_indexed(uint count, primitive_t) override;
				core::async::mutex_ptr_t driver_guard()const override;
				core::async::idispatcher_t dispatcher()const override;

				virtual bool init_driver(platform::icore_view_t wnd, long64 adapter_id);

			protected:
				virtual void close_driver();

			public: //intenal
				template<typename T>
				core::async::iasync<T> create_task(function<T(core::async::iasync<T>, d3d11_driver_t)> func)const {
					d3d11_driver_t auto_save = const_cast<d3d11_driver*>(this);
					return m_async_worker->run_async<T>([auto_save, func](core::async::iasync<T> async) {
						return func.get()->invoke(async, auto_save);
					});
				}
				inline core::files::ifile_system* get_file_system()const { return m_fs.is_empty() ? core::files::ifile_system::instance().get() : m_fs.get(); }
				inline ID3D11Device2* D3D11Device()const { return m_d3d_device.get(); }
				inline ID3D11DeviceContext2* D3D11Context()const { return m_d3d_context.get(); }
				//	inline safe_thread_wrapper<ID3D11DeviceContext2> D3D11Context()const { return{ d3d_context.get(), main_mutex }; }
				inline IDXGIFactory2* DXGIFactory()const { return m_dxgi_factory.get(); }
				inline d3d11_frame_buffer_t current_frame_buffer()const { return m_current_frame_buffer.get(); }

			protected:
				virtual~d3d11_driver();
			};
		}
	}
}


#if DIRECTX11_VRX_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			ang_object(holographic_driver);
			ang_object(holographic_frame);
			ang_object(holographic_camera);
		}
	}

	template<> struct is_inherited_from<graphics::d3d11::holographic_driver, object> : true_type {};
	template<> struct is_inherited_from<graphics::d3d11::holographic_driver, intf> : true_type {};

	namespace graphics
	{
		namespace d3d11
		{
			class holographic_camera
				: public smart<holographic_camera, icamera>
			{
				weak_ptr<d3d11_driver> m_parent_driver;
				d3d11_frame_buffer_t m_frame_buffer;
				Windows::Graphics::Holographic::HolographicCamera^ m_holographic_camera;
	

			public:
				holographic_camera(d3d11_driver* driver);

				ANG_DECLARE_INTERFACE();
				void clear()override;
				void viewport(box<float>) override {

				}
				box<float> viewport()const override;
				maths::float4x4 view_projection_matrix()const override;

				bool create(Windows::Graphics::Holographic::HolographicCamera^ camera);
			private:
				virtual~holographic_camera();
			};

			class holographic_frame
				: public smart<holographic_frame, isurface>
			{
				weak_ptr<d3d11_driver> m_parent_driver;
				Windows::Graphics::Holographic::HolographicFrame^ m_holographic_frame;
				collections::hash_map<uint, holographic_camera_t> m_camera_map;
				collections::vector<collections::tuple<icamera_t, iframe_buffer_t>> m_cameras;

				Windows::Foundation::EventRegistrationToken m_camera_added_token;
				Windows::Foundation::EventRegistrationToken m_camera_removed_token;

			public:
				holographic_frame(d3d11_driver*);

				ANG_DECLARE_INTERFACE();
			
				bool create(Windows::Graphics::Holographic::HolographicSpace^);
				bool close();

			public: //overrides
				void update() override;
				void swap_buffers(bool syncronize) override;
				iframe_buffer_t frame_buffer()const override { return null; }
				collections::ienum_ptr<collections::tuple<icamera_t, iframe_buffer_t>> cameras()const override;

			public:
				void on_camera_added(Windows::Graphics::Holographic::HolographicSpace ^, Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs ^);
				void on_camera_removed(Windows::Graphics::Holographic::HolographicSpace ^, Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs ^);

			private:
				virtual~holographic_frame();
			};

			class holographic_driver
				: public smart<holographic_driver, d3d11_driver>
			{
				Windows::Graphics::Holographic::HolographicSpace^ m_holographic_space;
				Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice^ m_d3d_interop_device;

			public:
				holographic_driver();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				graph_driver_type_t graph_driver_type()const override;
				isurface_t create_surface(platform::icore_view_t)const override;

				bool init_driver(platform::icore_view_t wnd, long64 adapter_id)override;

			protected:	
				void close_driver()override;

			public:
				inline Windows::Graphics::Holographic::HolographicSpace^ HolographicSpace()const {
					return m_holographic_space;
				}

			private:
				virtual~holographic_driver();
			};
		}
	}
}

#endif //DIRECTX11_HGX_SUPPORT


ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11Buffer);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11SamplerState);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11RenderTargetView);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11ShaderResourceView);

#endif
