/*********************************************************************************************************************/
/*   File Name: algorithms.cpp                                                                                       */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "ang_inlines.h"
#include "string_utils_inline.h"

using namespace ang;
using namespace ang::strings;
using namespace ang::text;

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

static const wchar nullwchar = L'\0';
static const char nullchar = '\0';

ANG_IMPLEMENT_ENUM(text, encoding, uint, encoding::unknown)
//ANG_IMPLEMENT_INTERFACE(ang::text, itext_buffer);

static ang_pair<encoding, cstr_t> to_string_encoding_map[] =
{
	{ ang::text::encoding::unicode, "ISO-10646"_s },
	{ ang::text::encoding::ascii, "ISO-8859-1"_s },
	{ ang::text::encoding::unknown, "UNKNOWN"_s },
	{ ang::text::encoding::utf_16, "UTF-16"_s },
	{ ang::text::encoding::utf_8, "UTF-8"_s }
};

static ang_pair<cstr_t, encoding> _parse_encoding_map[] =
{
	{ "ISO-10646"_s, ang::text::encoding::unicode },
	{ "ISO-8859-1"_s, ang::text::encoding::ascii },
	{ "UNKNOWN"_s, ang::text::encoding::unknown },
	{ "UTF-16"_s, ang::text::encoding::utf_16 },
	{ "UTF-8"_s, ang::text::encoding::utf_8 }
};

encoding_t encoding_t::parse(cstr_t text)
{
	auto idx = ang_binary_search<cstr_t>(text, _parse_encoding_map);
	if (idx >= array_size(_parse_encoding_map))
		return ang::text::encoding::unknown;
	else
		return _parse_encoding_map[idx].value;
}

encoding_t encoding_t::parse(cwstr_t text)
{
	auto idx = ang_binary_search<cwstr_t>(text, _parse_encoding_map);
	if (idx >= array_size(_parse_encoding_map))
		return ang::text::encoding::unknown;
	else
		return _parse_encoding_map[idx].value;
}

cstr_t encoding_t::to_string()const
{
	auto idx = ang_binary_search<encoding>(_value, to_string_encoding_map);
	if (idx >= array_size(to_string_encoding_map))
		return "UNKNOWN";
	else
		return to_string_encoding_map[idx].value;
}

uint algorithms::string_length(const char* cstr, uint max)
{
	if (cstr == null || max == 0U)
		return 0U;
	uint idx = 0;
	while (idx < max && cstr[idx] != 0) idx++;
	return idx;
}

uint algorithms::string_length(const wchar* cstr, uint max)
{
	if (cstr == null || max == 0U)
		return 0U;
	uint idx = 0;
	while (idx < max && cstr[idx] != 0) idx++;
	return idx;
}

uint algorithms::string_length(const mchar* cstr, uint max)
{
	if (cstr == null)
		return 0;
	uint c = 0;
	uint i = 0;
	uint n = 0;

	while ((i < max) && (cstr[i] != 0) && !((cstr[i] == 0XC0) && (cstr[i + 1] == 0X80)))
	{
		switch (cstr[i] & 0XC0)
		{
		case 0X00:
		case 0X40:
			if (n != 0)
				return -1;
			c++;
			break;
		case 0X80:
			if (n == 0)
				return -1;
			n--;
			break;
		case 0XC0:
			if (n != 0)
				return -1;
			if ((cstr[i] & 0XF0) == 0XC0)
				n = 1;
			if ((cstr[i] & 0XF0) == 0XE0)
				n = 2;
			if ((cstr[i] & 0XF0) == 0XF0)
				return -1;
			c++;
			break;
		}
		i++;
	}
	return c;
}

uint algorithms::string_size(const char* cstr, text::encoding_t encoding)
{
	if (cstr == null)
		return 0;

	switch (encoding)
	{
	case ang::text::encoding_t::ascii:
		return string_length(cstr);
	case ang::text::encoding_t::unicode:
		return string_length(cstr) * (uint)sizeof(wchar);
	case ang::text::encoding_t::utf_8:
	//case ang::text::encoding_t::utf_16:
	{
		uint i = 0, s = 0;
		while (cstr[i] != 0)
		{
			if (cstr[i] <= 0X7F)
				s += 1U;
			else //if(cstr[i] <= 0X7FF)
				s += 2U;
			i++;
		}
		return s;
	}
	default: return 0;
	}
}

