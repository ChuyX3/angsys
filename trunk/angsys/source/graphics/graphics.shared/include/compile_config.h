#ifndef __COMPILE_CONFIG_H__
#define __COMPILE_CONFIG_H__

#include <ang/base/platform.h>
#include <ang/base/types.h>

#if defined WINAPI_FAMILY
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#if defined _WINDLL || defined _USRDLL
#ifndef ANGRAPH_EXPORTS
#define ANGRAPH_EXPORTS
#endif//ANGRAPH_EXPORTS
#define ANGSYS_DYNAMIC_LIBRARY
#else
#define ANGSYS_STATIC_LIBRARY
#endif

#elif defined __ANDROID__
#define ANGSYS_STATIC_LIBRARY
#endif//WINAPI_FAMILY

#ifndef DEBUG_PRINT_LEVEL
#define DEBUG_PRINT_LEVEL_0	0
#define DEBUG_PRINT_LEVEL_1	1
#define DEBUG_PRINT_LEVEL_2	2
#define DEBUG_PRINT_LEVEL_3	3
#if defined _DEBUG
#define DEBUG_PRINT_LEVEL DEBUG_PRINT_LEVEL_3
#else
#define DEBUG_PRINT_LEVEL DEBUG_PRINT_LEVEL_0
#endif
#endif//DEBUG_PRINT_LEVEL


#define NULL_DRIVER			(0X00)
#define OPENGL_DRIVER		(0X01)
#define OPENGLES_DRIVER		(0X02)
#define DIRECTX_DRIVER		DIRECTX11_DRIVER
#define DIRECTX11_DRIVER	(0X04)
#define DIRECTX12_DRIVER	(0X08)
#define VULKAN_DRIVER		(0X10)




#define IS_DRIVER_SUPPORTED(driver)		((DRIVER_SUPPORTED & (driver)) == (driver))
#define DRIVER_SUPPORTED	((OPENGL_SUPPORT?1:0)|(OPENGLES_SUPPORT?2:0)|(DIRECTX_SUPPORT?4:0))
#define OPENGL_FAMILY_SUPPORT (OPENGL_SUPPORT||OPENGLES_SUPPORT)
#if defined WINDOWS_PLATFORM
#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#define OPENGL_SUPPORT		(1)
#define OPENGLES_SUPPORT	(1)
#define DIRECTX_SUPPORT		(DIRECTX9_SUPPORT || DIRECTX11_SUPPORT)
#define DIRECTX9_SUPPORT	(1)
#define DIRECTX11_SUPPORT	(1)
#define DIRECTX12_SUPPORT	(1)
#define VULKAN_SUPPORT		(0)
#else//elif WINDOWS_PLATFORM == WINDOWS_STORE_PLATFORM
#define OPENGL_SUPPORT		(0)
#define OPENGLES_SUPPORT	(0)
#define DIRECTX_SUPPORT		DIRECTX11_SUPPORT
#define DIRECTX11_SUPPORT	(1)
#define DIRECTX12_SUPPORT	(1)
#endif
#elif defined ANDROID_PLATFORM
#define OPENGL_SUPPORT		(0)
#define OPENGLES_SUPPORT	(1)
#define DIRECTX_SUPPORT		(0)
#elif defined LINUX_PLATFORM
#define OPENGL_SUPPORT		(1)
#define OPENGLES_SUPPORT	(0)
#define DIRECTX_SUPPORT		(0)
#else
#define OPENGL_SUPPORT		(0)
#define OPENGLES_SUPPORT	(0)
#define DIRECTX_SUPPORT		(0)
#endif

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY
#define LINK __declspec(dllimport)
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

ANG_EXTERN void LINK ang_debug_output_info(const char* format, ...);
ANG_EXTERN void LINK ang_debug_output_warning(const char* format, ...);
ANG_EXTERN void LINK ang_debug_output_error(const char* format, ...);

#undef LINK

#endif//__COMPILE_CONFIG_H__