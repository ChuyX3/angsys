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
#include "xml_utils.h"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::xml;

xml_entity_values_t xml::xml_entity_values[5] = {
	{ xml_entity::amp, "&amp;"_sm, "&"_sm },
	{ xml_entity::lt, "&lt;"_sm, "<"_sm },
	{ xml_entity::gt, "&gt;"_sm, ">"_sm },
	{ xml_entity::quot, "&quot;"_sm, "\""_sm },
	{ xml_entity::apos, "&apos;"_sm, "'"_sm }
};

xml_value::xml_value()
	: mstring()
{

}

xml_value::xml_value(strings::mstring_buffer* ptr)
	: mstring(ptr)
{
}

xml_value::xml_value(ang::nullptr_t const&)
	: mstring(null)
{
}

xml_value::xml_value(cstr_t cs)
	: mstring(cs)
{
}

xml_value::xml_value(cwstr_t cs)
	: mstring(cs)
{
}

xml_value::xml_value(cmstr_t cs)
	: mstring(cs)
{
}

xml_value::xml_value(mstring && other)
	: mstring(other)
{
}

xml_value::xml_value(mstring const& other)
	: mstring(other)
{
}

xml_value::xml_value(string const& other)
	: mstring(other)
{
}

xml_value::xml_value(wstring const& other)
	: mstring(other)
{
}


xml_value_t& xml_value_t::operator = (mstring && other)
{
	mstring::operator = (other);
	return *this;
}

xml_value_t& xml_value_t::operator = (mstring const& other)
{
	mstring::operator = (other);
	return *this;
}

xml_value_t& xml_value_t::operator = (string const& other)
{
	mstring::operator = (other);
	return *this;
}

xml_value_t& xml_value_t::operator = (wstring const& other)
{
	mstring::operator = (other);
	return *this;
}

xml_value_t& xml_value_t::operator = (strings::mstring_buffer* ptr)
{
	mstring::operator = (ptr);
	return *this;
}

xml_value_t& xml_value_t::operator = (ang::nullptr_t const&)
{
	mstring::operator = (null);
	return *this;
}

xml_value_t& xml_value_t::operator = (cstr_t cs)
{
	mstring::operator = (cs);
	return *this;
}

xml_value_t& xml_value_t::operator = (cwstr_t cs)
{
	mstring::operator = (cs);
	return *this;
}

xml_value_t& xml_value_t::operator = (cmstr_t cs)
{
	mstring::operator = (cs);
	return *this;
}

xml_value_t& xml_value_t::operator += (mstring const& other)
{
	mstring::operator += (other);
	return *this;
}

xml_value_t& xml_value_t::operator += (cstr_t cs)
{
	mstring::operator += (cs);
	return *this;
}

xml_value_t& xml_value_t::operator += (cwstr_t cs)
{
	mstring::operator += (cs);
	return *this;
}

xml_value_t& xml_value_t::operator += (cmstr_t cs)
{
	mstring::operator += (cs);
	return *this;
}

mstring& xml_value::xml_print(mstring& out, xml_format_t const& format, ushort)const
{
	if (is_empty())
		return out;

	if (format.is_active(xml_format::fix_entity))
	{
		mstring str = *this; //make a copy
		cmstr_t cstr = str;
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


template<> cstr_t xml::xml_value::as<cstr_t>()const { 
	return reinterpret_cast<string const&>(*this); //force cast
}
template<> cmstr_t xml::xml_value::as<cmstr_t>()const { return *this; }
template<> cwstr_t xml::xml_value::as<cwstr_t>()const { return null; }
template<> string xml::xml_value::as<string>()const { return *this; } //make a copy
template<> mstring xml::xml_value::as<mstring>()const { return *this; } //make a copy
template<> wstring xml::xml_value::as<wstring>()const { return *this; } //make a copy

//template<> short xml::xml_value::as<short>()const { return interger::parse(as<cstr_t>()); }
//template<> ushort xml::xml_value::as<ushort>()const { return uinterger::parse(as<cstr_t>()); }
//template<> int xml::xml_value::as<int>()const { return interger::parse(as<cstr_t>()); }
//template<> uint xml::xml_value::as<uint>()const { return uinterger::parse(as<cstr_t>()); }
//template<> long xml::xml_value::as<long>()const { return interger::parse(as<cstr_t>()); }
//template<> ulong xml::xml_value::as<ulong>()const { return uinterger::parse(as<cstr_t>()); }
//template<> long64 xml::xml_value::as<long64>()const { return interger64::parse(as<cstr_t>()); }
//template<> ulong64 xml::xml_value::as<ulong64>()const { return uinterger64::parse(as<cstr_t>()); }
//template<> float xml::xml_value::as<float>()const { return floating::parse(as<cstr_t>()); }
//template<> double xml::xml_value::as<double>()const { return floating64::parse(as<cstr_t>()); }
//template<> bool xml::xml_value::as<bool>()const { return boolean::parse(as<cstr_t>()); }

template<> xml::xml_encoding_t xml::xml_value::as<xml::xml_encoding_t>()const
{
	cstr_t type = as<cstr_t>();
	if (!type.cstr())
		return  xml::xml_encoding::unknown;
	if (type == "utf-8"_sm || type == "UTF-8"_sm)
		return xml::xml_encoding::utf_8;
	else if (type == "utf-16"_sm || type == "UTF-16"_sm
		|| type == "iso-10646"_sm || type == "ISO-10646"_sm
		|| type == "unicode"_sm || type == "UNICODE"_sm)
		return xml::xml_encoding::unicode;
	else if (type == "iso-8859-1"_sm || type == "ISO-8859-1"_sm
		|| type == "ascii"_sm || type == "ASCII"_sm)
		return xml::xml_encoding::ascii;
	return  xml::xml_encoding::unknown;
}