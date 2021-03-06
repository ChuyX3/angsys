/*********************************************************************************************************************/
/*   File Name: xml_text.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/dom/xml.h"
#include "xml/xml_internal.h"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::dom;
using namespace ang::dom::xml;

#define MY_TYPE ang::dom::xml::xml_text
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

xml_entity_values_t xml::xml_entity_values[5] = {
	{ xml_entity::amp, "&amp;"_s, "&"_s },
	{ xml_entity::lt, "&lt;"_s, "<"_s },
	{ xml_entity::gt, "&gt;"_s, ">" },
	{ xml_entity::quot, "&quot;"_s, "\""_s },
	{ xml_entity::apos, "&apos;"_s, "'"_s }
};

/////////////////////////////////////////////////////////////////////////
template<> cstr_t xml::xml_text::xml_as<cstr_t>()const { return this ? this->cstr() : cstr_t(); }
template<> astring xml::xml_text::xml_as<astring>()const { return this ? this->cstr() : astring(null); } //make a copy
template<> wstring xml::xml_text::xml_as<wstring>()const { return this ? this->cstr() : wstring(null); } //make a copy
template<> mstring xml::xml_text::xml_as<mstring>()const { return this ? this->cstr() : mstring(null); } //make a copy

template<> short xml::xml_text::xml_as<short>()const { return variable<int>::parse(xml_as<cstr_t>()); }
template<> ushort xml::xml_text::xml_as<ushort>()const { return variable<uint>::parse(xml_as<cstr_t>()); }
template<> int xml::xml_text::xml_as<int>()const { return variable<int>::parse(xml_as<cstr_t>()); }
template<> uint xml::xml_text::xml_as<uint>()const { return variable<uint>::parse(xml_as<cstr_t>()); }
template<> long xml::xml_text::xml_as<long>()const { return variable<long>::parse(xml_as<cstr_t>()); }
template<> ulong xml::xml_text::xml_as<ulong>()const { return variable<ulong>::parse(xml_as<cstr_t>()); }
template<> long64 xml::xml_text::xml_as<long64>()const { return variable<long64>::parse(xml_as<cstr_t>()); }
template<> ulong64 xml::xml_text::xml_as<ulong64>()const { return variable<ulong64>::parse(xml_as<cstr_t>()); }
template<> float xml::xml_text::xml_as<float>()const { return variable<float>::parse(xml_as<cstr_t>()); }
template<> double xml::xml_text::xml_as<double>()const { return variable<double>::parse(xml_as<cstr_t>()); }
template<> bool xml::xml_text::xml_as<bool>()const { return variable<bool>::parse(xml_as<cstr_t>()); }

template<> xml::xml_encoding_t xml::xml_text::xml_as<xml::xml_encoding_t>()const { 
	return xml::xml_encoding_t::parse(xml_as<cstr_t>());
}


/////////////////////////////////////////////////////////////////////////

xml_text::xml_text(xml_document* doc)
	: m_doc(doc)
	, m_value(null)
{
	m_value = doc->create_cdata(""_s);
}

xml_text::xml_text(xml_document* doc, string value)
	: m_doc(doc)
	, m_value(null)
{
	m_value = value.is_empty() ? doc->create_cdata(""_s) : value;
}

xml_text::~xml_text()
{
	m_value = null;
	m_doc = null;
}

void xml_text::clear()
{
	m_value->clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_text);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_text, object, ixml_object, text::istring);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_text, object, ixml_object, text::istring, text::istring_view);

xml_type_t xml_text::xml_type()const
{
	return xml_type::text;
}

bool xml_text::xml_is_type_of(xml_type_t type)const
{
	return type == xml_type::text;
}

streams::itext_output_stream_t& xml_text::xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level)const
{
	if (m_value.is_empty())
		return stream;

	if (false) //TODO: flag.is_active<xml_format::fix_entity>())
	{
		wstring str = this->cstr(); //make a copy
		cwstr_t cstr = str;
		index idx = 0;
		while (cstr[idx] != 0 && idx < cstr.size())
		{
			for (int i = 0; i < 5; i++)
			{
				if (xml_entity_values[i].code[0] == cstr[idx])
				{
					//TODO: str->replace(xml_entity_values[i].code, str->at(idx), str->at(idx + 1));
					cstr = str;
					break;
				}
			}
			idx++;
		}
		stream << str;
	}
	else
	{
		stream << this->cstr();
	}
	
	return stream;
}

xml_encoding_t xml_text::encoding()const
{
	return m_value->encoding();
}

wsize xml_text::length()const
{
	return m_value->length();
}

pointer xml_text::data()const
{
	return m_value->data();
}

rtti_t const& xml_text::char_type()const
{
	return m_value->char_type();
}

str_t xml_text::str(int)
{
	return m_value->str();
}

cstr_t xml_text::cstr(int)const
{
	return m_value->cstr();
}

char32_t xml_text::at(windex i, wsize* sz)const
{
	return m_value->at(i, sz);
}

int xml_text::compare(cstr_t cstr)const
{
	return m_value->compare(cstr);
}

windex xml_text::compare_until(cstr_t cstr)const
{
	return m_value->compare_until(cstr);
}

windex xml_text::find(cstr_t cstr, windex start, windex end)const
{
	return m_value->find(cstr, start, end);
}

windex xml_text::find_reverse(cstr_t cstr, windex start, windex end)const
{
	return m_value->find_reverse(cstr, start, end);
}

string xml_text::sub_string(string_ptr_t out, windex start, windex end)const
{
	return m_value->sub_string(out, start, end);
}

collections::ienum_ptr<string> xml_text::split(cstr_t cstr)const
{
	return m_value->split(cstr);
}

void xml_text::copy(cstr_t cstr)
{
	copy_cdata(cstr);
}

void xml_text::move(string str)
{
	if(!str.is_empty())
		m_value = str;
}

void xml_text::copy_cdata(cstr_t cstr)
{
	m_value->copy(cstr);
}

void xml_text::copy_pcdata(cstr_t cstr)
{
	m_value->copy(cstr); //TODO:
}

void xml_text::concat(cstr_t cstr)
{
	m_value->concat(cstr);
}

void xml_text::insert(windex i, cstr_t cstr)
{
	m_value->insert(i, cstr);
}

void xml_text::format(cstr_t cstr, var_args_t args)
{
	m_value->format(cstr, args);
}

void xml_text::concat_format(cstr_t cstr, var_args_t args)
{
	m_value->concat_format(cstr, args);
}

void xml_text::insert_format(windex i, cstr_t cstr, var_args_t args)
{
	m_value->insert_format(i, cstr, args);
}

