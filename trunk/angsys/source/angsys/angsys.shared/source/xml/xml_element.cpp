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
#include "ang/xml.hpp"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::xml;


ang::object_wrapper<xml_element>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<xml_element>::object_wrapper(xml_element* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<xml_element>::object_wrapper(object_wrapper && other) : _ptr(null) {
	xml_element * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_element>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<xml_element>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_element>::~object_wrapper() {
	clean();
}

void ang::object_wrapper<xml_element>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::object_wrapper<xml_element>::is_empty()const
{
	return _ptr == null;
}

xml_element* ang::object_wrapper<xml_element>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_element>::set(xml_element* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::object_wrapper<xml_element>& ang::object_wrapper<xml_element>::operator = (xml_element* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_element>& ang::object_wrapper<xml_element>::operator = (ang::object_wrapper<xml_element> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_element>& ang::object_wrapper<xml_element>::operator = (ang::object_wrapper<xml_element> const& other)
{
	set(other._ptr);
	return*this;
}

xml_element ** ang::object_wrapper<xml_element>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_element> ang::object_wrapper<xml_element>::operator & (void)
{
	return this;
}

xml_element * ang::object_wrapper<xml_element>::operator -> (void)
{
	return get();
}

xml_element const* ang::object_wrapper<xml_element>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_element>::operator xml::xml_element * (void) { return _ptr; }

ang::object_wrapper<xml_element>::operator xml::xml_element const* (void)const { return _ptr; }

ang::object_wrapper<xml_element>::operator xml::xml_node_t (void)const { return _ptr; }

ang::object_wrapper<xml_element>::operator xml::ixml_node_t(void)const { return _ptr; }

ang::object_wrapper<xml_element>::operator xml::xml_text_t()const { return _ptr ? _ptr->xml_value() : null; }

ixml_node_t object_wrapper<xml_element>::operator[](raw_str_t value)const
{
	xml_iterator_t it;
	if (_ptr)
	{
		if (_ptr->xml_has_attributes())
			it = _ptr->xml_attributes()->find(value);
		if (!it.is_valid() && _ptr->xml_has_children())
			it = _ptr->xml_children()->find(value);
	}
	return it.is_valid() ? (ixml_node*)it : null;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xml_element_t xml_element::create_new(const xml_element& element) {
	xml_element_t node = NEW xml_element(element.xml_parent_doc());

	if (!element._xml_name.is_empty())
		node->push_name(element._xml_name);

	if (!element._xml_content.is_empty() && element._xml_content->xml_is_type_of(xml_type_t::pcdata))
		node->push_value(element._xml_content.get()->xml_as<xml_text>());
	else if (!element._xml_content.is_empty() && element._xml_content->xml_is_type_of(xml_type_t::cdata))
		node->push_data(element._xml_content.get()->xml_as<xml_text>());
	else if (!element._xml_content.is_empty() && element._xml_content->xml_is_type_of(xml_type_t::element_list))
		node->push_children(element._xml_content.get()->xml_as<ixml_store>());
	if (element.xml_has_attributes())
		node->push_attributes(element._xml_attributes);
	return node;
}

xml_element_t xml_element::create_new(ixml_document_t doc, const xml_element* element) {
	xml_element_t node = NEW xml_element(doc);
	if (element)
	{
		if (!element->_xml_name.is_empty())
			node->push_name(element->_xml_name);

		if (!element->_xml_content.is_empty() && element->_xml_content->xml_is_type_of(xml_type_t::pcdata))
			node->push_value(element->_xml_content.get()->xml_as<xml_text>());
		else if (!element->_xml_content.is_empty() && element->_xml_content->xml_is_type_of(xml_type_t::cdata))
			node->push_data(element->_xml_content.get()->xml_as<xml_text>());
		else if (!element->_xml_content.is_empty() && element->_xml_content->xml_is_type_of(xml_type_t::element_list))
			node->push_children(element->_xml_content.get()->xml_as<ixml_store>());
		if (element->xml_has_attributes())
			node->push_attributes(element->_xml_attributes);
	}
	return node;
}

xml_element_t xml_element::create_new(ixml_document_t doc, wstring name) {
	xml_element_t node = NEW xml_element(doc);
	node->push_name(name.get());
	return node;
}

xml_element_t xml_element::create_new(ixml_document_t doc, wstring name, wstring value) {
	xml_element_t node = NEW xml_element(doc);
	node->push_name(name.get());
	node->push_value(value.get());
	return node;
}


xml_element::xml_element(ixml_document_t doc)
	: xml_node(doc, xml_type_t::element)
{

}

xml_element::~xml_element()
{
	clean();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_element, xml_node);


bool xml_element::xml_has_name()const
{
	return !_xml_name.is_empty();
}

bool xml_element::xml_has_value()const
{
	return !_xml_content.is_empty() && _xml_content ->xml_is_type_of(xml_type_t::text);
}

bool xml_element::xml_has_children()const
{
	return !_xml_content.is_empty() && _xml_content->xml_is_type_of(xml_type_t::element_list);
}

bool xml_element::xml_has_attributes()const
{
	return !_xml_attributes.is_empty();
}

ixml_node_t xml_element::xml_clone(ixml_document_t doc)const
{
	return xml_element::create_new(doc, this);
}

/////////////////////////////////////////////////////////////////////////////////////////

