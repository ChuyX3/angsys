/*********************************************************************************************************************/
/*   File Name: ang/base/base.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#define __ANG_BASE_H__

#include <ang/base/types.h>

#ifdef LINK
#undef LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
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

#ifdef __cplusplus
#include <ang/base/defines.h>
#include <ang/base/utils.h>
#include <ang/base/memory.h>
#include <ang/base/tuple.h>
#include <ang/base/array.h>
#include <ang/base/text.h>
#include <ang/base/runtime.h>
#include <ang/base/value.h>
#include <ang/base/interface.h>
#include <ang/base/async.h>
#endif//__cplusplus

#ifdef LINK
#undef LINK
#endif//LINK

#endif//__ANG_BASE_H__