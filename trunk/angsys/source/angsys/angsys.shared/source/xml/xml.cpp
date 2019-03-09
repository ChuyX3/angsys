/*********************************************************************************************************************/
/*   File Name: xml.cpp                                                                                           */
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

//#include "xml/inline/xml_node.inl"
//#include "xml/inline/xml_collection.inl"

using namespace ang;
using namespace ang::dom;
using namespace ang::dom::xml;

extern int ang_exceptions_regist_code(uint error_code, cstr_t error_message);

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_object, interface);

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_node, ixml_object);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_items, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_collection, ixml_object, ixml_items);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_document, ixml_object, ixml_items);



//safe_enum_rrti2(ang::dom::xml, xml_type);

static collections::pair<ang::dom::xml::xml_type, cstr_t> s_xml_type_to_string_map[] = 
{
	{ ang::dom::xml::xml_type::abstract, "abstract"_s },
	{ ang::dom::xml::xml_type::text, "text"_s },
	{ ang::dom::xml::xml_type::cdata, "cdata"_s },
	{ ang::dom::xml::xml_type::node, "node"_s },
	{ ang::dom::xml::xml_type::collection, "collection"_s },
	{ ang::dom::xml::xml_type::tree, "tree"_s },
	{ ang::dom::xml::xml_type::element, "element"_s },
	{ ang::dom::xml::xml_type::comment, "comment"_s },
	{ ang::dom::xml::xml_type::attribute, "attribute"_s },
	{ ang::dom::xml::xml_type::element_list, "element_list"_s },
	{ ang::dom::xml::xml_type::attribute_list, "attribute_list"_s },
	{ ang::dom::xml::xml_type::finder, "finder"_s },
	{ ang::dom::xml::xml_type::builder, "builder"_s },
	{ ang::dom::xml::xml_type::document, "document"_s },
};

static collections::pair<cstr_t, ang::dom::xml::xml_type> s_xml_type_parsing_map[] =
{
	{ "abstract"_s, ang::dom::xml::xml_type::abstract },
	{ "attribute"_s, ang::dom::xml::xml_type::attribute },
	{ "attribute_list"_s, ang::dom::xml::xml_type::attribute_list },
	{ "builder"_s, ang::dom::xml::xml_type::builder },
	{ "cdata"_s, ang::dom::xml::xml_type::cdata },
	{ "collection"_s, ang::dom::xml::xml_type::collection },
	{ "comment"_s, ang::dom::xml::xml_type::comment },
	{ "document"_s, ang::dom::xml::xml_type::document },
	{ "element"_s, ang::dom::xml::xml_type::element },
	{ "element_list"_s, ang::dom::xml::xml_type::element_list },
	{ "finder"_s, ang::dom::xml::xml_type::finder },
	{ "node"_s, ang::dom::xml::xml_type::node },
	{ "text"_s, ang::dom::xml::xml_type::text },
	{ "tree"_s, ang::dom::xml::xml_type::tree },
};

template<> ixml_text_t ixml_object::xml_as<ixml_text>()const
{
	return xml_is_type_of(xml_type::text) ? static_cast<xml_text*>(const_cast<ixml_object*>(this)) : null;
}

template<> xml_text_t ixml_object::xml_as<xml_text>()const
{
	return xml_is_type_of(xml_type::text) ? static_cast<xml_text*>(const_cast<ixml_object*>(this)) : null;
}

template<> ixml_node_t ixml_object::xml_as<ixml_node>()const
{
	return xml_is_type_of(xml_type::node) ? static_cast<xml_node*>(const_cast<ixml_object*>(this)) : null;
}

template<> xml_node_t ixml_object::xml_as<xml_node>()const
{
	return xml_is_type_of(xml_type::node) ? static_cast<xml_node*>(const_cast<ixml_object*>(this)) : null;
}

template<> xml_attribute_t ixml_object::xml_as<xml_attribute>()const
{
	return xml_is_type_of(xml_type::attribute) ? static_cast<xml_attribute*>(const_cast<ixml_object*>(this)) : null;
}

template<> xml_comment_t ixml_object::xml_as<xml_comment>()const
{
	return xml_is_type_of(xml_type::comment) ? static_cast<xml_comment*>(const_cast<ixml_object*>(this)) : null;
}

template<> xml_element_t ixml_object::xml_as<xml_element>()const
{
	return xml_is_type_of(xml_type::element) ? static_cast<xml_element*>(const_cast<ixml_object*>(this)) : null;
}

template<> ixml_collection_t ixml_object::xml_as<ixml_collection>()const
{
	return xml_is_type_of(xml_type::collection) ? static_cast<xml_collection*>(const_cast<ixml_object*>(this)) : null;
}

template<> xml_collection_t ixml_object::xml_as<xml_collection>()const
{
	return xml_is_type_of(xml_type::collection) ? static_cast<xml_collection*>(const_cast<ixml_object*>(this)) : null;
}

template<> ixml_document_t ixml_object::xml_as<ixml_document>()const
{
	return xml_is_type_of(xml_type::document) ? static_cast<xml_document*>(const_cast<ixml_object*>(this)) : null;
}

template<> xml_document_t ixml_object::xml_as<xml_document>()const
{
	return xml_is_type_of(xml_type::document) ? static_cast<xml_document*>(const_cast<ixml_object*>(this)) : null;
}


xml_type_t xml_type_t::parse(xml_cstr_t text)
{
	auto idx = algorithms::binary_search(text, collections::to_array(s_xml_type_parsing_map));
	if (idx >= algorithms::array_size(s_xml_type_parsing_map))
		return xml_type::abstract;
	else
		return s_xml_type_parsing_map[idx].value;
}

//xml_type_t xml_type_t::parse(cwstr_t text)
//{
//	auto idx = algorithms::binary_search(text, collections::to_array(s_xml_type_parsing_map));
//	if (idx >= algorithms::array_size(s_xml_type_parsing_map))
//		return xml_type::abstract;
//	else
//		return s_xml_type_parsing_map[idx].value;
//}

cstr_t xml_type_t::to_string()const
{
	auto idx = algorithms::binary_search(get(), collections::to_array(s_xml_type_to_string_map));
	if (idx >= algorithms::array_size(s_xml_type_to_string_map))
		return "abstract"_s;
	else
		return s_xml_type_to_string_map[idx].value;
}

safe_enum_rrti2(ang::dom::xml, xml_exception_code);

cstr_t xml_exception_code_t::to_string()const
{
	return ang::move(except_code_t((except_code)get()).to_string());
}

safe_flags_implement(ang::dom::xml, xml_format);

///////////////////////////////////////////////////////////////////
