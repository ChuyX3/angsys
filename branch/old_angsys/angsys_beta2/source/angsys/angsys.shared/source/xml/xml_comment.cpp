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


ang::object_wrapper<xml_comment>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<xml_comment>::object_wrapper(xml_comment* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<xml_comment>::object_wrapper(object_wrapper && other) : _ptr(null) {
	xml_comment * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_comment>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<xml_comment>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_comment>::~object_wrapper() {
	clean();
}

void ang::object_wrapper<xml_comment>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::object_wrapper<xml_comment>::is_empty()const
{
	return _ptr == null;
}

xml_comment* ang::object_wrapper<xml_comment>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_comment>::set(xml_comment* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::object_wrapper<xml_comment>& ang::object_wrapper<xml_comment>::operator = (xml_comment* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_comment>& ang::object_wrapper<xml_comment>::operator = (ang::object_wrapper<xml_comment> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_comment>& ang::object_wrapper<xml_comment>::operator = (ang::object_wrapper<xml_comment> const& other)
{
	set(other._ptr);
	return*this;
}

xml_comment ** ang::object_wrapper<xml_comment>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_comment> ang::object_wrapper<xml_comment>::operator & (void)
{
	return this;
}

xml_comment * ang::object_wrapper<xml_comment>::operator -> (void)
{
	return get();
}

xml_comment const* ang::object_wrapper<xml_comment>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_comment>::operator xml::xml_comment * (void) { return _ptr; }

ang::object_wrapper<xml_comment>::operator xml::xml_comment const* (void)const { return _ptr; }

ang::object_wrapper<xml_comment>::operator xml::xml_node_t(void)const { return _ptr; }

ang::object_wrapper<xml_comment>::operator xml::ixml_node_t(void)const { return _ptr; }

ang::object_wrapper<xml_comment>::operator xml::xml_text_t()const { return _ptr ? _ptr->xml_value() : null; }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xml_comment_t xml_comment::create_new(const xml_comment& comment) {
	xml_comment_t node = NEW xml_comment(comment.xml_parent_doc());

	if (!comment._xml_content.is_empty() && comment._xml_content->xml_is_type_of(xml_type_t::text))
		node->push_value(comment._xml_content.get()->xml_as<xml_text>()->cstr());
	return node;
}

xml_comment_t xml_comment::create_new(ixml_document_t doc, const xml_comment* comment) {
	xml_comment_t node = NEW xml_comment(doc);
	if (comment)
	{
		if (!comment->_xml_content.is_empty() && comment->_xml_content->xml_is_type_of(xml_type_t::text))
			node->push_value(comment->_xml_content.get()->xml_as<xml_text>()->cstr());
	}
	return node;
}

xml_comment_t xml_comment::create_new(ixml_document_t doc, wstring value) {
	xml_comment_t node = NEW xml_comment(doc);
	node->push_value(value.get());
	return node;
}

xml_comment::xml_comment(ixml_document_t doc)
	: xml_node(doc, xml_type_t::comment)
{

}

xml_comment::~xml_comment()
{
	clean();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_comment, xml_node);

bool xml_comment::xml_has_value()const
{
	return !_xml_content.is_empty() && _xml_content->xml_is_type_of(xml_type::text);
}

ixml_node_t xml_comment::xml_clone(ixml_document_t doc)const
{
	return xml_comment::create_new(doc, this);
}
