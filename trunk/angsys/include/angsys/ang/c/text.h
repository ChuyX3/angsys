/*********************************************************************************************************************/
/*   File Name: ang/base/text.h                                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_TEXT_H__
#define __ANG_BASE_TEXT_H__

#ifdef __cplusplus 
#error ...
#endif


typedef enum endian
{
	endian_little = 0,
	endian_big = 1,
#ifdef _WIN32
	endian_native = endian_little
#elif defined ANDROID_PLATFORM
	endian_native = endian_big
#else 
	endian_native = endian_little
#endif
}endian_t;

typedef enum encoding {
	encoding_none = 0,
	encoding_binary = 0,
	encoding_ascii,
	encoding_unicode,
	encoding_utf8,
	encoding_utf16,
	encoding_utf16_se,
	encoding_utf16_le,
	encoding_utf16_be,
	encoding_utf32,
	encoding_utf32_se,
	encoding_utf32_le,
	encoding_utf32_be,
	encoding_auto_detect = 0XFFFF
}encoding_t;

typedef struct
{
	encoding_t encoding;
	uint size;
	void* buffer;
}raw_str, raw_str_t;


ANG_EXTERN LINK uint ang_strlen(char const* str);
ANG_EXTERN LINK int ang_strcmp(char const* str1, char const* str2);
ANG_EXTERN LINK char* ang_strcpy(char* dest, char const* scr);
ANG_EXTERN LINK char* ang_strcat(char* dest, char const* src);

ANG_EXTERN LINK char* ang_itoa(ang_int32_t value, int base/* = [2|8|10|16]*/, char* dest);
ANG_EXTERN LINK char* ang_utoa(ang_uint32_t value, int base/* = [2|8|10|16]*/, char* dest);
ANG_EXTERN LINK char* ang_ftoa(double value, int pres, char* dest);

ANG_EXTERN LINK int ang_rstrcmp(raw_str_t const* str1, raw_str_t const* str2);
ANG_EXTERN LINK raw_str_t* ang_rstrcpy(raw_str_t* dest, raw_str_t const* scr);
ANG_EXTERN LINK raw_str_t* ang_rstrcat(raw_str_t* dest, raw_str_t const* src);


#endif//__ANG_BASE_TEXT_H__
