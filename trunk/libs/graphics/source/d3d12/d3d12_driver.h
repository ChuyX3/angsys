#pragma once

#include "graphic_object.h"

#if DIRECTX12_SUPPORT

#include <d3d12.h>
//#include <D3dx12.h>

//#include <d3dcompiler.h>
//#include <d2d1_3.h>
#include <dxgi1_6.h>

#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "dwrite.lib")
//#pragma comment(lib, "d2d1.lib")

#include <coffe/inline/com_wrapper.hpp>

//#if defined WINDOWS_PLATFORM 
//namespace winrt
//{
//	template <typename T>
//	T from_com(::IUnknown* from)
//	{
//		T to{ nullptr };
//
//		winrt::check_hresult(reinterpret_cast<::IUnknown*>(from)
//			->QueryInterface(winrt::guid_of<T>(),
//				reinterpret_cast<void**>(winrt::put_abi(to))));
//		return to;
//	}
//
//}
//#endif

namespace coffe
{
	namespace graphics
	{

		namespace d3d12
		{
			coffe_object(d3d12_driver);
			coffe_object(d3d12_surface);
			coffe_object(d3d12_frame_buffer);
			coffe_object(d3d12_texture);
			coffe_object(d3d12_shaders);

			/*typedef struct _model
			{
				buffers::iindex_buffer_t indices;
				buffers::ivertex_buffer_t vertices;
			}model_t, *model_ptr_t;*/

			template<typename... Ts>
			void d3d12_debug_print(int level, castr_t format, Ts... args)
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

	template<> struct is_inherited_from<graphics::d3d12::d3d12_driver, bean> : true_type {};
	template<> struct is_inherited_from<graphics::d3d12::d3d12_driver, intf> : true_type {};
	
}

//#include "d3d12/d3d12_buffers.h" 
//#include "d3d12/d3d12_shaders.h"
//#include "d3d12/d3d12_textures.h"

namespace coffe
{
	namespace graphics
	{
		namespace d3d12
		{

			class d3d12_surface final
				: public smart<d3d12_surface, system_object, isurface>
			{
				bool m_need_update;
				graphics::size<uint> m_pending_size;
				platform::icore_view_t m_view;
				graphics::size<uint> m_current_size;
				weak_ptr<d3d12_driver> m_parent_driver;
				com_wrapper<IDXGISwapChain1> m_dxgi_swap_chain;
				stack_array<com_wrapper<ID3D12Resource>, 3> m_d3d_buffers;
				//d3d11_frame_buffer_t m_d3d_frame_buffer;

			public: //internal
				graphics::size<uint> const& surface_size()const { return m_current_size; }
				inline IDXGISwapChain1* DXGISwapChain()const { return m_dxgi_swap_chain.get(); }

			public:
				d3d12_surface(d3d12_driver*);

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
				void on_display_size_changed_event(objptr, coffe::platform::events::idisplay_info_event_args_t);

			};

