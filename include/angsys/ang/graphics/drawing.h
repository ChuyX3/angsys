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
			ANG_INTERFACE(ibrush);
			ANG_INTERFACE(idraw_context);

			ANG_BEGIN_INTERFACE(LINK, ibrush)
			ANG_END_INTERFACE();


		}

	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANGRAPH_DRAWING_H__