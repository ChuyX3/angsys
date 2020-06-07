#pragma once

#include "d3d11/d3d11_driver.h"
#include <coffe/graphics/drawing.h>

#if DIRECTX11_SUPPORT

namespace coffe
{
	namespace graphics
	{
		namespace d3d11
		{
			coffe_object(d3d11_drawer);
			coffe_object(d3d11_solid_brush);
			coffe_object(d3d11_texturing_brush);
			coffe_object(d3d11_linear_gradient_brush);
			coffe_object(d3d11_linear_gradient_texturing_brush);

			class d3d11_brush
				: public smart<d3d11_brush, drawing::ibrush>
			{
			protected:
				d3d11_brush();
				virtual~d3d11_brush();

			public: /*overrides*/
				COFFE_DECLARE_INTERFACE();
				virtual void draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t) = 0;
			};

			class d3d11_solid_brush
				: public smart<d3d11_solid_brush, d3d11_brush>
			{
			private:
				color_t m_diffuse_color;
				color_t m_additive_color;
				effects::ishaders_t m_technique;

			public:
				d3d11_solid_brush();

			public: /*overrides*/
				COFFE_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t) override;

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
				effects::ishaders_t m_technique;

			public:
				d3d11_linear_gradient_brush();

			public: /*overrides*/
				COFFE_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t) override;

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
				effects::ishaders_t m_technique;

			public:
				d3d11_texturing_brush();

			public: /*overrides*/
				COFFE_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t) override;

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
				effects::ishaders_t m_technique;

			public:
				d3d11_linear_gradient_texturing_brush();

			public: /*overrides*/
				COFFE_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::mat4 const& tranform, model_ptr_t) override;
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

				collections::hash_map<string, effects::ishaders_t> m_techniques;

				bool m_is_drawing;
				d3d11_driver_t m_driver;

			public:
				d3d11_drawer(d3d11_driver_t);

			private:
				void dispose()override;

			public: /*overrides*/
				COFFE_DECLARE_INTERFACE();

				drawing::ibrush_t create_solid_brush(color_t, color_t) override;
				drawing::ibrush_t create_linear_gradient_brush(drawing::gradient_info_t, color_t, color_t) override;
				drawing::ibrush_t create_texturing_brush(drawing::texturing_info_t, color_t, color_t) override;
				drawing::ibrush_t create_linear_gradient_texturing_brush(drawing::gradient_info_t, drawing::texturing_info_t, color_t, color_t) override;

				void begin_draw(iframe_buffer_t) override;
				void end_draw() override;
				void clear(color_t) override;

				void draw_rect(drawing::ibrush_t brush, rect<float> rect)override;


			protected:
				bool create(d3d11_driver_t driver);
				bool close();

			public: //properties
				bool is_drawing()const { return m_is_drawing; }
				d3d11_driver_t driver()const { return m_driver; }
	
				effects::ishaders_t find_technique(cstr_t key)const {
					try { return m_techniques[key]; }
					catch (exception_t const& e) {
						cstr_t whats = e->what();
						d3d11_debug_print(0, "%s\n", whats.cstr<text::encoding::ascii>().cstr());
						return null;
					}
				}

				effects::ishaders_t load_technique(effects::shader_info_t const& vertex, effects::shader_info_t const& pixel, cstr_t name) {
					if (auto res = m_driver->compile_shaders(vertex, pixel, name))
					{
						m_techniques[name] = res.get();
						return res.get();
					}
					return null;
				}

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
