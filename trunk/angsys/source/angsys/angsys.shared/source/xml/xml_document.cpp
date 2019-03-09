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
#include "ang/dom/xml.h"
#include "xml/xml_internal.h"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


using namespace ang;
using namespace ang::dom::xml;

#define MY_TYPE ang::dom::xml::ixml_document
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::dom::xml::xml_document
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

/////////////////////////////////////////////////////////////////////////////////////

xml_document::xml_document_iterator::xml_document_iterator()
	: m_doc(null)
	, m_current(null)
	, m_child(null)
{
}

xml_document::xml_document_iterator::xml_document_iterator(xml_document* store)
	: m_doc(store)
	, m_current(null)
	, m_child(null)
{
	xml_iterator_t it;
	if (store && store->begin().is_valid())
		xml_current(*store->begin());
}

xml_document::xml_document_iterator::xml_document_iterator(const xml_document_iterator& it)
	: m_doc(it.m_doc)
	, m_current(null)
	, m_child(null)
{
	xml_current(it.xml_current());
	xml_child(it.xml_child());
}

xml_document::xml_document_iterator::~xml_document_iterator()
{
	m_child = null;
	m_current = null;
	m_doc = null;
}

xml_node_t xml_document::xml_document_iterator::xml_current()const
{
	return m_current;
}

xml_node_t xml_document::xml_document_iterator::xml_child()const
{
	return m_child;
}

void xml_document::xml_document_iterator::xml_current(xml_node_t xmlNode)
{
	m_current = xmlNode;
}

void xml_document::xml_document_iterator::xml_child(xml_node_t xmlNode)
{
	m_child = xmlNode;
}

void xml_document::xml_document_iterator::xml_parent(xml_document_t store)
{
	m_doc = store.get();
}

xml_document_t xml_document::xml_document_iterator::xml_parent()const
{
	return m_doc.lock();
}

xml_iterator_t xml_document::xml_document_iterator::current()const
{
	return xml_iterator_t(xml_parent().get(), m_current.get());
}

xml_iterator_t xml_document::xml_document_iterator::child()const
{
	return xml_iterator_t(xml_parent().get(), m_child.get());
}

bool xml_document::xml_document_iterator::begin_child()
{
	if (!xml_current())
		return false;
	auto children = xml_current()->xml_children();
	if (children.is_empty())
	{
		xml_child(null);
		return false;
	}
	if (children->begin().is_valid())
		xml_child(*children->begin());
	return true;
}

bool xml_document::xml_document_iterator::end_child()
{
	if (!xml_current())
		return false;
	auto children = xml_current()->xml_children();
	if (children.is_empty())
	{
		xml_child(null);
		return false;
	}
	if (children->begin().is_valid())
		xml_child(*children->rbegin());
	return true;
}

bool xml_document::xml_document_iterator::next()
{
	if (xml_current() && xml_current()->xml_next_sibling())
	{
		xml_current(m_current->xml_next_sibling());
		return true;
	}
	return false;
}

bool xml_document::xml_document_iterator::prev()
{
	if (xml_current() && xml_current()->xml_prev_sibling())
	{
		xml_current(m_current->xml_prev_sibling());
		return true;
	}
	return false;
}

bool xml_document::xml_document_iterator::next_child()
{
	if (!xml_child() && !begin_child())
		return false;

	if (!xml_child()->xml_next_sibling())
		return false;

	xml_child(xml_child()->xml_next_sibling());
	return true;
}

bool xml_document::xml_document_iterator::prev_Child()
{
	if (!xml_child() && !begin_child())
		return false;

	if (!xml_child()->xml_prev_sibling())
		return false;

	xml_child(xml_child()->xml_prev_sibling());
	return true;

}

bool xml_document::xml_document_iterator::backward()
{
	xml_child(xml_current());
	if (xml_current())
		xml_current(xml_current()->xml_parent());
	return true;
}

bool xml_document::xml_document_iterator::forward()
{
	if (!xml_child() && !begin_child())
		return false;
	xml_current(xml_child());
	begin_child();
	return true;
}

////////////////////////////////////////////////////////////////////////////////

xml_document_t xml_document::from_file(core::files::input_text_file_t file)
{
	try {
		xml_document_t doc = new xml_document();
		doc->load(file);
		return doc;
	}
	catch (exception_t const& e) {
		e->what();
	}
	
}

