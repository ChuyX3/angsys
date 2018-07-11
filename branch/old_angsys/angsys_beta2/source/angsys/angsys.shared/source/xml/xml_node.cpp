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

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


ang::intf_wrapper<ixml_node>::intf_wrapper() : _ptr(null) {

}

ang::intf_wrapper<ixml_node>::intf_wrapper(ixml_node* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<ixml_node>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ixml_node * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<ixml_node>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::intf_wrapper<ixml_node>::intf_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::intf_wrapper<ixml_node>::~intf_wrapper() {
	clean();
}

void ang::intf_wrapper<ixml_node>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::intf_wrapper<ixml_node>::is_empty()const
{
	return _ptr == null;
}

ixml_node* ang::intf_wrapper<ixml_node>::get(void)const
{
	return _ptr;
}

void ang::intf_wrapper<ixml_node>::set(ixml_node* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::intf_wrapper<ixml_node>& ang::intf_wrapper<ixml_node>::operator = (ixml_node* ptr)
{
	set(ptr);
	return*this;
}

ang::intf_wrapper<ixml_node>& ang::intf_wrapper<ixml_node>::operator = (ang::intf_wrapper<ixml_node> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::intf_wrapper<ixml_node>& ang::intf_wrapper<ixml_node>::operator = (ang::intf_wrapper<ixml_node> const& other)
{
	set(other._ptr);
	return*this;
}

ixml_node ** ang::intf_wrapper<ixml_node>::addres_of(void)
{
	return &_ptr;
}

ang::intf_wrapper_ptr<ixml_node> ang::intf_wrapper<ixml_node>::operator & (void)
{
	return this;
}

ixml_node * ang::intf_wrapper<ixml_node>::operator -> (void)
{
	return get();
}

ixml_node const* ang::intf_wrapper<ixml_node>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<ixml_node>::operator xml::ixml_node * (void) { return _ptr; }

ang::intf_wrapper<ixml_node>::operator xml::ixml_node const* (void)const { return _ptr; }

ang::intf_wrapper<ixml_node>::operator xml::xml_text_t()const { return _ptr ? _ptr->xml_value() : null; }

ixml_node_t intf_wrapper<ixml_node>::operator[](raw_str_t value)const
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




ang::object_wrapper<xml_node>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<xml_node>::object_wrapper(xml_node* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<xml_node>::object_wrapper(object_wrapper && other) : _ptr(null) {
	xml_node * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_node>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<xml_node>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_node>::~object_wrapper() {
	clean();
}

void ang::object_wrapper<xml_node>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::object_wrapper<xml_node>::is_empty()const
{
	return _ptr == null;
}

xml_node* ang::object_wrapper<xml_node>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_node>::set(xml_node* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::object_wrapper<xml_node>& ang::object_wrapper<xml_node>::operator = (xml_node* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_node>& ang::object_wrapper<xml_node>::operator = (ang::object_wrapper<xml_node> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_node>& ang::object_wrapper<xml_node>::operator = (ang::object_wrapper<xml_node> const& other)
{
	set(other._ptr);
	return*this;
}

xml_node ** ang::object_wrapper<xml_node>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_node> ang::object_wrapper<xml_node>::operator & (void)
{
	return this;
}

xml_node * ang::object_wrapper<xml_node>::operator -> (void)
{
	return get();
}

xml_node const* ang::object_wrapper<xml_node>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_node>::operator xml::xml_node * (void) { return _ptr; }

ang::object_wrapper<xml_node>::operator xml::xml_node const* (void)const { return _ptr; }

ang::object_wrapper<xml_node>::operator xml::ixml_node_t(void)const { return _ptr; }

ang::object_wrapper<xml_node>::operator xml::xml_text_t()const { return _ptr ? _ptr->xml_value() : null; }

ixml_node_t object_wrapper<xml_node>::operator[](raw_str_t value)const
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
xml_node::xml_node(ixml_document_t doc, xml_type_t type)
	: _xml_type(type)
	, _xml_parent(null)
	, _xml_parent_doc(doc)
	, _xml_prev(null)
	, _xml_next(null)
	, _xml_name(null)
	, _xml_attributes(null)
{
	_xml_content = null;
}

xml_node::~xml_node()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::xml::xml_node);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::xml_node);

bool xml_node::is_inherited_of(type_name_t className)
{
	if (className == type_of<xml_node>()
		|| object::is_inherited_of(className)
		|| ixml_node::is_inherited_of(className))
		return true;
	return false;
}

bool xml_node::is_kind_of(type_name_t className)const
{
	if (className == type_of<xml_node>()
		|| object::is_kind_of(className)
		|| ixml_node::is_kind_of(className))
		return true;
	return false;
}

bool xml_node::query_object(type_name_t className, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == type_of<xml_node>())
	{
		*out = static_cast<xml_node*>(this);
		return true;
	}
	else if (object::query_object(className, out))
	{
		return true;
	}
	else if (ixml_node::query_object(className, out))
	{
		return true;
	}
	return false;
}

void xml_node::clean()
{
	//_xml_parent;
	//_xml_prev;
	//_xml_next;
	_xml_name = null;
	_xml_attributes = null;
	_xml_content = null;
}

//template<> xml_header_t xml_node::xml_as<xml_header>() { return xml_is_type_of(xml_type::header) ? static_cast<xml_header*>(this) : null; }
//template<> xml_comment_t xml_node::xml_as<xml_comment>() { return xml_is_type_of(xml_type::comment) ? static_cast<xml_comment*>(this) : null; }
//template<> xml_element_t xml_node::xml_as<xml_element>() { return xml_is_type_of(xml_type::element) ? static_cast<xml_element*>(this) : null; }
//template<> xml_attribute_t xml_node::xml_as<xml_attribute>() { return xml_is_type_of(xml_type::attribute) ? static_cast<xml_attribute*>(this) : null; }

xml_type_t xml_node::xml_type()const
{
	return _xml_type;
}

bool xml_node::xml_is_type_of(xml_type_t type)const
{
	return (_xml_type == type || xml_type_t::node == type);
}

wstring& xml_node::xml_print(wstring& stream, xml_format_t const& flag, ushort level)const
{
	switch (_xml_type)
	{
	case xml_type_t::header:
	{
		auto version = (xml_text_t)_xml_attributes["version"_s];
		auto encoding = (xml_text_t)_xml_attributes["encoding"_s];
		auto standalone = (xml_text_t)_xml_attributes["standalone"_s];
		stream << L"<?xml "_s << L"version=\""_s << (wstring)version << L"\""_s;
		stream << L" encoding="_s << L"\""_s << (encoding.is_empty() ? L"utf-8"_s : encoding->cstr()) << L"\""_s;
		if (!standalone.is_empty())
			stream << L" standalone=\""_s << (wstring)standalone << L"\""_s;
		stream << L"?>"_s;
	}
	break;
	case xml_type_t::element:
		if (xml_has_name())
		{
			if (flag.is_active(xml_format::wrap_text_tab))
			{
				for (index i = 0; i < level; i++)
					stream << L"\t"_s;
			}
			else if (flag.is_active(xml_format::wrap_text_space))
			{
				for (index i = 0; i < level; i++)
					stream << L"  "_s;
			}

			stream << L"<"_s << (wstring)_xml_name;

			if (xml_has_attributes())
			{
				stream << L" "_s;
				_xml_attributes->xml_print(stream, flag, level);
			}

			if (!_xml_content.is_empty())
			{
				stream << L">"_s;
				_xml_content->xml_print(stream, flag, level + 1);
				if (!_xml_content->xml_is_type_of(xml_type_t::pcdata))
				{	
					stream << L"\n"_s;
					if (flag.is_active(xml_format::wrap_text_tab))
					{
						for (index i = 0; i < level; i++)
							stream << L"\t"_s;
					}
					else if (flag.is_active(xml_format::wrap_text_space))
					{
						for (index i = 0; i < level; i++)
							stream << L"  "_s;
					}
				}
				stream << L"</"_s << (wstring)_xml_name << L">"_s;
			}
			else
			{
				stream << L"/>"_s;
			}
		}
		break;
	case xml_type_t::comment:
		if (flag.is_active(xml_format::wrap_text_tab))
		{
			for (index i = 0; i < level; i++)
				stream << L"\t"_s;
		}
		else if (flag.is_active(xml_format::wrap_text_space))
		{
			for (index i = 0; i < level; i++)
				stream << L"  "_s;
		}
		stream << L"<!--"_s;
		if (xml_has_value()) _xml_content->xml_print(stream, flag, level + 1);
		else stream << " "_s;
		stream << L"-->"_s;
		break;
	case xml_type_t::attribute:
		if (xml_has_name() && xml_has_value())
		{
			stream << (wstring)_xml_name;
			xml_text_t val = _xml_content.get()->xml_as<xml_text>();
			
			if (val->find("\""_s, 0) != invalid_index)
			{
				stream << L"='"_s;
				val->xml_print(stream, xml_format::none, 0);
				stream << L"'"_s;
			}
			else
			{
				stream << L"=\""_s;
				val->xml_print(stream, xml_format::none, 0);
				stream << L"\""_s;
			}
		}
		break;
	}
	return stream;
}

bool xml_node::xml_has_name()const { return false; }
bool xml_node::xml_has_value()const { return false; }
bool xml_node::xml_has_children()const { return false; }
bool xml_node::xml_has_attributes()const { return false; }

ixml_node_t xml_node::xml_parent()const
{
	return _xml_parent.lock();
}

ixml_document_t xml_node::xml_parent_doc()const
{
	return _xml_parent_doc.lock();
}

ixml_node_t xml_node::xml_prev_sibling()const
{
	return _xml_prev;
}

ixml_node_t xml_node::xml_next_sibling()const
{
	return _xml_next;
}

void xml_node::xml_parent(ixml_node_t node)
{
	_xml_parent = node;
}


void xml_node::xml_prev_sibling(ixml_node_t node)
{
	if (_xml_prev == node) return;
	auto _old = _xml_prev;
	_xml_prev = (node) ? node->xml_as<xml_node>().get() : null;
	if (_xml_prev)_xml_prev->add_ref();
	if (_old)_old->release();
}


void xml_node::xml_next_sibling(ixml_node_t node)
{
	if (_xml_next == node) return;
	auto _old = _xml_next;
	_xml_next = (node) ? node->xml_as<xml_node>().get() : null;
	if(_xml_next)_xml_next->add_ref();
	if (_old)_old->release();
}

ixml_node_t xml_node::xml_first_child()const
{
	if(_xml_content.is_empty() || !_xml_content->xml_is_type_of(xml_type::store))
		return null;
	return *_xml_content.get()->xml_as<ixml_store>()->begin();
}

ixml_node_t xml_node::xml_last_child()const
{
	if (_xml_content.is_empty() || !_xml_content->xml_is_type_of(xml_type::store))
		return null;
	return *_xml_content.get()->xml_as<ixml_store>()->rbegin();
}

xml_text_t xml_node::xml_name()const
{
	return _xml_name.get();
}

xml_text_t xml_node::xml_value()const
{
	if (_xml_content.is_empty())
		return null;
	return _xml_content.get()->xml_as<xml_text>();
}

ixml_store_t xml_node::xml_children()const
{
	if (_xml_content.is_empty())
		return null;
	return _xml_content.get()->xml_as<ixml_store>();
}

ixml_attributes_t xml_node::xml_attributes()const
{
	return _xml_attributes.get();
}

bool xml_node::push_name(wstring value)
{
	if (_xml_type != xml_type::attribute
		&& _xml_type != xml_type::element)
		return false;
	if(_xml_name.is_empty())
		_xml_name = NEW xml_text();
	_xml_name->move(value.get());
	return true;
}

bool xml_node::push_value(wstring value)
{
	if (xml_is_type_of(xml_type::attribute) || xml_is_type_of(xml_type::comment))
	{
		if (_xml_content.is_empty() || _xml_content->xml_is_type_of(xml_type::pcdata) || _xml_content->xml_is_type_of(xml_type::cdata))
			_xml_content = NEW xml_text();
		_xml_content->xml_as<xml_text>()->move(value.get());
		return true;
	}
	else if (xml_is_type_of(xml_type::element))
	{
		if (_xml_content.is_empty() || !_xml_content->xml_is_type_of(xml_type::pcdata) )
			_xml_content = NEW xml_pcdata();
		_xml_content->xml_as<xml_text>()->move(value.get());
		return true;
	}
	return false;
}

bool xml_node::push_data(wstring value)
{
	if (_xml_type != xml_type::element)
		return false;
	if (_xml_content.is_empty() || !_xml_content->xml_is_type_of(xml_type::cdata))
		_xml_content = NEW xml_cdata();
	_xml_content->xml_as<xml_text>()->move(value.get());
	return true;
}

xml_iterator_t xml_node::push_attribute(xml_attribute_t attribute)
{
	if (attribute.is_empty())
		return xml_iterator_t(null, null);

	if (_xml_attributes.is_empty() || !_xml_attributes->xml_is_type_of(xml_type::attribute_list))
		_xml_attributes = NEW xml_store(this, xml_type::attribute_list);
	return _xml_attributes->xml_as<xml_store>()->push(attribute.get())
		? xml_iterator_t(static_cast<ixml_store*>(_xml_attributes.get()), (ixml_node*)attribute.get())
		: xml_iterator_t(null, null);
}

xml_iterator_t xml_node::push_attribute(xml_attribute_t attribute, xml_iterator_t next)
{
	if (attribute.is_empty())
		return xml_iterator_t(null, null);

	if (_xml_attributes.is_empty() || !_xml_attributes->xml_is_type_of(xml_type::attribute_list))
		_xml_attributes = NEW xml_store(this, xml_type::attribute_list);
	return _xml_attributes->xml_as<xml_store>()->insert(attribute.get(), next, true)
		? xml_iterator_t(static_cast<ixml_store*>(_xml_attributes.get()), (ixml_node*)attribute.get())
		: xml_iterator_t(null, null);
}

bool xml_node::push_attributes(ixml_store_t attributes)
{
	if (attributes.is_empty() || !attributes->xml_is_type_of(xml_type::attribute_list))
		return false;
	if (_xml_attributes.is_empty() || !_xml_attributes->xml_is_type_of(xml_type::attribute_list))
		_xml_attributes = NEW xml_store(this, xml_type::element_list);
	auto store = _xml_attributes->xml_as<xml_store>();
	auto doc = xml_parent_doc();
	for (auto it = attributes->begin(); it != attributes->end(); it++)
		store->push(it->xml_clone(doc));
	return true;
}


xml_iterator_t xml_node::push_child(xml_node_t element)
{
	if (element.is_empty() || (!element->xml_is_type_of(xml_type::element) && !element->xml_is_type_of(xml_type::comment)))
		return xml_iterator_t(null, null);

	if (_xml_content.is_empty() || !_xml_content->xml_is_type_of(xml_type::element_list))
		_xml_content = NEW xml_store(this, xml_type::element_list);
	return _xml_content->xml_as<xml_store>()->push(element.get())
		? xml_iterator_t(static_cast<ixml_store*>(_xml_content.get()), (ixml_node*)element.get())
		: xml_iterator_t(null, null);
}

xml_iterator_t xml_node::push_child(xml_node_t element, xml_iterator_t next)
{
	if (element.is_empty() || (!element->xml_is_type_of(xml_type::element) && !element->xml_is_type_of(xml_type::comment)))
		return xml_iterator_t(null, null);

	if (_xml_content.is_empty() || !_xml_content->xml_is_type_of(xml_type::element_list))
		_xml_content = NEW xml_store(this, xml_type::element_list);
	return _xml_content->xml_as<xml_store>()->insert(element.get(), next, true)
		? xml_iterator_t(static_cast<ixml_store*>(_xml_content.get()), (ixml_node*)element.get())
		: xml_iterator_t(null, null);
}

bool xml_node::push_children(ixml_store_t children)
{
	if (children.is_empty() || !children->xml_is_type_of(xml_type::element_list))
		return false;
	if (_xml_content.is_empty() || !_xml_content->xml_is_type_of(xml_type::element_list))
		_xml_content = NEW xml_store(this, xml_type::element_list);
	auto store = _xml_content->xml_as<xml_store>();
	auto doc = xml_parent_doc();
	for (auto it = children->begin(); it != children->end(); it++)
		store->push(it->xml_clone(doc));
	return true;
}
