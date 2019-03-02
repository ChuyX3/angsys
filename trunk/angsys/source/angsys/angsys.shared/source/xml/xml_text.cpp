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
using namespace ang::dom::xml;

#define MY_TYPE ang::dom::xml::xml_text
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

xml_entity_values_t xml_entity_values[5] = {
	{ xml_entity::amp, "&amp;"_s, "&"_s },
	{ xml_entity::lt, "&lt;"_s, "<"_s },
	{ xml_entity::gt, "&gt;"_s, ">" },
	{ xml_entity::quot, "&quot;"_s, "\""_s },
	{ xml_entity::apos, "&apos;"_s, "'"_s }
};

/////////////////////////////////////////////////////////////////////////

xml_text::xml_text(xml_document* doc)
	: m_doc(doc)
	, m_value(null)
{
	m_value = doc->create_cdata(""_s);
}

xml_text::xml_text(xml_document* doc, ixml_text_t value)
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
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_text, object, ixml_object, ixml_text);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_text, object, ixml_object, ixml_text, ixml_text_view);

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

xml_str_t xml_text::str(int)
{
	return m_value->str();
}

xml_cstr_t xml_text::cstr(int)const
{
	return m_value->cstr();
}

char32_t xml_text::at(windex i, wsize* sz)const
{
	return m_value->at(i, sz);
}

int xml_text::compare(xml_cstr_t cstr)const
{
	return m_value->compare(cstr);
}

windex xml_text::compare_until(xml_cstr_t cstr)const
{
	return m_value->compare_until(cstr);
}

windex xml_text::find(xml_cstr_t cstr, windex start, windex end)const
{
	return m_value->find(cstr, start, end);
}

windex xml_text::find_reverse(xml_cstr_t cstr, windex start, windex end)const
{
	return m_value->find_reverse(cstr, start, end);
}

ixml_text_t xml_text::sub_string(ixml_text_ptr_t out, windex start, windex end)const
{
	return m_value->sub_string(out, start, end);
}

void xml_text::copy(xml_cstr_t cstr)
{
	copy_cdata(cstr);
}

void xml_text::move(ixml_text_t str)
{
	if(!str.is_empty())
		m_value = str;
}

void xml_text::copy_cdata(xml_cstr_t cstr)
{
	m_value->copy(cstr);
}

void xml_text::copy_pcdata(xml_cstr_t cstr)
{
	m_value->copy(cstr); //TODO:
}

void xml_text::concat(xml_cstr_t cstr)
{
	m_value->concat(cstr);
}

void xml_text::insert(windex i, xml_cstr_t cstr)
{
	m_value->insert(i, cstr);
}

void xml_text::format(xml_cstr_t cstr, var_args_t args)
{
	m_value->format(cstr, args);
}

void xml_text::concat_format(xml_cstr_t cstr, var_args_t args)
{
	m_value->concat_format(cstr, args);
}

void xml_text::insert_format(windex i, xml_cstr_t cstr, var_args_t args)
{
	m_value->insert_format(i, cstr, args);
}

