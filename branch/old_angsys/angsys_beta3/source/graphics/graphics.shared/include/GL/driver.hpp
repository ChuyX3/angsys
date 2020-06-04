#pragma once

#include <ang/graphics/angraph.hpp>

#if OPENGL_SUPPORT
//ANG_OBJECT_WRAPPER_DECLARATION
namespace ang
{
	namespace graphics
	{
		namespace gl
		{
			class gl_driver;
			class gl_surface;

			class gl_frame_buffer;

			class gl_texture;

			typedef object_wrapper<gl_driver> gl_driver_t;
			typedef object_wrapper<gl_surface> gl_surface_t;
			typedef object_wrapper<gl_frame_buffer> gl_frame_buffer_t;

			template<typename... Ts>
			void gl_debug_print(int level, cstr_t format, Ts... args)
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

	ANG_OBJECT_WRAPPER_DECLARATION(, graphics::gl::gl_driver);
	ANG_OBJECT_WRAPPER_DECLARATION(, graphics::gl::gl_surface);
	ANG_OBJECT_WRAPPER_DECLARATION(, graphics::gl::gl_frame_buffer);

	template<> class weak_ptr<graphics::gl::gl_driver> : public safe_pointer
	{
	public:
		weak_ptr() : safe_pointer() {}
		weak_ptr(weak_ptr&& other) : safe_pointer((safe_pointer&&)other) {}
		weak_ptr(weak_ptr const& other) : safe_pointer((safe_pointer const&)other) {}
		weak_ptr(ang::nullptr_t const&) : safe_pointer(null) {}
		weak_ptr(graphics::gl::gl_driver* obj) : safe_pointer(obj) {}
		weak_ptr(graphics::gl::gl_driver_t obj) : safe_pointer(obj.get()) {}
		~weak_ptr() {}

	public: //properties
		graphics::gl::gl_driver_t lock()const {
			auto ptr = safe_pointer::lock<intfptr>();
			return reinterpret_cast<graphics::gl::gl_driver*>(ptr.get());
		}

		weak_ptr& operator = (graphics::gl::gl_driver_t obj) { safe_pointer::operator=(obj.get()); return *this; }
		weak_ptr& operator = (graphics::gl::gl_driver* obj) { safe_pointer::operator=(obj);  return *this; }
		weak_ptr& operator = (weak_ptr&& other) { safe_pointer::operator=(other); return *this; }
		weak_ptr& operator = (weak_ptr const& other) { safe_pointer::operator=(other);  return *this; }
		weak_ptr& operator = (ang::nullptr_t const&) { safe_pointer::operator=(null); return *this; }
	};
}

#include "gl/buffers.hpp" 
#include "gl/shaders.hpp"
#include "gl/textures.hpp"

namespace ang
{
	namespace graphics
	{
		namespace gl
		{

			class gl_frame_buffer
				: public object
				, public iframe_buffer
			{
				weak_ptr<gl_driver> _parent_driver;
				graphics::size<float> _dimentions;
				collections::vector<com_wrapper<ID3D11RenderTargetView>> d3d_render_target;
				collections::vector<gl_texture_t> d3d_texture2D;
				com_wrapper<ID3D11DepthStencilView> d3d_depth_stencil;
				com_wrapper<ID3D11DepthStencilState> d3d_depth_stencil_state;

			public: //internal
				inline ID3D11RenderTargetView* D3DRenderTargetView(index idx)const { return d3d_render_target.is_empty() ? null : d3d_render_target[idx].get(); }
				inline ID3D11ShaderResourceView* D3D11ShaderResourceView(index idx)const { return d3d_texture2D.is_empty()? null : d3d_texture2D[idx]->D3D11ShaderResourceView(); }
				inline ID3D11DepthStencilView* D3DDepthStencilView()const { return d3d_depth_stencil.get(); }
				inline ID3D11DepthStencilState* D3DDepthStencilState()const { return d3d_depth_stencil_state.get(); }

			public:
				gl_frame_buffer(gl_driver*);

				ANG_DECLARE_INTERFACE();

				bool create(gl_surface_t);
				bool create(array_view<textures::tex_format_t> color_format, textures::tex_format_t depth_stencil_format, graphics::size<float> dimentions);
				bool close();

			public: //overrides
				graphics::size<float> dimentions()const override;
				uint color_buffer_count()const override;
				textures::itexture_t color_buffer(index)const;
				bool has_depth_stencil_buffer()const override;

			private:
				virtual~gl_frame_buffer();
			};

			class gl_surface final
				: public object
				, public isurface
			{
				graphics::size<float> _current_size;
				weak_ptr<gl_driver> _parent_driver;
				com_wrapper<IDXGISwapChain1> dxgi_swap_chain;
				gl_frame_buffer_t d3d_frame_buffer;

			public: //internal
				graphics::size<float> const& surface_size()const { return _current_size; }
				inline IDXGISwapChain1* DXGISwapChain()const { return dxgi_swap_chain.get(); }

			public:
				gl_surface(gl_driver*);

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
				virtual~gl_surface();
			};

			class gl_driver final
				: public object
				, public idriver
			{

				com_wrapper<ID2D1Factory2> d2d_factory;
				com_wrapper<ID2D1Device1> d2d_device;
				com_wrapper<ID2D1DeviceContext1> d2d_context;
				com_wrapper<ID2D1Bitmap1> d2d_target_bitmap;
				com_wrapper<IDWriteFactory2> dwrite_factory;
				//com_wrapper<IWICImagingFactory> wic_factory;
				D2D1::Matrix3x2F orientation_transform2D;

				D3D_FEATURE_LEVEL _feature_level;
				com_wrapper<IDXGIFactory2> dxgi_factory;
				com_wrapper<ID3D11Device2> d3d_device;
				com_wrapper<ID3D11DeviceContext2> d3d_context;
				com_wrapper<ID3D11BlendState> d3d_blend_state;

				gl_frame_buffer_t _current_frame_buffer;
				gl_shaders_t _current_shaders;	
				primitive_t _primitive;

				graphics::cull_mode_t _cull_mode;
				graphics::front_face_t _front_face;
				graphics::blend_mode_t _blend_mode;

				core::async::mutex_ptr_t main_mutex;

			//	gl_index_buffer_t _current_index_buffer;
			//	gl_vertex_buffer_t _current_vertex_buffer;

			public:
				gl_driver();

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

			public: //intenal
				inline ID3D11Device2* D3D11Device()const { return d3d_device.get(); }
				inline ID3D11DeviceContext2* D3D11Context()const { return d3d_context.get(); }
				//	inline safe_thread_wrapper<ID3D11DeviceContext2> D3D11Context()const { return{ d3d_context.get(), main_mutex }; }
				inline IDXGIFactory2* DXGIFactory()const { return dxgi_factory.get(); }
				inline gl_frame_buffer_t current_frame_buffer()const { return _current_frame_buffer.get(); }

			private:
				virtual~gl_driver();
			};
		}
	}
}


#endif