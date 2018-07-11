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


ang::object_wrapper<xml_header>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<xml_header>::object_wrapper(xml_header* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<xml_header>::object_wrapper(object_wrapper && other) : _ptr(null) {
	xml_header * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_header>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<xml_header>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_header>::~object_wrapper() {
	clean();
}

void ang::object_wrapper<xml_header>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::object_wrapper<xml_header>::is_empty()const
{
	return _ptr == null;
}

xml_header* ang::object_wrapper<xml_header>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_header>::set(xml_header* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::object_wrapper<xml_header>& ang::object_wrapper<xml_header>::operator = (xml_header* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_header>& ang::object_wrapper<xml_header>::operator = (ang::object_wrapper<xml_header> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_header>& ang::object_wrapper<xml_header>::operator = (ang::object_wrapper<xml_header> const& other)
{
	set(other._ptr);
	return*this;
}

xml_header ** ang::object_wrapper<xml_header>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_header> ang::object_wrapper<xml_header>::operator & (void)
{
	return this;
}

xml_header * ang::object_wrapper<xml_header>::operator -> (void)
{
	return get();
}

xml_header const* ang::object_wrapper<xml_header>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_header>::operator xml::xml_header * (void) { return _ptr; }

ang::object_wrapper<xml_header>::operator xml::xml_header const* (void)const { return _ptr; }

ang::object_wrapper<xml_header>::operator xml::xml_node_t(void)const { return _ptr; }

ang::object_wrapper<xml_header>::operator xml::ixml_node_t(void)const { return _ptr; }

xml_text_t object_wrapper<xml_header>::operator[](raw_str_t value)const
{
	return _ptr ? _ptr->xml_attributes()[value] : xml_text_t();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xml_header_t xml_header::create_new(const xml_header& head) {
	xml_header_t header = NEW xml_header(head.xml_parent_doc());
	header->version(head.version());
	header->encoding(head.encoding());
	header->is_stand_alone(head.is_stand_alone());
	return header;
}

xml_header_t xml_header::create_new(ixml_document_t doc, const xml_header* head) {
	xml_header_t header = NEW xml_header(doc);
	if (head) {
		header->version(head->version());
		header->encoding(head->encoding());
		header->is_stand_alone(head->is_stand_alone());
	}
	else
	{
		header->version("1.0"_s);
		header->encoding(xml_encoding::utf8);
		header->is_stand_alone(true);
	}
	return header;
}

xml_header_t xml_header::create_new(ixml_document_t doc, wstring version, xml_encoding_t encoding, bool standalone) {
	xml_header_t header = NEW xml_header(doc);
	header->version(version);
	header->encoding(encoding);
	header->is_stand_alone(standalone);
	return header;
}

xml_header::xml_header(ixml_document_t doc)
	: xml_node(doc, xml_type_t::header)
{
}

xml_header::~xml_header()
{
	clean();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_header, xml_node);


ixml_node_t xml_header::xml_clone(ixml_document_t doc)const
{
	return create_new(doc, this);
}


wstring xml_header::version()const
{
	xml_text_t version = xml_attributes()["version"_s];
	return version;
}

xml_encoding xml_header::encoding()const
{
	xml_text_t encoding = xml_attributes()["encoding"_s];
	return encoding->xml_as<xml_encoding_t>();
}

bool xml_header::is_stand_alone()const
{
	xml_text_t standalone = xml_attributes()["standalone"_s];
	return standalone->xml_as<bool>();
}

void xml_header::version(wstring value)
{
	xml_text_t att = xml_attributes()["version"_s];
	if (att.is_empty())
		push_attribute(xml_attribute::create_new(xml_parent_doc(), "version"_s, value));
	else
		att->move(value.get());
}

void xml_header::encoding(xml_encoding_t value)
{
	xml_text_t att = xml_attributes()["encoding"_s];
	if (att.is_empty())
		push_attribute(xml_attribute::create_new(xml_parent_doc(), "encoding"_s, value.to_string()));
	else
		att->copy(value.to_string());
}

void xml_header::encoding(wstring value)
{
	xml_text_t att = xml_attributes()["encoding"_s];
	if (att.is_empty())
		push_attribute(xml_attribute::create_new(xml_parent_doc(), "encoding"_s, value));
	else
		att->move(value.get());
}

void xml_header::is_stand_alone(bool value)
{
	xml_text_t att = xml_attributes()["standalone"_s];
	if (att.is_empty())
		push_attribute(xml_attribute::create_new(xml_parent_doc(), "standalone"_s, value ? "yes"_s : "no"_s));
	else
		att->copy(value ? "yes"_s : "no"_s);
}