xml_document::xml_document(xml_encoding_t e)
	: m_count(0)
	, m_stand_alone(null)
	, m_version(null)
	, m_encoding(e)
	, m_first(null)
	, m_root(null)
	, m_last(null)
	, m_current(this)
{
	m_factory = text::istring_factory::get_factory(e == xml_encoding::auto_detect ? xml_encoding::utf8 : e.get());
	m_version = m_factory->create_string("1.0");
	m_encoding = m_factory->encoding();
}

xml_document::xml_document(xml_document const* doc)
	: m_count(0)
	, m_stand_alone(null)
	, m_version(null)
	, m_encoding(xml_encoding::auto_detect)
	, m_first(null)
	, m_root(null)
	, m_last(null)
	, m_current(this)
{
	if (doc)
	{
		xml_node_t node = doc->xml_first();
		if(node) 
			insert_first(node->xml_clone(this));
		while (!node.is_empty())
		{
			insert_last(node->xml_clone(this));
			if (node.get() == doc->xml_root())
				xml_root(xml_last());
			node = node->xml_next_sibling();
		}
	}
	m_factory = text::istring_factory::get_factory(doc->xml_encoding() == xml_encoding::auto_detect ? xml_encoding::utf8 : doc->xml_encoding());
	m_version = m_factory->create_string("1.0");
	m_encoding = m_factory->encoding();
}


xml_document::~xml_document()
{
	clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_document);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_document, object, ixml_document);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_document, object, ixml_document, ixml_object, ixml_items);

xml_encoding_t xml_document::xml_encoding()const
{
	return m_encoding;
}

xml_type_t xml_document::xml_type()const
{
	return xml_type::document;
}

bool xml_document::xml_is_type_of(xml_type_t type)const
{
	return xml_type::document == type;
}

void xml_document::clear()
{
	m_current.xml_child(null);
	m_current.xml_current(null);

	xml_node_t node = xml_first();
	xml_node_t temp;
	while (!node.is_empty())
	{
		temp = node->xml_next_sibling();
		node->xml_next_sibling(null);
		if (!temp.is_empty())
			temp->xml_prev_sibling(null);
		node = temp;
	}
	xml_first(null);
	xml_root(null);
	xml_last(null);
	m_last_parsing_error = null;
}

streams::itext_output_stream_t& xml_document::xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flags, ushort level)const
{
	stream << L"<?xml "_s << L"version=\""_s << (xml_cstr_t)m_version << L"\""_s;
	stream << L" encoding="_s << L"\""_s << (m_factory.is_empty() ? "utf-8"_s : m_factory->encoding().to_string()) << L"\""_s;
	if (!m_stand_alone.is_empty())
		stream << L" standalone=\""_s << m_stand_alone->to_string() << "\""_s;
	stream << L"?>"_s;	

	xml_node_t node = xml_first();
	while (!node.is_empty())
	{
		node->xml_print(stream, flags, level);
		node = node->xml_next_sibling();
		if (bool(flags & xml_format::wrap_text_space) || bool(flags & xml_format::wrap_text_tab))
			stream << L"\n"_s;
	}
	return stream;
}


wsize xml_document::counter()const
{
	return m_count;
}

xml_node_t xml_document::at(const xml_base_iterator_t& it)
{
	//if (!counter()) throw(ang::exception_t(except_code::invalid_memory));
	if (it.parent() != this)throw(ang::exception_t(except_code::invalid_param));
	return reinterpret_cast<xml_node*>(it.current());
}

bool xml_document::increase(xml_base_iterator_t& it)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	xml_node * other,* node = reinterpret_cast<xml_node*>(it.current());
	
	if (node)
	{
		if (other = node->xml_first_child())	
			node = other;
		else do
			if (other = node->xml_next_sibling()) {
				node = other; break;
			}
		while (node = node->xml_parent());
	}
	it = xml_iterator_t(const_cast<xml_document*>(this), node);
	return true;
}

bool xml_document::increase(xml_base_iterator_t& it, int offset)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	xml_node * other, *node = reinterpret_cast<xml_node*>(it.current());
	while (node && offset > 0)
	{
		offset--;
		if (other = node->xml_first_child())
			node = other;
		else do
			if (other = node->xml_next_sibling()) {
				node = other; break;
			}
		while (node = node->xml_parent());
	}

	it = xml_iterator_t(const_cast<xml_document*>(this), node);
	return true;
}

