/*********************************************************************************************************************/
/*   File Name: xml_document_builder.cpp                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                              */
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


xml_iterator_t xml_document::current()const
{
	return m_cursor.child();
}

ixml_node_t xml_document::current_element()const
{
	return m_cursor.child_node().get();
}

bool xml_document::move_to(xml_iterator_t it)
{
	if (!it.is_valid() || it.parent() != static_cast<ixml_items*>(this))
		return false;
	m_cursor.current_node((xml_node*)it.current());
	m_cursor.end_child();
	return true;
}

bool xml_document::move_to_child(xml_iterator_t it)
{
	if (!it.is_valid() || it.parent() != static_cast<ixml_items*>(this))
		return false;
	xml_node* child = (xml_node*)it.current();
	if (child->to_intf<xml_node_base>()->parent() == null)//it is the root
		return false;
	m_cursor.current_node(child->to_intf<xml_node_base>()->parent());
	m_cursor.child_node(child);
	return true;
}

bool xml_document::move_up()
{
	return m_cursor.prev_child();
}

bool xml_document::move_down()
{
	return m_cursor.next_child();
}

bool xml_document::move_forward()
{
	return m_cursor.forward();
}

bool xml_document::move_backward()
{
	return m_cursor.backward();
}

ixml_text_t xml_document::create_cdata(cstr_t value)const
{
	return xml_text_factory::create_text(m_encoding, value);
}

ixml_text_t xml_document::create_pcdata(cstr_t value)const
{
	//TODO: validate characters
	return xml_text_factory::create_text(m_encoding, value);
}

void xml_document::push_header(cstr_t version, nullable<bool> standalone)
{
	m_version = create_pcdata(version);
	m_stand_alone = standalone.is_empty() ? stand_alone_none
		: standalone->get() ? stand_alone_true : stand_alone_false;
}

bool xml_document::begin_element(cstr_t name)
{	
	if (root_element() == null)
	{
		m_root = xml_node::create_element(this, name);
		push_back(m_root);	
		m_cursor.current_node(m_root.get());
		m_count++;
		return true;
	}
	else if (m_cursor.current_node() != null)
	{
		xml_node_t current = m_cursor.current_node();

		if (current == null || !current->is_type_of(xml_type::element))
			return false;

		auto list = current->to_intf<ixml_list>();
		if (m_cursor.child_node() == null)
		{
			list->push_back(xml_node::create_element(this, name));
		}
		else
		{
			auto it = ++xml_forward_iterator_t(list, m_cursor.child_node(), 0);
			it = list->insert(it, xml_node::create_element(this, name));
		}
		m_cursor.next_child();
		m_cursor.forward();
		m_count++;
		return true;
	}
	return false;
}

bool xml_document::end_element()
{
	if (m_cursor.current_node() == null)
		return false;
	m_cursor.backward();
	m_count++;
	return true;
}

bool xml_document::push_element(cstr_t name, cstr_t value)
{
	if (root_element() == null)
	{
		m_root = xml_node::create_element(this, name, value);
		push_back(m_root);
		m_count++;
		return true;
	}
	xml_node_t current = m_cursor.current_node();
	if (current != null && current->is_type_of(xml_type::element))
	{
		ixml_list_t list = current->to_intf<ixml_list>();
		xml_node_t child = m_cursor.child_node();
		if (child == null)
		{
			list->push_back(xml_node::create_element(this, name, value));
		}
		else
		{
			auto it = ++xml_forward_iterator_t(list, m_cursor.child_node(), 0);
			it = list->insert(it, xml_node::create_element(this, name));
		}		
		m_cursor.next_child();
		m_count++;
		return true;
	}
	return false;
}

bool xml_document::push_element(cstr_t name)
{
	if (root_element() == null)
	{
		m_root = xml_node::create_element(this, name);
		push_back(m_root);
		m_count++;
		return true;
	}
	xml_node_t current = m_cursor.current_node();
	if (current != null && current->is_type_of(xml_type::element))
	{
		ixml_list_t list = current->to_intf<ixml_list>();
		xml_node_t child = m_cursor.child_node();
		if (child == null)
		{
			list->push_back(xml_node::create_element(this, name));
		}
		else
		{
			auto it = ++xml_forward_iterator_t(list, m_cursor.child_node(), 0);
			it = list->insert(it, xml_node::create_element(this, name));
		}
		m_cursor.next_child();
		m_count++;
		return true;
	}
	return false;
}

bool xml_document::push_data(cstr_t value)
{
	xml_node_t current = m_cursor.current_node();
	if (current &&  current->is_type_of(xml_type::element))
	{
		ixml_list_t list = current->to_intf<ixml_list>();
		xml_node_t child = m_cursor.child_node();
		if (child == null)
		{
			list->push_back(xml_node::create_cdata(this, value));
		}
		else
		{
			auto it = ++xml_forward_iterator_t(list, m_cursor.child_node(), 0);
			it = list->insert(it, xml_node::create_cdata(this, value));
		}
		m_cursor.next_child();
		m_count++;
		return true;
	}
	return false;
}

bool xml_document::push_value(cstr_t value)
{
	xml_node_t current = m_cursor.current_node();
	if (current &&  current->is_type_of(xml_type::element))
	{
		current->to_intf<xml_node_base>()->value(create_pcdata(value));
		m_count++;
		return true;
	}
	return false;
}

bool xml_document::push_attribute(cstr_t name, cstr_t value)
{
	xml_node_t current = m_cursor.current_node();
	if (current &&  current->is_type_of(xml_type::element))
	{
		auto atrribs = current->to_intf<xml_node_base>()->attributes();
		if (atrribs.is_empty()) {
			atrribs = xml_node::create_attribute_list(this);
			current->to_intf<xml_node_base>()->attributes(atrribs);
		}
		atrribs->push_back(xml_node::create_attribute(this, name, value));
		m_count++;
		return true;
	}
	return false;
}

bool xml_document::push_namespace(cstr_t name, cstr_t value)
{
	xml_node_t current = m_cursor.current_node();
	if (current &&  current->is_type_of(xml_type::element))
	{
		auto atrribs = current->to_intf<xml_node_base>()->attributes();
		if (atrribs.is_empty()) {
			atrribs = xml_node::create_attribute_list(this);
			current->to_intf<xml_node_base>()->attributes(atrribs);
		}
		atrribs->push_back(xml_node::create_attribute(this, name, value));
		m_count++;
		return true;
	}
	return false;
}

bool xml_document::push_comment(cstr_t value)
{
	if (back() == null || m_cursor.current_node() == null)
	{
		push_back(xml_node::create_comment(this, value));
		m_count++;
		return true;
	}

	xml_node_t current = m_cursor.current_node();
	if (current->is_type_of(xml_type::element))
	{
		ixml_list_t list = current->to_intf<ixml_list>();
		xml_node_t child = m_cursor.child_node();
		if (child == null)
		{
			list->push_back(xml_node::create_comment(this, value));
		}
		else
		{
			auto it = ++xml_forward_iterator_t(list, m_cursor.child_node(), 0);
			it = list->insert(it, xml_node::create_comment(this, value));
		}
		m_cursor.next_child();
		m_count++;
		return true;
	}
	return false;
}
