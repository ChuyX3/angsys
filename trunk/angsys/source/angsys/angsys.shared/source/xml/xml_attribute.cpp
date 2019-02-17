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



ang::object_wrapper<xml_attribute>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<xml_attribute>::object_wrapper(xml_attribute* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<xml_attribute>::object_wrapper(object_wrapper && other) : _ptr(null) {
	xml_attribute * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_attribute>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<xml_attribute>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_attribute>::~object_wrapper() {
	clean();
}

void ang::object_wrapper<xml_attribute>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::object_wrapper<xml_attribute>::is_empty()const
{
	return _ptr == null;
}

xml_attribute* ang::object_wrapper<xml_attribute>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_attribute>::set(xml_attribute* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::object_wrapper<xml_attribute>& ang::object_wrapper<xml_attribute>::operator = (xml_attribute* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_attribute>& ang::object_wrapper<xml_attribute>::operator = (ang::object_wrapper<xml_attribute> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_attribute>& ang::object_wrapper<xml_attribute>::operator = (ang::object_wrapper<xml_attribute> const& other)
{
	set(other._ptr);
	return*this;
}

xml_attribute ** ang::object_wrapper<xml_attribute>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_attribute> ang::object_wrapper<xml_attribute>::operator & (void)
{
	return this;
}

xml_attribute * ang::object_wrapper<xml_attribute>::operator -> (void)
{
	return get();
}

xml_attribute const* ang::object_wrapper<xml_attribute>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_attribute>::operator xml::xml_attribute * (void) { return _ptr; }

ang::object_wrapper<xml_attribute>::operator xml::xml_attribute const* (void)const { return _ptr; }

ang::object_wrapper<xml_attribute>::operator xml::xml_node_t(void)const { return _ptr; }

ang::object_wrapper<xml_attribute>::operator xml::ixml_node_t(void)const { return _ptr; }

ang::object_wrapper<xml_attribute>::operator xml::xml_text_t()const { return _ptr ? _ptr->xml_value() : null; }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xml_attribute_t xml_attribute::create_new(const xml_attribute& att) {
	xml_attribute_t node = NEW xml_attribute(att.xml_parent_doc());
	if (!att._xml_name.is_empty())
		node->push_name(att._xml_name->cstr());

	if (!att._xml_content.is_empty() && att._xml_content->xml_is_type_of(xml_type::text))
		node->push_value(att._xml_content.get()->xml_as<xml_text>()->cstr());
	return node;
}

xml_attribute_t xml_attribute::create_new(ixml_document_t doc, const xml_attribute* att) {
	xml_attribute_t node = NEW xml_attribute(doc);
	if (att)
	{
		if (!att->_xml_name.is_empty())
			node->push_name(att->_xml_name->cstr());

		if (!att->_xml_content.is_empty() && att->_xml_content->xml_is_type_of(xml_type::text))
			node->push_value(att->_xml_content.get()->xml_as<xml_text>()->cstr());
	}
	return node;
}

xml_attribute_t xml_attribute::create_new(ixml_document_t doc, wstring name, wstring value) {
	xml_attribute_t node = NEW xml_attribute(doc);
	node->push_name(name);
	node->push_value(value);
	return node;
}

xml_attribute::xml_attribute(ixml_document_t doc)
	: xml_node(doc, xml_type_t::attribute)
{

}

xml_attribute::~xml_attribute()
{
	clean();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_attribute, xml_node);


bool xml_attribute::xml_has_name()const
{
	return !_xml_name.is_empty();
}

bool xml_attribute::xml_has_value()const
{
	return _xml_content.is_empty() && _xml_content->xml_is_type_of(xml_type::text);
}

ixml_node_t xml_attribute::xml_clone(ixml_document_t doc)const
{
	return xml_attribute::create_new(doc, this);
}


///////////////////////////////////////


ixml_attributes::ixml_attributes() : _ptr(null) {

}

ixml_attributes::ixml_attributes(ixml_store* ptr) : _ptr(null) {
	set(ptr);
}

ixml_attributes::ixml_attributes(ixml_attributes_t && other) : _ptr(null) {
	ixml_store * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ixml_attributes::ixml_attributes(ixml_attributes const& other) : _ptr(null) {
	set(other._ptr);
}

ixml_attributes::ixml_attributes(std::nullptr_t const&)
	: _ptr(null)
{
}

ixml_attributes::~ixml_attributes() {
	clean();
}

void ixml_attributes::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ixml_attributes::is_empty()const
{
	return _ptr == null;
}

ixml_store* ixml_attributes::get(void)const
{
	return _ptr;
}

void ixml_attributes::set(ixml_store* ptr)
{
	if (ptr == _ptr) return;
	if (ptr != null && !ptr->xml_is_type_of(xml_type::attribute_list)) return clean();
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ixml_attributes& ixml_attributes::operator = (ixml_store* ptr)
{
	set(ptr);
	return*this;
}

ixml_attributes& ixml_attributes::operator = (ixml_attributes_t && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ixml_attributes& ixml_attributes::operator = (ixml_attributes const& other)
{
	set(other._ptr);
	return*this;
}

ixml_store ** ixml_attributes::addres_of(void)
{
	return &_ptr;
}

//ang::ixml_attributes_ptr<ixml_store> ixml_attributes::operator & (void)
//{
//	return this;
//}

ixml_store * ixml_attributes::operator -> (void)
{
	return get();
}

ixml_store const* ixml_attributes::operator -> (void)const
{
	return get();
}

ixml_attributes::operator xml::ixml_store * (void) { return _ptr; }

ixml_attributes::operator xml::ixml_store const* (void)const { return _ptr; }

xml_text_t ixml_attributes::operator[](raw_str_t value)const
{
	xml_iterator_t it;
	if (_ptr)
		it = _ptr->find(value);
	return it.is_valid() ? it->xml_value() : null;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