bool xml_document::decrease(xml_base_iterator_t&it)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	xml_node * other, *node = reinterpret_cast<xml_node*>(it.current());

	if (node)
	{
		if (other = node->xml_prev_sibling())
			do node = other; while (other = node->xml_last_child());
		else		
			node = node->xml_parent();
		
	}
	it = xml_iterator_t(const_cast<xml_document*>(this), node);
	return true;
}

bool xml_document::decrease(xml_base_iterator_t&it, int offset)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	xml_node * other, *node = reinterpret_cast<xml_node*>(it.current());

	while (node && offset > 0)
	{
		offset--;
		if (other = node->xml_prev_sibling())
			do node = other; while (other = node->xml_last_child());
		else
			node = node->xml_parent();

	}
	it = xml_iterator_t(const_cast<xml_document*>(this), node);
	return true;
}

xml_forward_iterator_t xml_document::begin()
{
	return xml_iterator_t(const_cast<xml_document*>(this), (xml_node*)xml_first());
}

xml_forward_iterator_t xml_document::end()
{
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

xml_const_forward_iterator_t xml_document::begin()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), (xml_node*)xml_first());
}

xml_const_forward_iterator_t xml_document::end()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

xml_forward_iterator_t xml_document::last()
{
	return xml_iterator_t(const_cast<xml_document*>(this), (xml_node*)xml_last());
}

xml_const_forward_iterator_t xml_document::last()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), (xml_node*)xml_last());
}

xml_backward_iterator_t xml_document::rbegin()
{
	xml_node* child = null, *node = xml_last();
	while (node && (child = node->xml_last_child())) node = child;
	return xml_iterator_t(const_cast<xml_document*>(this), (xml_node*)node);
}

