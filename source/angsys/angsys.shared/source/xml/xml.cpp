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
#include "ang/xml.h"
#include "ang_inlines.h"

using namespace ang;
using namespace ang::xml;

extern int ang_exceptions_regist_code(uint error_code, cstr_t error_message);

ANG_IMPLEMENT_ENUM(xml, xml_type, byte_t, xml_type::abstract);

static ang_pair<ang::xml::xml_type, cstr_t> s_xml_type_to_string_map[] = 
{
	{ ang::xml::xml_type::abstract, "abstract"_s },
	{ ang::xml::xml_type::value, "value"_s },
	{ ang::xml::xml_type::node, "node"_s },
	{ ang::xml::xml_type::store, "store"_s },
	{ ang::xml::xml_type::tree, "tree"_s },
	{ ang::xml::xml_type::header, "header"_s },
	{ ang::xml::xml_type::element, "element"_s },
	{ ang::xml::xml_type::comment, "comment"_s },
	{ ang::xml::xml_type::attribute, "attribute"_s },
	{ ang::xml::xml_type::element_list, "element_list"_s },
	{ ang::xml::xml_type::attribute_list, "attribute_list"_s },
	{ ang::xml::xml_type::finder, "finder"_s },
	{ ang::xml::xml_type::builder, "builder"_s },
	{ ang::xml::xml_type::document, "document"_s },
};

static ang_pair<cstr_t, ang::xml::xml_type> s_xml_type_parsing_map[] =
{
	{ "abstract"_s, ang::xml::xml_type::abstract },
	{ "attribute"_s, ang::xml::xml_type::attribute },
	{ "attribute_list"_s, ang::xml::xml_type::attribute_list },
	{ "builder"_s, ang::xml::xml_type::builder },
	{ "comment"_s, ang::xml::xml_type::comment },
	{ "document"_s, ang::xml::xml_type::document },
	{ "element"_s, ang::xml::xml_type::element },
	{ "element_list"_s, ang::xml::xml_type::element_list },
	{ "finder"_s, ang::xml::xml_type::finder },
	{ "header"_s, ang::xml::xml_type::header },
	{ "node"_s, ang::xml::xml_type::node },
	{ "store"_s, ang::xml::xml_type::store },
	{ "tree"_s, ang::xml::xml_type::tree },
	{ "value"_s, ang::xml::xml_type::value },
};

xml_type_t xml_type_t::parse(cstr_t text)
{
	auto idx = ang_binary_search<cstr_t>(text, s_xml_type_parsing_map);
	if (idx >= array_size(s_xml_type_parsing_map))
		return xml_type::abstract;
	else
		return s_xml_type_parsing_map[idx].value;
}

xml_type_t xml_type_t::parse(cwstr_t text)
{
	auto idx = ang_binary_search<cwstr_t>(text, s_xml_type_parsing_map);
	if (idx >= array_size(s_xml_type_parsing_map))
		return xml_type::abstract;
	else
		return s_xml_type_parsing_map[idx].value;
}

cstr_t xml_type_t::to_string()const
{
	auto idx = ang_binary_search<xml_type>(_value, s_xml_type_to_string_map);
	if (idx >= array_size(s_xml_type_to_string_map))
		return "abstract"_s;
	else
		return s_xml_type_to_string_map[idx].value;
}

ANG_IMPLEMENT_ENUM(xml, xml_exception_code, uint, xml_exception_code::unknown);

cstr_t xml_exception_code_t::to_string()const
{
	return ang::move(except_code_t((except_code)_value).to_string());
}

ANG_IMPLEMENT_FLAGS(ang::xml, xml_format, uint);

ANG_IMPLEMENT_INTERFACE(ang::xml, ixmlobject);

bool ang::collections::ienum<xml_node>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::ienum<xml_node>::class_name())
		return true;
	return false;
}

bool ang::collections::ienum<xml_node>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::ienum<xml_node>::class_name())
		return true;
	return false;
}

ang::type_name_t ang::collections::ienum<xml_node>::class_name()
{
	return "ang::collections::ienum<xml_node>"_s;
}

ang::type_name_t ang::collections::ienum<xml_node>::object_name()const
{
	return ang::collections::ienum<xml_node>::class_name();
}

bool ang::collections::ienum<xml_node>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == class_name())
	{
		*out = static_cast<ang::collections::ienum<xml_node>*>(this);
		return true;
	}
	return false;
}

bool ang::collections::icollection<xml_node>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::icollection<xml_node>::class_name())
		return true;
	return false;
}

bool ang::collections::icollection<xml_node>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::icollection<xml_node>::class_name())
		return true;
	return false;
}

ang::type_name_t ang::collections::icollection<xml_node>::class_name()
{
	return "ang::collections::icollection<xml_node>"_s;
}

ang::type_name_t ang::collections::icollection<xml_node>::object_name()const
{
	return ang::collections::icollection<xml_node>::class_name();
}

bool ang::collections::icollection<xml_node>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == ang::collections::icollection<xml_node>::class_name())
	{
		*out = static_cast<ang::collections::icollection<xml_node>*>(this);
		return true;
	}
	return false;
}