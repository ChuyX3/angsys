/*********************************************************************************************************************/
/*   File Name: xml_items.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/xml.h"

using namespace ang;
using namespace ang::xml;

ang::object_wrapper<xml_items>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<xml_items>::object_wrapper(xml_items* ptr)
	: _ptr(null)
{
	set(ptr);
}

ang::object_wrapper<xml_items>::object_wrapper(object_wrapper && other)
	: _ptr(null)
{
	xml_items * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_items>::object_wrapper(object_wrapper const& other)
	: _ptr(null)
{
	set(other._ptr);
}

ang::object_wrapper<xml_items>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_items>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<xml_items>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<xml_items>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<xml_items>::is_empty()const
{
	return _ptr == null;
}

xml_items* ang::object_wrapper<xml_items>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_items>::set(xml_items* ptr)
{
	xml_items * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<xml_items>& ang::object_wrapper<xml_items>::operator = (xml_items* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_items>& ang::object_wrapper<xml_items>::operator = (ang::object_wrapper<xml_items> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_items>& ang::object_wrapper<xml_items>::operator = (ang::object_wrapper<xml_items> const& other)
{
	set(other._ptr);
	return*this;
}

object_wrapper<xml_items>&  ang::object_wrapper<xml_items>::operator += (xml_node_t node)
{
	if (_ptr)_ptr->push(node);
	return*this;
}

xml_items ** ang::object_wrapper<xml_items>::addres_of(void)
{
	return &_ptr;
}

xml_forward_iterator_t ang::object_wrapper<xml_items>::begin()
{
	return _ptr ? _ptr->begin() : xml_forward_iterator_t();
}

xml_forward_iterator_t ang::object_wrapper<xml_items>::end()
{
	return _ptr ? _ptr->end() : xml_forward_iterator_t();
}

xml_const_forward_iterator_t ang::object_wrapper<xml_items>::begin()const
{
	return _ptr ? ((const xml_items*)_ptr)->begin() : xml_const_forward_iterator_t();
}

xml_const_forward_iterator_t ang::object_wrapper<xml_items>::end()const
{
	return _ptr ? ((const xml_items*)_ptr)->end() : xml_const_forward_iterator_t();
}

ang::object_wrapper_ptr<xml_items> ang::object_wrapper<xml_items>::operator& (void)
{
	return this;
}

xml_items * ang::object_wrapper<xml_items>::operator -> (void)
{
	return get();
}

xml_items const* ang::object_wrapper<xml_items>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_items>::operator xml_items * (void)
{
	return get();
}

ang::object_wrapper<xml_items>::operator xml_items const* (void)const
{
	return get();
}

xml_node_t ang::object_wrapper<xml_items>::operator[](cstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? *it : null;
}

xml_node_t ang::object_wrapper<xml_items>::operator[](cwstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? *it : null;
}

xml_node_t ang::object_wrapper<xml_items>::operator[](cmstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? *it : null;
}

////////////////////////////////////////////////////////////////////////////////

xml_items::xml_items(xml_type_t type)
	: _xml_type(type)
	, _count(0)
	, _xml_parent(null)
	, _xml_first(null)
	, _xml_last(null)
{	
}

xml_items::~xml_items() { clean(); }

ANG_IMPLEMENT_CLASSNAME(ang::xml::xml_items);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::xml_items);

bool xml_items::is_child_of(type_name_t name)
{
	if (name == type_of<xml_items>()
		|| object::is_child_of(name)
		|| ixmlobject::is_child_of(name)
		|| collections::ienum<xml_node>::is_child_of(name))
		return true;
	return false;
}

bool xml_items::is_kind_of(type_name_t name)const
{
	if (name == type_of<xml_items>()
		|| object::is_kind_of(name)
		|| ixmlobject::is_kind_of(name)
		|| collections::ienum<xml_node>::is_kind_of(name))
		return true;
	return false;
}

bool xml_items::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<xml_items>())
	{
		*out = static_cast<xml_items*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (ixmlobject::is_kind_of(name))
	{
		*out = static_cast<ixmlobject*>(this);
		return true;
	}
	else if (collections::ienum<xml_node>::is_kind_of(name))
	{
		*out = static_cast<collections::ienum<xml_node>*>(this);
		return true;
	}
	return false;
}

void xml_items::clean()
{
	while (pop());
}

template<> xml_element_list_t xml_items::xml_as<xml_element_list>() { return xml_is_type_of(xml_type::element_list) ? static_cast<xml_element_list*>(this) : null; }
template<> xml_attributes_t xml_items::xml_as<xml_attribute_list>() { return xml_is_type_of(xml_type::attribute_list) ? static_cast<xml_attribute_list*>(this) : null; }

xml_type_t xml_items::xml_type()const
{
	return _xml_type;
}

bool xml_items::xml_is_type_of(xml_type_t type)const
{
	return _xml_type == type || xml_type_t::store == type;
}

wstring& xml_items::xml_print(wstring& stream, const xml_format_t& flag, ushort level)const
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

xml_node* xml_items::xml_first()const
{
	return _xml_first.get();
}

xml_node* xml_items::xml_last()const
{
	return _xml_last.get();
}

void xml_items::xml_first(xml_node* node)
{
	_xml_first = node;
}

void xml_items::xml_last(xml_node* node)
{
	_xml_last = node;
}

void xml_items::copy(const ixml_items* items)
{
	clean();
	if (items == null)
		return;
	for (xml_node_t node : *items)
		push(node, true);
}

void xml_items::extend(const ixml_items* items)
{
	if (items == null)
		return;
	for (xml_node_t node : *items)
		push(node, true);
}

void xml_items::push(xml_node_t node, bool last)
{
	if (node.is_empty())
		return;
	auto _p = _xml_parent.lock();
	node->xml_parent(_p.get());
	if (last)
	{
		if (_xml_first == null)
		{
			xml_first(node);
			xml_last(node);
		}
		else
		{
			xml_last()->xml_next(node);
			node->xml_prev(xml_last());
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
			xml_first()->xml_prev(node);
			node->xml_next(xml_first());
			xml_first(node);
		}
	}
	_count++;
}

bool xml_items::insert(xml_node_t node, xml_iterator_t at, bool nextTo)
{
	if (node == null)
		return false;

	if (!at.is_valid() || at.parent() != this)
	{
		push(node, nextTo);
		return true;
	}
	else if (nextTo && *at == xml_last())
	{
		push(node, true);
		return true;
	}
	else if (!nextTo && *at == xml_first())
	{
		push(node, false);
		return true;
	}
	auto _p = _xml_parent.lock();
	node->xml_parent(_p.get());
	if (nextTo)
	{
		at->xml_next(node);
		node->xml_prev(*at);
	}
	else
	{
		at->xml_prev(node);
		node->xml_next(*at);
	}
	_count++;
	return true;
}

bool xml_items::insert(xml_node_t node, index idx)
{
	xml_node_t _node = null;
	if (node == null)
		return false;
	else if (idx >= _count)
	{
		push(node, true);
		return true;
	}
	else if (idx > _count / 2)
	{
		idx = _count - idx - 1;
		_node = xml_last();
		for (uint i = 0; i < idx && _node; i++)
			_node = _node->xml_prev();
	}
	else
	{
		_node = xml_first();
		for (uint i = 0; i < idx && _node; i++)
			_node = _node->xml_next();
	}

	if (!_node) return false;

	auto _p = _xml_parent.lock();
	node->xml_parent(_p.get());
	_node->xml_prev(node);
	node->xml_next(_node);
	return true;
}

bool xml_items::pop(xml_node_ptr_t out, bool last)
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
		xml_node_t temp = null;
		if (last)
		{
			temp = xml_last();
			if (!out.is_empty())
			{
				*out = temp;
			}
			temp->xml_prev()->xml_next(null);
			xml_last(temp->xml_prev());
			temp->xml_prev(null);
		}
		else
		{
			temp = xml_first();
			if (!out.is_empty())
			{
				*out = temp;
			}
			temp->xml_next()->xml_prev(null);
			xml_first(temp->xml_prev());
			temp->xml_next(null);
		}
	}
	_count--;
	return true;
}

bool xml_items::pop_at(xml_iterator_t it, xml_node_ptr_t out)
{
	if (_count == 0)
		return false;

	if (it.parent() != this)
		return false;
	if (it.current() == null)
		return false;

	xml_node_t temp = *it;
	if (!out.is_empty())
	{
		*out = temp;
	}

	if (temp->xml_prev() != null)
		temp->xml_prev()->xml_next(temp->xml_next());

	if (temp->xml_next() != null)
		temp->xml_next()->xml_prev(temp->xml_prev());

	if (temp == xml_first())
		xml_first(temp->xml_next());
	if (temp == xml_last())
		xml_last(temp->xml_prev());

	temp->xml_next(null);
	temp->xml_prev(null);

	_count--;
	return true;
}

uint xml_items::counter()const
{
	return _count;
}

xml_node_t xml_items::at(const xml_base_iterator_t& it)
{
	if (!counter() || it.parent() != this) return null;
	return reinterpret_cast<xml_node*>(it.current());
}

bool xml_items::increase(xml_base_iterator_t& it)const
{
	if (!counter() || it.parent() != this) return false;
	it.current(reinterpret_cast<xml_node*>(it.current())->xml_next());
	if (it.current() == null)
		return false;
	return true;
}

bool xml_items::increase(xml_base_iterator_t& it, int offset)const
{
	if (!counter() || it.parent() != this) return false;
	auto node = reinterpret_cast<xml_node*>(it.current());
	int c = 0;
	while (node && c < offset) {
		node = node->xml_next();
		c++;
	}
	it.current(node);
	if (it.current() == null)
		return false;
	return true;
}

bool xml_items::decrease(xml_base_iterator_t&it)const
{
	if (!counter() || it.parent() != this) return false;
	it.current(reinterpret_cast<xml_node*>(it.current())->xml_prev());
	if (it.current() == null)
		return false;
	return true;
}

bool xml_items::decrease(xml_base_iterator_t&it, int offset)const
{
	if (!counter() || it.parent() != this) return false;
	auto node = reinterpret_cast<xml_node*>(it.current());
	int c = 0;
	while (node && c < offset) {
		node = node->xml_prev();
		c++;
	}
	it.current(node);
	if (it.current() == null)
		return false;
	return true;
}


xml_forward_iterator_t xml_items::begin()
{
	return xml_forward_iterator_t(const_cast<xml_items*>(this), xml_first());
}

xml_forward_iterator_t xml_items::end()
{
	return xml_forward_iterator_t(const_cast<xml_items*>(this), null);
}

xml_const_forward_iterator_t xml_items::begin()const
{
	return xml_const_forward_iterator_t(const_cast<xml_items*>(this), xml_first());
}

xml_const_forward_iterator_t xml_items::end()const
{
	return xml_const_forward_iterator_t(const_cast<xml_items*>(this), null);
}

xml_backward_iterator_t xml_items::rbegin()
{
	return xml_backward_iterator_t(const_cast<xml_items*>(this), xml_last(), 1);
}

xml_backward_iterator_t xml_items::rend()
{
	return xml_backward_iterator_t(const_cast<xml_items*>(this), null, 1);
}

xml_const_backward_iterator_t xml_items::rbegin()const
{
	return xml_const_backward_iterator_t(const_cast<xml_items*>(this), xml_last(), 1);
}

xml_const_backward_iterator_t xml_items::rend()const
{
	return xml_const_backward_iterator_t(const_cast<xml_items*>(this), null, 1);
}

xml_iterator_t xml_items::find(xml_node_t node)const
{
	for (xml_const_forward_iterator_t it = begin(); it != end(); ++it )
	{
		if ((*it).get() == node.get())
			return it;
	}
	return end();
}

xml_iterator_t xml_items::find_first(cstr_t value, bool invert)const
{
	xml_node_t node = null;
	if (invert)
	{
		for (node = xml_last();
			(node != null);
			node = node->xml_prev()) {
			if (value == (wstring const&)node->xml_name())
				break;
		}
	}
	else
	{
		for (node = xml_first();
			(node != null);
			node = node->xml_next()) {
			if (value == (wstring const&)node->xml_name())
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_items*>(this), node);
}

xml_iterator_t xml_items::find_first(cwstr_t value, bool invert)const
{
	xml_node_t node = null;
	if (invert)
	{
		for (node = xml_last();
			(node != null);
			node = node->xml_prev()) {
			if (value == (wstring const&)node->xml_name())
				break;
		}
	}
	else
	{
		for (node = xml_first();
			(node != null);
			node = node->xml_next()) {
			if (value == (wstring const&)node->xml_name())
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_items*>(this), node);
}

xml_iterator_t xml_items::find_first(cmstr_t value, bool invert)const
{
	xml_node_t node = null;
	if (invert)
	{
		for (node = xml_last();
			(node != null);
			node = node->xml_prev()) {
			if (value == (wstring const&)node->xml_name())
				break;
		}
	}
	else
	{
		for (node = xml_first();
			(node != null);
			node = node->xml_next()) {
			if (value == (wstring const&)node->xml_name())
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_items*>(this), node);
}


xml_iterator_t xml_items::find_next(cstr_t value, xml_iterator_t nextTo, bool invert)const
{
	xml_node_t node = null;

	if (nextTo.is_valid() && nextTo.parent() == this)
		node = reinterpret_cast<xml_node*>(nextTo.current());
	if (invert)
	{
		if (node == null)
			node = xml_last();
		else
			node = node->xml_prev();
		for (; (node != null);
			node = node->xml_prev()) {

			if ((wstring const&)node->xml_name() == value)
				break;
		}
	}
	else
	{
		if (node == null)
			node = xml_first();
		else
			node = node->xml_next();
		for (; (node != null);
			node = node->xml_next()) {
			if ((wstring const&)node->xml_name() == value)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_items*>(this), node);
}

xml_iterator_t xml_items::find_next(cwstr_t value, xml_iterator_t nextTo, bool invert)const
{
	xml_node_t node = null;

	if (nextTo.is_valid() && nextTo.parent() == this)
		node = reinterpret_cast<xml_node*>(nextTo.current());
	if (invert)
	{
		if (node == null)
			node = xml_last();
		else
			node = node->xml_prev();
		for (; (node != null);
			node = node->xml_prev()) {

			if ((wstring const&)node->xml_name() == value)
				break;
		}
	}
	else
	{
		if (node == null)
			node = xml_first();
		else
			node = node->xml_next();
		for (; (node != null);
			node = node->xml_next()) {
			if ((wstring const&)node->xml_name() == value)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_items*>(this), node);
}

xml_iterator_t xml_items::find_next(cmstr_t value, xml_iterator_t nextTo, bool invert)const
{
	xml_node_t node = null;

	if (nextTo.is_valid() && nextTo.parent() == this)
		node = reinterpret_cast<xml_node*>(nextTo.current());
	if (invert)
	{
		if (node == null)
			node = xml_last();
		else
			node = node->xml_prev();
		for (; (node != null);
			node = node->xml_prev()) {

			if ((wstring const&)node->xml_name() == value)
				break;
		}
	}
	else
	{
		if (node == null)
			node = xml_first();
		else
			node = node->xml_next();
		for (; (node != null);
			node = node->xml_next()) {
			if ((wstring const&)node->xml_name() == value)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_items*>(this), node);
}

bool xml_items::xml_parent(xml_node_t element)
{
	if (element.is_empty()) return false;
	_xml_parent = element.get();
	for (auto it = begin(); it.is_valid(); ++it)
		it->xml_parent(element);
	return true;
}