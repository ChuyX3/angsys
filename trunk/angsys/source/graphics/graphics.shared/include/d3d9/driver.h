#pragma once

#include <ang/graphics/graphics.h>

#if DIRECTX9_SUPPORT
//ANG_OBJECT_WRAPPER_DECLARATION

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib")

#include <ang/inline/com_wrapper.hpp>

namespace ang
{
	namespace graphics
	{
		namespace d3d9
		{
			ang_object(d3d9_driver);
			ang_object(d3d9_surface);
			ang_object(d3d9_frame_buffer);

			ang_object(d3d9_texture);

			template<typename... Ts>
			void d3d9_debug_print(int level, cstr_t format, Ts... args)
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

//#include "d3d9/buffers.h" 
//#include "d3d9/shaders.h"
//#include "d3d9/textures.h"

namespace ang
{
	namespace graphics
	{
		namespace d3d9
		{
			/*
			class d3d9_frame_buffer
				: public object
				, public iframe_buffer
			{
				weak_ptr<d3d9_driver> _parent_driver;
				graphics::size<float> _dimentions;
				collections::vector<com_wrapper<ID3D11RenderTargetView>> d3d_render_target;
				collections::vector<d3d9_texture_t> d3d_texture2D;
				com_wrapper<ID3D11DepthStencilView> d3d_depth_stencil;
				com_wrapper<ID3D11DepthStencilState> d3d_depth_stencil_state;

			public: //internal
				inline ID3D11RenderTargetView* D3DRenderTargetView(index idx)const { return d3d_render_target.is_empty() ? null : d3d_render_target[idx].get(); }
				inline ID3D11ShaderResourceView* D3D11ShaderResourceView(index idx)const { return d3d_texture2D.is_empty()? null : d3d_texture2D[idx]->D3D11ShaderResourceView(); }
				inline ID3D11DepthStencilView* D3DDepthStencilView()const { return d3d_depth_stencil.get(); }
				inline ID3D11DepthStencilState* D3DDepthStencilState()const { return d3d_depth_stencil_state.get(); }

			public:
				d3d9_frame_buffer(d3d9_driver*);

				ANG_DECLARE_INTERFACE();

				bool create(d3d9_surface_t);
				bool create(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<float> dimentions);
				bool close();

			public: //overrides
				graphics::size<float> dimentions()const override;
				uint color_buffer_count()const override;
				textures::itexture_t color_buffer(index)const;
				bool has_depth_stencil_buffer()const override;

			private:
				virtual~d3d9_frame_buffer();
			};

			class d3d9_surface final
				: public object
				, public isurface
			{
				graphics::size<float> _current_size;
				weak_ptr<d3d9_driver> _parent_driver;
				com_wrapper<IDXGISwapChain1> dxgi_swap_chain;
				d3d9_frame_buffer_t d3d_frame_buffer;

			public: //internal
				graphics::size<float> const& surface_size()const { return _current_size; }
				inline IDXGISwapChain1* DXGISwapChain()const { return dxgi_swap_chain.get(); }

			public:
				d3d9_surface(d3d9_driver*);

				ANG_DECLARE_INTERFACE();

				bool create(platform::icore_view_t);
				bool update(platform::icore_view_t, graphics::size<float> size, graphics::size<float> scale);
				bool close();

			public: //overrides
				void swap_buffers() override;
				iframe_buffer_t frame_buffer()const override;

			private:
				void on_display_size_changed_event(objptr, ang::platform::events::idisplay_info_event_args_t);

			private:
				virtual~d3d9_surface();
			};
			*/


			class d3d9_driver final
				: public object
				, public idriver
			{
				
				com_wrapper<IDirect3DDevice9> m_device;
				
				primitive_t m_primitive;
				graphics::cull_mode_t m_cull_mode;
				graphics::front_face_t m_front_face;
				graphics::blend_mode_t m_blend_mode;
				core::async::mutex_ptr_t main_mutex;

			public:
				d3d9_driver();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				graph_driver_type_t graph_driver_type()const override;
				isurface_t create_surface(platform::icore_view_t)const override;
				effects::ieffect_library_t create_effect_library()const override;
				textures::itexture_loader_t create_texture_loader()const override;
				buffers::ivertex_buffer_t create_vertex_buffer(buffers::buffer_usage_t usage, array_view<reflect::attribute_desc> vertex_desc, wsize vertex_count, array_view<byte> init_data)const override;
				buffers::iindex_buffer_t create_index_buffer(buffers::buffer_usage_t usage,	reflect::var_type_t index_type,	wsize index_count, array_view<byte> init_data)const override;
				iframe_buffer_t create_frame_buffer(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<float> dimentions)const override;

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

			private:
				bool init_driver();
				void close_driver();

			private:
				virtual~d3d9_driver();
			};
		}
	}
}


#endif
