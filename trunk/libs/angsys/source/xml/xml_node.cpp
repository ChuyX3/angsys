#include "pch.h"
#include "xml/xml_impl.h"

using namespace ang;
using namespace ang::dom;
using namespace ang::dom::xml;

xml_node_base::xml_node_base(dom::xml::xml_document* doc, xml_type_t type)
	: m_type(type)
	, m_document(doc)
	, m_parent(null)
	, m_prev_sibling(null)
	, m_next_sibling(null)
{
	switch (type)
	{
	case xml_type::element_list:
		m_type = type;
		m_content = xml_list();
		break;
	case xml_type::attribute_list:
		m_content = xml_list();
		break;
	default:
		break;
	}
}

void xml_node_base::dispose()
{
	m_parent = null;
	m_content.reset();
	m_prev_sibling = null;
	m_next_sibling = null;
}

xml_type xml_node_base::type()const {
	return m_type;
}

xml_node_t xml_node_base::to_node()const
{
	return static_cast<xml_node*>(const_cast<xml_node_base*>(this));
}

void xml_node_base::parent(xml_node* node) 
{
	m_parent = node;
}

xml_node_t xml_node_base::parent()const
{
	return m_parent.lock();
}

void xml_node_base::document(xml_document* doc)
{
	m_document = doc;
}

xml_document_t xml_node_base::document()const
{
	return m_document.lock();
}

void xml_node_base::prev_sibling(xml_node* node)
{
	m_prev_sibling = node;
}

xml_node_t xml_node_base::prev_sibling()const
{
	return m_prev_sibling;
}

void xml_node_base::next_sibling(xml_node* node)
{
	m_next_sibling = node;
}

xml_node_t xml_node_base::next_sibling()const
{
	return m_next_sibling;
}

ixml_text_t xml_node_base::name()const
{
	return m_name;
}

void xml_node_base::name(ixml_text_t value)
{
	m_name = value;
}

ixml_text_t xml_node_base::value()const
{
	return m_content.current() == 0 ? m_content.get<0>() : null;
}

void xml_node_base::value(ixml_text_t value)
{
	m_content = value;
}

ixml_list_t xml_node_base::children()const
{
	return static_cast<xml_node*>(const_cast<xml_node_base*>(this))->to_intf<ixml_list>();
}

ixml_list_t xml_node_base::attributes()const
{
	return m_attribs.get();
}

void xml_node_base::attributes(ixml_list_t list)
{
	m_attribs = list;
}

xml_node_t xml_node_base::first_child()const
{
	return m_content.current() == 1 ? m_content.get<1>().front().get() : null;
}

xml_node_t xml_node_base::last_child()const
{
	return m_content.current() == 1 ? m_content.get<1>().back().get() : null;
}

///////////////////////////////////////////////////////////////////////////////

xml_node_t xml_node::create_element(xml_document* doc, cstr_t name)
{
	xml_node_t node = new xml_node(doc, xml_type::element);
	node->to_intf<xml_node_base>()->name(doc->create_pcdata(name));
	return node;
}

xml_node_t xml_node::create_element(xml_document* doc, cstr_t name, cstr_t value)
{
	xml_node_t node = new xml_node(doc, xml_type::element);
	node->to_intf<xml_node_base>()->name(doc->create_pcdata(name));
	node->to_intf<xml_node_base>()->value(doc->create_pcdata(value));
	return node;
}

xml_node_t xml_node::create_attribute(xml_document* doc, cstr_t name, cstr_t value)
{
	xml_node_t node = new xml_node(doc, xml_type::attribute);
	node->to_intf<xml_node_base>()->name(doc->create_pcdata(name));
	node->to_intf<xml_node_base>()->value(doc->create_pcdata(value));
	return node;
}

xml_node_t xml_node::create_comment(xml_document* doc, cstr_t value)
{
	xml_node_t node = new xml_node(doc, xml_type::comment);
	node->to_intf<xml_node_base>()->value(doc->create_pcdata(value));
	return node;
}

xml_node_t xml_node::create_cdata(xml_document* doc, cstr_t value)
{
	xml_node_t node = new xml_node(doc, xml_type::cdata);
	node->to_intf<xml_node_base>()->value(doc->create_cdata(value));
	return node;
}

