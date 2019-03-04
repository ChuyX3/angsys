#ifndef __ANG_CONSOLE_H__
#define __ANG_CONSOLE_H__

#include <angsys.h>
#include <ang/core/async.h>
#include <ang/dom/xml.h>
#include <ang/platform/platform.h>
#include <ang/collections/list.h>
#include <ang/collections/hash_map.h>

#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY
#ifdef ANG_PLATFORM_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EX_EXPORTS
#else//#elif defined ANGsys_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace platform
	{
		namespace console
		{
			ang_object(app);
			ang_object(window);
			ang_object(input);
			ang_object(output);
		}
	}
}


#endif//WINDOWS_PLATFORM
#endif//__ANG_CONSOLE_H__