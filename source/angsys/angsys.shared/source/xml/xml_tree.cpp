/*********************************************************************************************************************/
/*   File Name: xml_tree.cpp                                                                                           */
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
using namespace ang::collections;

ang::object_wrapper<xml_tree>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<xml_tree>::object_wrapper(xml_tree* ptr)
	: _ptr(null)
{
	set(ptr);
}

ang::object_wrapper<xml_tree>::object_wrapper(object_wrapper && other)
	: _ptr(null)
{
	xml_tree * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_tree>::object_wrapper(object_wrapper const& other)
	: _ptr(null)
{
	set(other._ptr);
}

ang::object_wrapper<xml_tree>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_tree>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<xml_tree>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<xml_tree>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<xml_tree>::is_empty()const
{
	return _ptr == null;
}

xml_tree* ang::object_wrapper<xml_tree>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_tree>::set(xml_tree* ptr)
{
	xml_tree * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<xml_tree>& ang::object_wrapper<xml_tree>::operator = (xml_tree* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_tree>& ang::object_wrapper<xml_tree>::operator = (ang::object_wrapper<xml_tree> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_tree>& ang::object_wrapper<xml_tree>::operator = (ang::object_wrapper<xml_tree> const& other)
{
	set(other._ptr);
	return*this;
}

xml_tree ** ang::object_wrapper<xml_tree>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_tree> ang::object_wrapper<xml_tree>::operator& (void)
{
	return this;
}

xml_tree * ang::object_wrapper<xml_tree>::operator -> (void)
{
	return get();
}

xml_tree const* ang::object_wrapper<xml_tree>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_tree>::operator xml_tree * (void)
{
	return get();
}

ang::object_wrapper<xml_tree>::operator xml_tree const* (void)const
{
	return get();
}

//xml_node_t ang::object_wrapper<xml_tree>::operator[](xml_value value)const
//{
//	return _ptr ? (xml_node*)_ptr->find_first(value) : null;
//}
//
//xml_node_t ang::object_wrapper<xml_tree>::operator[](string_t const& value)const
//{
//	return _ptr ? (xml_node*)_ptr->find_first(value) : null;
//}
//
//xml_node_t ang::object_wrapper<xml_tree>::operator[](wstring_t const& value)const
//{
//	return _ptr ? (xml_node*)_ptr->find_first(value) : null;
//}
//
//xml_node_t ang::object_wrapper<xml_tree>::operator[](cstr_t value)const
//{
//	return _ptr ? (xml_node*)_ptr->find_first(value) : null;
//}
//
//xml_node_t ang::object_wrapper<xml_tree>::operator[](cwstr_t value)const
//{
//	return _ptr ? (xml_node*)_ptr->find_first(value) : null;
//}


ANG_IMPLEMENT_WEAK_PTR(xml_tree);

////////////////////////////////////////////////////////////////////////////////

xml_tree_iterator::xml_tree_iterator()
	: _xml_tree()
	, _xml_current(null)
	, _xml_child(null)
{

}

xml_tree_iterator::xml_tree_iterator(xml_tree_t store)
	: _xml_tree(store)
	, _xml_current(null)
	, _xml_child(null)
{
	xml_iterator_t it;
	if (!store.is_empty() && store->begin().is_valid())
		xml_current((xml_node*)store->begin());
}

xml_tree_iterator::xml_tree_iterator(const xml_tree_iterator& it)
	: _xml_tree(it._xml_tree)
	, _xml_current(null)
	, _xml_child(null)
{
	xml_current(it.xml_current());
	xml_child(it.xml_child());
}

xml_tree_iterator::~xml_tree_iterator()
{
	_xml_child = null;
	_xml_current = null;
	_xml_tree = null;
}

xml_node_t xml_tree_iterator::xml_current()const
{
	return _xml_current;
}

xml_node_t xml_tree_iterator::xml_child()const
{
	return _xml_child;
}

void xml_tree_iterator::xml_current(xml_node_t xmlNode)
{
	_xml_current = xmlNode;
}

void xml_tree_iterator::xml_child(xml_node_t xmlNode)
{
	_xml_child = xmlNode;
}

void xml_tree_iterator::xml_parent(xml_tree_t store)
{
	_xml_tree = store;
}

xml_tree_t xml_tree_iterator::xml_parent()const
{
	return _xml_tree.lock();
}

xml_iterator_t xml_tree_iterator::current()const
{
	return xml_iterator_t(xml_parent().get(), _xml_current.get());
}

xml_iterator_t xml_tree_iterator::child()const
{
	return xml_iterator_t(xml_parent().get(), _xml_child.get());
}

bool xml_tree_iterator::begin_child()
{
	if (xml_current())
	{
		xml_child((xml_current()->xml_children()) ? (xml_node*)xml_current()->xml_children()->begin() : null);
		if (!xml_child().is_empty())
			return true;
	}
	return false;
}

bool xml_tree_iterator::end_child()
{
	if (xml_current())
	{
		xml_child((xml_current()->xml_children()) ? (xml_node*)xml_current()->xml_children()->end() : null);
		if (xml_child())
			return true;
	}
	return false;
}

bool xml_tree_iterator::next()
{
	if (xml_current() && xml_current()->xml_next())
	{
		xml_current(_xml_current->xml_next());
		return true;
	}
	return false;
}

bool xml_tree_iterator::prev()
{
	if (xml_current() && xml_current()->xml_prev())
	{
		xml_current(_xml_current->xml_prev());
		return true;
	}
	return false;
}

bool xml_tree_iterator::next_child()
{
	if (!xml_child() && !begin_child())
		return false;

	if (xml_child()->xml_next())
		return false;

	xml_child(xml_child()->xml_next());
	return true;
}

bool xml_tree_iterator::prev_Child()
{
	if (!xml_child() && !begin_child())
		return false;

	if (!xml_child()->xml_prev())
		return false;

	xml_child(xml_child()->xml_prev());
	return true;

}

bool xml_tree_iterator::backward()
{
	xml_child(xml_current());
	if (xml_current())
		xml_current(xml_current()->xml_parent());
	return true;
}

bool xml_tree_iterator::forward()
{
	if (!xml_child() && !begin_child())
		return false;
	xml_current(xml_child());
	begin_child();
	return true;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

xml_tree::xml_tree(xml_type_t type)
	: _xml_type(type)
	, _count(0)
	, _xml_head(null)
	, _xml_root(null)
	, _xml_tail(null)
{

}

xml_tree::xml_tree(xml_node_t node)
	: _xml_type(xml_type::finder)
	, _count(0)
	, _xml_head(node)
	, _xml_root(node)
	, _xml_tail(node)
{

}


xml_tree::~xml_tree()
{
	clean();
}

ANG_IMPLEMENT_CLASSNAME(ang::xml::xml_tree);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::xml_tree);

bool xml_tree::is_child_of(type_name_t className)
{
	if (className == type_name<xml_tree>()
		|| object::is_child_of(className)
		|| className == type_name<ixmlobject>()
		|| className == type_name<collections::ienum<xml_node>>()
		|| className == type_name<collections::icollection<xml_node>>())
		return true;
	return false;
}

bool xml_tree::is_kind_of(type_name_t className)const
{
	if (className == type_name<xml_tree>()
		|| object::is_kind_of(className)
		|| className == type_name<ixmlobject>()
		|| className == type_name<collections::ienum<xml_node>>()
		|| className == type_name<collections::icollection<xml_node>>())
		return true;
	return false;
}

bool xml_tree::query_object(type_name_t className, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == type_name<xml_tree>())
	{
		*out = static_cast<xml_tree*>(this);
		return true;
	}
	else if (object::query_object(className, out))
	{
		return true;
	}
	else if (ienum_t::query_object(className, out))
	{
		return true;
	}
	else if (icollection_t::query_object(className, out))
	{
		return true;
	}

	return false;
}

xml_type_t xml_tree::xml_type()const
{
	return _xml_type;
}

bool xml_tree::xml_is_type_of(xml_type_t type)const
{
	return _xml_type == type || xml_type_t::tree == type;
}

void xml_tree::clean()
{
	xml_node_t node = xml_head();
	xml_node_t temp;
	while (!node.is_empty())
	{
		temp = node->xml_next();
		node->xml_next(null);
		if (!temp.is_empty())
			temp->xml_prev(null);
		node = temp;
	}
	_xml_head = null;
	_xml_root = null;
	_xml_tail = null;
}

wstring& xml_tree::xml_print(wstring& out, const xml_format_t& flags, ushort level)const
{
	xml_node_t node = xml_head();
	while (!node.is_empty())
	{
		node->xml_print(out, flags, level);
		node = node->xml_next();
		if (flags.is_active(xml_format_t::wrap_text_space) || flags.is_active(xml_format_t::wrap_text_tab))
			out << L"\n";
	}
	return out;
}

uint xml_tree::counter()const
{
	return _count;
}

xml_node_t xml_tree::at(const xml_iterator_t& it)const
{
	if (is_empty() && !it.is_valid()) throw(exception_t(except_code::invalid_memory));
	if (it.parent() != this) throw(exception_t(except_code::invalid_param));
	xml_node_t node = reinterpret_cast<xml_node*>(it.current());
	return node;
}

bool xml_tree::next(xml_iterator_t& it)const
{
	if (is_empty() && !it.is_valid()) return false;
	if (it.parent() != this) return false;

	xml_node* node = reinterpret_cast<xml_node*>(it.current());

	auto children = node->xml_children();
	if (children && children->counter())
	{
		it = xml_iterator_t(const_cast<xml_tree*>(this), (xml_node*)children->begin());
		return true;
	}
	else while (node != null)
	{
		if (node->xml_next() != null)
		{
			node = node->xml_next();
			break;
		}
		node = node->xml_parent();
	}
	it = xml_iterator_t(const_cast<xml_tree*>(this), node);
	return true;
}

bool xml_tree::next(xml_iterator_t& it, int offset)const
{
	int c = 0;
	while (c < offset)
	{
		if (!next(it))return false;
		c--;
	}
	return true;
}

bool xml_tree::prev(xml_iterator_t& it)const
{
	if (is_empty() && !it.is_valid()) return false;
	if (it.parent() != this) return false;

	xml_node* node = reinterpret_cast<xml_node*>(it.current());
	if (node->xml_prev() != null)
	{
		node = node->xml_prev();
		while (node->xml_children() && node->xml_children()->end().is_valid())
			node = (xml_node*)node->xml_children()->end();
		it = xml_iterator_t(const_cast<xml_tree*>(this), node);
	}
	else
	{
		it = xml_iterator_t(const_cast<xml_tree*>(this), node->xml_parent());
	}
	return true;
}

bool xml_tree::prev(xml_iterator_t& it, int offset)const
{
	int c = 0;
	while (c < offset)
	{
		if (!prev(it))return false;
		c--;
	}
	return true;
}

xml_iterator_t xml_tree::begin()const
{
	return xml_iterator_t(const_cast<xml_tree*>(this), xml_head());
}

xml_iterator_t xml_tree::end()const
{
	xml_node* node = xml_tail();
	if (node != null)
	{
		while (node->xml_children() && node->xml_children()->end().is_valid())
			node = (xml_node*)node->xml_children()->end();
	}
	return xml_iterator_t(const_cast<xml_tree*>(this), node);
}

bool  xml_tree::has_items()const
{
	return _count > 0;
}

bool  xml_tree::copy(const collections::ienum<xml_node>* store)
{
	return false;
}

index xml_tree::index_of(xml_iterator_t it)const
{
	if (!it.is_valid() || it.parent() != this)
		return invalid_index;
	index i = 0;
	--it;
	while (it.is_valid()) {
		i++;
		it--;
	}
	return i;
}

xml_iterator_t xml_tree::find_index(index idx)const
{
	auto it = begin();
	for (index i = 0; i < idx && it.is_valid(); i++, it++);
	return it;
}

xml_iterator_t xml_tree::find(xml_node_t node, bool invert)const
{
	if (invert)
	{
		for (auto it = end(); it.is_valid(); it--)
		{
			if (node == (xml_node*)it)
				return it;
		}
	}
	else
	{
		for (auto it = begin(); it.is_valid(); it++)
		{
			if (node == (xml_node*)it)
				return it;
		}
	}

	return xml_iterator_t(const_cast<xml_tree*>(this), null);
}

xml_iterator_t xml_tree::find(xml_node_t node, xml_iterator_t nextTo, bool invert)const
{
	if (!nextTo.is_valid() || nextTo.parent() != this)
		nextTo = (invert) ? end() : begin();
	if (invert)
	{
		for (auto it = nextTo; it.is_valid(); it--)
		{
			if (node == ((xml_node*)it))
				return it;
		}
	}
	else
	{
		for (auto it = nextTo; it.is_valid(); it++)
		{
			if (node == ((xml_node*)it))
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_tree*>(this), null);
}

xml_iterator_t xml_tree::find_first(cstr_t name, bool invert)const
{
	if (invert)
	{
		for (auto it = end(); it.is_valid(); it--)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	else
	{
		for (auto it = begin(); it.is_valid(); it++)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_tree*>(this), null);
}

xml_iterator_t xml_tree::find_first(cwstr_t name, bool invert)const
{
	if (invert)
	{
		for (auto it = end(); it.is_valid(); it--)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	else
	{
		for (auto it = begin(); it.is_valid(); it++)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_tree*>(this), null);
}

xml_iterator_t xml_tree::find_first(cmstr_t name, bool invert)const
{
	if (invert)
	{
		for (auto it = end(); it.is_valid(); it--)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	else
	{
		for (auto it = begin(); it.is_valid(); it++)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_tree*>(this), null);
}

xml_iterator_t xml_tree::find_next(cstr_t name, xml_iterator_t nextTo, bool invert)const
{
	if (!nextTo.is_valid() || nextTo.parent() != this)
		nextTo = (invert) ? end() : begin();

	if (invert)
	{
		for (auto it = nextTo; it.is_valid(); it--)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	else
	{
		for (auto it = nextTo; it.is_valid(); it++)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_tree*>(this), null);
}

xml_iterator_t xml_tree::find_next(cwstr_t name, xml_iterator_t nextTo, bool invert)const
{
	if (!nextTo.is_valid() || nextTo.parent() != this)
		nextTo = (invert) ? end() : begin();

	if (invert)
	{
		for (auto it = nextTo; it.is_valid(); it--)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	else
	{
		for (auto it = nextTo; it.is_valid(); it++)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_tree*>(this), null);
}

xml_iterator_t xml_tree::find_next(cmstr_t name, xml_iterator_t nextTo, bool invert)const
{
	if (!nextTo.is_valid() || nextTo.parent() != this)
		nextTo = (invert) ? end() : begin();

	if (invert)
	{
		for (auto it = nextTo; it.is_valid(); it--)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	else
	{
		for (auto it = nextTo; it.is_valid(); it++)
		{
			if (name == (wstring const&)it->xml_name())
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_tree*>(this), null);
}

bool xml_tree::is_empty()const
{
	return _count == 0;
}

xml_node* xml_tree::xml_head()const
{
	return _xml_head.get();
}

xml_node* xml_tree::xml_root()const
{
	return _xml_root.get();
}

xml_node* xml_tree::xml_tail()const
{
	return _xml_tail.get();
}

void xml_tree::xml_head(xml_node_t node)
{
	if (!node.is_empty() && (node->xml_is_type_of(xml_type::header)
		|| node->xml_is_type_of(xml_type::element)
		|| node->xml_is_type_of(xml_type::comment)))
	{
		_xml_head = node;
	}
}

void xml_tree::xml_root(xml_node_t node)
{
	if (!node.is_empty() && !node->xml_is_type_of(xml_type::element))
		return;
	_xml_root = node;
}

void xml_tree::xml_tail(xml_node_t node)
{
	if (!node.is_empty() && (node->xml_is_type_of(xml_type::header)
		|| node->xml_is_type_of(xml_type::element)
		|| node->xml_is_type_of(xml_type::comment)))
	{
		_xml_tail = node;
	}
}

void xml_tree::insert_next_to(xml_node* refNode, xml_node* newNode)
{
	refNode->xml_next()->xml_prev(newNode);
	newNode->xml_next(refNode->xml_next());
	newNode->xml_prev(refNode);
	refNode->xml_next(newNode);
}

void xml_tree::insert_prev_to(xml_node* refNode, xml_node* newNode)
{
	refNode->xml_prev()->xml_next(newNode);
	newNode->xml_prev(refNode->xml_prev());
	newNode->xml_next(refNode);
	refNode->xml_prev(newNode);
}

void xml_tree::insert_in_head(xml_node* newNode)
{
	xml_head()->xml_prev(newNode);
	newNode->xml_next(xml_head());
	xml_head(newNode);
}

void xml_tree::insert_in_tail(xml_node* newNode)
{
	xml_tail()->xml_next(newNode);
	newNode->xml_prev(xml_tail());
	xml_tail(newNode);
}

