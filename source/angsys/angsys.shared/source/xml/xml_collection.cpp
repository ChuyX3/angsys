/*********************************************************************************************************************/
/*   File Name: xml_collection.cpp                                                                                           */
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

ang::object_wrapper<xml_collection>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<xml_collection>::object_wrapper(xml_collection* ptr)
	: _ptr(null)
{
	set(ptr);
}

ang::object_wrapper<xml_collection>::object_wrapper(object_wrapper && other)
	: _ptr(null)
{
	xml_collection * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_collection>::object_wrapper(object_wrapper const& other)
	: _ptr(null)
{
	set(other._ptr);
}

ang::object_wrapper<xml_collection>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_collection>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<xml_collection>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<xml_collection>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<xml_collection>::is_empty()const
{
	return _ptr == null;
}

xml_collection* ang::object_wrapper<xml_collection>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_collection>::set(xml_collection* ptr)
{
	xml_collection * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<xml_collection>& ang::object_wrapper<xml_collection>::operator = (xml_collection* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_collection>& ang::object_wrapper<xml_collection>::operator = (ang::object_wrapper<xml_collection> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_collection>& ang::object_wrapper<xml_collection>::operator = (ang::object_wrapper<xml_collection> const& other)
{
	set(other._ptr);
	return*this;
}

object_wrapper<xml_collection>&  ang::object_wrapper<xml_collection>::operator += (xml_node_t node)
{
	if (_ptr)_ptr->append(node);
	return*this;
}

xml_collection ** ang::object_wrapper<xml_collection>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_collection> ang::object_wrapper<xml_collection>::operator& (void)
{
	return this;
}

xml_collection * ang::object_wrapper<xml_collection>::operator -> (void)
{
	return get();
}

xml_collection const* ang::object_wrapper<xml_collection>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_collection>::operator xml_collection * (void)
{
	return get();
}

ang::object_wrapper<xml_collection>::operator xml_collection const* (void)const
{
	return get();
}

xml_node_t ang::object_wrapper<xml_collection>::operator[](cstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? (xml_node*)it : null;
}

xml_node_t ang::object_wrapper<xml_collection>::operator[](cwstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? (xml_node*)it : null;
}

xml_node_t ang::object_wrapper<xml_collection>::operator[](cmstr_t value)const
{
	xml_iterator_t it;
	return (_ptr && (it = _ptr->find_first(value)).is_valid()) ? (xml_node*)it : null;
}

////////////////////////////////////////////////////////////////////////////////

xml_collection::xml_collection(xml_type_t type)
	: _xml_type(type)
	, _count(0)
	, _xml_parent(null)
	, _xml_first(null)
	, _xml_last(null)
{	
}

xml_collection::~xml_collection() { clean(); }

ANG_IMPLEMENT_CLASSNAME(ang::xml::xml_collection);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::xml_collection);

bool xml_collection::is_child_of(type_name_t name)
{
	if (name == type_name<xml_collection>()
		|| object::is_child_of(name)
		|| ixmlobject::is_child_of(name)
		|| collections::ienum<xml_node_t>::is_child_of(name)
		|| collections::icollection<xml_node_t>::is_child_of(name))
		return true;
	return false;
}

bool xml_collection::is_kind_of(type_name_t name)const
{
	if (name == type_name<xml_collection>()
		|| object::is_kind_of(name)
		|| ixmlobject::is_kind_of(name)
		|| collections::ienum<xml_node>::is_kind_of(name)
		|| collections::icollection<xml_node>::is_kind_of(name))
		return true;
	return false;
}

bool xml_collection::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<xml_collection>())
	{
		*out = static_cast<xml_collection*>(this);
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
	else if (collections::icollection<xml_node>::is_kind_of(name))
	{
		*out = static_cast<collections::icollection<xml_node>*>(this);
		return true;
	}
	return false;
}

void xml_collection::clean()
{
	while (pop());
}

template<> xml_element_list_t xml_collection::xml_as<xml_element_list>() { return xml_is_type_of(xml_type::element_list) ? static_cast<xml_element_list*>(this) : null; }
template<> xml_attributes_t xml_collection::xml_as<xml_attribute_list>() { return xml_is_type_of(xml_type::attribute_list) ? static_cast<xml_attribute_list*>(this) : null; }

xml_type_t xml_collection::xml_type()const
{
	return _xml_type;
}

bool xml_collection::xml_is_type_of(xml_type_t type)const
{
	return _xml_type == type || xml_type_t::store == type;
}

wstring& xml_collection::xml_print(wstring& stream, const xml_format_t& flag, ushort level)const
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
	default:break;
	}
	return stream;
}

xml_node* xml_collection::xml_first()const
{
	return _xml_first.get();
}

xml_node* xml_collection::xml_last()const
{
	return _xml_last.get();
}

void xml_collection::xml_first(xml_node* node)
{
	_xml_first = node;
}

void xml_collection::xml_last(xml_node* node)
{
	_xml_last = node;
}

bool xml_collection::append(xml_node_t node, bool last)
{
	if (node.is_empty())
		return false;
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
	return true;
}

bool xml_collection::insert(xml_node_t node, xml_iterator_t at, bool nextTo)
{
	if (node == null)
		return false;

	if (!at.is_valid() || at.parent() != this)
	{
		return append(node, nextTo);
	}

	if (nextTo && (xml_node*)at == xml_last())
		return append(node, true);

	if (!nextTo && (xml_node*)at == xml_first())
		return append(node, false);

	node->xml_parent(_xml_parent.lock());
	if (nextTo)
	{
		at->xml_next(node);
		node->xml_prev((xml_node*)at);
	}
	else
	{
		at->xml_prev(node);
		node->xml_next((xml_node*)at);
	}
	_count++;
	return true;
}

