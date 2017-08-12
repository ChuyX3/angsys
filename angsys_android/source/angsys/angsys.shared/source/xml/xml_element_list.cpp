/*********************************************************************************************************************/
/*   File Name: xml_element_list.cpp                                                                                           */
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

xml_element_list::xml_element_list()
	: xml_items(xml_type::element_list)
{

}

xml_element_list::xml_element_list(const xml_element_list& list)
	: xml_items(xml_type::element_list)
{
	copy(&list);
}

xml_element_list::xml_element_list(const xml_element_list* list)
	: xml_items(xml_type::element_list)
{
	if (list)copy(list);
}

xml_element_list::~xml_element_list()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_element_list, ang::xml::xml_items);

xml_items_t xml_element_list::xml_clone()const
{
	return new xml_element_list(this);
}

void xml_element_list::push(xml_node_t node, bool last)
{
	if (node.is_empty() || (!node->xml_is_type_of(xml_type::element)
		&& !node->xml_is_type_of(xml_type::comment)))
		return;
	xml_items::push(node, last);
}

bool xml_element_list::insert(xml_node_t node, xml_iterator_t at, bool nextTo)
{
	if (node.is_empty() || (!node->xml_is_type_of(xml_type::element)
		&& !node->xml_is_type_of(xml_type::comment)))
		return false;
	return xml_items::insert(node, at, nextTo);
}
