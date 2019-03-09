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


using namespace ang;
using namespace ang::dom::xml;

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


#define MY_TYPE ang::dom::xml::ixml_node
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::dom::xml::xml_node
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

dom::xml::xml_node_t object_wrapper<xml_node>::operator[] (dom::xml::xml_cstr_t key)const
{
	if (is_empty())
		return null;
	if (m_ptr->xml_has_attributes()) {
		auto it = m_ptr->xml_attributes()->find(key);
		if (it.is_valid())
			return*it;
	}
	if (m_ptr->xml_has_children()) {
		auto it = m_ptr->xml_children()->find(key);
		if (it.is_valid())
			return*it;
	}
	return null;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
xml_node::xml_node(xml_document_t doc, xml_type_t type)
	: m_type(type)
	, m_parent(null)
	, m_doc(doc)
	, m_prev(null)
	, m_next(null)
	, m_name(null)
	, m_content(null)
	, m_namespace(null)
	, m_attributes(null)
{
}

xml_node::~xml_node()
{
	//m_doc = null;
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_node);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_node, object, ixml_node);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_node, object, ixml_node, ixml_object);

void xml_node::clear()
{
	m_name = null;
	m_attributes = null;
	m_content = null;
	m_doc = null;
}

xml_type_t xml_node::xml_type()const
{
	return m_type;
}

bool xml_node::xml_is_type_of(xml_type_t type)const
{
	return (m_type == type || xml_type::node == type);
}

streams::itext_output_stream_t& xml_node::xml_print(streams::itext_output_stream_t& stream, xml_format_t const& flag, ushort level)const
{
	switch (m_type)
	{
	break;
	case xml_type::element:
		if (xml_has_name())
		{
			if (flag & xml_format::wrap_text_tab)
			{
				for (index i = 0; i < level; i++)
					stream << L"\t"_s;
			}
			else if (flag & xml_format::wrap_text_space)
			{
				for (index i = 0; i < level; i++)
					stream << L"  "_s;
			}

			stream << L"<"_s << m_name->cstr();

			if (xml_has_attributes())
			{
				stream << L" "_s;
				m_attributes->xml_print(stream, flag, level);
			}

			if (!m_content.is_empty())
			{
				stream << L">"_s;
				m_content->xml_print(stream, flag, level + 1);
				if (!m_content->xml_is_type_of(xml_type::text))
				{	
					stream << L"\n"_s;
					if (flag & xml_format::wrap_text_tab)
					{
						for (index i = 0; i < level; i++)
							stream << L"\t"_s;
					}
					else if (flag & xml_format::wrap_text_space)
					{
						for (index i = 0; i < level; i++)
							stream << L"  "_s;
					}
				}
				stream << L"</"_s << (xml_cstr_t)m_name << L">"_s;
			}
			else
			{
				stream << L"/>"_s;
			}
		}
		break;
	case xml_type::comment:
		if (flag & xml_format::wrap_text_tab)
		{
			for (index i = 0; i < level; i++)
				stream << L"\t"_s;
		}
		else if (flag & xml_format::wrap_text_space)
		{
			for (index i = 0; i < level; i++)
				stream << L"  "_s;
		}
		stream << L"<!--"_s;
		if (xml_has_value()) m_content->xml_print(stream, flag, level + 1);
		else stream << " "_s;
		stream << L"-->"_s;
		break;
	case xml_type::attribute:
		if (xml_has_name() && xml_has_value())
		{
			stream << (wstring)m_name;
			ixml_text_t val = m_content.get()->xml_as<ixml_text>();
			
			if (val->find("\""_s, 0) != invalid_index)
			{
				stream << L"='"_s;
				stream << (xml_cstr_t)val;
				//val->xml_print(stream, xml_format::none, 0);
				stream << L"'"_s;
			}
			else
			{
				stream << L"=\""_s;
				stream << (xml_cstr_t)val;
				//val->xml_print(stream, xml_format::none, 0);
				stream << L"\""_s;
			}
		}
		break;
	}
	return stream;
}

bool xml_node::xml_has_name()const { return false; }
bool xml_node::xml_has_value()const { return false; }
bool xml_node::xml_has_namespace()const { return false; }
bool xml_node::xml_has_children()const { return false; }
bool xml_node::xml_has_attributes()const { return false; }

xml_node_t xml_node::xml_parent()const
{
	return m_parent.lock();
}

xml_document_t xml_node::xml_parent_doc()const
{
	return m_doc.lock();
}

xml_node_t xml_node::xml_prev_sibling()const
{
	return m_prev;
}

xml_node_t xml_node::xml_next_sibling()const
{
	return m_next;
}

void xml_node::xml_parent(xml_node_t node)
{
	m_parent = node;
}