uint algorithms::string_size(const wchar* cstr, text::encoding_t encoding)
{
	if (cstr == null)
		return 0;

	switch (encoding)
	{
	case ang::text::encoding_t::ascii:
		return string_length(cstr);
	case ang::text::encoding_t::unicode:
		return string_length(cstr) * (uint)sizeof(wchar);
	case ang::text::encoding_t::utf_8:
	//case ang::text::encoding_t::utf_16:
	{
		uint i = 0, s = 0;
		while (cstr[i] != 0)
		{
			if (cstr[i] <= 0X7F)
				s += 1U;
			else if (cstr[i] <= 0X7FF)
				s += 2U;
			else if (cstr[i] <= 0XFFFF)
				s += 3U;
			i++;
		}
		return s;
	}
	default: return 0;
	}
}

uint algorithms::string_size(const mchar* cstr, text::encoding_t encoding)
{
	if (cstr == null)
		return 0;
	uint i = 0;

	switch (encoding)
	{
	case ang::text::encoding_t::ascii:
		string_length(cstr);
	case ang::text::encoding_t::unicode:
		return string_length(cstr) * (uint)sizeof(wchar);
	case ang::text::encoding_t::utf_8:
	//case ang::text::encoding_t::utf_16:
		while ((cstr[i] != 0) && !((cstr[i] == 0XC0) && (cstr[i + 1] == 0X80)))
			i++;
		return i;
	default: return 0;
	}
}

uint algorithms::string_copy(char* str, const char* cstr, uint max)
{
	if (str == null || cstr == null || max == 0U)
		return 0U;
	uint idx = 0;
	while (idx < max && cstr[idx] != 0)
		str[idx++] = cstr[idx];
	if(idx < max) str[idx] = 0;
	return idx;
}

uint algorithms::string_copy(char* str, const wchar* cstr, uint max)
{
	if (str == null || cstr == null || max == 0U)
		return 0U;
	uint idx = 0;
	while (idx < max && cstr[idx] != 0)
		str[idx++] = (char)cstr[idx];
	if (idx < max) str[idx] = 0;
	return idx;
}

uint algorithms::string_copy(char* str, const mchar* cstr, uint max)
{
	if (str == null)
		return 0;
	if (cstr == null)
		return 0;

	mbyte datum;
	bool exit = false;
	uint c = 0, i = 0;

	while (c < max && (cstr[i] != nullchar) && !exit)
	{
		datum.convert(nullwchar);
		datum[0] = cstr[i++];

		switch (datum[0] & 0XC0)
		{
		case 0X00:
		case 0X40:
			str[c++] = (char)datum.convert();
			break;
		case 0X80:
			exit = true;
			break;
		case 0XC0:
			if (cstr[i] == nullchar)
				return 0;
			if ((datum[0] & 0XF0) == 0XC0)
			{
				datum[1] = cstr[i++];
				if (datum.convert() == 0) //Can be Null Character (0XC0,0X80)
				{
					exit = true;
					break;
				}
			}
			else
			{
				datum[1] = cstr[i++];
				if (cstr[i] == nullchar)
					return 0;
				datum[2] = cstr[i++];
			}
			str[c++] = (char)datum.convert(); //Lost of data
			if (datum.size() < 2)exit = true;
			break;
		}
	}
	if (c < max) str[c] = 0;
	return c;
}

uint algorithms::string_copy(wchar* str, const wchar* cstr, uint max)
{
	if (str == null || cstr == null || max == 0U)
		return 0U;
	uint idx = 0;
	while (idx < max && cstr[idx] != 0)
		str[idx++] = cstr[idx];
	if(idx < max)str[idx] = 0;
	return idx;
}

