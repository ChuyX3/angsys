#pragma once

#include "d3d11/d3d11_driver.h"
#include <ang/graphics/drawing.h>

#if DIRECTX_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			ang_object(d3d11_drawer);
			ang_object(d3d11_solid_brush);
			ang_object(d3d11_texturing_brush);
			ang_object(d3d11_linear_gradient_brush);
			ang_object(d3d11_linear_gradient_texturing_brush);

			class d3d11_brush
				: public smart<d3d11_brush, drawing::ibrush>
			{
			protected:
				d3d11_brush();
				virtual~d3d11_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();
				virtual void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) = 0;
			};

			class d3d11_solid_brush
				: public smart<d3d11_solid_brush, d3d11_brush>
			{
			private:
				color_t m_diffuse_color;
				color_t m_additive_color;
				ishaders_t m_technique;

			public:
				d3d11_solid_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) override;

				bool create(d3d11_drawer_t context, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_solid_brush();
			};

			class d3d11_linear_gradient_brush
				: public smart<d3d11_linear_gradient_brush, d3d11_brush>
			{
			private:
				color_t m_diffuse_color;
				color_t m_additive_color;
				point<float> m_start_point;
				point<float> m_end_point;
				array<drawing::gradient_info::stop_color_info_t> m_gradients;
				ishaders_t m_technique;

			public:
				d3d11_linear_gradient_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) override;

				bool create(d3d11_drawer_t context, drawing::gradient_info_t gradients, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_linear_gradient_brush();
			};

			class d3d11_texturing_brush
				: public smart<d3d11_texturing_brush, d3d11_brush>
			{
			private:
				color_t m_diffuse_color;
				color_t m_additive_color;
				size<float> m_tilling;
				textures::tex_stretch_mode_t m_stretch;
				textures::itexture_t m_texture;
				ishaders_t m_technique;

			public:
				d3d11_texturing_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) override;

				bool create(d3d11_drawer_t context, drawing::texturing_info_t info, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_texturing_brush();
			};

			class d3d11_linear_gradient_texturing_brush
				: public smart<d3d11_linear_gradient_texturing_brush, d3d11_brush>
			{
			private:
				color_t m_diffuse_color;
				color_t m_additive_color;
				size<float> m_tilling;
				point<float> m_start_point;
				point<float> m_end_point;
				textures::tex_stretch_mode_t m_stretch;
				array<drawing::gradient_info::stop_color_info_t> m_gradients;
				textures::itexture_t m_texture;
				ishaders_t m_technique;

			public:
				d3d11_linear_gradient_texturing_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, model_ptr_t) override;
				bool create(d3d11_drawer_t context, drawing::gradient_info_t gradients_info, drawing::texturing_info_t texture_info, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_linear_gradient_texturing_brush();
			};

			class d3d11_drawer
				: public smart<d3d11_drawer, drawing::idrawer>
			{
			private:
				size<uint> m_dimentions;
				model_t m_square;
				model_t m_circle;

				bool m_is_drawing;
				d3d11_driver_t m_driver;
				effects::ieffect_library_t m_effect_library;

			public:
				d3d11_drawer(d3d11_driver_t, core::files::ifile_system_t = null);

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
				bool is_drawing()const { return m_is_drawing; }
				d3d11_driver_t driver()const { return m_driver; }
				effects::ieffect_library_t effect_library()const { return m_effect_library; }
				//property<const textures::itexture_loader_t, d3d11_drawer> texture_loader;

			private:
				virtual~d3d11_drawer();
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
