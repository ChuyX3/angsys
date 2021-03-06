/*********************************************************************************************************************/
/*   File Name: xml_value.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/xml.h"
#include "xml_entity_values.h"

#if defined _DEBUG
#define NEW ANG_DEBUG_NEW()
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::xml;

xml_entity_values_t xml::xml_entity_values[5] = {
	{ xml_entity::amp, "&amp;", "&" },
	{ xml_entity::lt, "&lt;", "<" },
	{ xml_entity::gt, "&gt;", ">" },
	{ xml_entity::quot, "&quot;", "\"" },
	{ xml_entity::apos, "&apos;", "'" }
};

xml_value::xml_value()
	: wstring()
{

}

xml_value::xml_value(strings::wstring_buffer* ptr)
	: wstring(ptr)
{
}

xml_value::xml_value(ang::nullptr_t const&)
	: wstring(null)
{
}

xml_value::xml_value(cstr_t cs)
	: wstring(cs)
{
}

xml_value::xml_value(cwstr_t cs)
	: wstring(cs)
{
}

xml_value::xml_value(cmstr_t cs)
	: wstring(cs)
{
}

xml_value::xml_value(wstring && other)
	: wstring(other)
{
}

xml_value::xml_value(wstring const& other)
	: wstring(other)
{
}

xml_value::xml_value(string const& other)
	: wstring(other)
{
}

xml_value::xml_value(mstring const& other)
	: wstring(other)
{
}


xml_value_t& xml_value_t::operator = (wstring && other)
{
	wstring::operator = (other);
	return *this;
}

xml_value_t& xml_value_t::operator = (wstring const& other)
{
	wstring::operator = (other);
	return *this;
}

xml_value_t& xml_value_t::operator = (string const& other)
{
	wstring::operator = (other);
	return *this;
}

xml_value_t& xml_value_t::operator = (mstring const& other)
{
	wstring::operator = (other);
	return *this;
}

xml_value_t& xml_value_t::operator = (strings::wstring_buffer* ptr)
{
	wstring::operator = (ptr);
	return *this;
}

xml_value_t& xml_value_t::operator = (ang::nullptr_t const&)
{
	wstring::operator = (null);
	return *this;
}

xml_value_t& xml_value_t::operator = (cstr_t cs)
{
	wstring::operator = (cs);
	return *this;
}

xml_value_t& xml_value_t::operator = (cwstr_t cs)
{
	wstring::operator = (cs);
	return *this;
}

xml_value_t& xml_value_t::operator = (cmstr_t cs)
{
	wstring::operator = (cs);
	return *this;
}

xml_value_t& xml_value_t::operator += (wstring const& other)
{
	wstring::operator += (other);
	return *this;
}

xml_value_t& xml_value_t::operator += (cstr_t cs)
{
	wstring::operator += (cs);
	return *this;
}

xml_value_t& xml_value_t::operator += (cwstr_t cs)
{
	wstring::operator += (cs);
	return *this;
}

xml_value_t& xml_value_t::operator += (cmstr_t cs)
{
	wstring::operator += (cs);
	return *this;
}

wstring& xml_value::xml_print(wstring& out, xml_format_t const& format, ushort)const
{
	if (is_empty())
		return out;

	if (format.is_active(xml_format::fix_entity))
	{
		wstring str = *this; //make a copy
		cwstr_t cstr = str;
		index idx = 0;
		while (cstr[idx] != 0 && idx < cstr.size())
		{
			for (int i = 0; i < 5; i++)
			{
				if (xml_entity_values[i].code[0] == cstr[idx])
				{
					str->replace(xml_entity_values[i].code, str->at(idx), str->at(idx + 1));
					cstr = str;
					break;
				}
			}
			idx++;
		}
		out << str;
	}
	else
	{
		out << *this;
	}
	return out;
}


template<> cstr_t xml::xml_value::as<cstr_t>()const { return null; }
template<> cwstr_t xml::xml_value::as<cwstr_t>()const { return *this; }
template<> string xml::xml_value::as<string>()const { return *this; } //make a copy
template<> wstring xml::xml_value::as<wstring>()const { return *this; } //make a copy
template<> mstring xml::xml_value::as<mstring>()const { return *this; } //make a copy

template<> short xml::xml_value::as<short>()const { return interger::parse(as<cwstr_t>()); }
template<> ushort xml::xml_value::as<ushort>()const { return uinterger::parse(as<cwstr_t>()); }
template<> int xml::xml_value::as<int>()const { return interger::parse(as<cwstr_t>()); }
template<> uint xml::xml_value::as<uint>()const { return uinterger::parse(as<cwstr_t>()); }
template<> long xml::xml_value::as<long>()const { return interger::parse(as<cwstr_t>()); }
template<> ulong xml::xml_value::as<ulong>()const { return uinterger::parse(as<cwstr_t>()); }
template<> long64 xml::xml_value::as<long64>()const { return interger64::parse(as<cwstr_t>()); }
template<> ulong64 xml::xml_value::as<ulong64>()const { return uinterger64::parse(as<cwstr_t>()); }
template<> float xml::xml_value::as<float>()const { return floating::parse(as<cwstr_t>()); }
template<> double xml::xml_value::as<double>()const { return floating64::parse(as<cwstr_t>()); }
template<> bool xml::xml_value::as<bool>()const { return boolean::parse(as<cwstr_t>()); }

template<> xml::xml_encoding_t xml::xml_value::as<xml::xml_encoding_t>()const
{
	cwstr_t type = as<cwstr_t>();
	if (!type.cstr())
		return  xml::xml_encoding::unknown;
	if (type == L"utf-8" || type == L"UTF-8")
		return xml::xml_encoding::utf_8;
	else if (type == L"utf-16" || type == L"UTF-16"
		|| type == L"iso-10646" || type == L"ISO-10646" 
		|| type == L"unicode" || type == L"UNICODE")
		return xml::xml_encoding::unicode;
	else if (type == L"iso-8859-1" || type == L"ISO-8859-1"
		|| type == L"ascii" || type == L"ASCII")
		return xml::xml_encoding::ascii;
	return  xml::xml_encoding::unknown;
}