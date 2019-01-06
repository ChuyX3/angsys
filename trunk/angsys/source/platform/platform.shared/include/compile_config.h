#ifndef __COMPILE_CONFIG_H__
#define __COMPILE_CONFIG_H__

#include <ang/base/platform.h>
#include <ang/base/types.h>

#if defined WINAPI_FAMILY
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#if defined _WINDLL || defined _USRDLL
#ifndef ANG_PLATFORM_EXPORTS
#define ANG_PLATFORM_EXPORTS
#endif//ANG_PLATFORM_EXPORTS
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

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllimport)
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

#ifdef __cplusplus
extern "C" LINK void ang_debug_output_info(const char* format, ...);
extern "C" LINK void ang_debug_output_warning(const char* format, ...);
extern "C" LINK void ang_debug_output_error(const char* format, ...);
#else
LINK void ang_debug_output_info(const char* format, ...);
LINK void ang_debug_output_warning(const char* format, ...);
LINK void ang_debug_output_error(const char* format, ...);
#endif


#ifdef  LINK
#undef  LINK
#endif//LINK

#define ANG_LOGI(...)  ang_debug_output_info(__VA_ARGS__)
#define ANG_LOGW(...)  ang_debug_output_warning(__VA_ARGS__)
#define ANG_LOGE(...)  ang_debug_output_error(__VA_ARGS__)

#endif//__COMPILE_CONFIG_H__