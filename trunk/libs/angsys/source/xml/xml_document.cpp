/*********************************************************************************************************************/
/*   File Name: xml.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/
#include "pch.h"
#include "coffe/dom/xml.h"
#include "xml/xml_impl.h"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


using namespace coffe;
using namespace coffe::dom::xml;

xml_document_iterator::xml_document_iterator()
	: m_doc(null)
	, m_current(null)
	, m_child(null)
{

}

xml_document_iterator::xml_document_iterator(xml_document* doc)
	: m_doc(doc)
	, m_current(null)
	, m_child(null)
{
	xml_iterator_t it;
	if (doc)
		current_node(doc->m_root);
}

xml_document_iterator::xml_document_iterator::xml_document_iterator(const xml_document_iterator& it)
	: m_doc(it.m_doc)
	, m_current(null)
	, m_child(null)
{
	current_node(it.current_node());
	child_node(it.child_node());
}

xml_document_iterator::~xml_document_iterator()
{
	m_child = null;
	m_current = null;
	m_doc = null;
}

xml_node_t xml_document_iterator::current_node()const
{
	return m_current;
}

xml_node_t xml_document_iterator::child_node()const
{
	return m_child;
}

void xml_document_iterator::current_node(xml_node_t node)
{
	m_current = node;
}

void xml_document_iterator::child_node(xml_node_t node)
{
	m_child = node;
}

void xml_document_iterator::document(xml_document* doc)
{
	m_doc = doc;
}

xml_document* xml_document_iterator::document()const
{
	return m_doc;
}

xml_iterator_t xml_document_iterator::current()const
{
	return xml_iterator_t(m_doc, m_current.get(), 0);
}

xml_iterator_t xml_document_iterator::child()const
{
	return xml_iterator_t(m_doc, m_child.get(), 0);
}

void xml_document_iterator::reset()
{

}

bool xml_document_iterator::begin_child()
{
	if (current_node() == null)
		return false;
	auto children = current_node()->to_intf<ixml_list>();
	if (children == null || children->counter() == 0)
	{
		child_node(null);
		return false;
	}
	child_node(children->front());
	return true;
}

bool xml_document_iterator::end_child()
{
	if (current_node() == null)
		return false;
	auto children = current_node()->to_intf<ixml_list>();
	if (children == null || children->counter() == 0)
	{
		child_node(null);
		return false;
	}
	if (children->begin().is_valid())
		child_node(children->back());
	return true;
}

bool xml_document_iterator::next()
{
	if (current_node() != null && current_node()->next_sibling() != null)
	{
		current_node(current_node()->next_sibling());
		return true;
	}
	return false;
}

bool xml_document_iterator::prev()
{
	if (current_node() != null && current_node()->prev_sibling() != null)
	{
		current_node(m_current->prev_sibling());
		return true;
	}
	return false;
}

bool xml_document_iterator::next_child()
{
	if (child_node() == null && !begin_child())
		return false;

	if (child_node()->next_sibling() == null)
		return false;

	child_node(child_node()->next_sibling());
	return true;
}

bool xml_document_iterator::prev_child()
{
	if (child_node() == null && !begin_child())
		return false;

	if (child_node()->prev_sibling() == null)
		return false;

	child_node(child_node()->prev_sibling());
	return true;
}

bool xml_document_iterator::backward()
{
	child_node(current_node());
	if (current_node() != null)
		current_node(current_node()->to_intf<xml_node_base>()->parent());
	return true;
}

bool xml_document_iterator::forward()
{
	if (child_node() == null && !begin_child())
		return false;
	current_node(child_node());
	begin_child();
	return true;
}


////////////////////////////////////////////////////////////////////////////////

xml_document::xml_document(xml_encoding_t e)
	: m_count(0)
	, m_version(null)
	, m_stand_alone(stand_alone_none)
	, m_encoding(e == xml_encoding::auto_detect ? xml_encoding::utf8 : e.get())
	, m_root(null)
	, m_cursor(this)
{
	m_version = xml_text_factory::create_text(m_encoding, "1.0"_r);
}

xml_document::xml_document(xml_document const* doc)
	: m_count(0)
	, m_version(null)
	, m_stand_alone(stand_alone_none)
	, m_encoding(doc ? doc->encoding() : text::encoding::utf8)
	, m_root(null)
	, m_cursor(this)
{
	m_version = xml_text_factory::create_text(m_encoding, "1.0"_r);
}

void xml_document::dispose()
{
	clear();
}

xml_type_t xml_document::type()const
{
	return xml_type::document;
}

bool xml_document::is_type_of(xml_type_t type)const
{
	return type == xml_type::document;
}

streams::itext_output_stream_t& xml_document::print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level)const
{
	return stream;
}

ixml_document_builder_t xml_document::builder()
{
	return this;
}

string xml_document::version()const
{
	return m_version.get();
}

xml_encoding_t xml_document::encoding()const
{
	return m_encoding;
}

bool xml_document::stand_alone()const
{
	return m_stand_alone == stand_alone_true;
}

ixml_node_t xml_document::data_type()const
{
	//TODO:
	return null;
}


wsize xml_document::counter()const
{
	return m_count;
}

ixml_node_t xml_document::at(xml_base_iterator_t const& it)
{
	if (it.parent() != this)
		throw_exception(error_code::invalid_param);
	return interface_cast<ixml_node>((xml_node*)it.current());
}

bool xml_document::increase(xml_base_iterator_t& it)const
{
	if (it.parent() != this || !it.is_valid())
		throw_exception(error_code::invalid_param);

	xml_node * other, *node = reinterpret_cast<xml_node*>(it.current());
	if (node)
	{
		if (other = node->first_child())
			node = other;
		else do
			if (other = node->next_sibling()) {
				node = other; break;
			}
		while (node = node->to_intf<xml_node_base>()->parent());
	}
	it = xml_iterator_t(const_cast<xml_document*>(this), node);
	return true;
}

bool xml_document::increase(xml_base_iterator_t& it, int offset)const
{
	if (it.parent() != this || !it.is_valid())
		throw_exception(error_code::invalid_param);

	xml_node * other, *node = reinterpret_cast<xml_node*>(it.current());
	while (node && offset > 0)
	{
		offset--;
		if (other = node->first_child())
			node = other;
		else do
			if (other = node->next_sibling()) {
				node = other; break;
			}
		while (node = node->to_intf<xml_node_base>()->parent());
	}

	it = xml_iterator_t(const_cast<xml_document*>(this), node);
	return true;
}

bool xml_document::decrease(xml_base_iterator_t& it)const
{
	if (it.parent() != this || !it.is_valid())
		throw_exception(error_code::invalid_param);

	xml_node * other, *node = reinterpret_cast<xml_node*>(it.current());
	if (node)
	{
		if (other = node->prev_sibling())
			do node = other; while (other = node->last_child());
		else
			node = node->to_intf<xml_node_base>()->parent();
	}
	it = xml_iterator_t(const_cast<xml_document*>(this), node);
	return true;
}

bool xml_document::decrease(xml_base_iterator_t& it, int offset)const
{
	if (it.parent() != this || !it.is_valid())
		throw_exception(error_code::invalid_param);

	xml_node * other, *node = reinterpret_cast<xml_node*>(it.current());
	while (node && offset > 0)
	{
		offset--;
		if (other = node->prev_sibling())
			do node = other; while (other = node->last_child());
		else
			node = node->to_intf<xml_node_base>()->parent();
	}
	it = xml_iterator_t(const_cast<xml_document*>(this), node);
	return true;
}

xml_forward_iterator_t xml_document::begin()
{
	return xml_iterator_t(const_cast<xml_document*>(this), m_items.front().get(), 0);
}

xml_forward_iterator_t xml_document::end()
{
	return xml_iterator_t(const_cast<xml_document*>(this), null, 0);
}

xml_const_forward_iterator_t xml_document::begin()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), m_items.front().get(), 0);
}

xml_const_forward_iterator_t xml_document::end()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), null, 0);
}

xml_forward_iterator_t xml_document::last()
{
	return xml_iterator_t(const_cast<xml_document*>(this), m_items.back().get(), 0);
}

xml_const_forward_iterator_t xml_document::last()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), m_items.back().get(), 0);
}

xml_backward_iterator_t xml_document::rbegin()
{
	return xml_iterator_t(const_cast<xml_document*>(this), m_items.back().get(), 0);
}

xml_backward_iterator_t xml_document::rend()
{
	return xml_iterator_t(const_cast<xml_document*>(this), null, 0);
}

xml_const_backward_iterator_t xml_document::rbegin()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), m_items.back().get(), 0);
}

xml_const_backward_iterator_t xml_document::rend()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), null, 0);
}

xml_iterator_t xml_document::find(cstr_t name, bool invert)const
{
	if (invert)
	{
		for (auto it = rbegin(); it != rend(); it++) {
			if (it->name() == name)
				return it;
		}
	}
	else
	{
		for (auto it = begin(); it != end(); it++) {
			if (it->name() == name)
				return it;
		}
	}
	return end();
}

xml_iterator_t xml_document::find(cstr_t name, xml_base_iterator_t next_to, bool invert)const
{
	if (invert)
	{
		for (xml_const_backward_iterator_t it = next_to.is_valid() ? next_to : (xml_base_iterator_t)rbegin(); it != rend(); it++) {
			if (it->name() == name)
				return it;
		}
	}
	else
	{
		for (xml_const_forward_iterator_t it = next_to.is_valid() ? next_to : (xml_base_iterator_t)begin(); it != end(); it++) {
			if (it->name() == name)
				return it;
		}
	}
	return end();
}

ixml_enum_t xml_document::find_all(cstr_t name)const
{
	smart_vector<ixml_node> out = null;
	for (auto it = begin(); it != end(); it++) {
		if (it->name() == name)
			out += *it;
	}
	return out.get();
}

xml_node_t xml_document::front()const
{
	return m_items.front();
}

xml_node_t xml_document::back()const
{
	return m_items.back();
}

void xml_document::push_front(xml_node_t node)
{
	m_items.push_front(node);
}

void xml_document::push_back(xml_node_t node)
{
	m_items.push_back(node);
}

xml_iterator_t xml_document::insert(xml_base_iterator_t it, xml_node_t node)
{
	algorithms::base_iterator<xml_node> it2(algorithms::xml_iteration(), (xml_node*)it.current(),0);
	it2 = m_items.insert(it2, node);
	return xml_iterator_t(this, it2.get_node(), 0);
}

bool xml_document::pop_front(xml_node_ptr_t out)
{
	return m_items.pop_front(out);
}

bool xml_document::pop_back(xml_node_ptr_t out)
{
	return m_items.pop_back(out);
}

xml_iterator_t xml_document::remove(xml_base_iterator_t it, xml_node_ptr_t out)
{
	algorithms::base_iterator<xml_node> it2(algorithms::xml_iteration(), (xml_node*)it.current(), 0);
	if (!m_items.remove(it2, out))
		return end();
	return xml_iterator_t(this, it2.get_node(), 0);
}


ixml_items_t xml_document::elements()const
{
	return  to_intf<ixml_items>();
}

ixml_node_t xml_document::root_element()const
{
	return m_root.get();
}

ixml_document_t xml_document::clone()const
{
	return new xml_document(this);
}

void xml_document::clear()
{
	m_items.clear();
}

