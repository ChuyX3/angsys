#ifndef __ANGRAPH_DRAWING_H__
#define __ANGRAPH_DRAWING_H__

#include <ang/graphics/graphics.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef GRAPHICS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
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
			safe_enum(LINK, tex_stretch_mode, uint)
			{
				none = 0,
				fill = 1,
				uniform = 2,
				uniform_to_fill = 3,
			};
		}

		namespace drawing
		{
			typedef struct gradient_info
			{
				typedef struct stop_color_info
				{
					float stop_factor;
					color_t stop_color;
					bool operator == (stop_color_info const& other)const { return false; }
					bool operator != (stop_color_info const& other)const { return false; }
				}stop_color_info_t;

				point<float> start_point;
				point<float> end_point;
				array_view<stop_color_info_t> stop_colors;
				inline gradient_info(point<float> start, point<float>end, array_view<stop_color_info_t>colors)
					: start_point(start)
					, end_point(end)
					, stop_colors(colors){
				}
			}gradient_info_t;

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
					, tiling_factor(tiling){
				}
			}texturing_info_t, *texturing_info_ptr_t;
		

			ang_interface(ibrush);
			ang_interface(idrawer);

			ang_begin_interface(LINK ibrush)
			ang_end_interface();

			ang_begin_interface(LINK idrawer)
				visible vcall ibrush_t create_solid_brush(
					color_t /*diffuse*/, 
					color_t /*additive*/ = colors::null
				) pure;
				visible vcall ibrush_t create_linear_gradient_brush(
					gradient_info_t,
					color_t /*diffuse*/ = colors::white,
					color_t /*additive*/ = colors::null) pure;
				visible vcall ibrush_t create_texturing_brush(
					texturing_info_t,
					color_t /*diffuse*/ = colors::white,
					color_t /*additive*/ = colors::null) pure;
				visible vcall ibrush_t create_linear_gradient_texturing_brush(
					gradient_info_t, 
					texturing_info_t,
					color_t /*diffuse*/ = colors::white,
					color_t /*additive*/ = colors::null) pure;
				visible vcall void begin_draw(iframe_buffer_t) pure;
				visible vcall void end_draw() pure;
				visible vcall void clear(color_t) pure;
				visible vcall void draw_rect(ibrush_t brush, rect<float> rect)pure;
			ang_end_interface();

			LINK idrawer_t create_drawing_context(idriver_t, core::files::ifile_system_t = null);
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

//ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::drawing::gradient_info_t)
//ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::drawing::gradient_info::stop_color_info_t)

#endif//__ANGRAPH_DRAWING_H__