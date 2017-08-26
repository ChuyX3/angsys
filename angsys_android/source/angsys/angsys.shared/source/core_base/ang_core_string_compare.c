#include <angtypes.h>
#include "ang_core_string.h"

#define new(_TYPE) (_TYPE*)ang_alloc_unmanaged_memory(sizeof(_TYPE))
#define delete(_PTR) ang_free_unmanaged_memory(_PTR)


#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
#elif defined WINDOWS_PLATFORM
#endif


int ang_core_string_compare_ascii_ascii(ang_ascii_cstr_t first, ang_ascii_cstr_t second)
{
	wsize c = 0;
	if (first == second)
		return 0;
	else if (first == NULL)
		return 1;
	else if (second == NULL)
		return -1;

LOOP:
	if ((first[c] == 0) && (second[c] == 0))
		return 0;
	else if (first[c] > second[c])
		return 1;
	else if (first[c] < second[c])
		return -1;
	else
		c++;
	goto LOOP;	
}

int ang_core_string_compare_ascii_utf8(ang_ascii_cstr_t first, ang_utf8_cstr_t second)
{
	
	return 0;
}

int ang_core_string_compare_ascii_utf16(ang_ascii_cstr_t first, ang_utf16_cstr_t second)
{
	return 0;
}

int ang_core_string_compare_ascii_utf32(ang_ascii_cstr_t first, ang_utf32_cstr_t second)
{
	return 0;
}


int ang_core_string_compare_utf8_ascii(ang_utf8_cstr_t first, ang_ascii_cstr_t second)
{
	return 0;
}

int ang_core_string_compare_utf8_utf8(ang_utf8_cstr_t first, ang_utf8_cstr_t second)
{
	wsize c = 0;
	if (first == second)
		return 0;
	else if (first == NULL)
		return 1;
	else if (second == NULL)
		return -1;

LOOP:
	if ((first[c] == 0) && (second[c] == 0))
		return 0;
	else if (first[c] > second[c])
		return 1;
	else if (first[c] < second[c])
		return -1;
	else
		c++;
	goto LOOP;
}

int ang_core_string_compare_utf8_utf16(ang_utf8_cstr_t first, ang_utf16_cstr_t second)
{
	return 0;
}

int ang_core_string_compare_utf8_utf32(ang_utf8_cstr_t first, ang_utf32_cstr_t second)
{
	return 0;
}


int ang_core_string_compare_utf16_ascii(ang_utf16_cstr_t first, ang_ascii_cstr_t second)
{
	return 0;
}

int ang_core_string_compare_utf16_utf8(ang_utf16_cstr_t first, ang_utf8_cstr_t second)
{
	return 0;
}

int ang_core_string_compare_utf16_utf16(ang_utf16_cstr_t first, ang_utf16_cstr_t second)
{
	wsize c = 0;
	if (first == second)
		return 0;
	else if (first == NULL)
		return 1;
	else if (second == NULL)
		return -1;

LOOP:
	if ((first[c] == 0) && (second[c] == 0))
		return 0;
	else if (first[c] > second[c])
		return 1;
	else if (first[c] < second[c])
		return -1;
	else
		c++;
	goto LOOP;
}

int ang_core_string_compare_utf16_utf32(ang_utf16_cstr_t first, ang_utf32_cstr_t second)
{
	return 0;
}


int ang_core_string_compare_utf32_ascii(ang_utf32_cstr_t first, ang_ascii_cstr_t second)
{
	return 0;
}

int ang_core_string_compare_utf32_utf8(ang_utf32_cstr_t first, ang_utf8_cstr_t second)
{
	return 0;
}

int ang_core_string_compare_utf32_utf16(ang_utf32_cstr_t first, ang_utf16_cstr_t second)
{
	return 0;
}

int ang_core_string_compare_utf32_utf32(ang_utf32_cstr_t first, ang_utf32_cstr_t second)
{
	wsize c = 0;
	if (first == second)
		return 0;
	else if (first == NULL)
		return 1;
	else if (second == NULL)
		return -1;

LOOP:
	if ((first[c] == 0) && (second[c] == 0))
		return 0;
	else if (first[c] > second[c])
		return 1;
	else if (first[c] < second[c])
		return -1;
	else
		c++;
	goto LOOP;
}