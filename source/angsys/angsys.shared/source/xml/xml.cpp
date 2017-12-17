/*********************************************************************************************************************/
/*   File Name: xml.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/
#include "pch.h"
#include "ang/xml.hpp"
//#include "ang_inlines.h"

using namespace ang;
using namespace ang::xml;

extern int ang_exceptions_regist_code(uint error_code, cstr_t error_message);

ANG_IMPLEMENT_INTERFACE(ang::xml, ixml_object);
ANG_IMPLEMENT_INTERFACE(ang::collections, ienum<ixml_node>);

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::ixml_node, ixml_object);
//ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::ixml_store, ixml_object);
ANG_IMPLEMENT_CLASSNAME(ang::xml::ixml_store);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::ixml_store);

bool ang::xml::ixml_store::is_kind_of(ang::type_name_t name)const
{
	return name == ang::xml::ixml_store::class_name()
		|| ang::xml::ixml_object::is_kind_of(name)
		|| ang::collections::ienum<ixml_node>::is_kind_of(name);
}

bool ang::xml::ixml_store::is_inherited_of(ang::type_name_t name)
{
	return name == ang::xml::ixml_store::class_name()
		|| ang::xml::ixml_object::is_inherited_of(name)
		|| ang::collections::ienum<ixml_node>::is_inherited_of(name);
}


bool ang::xml::ixml_store::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::xml::ixml_store*>(this);
		return true;
	}
	else if (ixml_object::query_object(name, out))
	{
		return true;
	}
	else if (collections::ienum<ixml_node>::query_object(name, out))
	{
		return true;
	}
	return false;
}


ANG_IMPLEMENT_CLASSNAME(ang::xml::ixml_document);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::ixml_document);

bool ang::xml::ixml_document::is_kind_of(ang::type_name_t name)const
{
	return name == ang::xml::ixml_document::class_name()
		|| ang::xml::ixml_object::is_kind_of(name)
		|| ang::collections::ienum<ixml_node>::is_kind_of(name);
}

bool ang::xml::ixml_document::is_inherited_of(ang::type_name_t name)
{
	return name == ang::xml::ixml_document::class_name()
		|| ang::xml::ixml_object::is_inherited_of(name)
		|| ang::collections::ienum<ixml_node>::is_inherited_of(name);
}


bool ang::xml::ixml_document::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::xml::ixml_document*>(this);
		return true;
	}
	else if (ixml_object::query_object(name, out))
	{
		return true;
	}
	else if (collections::ienum<ixml_node>::query_object(name, out))
	{
		return true;
	}
	return false;
}


ANG_IMPLEMENT_ENUM(xml, xml_type, byte_t, xml_type::abstract);

static collections::pair<ang::xml::xml_type, cstr_t> s_xml_type_to_string_map[] = 
{
	{ ang::xml::xml_type::abstract, "abstract"_s },
	{ ang::xml::xml_type::text, "text"_s },
	{ ang::xml::xml_type::cdata, "cdata"_s },
	{ ang::xml::xml_type::pcdata, "pcdata"_s },
	{ ang::xml::xml_type::node, "node"_s },
	{ ang::xml::xml_type::store, "store"_s },
	{ ang::xml::xml_type::tree, "tree"_s },
	{ ang::xml::xml_type::header, "header"_s },
	{ ang::xml::xml_type::element, "element"_s },
	{ ang::xml::xml_type::comment, "comment"_s },
	{ ang::xml::xml_type::attribute, "attribute"_s },
	{ ang::xml::xml_type::element_list, "element_list"_s },
	{ ang::xml::xml_type::attribute_list, "attribute_list"_s },
	{ ang::xml::xml_type::finder, "finder"_s },
	{ ang::xml::xml_type::builder, "builder"_s },
	{ ang::xml::xml_type::document, "document"_s },
};

static collections::pair<cstr_t, ang::xml::xml_type> s_xml_type_parsing_map[] =
{
	{ "abstract"_s, ang::xml::xml_type::abstract },
	{ "attribute"_s, ang::xml::xml_type::attribute },
	{ "attribute_list"_s, ang::xml::xml_type::attribute_list },
	{ "builder"_s, ang::xml::xml_type::builder },
	{ "cdata"_s, ang::xml::xml_type::cdata },
	{ "comment"_s, ang::xml::xml_type::comment },
	{ "document"_s, ang::xml::xml_type::document },
	{ "element"_s, ang::xml::xml_type::element },
	{ "element_list"_s, ang::xml::xml_type::element_list },
	{ "finder"_s, ang::xml::xml_type::finder },
	{ "header"_s, ang::xml::xml_type::header },
	{ "node"_s, ang::xml::xml_type::node },
	{ "pcdata"_s, ang::xml::xml_type::pcdata },
	{ "store"_s, ang::xml::xml_type::store },
	{ "text"_s, ang::xml::xml_type::text },
	{ "tree"_s, ang::xml::xml_type::tree },
};

xml_type_t xml_type_t::parse(cstr_t text)
{
	auto idx = collections::algorithms::binary_search(text, collections::to_array(s_xml_type_parsing_map));
	if (idx >= array_size(s_xml_type_parsing_map))
		return xml_type::abstract;
	else
		return s_xml_type_parsing_map[idx].value;
}

xml_type_t xml_type_t::parse(cwstr_t text)
{
	auto idx = collections::algorithms::binary_search(text, collections::to_array(s_xml_type_parsing_map));
	if (idx >= array_size(s_xml_type_parsing_map))
		return xml_type::abstract;
	else
		return s_xml_type_parsing_map[idx].value;
}

cstr_t xml_type_t::to_string()const
{
	auto idx = collections::algorithms::binary_search(_value, collections::to_array(s_xml_type_to_string_map));
	if (idx >= array_size(s_xml_type_to_string_map))
		return "abstract"_s;
	else
		return s_xml_type_to_string_map[idx].value;
}

ANG_IMPLEMENT_ENUM(xml, xml_exception_code, uint, xml_exception_code::unknown);

cstr_t xml_exception_code_t::to_string()const
{
	return ang::move(except_code_t((except_code)_value).to_string());
}

ANG_IMPLEMENT_FLAGS(ang::xml, xml_format, uint);

///////////////////////////////////////////////////////////////////


template<> typename smart_ptr_type<xml_text>::smart_ptr_t  ixml_object::xml_as<xml_text>() {
	return xml_is_type_of(xml_type::text) ? static_cast<xml_text*>(this) : null;
}

template<> typename smart_ptr_type<ixml_node>::smart_ptr_t  ixml_object::xml_as<ixml_node>() {
	return (xml_is_type_of(xml_type::node) || xml_is_type_of(xml_type::text))? static_cast<ixml_node*>(this) : null;
}

template<> typename smart_ptr_type<xml_node>::smart_ptr_t  ixml_object::xml_as<xml_node>() {
	return xml_is_type_of(xml_type::node) ? static_cast<xml_node*>(this) : null;
}

template<> typename smart_ptr_type<xml_attribute>::smart_ptr_t  ixml_object::xml_as<xml_attribute>() {
	return xml_is_type_of(xml_type::attribute) ? static_cast<xml_attribute*>(this) : null;
}

template<> typename smart_ptr_type<xml_header>::smart_ptr_t  ixml_object::xml_as<xml_header>() {
	return xml_is_type_of(xml_type::header) ? static_cast<xml_header*>(this) : null;
}

template<> typename smart_ptr_type<xml_comment>::smart_ptr_t  ixml_object::xml_as<xml_comment>() {
	return xml_is_type_of(xml_type::comment) ? static_cast<xml_comment*>(this) : null;
}

template<> typename smart_ptr_type<xml_element>::smart_ptr_t  ixml_object::xml_as<xml_element>() {
	return xml_is_type_of(xml_type::element) ? static_cast<xml_element*>(this) : null;
}

template<> typename smart_ptr_type<ixml_store>::smart_ptr_t  ixml_object::xml_as<ixml_store>() {
	return xml_is_type_of(xml_type::store) || xml_is_type_of(xml_type::tree) ? static_cast<ixml_store*>(this) : null;
}

template<> typename smart_ptr_type<xml_node::xml_store>::smart_ptr_t  ixml_object::xml_as<xml_node::xml_store>() {
	return xml_is_type_of(xml_type::store) ? static_cast<xml_node::xml_store*>(this) : null;
}

template<> typename smart_ptr_type<ixml_document>::smart_ptr_t  ixml_object::xml_as<ixml_document>() {
	return xml_is_type_of(xml_type::document) ? static_cast<ixml_document*>(this) : null;
}

template<> typename smart_ptr_type<xml_document>::smart_ptr_t  ixml_object::xml_as<xml_document>() {
	return xml_is_type_of(xml_type::document) ? static_cast<xml_document*>(this) : null;
}

/////////////////////////////////////////////////////////////////////////////////////////////////


weak_ptr<ixml_node>::weak_ptr() : safe_pointer() {}
weak_ptr<ixml_node>::weak_ptr(weak_ptr&& other) : safe_pointer((safe_pointer&&)other) {}
weak_ptr<ixml_node>::weak_ptr(weak_ptr const& other) : safe_pointer((safe_pointer const&)other) {}
weak_ptr<ixml_node>::weak_ptr(ang::nullptr_t const&) : safe_pointer(null) {}
weak_ptr<ixml_node>::weak_ptr(ixml_node* obj) : safe_pointer(obj) {}
weak_ptr<ixml_node>::weak_ptr(ixml_node_t obj) : safe_pointer(obj.get()) {}
weak_ptr<ixml_node>::~weak_ptr() { }
weak_ptr<ixml_node>& weak_ptr<ixml_node>::operator = (ixml_node_t obj) { safe_pointer::operator=(obj.get()); return *this; }
weak_ptr<ixml_node>& weak_ptr<ixml_node>::operator = (ixml_node* obj) { safe_pointer::operator=(obj);  return *this; }
weak_ptr<ixml_node>& weak_ptr<ixml_node>::operator = (weak_ptr&& other) { safe_pointer::operator=(other); return *this; }
weak_ptr<ixml_node>& weak_ptr<ixml_node>::operator = (weak_ptr const& other) { safe_pointer::operator=(other);  return *this; }
weak_ptr<ixml_node>& weak_ptr<ixml_node>::operator = (ang::nullptr_t const&) { safe_pointer::operator=(null); return *this; }
ixml_node_t weak_ptr<ixml_node>::lock() {
	auto ptr = safe_pointer::lock<intfptr>();
	return reinterpret_cast<ixml_node*>(ptr.get());
}


weak_ptr<ixml_document>::weak_ptr() : safe_pointer() {}
weak_ptr<ixml_document>::weak_ptr(weak_ptr&& other) : safe_pointer((safe_pointer&&)other) {}
weak_ptr<ixml_document>::weak_ptr(weak_ptr const& other) : safe_pointer((safe_pointer const&)other) {}
weak_ptr<ixml_document>::weak_ptr(ang::nullptr_t const&) : safe_pointer(null) {}
weak_ptr<ixml_document>::weak_ptr(ixml_document* obj) : safe_pointer(obj) {}
weak_ptr<ixml_document>::weak_ptr(ixml_document_t obj) : safe_pointer(obj.get()) {}
weak_ptr<ixml_document>::~weak_ptr() { }
weak_ptr<ixml_document>& weak_ptr<ixml_document>::operator = (ixml_document_t obj) { safe_pointer::operator=(obj.get()); return *this; }
weak_ptr<ixml_document>& weak_ptr<ixml_document>::operator = (ixml_document* obj) { safe_pointer::operator=(obj);  return *this; }
weak_ptr<ixml_document>& weak_ptr<ixml_document>::operator = (weak_ptr&& other) { safe_pointer::operator=(other); return *this; }
weak_ptr<ixml_document>& weak_ptr<ixml_document>::operator = (weak_ptr const& other) { safe_pointer::operator=(other);  return *this; }
weak_ptr<ixml_document>& weak_ptr<ixml_document>::operator = (ang::nullptr_t const&) { safe_pointer::operator=(null); return *this; }
ixml_document_t weak_ptr<ixml_document>::lock() {
	auto ptr = safe_pointer::lock<intfptr>();
	return reinterpret_cast<ixml_document*>(ptr.get());
}


ang::intf_wrapper<ixml_object>::intf_wrapper() : _ptr(null) {

}

ang::intf_wrapper<ixml_object>::intf_wrapper(ixml_object* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<ixml_object>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ixml_object * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<ixml_object>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::intf_wrapper<ixml_object>::intf_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::intf_wrapper<ixml_object>::~intf_wrapper() {
	clean();
}

void ang::intf_wrapper<ixml_object>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::intf_wrapper<ixml_object>::is_empty()const
{
	return _ptr == null;
}

ixml_object* ang::intf_wrapper<ixml_object>::get(void)const
{
	return _ptr;
}

void ang::intf_wrapper<ixml_object>::set(ixml_object* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::intf_wrapper<ixml_object>& ang::intf_wrapper<ixml_object>::operator = (ixml_object* ptr)
{
	set(ptr);
	return*this;
}

ang::intf_wrapper<ixml_object>& ang::intf_wrapper<ixml_object>::operator = (ang::intf_wrapper<ixml_object> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::intf_wrapper<ixml_object>& ang::intf_wrapper<ixml_object>::operator = (ang::intf_wrapper<ixml_object> const& other)
{
	set(other._ptr);
	return*this;
}

ixml_object ** ang::intf_wrapper<ixml_object>::addres_of(void)
{
	return &_ptr;
}

ang::intf_wrapper_ptr<ixml_object> ang::intf_wrapper<ixml_object>::operator & (void)
{
	return this;
}

ixml_object * ang::intf_wrapper<ixml_object>::operator -> (void)
{
	return get();
}

ixml_object const* ang::intf_wrapper<ixml_object>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<ixml_object>::operator xml::ixml_object * (void) { return _ptr; }

ang::intf_wrapper<ixml_object>::operator xml::ixml_object const* (void)const { return _ptr; }