uint algorithms::string_copy(wchar* str, const char* cstr, uint max)
{
	if (str == null || cstr == null || max == 0U)
		return 0U;
	uint idx = 0;
	while (idx < max && cstr[idx] != 0)
		str[idx++] = (wchar)cstr[idx];
	if (idx < max)str[idx] = 0;
	return idx;
}

uint algorithms::string_copy(wchar* str, const mchar* cstr, uint max)
{
	if (str == null)
		return false;
	if (cstr == null)
		return false;

	mbyte datum;
	bool exit = false;
	uint c = 0, i = 0;

	while (c < max && (cstr[i] != nullchar) && !exit && c < max)
	{
		datum.convert(nullwchar);
		datum[0] = cstr[i++];

		switch (datum[0] & 0XC0)
		{
		case 0X00:
		case 0X40:
			str[c++] = datum.convert();
			break;
		case 0X80:
			exit = true;
			break;
		case 0XC0:
			if (cstr[i] == nullchar)
				return false;
			if ((datum[0] & 0XF0) == 0XC0)
			{
				datum[1] = cstr[i++];
				if (datum.convert() == 0) //Can be Null Character (0XC0,0X80)
				{
					exit = true;
					break;
				}
			}
			else
			{
				datum[1] = cstr[i++];
				if (cstr[i] == nullchar)
					return false;
				datum[2] = cstr[i++];
			}
			str[c++] = datum.convert();
			if (datum.size() < 2)exit = true;
			break;
		}
	}
	if (c < max) str[c] = 0;
	return c;
}


uint algorithms::string_copy(mchar* str, const mchar* cstr, uint max)
{
	if ((cstr == null) || (str == null))
		return 0;
	uint c = 0;
	while (c < max && (str[c] = cstr[c]) != 0)
		c++;
	return c;
}

uint algorithms::string_copy(mchar* str, const char* cstr, uint max)
{
	if ((cstr == null) || (str == null))
		return 0;

	mbyte datum;
	uint c = 0, i = 0;
	while (c < max && cstr[i] != 0)
	{
		datum.convert(cstr[i]);
		if (datum.size() == 1) {
			str[c++] = datum[0];
		}
		else {
			str[c++] = datum[0];
			str[c++] = datum[1];
		}
		i++;
	}
	if (c < max)str[c] = 0;
	return c;
}

uint algorithms::string_copy(mchar* str, const wchar* cstr, uint max)
{
	if ((cstr == null) || (str == null))
		return 0;

	mbyte datum;
	uint c = 0, i = 0;
	while (c < max && cstr[i] != 0)
	{
		datum.convert((wchar)cstr[i]);
		if (datum.size() == 1) {
			str[c++] = datum[0];
		}
		else if (datum.size() == 2) {
			str[c++] = datum[0];
			str[c++] = datum[1];

		}
		else if (datum.size() == 3) {
			str[c++] = datum[0];
			str[c++] = datum[1];
			str[c++] = datum[2];

		}
		else {
			str[c++] = datum[0];
			str[c++] = datum[1];
			str[c++] = datum[2];
			str[c++] = datum[3];
		}
		i++;
	}
	if (c < max)str[c] = 0;
	return c;
}

uint algorithms::string_expand(char* str, uint len, uint beg, uint end, uint max)
{
	if (beg >= end || beg >= len || len >= max)
		return 0;
	if (end >= max)
		return (max - beg - 1);

	uint count = len - beg;
	if (count >= (max - end))
		count = max - end - 1;
		
	for (uint i = count; i > 0U; --i)
	{
		str[end + i - 1] = str[beg + i - 1];
	}
	str[end + count] = 0;
	return end - beg;
}

uint algorithms::string_expand(wchar* str, uint len, uint beg, uint end, uint max)
{
	if (beg >= end || beg >= len || len >= max)
		return 0;
	if (end >= max)
		return (max - beg - 1);

	uint count = len - beg;
	if (count >= (max - end))
		count = max - end - 1;

	for (uint i = count; i > 0U; --i)
	{
		str[end + i - 1] = str[beg + i - 1];
	}
	str[end + count] = 0;
	return end - beg;
}

