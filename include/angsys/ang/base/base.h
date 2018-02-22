/*********************************************************************************************************************/
/*   File Name: angbase.h                                                                                            */
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

#ifdef __cplusplus

//#include <ang/interface.h>
//#include <ang/utils.h>

#endif//__cplusplus

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

#include <ang/base/defines.h>
#include <ang/base/utils.h>
#include <ang/base/memory.h>
#include <ang/base/value.h>

namespace ang
{

}


#ifdef LINK
#undef LINK
#endif//LINK

#endif//__ANG_BASE_H__