/*********************************************************************************************************************/
/*   File Name: xml_header.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/xml.h"

//#if defined _DEBUG
//#define NEW ANG_DEBUG_NEW()
//#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
//#else
//#define NEW new
//#define NEW_ARGS(...) new(__VA_ARGS__)
//#endif

using namespace ang;
using namespace ang::xml;


xml_header::xml_header()
	: xml_node(xml_type_t::header)
{
	version("1.0");
	encoding(xml_encoding::utf_8);
}

xml_header::xml_header(const xml_header& att)
	: xml_node(xml_type_t::header)
{
	version(att.version());
	encoding(att.encoding());
	is_stand_alone(att.is_stand_alone());
}

xml_header::xml_header(const xml_header* att)
	: xml_node(xml_type_t::header)
{
	if (att)
	{
		version(att->version());
		encoding(att->encoding());
		is_stand_alone(att->is_stand_alone());
	}
	else
	{
		version("1.0");
		encoding(xml_encoding::utf_8);
	}
}

xml_header::~xml_header()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_header, xml_node);


xml_node_t xml_header::xml_clone()const
{
	return new xml_header(this);
}


wstring xml_header::version()const
{
	xml_value_t version = xml_attributes()["version"];
	return version;
}

xml_encoding xml_header::encoding()const
{
	xml_value_t encoding = xml_attributes()["encoding"];
	return encoding.as<xml_encoding_t>();
}

bool xml_header::is_stand_alone()const
{
	xml_value_t standalone = xml_attributes()["standalone"];
	return standalone.as<bool>();
}

void xml_header::version(wstring value)
{
	if (_xml_attributes.is_empty())
	{
		_xml_attributes = new xml_attribute_list();
		//xmlAttributes->XmlParent(this);
	}
	auto it = _xml_attributes->find_first("version");
	if (it.is_valid())
		it->xml_value(ang::move(value));
	else
		_xml_attributes->insert(new xml_attribute("version", ang::move(value)), 0);
}

void xml_header::encoding(xml_encoding_t value)
{
	if (_xml_attributes.is_empty())
	{
		_xml_attributes = new xml_attribute_list();
		//xmlAttributes->XmlParent(this);
	}

	auto it = _xml_attributes->find_first("encoding");
	if (it.is_valid())
		it->xml_value(value.to_string());
	else
		_xml_attributes->insert(new xml_attribute("encoding", value.to_string()), 1);
}

void xml_header::encoding(wstring value)
{
	if (value.is_empty())
		return;
	if (_xml_attributes.is_empty())
	{
		_xml_attributes = new xml_attribute_list();
		//xmlAttributes->XmlParent(this);
	}
	value->lowercase();
	auto it = _xml_attributes->find_first("encoding");
	if (it.is_valid())
		it->xml_value(ang::move(value));
	else
		_xml_attributes->insert(new xml_attribute("encoding", ang::move(value)), 1);
}

void xml_header::is_stand_alone(bool value)
{
	if (_xml_attributes.is_empty())
	{
		_xml_attributes = new xml_attribute_list();
		//xmlAttributes->XmlParent(this);
	}
	auto it = _xml_attributes->find_first("standalone");
	if (it.is_valid() && value)
		_xml_attributes->pop_at(it);
	else if (!value)
		_xml_attributes->insert(new xml_attribute("standalone", "no"), 2);
}