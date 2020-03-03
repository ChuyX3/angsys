#ifndef __COMPILE_CONFIG_H__
#define __COMPILE_CONFIG_H__

#include <ang/base/platform.h>
#include <ang/base/types.h>

#if defined WINAPI_FAMILY
#include <WinSock2.h>
#include <ws2tcpip.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#if defined _WINDLL || defined _USRDLL
#ifndef ANG_EXPORTS
#define ANG_EXPORTS
#endif//ANG_EXPORTS
#define ANG_DYNAMIC_LIBRARY
#else
#define ANG_STATIC_LIBRARY
#endif

#elif defined __ANDROID__
#define ANG_STATIC_LIBRARY
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
#if defined ANG_DYNAMIC_LIBRARY


#ifdef ANG_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

#ifdef __cplusplus
extern "C" LINK void ang_debug_output_info(const char* message);
extern "C" LINK void ang_debug_output_warning(const char* message);
extern "C" LINK void ang_debug_output_error(const char* message);
#else
LINK void ang_debug_output_info(const char* message);
LINK void ang_debug_output_warning(const char* message);
LINK void ang_debug_output_error(const char* message);
#endif


#ifdef  LINK
#undef  LINK
#endif//LINK

#define ANG_LOGI(...)  ang_debug_output_info(__VA_ARGS__)
#define ANG_LOGW(...)  ang_debug_output_warning(__VA_ARGS__)
#define ANG_LOGE(...)  ang_debug_output_error(__VA_ARGS__)

#endif//__COMPILE_CONFIG_H__