xml_node_t xml_node::create_attribute_list(xml_document* doc)
{
	xml_node_t node = new xml_node(doc, xml_type::attribute_list);
	return node;
}

xml_node::xml_node(xml_document* doc, xml_type_t type)
	: base(doc, type)
{

}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::dom::xml::xml_node);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_node);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::dom::xml::xml_node);

void xml_node::dispose()
{
	xml_node_base::dispose();
}

xml_type_t xml_node::type()const
{
	return m_type;
}

bool xml_node::is_type_of(xml_type_t type)const
{
	return m_type == type;
}

streams::itext_output_stream_t& xml_node::print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level)const
{
	return stream;
}

ixml_node_t xml_node::parent()const
{
	return interface_cast<ixml_node>(xml_node_base::parent());
}

ixml_document_t xml_node::document()const
{
	return interface_cast<ixml_document>(xml_node_base::document());
}

ixml_node_t xml_node::clone(ixml_document_t)const
{
	//TODO
	return null;
}

bool xml_node::has_name()const
{
	return !m_name.is_empty();
}

bool xml_node::has_value()const
{
	return m_content.current() == 0;
}

bool xml_node::has_namespace()const
{
	return false;
}

bool xml_node::has_children()const
{
	return m_content.current() == 1;
}

bool xml_node::has_attributes()const
{
	return !m_attribs.is_empty();
}

ixml_text_t xml_node::name()const
{
	return m_name.get();
}

ixml_text_t xml_node::value()const
{
	return has_value() ? m_content.get<0>() : null;
}

ixml_text_t xml_node::xmlns()const
{
	return null;
}

ixml_node_t xml_node::xmlns(cstr_t)const
{
	return null;
}

ixml_items_t xml_node::children()const
{
	return to_intf<ixml_items>();
}

ixml_attbs_t xml_node::attributes()const
{
	return m_attribs.get();
}

/////////////////////////////////////////////////////////

wsize xml_node::counter()const
{
	return has_children() ? m_content.get<1>().size() : 0;
}

ixml_node_t xml_node::at(xml_base_iterator_t const& it)
{
	if (!has_children() || it.parent() != this || it.current() == null)
		throw_exception(error_code::invalid_param);
	return interface_cast<ixml_node>((xml_node*)it.current());
}

bool xml_node::increase(xml_base_iterator_t& it)const
{
	if (!has_children() || it.parent() != this || it.current() == null)
		return false;
	xml_node* node = static_cast<xml_node*>(it.current());
	it.current(node->next_sibling().get());
	return true;
}

bool xml_node::increase(xml_base_iterator_t& it, int offset)const 
{
	if (!has_children() || it.parent() != this || it.current() == null)
		return false;
	xml_node* node = static_cast<xml_node*>(it.current());
	while (node != null && offset > 0) {
		node = node->next_sibling();
		offset--;
	}
	it.current(node);
	return true;
}

bool xml_node::decrease(xml_base_iterator_t& it)const
{
	if (!has_children() || it.parent() != this || it.current() == null)
		return false;
	xml_node* node = static_cast<xml_node*>(it.current());
	it.current(node->prev_sibling().get());
	return true;
}
bool xml_node::decrease(xml_base_iterator_t& it, int offset)const
{
	if (!has_children() || it.parent() != this || it.current() == null)
		return false;
	xml_node* node = static_cast<xml_node*>(it.current());
	while (node != null && offset > 0) {
		node = node->prev_sibling();
		offset--;
	}
	it.current(node);
	return true;
}

xml_forward_iterator_t xml_node::begin() {
	
	return xml_base_iterator_t(const_cast<xml_node*>(this), has_children() ? (m_content.get<1>().front()).get() : null);
}

xml_forward_iterator_t xml_node::end()
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), null);
}

xml_const_forward_iterator_t xml_node::begin()const
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), has_children() ? (m_content.get<1>().front()).get() : null);
}

xml_const_forward_iterator_t xml_node::end()const
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), null);
}

xml_forward_iterator_t xml_node::last()
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), has_children() ? (m_content.get<1>().back()).get() : null);
}

xml_const_forward_iterator_t xml_node::last()const
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), has_children() ? (m_content.get<1>().back()).get() : null);
}

xml_backward_iterator_t xml_node::rbegin()
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), has_children() ? (m_content.get<1>().back()).get() : null);
}

