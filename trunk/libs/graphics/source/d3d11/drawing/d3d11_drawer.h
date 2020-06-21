#pragma once

#include "d3d11/d3d11_driver.h"
#include <ang/graphics/drawing.h>

#if DIRECTX11_SUPPORT

namespace ang
{
	namespace graphics
	{
		namespace d3d11
		{
			ang_declare_object(d3d11_drawer);
			ang_declare_object(d3d11_brush);
			ang_declare_object(d3d11_solid_brush);
			ang_declare_object(d3d11_texturing_brush);
			ang_declare_object(d3d11_radial_gradient_brush);
			ang_declare_object(d3d11_linear_gradient_brush);

			class d3d11_brush
				: public graphic<d3d11_brush
				, iid("ang::graphics::d3d11::d3d11_brush")>
			{
			protected:
				d3d11_brush();
				virtual~d3d11_brush();

			public: /*overrides*/
				virtual void dispose() override = 0;
				virtual void draw(d3d11_drawer_t context, maths::mat4 const& tranform, model_ptr_t) = 0;
			};

			class d3d11_solid_brush
				: public implement<d3d11_solid_brush
				, iid("ang::graphics::d3d11::d3d11_solid_brush")
				, d3d11_brush
				, drawing::ibrush>
			{
			private:
				effects::ishaders_t m_technique;
				color_t m_diffuse_color;
				color_t m_additive_color;
				float m_opacity;			

			public:
				d3d11_solid_brush();

			public: /*overrides*/
				void dispose() override;
				void opacity(float value) override;
				float opacity()const override;
				void diffuse(color_t valu) override;
				color_t diffuse()const override;
				void additive(color_t valu) override;
				color_t additive()const override;
				void draw(d3d11_drawer_t context, maths::mat4 const& tranform, model_ptr_t) override;
				
			public: /*methods*/
				bool create(d3d11_drawer_t context, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_solid_brush();
			};

			class d3d11_linear_gradient_brush
				: public implement<d3d11_linear_gradient_brush
				, iid("ang::graphics::d3d11::d3d11_linear_gradient_brush")
				, d3d11_brush
				, drawing::ilinear_gradient_brush
				, drawing::ibrush>
			{
			private:
				effects::ishaders_t m_technique;
				color_t m_diffuse_color;
				color_t m_additive_color;
				float m_opacity;
				point<float> m_start_point;
				point<float> m_end_point;
				array<drawing::stop_color_info_t> m_gradients;

			public:
				d3d11_linear_gradient_brush();

			public: /*overrides*/
				void dispose() override;
				void opacity(float value) override;
				float opacity()const override;
				void diffuse(color_t valu) override;
				color_t diffuse()const override;
				void additive(color_t valu) override;
				color_t additive()const override;
				void start_point(point<float> value) override;
				point<float> start_point()const override;
				void end_point(point<float> value) override;
				point<float> end_point()const override;
				void stop_colors(array_view<drawing::stop_color_info_t> value) override;
				array<drawing::stop_color_info_t> stop_colors()const override;
				void draw(d3d11_drawer_t context, maths::mat4 const& tranform, model_ptr_t) override;

			public: /*methods*/
				bool create(d3d11_drawer_t context, drawing::linear_gradient_info_t gradients, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_linear_gradient_brush();
			};

			class d3d11_radial_gradient_brush
				: public implement<d3d11_radial_gradient_brush
				, iid("ang::graphics::d3d11::d3d11_radial_gradient_brush")
				, d3d11_brush
				, drawing::iradial_gradient_brush
				, drawing::ibrush>
			{
			private:
				effects::ishaders_t m_technique;
				color_t m_diffuse_color;
				color_t m_additive_color;
				float m_opacity;
				point<float> m_first_focus;
				point<float> m_second_focus;
				array<drawing::stop_color_info_t> m_gradients;

			public:
				d3d11_radial_gradient_brush();

			public: /*overrides*/
				void dispose() override;
				void opacity(float value) override;
				float opacity()const override;
				void diffuse(color_t valu) override;
				color_t diffuse()const override;
				void additive(color_t valu) override;
				color_t additive()const override;
				void first_focus(point<float>) override;
				point<float> first_focus()const override;
				void second_focus(point<float>) override;
				point<float> second_focus()const override;
				void stop_colors(array_view<drawing::stop_color_info_t>) override;
				array<drawing::stop_color_info_t> stop_colors()const override;
				void draw(d3d11_drawer_t context, maths::mat4 const& tranform, model_ptr_t) override;
			
			public: /*methods*/
				bool create(d3d11_drawer_t context, drawing::radial_gradient_info_t gradients, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_radial_gradient_brush();
			};

			class d3d11_texturing_brush
				: public implement<d3d11_texturing_brush
				, iid("ang::graphics::d3d11::d3d11_texturing_brush")
				, d3d11_brush
				, drawing::itexturing_brush
				, drawing::ibrush>
			{
			private:
				effects::ishaders_t m_technique;
				color_t m_diffuse_color;
				color_t m_additive_color;
				float m_opacity;
				size<float> m_tilling;
				textures::tex_wrap_mode_t m_wrap_mode;
				textures::tex_stretch_mode_t m_stretch;
				textures::itexture_t m_texture;

			public:
				d3d11_texturing_brush();

			public: /*overrides*/
				void dispose() override;
				void opacity(float value) override;
				float opacity()const override;
				void diffuse(color_t valu) override;
				color_t diffuse()const override;
				void additive(color_t valu) override;
				color_t additive()const override;
				void texture(textures::itexture_t) override;
				textures::itexture_t texture()const override;
				void wrap_mode(textures::tex_wrap_mode_t) override;
				textures::tex_wrap_mode_t wrap_mode()const override;
				void stretch_mode(textures::tex_stretch_mode_t) override;
				textures::tex_stretch_mode_t stretch_mode()const override;
				void tiling_factor(size<float>) override;
				size<float> tiling_factor()const override;
				void draw(d3d11_drawer_t context, maths::mat4 const& tranform, model_ptr_t) override;

			public: /*methods*/
				bool create(d3d11_drawer_t context, drawing::texturing_info_t info, color_t diffuse, color_t additive);

			private:
				virtual~d3d11_texturing_brush();
			};

			class d3d11_drawer
				: public graphic<d3d11_drawer
				, iid("ang::graphics::d3d11::d3d11_drawer")
				, drawing::idrawer>
			{
			private:
				size<uint> m_dimentions;
				model_t m_square;
				model_t m_circle;
				blend_mode_t m_blend;

				stack_array<com_wrapper<ID3D11SamplerState>, 3> m_samperStates;
				collections::hash_map<string, effects::ishaders_t> m_techniques;

				bool m_is_drawing;
				d3d11_driver_t m_driver;

			public:
				d3d11_drawer(d3d11_driver_t);

			public: /*overrides*/
				void dispose()override;
				drawing::ibrush_t create_solid_brush(color_t diffuse, color_t additive)override;
				drawing::ilinear_gradient_brush_t create_linear_gradient_brush(drawing::linear_gradient_info_t, color_t diffuse, color_t additive)override;
				drawing::iradial_gradient_brush_t create_radial_gradient_brush(drawing::radial_gradient_info_t, color_t diffuse, color_t additive)override;
				drawing::itexturing_brush_t create_texturing_brush(drawing::texturing_info_t, color_t diffuse, color_t additive)override;

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
				ID3D11SamplerState* D3DSamperState(windex i)const {
					return m_samperStates[i].get();
				}

				effects::ishaders_t find_technique(cstr_t key)const {
					try { return m_techniques[key]; }
					catch (exception const& e) {
						cstr_t whats = e.what();
						d3d11_debug_print(0, "{0}\n", whats.cstr<text::encoding::ascii>());
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
