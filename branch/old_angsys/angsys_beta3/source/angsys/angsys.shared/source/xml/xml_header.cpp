/*********************************************************************************************************************/
/*   File Name: xml_element.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <ang/dom/xml.h>

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::dom::xml;

#define MY_TYPE ang::dom::xml::xml_header
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xml_header_t xml_header::create_new(xml_document_t doc) {
	xml_header_t header = NEW xml_header(doc);
	header->version(doc->create_pcdata("1.0"_s));
	header->encoding(doc->xml_encoding());
	header->is_stand_alone(true);
	return header;
}

xml_header_t xml_header::create_new(xml_document_t doc, const xml_header* head) {
	xml_header_t header = NEW xml_header(doc);
	if (head) {
		header->version((xml_cstr_t)head->xml_version());
		header->encoding(head->xml_encoding());
		header->is_stand_alone(head->xml_stand_alone());
	}
	else
	{
		header->version(doc->create_pcdata("1.0"_s));
		header->encoding(xml_encoding::utf8);
		header->is_stand_alone(true);
	}
	return header;
}

xml_header_t xml_header::create_new(xml_document_t doc, xml_cstr_t version, bool standalone) {
	xml_header_t header = NEW xml_header(doc);
	header->version(doc->create_pcdata(version));
	header->encoding(doc->xml_encoding());
	header->is_stand_alone(standalone);
	return header;
}

xml_header::xml_header(xml_document_t doc)
	: xml_node(doc, xml_type::header)
{
}

xml_header::~xml_header()
{
	clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_header);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_header, xml_node, ixml_header);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_header, xml_node, ixml_header);

xml_node_t xml_header::xml_clone(xml_document_t doc)const
{
	return create_new(doc, this);
}


ixml_text_view_t xml_header::xml_version()const
{
	xml_text_t version = m_attributes["version"_s];
	return version;
}

xml_encoding_t xml_header::xml_encoding()const
{
	xml_text_t encoding = m_attributes["encoding"_s];
	return encoding->xml_as<xml_encoding_t>();
}

bool xml_header::xml_stand_alone()const
{
	xml_text_t standalone = m_attributes["standalone"_s];
	return standalone->xml_as<bool>();
}

void xml_header::version(ixml_text_t value)
{
	xml_document_t doc = xml_parent_doc();
	auto it = m_attributes->find("version"_s);
	if (it.is_valid())
	{
		it->push_value(value);
	}
	push_attribute(xml_attribute::create_new(doc, doc->create_pcdata("version"_s), value));
}

void xml_header::encoding(xml_encoding_t value)
{
	xml_document_t doc = xml_parent_doc();
	auto it = m_attributes->find("encoding"_s);
	if (it.is_valid())
	{
		it->push_value(doc->create_pcdata((xml_cstr_t)value.to_string()));
	}
	push_attribute(xml_attribute::create_new(doc, doc->create_pcdata("encoding"_s), doc->create_pcdata((xml_cstr_t)value.to_string())));
}

void xml_header::encoding(ixml_text_t value)
{
	xml_document_t doc = xml_parent_doc();
	auto it = m_attributes->find("encoding"_s);
	if (it.is_valid())
	{
		it->push_value(value);
	}
	push_attribute(xml_attribute::create_new(doc, doc->create_pcdata("encoding"_s), value));
}

void xml_header::is_stand_alone(bool value)
{
	xml_document_t doc = xml_parent_doc();
	auto it = m_attributes->find("standalone"_s);
	if (it.is_valid())
	{
		it->push_value(doc->create_pcdata(value ? "true"_s : "false"_s));
	}
	push_attribute(xml_attribute::create_new(doc, doc->create_pcdata("standalone"_s), doc->create_pcdata(value ? "true"_s : "false"_s)));
}
