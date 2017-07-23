#pragma once

#include "d3d11/driver.h"
#include <ang/graphics/drawing.h>

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

			typedef object_wrapper<d3d11_solid_brush> d3d11_solid_brush_t;
			typedef object_wrapper<d3d11_linear_gradient_brush> d3d11_linear_gradient_brush_t;
			//typedef object_wrapper<d3d11_texturing_brush> d3d11_texturing_brush_t;

			typedef struct square
			{
				buffers::iindex_buffer_t indices;
				buffers::ivertex_buffer_t vertices;
			}square_t, *square_ptr_t;


			class d3d11_brush
				: public object
				, public drawing::ibrush
			{
			protected:
				d3d11_brush();
				virtual~d3d11_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();
				virtual void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, square_ptr_t) = 0;
			};

			class d3d11_solid_brush
				: public d3d11_brush
			{
			private:
				color_t _color;
				effects::ishaders_t _technique;

			public:
				d3d11_solid_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, square_ptr_t) override;

				bool create(d3d11_draw_context_t, color_t);

			private:
				virtual~d3d11_solid_brush();
			};

			class d3d11_linear_gradient_brush
				: public d3d11_brush
			{
			private:
				point<float> _start_point;
				point<float> _end_point;
				array<drawing::gradient_info::stop_color_info_t> _gradients;
				effects::ishaders_t _technique;

			public:
				d3d11_linear_gradient_brush();

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				void draw(d3d11_driver_t driver, maths::matrix4 const& tranform, square_ptr_t) override;

				bool create(d3d11_draw_context_t, drawing::gradient_info_t);

			private:
				virtual~d3d11_linear_gradient_brush();
			};


			class d3d11_draw_context
				: public object
				, public drawing::idraw_context
			{
			private:
				size<float> _dimentions;
				square_t _square;

			public:
				d3d11_draw_context(d3d11_driver_t, core::files::ifile_system_t = null);

			public: /*overrides*/
				ANG_DECLARE_INTERFACE();

				drawing::ibrush_t create_solid_brush(color_t) override;
				drawing::ibrush_t create_linear_gradient_brush(drawing::gradient_info_t) override;
				drawing::ibrush_t create_texturing_brush(textures::tex_wrap_mode_t, textures::itexture_t) override;
				void begin_draw(iframe_buffer_t) override;
				void end_draw() override;
				void clear(color_t) override;

				void draw_rect(drawing::ibrush_t brush, rect<float> rect)override;


			protected:
				bool create(d3d11_driver_t driver, core::files::ifile_system_t fs);
				bool close();

			public: //properties
				property<const bool, d3d11_draw_context> is_drawing;
				property<const d3d11_driver_t, d3d11_draw_context> driver;
				property<const effects::ieffect_library_t, d3d11_draw_context> effect_library;
				//property<const textures::itexture_loader_t, d3d11_draw_context> texture_loader;

			private:
				virtual~d3d11_draw_context();
			};


			inline xml::xml_node_t create_tecnique_template(cwstr_t name, cwstr_t vertex_shader, cwstr_t pixel_shader)
			{
				xml::xml_builder_t builder = new xml::xml_builder();
				builder->begin_element("technique"_s);
				{
					builder->attribute("name"_s, name);
					builder->begin_element("vertex_shader"_s);
					{
						builder->begin_element("uniforms"_s);
						{
							builder->attribute("name"_s, "world_data"_s);
							builder->begin_element("var"_s);
							{
								builder->attribute("type"_s, "f32"_s);
								builder->attribute("class"_s, "mat4"_s);
								builder->attribute("name"_s, "world"_s);
							}builder->end_element(/*var*/);
						}builder->end_element(/*uniforms*/);
						builder->begin_element("input_layout"_s);
						{
							builder->begin_element("var"_s);
							{
								builder->attribute("type"_s, "f32"_s);
								builder->attribute("class"_s, "vec3"_s);
								builder->attribute("name"_s, "position"_s);
								builder->attribute("semantic"_s, "POSITION"_s);
								builder->attribute("semantic_idx"_s, "0"_s);
							}builder->end_element(/*var*/);
						}builder->end_element(/*input_layout*/);
						builder->element("code"_s, vertex_shader);
					}builder->end_element(/*vertex_shader*/);
					builder->begin_element("pixel_shader"_s);
					{
						builder->begin_element("uniforms"_s);
						{
							builder->attribute("name"_s, "color_info"_s);
							builder->begin_element("var"_s);
							{
								builder->attribute("type"_s, "f32"_s);
								builder->attribute("class"_s, "vec4"_s);
								builder->attribute("name"_s, "additive_color"_s);
							}builder->end_element(/*var*/);
							builder->begin_element("var"_s);
							{
								builder->attribute("type"_s, "f32"_s);
								builder->attribute("class"_s, "vec4"_s);
								builder->attribute("name"_s, "diffuse_color"_s);
							}builder->end_element(/*var*/);
						}builder->end_element(/*uniforms*/);

						builder->begin_element("uniforms"_s);
						{
							builder->attribute("name"_s, "gradient_info"_s);
							builder->begin_element("var"_s);
							{
								builder->attribute("type"_s, "f32"_s);
								builder->attribute("class"_s, "vec2"_s);
								builder->attribute("name"_s, "gradient_start_point"_s);
							}builder->end_element(/*var*/);
							builder->begin_element("var"_s);
							{
								builder->attribute("type"_s, "f32"_s);
								builder->attribute("class"_s, "vec2"_s);
								builder->attribute("name"_s, "gradient_end_point"_s);
							}builder->end_element(/*var*/);
							builder->begin_element("var"_s);
							{
								builder->attribute("type"_s, "s32"_s);
								builder->attribute("class"_s, "scalar"_s);
								builder->attribute("name"_s, "gradient_colors_count"_s);
							}builder->end_element(/*var*/);
							builder->begin_element("var"_s);
							{
								builder->attribute("type"_s, "f32"_s);
								builder->attribute("class"_s, "vec4"_s);
								builder->attribute("name"_s, "color_factor"_s);
								builder->attribute("array"_s, "10"_s);
							}builder->end_element(/*var*/);
						}builder->end_element(/*uniforms*/);

						builder->element("code"_s, pixel_shader);
					}builder->end_element(/*pixel_shader*/);
				}builder->end_element(/*technique*/);
				return builder->xml_root();
			}

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
