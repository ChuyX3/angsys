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




ang::intf_wrapper<ixml_store>::intf_wrapper() : _ptr(null) {

}

ang::intf_wrapper<ixml_store>::intf_wrapper(ixml_store* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<ixml_store>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ixml_store * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<ixml_store>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::intf_wrapper<ixml_store>::intf_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}


ang::intf_wrapper<ixml_store>::~intf_wrapper() {
	clean();
}

void ang::intf_wrapper<ixml_store>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::intf_wrapper<ixml_store>::is_empty()const
{
	return _ptr == null;
}

ixml_store* ang::intf_wrapper<ixml_store>::get(void)const
{
	return _ptr;
}

void ang::intf_wrapper<ixml_store>::set(ixml_store* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::intf_wrapper<ixml_store>& ang::intf_wrapper<ixml_store>::operator = (ixml_store* ptr)
{
	set(ptr);
	return*this;
}

ang::intf_wrapper<ixml_store>& ang::intf_wrapper<ixml_store>::operator = (ang::intf_wrapper<ixml_store> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::intf_wrapper<ixml_store>& ang::intf_wrapper<ixml_store>::operator = (ang::intf_wrapper<ixml_store> const& other)
{
	set(other._ptr);
	return*this;
}

ixml_store ** ang::intf_wrapper<ixml_store>::addres_of(void)
{
	return &_ptr;
}

ang::intf_wrapper_ptr<ixml_store> ang::intf_wrapper<ixml_store>::operator & (void)
{
	return this;
}

ixml_store * ang::intf_wrapper<ixml_store>::operator -> (void)
{
	return get();
}

ixml_store const* ang::intf_wrapper<ixml_store>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<ixml_store>::operator xml::ixml_store * (void) { return _ptr; }

ang::intf_wrapper<ixml_store>::operator xml::ixml_store const* (void)const { return _ptr; }


ixml_node_t intf_wrapper<ixml_store>::operator[](raw_str_t value)const
{
	xml_iterator_t it;
	if (_ptr && _ptr->counter() > 0)
		it = _ptr->find(value);
	return it.is_valid() ? (ixml_node*)it : null;
}

/////////////////////////////////////////////////////////////////////////////////////


xml_node::xml_store::xml_store(xml_node_t node, xml_type_t type)
	: _xml_type(type)
	, _count(0)
	, _xml_parent(node)
	, _xml_first(null)
	, _xml_last(null)
{
}

xml_node::xml_store::~xml_store() { clean(); }

ANG_IMPLEMENT_CLASSNAME(ang::xml::xml_node::xml_store);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::xml_node::xml_store);

bool xml_node::xml_store::is_inherited_of(type_name_t name)
{
	return name == type_of<xml_store>()
		|| object::is_inherited_of(name)
		|| ixml_store::is_inherited_of(name);
}

bool xml_node::xml_store::is_kind_of(type_name_t name)const
{
	return name == type_of<xml_node::xml_store>()
		|| object::is_kind_of(name)
		|| ixml_store::is_kind_of(name);
}

bool xml_node::xml_store::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<xml_node::xml_store>())
	{
		*out = static_cast<xml_node::xml_store*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (ixml_store::query_object(name, out))
	{
		return true;
	}
	return false;
}

void xml_node::xml_store::clean()
{
	while (pop());
}

xml_type_t xml_node::xml_store::xml_type()const
{
	return _xml_type;
}

bool xml_node::xml_store::xml_is_type_of(xml_type_t type)const
{
	return _xml_type == type || xml_type_t::store == type;
}

wstring& xml_node::xml_store::xml_print(wstring& stream, const xml_format_t& flag, ushort level)const
{
	switch (_xml_type)
	{
	case xml_type_t::attribute_list:
		for (auto it = begin(); it.is_valid(); ++it)
			it->xml_print(stream, flag, level) << L" "_s;
		break;
	case xml_type_t::element_list:
		for (auto it = begin(); it.is_valid(); ++it)
		{
			if (flag.is_active(xml_format::wrap_text_tab)
				|| flag.is_active(xml_format::wrap_text_space))
				stream << L"\n"_s;
			it->xml_print(stream, flag, level);
		}
		break;
	}
	return stream;
}

ixml_node_t xml_node::xml_store::xml_parent()const
{
	return _xml_parent.lock();
}

ixml_document_t xml_node::xml_store::xml_parent_doc()const
{
	auto parent = xml_parent();
	return parent.is_empty() ? null : parent->xml_parent_doc();
}


