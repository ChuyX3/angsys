#pragma once

#include <ang/graphics/angraph.h>
#include "vulkan/VulkanSDK.h"

#if VULKAN_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace vulkan
		{
			class vulkan_driver;
			class vulkan_surface;
			class vulkan_frame_buffer;

			class vulkan_texture;

			typedef object_wrapper<vulkan_driver> vulkan_driver_t;
			typedef object_wrapper<vulkan_surface> vulkan_surface_t;
			typedef object_wrapper<vulkan_frame_buffer> vulkan_frame_buffer_t;

			typedef object_wrapper_ptr<vulkan_driver> vulkan_driver_ptr_t;
			typedef object_wrapper_ptr<vulkan_surface> vulkan_surface_ptr_t;
			typedef object_wrapper_ptr<vulkan_frame_buffer> vulkan_frame_buffer_ptr_t;

			template<typename... Ts>
			void vulkan_debug_print(int level, cstr_t format, Ts... args)
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
}



namespace ang
{
	namespace graphics
	{
		namespace vulkan
		{

			class vulkan_frame_buffer
				: public object
				, public iframe_buffer
			{
				safe_pointer _parent_driver;
				foundation::size<float> _dimentions;
				//collections::vector<com_wrapper<ID3D11RenderTargetView>> d3d_render_target;
				//collections::vector<com_wrapper<ID3D11ShaderResourceView>> d3d_texture2D;
				//com_wrapper<ID3D11DepthStencilView> d3d_depth_stencil;
				//com_wrapper<ID3D11DepthStencilState> d3d_depth_stencil_state;

			public: //internal
				//inline ID3D11RenderTargetView* D3DRenderTargetView(index idx)const { return d3d_render_target[idx].get(); }
				//inline ID3D11ShaderResourceView* D3D11ShaderResourceView(index idx)const { return d3d_texture2D[idx].get(); }
				//inline ID3D11DepthStencilView* D3DDepthStencilView()const { return d3d_depth_stencil.get(); }
				//inline ID3D11DepthStencilState* D3DDepthStencilState()const { return d3d_depth_stencil_state.get(); }

			public:
				vulkan_frame_buffer(vulkan_driver*);

				ANG_DECLARE_INTERFACE();

				bool create(vulkan_surface_t);
				//	bool create(frame_buffer_create_args_t);
				bool close();

			public: //overrides
				foundation::size<float> dimentions()const override;
				uint color_buffer_count()const override;
				bool has_depth_stencil_buffer()const override;

			private:
				virtual~vulkan_frame_buffer();
			};

			class vulkan_surface final
				: public object
				, public isurface
			{
				foundation::size<float> _current_size;
				safe_pointer _parent_driver;
				VkSurfaceKHR vk_surface;
				VkQueue vk_present_queue;
				//com_wrapper<IDXGISwapChain1> dxgi_swap_chain;
				//vulkan_frame_buffer_t d3d_frame_buffer;

			public: //internal
				foundation::size<float> const& surface_size()const { return _current_size; }
				//inline IDXGISwapChain1* DXGISwapChain()const { return dxgi_swap_chain.get(); }

			public:
				vulkan_surface(vulkan_driver*);

				ANG_DECLARE_INTERFACE();

				bool create(platform::icore_view_t view, VkSurfaceKHR vk_surface);
				bool update(platform::icore_view_t view, foundation::size<float> size, foundation::size<float> scale);
				bool close();

			public: //overrides
				void swap_buffers() override;
				iframe_buffer_t frame_buffer()const override;

			private:
				void on_display_size_changed_event(objptr, ang::platform::events::idisplay_info_event_args_t);

			private:
				virtual~vulkan_surface();
			};

			class vulkan_driver final
				: public object
				, public idriver
			{
			private:
				VkInstance _vk_instance;
				VKInternalDevice_t _device;

				graphics::cull_mode_t _cull_mode;
				graphics::front_face_t _front_face;
				graphics::blend_mode_t _blend_mode;

				core::async::mutex_t main_mutex;

				VkSemaphore _rendering_event;

				//	vulkan_index_buffer_t _current_index_buffer;
				//	vulkan_vertex_buffer_t _current_vertex_buffer;

			public:
				vulkan_driver();
				vulkan_driver(platform::icore_view_t view, vulkan_surface_ptr_t);
			
			public: //overrides
				ANG_DECLARE_INTERFACE();

				isurface_t create_surface(platform::icore_view_t)const override;
				effects::ieffect_library_t create_effect_library()const override;
				textures::itexture_loader_t create_texture_loader()const override;
				buffers::ivertex_buffer_t create_vertex_buffer(buffers::buffer_usage_t usage, static_array<reflect::attribute_desc> vertex_desc, wsize vertex_count, static_array<byte> init_data)const override;
				buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage, reflect::var_type_t index_type, wsize index_count, static_array<byte> init_data)const override;

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
				core::async::mutex_t driver_guard()const override;
			private:
				bool init_driver();
				void close_driver();

			public: //intenal
				//inline ID3D11Device2* D3D11Device()const { return d3d_device.get(); }
				//inline safe_thread_wrapper<ID3D11DeviceContext2> D3D11Context()const { return{ d3d_context.get(), main_mutex }; }
				//inline IDXGIFactory2* DXGIFactory()const { return dxgi_factory.get(); }
				//inline vulkan_frame_buffer_t current_frame_buffer()const { return _current_frame_buffer.get(); }

			private:
				virtual~vulkan_driver();
			};
		}
	}
}

#endif