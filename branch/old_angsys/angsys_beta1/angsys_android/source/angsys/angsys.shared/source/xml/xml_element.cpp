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
#include "ang/xml.h"

#if defined _DEBUG
#define NEW ANG_DEBUG_NEW()
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::xml;


xml_element::xml_element()
	: xml_node(xml_type_t::element)
{

}

xml_element::xml_element(mstring name, mstring value)
	: xml_node(xml_type_t::element)
{
	xml_name(ang::move(name));
	xml_value(ang::move(value));
}

xml_element::xml_element(mstring name, xml_items_t items)
	: xml_node(xml_type_t::element)
{
	xml_name(ang::move(name));

	if(!items.is_empty() && items->xml_is_type_of(xml_type_t::element_list))
		xml_children(ang::move(items));
	if (!items.is_empty() && items->xml_is_type_of(xml_type_t::attribute_list))
		xml_attributes(ang::move(items));
}

xml_element::xml_element(const xml_element& att)
	: xml_node(xml_type_t::element)
{
	if (!att._xml_name.is_empty())
		xml_name(att._xml_name);

	if (att._xml_content.data_type == xml_type_t::value)
		xml_value(mstring(static_cast<strings::mstring_buffer*>(att._xml_content.unknown.get())));
	else if (att._xml_content.data_type == xml_type_t::element_list)
		xml_children(static_cast<xml_element_list*>(att._xml_content.unknown.get())->xml_clone());
	if (att.xml_has_attributes())
		xml_attributes(att._xml_attributes->xml_clone());
}

xml_element::xml_element(const xml_element* att)
	: xml_node(xml_type_t::element)
{
	if (att)
	{
		if (!att->_xml_name.is_empty())
			xml_name(att->_xml_name);

		if (att->_xml_content.data_type == xml_type_t::value)
			xml_value(mstring(static_cast<strings::mstring_buffer*>(att->_xml_content.unknown.get())));
		else if (att->_xml_content.data_type == xml_type_t::element_list)
			xml_children(static_cast<xml_element_list*>(att->_xml_content.unknown.get())->xml_clone());
		if (att->xml_has_attributes())
			xml_attributes(att->_xml_attributes->xml_clone());
	}
}

xml_element::~xml_element()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_element, xml_node);


bool xml_element::xml_has_name()const
{
	return !_xml_name.is_empty();
}

bool xml_element::xml_has_value()const
{
	return _xml_content.data_type == xml_type_t::value;
}

bool xml_element::xml_has_children()const
{
	return _xml_content.data_type == xml_type_t::element_list;
}

bool xml_element::xml_has_attributes()const
{
	return !_xml_attributes.is_empty();
}

xml_node_t xml_element::xml_clone()const
{
	return new xml_element(this);
}