#ifndef __ANG_DRAWING_H__
#define __ANG_DRAWING_H__

#include <ang/graphics/graphics.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY

#ifdef ANG_GRAPHICS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_GRAPHICS_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

namespace ang
{
	namespace graphics
	{
		namespace textures
		{
			declare_enum(LINK, tex_stretch_mode, uint)
			{
				none = 0,
				fill = 1,
				uniform = 2,
				uniform_to_fill = 3,
			};
		}

		namespace drawing
		{
			typedef collections::tuple<float, color_t> stop_color_info_t;

			typedef struct linear_gradient_info
			{
				point<float> start_point;
				point<float> end_point;
				array<stop_color_info_t> stop_colors;
				inline linear_gradient_info(point<float> start, point<float>end, array_view<stop_color_info_t>colors)
					: start_point(start)
					, end_point(end)
					, stop_colors(colors) {
				}
			}linear_gradient_info_t;

			typedef struct radial_gradient_info
			{
				point<float> first_focus;
				point<float> second_focus;
				array<stop_color_info_t> stop_colors;
				inline radial_gradient_info(point<float> first, point<float> second, array_view<stop_color_info_t>colors)
					: first_focus(first)
					, second_focus(second)
					, stop_colors(colors) {
				}
			}radial_gradient_info_t;

			typedef struct texturing_info
			{
				textures::itexture_t texture;
				textures::tex_wrap_mode_t wrap_mode;
				textures::tex_stretch_mode_t stretch_mode;
				size<float> tiling_factor;
				inline texturing_info(textures::itexture_t tex,
					textures::tex_wrap_mode_t wrap = textures::tex_wrap_mode::def,
					textures::tex_stretch_mode_t stretch = textures::tex_stretch_mode::fill,
					size<float> tiling = { 1,1 })
					: texture(tex)
					, wrap_mode(wrap)
					, stretch_mode(stretch)
					, tiling_factor(tiling) {
				}
			}texturing_info_t, *texturing_info_ptr_t;


			ang_declare_interface(ibrush);
			ang_declare_interface(ilinear_gradient_brush);
			ang_declare_interface(iradial_gradient_brush);
			ang_declare_interface(itexturing_brush);
			ang_declare_interface(idrawer);

			struct nvt LINK ibrush
				: intf<ibrush
				, iid("ang::graphics::drawing::ibrush")>
			{
				virtual void opacity(float) = 0;
				virtual float opacity()const = 0;

				virtual void diffuse(color_t) = 0;
				virtual color_t diffuse()const = 0;

				virtual void additive(color_t) = 0;
				virtual color_t additive()const = 0;

			};

			struct nvt LINK ilinear_gradient_brush
				: intf<ilinear_gradient_brush
				, iid("ang::graphics::drawing::ilinear_gradient_brush")
				, ibrush>
			{
				virtual void start_point(point<float>) = 0;
				virtual point<float> start_point()const = 0;
				virtual void end_point(point<float>) = 0;
				virtual point<float> end_point()const = 0;
				virtual void stop_colors(array_view<stop_color_info_t>) = 0;
				virtual array_view<stop_color_info_t const> stop_colors()const = 0;
			};

			struct nvt LINK iradial_gradient_brush
				: intf<iradial_gradient_brush
				, iid("ang::graphics::drawing::iradial_gradient_brush")
				, ibrush>
			{
				virtual void first_focus(point<float>) = 0;
				virtual point<float> first_focus()const = 0;
				virtual void second_focus(point<float>) = 0;
				virtual point<float> second_focus()const = 0;
				virtual void stop_colors(array_view<stop_color_info_t>) = 0;
				virtual array_view<stop_color_info_t> stop_colors()const = 0;
			};

			struct nvt LINK itexturing_brush
				: intf<itexturing_brush
				, iid("ang::graphics::drawing::itexturing_brush")
				, ibrush>
			{
				virtual void texture(textures::itexture_t) = 0;
				virtual textures::itexture_t texture()const = 0;
				virtual void wrap_mode(textures::tex_wrap_mode_t) = 0;
				virtual textures::tex_wrap_mode_t wrap_mode()const = 0;
				virtual void stretch_mode(textures::tex_stretch_mode_t) = 0;
				virtual textures::tex_stretch_mode_t stretch_mode()const = 0;
				virtual void tiling_factor(size<float>) = 0;
				virtual size<float> tiling_factor()const = 0;
			};

			struct nvt LINK idrawer
				: intf<idrawer
				, iid("ang::graphics::drawing::idrawer")>
			{
				virtual ibrush_t create_solid_brush(
					color_t /*diffuse*/,
					color_t /*additive*/ = colors::null) = 0;
				virtual ilinear_gradient_brush_t create_linear_gradient_brush(
					linear_gradient_info_t,
					color_t /*diffuse*/ = colors::white,
					color_t /*additive*/ = colors::null) = 0;
				virtual iradial_gradient_brush_t create_radial_gradient_brush(
					radial_gradient_info_t,
					color_t /*diffuse*/ = colors::white,
					color_t /*additive*/ = colors::null) = 0;
				virtual itexturing_brush_t create_texturing_brush(
					texturing_info_t,
					color_t /*diffuse*/ = colors::white,
					color_t /*additive*/ = colors::null) = 0;
				
				virtual void begin_draw(iframe_buffer_t) = 0;
				virtual void end_draw() = 0;
				virtual void clear(color_t) = 0;
				virtual void draw_rect(ibrush_t brush, rect<float> rect) = 0;
			};

			LINK idrawer_t create_drawing_context(idriver_t);
		}
	}
}

ANG_ENUM_DECLARATION(LINK, ang::graphics::textures::tex_stretch_mode)

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_DRAWING_H__