uint algorithms::string_contract(char* str, uint len, uint beg, uint end)
{
	if (beg >= end || beg >= len || str == null)
		return 0;

	if (end >= len)
	{
		str[beg] = 0;
		return (len - beg);
	}

	for (uint i = 0; i < (len - end); ++i)
	{
		str[beg + i] = str[end + i];
	}
	str[len + beg - end] = 0;
	return end - beg;
}

uint algorithms::string_contract(wchar* str, uint len, uint beg, uint end)
{
	if (beg >= end || beg >= len || str == null)
		return 0;

	if (end >= len)
	{
		str[beg] = 0;
		return (len - beg);
	}

	for (uint i = 0; i < (len - end); ++i)
	{
		str[beg + i] = str[end + i];
	}
	str[len + beg - end] = 0;
	return end - beg;
}

int algorithms::string_compare(const char* first, const char* second)
{
	if (first == second)
		return 0;
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;
	uint c = 0;
	while (true) {
		if ((first[c] == 0) && (second[c] == 0))
			return 0;
		else if (first[c] > second[c])
			return 1;
		else if (first[c] < second[c])
			return -1;
		else
			c++;
	}
}

int algorithms::string_compare(const char* first, const wchar* second)
{
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;
	uint c = 0;
	while (true) {
		if ((first[c] == 0) && (second[c] == 0))
			return 0;
		else if (first[c] > second[c])
			return 1;
		else if (first[c] < second[c])
			return -1;
		else
			c++;
	}
}

int algorithms::string_compare(const char* first, const mchar* second)
{
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;
	uint c = 0, c2 = 0;
	mbyte datum1, datum2;
	while (true) {
		datum1.convert(first[c++]);
		if (datum1.size() == 1)
			datum2[0] = second[c2++];
		else
		{
			datum2[0] = second[c2++];
			if (second[c2] == 0)
				return 1;
			datum2[1] = second[c2++];
		}

		if ((datum1 == 0) && (datum2 == 0))
			return 0;
		else if (datum1 > datum2)
			return 1;
		else if (datum1 < datum2)
			return -1;
	}
	return 0;
}

int algorithms::string_compare(const wchar* first, const wchar* second)
{
	if (first == second)
		return 0;
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;
	uint c = 0;
	while (true) {
		if ((first[c] == 0) && (second[c] == 0))
			return 0;
		else if (first[c] > second[c])
			return 1;
		else if (first[c] < second[c])
			return -1;
		else
			c++;
	}
}

int algorithms::string_compare(const wchar* first, const char* second)
{
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;
	uint c = 0;
	while (true) {
		if ((first[c] == 0) && (second[c] == 0))
			return 0;
		else if (first[c] > second[c])
			return 1;
		else if (first[c] < second[c])
			return -1;
		else
			c++;
	}
}

int algorithms::string_compare(const wchar* first, const mchar* second)
{
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;
	uint c = 0, c2 = 0;
	mbyte datum1, datum2;
	while (true) {
		datum1.convert(first[c++]);
		int s = datum1.size();
		if (s == 1)
			datum2[0] = second[c2++];
		else if (s == 2)
		{
			datum2[0] = second[c2++];
			if (second[c2] == 0)
				return 1;
			datum2[1] = second[c2++];
		}
		else if (s == 3)
		{
			datum2[0] = second[c2++];
			if (second[c2] == 0)
				return 1;
			datum2[1] = second[c2++];
			if (second[c2] == 0)
				return 1;
			datum2[2] = second[c2++];
		}
		else if (s == 4)
		{
			datum2[0] = second[c2++];
			if (second[c2] == 0)
				return 1;
			datum2[1] = second[c2++];
			if (second[c2] == 0)
				return 1;
			datum2[2] = second[c2++];
			if (second[c2] == 0)
				return 1;
			datum2[3] = second[c2++];
		}

		if ((datum1 == 0) && (datum2 == 0))
			return 0;
		else if (datum1 > datum2)
			return 1;
		else if (datum1 < datum2)
			return -1;
	}
	return 0;
}


