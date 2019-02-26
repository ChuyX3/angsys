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

#include "xml/inline/xml_node.inl"
#include "xml/inline/xml_collection.inl"

using namespace ang;
using namespace ang::dom;
using namespace ang::dom::xml;

extern int ang_exceptions_regist_code(uint error_code, cstr_t error_message);

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_object, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::collections::ienum<ang::dom::xml::ixml_node>, interface);

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_node, ixml_object);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_collection, ixml_object, collections::ienum<ang::dom::xml::ixml_node>);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::dom::xml::ixml_document, ixml_object, collections::ienum<ang::dom::xml::ixml_node>);

//safe_enum_rrti2(ang::dom::xml, xml_type);

static collections::pair<ang::dom::xml::xml_type, cstr_t> s_xml_type_to_string_map[] = 
{
	{ ang::dom::xml::xml_type::abstract, "abstract"_s },
	{ ang::dom::xml::xml_type::text, "text"_s },
	{ ang::dom::xml::xml_type::cdata, "cdata"_s },
	{ ang::dom::xml::xml_type::pcdata, "pcdata"_s },
	{ ang::dom::xml::xml_type::node, "node"_s },
	{ ang::dom::xml::xml_type::store, "store"_s },
	{ ang::dom::xml::xml_type::tree, "tree"_s },
	{ ang::dom::xml::xml_type::header, "header"_s },
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
	{ "comment"_s, ang::dom::xml::xml_type::comment },
	{ "document"_s, ang::dom::xml::xml_type::document },
	{ "element"_s, ang::dom::xml::xml_type::element },
	{ "element_list"_s, ang::dom::xml::xml_type::element_list },
	{ "finder"_s, ang::dom::xml::xml_type::finder },
	{ "header"_s, ang::dom::xml::xml_type::header },
	{ "node"_s, ang::dom::xml::xml_type::node },
	{ "pcdata"_s, ang::dom::xml::xml_type::pcdata },
	{ "store"_s, ang::dom::xml::xml_type::store },
	{ "text"_s, ang::dom::xml::xml_type::text },
	{ "tree"_s, ang::dom::xml::xml_type::tree },
};


ixml_document_t xml::xml_create_document(xml_encoding_t e, core::files::input_text_file_t file)
{
	ixml_document_t doc;
	
	ixml_items_t items = new xml_element<xml_encoding::ascii>::xml_collection(null, xml_type::element_list);

	switch (e.get())
	{
	case xml_encoding::ascii: doc = new xml_document<xml_encoding::ascii>(); break;
	case xml_encoding::utf8: doc = new xml_document<xml_encoding::utf8>(); break;
	case xml_encoding::utf16: doc = new xml_document<xml_encoding::utf16>(); break;
	case xml_encoding::utf32: doc = new xml_document<xml_encoding::utf32>(); break;
	case xml_encoding::unicode: doc = new xml_document<xml_encoding::unicode>(); break;
	default:
		return null;
		break;
	}
	try {
		doc->load(file);
		return doc;
	}
	catch (exception_t const& e) {
		return null;
	}
	
}


//xml_type_t xml_type_t::parse(cstr_t text)
//{
//	auto idx = algorithms::binary_search(text, collections::to_array(s_xml_type_parsing_map));
//	if (idx >= algorithms::array_size(s_xml_type_parsing_map))
//		return xml_type::abstract;
//	else
//		return s_xml_type_parsing_map[idx].value;
//}
//
//xml_type_t xml_type_t::parse(cwstr_t text)
//{
//	auto idx = algorithms::binary_search(text, collections::to_array(s_xml_type_parsing_map));
//	if (idx >= algorithms::array_size(s_xml_type_parsing_map))
//		return xml_type::abstract;
//	else
//		return s_xml_type_parsing_map[idx].value;
//}

wstring xml_type_t::to_string()const
{
	auto idx = algorithms::binary_search(get(), collections::to_array(s_xml_type_to_string_map));
	if (idx >= algorithms::array_size(s_xml_type_to_string_map))
		return "abstract"_s;
	else
		return s_xml_type_to_string_map[idx].value;
}

safe_enum_rrti2(ang::dom::xml, xml_exception_code);

wstring xml_exception_code_t::to_string()const
{
	return ang::move(except_code_t((except_code)get()).to_string());
}

safe_flags_implement(ang::dom::xml, xml_format);

///////////////////////////////////////////////////////////////////

#define MY_TYPE	ang::dom::xml::ixml_node
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

intf_wrapper<ixml_node>::operator dom::xml::ixml_string_t()const {
	return is_empty() || !m_ptr->xml_has_value() ? null : m_ptr->xml_name();
}