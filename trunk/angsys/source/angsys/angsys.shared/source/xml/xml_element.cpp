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

#define MY_TYPE ang::dom::xml::xml_element
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xml_element_t xml_element::create_new(xml_document_t doc, const xml_element* element) {
	xml_element_t node = NEW xml_element(doc);
	if (element)
	{
		if (!element->m_name.is_empty())
			node->push_name(element->m_name);

		if (!element->m_content.is_empty() && element->m_content->xml_is_type_of(xml_type::text))
			node->push_value(element->m_content.get()->xml_as<xml_text>());
		else if (!element->m_content.is_empty() && element->m_content->xml_is_type_of(xml_type::cdata))
			node->push_data(element->m_content.get()->xml_as<xml_text>());
		else if (!element->m_content.is_empty() && element->m_content->xml_is_type_of(xml_type::element_list))
			node->push_children(element->m_content.get()->xml_as<ixml_collection>());
		if (element->xml_has_attributes())
			node->push_attributes(element->m_attributes.get());
	}
	return node;
}

xml_element_t xml_element::create_new(xml_document_t doc, string name) {
	xml_element_t node = NEW xml_element(doc);
	node->push_name(name);
	return node;
}

xml_element_t xml_element::create_new(xml_document_t doc, string name, string value) {
	xml_element_t node = NEW xml_element(doc);
	node->push_name(name);
	node->push_value(value);
	return node;
}


xml_element::xml_element(xml_document_t doc)
	: base(doc, xml_type::element)
{

}

xml_element::~xml_element()
{
	clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_element);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_element, xml_node);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_element, xml_node);

bool xml_element::xml_has_name()const
{
	return !m_name.is_empty();
}

bool xml_element::xml_has_namespace()const
{
	return !m_namespace.is_empty();
}

bool xml_element::xml_has_value()const
{
	return !m_content.is_empty() && m_content ->xml_is_type_of(xml_type::text);
}

bool xml_element::xml_has_children()const
{
	return !m_content.is_empty() && m_content->xml_is_type_of(xml_type::element_list);
}

bool xml_element::xml_has_attributes()const
{
	return !m_attributes.is_empty();
}

xml_node_t xml_element::xml_clone(xml_document_t doc)const
{
	return xml_element::create_new(doc, this);
}

/////////////////////////////////////////////////////////////////////////////////////////