xml_backward_iterator_t xml_document::rend()
{
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

xml_const_backward_iterator_t xml_document::rbegin()const
{
	xml_node* child = null, *node = xml_last();
	while (node && (child = node->xml_last_child())) node = child;
	return xml_iterator_t(const_cast<xml_document*>(this), (xml_node*)node);
}

xml_const_backward_iterator_t xml_document::rend()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

/////////////////////////////////////////////////////////////////////////
ixml_text_t xml_document::create_cdata(xml_cstr_t cstr)const
{
	return m_factory->create_string(cstr);
}

ixml_text_t xml_document::create_pcdata(xml_cstr_t cstr)const
{
	//TODO: validate characters
	return m_factory->create_string(cstr);
}

bool xml_document::begin_element(xml_cstr_t name)
{
	return begin_element(m_factory->create_string(name));
}

bool xml_document::push_element(xml_cstr_t name, xml_cstr_t value)
{
	return push_element(m_factory->create_string(name), m_factory->create_string(value));
}

bool xml_document::push_element(xml_cstr_t name)
{
	return push_element(m_factory->create_string(name));
}

bool xml_document::push_data(xml_cstr_t value)
{
	return push_data(m_factory->create_string(value));
}

bool xml_document::push_value(xml_cstr_t value)
{
	return push_value(m_factory->create_string(value));
}

bool xml_document::push_attribute(xml_cstr_t name, xml_cstr_t value)
{
	return push_attribute(m_factory->create_string(name), m_factory->create_string(value));
}

bool xml_document::push_namespace(xml_cstr_t name, xml_cstr_t value)
{
	ixml_text_t ns = m_factory->create_string("xmlns"_s);
	if (name.size() != 0)
	{
		ns->concat(":"_s);
		ns->concat(name);
	}	
	return push_attribute(ns, m_factory->create_string(value));
}

bool xml_document::push_comment(xml_cstr_t value)
{
	return push_comment(m_factory->create_string(value));
}



ixml_text_view_t xml_document::xml_version()const
{
	return m_version.get();
}

bool xml_document::xml_stand_alone()const
{
	return m_stand_alone;
}

xml_node_t xml_document::xml_data_type()const
{
	return null;
}

xml_node_t xml_document::xml_root_element()const
{
	return m_root;
}

xml_document_t xml_document::xml_clone()const
{
	return NEW xml_document(this);
}

xml_iterator_t xml_document::find(xml_cstr_t name, bool invert)const
{
	if (invert)
	{
		for (xml_const_backward_iterator_t it = rbegin(); it != rend(); it++)
		{
			if((xml_cstr_t)it->xml_name() == name)
				return it;
		}
	}
	else
	{
		for (xml_const_forward_iterator_t it = begin(); it != end(); it++)
		{
			if ((xml_cstr_t)it->xml_name() == name)
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

xml_iterator_t xml_document::find(xml_cstr_t name, xml_iterator_t nextTo, bool invert)const
{
	if (!nextTo.is_valid() || nextTo.parent() != this)
		nextTo = (invert) ? xml_iterator_t(rbegin()) : xml_iterator_t(begin());

	if (invert)
	{
		for (xml_const_backward_iterator_t it = nextTo; it != rend(); it--)
		{
			if ((xml_cstr_t)it->xml_name() == name)
				return it;
		}
	}
	else
	{
		for (xml_const_forward_iterator_t it = nextTo; it != end(); it++)
		{
			if ((xml_cstr_t)it->xml_name() == name)
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

xml_node* xml_document::xml_first()const { return m_first; }
xml_node* xml_document::xml_root()const { return m_root; }
xml_node* xml_document::xml_last()const { return m_last; }

//void xml_document::insert_next_to(xml_node*, xml_node*);
//void xml_document::insert_prev_to(xml_node*, xml_node*);
//void xml_document::insert_in_head(xml_node*);
//void xml_document::insert_in_tail(xml_node*);


void xml_document::xml_first(xml_node_t node)
{
	if (node.is_empty()
		|| node->xml_is_type_of(xml_type::element)
		|| node->xml_is_type_of(xml_type::comment))
	{
		if (m_first == node.get()) return;
		auto old = m_first;
		m_first = node.get();
		if (m_first)m_first->add_ref();
		if (old)old->release();
	}
}

void xml_document::xml_root(xml_node_t node)
{
	if (node.is_empty() || node->xml_is_type_of(xml_type::element))
	{
		if (m_root == node.get()) return;
		auto old = m_root;
		m_root = (!node.is_empty()) ? node->xml_as<xml_element>().get() : null;
		if (m_root)m_root->add_ref();
		if (old)old->release();
	}
}

void xml_document::xml_last(xml_node_t node)
{
	if (node.is_empty()
		|| node->xml_is_type_of(xml_type::element)
		|| node->xml_is_type_of(xml_type::comment))
	{
		if (m_last == node.get()) return;
		auto old = m_last;
		m_last = node.get();
		if (m_last)m_last->add_ref();
		if (old)old->release();
	}
}

void xml_document::insert_next_to(xml_node* refNode, xml_node* newNode)
{
	refNode->xml_next_sibling()->xml_prev_sibling(newNode);
	newNode->xml_next_sibling(refNode->xml_next_sibling());
	newNode->xml_prev_sibling(refNode);
	refNode->xml_next_sibling(newNode);
}

void xml_document::insert_prev_to(xml_node* refNode, xml_node* newNode)
{
	refNode->xml_prev_sibling()->xml_next_sibling(newNode);
	newNode->xml_prev_sibling(refNode->xml_prev_sibling());
	newNode->xml_next_sibling(refNode);
	refNode->xml_prev_sibling(newNode);
}

void xml_document::insert_first(xml_node* newNode)
{
	xml_first()->xml_prev_sibling(newNode);
	newNode->xml_next_sibling(xml_first());
	xml_first(newNode);
}

void xml_document::insert_last(xml_node* newNode)
{
	xml_last()->xml_next_sibling(newNode);
	newNode->xml_prev_sibling(xml_last());
	xml_last(newNode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

xml_iterator_t xml_document::xml_current()const
{
	return m_current.current();
}

xml_node_t xml_document::xml_current_element()const
{
	if (!m_current.current().is_valid()) return null;
	return m_current.current()->xml_as<xml_element>();
}

bool xml_document::move_to(xml_iterator_t current)
{
	if (!current.is_valid() || current.parent() != static_cast<ixml_items*>(this))
		return false;
	m_current.xml_current(*current);
	m_current.end_child();
	return true;
}

bool xml_document::move_to_child(xml_iterator_t child)
{
	if (!child.is_valid() || child.parent() != static_cast<ixml_items*>(this))
		return false;
	if (child->xml_parent() == null)//IS a root
		return false;
	m_current.xml_current(child->xml_parent());
	m_current.xml_child(*child);
	return true;
}


bool xml_document::move_up()
{
	return m_current.prev_Child();
}

bool xml_document::move_down()
{
	return m_current.next_child();
}

bool xml_document::move_forward()
{
	return m_current.forward();
}

bool xml_document::move_backward()
{
	return m_current.backward();
}

void xml_document::push_header(xml_cstr_t version, nullable<bool> standalone)
{
	m_version = m_factory->create_string(version);
	m_stand_alone = standalone;
}

bool xml_document::begin_element(ixml_text_t name)
{
	if (xml_last() == null)
	{
		xml_node_t element = xml_element::create_new(this, name);
		xml_first(element.get());
		xml_last(element.get());
		xml_root(element.get());
		m_current.xml_current(element.get());
	}
	else if (xml_root() == null)
	{
		xml_root(xml_element::create_new(this, name));
		m_current.xml_current(xml_root());
		xml_last()->xml_next_sibling(xml_root());
		xml_root()->xml_prev_sibling(xml_last());
		xml_last(xml_root());
	}
	else if (m_current.xml_current().get() == null)
	{
		return false;
	}
	else
	{
		xml_node_t current = m_current.xml_current();
		if (current.get() == null || !current->xml_is_type_of(xml_type::element))
			return false;

		current->push_child((xml_node_t)xml_element::create_new(this, name), xml_iterator_t(current->xml_children(), m_current.xml_child().get()));
		m_current.next_child();
		m_current.forward();
	}
	return true;
}

bool xml_document::end_element()
{
	if (m_current.xml_current() == null)
		return false;
	m_current.backward();
	return true;
}

bool xml_document::push_element(ixml_text_t name)
{
	if (xml_last() == null)
	{
		xml_root(xml_element::create_new(this, name));
		xml_first(xml_root());
		xml_last(xml_first());
	}
	else if (xml_root() == null)
	{
		xml_root(xml_element::create_new(this, name));

		xml_last()->xml_next_sibling(xml_root());
		xml_root()->xml_prev_sibling(xml_last());
		xml_last(xml_root());
	}
	else
	{
		if (m_current.xml_current().get() == null)
		{
			return false;
		}
		else
		{
			xml_node_t current = m_current.xml_current();
			if (current.get() == null || !current->xml_is_type_of(xml_type::element))
				return false;
			current->push_child(xml_element::create_new(this, name), xml_iterator_t(current->xml_children(), m_current.xml_child().get()));
			m_current.next_child();
		}
	}
	return true;
}

bool xml_document::push_element(ixml_text_t name, ixml_text_t value)
{
	if (xml_last() == null)
	{
		xml_root(xml_element::create_new(this, name, value));
		xml_first(xml_root());
		xml_last(xml_first());
	}
	else if (xml_root() == null)
	{
		xml_root(xml_element::create_new(this, name, value));

		xml_last()->xml_next_sibling(xml_root());
		xml_root()->xml_prev_sibling(xml_last());
		xml_last(xml_root());
	}
	else
	{
		if (m_current.xml_current().get() == null)
		{
			return false;
		}
		else
		{
			xml_node_t current = m_current.xml_current();
			if (current.get() == null || !current->xml_is_type_of(xml_type::element))
				return false;
			current->push_child(xml_element::create_new(this, name, value), xml_iterator_t(current->xml_children(), m_current.xml_child().get()));
			m_current.next_child();
		}
	}
	return true;
}

bool xml_document::push_comment(ixml_text_t value)
{
	if (xml_last() == null)
	{
		xml_first(xml_comment::create_new(this, value));
		xml_last(xml_first());
	}
	else
	{
		if (m_current.xml_current().get() == null)
		{
			xml_last()->xml_next_sibling(xml_comment::create_new(this, value));
			xml_last()->xml_next_sibling()->xml_prev_sibling(xml_last());
			xml_last(xml_last()->xml_next_sibling());
		}
		else
		{
			xml_node_t current = m_current.xml_current();
			if (!current->xml_is_type_of(xml_type::element))
				return false;
			current->push_child(xml_comment::create_new(this, value), xml_iterator_t(current->xml_children(), m_current.xml_child().get()));
			m_current.next_child();
		}
	}
	return true;
}


bool xml_document::push_data(ixml_text_t value)
{
	if (m_current.xml_current().get() == null)
		return false;
	xml_node_t current = m_current.xml_current();
	if (!current->xml_is_type_of(xml_type::element))
		return false;
	current->push_data(value);
	return true;
}

bool xml_document::push_value(ixml_text_t value)
{
	if (m_current.xml_current().get() == null)
		return false;
	xml_node_t current = m_current.xml_current();
	if (!current->xml_is_type_of(xml_type::element))
		return false;
	current->push_value(value);
	return true;
}

bool xml_document::push_attribute(ixml_text_t name, ixml_text_t value)
{
	if (m_current.xml_current() == null)
		return false;
	xml_node_t current = m_current.xml_current();
	if (!current->xml_is_type_of(xml_type::element))
		return false;
	current->push_attribute(xml_attribute::create_new(this, name, value));
	return true;
}



////////////////////////////////////////////////////////////////////////////

static xml_encoding_t  xml_detect_encoding(pointer bom, xml_encoding_t e, windex& idx)
{
	if (bom == null)
		return xml_encoding::none;
	
	/*alignas(4) static byte utf8_bom[4] = { 0xef, 0xbb, 0xbf, 0x0 };
	alignas(4) static byte utf16_le_bom[4] = { 0xff, 0xfe, 0x0, 0x0 };
	alignas(4) static byte utf16_be_bom[4] = { 0xfe, 0xff, 0x0, 0x0 };
	alignas(4) static byte utf32_le_bom[8] = { 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	alignas(4) static byte utf32_be_bom[8] = { 0x0, 0x0, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0 };*/

	switch (e.get())
	{
	case text::encoding::auto_detect:
		if (text::load_bom<text::encoding::utf8>(bom) > 0)
		{
			idx += 3; 
			return text::encoding::utf8;
		}
		else if (text::load_bom<text::encoding::utf32_le>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf32_le>();
		}
		else  if (text::load_bom<text::encoding::utf32_be>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf32_be>();
		}
		else if (text::load_bom<text::encoding::utf16_le>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_le>();
		}
		else  if (text::load_bom<text::encoding::utf16_be>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_be>();
		}

		else if (text::utf8::compare_until((mchar const*)bom, u8"<?xml"_sm.cstr()) == 5)
			return text::encoding::utf8;
		else if (text::utf32::compare_until((char32_t const*)bom, U"<?xml") == 5)
			return text::encoding::utf32;
		else if (text::utf32_se::compare_until((char32_t const*)bom, U"<?xml") == 5)
			return text::encoding::utf32_se;
		else if (text::utf16::compare_until((char16_t const*)bom, u"<?xml") == 5)
			return text::encoding::utf16;
		else if (text::utf16_se::compare_until((char16_t const*)bom, u"<?xml") == 5)
			return text::encoding::utf16_se;	

		return text::encoding::utf8;

	case text::encoding::utf16_le: return text::native_encoding<text::encoding::utf16_le>();
	case text::encoding::utf16_be: return text::native_encoding<text::encoding::utf16_be>();
	case text::encoding::utf32_le: return text::native_encoding<text::encoding::utf32_le>();
	case text::encoding::utf32_be: return text::native_encoding<text::encoding::utf32_be>();
	default:
		return e;
	}
}


void xml_document::load(core::files::input_text_file_t file)
{
	ang::exception_t ex = null;
	if (!file->map([&](ibuffer_view_t data)
	{
		try {
			parse(data); return true;
		}
		catch (ang::exception_t e) {
			ex = e; return false; 
		}
	})) {

		if (ex.is_empty())throw_exception(except_code::unknown);
		else throw ex;
	}
}

void xml_document::save(core::files::output_text_file_t file)const
{

}


void xml_document::parse(ixml_text_view_t view)
{
	windex idx = 0;
	ixml_text_t version = null;
	nullable<bool> standalone = null;
	xml_encoding_t encoding;

	encoding = xml_detect_encoding(view->data(), xml_encoding::auto_detect, idx);
	
	if (decode_header(view, idx, version, encoding, standalone))
	{
		if (m_encoding != encoding && encoding != xml_encoding::auto_detect)
		{
			m_encoding = encoding;
			m_factory = text::istring_factory::get_factory(encoding);
			//code = doc->m_factory->create_wrapper()
		}
		push_header(version, standalone);
	}
	
	if (!decode_elements(this, view, idx))
	{
		clear();
		string error = "";
		error->format("exception: ang::xml::xml_document: {0}"_s, m_last_parsing_error);
		throw(exception_t((dword)xml_exception_code::parsing_error, ang::move(error)));
	}
}

void xml_document::parse(ibuffer_view_t data)
{
	windex idx = 0;
	ixml_text_t version = null;
	nullable<bool> standalone = null;
	xml_encoding_t encoding;

	encoding = xml_detect_encoding(data->buffer_ptr(), xml_encoding::auto_detect, idx);
	m_encoding = encoding;
	m_factory = text::istring_factory::get_factory(encoding);
	ixml_text_view_t view = m_factory->create_wrapper(data);

	if (decode_header(view, idx, version, encoding, standalone))
	{
		if (m_encoding != encoding && encoding != xml_encoding::auto_detect)
		{
			m_encoding = encoding;
			m_factory = text::istring_factory::get_factory(encoding);
			view = m_factory->create_wrapper(data);
		}
		push_header(version, standalone);
	}

	if (!decode_elements(this, view, idx))
	{
		clear();
		string error = "";
		error->format("exception: ang::xml::xml_document: {0}"_s, m_last_parsing_error);
		throw(exception_t((dword)xml_exception_code::parsing_error, ang::move(error)));
	}
}

bool xml_document::decode_header(text::istring_view_t code, windex& idx, ixml_text_t& version, xml_encoding_t& encoding, nullable<bool>& standalone)
{
	ixml_text_t value = null;
	ixml_text_t header = null;

	windex beg;
	windex end;

	beg = code->find("<?xml"_s, idx, idx + 5);

	if (beg == invalid_index)
		return false;

	end = code->find("?>"_s, beg);
	if (end == invalid_index)
		return false;

	code->sub_string(&header, beg, end + 2);

	idx = end + 2;

	beg = header->find("version"_s);
	if (beg != invalid_index)
	{
		beg = header->find("\""_s, beg);
		beg++;
		end = header->find(L"\""_s, beg);
		header->sub_string(&version, beg, end);
	}

	beg = header->find("encoding"_s, beg);
	if (beg != invalid_index)
	{
		beg = header->find("\""_s, beg);
		beg++;
		end = header->find("\""_s, beg);
		header->sub_string(&value, beg, end);
		encoding = xml_encoding_t::parse(value->cstr());
	}

	return true;
}


#if defined _DEBUG
bool xml_document::decode_elements(xml_document_t doc, text::istring_view_t code, windex& beg)
#else
bool xml_document::decode_elements(xml_document_t doc, text::istring_view_t code, windex& beg)
#endif
{
//#if defined _DEBUG
//	xml_parsing_iterator beg(code.data(), __beg);
//#endif

	windex end;
	uint c = 0;
	ixml_text_t name = null;// = { 0 };
	ixml_text_t value = null;

	beg = code->find("<"_s, beg);
	while (beg != invalid_index)
	{
		c = 0;
		//Check if the node is a comment
		wsize find = code->find("<!--"_s, beg, beg + 5);
		if (find == beg)
		{
			end = code->find("-->"_s, beg);
			if (end != invalid_index)
			{
				//get comment value
				code->sub_string(&value, beg + 4, end - 4);

				//insert comment
				doc->push_comment(value);
			}
			//Finding next node start position
			beg = code->find("<"_s, end);
			continue;
		}

		//Check if the node is a CDATA
		find = code->find("<![CDATA["_s, beg, beg + 10);
		if (find == beg)
		{
			end = code->find("]]>"_s, beg);
			if (end != invalid_index)
			{
				//get CDATA value
				code->sub_string(&value, beg + 9, end - 4);

				//insert comment
				doc->push_data(value);
			}
			//Finding next node start position
			beg = code->find("<"_s, end);
			continue;
		}

		//checking if it is a node ending
		if (code[beg + 1] == U'/')
		{
			xml_node_t current = doc->xml_current_element();
			if (current == null)//unkown error
			{
				doc->m_last_parsing_error = "unknown Error"_s;
				return false;
			}
			beg += 2;

			//get element name
			bool same = true;
			char32 nC = 0;
			ixml_text_view_t temp = current->xml_name();
			wsize length = current->xml_name()->length();
			c = 0;
			while (same)
			{
				nC = code[beg + c];
				if (c > length)
					same = false;
				else if (c == length)
				{
					if (nC != U' ' && nC != U'\t' && nC != U'\r' && nC != U'\n' && nC != U'>')
						same = false;
					else
						break;
				}
				else if (nC != temp[c])
				{
					same = false;
				}
				c++;
			}

			if (!same)
			{
				doc->m_last_parsing_error = "";
				doc->m_last_parsing_error->format("bad end of element <{0}> in position {1}"_s, interface_cast<ivariant>(current->xml_name().get()), (index)beg);
				return false;
			}//parsing error

			beg += c;

			find = code->find(">"_s, beg);
			if (find == invalid_index)
			{
				doc->m_last_parsing_error = "";
				doc->m_last_parsing_error->format("missing character > in position {0}"_s, (index)beg + current->xml_name()->length());
				return false;
			}//parsing error
			beg = find;

			doc->end_element();
			beg = code->find("<"_s, beg);
			continue;
		}

		//get element name
		name.reset();
		c = xml_utils<void,xml_encoding::auto_detect>::xml_get_element_name(code, beg);
		code->sub_string(&name, beg + 1, beg + c);

		doc->begin_element(name.get());
		beg += c;
		c = 0;

		//parse attributes
		while (code[beg] != U'>')
		{
			beg += xml_utils<void, xml_encoding::auto_detect>::xml_skip_space(code, beg);
			if (code[beg] == U'>')
			{
				break;
			}
			else if (code[beg] == U'/' && code[beg + 1] == U'>')
			{
				++beg;
				break;
			}

			name.reset();
			c = xml_utils<void, xml_encoding::auto_detect>::xml_get_attibute_name(code, beg);
			code->sub_string(&name, beg, beg + c);
			beg += c;
			c = 0;
			beg = code->find("="_s, beg);
			if (beg == invalid_index)
			{
				doc->m_last_parsing_error = "";
				doc->m_last_parsing_error->format("unexpected end of file parsing element {0}"_s, interface_cast<ivariant>(name.get()));
				return false;
			}

			++beg;

			beg += xml_utils<void, xml_encoding::auto_detect>::xml_skip_space(code, beg);
			c = 0;

			if (code[beg] == U'"')
			{
				end = code->find("\""_s, beg + 1);
			}
			else if (code[beg] == U'\'')
			{
				end = code->find("\'"_s, beg + 1);
			}
			else//parsing error
			{
				doc->m_last_parsing_error = "";
				doc->m_last_parsing_error->format("unexpected character in position {0}"_s, (index)beg);
				return false;
			}
			if (end == invalid_index)//parsing error
			{
				doc->m_last_parsing_error = "";
				doc->m_last_parsing_error->format("missing character \' or \" in position {0}"_s, (index)beg);
				return false;
			}

			value.reset();
			code->sub_string(&value, beg + 1, end);
			doc->push_attribute(name.get(), value.get());
			beg = end + 1;

		}

		//if is not an empty node
		if (code[beg - 1] == U'/')
		{
			doc->end_element();
			beg = code->find("<"_s, beg);
			continue;
		}
		else
		{
			++beg;

			c = xml_utils<void, xml_encoding::auto_detect>::xml_skip_space(code, beg);

			if (code[beg + c] == '<')
			{
				//checking if it is a node ending
				if (code[beg + c + 1] == '\'')
				{
					value.reset();
					code->sub_string(&value, beg, beg + c);
					doc->push_value(value.get());
					beg += c;

					xml_node_t current = doc->xml_current_element();
					if (current.is_empty())//unkown error
					{
						doc->m_last_parsing_error = "unknown Error"_s;
						return false;
					}
					beg += 2;

					//get element name
					bool same = true;
					char32_t _c = 0;
					ixml_text_t temp = current->xml_name();
					wsize length = current->xml_name()->length();
					c = 0;
					while (same)
					{
						_c = code[beg + c];
						if (c > length)
							same = false;
						else if (c == length)
						{
							if (_c != ' ' && _c != U'\t' && _c != U'\r' && _c != U'\n' && _c != U'>')
								same = false;
							else
								break;
						}
						else if (_c != temp[c])
						{
							same = false;
						}
						c++;
					}

					if (!same)
					{
						doc->m_last_parsing_error = "";
						doc->m_last_parsing_error->format("bad end of element <{0}> in position {1}"_s, interface_cast<ivariant>(current->xml_name().get()), (index)beg);
						return false;
					}//parsing error

					beg += c;

					find = code->find(">"_s, beg);
					if (find == invalid_index)
					{
						doc->m_last_parsing_error = "";
						doc->m_last_parsing_error->format("missing character > in position {0}"_s, (index)beg + current->xml_name()->length());
						return false;
					}//parsing error
					beg = find;
					doc->end_element();
					beg = code->find("<"_s, beg);
					continue;
				}
				else
				{
					//parsing child
					beg += c;
					continue;
				}
			}
			else
			{
				end = code->find("<"_s, beg + c);
				if (end == invalid_index)
				{
					doc->m_last_parsing_error = "";
					doc->m_last_parsing_error->format("missing character < in position {0}"_s, (index)beg + c);
					return false;
				}//parsing error
				value.reset();
				code->sub_string(&value, beg, end);
				doc->push_value(value.get());
				beg = end;
				continue;
			}
		}
	}

	return true;
}
