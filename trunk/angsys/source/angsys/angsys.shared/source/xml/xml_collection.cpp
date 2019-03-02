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
#include <ang/dom/xml.h>
//#include "ang_inlines.h"

using namespace ang;
using namespace ang::dom::xml;



#define MY_TYPE ang::dom::xml::ixml_collection
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::dom::xml::xml_collection
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

/////////////////////////////////////////////////////////////////////////////////////


xml_collection::xml_collection(xml_node_t node, xml_type_t type)
	: m_type(type)
	, m_count(0)
	, m_parent(node.get())
	, m_first(null)
	, m_last(null)
{
}

xml_collection::~xml_collection() 
{
	clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_collection);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_collection, object, ixml_collection);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_collection, object, ixml_collection, ixml_object, ixml_items);

void xml_collection::clear()
{
	while (pop());
}

xml_type_t xml_collection::xml_type()const
{
	return m_type;
}

bool xml_collection::xml_is_type_of(xml_type_t type)const
{
	return m_type == type || xml_type::collection == type;
}

streams::itext_output_stream_t& xml_collection::xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level)const
{
	switch (m_type)
	{
	case xml_type::attribute_list:
		for (auto it = begin(); it.is_valid(); ++it)
			it->xml_print(stream, flag, level) << L" "_s;
		break;
	case xml_type::element_list:
		for (auto it = begin(); it.is_valid(); ++it)
		{
			if ((flag & xml_format::wrap_text_tab)
				|| (flag & xml_format::wrap_text_space))
				stream << L"\n"_s;
			it->xml_print(stream, flag, level);
		}
		break;
	}
	return stream;
}

xml_node_t xml_collection::xml_parent()const
{
	return m_parent.lock();
}

xml_document_t xml_collection::xml_parent_doc()const
{
	auto parent = xml_parent();
	return parent.is_empty() ? null : parent->xml_parent_doc();
}


xml_node* xml_collection::xml_first()const
{
	return m_first;
}

xml_node* xml_collection::xml_last()const
{
	return m_last;
}


void xml_collection::xml_first(xml_node* node)
{
	if (m_first == node) return;
	auto old = m_first;
	m_first = node;
	if (m_first)m_first->add_ref();
	if (old)old->release();
}


void xml_collection::xml_last(xml_node* node)
{
	if (m_last == node) return;
	auto old = m_last;
	m_last = node;
	if (m_last)m_last->add_ref();
	if (old)old->release();
}