void xml_node::xml_prev_sibling(xml_node_t node)
{
	if (m_prev == node) return;
	auto _old = m_prev;
	m_prev = (node) ? node->xml_as<xml_node>().get() : null;
	if (m_prev)m_prev->add_ref();
	if (_old)_old->release();
}


void xml_node::xml_next_sibling(xml_node_t node)
{
	if (m_next == node) return;
	auto _old = m_next;
	m_next = (node) ? node->xml_as<xml_node>().get() : null;
	if(m_next)m_next->add_ref();
	if (_old)_old->release();
}

xml_node_t xml_node::xml_first_child()const
{
	if(m_content.is_empty() || !m_content->xml_is_type_of(xml_type::collection))
		return null;
	return *m_content.get()->xml_as<ixml_collection>()->begin();
}

xml_node_t xml_node::xml_last_child()const
{
	if (m_content.is_empty() || !m_content->xml_is_type_of(xml_type::collection))
		return null;
	return *m_content.get()->xml_as<ixml_collection>()->rbegin();
}

xml_text_t xml_node::xml_name()const
{
	return m_name.get();
}

xml_text_t xml_node::xml_value()const
{
	if (m_content.is_empty())
		return null;
	return m_content.get()->xml_as<xml_text>();
}

xml_text_t xml_node::xml_namespace()const
{
	return m_namespace.get();
}

xml_node_t xml_node::xml_namespace(xml_cstr_t key)const
{
	try {
		return m_ns_map[key].lock();
	}
	catch (exception_t const& e) {
		xml_node_t parent = m_parent.lock();
		return parent.is_empty() ? null : parent->xml_namespace(key);
	}
}

ixml_collection_t xml_node::xml_children()const
{
	if (m_content.is_empty())
		return null;
	return m_content.get()->xml_as<ixml_collection>();
}

xml_attributes_t xml_node::xml_attributes()const
{
	return m_attributes.get();
}

bool xml_node::push_data(xml_cstr_t value)
{
	if (m_type != xml_type::element)
		return false;
	xml_document_t doc = xml_parent_doc();
	xml_node_t child = NEW xml_cdata(doc);
	child->push_value(doc->create_cdata(value));
	return push_child(child);
}

bool xml_node::push_name(xml_cstr_t value)
{
	if (m_type != xml_type::attribute
		&& m_type != xml_type::name_space
		&& m_type != xml_type::element)
		return false;
	xml_document_t doc = m_doc.lock();
	if (m_name.is_empty())
		m_name = new xml_text(doc, doc->create_pcdata(value));
	else
		m_name->move(doc->create_pcdata(value));
	return true;
}

bool xml_node::push_value(xml_cstr_t value)
{
	xml_document_t doc = xml_parent_doc();
	if (xml_is_type_of(xml_type::attribute) || xml_is_type_of(xml_type::comment))
	{
		if (m_content.is_empty())
			m_content = NEW xml_text(doc, doc->create_pcdata(value));
		else
			m_content->xml_as<xml_text>()->move(doc->create_pcdata(value));
		return true;
	}
	else if (xml_is_type_of(xml_type::cdata))
	{
		if (m_content.is_empty())
			m_content = NEW xml_text(doc, doc->create_pcdata(value));
		else
			m_content->xml_as<xml_text>()->move(doc->create_pcdata(value));
		return true;
	}
	else if (xml_is_type_of(xml_type::element))
	{
		if (m_content.is_empty() || m_content->xml_is_type_of(xml_type::element_list))
			m_content = NEW xml_text(doc, doc->create_pcdata(value));
		else if (m_content->xml_is_type_of(xml_type::cdata))
			m_content->xml_as<xml_cdata>()->push_value(doc->create_pcdata(value));
		else if (m_content->xml_is_type_of(xml_type::text))
			m_content->xml_as<xml_text>()->move(doc->create_pcdata(value));
		else
			return false;
		return true;
	}
	return false;
}

bool xml_node::push_name(ixml_text_t value)
{
	if (m_type != xml_type::attribute
		&& m_type != xml_type::name_space
		&& m_type != xml_type::element)
		return false;

	if (m_name.is_empty())
		m_name = new xml_text(m_doc.lock(), value);
	else
		m_name->move(value);
	return true;
}

bool xml_node::push_value(ixml_text_t value)
{
	xml_document_t doc = xml_parent_doc();
	if (xml_is_type_of(xml_type::attribute) || xml_is_type_of(xml_type::comment))
	{	
		if (m_content.is_empty())
			m_content = NEW xml_text(doc, value);
		else
			m_content->xml_as<xml_text>()->move(value);
		return true;
	}
	else if (xml_is_type_of(xml_type::cdata))
	{
		if (m_content.is_empty())
			m_content = NEW xml_text(doc, value);
		else
			m_content->xml_as<xml_text>()->move(value);
		return true;
	}
	else if (xml_is_type_of(xml_type::element))
	{
		if (m_content.is_empty() || m_content->xml_is_type_of(xml_type::element_list))
			m_content = NEW xml_text(doc, value);
		else if (m_content->xml_is_type_of(xml_type::cdata))
			m_content->xml_as<xml_cdata>()->push_value(value);
		else if (m_content->xml_is_type_of(xml_type::text))
			m_content->xml_as<xml_text>()->move(value);
		else 
			return false;
		return true;
	}
	return false;
}

