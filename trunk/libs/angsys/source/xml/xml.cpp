
/*********************************************************************************************************************/
/*   File Name: xml.cpp                                                                                              */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                              */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "coffe.h"
#include "coffe/dom/xml.h"
#include "xml/xml_impl.h"

using namespace coffe;
using namespace coffe::dom;
using namespace coffe::dom::xml;

//COFFE_IMPLEMENT_INTERFACE_CLASS_INFO(coffe::dom::xml::ixml_enum);
//COFFE_IMPLEMENT_INTERFACE_CLASS_INFO(coffe::dom::xml::ixml_text);
//COFFE_IMPLEMENT_INTERFACE_CLASS_INFO(coffe::dom::xml::ixml_node);
//COFFE_IMPLEMENT_INTERFACE_CLASS_INFO(coffe::dom::xml::ixml_document);
//COFFE_IMPLEMENT_INTERFACE_CLASS_INFO(coffe::dom::xml::ixml_streamer);

#define MY_TYPE coffe::dom::xml::ixml_text
#include "coffe/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE coffe::dom::xml::ixml_node
#include "coffe/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE coffe::dom::xml::ixml_enum
#include "coffe/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE coffe::dom::xml::ixml_items
#include "coffe/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE coffe::dom::xml::ixml_document
#include "coffe/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE


COFFE_ENUM_IMPLEMENT(coffe::dom::xml, xml_type);

static collections::pair<coffe::dom::xml::xml_type, castr_t> s_xml_type_to_string_map[] =
{
	{ coffe::dom::xml::xml_type::abstract, "abstract"_sv },
	{ coffe::dom::xml::xml_type::document, "document"_sv },
	{ coffe::dom::xml::xml_type::text, "text"_sv },
	{ coffe::dom::xml::xml_type::cdata, "cdata"_sv },
	{ coffe::dom::xml::xml_type::node, "node"_sv },
	{ coffe::dom::xml::xml_type::element, "element"_sv },
	{ coffe::dom::xml::xml_type::comment, "comment"_sv },
	{ coffe::dom::xml::xml_type::attribute, "attribute"_sv },
	{ coffe::dom::xml::xml_type::element_list, "element_list"_sv },
	{ coffe::dom::xml::xml_type::attribute_list, "attribute_list"_sv },
};

static collections::pair<castr_t, coffe::dom::xml::xml_type> s_xml_type_parsing_map[] =
{
	{ "abstract"_sv, coffe::dom::xml::xml_type::abstract },
	{ "attribute"_sv, coffe::dom::xml::xml_type::attribute },
	{ "attribute_list"_sv, coffe::dom::xml::xml_type::attribute_list },
	{ "cdata"_sv, coffe::dom::xml::xml_type::cdata },
	{ "comment"_sv, coffe::dom::xml::xml_type::comment },
	{ "document"_sv, coffe::dom::xml::xml_type::document },
	{ "element"_sv, coffe::dom::xml::xml_type::element },
	{ "element_list"_sv, coffe::dom::xml::xml_type::element_list },
	{ "node"_sv, coffe::dom::xml::xml_type::node },
	{ "text"_sv, coffe::dom::xml::xml_type::text },
};

COFFE_ENUM_PARSE_IMPLEMENT(xml_type, s_xml_type_parsing_map, xml_type::abstract)
COFFE_ENUM_TO_STRING_IMPLEMENT(xml_type, s_xml_type_to_string_map, "abstract")

xml_entity_values_t xml::xml_entity_values[5] = {
	{ xml_entity::amp, "&amp;"_sv, "&"_sv },
	{ xml_entity::lt, "&lt;"_sv, "<"_sv },
	{ xml_entity::gt, "&gt;"_sv, ">" },
	{ xml_entity::quot, "&quot;"_sv, "\""_sv },
	{ xml_entity::apos, "&apos;"_sv, "'"_sv }
};

COFFE_FLAGS_IMPLEMENT(coffe::dom::xml, xml_format);