int algorithms::string_compare(const mchar* first, const mchar* second)
{
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;
	uint c = 0;
	while (true) {
		if ((first[c] == 0) && (second[c] == 0))
			return 0;
		else if (first[c] > second[c])
			return 1;
		else if (first[c] < second[c])
			return -1;
		else
			c++;
	}
}

int algorithms::string_compare(const mchar* first, const char* second)
{
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;
	uint c = 0, c2 = 0;
	mbyte datum1, datum2;
	while (true) {
		datum2.convert(second[c++]);
		if (datum2.size() == 1)
			datum1[0] = first[c2++];
		else
		{
			datum1[0] = first[c2++];
			if (first[c2] == 0)
				return -1;
			datum1[1] = first[c2++];
		}

		if ((datum1 == 0) && (datum2 == 0))
			return 0;
		else if (datum1 > datum2)
			return 1;
		else if (datum1 < datum2)
			return -1;
	}
	return 0;
}

int algorithms::string_compare(const mchar* first, const wchar* second)
{
	if ((first == null) && (second == null))
		return 0;
	else if (first == null)
		return 1;
	else if (second == null)
		return -1;

	uint c = 0, c2 = 0;
	mbyte datum1, datum2;
	while (true) {
		datum2.convert(second[c++]);
		int s = datum2.size();
		if (s == 1)
			datum1[0] = first[c2++];
		else if (s == 2)
		{
			datum1[0] = first[c2++];
			if (first[c2] == 0)
				return -1;
			datum1[1] = first[c2++];
		}
		else if (s == 3)
		{
			datum1[0] = first[c2++];
			if (first[c2] == 0)
				return -1;
			datum1[1] = first[c2++];
			if (first[c2] == 0)
				return -1;
			datum1[2] = first[c2++];
		}
		else
		{
			datum1[0] = first[c2++];
			if (first[c2] == 0)
				return -1;
			datum1[1] = first[c2++];
			if (first[c2] == 0)
				return -1;
			datum1[2] = first[c2++];
			if (first[c2] == 0)
				return -1;
			datum1[3] = first[c2++];
		}

		if ((datum1 == 0) && (datum2 == 0))
			return 0;
		else if (datum1 > datum2)
			return 1;
		else if (datum1 < datum2)
			return -1;
	}
	return 0;
}


index algorithms::string_compare_until(const char* first, const char* second)
{
	if (first == second)
		return string_length(first);
	if ((first == null) || (second == null))
		return 0;
	
	index c = 0;
	while (first[c] != 0 && second[c] != 0 && first[c] == second[c])
		c++;
	return c;
}

index algorithms::string_compare_until(const char* first, const wchar* second)
{
	if ((first == null) || (second == null))
		return 0;
	index c = 0;
	while (first[c] != 0 && second[c] != 0 && first[c] == second[c])
		c++;
	return c;
}

index algorithms::string_compare_until(const char* first, const mchar* second)
{
	if ((first == null) || (second == null))
		return 0;
	
	index c = 0, c2 = 0;
	mbyte datum1, datum2;
	while (true) {
		datum1.convert(first[c++]);
		if (datum1.size() == 1)
			datum2[0] = second[c2++];
		else
		{
			datum2[0] = second[c2++];
			if (second[c2] == 0)
				return c;
			datum2[1] = second[c2++];
		}

		if (datum1 == 0 || datum2 == 0 || datum1 != datum2)
			return c;
	}
	return -1;
}

index algorithms::string_compare_until(const wchar* first, const wchar* second)
{
	if (first == second)
		return string_length(first);
	if ((first == null) || (second == null))
		return 0;

	index c = 0;
	while (first[c] != 0 && second[c] != 0 && first[c] == second[c])
		c++;
	return c;
}

index algorithms::string_compare_until(const wchar* first, const char* second)
{
	if ((first == null) || (second == null))
		return 0;

	index c = 0;
	while (first[c] != 0 && second[c] != 0 && first[c] == second[c])
		c++;
	return c;
}

