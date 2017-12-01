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
#include "xml_entity_values.hpp"

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


using namespace ang;
using namespace ang::xml;


ang::intf_wrapper<ixml_document>::intf_wrapper() : _ptr(null) {

}

ang::intf_wrapper<ixml_document>::intf_wrapper(ixml_document* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<ixml_document>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ixml_document * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<ixml_document>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::intf_wrapper<ixml_document>::intf_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}


ang::intf_wrapper<ixml_document>::~intf_wrapper() {
	clean();
}

void ang::intf_wrapper<ixml_document>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::intf_wrapper<ixml_document>::is_empty()const
{
	return _ptr == null;
}

ixml_document* ang::intf_wrapper<ixml_document>::get(void)const
{
	return _ptr;
}

void ang::intf_wrapper<ixml_document>::set(ixml_document* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::intf_wrapper<ixml_document>& ang::intf_wrapper<ixml_document>::operator = (ixml_document* ptr)
{
	set(ptr);
	return*this;
}

ang::intf_wrapper<ixml_document>& ang::intf_wrapper<ixml_document>::operator = (ang::intf_wrapper<ixml_document> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::intf_wrapper<ixml_document>& ang::intf_wrapper<ixml_document>::operator = (ang::intf_wrapper<ixml_document> const& other)
{
	set(other._ptr);
	return*this;
}

ixml_document ** ang::intf_wrapper<ixml_document>::addres_of(void)
{
	return &_ptr;
}

ang::intf_wrapper_ptr<ixml_document> ang::intf_wrapper<ixml_document>::operator & (void)
{
	return this;
}

ixml_document * ang::intf_wrapper<ixml_document>::operator -> (void)
{
	return get();
}

ixml_document const* ang::intf_wrapper<ixml_document>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<ixml_document>::operator xml::ixml_document * (void) { return _ptr; }

ang::intf_wrapper<ixml_document>::operator xml::ixml_document const* (void)const { return _ptr; }


/////////////////////////////////////////////////////////////////////////////////////

xml_document::xml_document_iterator::xml_document_iterator()
	: _xml_doc(null)
	, _xml_current(null)
	, _xml_child(null)
{

}

xml_document::xml_document_iterator::xml_document_iterator(xml_document* store)
	: _xml_doc(store)
	, _xml_current(null)
	, _xml_child(null)
{
	xml_iterator_t it;
	if (store && store->begin().is_valid())
		xml_current(*store->begin());
}

xml_document::xml_document_iterator::xml_document_iterator(const xml_document_iterator& it)
	: _xml_doc(it._xml_doc)
	, _xml_current(null)
	, _xml_child(null)
{
	xml_current(it.xml_current());
	xml_child(it.xml_child());
}

xml_document::xml_document_iterator::~xml_document_iterator()
{
	_xml_child = null;
	_xml_current = null;
	_xml_doc = null;
}

ixml_node_t xml_document::xml_document_iterator::xml_current()const
{
	return _xml_current;
}

ixml_node_t xml_document::xml_document_iterator::xml_child()const
{
	return _xml_child;
}

void xml_document::xml_document_iterator::xml_current(ixml_node_t xmlNode)
{
	_xml_current = xmlNode;
}

void xml_document::xml_document_iterator::xml_child(ixml_node_t xmlNode)
{
	_xml_child = xmlNode;
}

void xml_document::xml_document_iterator::xml_parent(xml_document_t store)
{
	_xml_doc = store;
}

ixml_document_t xml_document::xml_document_iterator::xml_parent()const
{
	return _xml_doc.lock();
}

xml_iterator_t xml_document::xml_document_iterator::current()const
{
	return xml_iterator_t(xml_parent().get(), _xml_current.get());
}

xml_iterator_t xml_document::xml_document_iterator::child()const
{
	return xml_iterator_t(xml_parent().get(), _xml_child.get());
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
		xml_current(_xml_current->xml_next_sibling());
		return true;
	}
	return false;
}

bool xml_document::xml_document_iterator::prev()
{
	if (xml_current() && xml_current()->xml_prev_sibling())
	{
		xml_current(_xml_current->xml_prev_sibling());
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

xml_document::xml_document()
	: _count(0)
	, _xml_first(null)
	, _xml_root(null)
	, _xml_last(null)
	, _current(this)
{

}

xml_document::xml_document(xml_document const* doc)
	: _count(0)
	, _xml_first(null)
	, _xml_root(null)
	, _xml_last(null)
	, _current(this)
{
	if (doc)
	{
		ixml_node_t node = doc->xml_first();
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
}


xml_document::~xml_document()
{
	clean();
}

ANG_IMPLEMENT_CLASSNAME(ang::xml::xml_document);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::xml_document);

bool xml_document::is_inherited_of(type_name_t name)
{
	return name == type_of<xml_document>()
		|| object::is_inherited_of(name)
		|| ixml_document::is_inherited_of(name);
}

bool xml_document::is_kind_of(type_name_t name)const
{
	return name == type_of<xml_document>()
		|| object::is_kind_of(name)
		|| ixml_document::is_kind_of(name);
}

bool xml_document::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<xml_document>())
	{
		*out = static_cast<xml_document*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (ixml_document::query_object(name, out))
	{
		return true;
	}
	return false;
}

xml_type_t xml_document::xml_type()const
{
	return xml_type::document;
}

bool xml_document::xml_is_type_of(xml_type_t type)const
{
	return xml_type::document == type;
}

void xml_document::clean()
{
	_current.xml_child(null);
	_current.xml_current(null);

	ixml_node_t node = xml_first();
	ixml_node_t temp;
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
	_last_parsing_error = null;
}

wstring& xml_document::xml_print(wstring& out, const xml_format_t& flags, ushort level)const
{
	ixml_node_t node = xml_first();
	while (!node.is_empty())
	{
		node->xml_print(out, flags, level);
		node = node->xml_next_sibling();
		if (flags.is_active(xml_format_t::wrap_text_space) || flags.is_active(xml_format_t::wrap_text_tab))
			out << L"\n"_s;
	}
	return out;
}


wsize xml_document::counter()const
{
	return _count;
}

ixml_node_t xml_document::at(const xml_base_iterator_t& it)
{
	//if (!counter()) throw(ang::exception_t(except_code::invalid_memory));
	if (it.parent() != this)throw(ang::exception_t(except_code::invalid_param));
	return reinterpret_cast<ixml_node*>(it.current());
}

bool xml_document::increase(xml_base_iterator_t& it)const
{
	if (it.parent() != this || !it.is_valid()) throw(ang::exception_t(except_code::invalid_param));
	ixml_node * other,* node = reinterpret_cast<ixml_node*>(it.current());
	
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
	ixml_node * other, *node = reinterpret_cast<ixml_node*>(it.current());
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
	ixml_node * other, *node = reinterpret_cast<ixml_node*>(it.current());

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
	ixml_node * other, *node = reinterpret_cast<ixml_node*>(it.current());

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
	return xml_iterator_t(const_cast<xml_document*>(this), (ixml_node*)xml_first());
}

xml_forward_iterator_t xml_document::end()
{
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

xml_const_forward_iterator_t xml_document::begin()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), (ixml_node*)xml_first());
}

