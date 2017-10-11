/*********************************************************************************************************************/
/*   File Name: xml_attribute_list.cpp                                                                                           */
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


ang::object_wrapper<xml_attribute_list>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<xml_attribute_list>::object_wrapper(xml_attribute_list* ptr)
	: _ptr(null)
{
	set(ptr);
}

ang::object_wrapper<xml_attribute_list>::object_wrapper(object_wrapper && other)
	: _ptr(null)
{
	xml_attribute_list * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_attribute_list>::object_wrapper(object_wrapper const& other)
	: _ptr(null)
{
	set(other._ptr);
}

ang::object_wrapper<xml_attribute_list>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_attribute_list>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<xml_attribute_list>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<xml_attribute_list>::clean_unsafe()
{
	_ptr = null;
}

ang::bool_t ang::object_wrapper<xml_attribute_list>::is_empty()const
{
	return _ptr == null;
}

xml_attribute_list* ang::object_wrapper<xml_attribute_list>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_attribute_list>::set(xml_attribute_list* ptr)
{
	xml_attribute_list * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<xml_attribute_list>& ang::object_wrapper<xml_attribute_list>::operator = (xml_attribute_list* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_attribute_list>& ang::object_wrapper<xml_attribute_list>::operator = (ang::object_wrapper<xml_attribute_list> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_attribute_list>& ang::object_wrapper<xml_attribute_list>::operator = (ang::object_wrapper<xml_attribute_list> const& other)
{
	set(other._ptr);
	return*this;
}

object_wrapper<xml_attribute_list>&  ang::object_wrapper<xml_attribute_list>::operator += (xml_node_t node)
{
	if (_ptr)_ptr->append(node);
	return*this;
}

xml_attribute_list ** ang::object_wrapper<xml_attribute_list>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_attribute_list> ang::object_wrapper<xml_attribute_list>::operator& (void)
{
	return this;
}

xml_attribute_list * ang::object_wrapper<xml_attribute_list>::operator -> (void)
{
	return get();
}

xml_attribute_list const* ang::object_wrapper<xml_attribute_list>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_attribute_list>::operator xml_attribute_list * (void)
{
	return get();
}

ang::object_wrapper<xml_attribute_list>::operator xml_attribute_list const* (void)const
{
	return get();
}

xml_value_t ang::object_wrapper<xml_attribute_list>::operator[](cstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? it->xml_value() : xml_value();
}

xml_value_t ang::object_wrapper<xml_attribute_list>::operator[](cwstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? it->xml_value() : xml_value();
}

xml_value_t ang::object_wrapper<xml_attribute_list>::operator[](cmstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? it->xml_value() : xml_value();
}

///////////////////////////////////////

xml_attribute_list::xml_attribute_list()
	: xml_collection(xml_type::attribute_list)
{

}

xml_attribute_list::xml_attribute_list(const xml_attribute_list& list)
	: xml_collection(xml_type::attribute_list)
{
	copy(&list);
}

xml_attribute_list::xml_attribute_list(const xml_attribute_list* list)
	: xml_collection(xml_type::attribute_list)
{
	if(list)copy(list);
}

xml_attribute_list::~xml_attribute_list()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_attribute_list, ang::xml::xml_collection);

xml_collection_t xml_attribute_list::xml_clone()const
{
	return NEW xml_attribute_list(this);
}

bool_t xml_attribute_list::append(xml_node_t node, bool_t last)
{
	if (node.is_empty() || !node->xml_is_type_of(xml_type::attribute))
		return false;
	return xml_collection::append(node, last);
}

bool_t xml_attribute_list::insert(xml_node_t node, xml_iterator_t at, bool_t nextTo)
{
	if (node.is_empty() || !node->xml_is_type_of(xml_type::attribute))
		return false;
	return xml_collection::insert(node, at, nextTo);
}
