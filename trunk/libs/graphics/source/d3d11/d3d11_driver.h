#pragma once

#include "graphic_object.h"

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


#if DIRECTX11_VRX_SUPPORT


#include <wrl\client.h>

#include <ppl.h>
#include <winrt\Windows.ApplicationModel.Activation.h>
#include <winrt\Windows.ApplicationModel.Core.h>
#include <winrt\Windows.Devices.h>
#include <winrt\Windows.Devices.haptics.h>
#include <winrt\Windows.Foundation.h>
#include <winrt\Windows.Foundation.Collections.h>
#include <winrt\Windows.Gaming.Input.h>
#include <winrt\Windows.Graphics.Display.h>
#include <winrt\Windows.Graphics.Holographic.h>
#include <winrt\Windows.Perception.People.h>
#include <winrt\Windows.Perception.Spatial.h>
#include <winrt\Windows.Storage.h>
#include <winrt\Windows.Storage.Streams.h>
#include <winrt\Windows.UI.Core.h>
#include <winrt\Windows.UI.Input.Spatial.h>

#include <windows.graphics.holographic.h>
#include <Windows.Graphics.Directx.Direct3D11.Interop.h>
#include <windows.graphics.directx.direct3d11.h>

#include <HolographicSpaceInterop.h>

#endif//DIRECTX11_VRX_SUPPORT


#if defined WINDOWS_PLATFORM 
namespace winrt
{
	template <typename T>
	T from_com(::IUnknown* from)
	{
		T to{ nullptr };

		winrt::check_hresult(reinterpret_cast<::IUnknown*>(from)
			->QueryInterface(winrt::guid_of<T>(),
				reinterpret_cast<void**>(winrt::put_abi(to))));
		return to;
	}

}
#endif

namespace ang
{
	namespace graphics
	{

		namespace d3d11
		{
			ang_declare_object(d3d11_driver);
			ang_declare_object(d3d11_surface);
			ang_declare_object(d3d11_frame_buffer);
			ang_declare_object(d3d11_texture);
			ang_declare_object(d3d11_shaders);
			ang_declare_object(d3d11_shader_code);

#if DIRECTX11_VRX_SUPPORT
			ang_declare_object(holographic_driver);
			ang_declare_object(holographic_scene);
			ang_declare_object(holographic_camera);
#endif

			typedef struct _model
			{
				buffers::iindex_buffer_t indices;
				buffers::ivertex_buffer_t vertices;
			}model_t, *model_ptr_t;

			