index algorithms::string_compare_until(const wchar* first, const mchar* second)
{
	if ((first == null) || (second == null))
		return 0;
	
	index c = 0, c2 = 0;
	mbyte datum1, datum2;
	while (true) {
		datum1.convert(first[c++]);
		if (datum1.size() == 1)
			datum2[0] = second[c2++];
		else if (datum1.size() == 2)
		{
			datum2[0] = second[c2++];
			if (second[c2] == 0)
				return c;
			datum2[1] = second[c2++];
		}
		else if (datum1.size() == 3)
		{
			datum2[0] = second[c2++];
			if (second[c2] == 0)
				return c;
			datum2[1] = second[c2++];
			if (second[c2] == 0)
				return c;
			datum2[2] = second[c2++];
		}
		else if (datum1.size() == 4)
		{
			datum2[0] = second[c2++];
			if (second[c2] == 0)
				return c;
			datum2[1] = second[c2++];
			if (second[c2] == 0)
				return c;
			datum2[2] = second[c2++];
			if (second[c2] == 0)
				return c;
			datum2[3] = second[c2++];
		}

		if (datum1 == 0 || datum2 == 0 || datum1 != datum2)
			return c;
	}
	return -1;
}


index algorithms::string_compare_until(const mchar* first, const mchar* second)
{
	if (first == second)
		return string_length(first);
	if ((first == null) || (second == null))
		return 0;

	index c = 0;
	while (first[c] != 0 && second[c] != 0 && first[c] == second[c])
		c++;
	return c;
}

index algorithms::string_compare_until(const mchar* first, const char* second)
{
	if ((first == null) || (second == null))
		return 0;

	index c = 0, c2 = 0;
	mbyte datum1, datum2;
	while (true) {
		datum2.convert(second[c2++]);
		if (datum2.size() == 1)
			datum1[0] = first[c++];
		else
		{
			datum1[0] = first[c++];
			if (first[c] == 0)
				return c;
			datum1[1] = first[c++];
		}

		if (datum1 == 0 || datum2 == 0 || datum1 != datum2)
			return c;
	}
	return -1;
}