bool xml_collection::insert(xml_node_t node, index idx)
{
	if (node == null)
		return false;
	if (idx >= _count)
		return append(node, true);
	return insert(node, find_index(idx), false);
}

bool xml_collection::pop(xml_node_ptr_t out, bool last)
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

bool xml_collection::pop_at(xml_iterator_t it, xml_node_ptr_t out)
{
	if (_count == 0)
		return false;

	if (it.parent() != this)
		return false;
	if (it.current() == null)
		return false;

	xml_node_t temp = (xml_node*)it;
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

uint xml_collection::counter()const
{
	return _count;
}

xml_node_t xml_collection::at(const xml_iterator_t& it)const
{
	if (!has_items()) throw(ang::exception_t(except_code::invalid_memory));
	if (it.parent() != this)throw(ang::exception_t(except_code::invalid_param));
	return reinterpret_cast<xml_node*>(it.current());
}

bool xml_collection::next(xml_iterator_t& it)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	it.current(reinterpret_cast<xml_node*>(it.current())->xml_next());
	if (it.current() == null)
		return false;
	return true;
}

bool xml_collection::next(xml_iterator_t& it, int offset)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
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

bool xml_collection::prev(xml_iterator_t&it)const
{
	if (it.parent() != this) throw(ang::exception_t(except_code::invalid_memory));
	it.current(reinterpret_cast<xml_node*>(it.current())->xml_prev());
	if (it.current() == null)
		return false;
	return true;
}

bool xml_collection::prev(xml_iterator_t&it, int offset)const
{
	if (it.parent() != this) throw(ang::exception_t(except_code::invalid_memory));
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

xml_iterator_t xml_collection::begin()const
{
	return xml_iterator_t(const_cast<xml_collection*>(this), xml_first());
}

xml_iterator_t xml_collection::end()const
{
	return xml_iterator_t(const_cast<xml_collection*>(this), xml_last());
}

bool xml_collection::has_items()const
{
	return _count > 0;
}

bool xml_collection::copy(const xml_collection::ienum_t* _items)
{
	clean();
	foreach(_items, [this](xml_node& it)
	{
		append(static_cast<xml_node*>(it.xml_clone()));
	});
	return true;
}

index xml_collection::index_of(xml_iterator_t it)const
{
	if (!it.is_valid() || it.parent() != this)
		return invalid_index;
	auto node = reinterpret_cast<xml_node*>(it.current());
	index i = 0;
	while ((node = node->xml_prev()) != null)
		i++;
	return i;
}

xml_iterator_t xml_collection::find_index(index idx)const
{
	if (_count <= idx)
		return xml_iterator_t(const_cast<xml_collection*>(this), null);
	if (idx == invalid_index)
		return xml_iterator_t(const_cast<xml_collection*>(this), null);
	if (idx > _count / 2)
	{
		idx = _count - idx - 1;
		xml_node_t pNode = xml_last();
		for (uint i = 0; i < idx; i++)
			pNode = pNode->xml_prev();
		return xml_iterator_t(const_cast<xml_collection*>(this), pNode);
	}
	else
	{
		xml_node_t pNode = xml_first();
		for (uint i = 0; i < idx; i++)
			pNode = pNode->xml_next();
		return xml_iterator_t(const_cast<xml_collection*>(this), pNode);
	}
}

xml_iterator_t xml_collection::find(xml_node_t value, bool invert)const
{
	xml_node_t node = null;
	if (invert)
	{
		for (node = xml_last();
			(node != null);
			node = node->xml_prev()) {
			if (node == value)
				break;
		}
	}
	else
	{
		for (node = xml_first();
			(node != null);
			node = node->xml_next()) {
			if (node == value)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}

xml_iterator_t xml_collection::find(xml_node_t value, xml_iterator_t nextTo, bool invert)const
{
	xml_node_t node = null;

	if (nextTo.is_valid() && nextTo.parent() == this)
		node = reinterpret_cast<xml_node*>(nextTo.current());
	if (invert)
	{
		if (node == null)
			node = xml_last();
		for (; (node != null);
			node = node->xml_prev()) {
			if (node == value)
				break;
		}
	}
	else
	{
		if (node == null)
			node = xml_first();
		for (; (node != null);
			node = node->xml_next()) {
			if (node == value)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}


xml_iterator_t xml_collection::find_first(cstr_t value, bool invert)const
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
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}

xml_iterator_t xml_collection::find_first(cwstr_t value, bool invert)const
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
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}

xml_iterator_t xml_collection::find_first(cmstr_t value, bool invert)const
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
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}


xml_iterator_t xml_collection::find_next(cstr_t value, xml_iterator_t nextTo, bool invert)const
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
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}

xml_iterator_t xml_collection::find_next(cwstr_t value, xml_iterator_t nextTo, bool invert)const
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
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}

xml_iterator_t xml_collection::find_next(cmstr_t value, xml_iterator_t nextTo, bool invert)const
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
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}

bool xml_collection::xml_parent(xml_node_t element)
{
	if (element.is_empty())
		return false;
	_xml_parent = element;
	for (auto it = begin(); it.is_valid(); ++it)
	{
		it->xml_parent(element);
	}
	return true;
}