xml_const_forward_iterator_t xml_document::end()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

xml_backward_iterator_t xml_document::rbegin()
{
	ixml_node* child, *node = xml_last();
	while (node && (child = node->xml_last_child())) node = child;
	return xml_iterator_t(const_cast<xml_document*>(this), (ixml_node*)node);
}

xml_backward_iterator_t xml_document::rend()
{
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

xml_const_backward_iterator_t xml_document::rbegin()const
{
	ixml_node* child, *node = xml_last();
	while (node && (child = node->xml_last_child())) node = child;
	return xml_iterator_t(const_cast<xml_document*>(this), (ixml_node*)node);
}

xml_const_backward_iterator_t xml_document::rend()const
{
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

/////////////////////////////////////////////////////////////////////////

xml_header_t xml_document::xml_header()const
{
	return _xml_first ? _xml_first->xml_as<xml::xml_header>() : null;
}

ixml_node_t xml_document::xml_data_type()const
{
	return null;
}

xml_element_t xml_document::xml_root_element()const
{
	return _xml_root ? _xml_root->xml_as<xml::xml_element>() : null;
}

ixml_document_t xml_document::xml_clone()const
{
	return NEW xml_document(this);
}


xml_iterator_t xml_document::find(raw_str_t name, bool invert)const
{
	if (invert)
	{
		for (xml_const_backward_iterator_t it = rbegin(); it != rend(); it++)
		{
			if(text::UNICODE().compare((cwstr_t)it->xml_name(), name) == 0)
				return it;
		}
	}
	else
	{
		for (xml_const_forward_iterator_t it = begin(); it != end(); it++)
		{
			if (text::UNICODE().compare((cwstr_t)it->xml_name(), name) == 0)
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

xml_iterator_t xml_document::find(raw_str_t name, xml_iterator_t nextTo, bool invert)const
{
	if (!nextTo.is_valid() || nextTo.parent() != this)
		nextTo = (invert) ? xml_iterator_t(rbegin()) : xml_iterator_t(begin());

	if (invert)
	{
		for (xml_const_backward_iterator_t it = nextTo; it != rend(); it--)
		{
			if (text::UNICODE().compare((cwstr_t)it->xml_name(), name) == 0)
				return it;
		}
	}
	else
	{
		for (xml_const_forward_iterator_t it = nextTo; it != end(); it++)
		{
			if (text::UNICODE().compare((cwstr_t)it->xml_name(), name) == 0)
				return it;
		}
	}
	return xml_iterator_t(const_cast<xml_document*>(this), null);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ixml_node* xml_document::xml_first()const { return _xml_first; }
ixml_node* xml_document::xml_root()const { return _xml_root; }
ixml_node* xml_document::xml_last()const { return _xml_last; }

//void xml_document::insert_next_to(ixml_node*, ixml_node*);
//void xml_document::insert_prev_to(ixml_node*, ixml_node*);
//void xml_document::insert_in_head(ixml_node*);
//void xml_document::insert_in_tail(ixml_node*);


void xml_document::xml_first(ixml_node_t node)
{
	if (node.is_empty() || (node->xml_is_type_of(xml_type::header)
		|| node->xml_is_type_of(xml_type::element)
		|| node->xml_is_type_of(xml_type::comment)))
	{
		if (_xml_first == node.get()) return;
		auto _old = _xml_first;
		_xml_first = (!node.is_empty()) ? node->xml_as<xml_node>().get() : null;
		if (_xml_first)_xml_first->add_ref();
		if (_old)_old->release();
	}
}

void xml_document::xml_root(ixml_node_t node)
{
	if (node.is_empty() || node->xml_is_type_of(xml_type::element))
	{
		if (_xml_root == node.get()) return;
		auto _old = _xml_root;
		_xml_root = (!node.is_empty()) ? node->xml_as<xml_node>().get() : null;
		if (_xml_root)_xml_root->add_ref();
		if (_old)_old->release();
	}
}

void xml_document::xml_last(ixml_node_t node)
{
	if (node.is_empty() || (node->xml_is_type_of(xml_type::header)
		|| node->xml_is_type_of(xml_type::element)
		|| node->xml_is_type_of(xml_type::comment)))
	{
		if (_xml_last == node.get()) return;
		auto _old = _xml_last;
		_xml_last = (!node.is_empty()) ? node->xml_as<xml_node>().get() : null;
		if (_xml_last)_xml_last->add_ref();
		if (_old)_old->release();
	}
}

void xml_document::insert_next_to(ixml_node* refNode, ixml_node* newNode)
{
	refNode->xml_next_sibling()->xml_prev_sibling(newNode);
	newNode->xml_next_sibling(refNode->xml_next_sibling());
	newNode->xml_prev_sibling(refNode);
	refNode->xml_next_sibling(newNode);
}

void xml_document::insert_prev_to(ixml_node* refNode, ixml_node* newNode)
{
	refNode->xml_prev_sibling()->xml_next_sibling(newNode);
	newNode->xml_prev_sibling(refNode->xml_prev_sibling());
	newNode->xml_next_sibling(refNode);
	refNode->xml_prev_sibling(newNode);
}

void xml_document::insert_first(ixml_node* newNode)
{
	xml_first()->xml_prev_sibling(newNode);
	newNode->xml_next_sibling(xml_first());
	xml_first(newNode);
}

void xml_document::insert_last(ixml_node* newNode)
{
	xml_last()->xml_next_sibling(newNode);
	newNode->xml_prev_sibling(xml_last());
	xml_last(newNode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

xml_iterator_t xml_document::xml_current()const
{
	return _current.current();
}

xml_element_t xml_document::xml_current_element()const
{
	if (!_current.current().is_valid()) return null;
	return _current.current()->xml_as<xml_element>();
}

bool xml_document::move_to(xml_iterator_t current)
{
	if (!current.is_valid() || current.parent() != static_cast<ixml_items*>(this))
		return false;
	_current.xml_current(*current);
	_current.end_child();
	return true;
}

bool xml_document::move_to_child(xml_iterator_t child)
{
	if (!child.is_valid() || child.parent() != static_cast<ixml_items*>(this))
		return false;
	if (child->xml_parent() == null)//IS a root
		return false;
	_current.xml_current(child->xml_parent());
	_current.xml_child(*child);
	return true;
}


bool xml_document::move_up()
{
	return _current.prev_Child();
}

bool xml_document::move_down()
{
	return _current.next_child();
}

bool xml_document::move_forward()
{
	return _current.forward();
}

bool xml_document::move_backward()
{
	return _current.backward();
}

void xml_document::push_default_header()
{
	ixml_node_t node = NEW xml::xml_header(this);
	if (xml_first() == null)
	{
		xml_first(node.get());
		xml_last(node.get());
	}
	else
	{
		if (xml_first()->xml_is_type_of(xml_type::header))
		{
			xml_node_t old = xml_first()->xml_as<xml_node>();
			xml_first(node.get());
			if (old->xml_next_sibling())
			{
				old->xml_next_sibling()->xml_prev_sibling(node.get());
				((xml_node*)node.get())->xml_next_sibling(old->xml_next_sibling());
			}
			else
			{
				xml_last(node.get());
			}
		}
		else
		{
			ixml_node_t old = xml_first();
			xml_first(node.get());
			old->xml_prev_sibling(node.get());
			node->xml_next_sibling(old);
		}
	}
}

void xml_document::push_header(wstring version, xml_encoding_t encoding, bool standalone)
{
	ixml_node_t node = xml::xml_header::create_new(this, version, encoding, standalone);;
	if (xml_first() == null)
	{
		xml_first(node.get());
		xml_last(node.get());
	}
	else
	{
		if (xml_first()->xml_is_type_of(xml_type::header))
		{
			xml_node_t old = xml_first()->xml_as<xml_node>();;
			xml_first(node.get());
			if (old->xml_next_sibling())
			{
				old->xml_next_sibling()->xml_prev_sibling(node.get());
				((xml_node*)node.get())->xml_next_sibling(old->xml_next_sibling());
			}
			else
			{
				xml_last(node.get());
			}
		}
		else
		{
			ixml_node_t old = xml_first();
			xml_first(node.get());
			old->xml_prev_sibling(node.get());
			node->xml_next_sibling(old);
		}
	}
}

bool xml_document::begin_element(wstring name)
{
	if (xml_last() == null)
	{
		xml_element_t element = xml_element::create_new(this, name);
		xml_first(element.get());
		xml_last(element.get());
		xml_root(element.get());
		_current.xml_current(element.get());
	}
	else if (xml_root() == null)
	{
		xml_root(xml_element::create_new(this, name));
		_current.xml_current(xml_root());
		xml_last()->xml_next_sibling(xml_root());
		xml_root()->xml_prev_sibling(xml_last());
		xml_last(xml_root());
	}
	else if (_current.xml_current().get() == null)
	{
		return false;
	}
	else
	{
		xml_node_t current = _current.xml_current()->xml_as<xml_node>();
		if (current.get() == null || !current->xml_is_type_of(xml_type::element))
			return false;

		current->push_child(xml_element::create_new(this, name), xml_iterator_t(current->xml_children(), _current.xml_child().get()));
		_current.next_child();
		_current.forward();
	}
	return true;
}

bool xml_document::end_element()
{
	if (_current.xml_current() == null)
		return false;
	_current.backward();
	return true;
}


bool xml_document::push_element(wstring name)
{
	if (xml_last() == null)
	{
		xml_root(xml_element::create_new(this, ang::move(name)));
		xml_first(xml_root());
		xml_last(xml_first());
	}
	else if (xml_root() == null)
	{
		xml_root(xml_element::create_new(this, ang::move(name)));

		xml_last()->xml_next_sibling(xml_root());
		xml_root()->xml_prev_sibling(xml_last());
		xml_last(xml_root());
	}
	else
	{
		if (_current.xml_current().get() == null)
		{
			return false;
		}
		else
		{
			xml_node_t current = _current.xml_current()->xml_as<xml_node>();
			if (current.get() == null || !current->xml_is_type_of(xml_type::element))
				return false;
			current->push_child(xml_element::create_new(this, name), xml_iterator_t(current->xml_children(), _current.xml_child().get()));
			_current.next_child();
		}
	}
	return true;
}

bool xml_document::push_element(wstring name, wstring value)
{
	if (xml_last() == null)
	{
		xml_root(xml_element::create_new(this, ang::move(name), ang::move(value)));
		xml_first(xml_root());
		xml_last(xml_first());
	}
	else if (xml_root() == null)
	{
		xml_root(xml_element::create_new(this, ang::move(name), ang::move(value)));

		xml_last()->xml_next_sibling(xml_root());
		xml_root()->xml_prev_sibling(xml_last());
		xml_last(xml_root());
	}
	else
	{
		if (_current.xml_current().get() == null)
		{
			return false;
		}
		else
		{
			xml_node_t current = _current.xml_current()->xml_as<xml_node>();
			if (current.get() == null || !current->xml_is_type_of(xml_type::element))
				return false;
			current->push_child(xml_element::create_new(this, ang::move(name), ang::move(value)), xml_iterator_t(current->xml_children(), _current.xml_child().get()));
			_current.next_child();
		}
	}
	return true;
}


bool xml_document::push_comment(wstring value)
{
	if (xml_last() == null)
	{
		xml_first(xml_comment::create_new(this, ang::move(value)));
		xml_last(xml_first());
	}
	else
	{
		if (_current.xml_current().get() == null)
		{
			xml_last()->xml_next_sibling(xml_comment::create_new(this, ang::move(value)));
			xml_last()->xml_next_sibling()->xml_prev_sibling(xml_last());
			xml_last(xml_last()->xml_next_sibling());
		}
		else
		{
			xml_node_t current = _current.xml_current()->xml_as<xml_node>();
			if (!current->xml_is_type_of(xml_type::element))
				return false;
			current->push_child(xml_comment::create_new(this, ang::move(value)), xml_iterator_t(current->xml_children(), _current.xml_child().get()));
			_current.next_child();
		}
	}
	return true;
}


bool xml_document::push_data(wstring value)
{
	if (_current.xml_current().get() == null)
		return false;
	xml_node_t current = _current.xml_current()->xml_as<xml_node>();
	if (!current->xml_is_type_of(xml_type::element))
		return false;
	current->push_data(ang::move(value));
	return true;
}

bool xml_document::push_value(wstring value)
{
	if (_current.xml_current().get() == null)
		return false;
	xml_node_t current = _current.xml_current()->xml_as<xml_node>();
	if (!current->xml_is_type_of(xml_type::element))
		return false;
	current->push_value(ang::move(value));
	return true;
}

bool xml_document::push_attribute(wstring name, wstring value)
{
	if (_current.xml_current() == null)
		return false;
	xml_node_t current = _current.xml_current()->xml_as<xml_node>();
	if (!current->xml_is_type_of(xml_type::element))
		return false;
	current->push_attribute(xml_attribute::create_new(this, ang::move(name), ang::move(value)));
	return true;
}



////////////////////////////////////////////////////////////////////////////

static xml_encoding_t  xml_detect_encoding(raw_str_t text, windex& idx)
{
	if (text.size() < 4) return xml_encoding::none;

	alignas(4) static byte utf8_bom[4] = { 0xef, 0xbb, 0xbf, 0x0 };
	alignas(4) static byte utf16_le_bom[4] = { 0xff, 0xfe, 0x0, 0x0 };
	alignas(4) static byte utf16_be_bom[4] = { 0xfe, 0xff, 0x0, 0x0 };
	alignas(4) static byte utf32_le_bom[8] = { 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	alignas(4) static byte utf32_be_bom[8] = { 0x0, 0x0, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0 };

	switch (text.encoding())
	{
	case text::encoding::auto_detect:
		if (text::UTF8().compare_until((utf8_char_t const*)text.ptr(), (utf8_char_t const*)utf8_bom) == 3)
		{
			idx += 3; 
			return text::encoding::utf8;
		}
		else if (text::UTF32().compare_until((utf32_char_t const*)text.ptr(), (utf32_char_t const*)utf32_le_bom) == 1)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf32_le>();
		}
		else if (text::UTF32().compare_until((utf32_char_t const*)text.ptr(), (utf32_char_t const*)utf32_be_bom) == 1)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_be>();
		}
		else if (text::UTF16().compare_until((utf16_char_t const*)text.ptr(), (utf16_char_t const*)utf16_le_bom) == 1)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_le>();
		}
		else if (text::UTF16().compare_until((utf16_char_t const*)text.ptr(), (utf16_char_t const*)utf16_be_bom) == 1)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_be>();
		}

		else if (text::UTF8().compare_until((utf8_char_t const*)text.ptr(), u8"<?xml"_sm.cstr()) == 5)
			return text::encoding::utf8;
		else if (text::UTF32().compare_until((utf32_char_t const*)text.ptr(), U"<?xml") == 5)
			return text::encoding::utf32;
		else if (text::UTF32_SE().compare_until((utf32_char_t const*)text.ptr(), U"<?xml") == 5)
			return text::encoding::utf32_se;
		else if (text::UTF16().compare_until((utf16_char_t const*)text.ptr(), u"<?xml") == 5)
			return text::encoding::utf16;
		else if (text::UTF16_SE().compare_until((utf16_char_t const*)text.ptr(), u"<?xml") == 5)
			return text::encoding::utf16_se;	

		return text::encoding::utf8;

	case text::encoding::utf16_le: return text::native_encoding<text::encoding::utf16_le>();
	case text::encoding::utf16_be: return text::native_encoding<text::encoding::utf16_be>();
	case text::encoding::utf32_le: return text::native_encoding<text::encoding::utf32_le>();
	case text::encoding::utf32_be: return text::native_encoding<text::encoding::utf32_be>();
	default:
		return text.encoding();
	}
}

void xml_document::parse(raw_str_t text)
{
	windex idx = 0;
	if (text.size() < 4)
		throw(exception_t((dword)xml_exception_code::parsing_error, "invalid input"));
	
	text._encoding = xml_detect_encoding(text, idx);

	wstring version;
	xml_encoding_t encoding;
	bool standalone;

	text::string_util code = text;

	if (decode_header(code,	idx, version, encoding, standalone))
	{
		text._encoding = encoding != xml_encoding::auto_detect ? encoding.get() : text._encoding;
		push_header(version, text._encoding, standalone);
	}
	
	if (!decode_elements(this, code, idx))
	{
		clean();
		string error = "";
		error->format("exception: ang::xml::xml_document: %s"_s, (char const*)(cstr_t)_last_parsing_error);
		throw(exception_t((dword)xml_exception_code::parsing_error, ang::move(error)));
	}
}


bool xml_document::decode_header(text::string_util& code, windex& idx, wstring& version, xml_encoding_t& encoding, bool& standalone)
{
	wstring value;
	xml::xml_text_t txt;
	wstring header = "";
	windex beg;
	windex end;

	beg = code.find("<?xml"_s, 0, 5);

	if (beg == invalid_index)
		return false;

	end = code.find("?>"_s, beg);
	if (end == invalid_index)
		return false;

	code.sub_string(header, beg, end + 2);

	idx = end + 2;

	beg = header->find("version"_s);
	if (beg != invalid_index)
	{
		beg = header->find("\""_s, beg);
		beg++;
		end = header->find(L"\""_s, beg);
		header->sub_string(version, beg, end);
	}

	beg = header->find("encoding"_s, beg);
	if (beg != invalid_index)
	{
		beg = header->find("\""_s, beg);
		beg++;
		end = header->find("\""_s, beg);
		header->sub_string(value, beg, end);
		encoding = xml_encoding_t::parse(value);
	}
	return true;
}


#if defined _DEBUG
bool xml_document::decode_elements(xml_document_t doc, text::string_util& code, windex& __begin)
#else
bool xml_document::decode_elements(xml_document_t doc, text::string_util& code, windex& beg)
#endif
{
#if defined _DEBUG
	xml_parsing_iterator beg(code.data(), __begin);
#endif

	windex end;
	uint c = 0;
	wstring name = null;// = { 0 };
	wstring value = null;

	beg = code.find("<"_s, beg);
	while (beg != invalid_index)
	{
		c = 0;
		//Check if the node is a comment
		auto find = code.find("<!--"_s, beg, beg + 5);
		if (find == beg)
		{
			end = code.find("-->"_s, beg);
			if (end != invalid_index)
			{
				//get comment value
				code.sub_string(value, beg + 4, end - 4);

				//insert comment
				doc->push_comment(value.get());
			}
			//Finding next node start position
			beg = code.find("<"_s, end);
			continue;
		}

		//checking if it is a node ending
		if (code[beg + 1] == U'/')
		{
			xml_element_t current = doc->xml_current_element();
			if (current == null)//unkown error
			{
				doc->_last_parsing_error = "unknown Error"_s;
				return false;
			}
			beg += 2;

			//get element name
			bool same = true;
			char32 nC = 0;
			cwstr_t temp = current->xml_name()->cstr();
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
				doc->_last_parsing_error = "";
				doc->_last_parsing_error->format("bad end of element <%ls> in position %u"_s, (wchar const*)(cwstr_t)current->xml_name(), (index)beg);
				return false;
			}//parsing error

			beg += c;

			find = code.find(">"_s, beg);
			if (find == invalid_index)
			{
				doc->_last_parsing_error = "";
				doc->_last_parsing_error->format("missing character > in position %u"_s, (index)beg + current->xml_name()->length());
				return false;
			}//parsing error
			beg = find;

			doc->end_element();
			beg = code.find("<"_s, beg);
			continue;
		}

		//get element name
		name.clean();
		c = xml_get_element_name(code, beg);
		code.sub_string(name, beg + 1, beg + c);

		doc->begin_element(name.get());
		beg += c;
		c = 0;

		//parse attributes
		while (code[beg] != U'>')
		{
			beg += xml_skip_space(code, beg);
			if (code[beg] == U'>')
			{
				break;
			}
			else if (code[beg] == U'/' && code[beg + 1] == U'>')
			{
				++beg;
				break;
			}

			name.clean();
			c = xml_get_attibute_name(code, beg);
			code.sub_string(name, beg, beg + c);
			beg += c;
			c = 0;
			beg = code.find("="_s, beg);
			if (beg == invalid_index)
			{
				doc->_last_parsing_error = "";
				doc->_last_parsing_error->format("unexpected end of file parsing element %ls"_s, (wchar const*)(cwstr_t)name);
				return false;
			}

			++beg;

			beg += xml_skip_space(code, beg);
			c = 0;

			if (code[beg] == U'"')
			{
				end = code.find("\""_s, beg + 1);
			}
			else if (code[beg] == U'\'')
			{
				end = code.find("\'"_s, beg + 1);
			}
			else//parsing error
			{
				doc->_last_parsing_error = "";
				doc->_last_parsing_error->format("unexpected character in position %u"_s, (index)beg);
				return false;
			}
			if (end == invalid_index)//parsing error
			{
				doc->_last_parsing_error = "";
				doc->_last_parsing_error->format("missing character \' or \" in position %u"_s, (index)beg);
				return false;
			}

			value.clean();
			code.sub_string(value, beg + 1, end);
			doc->push_attribute(name.get(), value.get());
			beg = end + 1;

		}

		//if is not an empty node
		if (code[beg - 1] == U'/')
		{
			doc->end_element();
			beg = code.find("<"_s, beg);
			continue;
		}
		else
		{
			++beg;

			c = xml_skip_space(code, beg);

			if (code[beg + c] == '<')
			{
				//checking if it is a node ending
				if (code[beg + c + 1] == '\'')
				{
					value.clean();
					code.sub_string(value, beg, beg + c);
					doc->push_value(value.get());
					beg += c;

					xml_element* current = doc->xml_current_element();
					if (current == null)//unkown error
					{
						doc->_last_parsing_error = "unknown Error"_s;
						return false;
					}
					beg += 2;

					//get element name
					bool same = true;
					wchar _c = 0;
					cwstr_t temp = current->xml_name();
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
						doc->_last_parsing_error = "";
						doc->_last_parsing_error->format("bad end of element <%ls> in position %u"_s, (wchar const*)(cwstr_t)current->xml_name(), (index)beg);
						return false;
					}//parsing error

					beg += c;

					find = code.find(">"_s, beg);
					if (find == invalid_index)
					{
						doc->_last_parsing_error = "";
						doc->_last_parsing_error->format("missing character > in position %u"_s, (index)beg + current->xml_name()->length());
						return false;
					}//parsing error
					beg = find;
					doc->end_element();
					beg = code.find("<"_s, beg);
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
				end = code.find("<"_s, beg + c);
				if (end == invalid_index)
				{
					doc->_last_parsing_error = "";
					doc->_last_parsing_error->format("missing character < in position %u"_s, (index)beg + c);
					return false;
				}//parsing error
				value.clean();
				code.sub_string(value, beg, end);
				doc->push_value(value.get());
				beg = end;
				continue;
			}
		}
	}

	return true;
}