index algorithms::string_compare_until(const mchar* first, const wchar* second)
{
	if ((first == null) || (second == null))
		return 0;

	index c = 0, c2 = 0;
	mbyte datum1, datum2;
	while (true) {
		datum2.convert(second[c2++]);
		if (datum2.size() == 1)
			datum1[0] = first[c++];
		else if (datum2.size() == 2)
		{
			datum1[0] = first[c++];
			if (first[c] == 0)
				return c;
			datum1[1] = first[c++];
		}
		else if (datum2.size() == 3)
		{
			datum1[0] = first[c++];
			if (first[c] == 0)
				return c;
			datum1[1] = first[c++];
			if (first[c] == 0)
				return c;
			datum1[2] = first[c++];
		}
		else if (datum2.size() == 4)
		{
			datum1[0] = first[c++];
			if (first[c] == 0)
				return c;
			datum1[1] = first[c++];
			if (first[c] == 0)
				return c;
			datum1[2] = first[c++];
			if (first[c] == 0)
				return c;
			datum1[3] = first[c++];
		}

		if (datum1 == 0 || datum2 == 0 || datum1 != datum2)
			return c;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////////////////

wstring ang::operator + (wstring const& first, wstring const& second)
{
	wstring str = new strings::wstring_buffer(first);
	str->concat(second);
	return str;
}

wstring ang::operator + (wstring const& first, string const& second)
{
	wstring str = new strings::wstring_buffer(first);
	str->concat(second);
	return str;
}

wstring ang::operator + (wstring const& first, mstring const& second)
{
	wstring str = new strings::wstring_buffer(first);
	str->concat(second);
	return str;
}

wstring ang::operator + (wstring const& first, cstr_t second)
{
	wstring str = new strings::wstring_buffer(first);
	str->concat(second);
	return str;
}

wstring ang::operator + (cstr_t first, wstring const& second)
{
	wstring str = new strings::wstring_buffer(first);
	str->concat(second);
	return str;
}

wstring ang::operator + (wstring const& first, cwstr_t second)
{
	wstring str = new strings::wstring_buffer(first);
	str->concat(second);
	return str;
}

wstring ang::operator + (cwstr_t first, wstring const& second)
{
	wstring str = new strings::wstring_buffer(first);
	str->concat(second);
	return str;
}

//////////////////////////////////////////////////////////////////////////

string ang::operator + (string const& first, string const& second)
{
	string str = new strings::string_buffer(first);
	str->concat(second);
	return str;
}

string ang::operator + (string const& first, wstring const& second)
{
	string str = new strings::string_buffer(first);
	str->concat(second);
	return str;
}

string ang::operator + (string const& first, mstring const& second)
{
	string str = new strings::string_buffer(first);
	str->concat(second);
	return str;
}

string ang::operator + (string const& first, cstr_t second)
{
	string str = new strings::string_buffer(first);
	str->concat(second);
	return str;
}

string ang::operator + (cstr_t first, string const& second)
{
	string str = new strings::string_buffer(first);
	str->concat(second);
	return str;
}

string ang::operator + (string const& first, cwstr_t second)
{
	string str = new strings::string_buffer(first);
	str->concat(second);
	return str;
}

string ang::operator + (cwstr_t first, string const& second)
{
	string str = new strings::string_buffer(first);
	str->concat(second);
	return str;
}

//////////////////////////////////////////////////////////////////////////

mstring ang::operator + (mstring const& first, mstring const& second)
{
	mstring str = new strings::mstring_buffer(first);
	str->concat(second);
	return str;
}

mstring ang::operator + (mstring const& first, wstring const& second)
{
	mstring str = new strings::mstring_buffer(first);
	str->concat(second);
	return str;
}

mstring ang::operator + (mstring const& first, string const& second)
{
	mstring str = new strings::mstring_buffer(first);
	str->concat(second);
	return str;
}

mstring ang::operator + (mstring const& first, cstr_t second)
{
	mstring str = new strings::mstring_buffer(first);
	str->concat(second);
	return str;
}

mstring ang::operator + (cstr_t first, mstring const& second)
{
	mstring str = new strings::mstring_buffer(first);
	str->concat(second);
	return str;
}

mstring ang::operator + (mstring const& first, cwstr_t second)
{
	mstring str = new strings::mstring_buffer(first);
	str->concat(second);
	return str;
}

mstring ang::operator + (cwstr_t first, mstring const& second)
{
	mstring str = new strings::mstring_buffer(first);
	str->concat(second);
	return str;
}


//////////////////////////////////////////////////////////////////////


uint cwstr_t::find(cwstr_t str, uint beg)const
{
	return string_find(cstr(), size(), str.cstr(), str.size(), beg, false);
}

uint cwstr_t::find(cwstr_t str,uint beg, uint end)const
{
	return string_find(cstr(), min(size(), end), str.cstr(), str.size(), beg, false);
}

uint cwstr_t::find(cstr_t str, uint beg)const
{
	return string_find(cstr(), size(), str.cstr(), str.size(), beg, false);
}

uint cwstr_t::find(cstr_t str, uint beg, uint end)const
{
	return string_find(cstr(), min(size(), end), str.cstr(), str.size(), beg, false);
}

uint cwstr_t::find(wchar c, index beg)const {
	wchar str[] = { c,0 };
	return string_find(cstr(), size(), str, 1U, beg, false);
}

uint cwstr_t::find(wchar c, index beg, index end)const {
	wchar str[] = { c,0 };
	return string_find(cstr(), min(size(), end), str, 1U, beg, false);
}

uint cwstr_t::sub_string(string& out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;

	if (out.is_empty())
		out = new string_buffer();

	if (!out->realloc(count, false))
		return 0;
	string_buffer* buffer = out.get();
	buffer->length(string_substr(cstr(), size(), buffer->str().str(), start, count));
	return buffer->length();
}

uint cwstr_t::sub_string(char* out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;
	return string_substr(cstr(), size(), out, start, count);
}

uint cwstr_t::sub_string(wstring& out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;

	if (out.is_empty())
		out = new wstring_buffer();

	if (!out->realloc(count, false))
		return 0;
	wstring_buffer* buffer = out.get();
	buffer->length(string_substr(cstr(), size(), buffer->str().str(), start, count));
	return buffer->length();
}

uint cwstr_t::sub_string(wchar* out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;
	return string_substr(cstr(), size(), out, start, count);
}

////////////////////////////////////////////////////////////////////

uint cstr_t::find(cwstr_t str,uint beg)const
{
	return string_find(cstr(), size(), str.cstr(), str.size(), beg, false);
}

uint cstr_t::find(cwstr_t str, uint beg, uint end)const
{
	return string_find(cstr(), min(size(), end), str.cstr(), str.size(), beg, false);
}

uint cstr_t::find(cstr_t str, uint beg)const
{
	return string_find(cstr(), size(), str.cstr(), str.size(), beg, false);
}

uint cstr_t::find(cstr_t str, uint beg, uint end)const
{
	return string_find(cstr(), min(size(), end), str.cstr(), str.size(), beg, false);
}

uint cstr_t::find(char c, uint beg)const {
	char str[] = { c,0 };
	return string_find(cstr(), size(), str, 1U, beg, false);
}

uint cstr_t::find(char c, uint beg, uint end)const{
	char str[] = { c,0 };
	return string_find(cstr(), min(size(), end), str, 1U, beg, false);
}

uint cstr_t::sub_string(string& out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;

	if (out.is_empty())
		out = new string_buffer();

	if (!out->realloc(count, false))
		return 0;
	string_buffer* buffer = out.get();
	buffer->length(string_substr(cstr(), size(), buffer->str().str(), start, count));
	return buffer->length();
}

uint cstr_t::sub_string(char* out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;
	return string_substr(cstr(), size(), out, start, count);
}

uint cstr_t::sub_string(wstring& out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;

	if (out.is_empty())
		out = new wstring_buffer();

	if (!out->realloc(count, false))
		return 0;
	wstring_buffer* buffer = out.get();
	buffer->length(string_substr(cstr(), size(), buffer->str().str(), start, count));
	return buffer->length();
}

uint cstr_t::sub_string(wchar* out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;
	return string_substr(cstr(), size(), out, start, count);
}


////////////////////////////////////////////////////////////////////

uint cmstr_t::find(cwstr_t str, uint beg, uint* plenght)const
{
	return string_find(cstr(), size(), str.cstr(), str.size(), beg, false, plenght);
}

uint cmstr_t::find(cwstr_t str, uint beg, uint end, uint* plenght)const
{
	return string_find(cstr(), min(size(), end), str.cstr(), str.size(), beg, false, plenght);
}

uint cmstr_t::find(cstr_t str, uint beg, uint* plenght)const
{
	return string_find(cstr(), size(), str.cstr(), str.size(), beg, false, plenght);
}

uint cmstr_t::find(cstr_t str, uint beg, uint end, uint* plenght)const
{
	return string_find(cstr(), min(size(), end), str.cstr(), str.size(), beg, false, plenght);
}

uint cmstr_t::find(cmstr_t str, uint beg)const
{
	return string_find(cstr(), size(), str.cstr(), str.size(), beg, false);
}

uint cmstr_t::find(cmstr_t str, uint beg, uint end)const
{
	return string_find(cstr(), min(size(), end), str.cstr(), str.size(), beg, false);
}

uint cmstr_t::sub_string(string& out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;

	if (out.is_empty())
		out = new string_buffer();

	if (!out->realloc(count, false))
		return 0;
	string_buffer* buffer = out.get();
	buffer->length(string_substr(cstr(), size(), buffer->str().str(), start, count));
	return buffer->length();
}

uint cmstr_t::sub_string(char* out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;
	return string_substr(cstr(), size(), out, start, count);
}

uint cmstr_t::sub_string(wstring& out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;

	if (out.is_empty())
		out = new wstring_buffer();

	if (!out->realloc(count, false))
		return 0;
	wstring_buffer* buffer = out.get();
	buffer->length(string_substr(cstr(), size(), buffer->str().str(), start, count));
	return buffer->length();
}

uint cmstr_t::sub_string(wchar* out, uint start, uint count)const
{
	if (cstr() == null || size() == 0)
		return 0;
	return string_substr(cstr(), size(), out, start, count);
}