			class d3d12_driver
				: public smart<d3d12_driver, system_object, idriver, ifactory>
			{
			private:
				ulong m_max_version;
				ulong m_min_version;
				D3D_FEATURE_LEVEL m_feature_level;
				com_wrapper<IDXGIAdapter4> m_dxgi_adapter;
				com_wrapper<IDXGIFactory4> m_dxgi_factory;
				com_wrapper<ID3D12Device2> m_d3d_device;
				com_wrapper<ID3D12CommandQueue> m_d3d_queue;
				
				stack_array<com_wrapper<ID3D12DescriptorHeap>, 4> m_d3d_desc_heaps;

				//d3d12_frame_buffer_t m_current_frame_buffer;
				//d3d12_shaders_t m_current_technique;
				//primitive_t m_primitive;

				graphics::box<float> m_viewport;
				graphics::cull_mode_t m_cull_mode;
				graphics::front_face_t m_front_face;
				graphics::blend_mode_t m_blend_mode;

				mutable core::async::mutex_t m_mutex;
				weak_ptr<core::files::ifile_system> m_fs;
				mutable core::async::thread_t m_async_worker;

			public:
				d3d12_driver();

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

				idriver_t driver()const override { return const_cast<d3d12_driver*>(this); }
				void set_file_system(core::files::ifile_system_t)override;
				optional<buffers::ivertex_buffer> create_vertex_buffer(buffers::buffer_usage_t usage, vector<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t init_data, string sid = null)const override;
				optional<buffers::iindex_buffer> create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				optional<textures::itexture> create_texture(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				optional<textures::itexture> create_texture(unknown_t tex_handle, string sid = null)const override;
				optional<textures::itexture> load_texture(text::string file, textures::tex_type_t, string sid = null)const override;
				optional<textures::itexture> load_texture(array_view<text::string> files,textures::tex_type_t, string sid = null)const override;
				optional<iframe_buffer> create_frame_buffer(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, size<uint> dimentions, bool is_stereo = false, string sid = null)const override;
				optional<iframe_buffer> create_frame_buffer(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil_format = null, string sid = null)const override;
				optional<effects::ishaders> compile_shaders(wstring vertex_shader, wstring pixel_shader, string sid = null)const override;
				optional<effects::ishaders> compile_shaders(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid = null)const override;

				core::async::iasync_optional<buffers::ivertex_buffer> create_vertex_buffer_async(buffers::buffer_usage_t usage, vector<reflect::attribute_desc> vertex_desc, wsize vertex_count, ibuffer_t init_data, string sid = null)const override;
				core::async::iasync_optional<buffers::iindex_buffer> create_index_buffer_async(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, ibuffer_t init_data, string sid = null)const override;
				core::async::iasync_optional<textures::itexture> create_texture_async(textures::tex_type_t type, textures::tex_format_t color_format, buffers::buffer_usage_t usage, buffers::buffer_bind_flag_t flags, size3d<uint> dimentions, string sid = null)const override;
				core::async::iasync_optional<textures::itexture> create_texture_async(unknown_t tex_handle, string sid = null)const override;
				core::async::iasync_optional<textures::itexture> load_texture_async(text::string file, textures::tex_type_t, string sid = null)const override;
				core::async::iasync_optional<textures::itexture> load_texture_async(array_view<text::string> files, textures::tex_type_t, string sid = null)const override;
				core::async::iasync_optional<iframe_buffer> create_frame_buffer_async(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, size<uint> dimentions, bool is_stereo = false, string sid = null)const override;
				core::async::iasync_optional<iframe_buffer> create_frame_buffer_async(array_view<textures::itexture_t> color_tex, textures::itexture_t depth_stencil_format = null, string sid = null)const override;
				core::async::iasync_optional<effects::ishaders> compile_shaders_async(wstring vertex_shader, wstring pixel_shader, string sid = null)const override;
				core::async::iasync_optional<effects::ishaders> compile_shaders_async(effects::shader_info_t const& vertex_shader, effects::shader_info_t const& pixel_shader, string sid = null)const override;


				void d3d12_driver::viewport(box<float> vp) override;
				box<float> d3d12_driver::viewport()const override;

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
				void create_descriptor_heap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32 numDescriptors);

			protected:
				virtual void close_driver();

			public: //intenal
				template<typename T>
				core::async::iasync<T> create_task(function<T(core::async::iasync<T>, d3d12_driver_t)> func)const {
					d3d12_driver_t auto_save = const_cast<d3d12_driver*>(this);
					return m_async_worker->run_async<T>([auto_save, func](core::async::iasync<T> async) {
						return func(async, auto_save);
					});
				}
				inline core::files::ifile_system* get_file_system()const {
					auto fs = m_fs.lock();
					return fs.is_empty() ? core::files::ifile_system::instance().get() : fs.get();
				}
				inline ID3D12Device2* D3D12Device()const { return m_d3d_device.get(); }
				inline ID3D12CommandQueue* D3D12Queue()const { return m_d3d_queue.get(); }
				inline IDXGIFactory4* DXGIFactory()const { return m_dxgi_factory.get(); }
				inline ID3D12DescriptorHeap* D3D12DescriptorHeap(windex i) { return m_d3d_desc_heaps[i]; }
				inline bool AllowTearing()const {
					com_wrapper<IDXGIFactory4> factory4 = m_dxgi_factory.get();
					com_wrapper<IDXGIFactory5> factory5;
					BOOL allowTearing = FALSE;
					if (SUCCEEDED(factory4.as(&factory5)))
					{		
						if (FAILED(factory5->CheckFeatureSupport(
							DXGI_FEATURE_PRESENT_ALLOW_TEARING,
							&allowTearing, sizeof(allowTearing))))
						{
							allowTearing = FALSE;
						}
					}
					return allowTearing ? true : false;
				}
				//inline d3d12_frame_buffer_t current_frame_buffer()const { return m_current_frame_buffer.get(); }
			};
		}
	}
}


//COFFE_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11Buffer);
//COFFE_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11SamplerState);
//COFFE_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11RenderTargetView);
//COFFE_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(ID3D11ShaderResourceView);

#endif
