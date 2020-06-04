
/*********************************************************************************************************************/
/*   File Name: xml.cpp                                                                                              */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) ang sys, Jesus Angel Rocha Morales                                                              */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "ang/dom/xml.h"
#include "xml/xml_impl.h"

using namespace ang;
using namespace ang::dom;
using namespace ang::dom::xml;

#define MY_TYPE ang::dom::xml::ixml_text
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::dom::xml::ixml_node
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::dom::xml::ixml_enum
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::dom::xml::ixml_items
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::dom::xml::ixml_document
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE


ANG_ENUM_IMPLEMENT(ang::dom::xml, xml_type);

static collections::pair<ang::dom::xml::xml_type, castr_t> s_xml_type_to_string_map[] =
{
	{ ang::dom::xml::xml_type::abstract, "abstract"_sv },
	{ ang::dom::xml::xml_type::document, "document"_sv },
	{ ang::dom::xml::xml_type::text, "text"_sv },
	{ ang::dom::xml::xml_type::cdata, "cdata"_sv },
	{ ang::dom::xml::xml_type::node, "node"_sv },
	{ ang::dom::xml::xml_type::element, "element"_sv },
	{ ang::dom::xml::xml_type::comment, "comment"_sv },
	{ ang::dom::xml::xml_type::attribute, "attribute"_sv },
	{ ang::dom::xml::xml_type::element_list, "element_list"_sv },
	{ ang::dom::xml::xml_type::attribute_list, "attribute_list"_sv },
};

static collections::pair<castr_t, ang::dom::xml::xml_type> s_xml_type_parsing_map[] =
{
	{ "abstract"_sv, ang::dom::xml::xml_type::abstract },
	{ "attribute"_sv, ang::dom::xml::xml_type::attribute },
	{ "attribute_list"_sv, ang::dom::xml::xml_type::attribute_list },
	{ "cdata"_sv, ang::dom::xml::xml_type::cdata },
	{ "comment"_sv, ang::dom::xml::xml_type::comment },
	{ "document"_sv, ang::dom::xml::xml_type::document },
	{ "element"_sv, ang::dom::xml::xml_type::element },
	{ "element_list"_sv, ang::dom::xml::xml_type::element_list },
	{ "node"_sv, ang::dom::xml::xml_type::node },
	{ "text"_sv, ang::dom::xml::xml_type::text },
};

ANG_ENUM_PARSE_IMPLEMENT(xml_type, s_xml_type_parsing_map, xml_type::abstract)
ANG_ENUM_TO_STRING_IMPLEMENT(xml_type, s_xml_type_to_string_map, "abstract")

xml_entity_values_t xml::xml_entity_values[5] = {
	{ xml_entity::amp, "&amp;"_sv, "&"_sv },
	{ xml_entity::lt, "&lt;"_sv, "<"_sv },
	{ xml_entity::gt, "&gt;"_sv, ">" },
	{ xml_entity::quot, "&quot;"_sv, "\""_sv },
	{ xml_entity::apos, "&apos;"_sv, "'"_sv }
};

ANG_FLAGS_IMPLEMENT(ang::dom::xml, xml_format);

ang::intf_wrapper<ang::dom::xml::ixml_text>::operator ang::cstr_t()const
{
	return is_empty() ? cstr_t() : get()->cstr();
}

ang::intf_wrapper<ang::dom::xml::ixml_node>::operator ang::dom::xml::ixml_text_t()const 
{
	return is_empty() ? null : get()->value();
}

ang::dom::xml::ixml_node_t ang::intf_wrapper<ang::dom::xml::ixml_node>::operator[](ang::cstr_t name)const
{
	return is_empty() ? null : get()->children()[name];
}

ang::dom::xml::xml_forward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_enum>::begin() 
{
	return is_empty() ? dom::xml::xml_forward_iterator_t() : get()->begin();
}

ang::dom::xml::xml_forward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_enum>::end()
{
	return is_empty() ? dom::xml::xml_forward_iterator_t() : get()->end();
}

ang::dom::xml::xml_const_forward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_enum>::begin()const 
{
	return is_empty() ? dom::xml::xml_const_forward_iterator_t() : static_cast<dom::xml::ixml_enum const*>(get())->begin();
}

ang::dom::xml::xml_const_forward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_enum>::end()const
{
	return is_empty() ? dom::xml::xml_const_forward_iterator_t() : static_cast<dom::xml::ixml_enum const*>(get())->end();
}

ang::dom::xml::xml_backward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_enum>::rbegin()
{
	return is_empty() ? dom::xml::xml_backward_iterator_t() : get()->rbegin();
}

ang::dom::xml::xml_backward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_enum>::rend()
{
	return is_empty() ? dom::xml::xml_backward_iterator_t() : get()->rend();
}

ang::dom::xml::xml_const_backward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_enum>::rbegin()const
{
	return is_empty() ? dom::xml::xml_const_backward_iterator_t() : static_cast<dom::xml::ixml_enum const*>(get())->rbegin();
}

ang::dom::xml::xml_const_backward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_enum>::rend()const
{
	return is_empty() ? dom::xml::xml_const_backward_iterator_t() : static_cast<dom::xml::ixml_enum const*>(get())->rend();
}

///////////////////////////////////////////////////

ang::dom::xml::xml_forward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_items>::begin()
{
	return is_empty() ? dom::xml::xml_forward_iterator_t() : get()->begin();
}

ang::dom::xml::xml_forward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_items>::end()
{
	return is_empty() ? dom::xml::xml_forward_iterator_t() : get()->end();
}

ang::dom::xml::xml_const_forward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_items>::begin()const
{
	return is_empty() ? dom::xml::xml_const_forward_iterator_t() : static_cast<dom::xml::ixml_items const*>(get())->begin();
}

ang::dom::xml::xml_const_forward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_items>::end()const
{
	return is_empty() ? dom::xml::xml_const_forward_iterator_t() : static_cast<dom::xml::ixml_items const*>(get())->end();
}

ang::dom::xml::xml_backward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_items>::rbegin()
{
	return is_empty() ? dom::xml::xml_backward_iterator_t() : get()->rbegin();
}

ang::dom::xml::xml_backward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_items>::rend()
{
	return is_empty() ? dom::xml::xml_backward_iterator_t() : get()->rend();
}

ang::dom::xml::xml_const_backward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_items>::rbegin()const
{
	return is_empty() ? dom::xml::xml_const_backward_iterator_t() : static_cast<dom::xml::ixml_items const*>(get())->rbegin();
}

ang::dom::xml::xml_const_backward_iterator_t ang::intf_wrapper<ang::dom::xml::ixml_items>::rend()const
{
	return is_empty() ? dom::xml::xml_const_backward_iterator_t() : static_cast<dom::xml::ixml_items const*>(get())->rend();
}

ang::dom::xml::ixml_node_t ang::intf_wrapper<ang::dom::xml::ixml_items>::operator[](ang::cstr_t name)const
{
	if (is_empty())
		return null;
	auto it = get()->find(name);
	if (!it.is_valid())
		return null;
	return it;
}

