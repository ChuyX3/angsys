#pragma once

#include "d3d11/driver.hpp"
#include <ang/graphics/drawing.hpp>

#if DIRECTX_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			class d3d11_draw_context;
			typedef object_wrapper<d3d11_draw_context> d3d11_draw_context_t;

			class d3d11_solid_brush;
			class d3d11_texturing_brush;
			class d3d11_linear_gradient_brush;
			class d3d11_linear_gradient_texturing_brush;

			typedef object_wrapper<d3d11_solid_brush> d3d11_solid_brush_t;
			typedef object_wrapper<d3d11_texturing_brush> d3d11_texturing_brush_t;
			typedef object_wrapper<d3d11_linear_gradient_brush> d3d11_linear_gradient_brush_t;
			typedef object_wrapper<d3d11_linear_gradient_texturing_brush> d3d11_linear_gradient_texturing_brush_t;

			typedef struct _model
			{
				buffers::iindex_buffer_t indices;
				buffers::ivertex_buffer_t vertices;
			}model_t, *model_ptr_t;


			class d3d11_brush
				: public object
				, public drawing::ibrush
			{
			protected:
				d3d11_brush();
				virtual~d3d11_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();
				virtual void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) = 0;
			};

			class d3d11_solid_brush
				: public d3d11_brush
			{
			private:
				color_t _diffuse_color;
				color_t _additive_color;
				effects::ishaders_t _technique;

			public:
				d3d11_solid_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) override;

				bool create(d3d11_draw_context_t context, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_solid_brush();
			};

			class d3d11_linear_gradient_brush
				: public d3d11_brush
			{
			private:
				color_t _diffuse_color;
				color_t _additive_color;
				point<float> _start_point;
				point<float> _end_point;
				array<drawing::gradient_info::stop_color_info_t> _gradients;
				effects::ishaders_t _technique;

			public:
				d3d11_linear_gradient_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) override;

				bool create(d3d11_draw_context_t context, drawing::gradient_info_t gradients, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_linear_gradient_brush();
			};

			class d3d11_texturing_brush
				: public d3d11_brush
			{
			private:
				color_t _diffuse_color;
				color_t _additive_color;
				size<float> _tilling;
				textures::tex_stretch_mode_t _stretch;
				textures::itexture_t _texture;
				effects::ishaders_t _technique;

			public:
				d3d11_texturing_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) override;

				bool create(d3d11_draw_context_t context, drawing::texturing_info_t info, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_texturing_brush();
			};

			class d3d11_linear_gradient_texturing_brush
				: public d3d11_brush
			{
			private:
				color_t _diffuse_color;
				color_t _additive_color;
				size<float> _tilling;
				point<float> _start_point;
				point<float> _end_point;
				textures::tex_stretch_mode_t _stretch;
				array<drawing::gradient_info::stop_color_info_t> _gradients;
				textures::itexture_t _texture;
				effects::ishaders_t _technique;

			public:
				d3d11_linear_gradient_texturing_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) override;
				bool create(d3d11_draw_context_t context, drawing::gradient_info_t gradients_info, drawing::texturing_info_t texture_info, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_linear_gradient_texturing_brush();
			};

			class d3d11_draw_context
				: public object
				, public drawing::idraw_context
			{
			private:
				size<float> _dimentions;
				model_t _square;
				model_t _circle;

				bool _is_drawing;
				d3d11_driver_t _driver;
				effects::ieffect_library_t _effect_library;

			public:
				d3d11_draw_context(d3d11_driver_t, core::files::ifile_system_t = null);

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				drawing::ibrush_t create_solid_brush(color_t, color_t) override;
				drawing::ibrush_t create_linear_gradient_brush(drawing::gradient_info_t, color_t, color_t) override;
				drawing::ibrush_t create_texturing_brush(drawing::texturing_info_t, color_t, color_t) override;
				drawing::ibrush_t create_linear_gradient_texturing_brush(drawing::gradient_info_t, drawing::texturing_info_t, color_t, color_t) override;

				void begin_draw(iframe_buffer_t) override;
				void end_draw() override;
				void clear(color_t) override;

				void draw_rect(drawing::ibrush_t brush, rect<float> rect)override;


			protected:
				bool create(d3d11_driver_t driver, core::files::ifile_system_t fs);
				bool close();

			public: //properties
				bool is_drawing()const { return _is_drawing; }
				d3d11_driver_t driver()const { return _driver; }
				effects::ieffect_library_t effect_library()const { return _effect_library; }
				//property<const textures::itexture_loader_t, d3d11_draw_context> texture_loader;

			private:
				virtual~d3d11_draw_context();
			};

			inline maths::float4 color_to_vector(graphics::color_t color)
			{
				return{ color.components.red / 255.0f,
					color.components.green / 255.0f,
					color.components.blue / 255.0f,
					color.components.alpha / 255.0f,
				};
			}

			inline maths::float4 color_to_vector(graphics::color_t color, float alpha)
			{
				return{ color.components.red / 255.0f,
					color.components.green / 255.0f,
					color.components.blue / 255.0f,
					alpha,
				};
			}


		}
	}
}

#endif
