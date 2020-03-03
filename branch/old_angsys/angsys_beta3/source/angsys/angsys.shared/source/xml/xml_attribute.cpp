/*********************************************************************************************************************/
/*   File Name: xml_attribute.cpp                                                                                           */
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

#define MY_TYPE ang::dom::xml::xml_attribute
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE


#define MY_TYPE ang::dom::xml::xml_namespace
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


xml_attribute_t xml_attribute::create_new(xml_document_t doc, const xml_attribute* att) {
	xml_attribute_t node = NEW xml_attribute(doc);
	if (att)
	{
		if (!att->m_name.is_empty())
			node->push_name(doc->create_pcdata((cstr_t)att->m_name));

		if (!att->m_content.is_empty() && att->m_content->xml_is_type_of(xml_type::text))
			node->push_value(doc->create_pcdata((cstr_t)att->m_content->xml_as<xml_text>()));
	}
	return node;
}

xml_attribute_t xml_attribute::create_new(xml_document_t doc, string name, string value) {
	xml_attribute_t node = NEW xml_attribute(doc);
	node->push_name(name);
	node->push_value(value);
	return node;
}

xml_attribute::xml_attribute(xml_document_t doc)
	: base(doc, xml_type::attribute)
{

}

xml_attribute::~xml_attribute()
{
	clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_attribute);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_attribute, xml_node);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_attribute, xml_node);

bool xml_attribute::xml_has_name()const
{
	return !m_name.is_empty();
}

bool xml_attribute::xml_has_value()const
{
	return !m_content.is_empty() && m_content->xml_is_type_of(xml_type::text);
}

xml_node_t xml_attribute::xml_clone(xml_document_t doc)const
{
	return xml_attribute::create_new(doc, this);
}

////////////////////////////////////////////


xml_namespace_t xml_namespace::create_new(xml_document_t doc, const xml_namespace* att) {
	xml_namespace_t node = NEW xml_namespace(doc);
	if (att)
	{
		if (!att->m_name.is_empty())
			node->push_name(doc->create_pcdata((cstr_t)att->m_name));

		if (!att->m_content.is_empty() && att->m_content->xml_is_type_of(xml_type::text))
			node->push_value(doc->create_pcdata((cstr_t)att->m_content->xml_as<xml_text>()));
	}
	return node;
}

xml_namespace_t xml_namespace::create_new(xml_document_t doc, string name, string value) {
	xml_namespace_t node = NEW xml_namespace(doc);
	node->push_name(name);
	node->push_value(value);
	return node;
}

xml_namespace::xml_namespace(xml_document_t doc)
	: xml_node(doc, xml_type::name_space)
{

}

xml_namespace::~xml_namespace()
{
	clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_namespace);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_namespace, xml_node);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::dom::xml::xml_namespace, xml_node);

bool xml_namespace::xml_has_name()const
{
	return !m_name.is_empty();
}

bool xml_namespace::xml_has_value()const
{
	return m_content.is_empty() && m_content->xml_is_type_of(xml_type::text);
}

xml_node_t xml_namespace::xml_clone(xml_document_t doc)const
{
	return xml_namespace::create_new(doc, this);
}


////////////////////////////////////////////

xml_attributes::xml_attributes() : m_ptr(null) {

}

xml_attributes::xml_attributes(ixml_collection* ptr) : m_ptr(null) {
	set(ptr);
}

xml_attributes::xml_attributes(xml_attributes_t const& ptr) : m_ptr(null) {
	set(ptr.get());
}

xml_attributes::xml_attributes(xml_attributes_t && other) : m_ptr(null) {
	ixml_collection * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

xml_attributes::xml_attributes(std::nullptr_t const&)
	: m_ptr(null)
{
}

xml_attributes::~xml_attributes() {
	reset();
}

void xml_attributes::reset()
{
	iobject * _obj = interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

void xml_attributes::reset_unsafe()
{
	m_ptr = null;
}

bool xml_attributes::is_empty()const
{
	return m_ptr == null;
}

ixml_collection* xml_attributes::get(void)const
{
	return m_ptr;
}

void xml_attributes::set(ixml_collection* ptr)
{
	if (ptr == m_ptr) return;
	if (ptr != null && !ptr->xml_is_type_of(xml_type::attribute_list)) return reset();
	iobject * _old = interface_cast<iobject>(m_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

xml_attributes& xml_attributes::operator = (ixml_collection* ptr)
{
	set(ptr);
	return*this;
}

xml_attributes& xml_attributes::operator = (xml_attributes_t const& ptr)
{
	set(ptr.get());
	return*this;
}


xml_attributes& xml_attributes::operator = (xml_attributes_t && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

ixml_collection ** xml_attributes::addres_of(void)
{
	return &m_ptr;
}

//ang::ixml_attributes_ptr<ixml_collection> xml_attributes::operator & (void)
//{
//	return this;
//}

ixml_collection * xml_attributes::operator -> (void)
{
	return get();
}

ixml_collection const* xml_attributes::operator -> (void)const
{
	return get();
}

xml_attributes::operator ixml_collection * (void) { return m_ptr; }

xml_attributes::operator ixml_collection const* (void)const { return m_ptr; }

xml_text_t xml_attributes::operator[](cstr_t value)const
{
	xml_iterator_t it;
	if (m_ptr)
		it = m_ptr->find(value);
	return it.is_valid() ? static_cast<xml_text*>(it->xml_value().get()) : null;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
