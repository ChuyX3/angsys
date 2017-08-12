/*********************************************************************************************************************/
/*   File Name: xml_comment.cpp                                                                                           */
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


xml_comment::xml_comment()
	: xml_node(xml_type_t::comment)
{

}

xml_comment::xml_comment(wstring value)
	: xml_node(xml_type_t::comment)
{
	xml_value(ang::move(value));
}

xml_comment::xml_comment(const xml_comment& comment)
	: xml_node(xml_type_t::comment)
{
	if (comment._xml_content.data_type == xml_type_t::value)
		xml_value(wstring(static_cast<strings::wstring_buffer*>(comment._xml_content.unknown.get())));
}

xml_comment::xml_comment(const xml_comment* comment)
	: xml_node(xml_type_t::comment)
{
	if (comment)
	{
		if (!comment->_xml_name.is_empty())
			xml_name(comment->_xml_name);

		if (comment->_xml_content.data_type == xml_type_t::value)
			xml_value(wstring(static_cast<strings::wstring_buffer*>(comment->_xml_content.unknown.get())));
	}
}

xml_comment::~xml_comment()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_comment, xml_node);

bool xml_comment::xml_has_value()const
{
	return _xml_content.data_type == xml_type_t::value;
}

xml_node_t xml_comment::xml_clone()const
{
	return new xml_comment(this);
}