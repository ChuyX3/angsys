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

#if defined _DEBUG
#define NEW ANG_DEBUG_NEW()
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::xml;

xml_element_list::xml_element_list()
	: xml_collection(xml_type::element_list)
{

}

xml_element_list::xml_element_list(const xml_element_list& list)
	: xml_collection(xml_type::element_list)
{
	copy(&list);
}

xml_element_list::xml_element_list(const xml_element_list* list)
	: xml_collection(xml_type::element_list)
{
	if (list)copy(list);
}

xml_element_list::~xml_element_list()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_element_list, ang::xml::xml_collection);

xml_collection_t xml_element_list::xml_clone()const
{
	return NEW xml_element_list(this);
}

bool_t xml_element_list::append(xml_node_t node, bool_t last)
{
	if (node.is_empty() || (!node->xml_is_type_of(xml_type::element)
		&& !node->xml_is_type_of(xml_type::comment)))
		return false;
	return xml_collection::append(node, last);
}

bool_t xml_element_list::insert(xml_node_t node, xml_iterator_t at, bool_t nextTo)
{
	if (node.is_empty() || (!node->xml_is_type_of(xml_type::element)
		&& !node->xml_is_type_of(xml_type::comment)))
		return false;
	return xml_collection::insert(node, at, nextTo);
}