bool xml_collection::push(xml_node_t node, bool last)
{
	if (node.is_empty() 
		|| (xml_is_type_of(xml_type::attribute_list) && !node->xml_is_type_of(xml_type::name_space) && !node->xml_is_type_of(xml_type::attribute))
		|| (xml_is_type_of(xml_type::element_list) && !node->xml_is_type_of(xml_type::element) && !node->xml_is_type_of(xml_type::comment))
		) return false;

	node->xml_parent(m_parent.lock());
	if (last)
	{
		if (xml_first() == null)
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
		if (xml_first() == null)
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
	m_count++;
	return true;
}

bool xml_collection::insert(xml_node_t node, xml_iterator_t at, bool nextTo)
{
	if (node.is_empty()
		|| (xml_is_type_of(xml_type::attribute_list) && !node->xml_is_type_of(xml_type::name_space) && !node->xml_is_type_of(xml_type::attribute))
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

	node->xml_parent(m_parent.lock());
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
	m_count++;
	return true;
}

bool xml_collection::pop(xml_node_ptr_t out, bool last)
{
	if (m_count == 0)
		return false;
	if (m_count == 1)
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
	m_count--;
	return true;
}

bool xml_collection::pop_at(xml_iterator_t it, xml_node_ptr_t out)
{
	if (m_count == 0)
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

	m_count--;
	return true;
}

wsize xml_collection::counter()const
{
	return m_count;
}

xml_node_t xml_collection::at(const xml_base_iterator_t& it)
{
	if (!counter()) throw(ang::exception_t(except_code::invalid_memory));
	if (it.parent() != this)throw(ang::exception_t(except_code::invalid_param));
	return reinterpret_cast<xml_node*>(it.current());
}

bool xml_collection::increase(xml_base_iterator_t& it)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	it.current(reinterpret_cast<xml_node*>(it.current())->xml_next_sibling());
	if (it.current() == null)
		return false;
	return true;
}

bool xml_collection::increase(xml_base_iterator_t& it, int offset)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	auto node = reinterpret_cast<xml_node*>(it.current());
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

bool xml_collection::decrease(xml_base_iterator_t&it)const
{
	if (it.parent() != this) throw(ang::exception_t(except_code::invalid_memory));
	it.current(reinterpret_cast<xml_node*>(it.current())->xml_prev_sibling());
	if (it.current() == null)
		return false;
	return true;
}

bool xml_collection::decrease(xml_base_iterator_t&it, int offset)const
{
	if (it.parent() != this) throw(ang::exception_t(except_code::invalid_memory));
	auto node = reinterpret_cast<xml_node*>(it.current());
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

xml_forward_iterator_t xml_collection::begin()
{
	return xml_iterator_t(const_cast<xml_collection*>(this), (xml_node*)xml_first());
}

xml_forward_iterator_t xml_collection::end()
{
	return xml_iterator_t(const_cast<xml_collection*>(this), null);
}

xml_const_forward_iterator_t xml_collection::begin()const
{
	return xml_iterator_t(const_cast<xml_collection*>(this), (xml_node*)xml_first());
}

xml_const_forward_iterator_t xml_collection::end()const
{
	return xml_iterator_t(const_cast<xml_collection*>(this), null);
}

xml_forward_iterator_t xml_collection::last()
{
	return xml_iterator_t(const_cast<xml_collection*>(this), (xml_node*)xml_last());
}

xml_const_forward_iterator_t xml_collection::last()const
{
	return xml_iterator_t(const_cast<xml_collection*>(this), (xml_node*)xml_last());
}


xml_backward_iterator_t xml_collection::rbegin()
{
	return xml_iterator_t(const_cast<xml_collection*>(this), (xml_node*)xml_last());
}

xml_backward_iterator_t xml_collection::rend()
{
	return xml_iterator_t(const_cast<xml_collection*>(this), null);
}

xml_const_backward_iterator_t xml_collection::rbegin()const
{
	return xml_iterator_t(const_cast<xml_collection*>(this), (xml_node*)xml_last());
}

xml_const_backward_iterator_t xml_collection::rend()const
{
	return xml_iterator_t(const_cast<xml_collection*>(this), null);
}

xml_iterator_t xml_collection::find(xml_cstr_t value, bool invert)const
{
	xml_node_t node = null;
	if (invert)
	{
		for (node = xml_last();
			(node != null);
			node = node->xml_prev_sibling()) {
			if ((xml_cstr_t)node->xml_name() == value)
				break;
		}
	}
	else
	{
		for (node = xml_first();
			(node != null);
			node = node->xml_next_sibling()) {
			if ((xml_cstr_t)node->xml_name() == value)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}

xml_iterator_t xml_collection::find(xml_cstr_t value, xml_iterator_t nextTo, bool invert)const
{
	xml_node_t node = null;

	if (nextTo.is_valid() && nextTo.parent() == this)
		node = reinterpret_cast<xml_node*>(nextTo.current());
	if (invert)
	{
		if (node == null)
			node = xml_last();
		for (; (node != null);
			node = node->xml_prev_sibling()) {
			if ((xml_cstr_t)node->xml_name() == value)
				break;
		}
	}
	else
	{
		if (node == null)
			node = xml_first();
		for (; (node != null);
			node = node->xml_next_sibling()) {
			if ((xml_cstr_t)node->xml_name() == value)
				break;
		}
	}
	return xml_iterator_t(const_cast<xml_collection*>(this), node);
}