			template<typename... Ts>
			void d3d11_debug_print(int level, castr_t format, Ts... args)
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
	template<> struct is_base_of<ang::object, ang::graphics::d3d11::d3d11_driver> : true_type { };
	template<> struct is_base_of<ang::iintf, ang::graphics::d3d11::d3d11_driver> : true_type { };
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
				: public graphic<d3d11_frame_buffer
				, iid("ang::graphics::d3d11::d3d11_frame_buffer")
				, iframe_buffer
				, resources::iresource>
			{
				weak_ptr<d3d11_driver> m_parent_driver;
				string m_resource_sid;
				bool m_is_stereo;
				graphics::size<uint> m_dimentions;
				collections::vector<com_wrapper<ID3D11RenderTargetView>> m_d3d_render_targets;
				collections::vector<d3d11_texture_t> m_d3d_color_textures;
				d3d11_texture_t m_d3d_depth_stencil_texture;
				com_wrapper<ID3D11DepthStencilView> m_d3d_depth_stencil;
				com_wrapper<ID3D11DepthStencilState> m_d3d_depth_stencil_state;

			public: //internal
				inline ID3D11RenderTargetView* D3DRenderTargetView(windex idx)const { return m_d3d_render_targets.is_empty() ? null : m_d3d_render_targets[idx].get(); }
				inline ID3D11ShaderResourceView* D3DShaderResourceView(windex idx)const { return m_d3d_color_textures.is_empty() ? null : m_d3d_color_textures[idx]->D3D11ShaderResourceView(); }
				inline ID3D11DepthStencilView* D3DDepthStencilView()const { return m_d3d_depth_stencil.get(); }
				inline ID3D11DepthStencilState* D3DDepthStencilState()const { return m_d3d_depth_stencil_state.get(); }

			public:
				d3d11_frame_buffer(d3d11_driver*);

				bool create(d3d11_surface_t);
				bool create(array_view<d3d11_texture_t> textures, d3d11_texture_t depth_stencil, string sid = null);
				bool create(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<uint> dimentions, bool stereo = false, string sid = null);
				bool update(d3d11_texture_t resource);
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

			class d3d11_surface final
				: public graphic<d3d11_surface
				, iid("ang::graphics::d3d11::d3d11_surface")
				, isurface>
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

			class d3d11_driver
				: public graphic<d3d11_driver
				, iid("ang::graphics::d3d11::d3d11_driver")
				, idriver
				, ifactory>
			{
			private:
				ulong m_max_version;
				ulong m_min_version;
				D3D_FEATURE_LEVEL m_feature_level;
				com_wrapper<IDXGIAdapter3> m_dxgi_adapter;
				com_wrapper<IDXGIFactory2> m_dxgi_factory;
				com_wrapper<ID3D11Device2> m_d3d_device;
				com_wrapper<ID3D11DeviceContext2> m_d3d_context;
				com_wrapper<ID3D11BlendState> m_d3d_blend_state;
				collections::stack_array<com_wrapper<ID3D11SamplerState>, 3> m_d3d_samplers;

				d3d11_frame_buffer_t m_current_frame_buffer;
				d3d11_shaders_t m_current_technique;
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

				collections::hash_map<string, d3d11_shader_code_t> m_compiled_shaders;

			public:
				d3d11_driver();

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

				idriver_t driver()const override { return const_cast<d3d11_driver*>(this); }
				void set_file_system(core::files::ifile_system_t)override;
				optional<buffers::ivertex_buffer> create_vertex_buffer(buffers::buffer_usage_t usage, array_view<reflect::attribute_desc_t> vertex_desc, wsize vertex_count, ibuffer_t vertex_data, string sid = null)const override;
				optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				optional<textures::itexture> create_texture(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				optional<textures::itexture> create_texture(unknown_t tex_handle, string sid = null)const override;
				optional<textures::itexture> load_texture(text::string file, textures::tex_type_t, string sid = null)const override;
				optional<textures::itexture> load_texture(array_view<text::string> files,textures::tex_type_t, string sid = null)const override;
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

				optional<d3d11_shader_code_t> compile_shader(string, shader_type_t const&);
				optional<d3d11_shader_code_t> compile_shader(effects::shader_info_t const&, shader_type_t const&);

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
				core::async::iasync<T> create_task(function<T(core::async::iasync<T>, d3d11_driver_t)> func)const {
					d3d11_driver_t auto_save = const_cast<d3d11_driver*>(this);
					return m_async_worker->run_async<T>([auto_save, func](core::async::iasync<T> async) {
						return func(async, auto_save);
					});
				}
				inline core::files::ifile_system* get_file_system()const {
					auto fs = m_fs.lock();
					return fs.is_empty() ? core::files::ifile_system::instance().get() : fs.get();
				}
				inline ID3D11Device2* D3D11Device()const { return m_d3d_device.get(); }
				inline ID3D11DeviceContext2* D3D11Context()const { return m_d3d_context.get(); }
				//	inline safe_thread_wrapper<ID3D11DeviceContext2> D3D11Context()const { return{ d3d_context.get(), main_mutex }; }
				inline IDXGIFactory2* DXGIFactory()const { return m_dxgi_factory.get(); }				
				inline ID3D11SamplerState* D3D11SamplerState(windex i)const { return m_d3d_samplers[i].get(); }
				inline d3d11_frame_buffer_t current_frame_buffer()const { return m_current_frame_buffer.get(); }
			

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


#if DIRECTX11_VRX_SUPPORT

namespace std {
	template<> struct is_base_of<ang::object, ang::graphics::d3d11::holographic_driver> : true_type { };
	template<> struct is_base_of<ang::iintf, ang::graphics::d3d11::holographic_driver> : true_type { };
}

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			class holographic_camera
				: public graphic<holographic_camera
				, iid("ang::graphics::d3d11::holographic_camera")
				, scenes::itransform>
			{
				maths::float4x4 m_view_matrix[2];
				maths::float4x4 m_proj_matrix[2];
				box<float> m_viewport;
				weak_ptr<holographic_driver> m_parent_driver;
				d3d11_frame_buffer_t m_frame_buffer;
				d3d11_texture_t m_back_buffer;
				bool m_is_stereo;
				winrt::Windows::Graphics::Holographic::HolographicCamera m_holographic_camera;
	
			public:
				holographic_camera(holographic_driver* driver, winrt::Windows::Graphics::Holographic::HolographicCamera const& camera);

				void dispose()override;
				bool is_stereo()const override { return m_is_stereo; }
				void viewport(box<float>) override { }
				box<float> viewport()const override;
				maths::float4x4 view_matrix(windex idx)const override;
				maths::float4x4 projection_matrix(windex idx)const override;

				bool update_resources(winrt::Windows::Graphics::Holographic::HolographicCameraRenderingParameters const&,
					winrt::Windows::Graphics::Holographic::HolographicCameraPose const&);

				bool update_transform(winrt::Windows::Perception::Spatial::SpatialCoordinateSystem const&,
					winrt::Windows::Graphics::Holographic::HolographicCameraPose const&);
	
				d3d11_frame_buffer_t frame_buffer()const;
			};

			class holographic_scene
				: public graphic<holographic_scene
				, iid("ang::graphics::d3d11::holographic_scene")
				, isurface>
			{
				weak_ptr<holographic_driver> m_parent_driver;

				mutable core::async::mutex_t m_mutex;

				winrt::Windows::Perception::Spatial::SpatialStageFrameOfReference m_stage_frame_of_reference;
				winrt::Windows::Perception::Spatial::SpatialStationaryFrameOfReference m_stationary_frame_of_reference;
				winrt::Windows::Graphics::Holographic::HolographicSpace m_holographic_space;
				winrt::Windows::Graphics::Holographic::HolographicFrame m_current_frame;
				collections::hash_map<uint, holographic_camera_t> m_camera_map;
				//collections::vector<collections::tuple<scenes::icamera_t, iframe_buffer_t>> m_cameras;

				winrt::event_token m_camera_added_token;
				winrt::event_token m_camera_removed_token;
				winrt::event_token m_spatial_stage_current_changed;


			public:
				holographic_scene(holographic_driver*);
			
				void dispose()override;

				bool create(winrt::Windows::Graphics::Holographic::HolographicSpace const&);
				bool close();

			public: //overrides
				collections::ienum_ptr<collections::tuple<scenes::itransform_t, iframe_buffer_t>> update() override;
				bool swap_buffers(bool syncronize) override;
				iframe_buffer_t frame_buffer()const override { return null; }
				winrt::Windows::Perception::Spatial::SpatialCoordinateSystem world_coordinate_system() const;

			public:
				void on_camera_added(winrt::Windows::Graphics::Holographic::HolographicSpace const&, winrt::Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs const&);
				void on_camera_removed(winrt::Windows::Graphics::Holographic::HolographicSpace const&, winrt::Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs const&);
				void on_current_stage_changed(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::Foundation::IInspectable const&);
			};

			class holographic_driver
				: public implement<holographic_driver
				, iid("ang::graphics::d3d11::holographic_driver")
				, d3d11_driver>
			{
				bool m_vr_support;
				winrt::Windows::Graphics::Holographic::HolographicSpace m_holographic_space;
				winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice m_d3d_interop_device;

			public:
				holographic_driver();

			public: //overrides
				graph_driver_type_t graph_driver_type()const override;
				bool vr_extension_support()const override;
				isurface_t create_surface(platform::icore_view_t)const override;
				bool init_driver(platform::icore_view_t wnd, long64 adapter_id)override;

			protected:	
				void close_driver()override;

			public:
				inline winrt::Windows::Graphics::Holographic::HolographicSpace const& holographic_space()const {
					return m_holographic_space;
				}

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
