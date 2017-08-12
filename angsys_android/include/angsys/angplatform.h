/*********************************************************************************************************************/
/*   File Name: angplatform.h                                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes useful macros for multiplatform programing.                              */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef ANG_PLATFORM

#if defined WINAPI_FAMILY

#define WINDOWS_PLATFORM WINAPI_FAMILY
#define WINDOWS_STORE_PLATFORM WINAPI_FAMILY_PC_APP
#define WINDOWS_DESKTOP_PLATFORM WINAPI_FAMILY_DESKTOP_APP
#define WINDOWS_PHONE_PLATFORM WINAPI_FAMILY_PHONE_APP
#define ANG_PLATFORM WINDOWS_PLATFORM

#elif defined __ANDROID__

#define ANDROID_PLATFORM
#define ANG_PLATFORM ANDROID_PLATFORM

#elif defined __18CXX

#define MICRO8_PLATFORM
#define ANG_PLATFORM MICRO8_PLATFORM
#include <stddef.h>
#define ANG_DONT_USE_CORE_INTERFACE
#endif//WINAPI_FAMILY

#ifdef MICRO8_PLATFORM
//#define ANG_MICRO8_USE_EXTEND_MEMORY
#define ANG_NEAR	near
#define ANG_FAR		far
#else
#define ANG_NEAR
#define ANG_FAR
#endif

#ifdef WINDOWS_PLATFORM
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#ifndef ANG_NO_INCLUDE_WINDOWS_H
#include <windows.h>
#endif//ANG_NO_INCLUDE_WINDOWS_H
#include <intrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif//WINDOWS_PLATFORM

#ifdef ANDROID_PLATFORM

#include <initializer_list>

#define index _index
#define rindex _rindex
#define strchr _strchr
#define strrchr _strrchr

#include <stdio.h>
#include <stdlib.h>

#undef index
#undef rindex
#undef strchr
#undef strrchr

#include <jni.h>
#include <cstddef>
#include <unistd.h>
#include <poll.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#include <android/log.h>

#include <wchar.h>
#include <sys/resource.h>
#include <android/log.h>
#include <pthread.h>

#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/mman.h>
#include <mm_malloc.h>


#define _vscprintf(_format, _va_list)  vsnprintf(NULL, 0, _format, _va_list)
#define vsprintf_s(_buffer, _buff_size, _format, _va_list) vsprintf(_buffer, _format, _va_list)
#define _vscwprintf(_format, _va_list)  vswprintf(NULL, 0, _format, _va_list)
#define vswprintf_s(_buffer, _buff_size, _format, _va_list) vswprintf(_buffer, _buff_size, _format, _va_list)
#define wcstoull wcstoul
#define wcstoll wcstol

#define __debugbreak() __android_log_assert("","angsys","debug break")

#endif//ANDROID_PLATFORM

#ifdef __cplusplus
#include <initializer_list>
#endif//__cplusplus

#endif//ANG_PLATFORM