xml_backward_iterator_t xml_node::rend()
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), null);
}

xml_const_backward_iterator_t xml_node::rbegin()const
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), has_children() ? (m_content.get<1>().back()).get() : null);
}

xml_const_backward_iterator_t xml_node::rend()const
{
	return xml_base_iterator_t(const_cast<xml_node*>(this), null);
}

xml_iterator_t xml_node::find(cstr_t name, bool invert)const
{
	if (!has_children())
		return end();
	if (invert) {
		for (auto it = rbegin(), e = rend(); it != e; it++) {
			ixml_node_t node = (*it).get();
			if (name == node->name())
				return xml_iterator_t(const_cast<xml_node*>(this), static_cast<xml_node*>(node.get()));
		}
	}
	else {
		for (auto it = begin(), e = end(); it != e; it++) {
			ixml_node_t node = (*it).get();
			if (name == node->name())
				return xml_iterator_t(const_cast<xml_node*>(this), static_cast<xml_node*>(node.get()));
		}
	}
	return end();
}

xml_iterator_t xml_node::find(cstr_t name, xml_base_iterator_t next_to, bool invert)const
{
	if (!has_children())
		return end();
	if (invert) {
		if (next_to.parent() != this)
			next_to = rbegin();
		for (auto it = rbegin(), e = rend(); it != e; it++) {
			ixml_node_t node = (*it).get();
			if (name == node->name())
				return xml_iterator_t(const_cast<xml_node*>(this), static_cast<xml_node*>(node.get()));
		}
	}
	else {
		if (next_to.parent() != this)
			next_to = begin();
		for (auto it = begin(), e = end(); it != e; it++) {
			ixml_node_t node = (*it).get();
			if (name == node->name())
				return xml_iterator_t(const_cast<xml_node*>(this), static_cast<xml_node*>(node.get()));
		}
	}
	return end();
}

ixml_enum_t xml_node::find_all(cstr_t name)const
{
	if (!has_children())
		return null;
	smart_vector<ixml_node> nodes;
	for (auto it = begin(), e = end(); it != e; it++) {
		ixml_node_t node = (*it).get();
		if (name == node->name())
			nodes += ang::move(node);
	}
	return nodes.get();
}

xml_node_t xml_node::front()const
{
	if (!has_children())
		return null;
	return m_content.get<1>().front();
}

xml_node_t xml_node::back()const
{
	if (!has_children())
		return null;
	return m_content.get<1>().back();
}

void xml_node::push_front(xml_node_t node)
{
	if (node.is_empty())
		return;
	if (!has_children())
		m_content = xml_list();
	m_content.get<1>().push_front(node);
	node->to_intf<xml_node_base>()->parent(this);
}

void xml_node::push_back(xml_node_t node)
{
	if (node.is_empty())
		return;
	if (!has_children())
		m_content = xml_list();
	m_content.get<1>().push_back(node);
	node->to_intf<xml_node_base>()->parent(this);
}

xml_iterator_t xml_node::insert(xml_base_iterator_t at, xml_node_t node)
{
	if (node.is_empty())
		return end();

	if (!has_children())
		m_content = xml_list();
	xml_list::iterator_t it = xml_list::iterator_t(xml_list::iteration_type(), (xml_node*)at.current(), 0);
	it = m_content.get<1>().insert(it, node);
	node->to_intf<xml_node_base>()->parent(this);
	return it.is_valid() ? xml_forward_iterator_t(this, it.get_node(), 0) : end();
}

bool xml_node::pop_front(xml_node_ptr_t out)
{
	if (!has_children())
		return false;
	if (!m_content.get<1>().pop_front(out))
		return false;
	return true;
}

bool xml_node::pop_back(xml_node_ptr_t out)
{
	if (!has_children())
		return false;
	if (!m_content.get<1>().pop_back(out))
		return false;
	return true;
}

xml_iterator_t xml_node::remove(xml_base_iterator_t at, xml_node_ptr_t out)
{
	if (!has_children() || at.parent() != this || at.current()  == null)
		return false;
	xml_list::iterator_t it = xml_list::iterator_t(xml_list::iteration_type(), (xml_node*)at.current(), 0);
	if (!m_content.get<1>().remove(it, out))
		return end();
	return xml_iterator_t(this, (*it).get());
}
