#ifndef __ANG_CORE_STRING_H__
#define __ANG_CORE_STRING_H__

#include <angtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef char  ang_ascii_char_t;
typedef dword  ang_utf32_char_t;

//typedef unsigned char  ang_utf8_char_t;
//typedef unsigned short  ang_utf16_char_t;
//typedef unsigned int  ang_utf32_char_t;

#pragma pack(push, 8)
typedef union {
	dword value;
	byte bytes[4];
}ang_utf8_char_t;
#pragma pack(pop)

#pragma pack(push, 16)
typedef union {
	dword value;
	word bytes[2];
}ang_utf16_char_t;
#pragma pack(pop)

typedef char*  ang_ascii_str_t;
typedef byte*  ang_utf8_str_t;
typedef word*  ang_utf16_str_t;
typedef dword*  ang_utf32_str_t;

typedef char const*  ang_ascii_cstr_t;
typedef byte const*  ang_utf8_cstr_t;
typedef word const*  ang_utf16_cstr_t;
typedef dword const*  ang_utf32_cstr_t;

typedef  struct ang_core_string ang_core_string_t, *ang_core_string_ptr_t, *ang_core_hstring_t;

typedef enum
{
	ang_core_char_format_ascii = 0XFF,
	ang_core_char_format_utf8 = 0,
	ang_core_char_format_utf16 = 3,
	ang_core_char_format_utf16_be = 1,
	ang_core_char_format_utf16_le = 2,
	ang_core_char_format_utf32 = 12,
	ang_core_char_format_utf32_be = 4,
	ang_core_char_format_utf32_le = 8,
}ang_core_char_format_t;

typedef enum
{
	ang_special_chars_be_repl = 0XFFFDU,//Replcement Character
	ang_special_chars_be_ubom = 0XFEFFU,//Unicode Byte Order Mark
	ang_special_chars_be_mbom = 0XEFBBBFU,//MultiByte Order Mark
	ang_special_chars_be_null = 0XC080U,//modified utf-8, null character
	ang_special_chars_le_repl = 0XFDFFU,//Inverted Replcement Character
	ang_special_chars_le_ubom = 0XFFFEU,//Inverted Unicode Byte Order Mark
	ang_special_chars_le_mbom = 0XBFBBEFU,//Inverted MultiByte Order Mark
	ang_special_chars_le_null = 0X80C0U,//Inverted modified utf-8, null character
}ang_special_chars, ang_special_chars_t;


typedef union
{
	word _data_format;
	word _flag_is_local_data;
	struct
	{
		uint _unused;
		uint _buffer_size_used;
		uint _buffer_capacity;
		pointer _buffer_ptr;
	};
	struct //32 bytes
	{
		uint _local_size;
		union
		{
			char _local_ascii_buffer[128]; //128 ascii char
			byte _local_utf8_buffer[128]; //128 utf8 char
			word _local_utf16_buffer[64]; //64 utf16 char
			uint _local_utf32_buffer[32]; //32 utf32 char
		};
	};
}ang_core_string_data, ang_core_string_data_t;

struct ang_core_string_impl
{
	ang_core_char_format_t(*format)(ang_core_hstring_t);
	wsize(*length)(ang_core_hstring_t);
	pointer(*data)(ang_core_hstring_t);

	int(*compare)(ang_core_hstring_t, ang_core_hstring_t);
	wsize(*concat)(ang_core_hstring_t, ang_core_hstring_t);
	wsize(*copy)(ang_core_hstring_t, ang_core_hstring_t);
	void(*move)(ang_core_hstring_t, ang_core_hstring_t);

	int(*compare_rawdata)(ang_core_hstring_t, pointer, ang_core_char_format_t);
	wsize(*concat_rawdata)(ang_core_hstring_t, pointer, ang_core_char_format_t);
	wsize(*copy_rawdata)(ang_core_hstring_t, pointer, ang_core_char_format_t);

	ang_core_string_data_t _data;
};

ang_core_char_format_t ang_core_string_get_format(ang_core_hstring_t);
wsize ang_core_string_get_length(ang_core_hstring_t);
pointer ang_core_string_get_data(ang_core_hstring_t);

int ang_core_string_compare(ang_core_hstring_t, ang_core_hstring_t);
wsize ang_core_string_concat(ang_core_hstring_t, ang_core_hstring_t);
wsize ang_core_string_copy(ang_core_hstring_t, ang_core_hstring_t);
void ang_core_string_move(ang_core_hstring_t, ang_core_hstring_t);


int ang_core_string_compare_ascii_ascii(ang_ascii_cstr_t, ang_ascii_cstr_t);
int ang_core_string_compare_ascii_utf8(ang_ascii_cstr_t, ang_utf8_cstr_t);
int ang_core_string_compare_ascii_utf16(ang_ascii_cstr_t, ang_utf16_cstr_t);
int ang_core_string_compare_ascii_utf32(ang_ascii_cstr_t, ang_utf32_cstr_t);

int ang_core_string_compare_utf8_ascii(ang_utf8_cstr_t, ang_ascii_cstr_t);
int ang_core_string_compare_utf8_utf8(ang_utf8_cstr_t, ang_utf8_cstr_t);
int ang_core_string_compare_utf8_utf16(ang_utf8_cstr_t, ang_utf16_cstr_t);
int ang_core_string_compare_utf8_utf32(ang_utf8_cstr_t, ang_utf32_cstr_t);

int ang_core_string_compare_utf16_ascii(ang_utf16_cstr_t, ang_ascii_cstr_t);
int ang_core_string_compare_utf16_utf8(ang_utf16_cstr_t, ang_utf8_cstr_t);
int ang_core_string_compare_utf16_utf16(ang_utf16_cstr_t, ang_utf16_cstr_t);
int ang_core_string_compare_utf16_utf32(ang_utf16_cstr_t, ang_utf32_cstr_t);

int ang_core_string_compare_utf32_ascii(ang_utf32_cstr_t, ang_ascii_cstr_t);
int ang_core_string_compare_utf32_utf8(ang_utf32_cstr_t, ang_utf8_cstr_t);
int ang_core_string_compare_utf32_utf16(ang_utf32_cstr_t, ang_utf16_cstr_t);
int ang_core_string_compare_utf32_utf32(ang_utf32_cstr_t, ang_utf32_cstr_t);

#ifdef __cplusplus
}
#endif

#endif//__ANG_CORE_STRING_H__



