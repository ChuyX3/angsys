/*********************************************************************************************************************/
/*   File Name: boolean.cpp                                                                                          */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.hpp"
#include "format_parser.h"

using namespace ang;

collections::pair<cstr_t, cstr_t> _parse_boolean_map[] = {
	"TRUE"_s,
	"True"_s,
	"true"_s,
	"YES"_s,
	"Yes"_s,
	"yes"_s,
};

value<bool> boolean::parse(cstr_t cstr)
{
	wsize idx = collections::algorithms::binary_search(cstr, to_array(_parse_boolean_map));
	if (idx > array_size(_parse_boolean_map))
		return false;
	return true;
}

value<bool> boolean::parse(cwstr_t cstr)
{
	wsize idx = collections::algorithms::binary_search(cstr, to_array(_parse_boolean_map));
	if (idx > array_size(_parse_boolean_map))
		return false;
	return true;
}

string boolean::to_string(value<bool> value)
{
	return value.get() ? "true"_s : "false"_s;
}

wstring boolean::to_wstring(value<bool> value)
{
	return value.get() ? L"true"_s : L"false"_s;
}

boolean::value_wrapper()
	: value<bool>()
{

}

boolean::value_wrapper(bool val)
	: value<bool>(val)
{

}

boolean::value_wrapper(boolean const& other)
	: value<bool>(other)
{

}

boolean::value_wrapper(value<bool> const& other)
	: value<bool>(other)
{

}

boolean::~value_wrapper()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::boolean);
ANG_IMPLEMENT_OBJECTNAME(ang::boolean);

bool boolean::is_kind_of(type_name_t type)const
{
	return (type == type_of<boolean>())
		|| object::is_kind_of(type)
		|| (type == type_of<value<bool>>());
}

bool boolean::is_inherited_of(type_name_t type)
{
	return (type == type_of<boolean>())
		|| object::is_inherited_of(type)
		|| (type == type_of<value<bool>>());
}

bool boolean::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<boolean>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<value<bool>>())
	{
		*out = static_cast<value<bool>*>(this);
		return true;
	}
	return false;
}

string boolean::to_string()const
{
	return ang::move(to_string(*this));
}