bool xml_node::push_data(ixml_text_t value)
{
	if (m_type != xml_type::element)
		return false;
	xml_document_t doc = xml_parent_doc();
	xml_node_t child = NEW xml_cdata(doc);
	child->push_value(value);
	return push_child(child);
}

xml_iterator_t xml_node::push_attribute(xml_node_t attribute)
{
	if (attribute.is_empty())
		return xml_iterator_t(null, null);

	if (attribute->xml_is_type_of(xml_type::name_space) && attribute->xml_has_name())
	{
		m_ns_map[attribute->xml_name()] = attribute.get();
	}

	if (m_attributes.is_empty() || !m_attributes->xml_is_type_of(xml_type::attribute_list))
		m_attributes = NEW xml_collection(this, xml_type::attribute_list);
	return m_attributes->xml_as<xml_collection>()->push(attribute.get())
		? xml_iterator_t(static_cast<ixml_collection*>(m_attributes.get()), (xml_node*)attribute.get())
		: xml_iterator_t(null, null);
}

xml_iterator_t xml_node::push_attribute(xml_node_t attribute, xml_iterator_t next)
{
	if (attribute.is_empty())
		return xml_iterator_t(null, null);

	if (attribute->xml_is_type_of(xml_type::name_space) && attribute->xml_has_name())
	{
		m_ns_map[attribute->xml_name()] = attribute.get();
	}

	if (m_attributes.is_empty() || !m_attributes->xml_is_type_of(xml_type::attribute_list))
		m_attributes = NEW xml_collection(this, xml_type::attribute_list);
	return m_attributes->xml_as<xml_collection>()->insert(attribute.get(), next, true)
		? xml_iterator_t(static_cast<ixml_collection*>(m_attributes.get()), (xml_node*)attribute.get())
		: xml_iterator_t(null, null);
}

bool xml_node::push_attributes(ixml_collection_t attributes)
{
	if (attributes.is_empty() || !attributes->xml_is_type_of(xml_type::attribute_list))
		return false;
	if (m_attributes.is_empty() || !m_attributes->xml_is_type_of(xml_type::attribute_list))
		m_attributes = NEW xml_collection(this, xml_type::element_list);
	auto collection = m_attributes->xml_as<xml_collection>();
	auto doc = xml_parent_doc();
	for (auto it = attributes->begin(); it != attributes->end(); it++)
		collection->push(it->xml_clone(doc));
	return true;
}


xml_iterator_t xml_node::push_child(xml_node_t element)
{
	if (element.is_empty() || 
		(!element->xml_is_type_of(xml_type::element) &&
		!element->xml_is_type_of(xml_type::comment) &&
		!element->xml_is_type_of(xml_type::cdata)))
		return xml_iterator_t(null, null);

	if (m_content.is_empty() || !m_content->xml_is_type_of(xml_type::element_list))
		m_content = NEW xml_collection(this, xml_type::element_list);
	return m_content->xml_as<xml_collection>()->push(element.get())
		? xml_iterator_t(static_cast<ixml_collection*>(m_content.get()), (ixml_node*)element.get())
		: xml_iterator_t(null, null);
}

xml_iterator_t xml_node::push_child(xml_node_t element, xml_iterator_t next)
{
	if (element.is_empty() || 
		(!element->xml_is_type_of(xml_type::element) &&
		!element->xml_is_type_of(xml_type::comment) &&
		!element->xml_is_type_of(xml_type::cdata)))
		return xml_iterator_t(null, null);

	if (m_content.is_empty() || !m_content->xml_is_type_of(xml_type::element_list))
		m_content = NEW xml_collection(this, xml_type::element_list);
	return m_content->xml_as<xml_collection>()->insert(element.get(), next, true)
		? xml_iterator_t(static_cast<ixml_collection*>(m_content.get()), (ixml_node*)element.get())
		: xml_iterator_t(null, null);
}

bool xml_node::push_children(ixml_collection_t children)
{
	if (children.is_empty() || !children->xml_is_type_of(xml_type::element_list))
		return false;
	if (m_content.is_empty() || !m_content->xml_is_type_of(xml_type::element_list))
		m_content = NEW xml_collection(this, xml_type::element_list);
	auto store = m_content->xml_as<xml_collection>();
	auto doc = xml_parent_doc();
	for (auto it = children->begin(); it != children->end(); it++)
		store->push(it->xml_clone(doc));
	return true;
}