ixml_node* xml_node::xml_store::xml_first()const
{
	return _xml_first;
}

ixml_node* xml_node::xml_store::xml_last()const
{
	return _xml_last;
}


void xml_node::xml_store::xml_first(ixml_node* node)
{
	if (_xml_first == node) return;
	auto _old = _xml_first;
	_xml_first = (node) ? node->xml_as<xml_node>().get() : null;
	if (_xml_first)_xml_first->add_ref();
	if (_old)_old->release();
}


void xml_node::xml_store::xml_last(ixml_node* node)
{
	if (_xml_last == node) return;
	auto _old = _xml_last;
	_xml_last = (node) ? node->xml_as<xml_node>().get() : null;
	if (_xml_last)_xml_last->add_ref();
	if (_old)_old->release();
}

bool xml_node::xml_node::xml_store::push(ixml_node_t node, bool last)
{
	if (node.is_empty() 
		|| (xml_is_type_of(xml_type::attribute_list) && !node->xml_is_type_of(xml_type::attribute))
		|| (xml_is_type_of(xml_type::element_list) && !node->xml_is_type_of(xml_type::element) && !node->xml_is_type_of(xml_type::comment))
		) return false;

	node->xml_parent(_xml_parent.lock());
	if (last)
	{
		if (_xml_first == null)
		{
			xml_first(node);
			xml_last(node);
		}
		else
		{
			xml_last()->xml_next_sibling(node);
			node->xml_prev_sibling(xml_last());
			xml_last(node);
		}
	}
	else
	{
		if (_xml_first == null)
		{
			xml_first(node);
			xml_last(node);
		}
		else
		{
			xml_first()->xml_prev_sibling(node);
			node->xml_next_sibling(xml_first());
			xml_first(node);
		}
	}
	_count++;
	return true;
}

bool xml_node::xml_store::insert(ixml_node_t node, xml_iterator_t at, bool nextTo)
{
	if (node.is_empty()
		|| (xml_is_type_of(xml_type::attribute_list) && !node->xml_is_type_of(xml_type::attribute))
		|| (xml_is_type_of(xml_type::element_list) && !node->xml_is_type_of(xml_type::element) && !node->xml_is_type_of(xml_type::comment))
		) return false;

	if (node == null)
		return false;

	if (!at.is_valid() || at.parent() != this)
	{
		return push(node, nextTo);
	}

	if (nextTo && *at == xml_last())
		return push(node, true);

	if (!nextTo && *at == xml_first())
		return push(node, false);

	node->xml_parent(_xml_parent.lock());
	if (nextTo)
	{
		at->xml_next_sibling(node);
		node->xml_prev_sibling(*at);
	}
	else
	{
		at->xml_prev_sibling(node);
		node->xml_next_sibling(*at);
	}
	_count++;
	return true;
}

bool xml_node::xml_store::pop(ixml_node_ptr_t out, bool last)
{
	if (_count == 0)
		return false;
	if (_count == 1)
	{
		if (!out.is_empty())
		{
			*out = xml_first();
		}
		xml_first(null);
		xml_last(null);
	}
	else
	{
		ixml_node_t temp = null;
		if (last)
		{
			temp = xml_last();
			if (!out.is_empty())
			{
				*out = temp;
			}
			temp->xml_prev_sibling()->xml_next_sibling(null);
			xml_last(temp->xml_prev_sibling());
			temp->xml_prev_sibling(null);
		}
		else
		{
			temp = xml_first();
			if (!out.is_empty())
			{
				*out = temp;
			}
			temp->xml_next_sibling()->xml_prev_sibling(null);
			xml_first(temp->xml_prev_sibling());
			temp->xml_next_sibling(null);
		}
	}
	_count--;
	return true;
}

bool xml_node::xml_store::pop_at(xml_iterator_t it, ixml_node_ptr_t out)
{
	if (_count == 0)
		return false;

	if (it.parent() != this)
		return false;
	if (it.current() == null)
		return false;

	ixml_node_t temp = it;
	if (!out.is_empty())
	{
		*out = temp;
	}

	if (temp->xml_prev_sibling() != null)
		temp->xml_prev_sibling()->xml_next_sibling(temp->xml_next_sibling());

	if (temp->xml_next_sibling() != null)
		temp->xml_next_sibling()->xml_prev_sibling(temp->xml_prev_sibling());

	if (temp == xml_first())
		xml_first(temp->xml_next_sibling());
	if (temp == xml_last())
		xml_last(temp->xml_prev_sibling());

	temp->xml_next_sibling(null);
	temp->xml_prev_sibling(null);

	_count--;
	return true;
}

