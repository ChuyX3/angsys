#ifndef __ANG_WIN32_KEYBOARD_H__
#define __ANG_WIN32_KEYBOARD_H__

#include <angsys.h>
#include <ang/core/timer.h>
#include <ang/platform/platform.h>
#include <ang/platform/win32/windows.h>

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
		namespace input
		{
			ang_object(keyboard);

			class LINK keyboard
				: public smart<keyboard, ikeyboard>
			{
			private:
				windows::ui::text_box_t m_control;

			public:
				keyboard();
				ANG_DECLARE_INTERFACE();

			private:
				virtual~keyboard();
			};
		}
	}
}


#endif//WINDOWS_PLATFORM
#endif//__ANG_WIN32_KEYBOARD_H__
