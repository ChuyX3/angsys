#ifndef __COMPILE_CONFIG_H__
#define __COMPILE_CONFIG_H__

#include "angtypes.h"

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

#endif//__COMPILE_CONFIG_H__