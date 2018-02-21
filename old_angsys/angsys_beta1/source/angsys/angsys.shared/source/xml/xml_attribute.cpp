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
#include "ang/xml.h"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::xml;


xml_attribute::xml_attribute()
	: xml_node(xml_type_t::attribute)
{

}

xml_attribute::xml_attribute(wstring name, wstring value)
	: xml_node(xml_type_t::attribute)
{
	xml_name(ang::move(name));
	xml_value(ang::move(value));
}

xml_attribute::xml_attribute(const xml_attribute& att)
	: xml_node(xml_type_t::attribute)
{
	if (!att._xml_name.is_empty())
		xml_name(att._xml_name);

	if (att._xml_content.data_type == xml_type_t::value)
		xml_value(wstring(static_cast<strings::wstring_buffer*>(att._xml_content.unknown.get())));
}

xml_attribute::xml_attribute(const xml_attribute* att)
	: xml_node(xml_type_t::attribute)
{
	if (att)
	{
		if (!att->_xml_name.is_empty())
			xml_name(att->_xml_name);

		if (att->_xml_content.data_type == xml_type_t::value)
			xml_value(wstring(static_cast<strings::wstring_buffer*>(att->_xml_content.unknown.get())));
	}
}

xml_attribute::~xml_attribute()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_attribute, xml_node);


bool xml_attribute::xml_has_name()const
{
	return !_xml_name.is_empty();
}

bool xml_attribute::xml_has_value()const
{
	return _xml_content.data_type == xml_type_t::value;
}

xml_node_t xml_attribute::xml_clone()const
{
	return NEW xml_attribute(this);
}