coffe::intf_wrapper<coffe::dom::xml::ixml_text>::operator coffe::cstr_t()const
{
	return is_empty() ? cstr_t() : get()->cstr();
}

coffe::intf_wrapper<coffe::dom::xml::ixml_node>::operator coffe::dom::xml::ixml_text_t()const 
{
	return is_empty() ? null : get()->value();
}

coffe::dom::xml::ixml_node_t coffe::intf_wrapper<coffe::dom::xml::ixml_node>::operator[](coffe::cstr_t name)const
{
	return is_empty() ? null : get()->children()[name];
}

coffe::dom::xml::xml_forward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_enum>::begin() 
{
	return is_empty() ? dom::xml::xml_forward_iterator_t() : get()->begin();
}

coffe::dom::xml::xml_forward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_enum>::end()
{
	return is_empty() ? dom::xml::xml_forward_iterator_t() : get()->end();
}

coffe::dom::xml::xml_const_forward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_enum>::begin()const 
{
	return is_empty() ? dom::xml::xml_const_forward_iterator_t() : static_cast<dom::xml::ixml_enum const*>(get())->begin();
}

coffe::dom::xml::xml_const_forward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_enum>::end()const
{
	return is_empty() ? dom::xml::xml_const_forward_iterator_t() : static_cast<dom::xml::ixml_enum const*>(get())->end();
}

coffe::dom::xml::xml_backward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_enum>::rbegin()
{
	return is_empty() ? dom::xml::xml_backward_iterator_t() : get()->rbegin();
}

coffe::dom::xml::xml_backward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_enum>::rend()
{
	return is_empty() ? dom::xml::xml_backward_iterator_t() : get()->rend();
}

coffe::dom::xml::xml_const_backward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_enum>::rbegin()const
{
	return is_empty() ? dom::xml::xml_const_backward_iterator_t() : static_cast<dom::xml::ixml_enum const*>(get())->rbegin();
}

coffe::dom::xml::xml_const_backward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_enum>::rend()const
{
	return is_empty() ? dom::xml::xml_const_backward_iterator_t() : static_cast<dom::xml::ixml_enum const*>(get())->rend();
}

///////////////////////////////////////////////////

coffe::dom::xml::xml_forward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::begin()
{
	return is_empty() ? dom::xml::xml_forward_iterator_t() : get()->begin();
}

coffe::dom::xml::xml_forward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::end()
{
	return is_empty() ? dom::xml::xml_forward_iterator_t() : get()->end();
}

coffe::dom::xml::xml_const_forward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::begin()const
{
	return is_empty() ? dom::xml::xml_const_forward_iterator_t() : static_cast<dom::xml::ixml_items const*>(get())->begin();
}

coffe::dom::xml::xml_const_forward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::end()const
{
	return is_empty() ? dom::xml::xml_const_forward_iterator_t() : static_cast<dom::xml::ixml_items const*>(get())->end();
}

coffe::dom::xml::xml_backward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::rbegin()
{
	return is_empty() ? dom::xml::xml_backward_iterator_t() : get()->rbegin();
}

coffe::dom::xml::xml_backward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::rend()
{
	return is_empty() ? dom::xml::xml_backward_iterator_t() : get()->rend();
}

coffe::dom::xml::xml_const_backward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::rbegin()const
{
	return is_empty() ? dom::xml::xml_const_backward_iterator_t() : static_cast<dom::xml::ixml_items const*>(get())->rbegin();
}

coffe::dom::xml::xml_const_backward_iterator_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::rend()const
{
	return is_empty() ? dom::xml::xml_const_backward_iterator_t() : static_cast<dom::xml::ixml_items const*>(get())->rend();
}

coffe::dom::xml::ixml_node_t coffe::intf_wrapper<coffe::dom::xml::ixml_items>::operator[](coffe::cstr_t name)const
{
	if (is_empty())
		return null;
	auto it = get()->find(name);
	if (!it.is_valid())
		return null;
	return it;
}

