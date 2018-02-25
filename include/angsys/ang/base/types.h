/*********************************************************************************************************************/
/*   File Name: ang/base/types.h                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types for multiplatform use                                  */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_TYPES_H__
#define __ANG_TYPES_H__

#include<ang/base/platform.h>

#if defined WINDOWS_PLATFORM
#define STDCALL	__stdcall
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
#define __stdcall
#define STDCALL __stdcall
#elif defined MICRO8_PLATFORM
#define __stdcall
#define STDCALL __stdcall
#endif

#ifdef _DEVELOPPER
#define ANG_DEVELOPPER //for developper
#endif//ANG_DEVELOPPER

#if defined _DEBUG || defined _DEVELOPPER
#define DEBUG_SAFE_CODE
#endif//_DEBUG

#ifdef __cplusplus
#define ANG_EXTERN extern "C"
#else
#define ANG_EXTERN extern
#endif//__cplusplus

typedef char				ang_char_t;
typedef unsigned char		ang_mchar_t;
typedef wchar_t				ang_wchar_t;

typedef char				ang_int8_t;
typedef unsigned char		ang_uint8_t;

typedef ang_uint8_t 		ang_bool_t;
#define ang_false ((ang_bool_t)0)
#define ang_true ((ang_bool_t)1)

#ifndef MICRO8_PLATFORM
typedef short				ang_int16_t;
typedef unsigned short		ang_uint16_t;
typedef int					ang_int32_t;
typedef unsigned int		ang_uint32_t;
typedef long				ang_lint32_t;
typedef unsigned long		ang_ulint32_t;
typedef long long			ang_int64_t;
typedef unsigned long long	ang_uint64_t;
typedef float				ang_float32_t;
typedef double				ang_float64_t;
#else
typedef short int				ang_int16_t;
typedef unsigned short int		ang_uint16_t;
typedef short long int			ang_int24_t;
typedef unsigned short long int	ang_uint24_t;
typedef long int				ang_int32_t;
typedef unsigned long int		ang_uint32_t;
typedef double					ang_float32_t;
#endif//MICRO8_PLATFORM

#if defined WINDOWS_PLATFORM
#if defined _WIN64
typedef unsigned __int64	ang_size_t;
typedef unsigned __int64	ang_index_t;
typedef unsigned __int64	ang_int_ptr_t;
typedef __int64				ang_long_t;
typedef unsigned __int64	ang_ulong_t;
#elif defined _ARM_
typedef unsigned int	ang_size_t;
typedef unsigned int	ang_index_t;
typedef unsigned int	ang_int_ptr_t;
typedef unsigned long	ang_long_t;
typedef long			ang_ulong_t;
#elif defined _WIN32 
typedef __w64 unsigned int	ang_size_t;
typedef __w64 unsigned int	ang_index_t;
typedef __w64 unsigned int	ang_int_ptr_t;
typedef __w64 int			ang_long_t;
typedef __w64 unsigned int	ang_ulong_t;
#endif
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
typedef unsigned int		ang_size_t;
typedef unsigned int		ang_index_t;
typedef unsigned int*		ang_int_ptr_t;
typedef unsigned long		ang_long_t;
typedef long				ang_ulong_t;
#elif defined MICRO8_PLATFORM
typedef unsigned int		ang_size_t;
typedef unsigned int		ang_index_t;
typedef unsigned int*		ang_int_ptr_t;
typedef unsigned int* far	ang_int_lptr_t;

#endif
typedef void*				ang_void_ptr_t;
typedef void*				ang_object_ptr_t;
typedef void*				ang_buffer_t;
typedef void*				ang_handle_t;
typedef char*				ang_str_t;
typedef wchar_t*			ang_wstr_t;
typedef unsigned char*		ang_mstr_t;
typedef char const*			ang_cstr_t;
typedef wchar_t const*		ang_cwstr_t;
typedef unsigned char const* ang_cmstr_t;
typedef char const*			ang_type_name_t;

typedef unsigned char		ang_byte_t;
typedef unsigned short		ang_word_t;
typedef unsigned long		ang_dword_t;

typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned char mchar;
typedef wchar_t wchar;
#ifdef __cplusplus
typedef char16_t char16;
typedef char32_t char32;
#else
typedef unsigned short char16;
typedef unsigned long char32;
#endif
typedef unsigned short word;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long dword;
typedef unsigned long ulong;
typedef long long long64;
typedef unsigned long long qword;
typedef unsigned long long ulong64;
typedef void * pointer;
#ifdef  index
#undef  index
#endif//index
typedef uint index;
typedef size_t windex;
typedef size_t wsize;

typedef union
{
	ang_byte_t value;
	struct {
		ang_bool_t bit0 : 1;
		ang_bool_t bit1 : 1;
		ang_bool_t bit2 : 1;
		ang_bool_t bit3 : 1;
		ang_bool_t bit4 : 1;
		ang_bool_t bit5 : 1;
		ang_bool_t bit6 : 1;
		ang_bool_t bit7 : 1;
	}bits;
}ang_byte_bits_t;

typedef union
{
	ang_word_t value;
	struct {
		ang_byte_bits_t lo;
		ang_byte_bits_t hi;
	}bytes;
}ang_word_bits_t;

typedef union
{
	ang_dword_t value;
	struct {
		ang_word_bits_t lo;
		ang_word_bits_t hi;
	}words;
}ang_dword_bits_t;



#endif//__ANG_TYPES_H__