wsize xml_node::xml_store::counter()const
{
	return _count;
}

ixml_node_t xml_node::xml_store::at(const xml_base_iterator_t& it)
{
	if (!counter()) throw(ang::exception_t(except_code::invalid_memory));
	if (it.parent() != this)throw(ang::exception_t(except_code::invalid_param));
	return reinterpret_cast<ixml_node*>(it.current());
}

bool xml_node::xml_store::increase(xml_base_iterator_t& it)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	it.current(reinterpret_cast<ixml_node*>(it.current())->xml_next_sibling());
	if (it.current() == null)
		return false;
	return true;
}

bool xml_node::xml_store::increase(xml_base_iterator_t& it, int offset)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	auto node = reinterpret_cast<ixml_node*>(it.current());
	int c = 0;
	while (node && c < offset) {
		node = node->xml_next_sibling();
		c++;
	}
	it.current(node);
	if (it.current() == null)
		return false;
	return true;
}

bool xml_node::xml_store::decrease(xml_base_iterator_t&it)const
{
	if (it.parent() != this) throw(ang::exception_t(except_code::invalid_memory));
	it.current(reinterpret_cast<ixml_node*>(it.current())->xml_prev_sibling());
	if (it.current() == null)
		return false;
	return true;
}

bool xml_node::xml_store::decrease(xml_base_iterator_t&it, int offset)const
{
	if (it.parent() != this) throw(ang::exception_t(except_code::invalid_memory));
	auto node = reinterpret_cast<ixml_node*>(it.current());
	int c = 0;
	while (node && c < offset) {
		node = node->xml_prev_sibling();
		c++;
	}
	it.current(node);
	if (it.current() == null)
		return false;
	return true;
}

xml_forward_iterator_t xml_node::xml_store::begin()
{
	return xml_iterator_t(const_cast<xml_store*>(this), (ixml_node*)xml_first());
}

xml_forward_iterator_t xml_node::xml_store::end()
{
	return xml_iterator_t(const_cast<xml_store*>(this), null);
}

xml_const_forward_iterator_t xml_node::xml_store::begin()const
{
	return xml_iterator_t(const_cast<xml_store*>(this), (ixml_node*)xml_first());
}

xml_const_forward_iterator_t xml_node::xml_store::end()const
{
	return xml_iterator_t(const_cast<xml_store*>(this), null);
}

xml_backward_iterator_t xml_node::xml_store::rbegin()
{
	return xml_iterator_t(const_cast<xml_store*>(this), (ixml_node*)xml_last());
}

xml_backward_iterator_t xml_node::xml_store::rend()
{
	return xml_iterator_t(const_cast<xml_store*>(this), null);
}

xml_const_backward_iterator_t xml_node::xml_store::rbegin()const
{
	return xml_iterator_t(const_cast<xml_store*>(this), (ixml_node*)xml_last());
}

xml_const_backward_iterator_t xml_node::xml_store::rend()const
{
	return xml_iterator_t(const_cast<xml_store*>(this), null);
}

xml_iterator_t xml_node::xml_store::find(raw_str_t value, bool invert)const
{
	ixml_node_t node = null;
	if (invert)
	{
		for (node = xml_last();
			(node != null);
			node = node->xml_prev_sibling()) {
			if (text::UNICODE().compare((cwstr_t)(wstring)node->xml_name(), value._value, value._encoding) == 0)
				break;
		}
	}
	else
	{
		for (node = xml_first();
			(node != null);
			node = node->xml_next_sibling()) {
			if (text::UNICODE().compare((cwstr_t)(wstring)node->xml_name(), value._value, value._encoding) == 0)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_store*>(this), node);
}

xml_iterator_t xml_node::xml_store::find(raw_str_t value, xml_iterator_t nextTo, bool invert)const
{
	ixml_node_t node = null;

	if (nextTo.is_valid() && nextTo.parent() == this)
		node = reinterpret_cast<xml_node*>(nextTo.current());
	if (invert)
	{
		if (node == null)
			node = xml_last();
		for (; (node != null);
			node = node->xml_prev_sibling()) {
			if (text::UNICODE().compare((cwstr_t)(wstring)node->xml_name(), value._value, value._encoding) == 0)
				break;
		}
	}
	else
	{
		if (node == null)
			node = xml_first();
		for (; (node != null);
			node = node->xml_next_sibling()) {
			if (text::UNICODE().compare((cwstr_t)(wstring)node->xml_name(), value._value, value._encoding) == 0)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_store*>(this), node);
}

