#ifndef __ANGRAPH_DRAWING_H__
#define __ANGRAPH_DRAWING_H__

#include <ang/graphics/angraph.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGRAPH_EXPORTS
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
		namespace drawing
		{
			typedef struct gradient_point
			{
				color_t color;
				point<float> position;
				bool operator == (gradient_point const& other)const {
					return color.code == other.color.code && position.x == other.position.x && position.y == other.position.y;
				}
				bool operator != (gradient_point const& other)const {
					return color.code != other.color.code || position.x != other.position.x || position.y != other.position.y;
				}
			}gradient_point_t;

			ANG_INTERFACE(ibrush);
			ANG_INTERFACE(idraw_context);

			ANG_BEGIN_INTERFACE(LINK, ibrush)
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE(LINK, idraw_context)
				visible vcall ibrush_t create_solid_brush(color_t) pure;
				visible vcall ibrush_t create_linear_gradient_brush(static_array<gradient_point>) pure;
				visible vcall ibrush_t create_texturing_brush(textures::tex_wrap_mode_t, textures::itexture_t) pure;
				visible vcall void begin_draw(iframe_buffer_t) pure;
				visible vcall void end_draw() pure;
				visible vcall void clear(color_t) pure;
				visible vcall void draw_rect(ibrush_t brush, rect<float> rect)pure;
			ANG_END_INTERFACE();

			LINK idraw_context_t create_drawing_context(idriver_t, core::files::ifile_system_t = null);
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

ANG_REGISTER_RUNTIME_TYPENAME(ang::graphics::drawing::gradient_point_t)

#endif//__ANGRAPH_DRAWING_H__