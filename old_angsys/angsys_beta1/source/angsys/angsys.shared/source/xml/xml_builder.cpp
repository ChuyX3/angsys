/*********************************************************************************************************************/
/*   File Name: xml_builder.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/xml.h"

#if defined _DEBUG
#define NEW ANG_DEBUG_NEW()
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif


using namespace ang;
using namespace ang::xml;
using namespace ang::collections;

xml_builder::xml_builder()
	: xml_tree(xml_type::builder)
{
	_current._xml_tree = (object*)this;
}

xml_builder::xml_builder(const xml_builder&)
	: xml_tree(xml_type::builder)
{
	_current._xml_tree = object_t(this);
}

xml_builder::xml_builder(const xml_builder*)
	: xml_tree(xml_type::builder)
{
	_current._xml_tree = object_t(this);
}

xml_builder::~xml_builder()
{
	clean();
}


ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_builder, xml_tree);


void xml_builder::clean()
{

	xml_tree::clean();
}



bool xml_builder::append(xml_node_t node, bool last)
{
	if (node == null || (!node->xml_is_type_of(xml_type::element) && !node->xml_is_type_of(xml_type::comment)))
		return false;

	if (xml_tail() == null)
	{
		xml_root(node);
		xml_head(node);
		xml_tail(node);
	}
	else
	{
		xml_node_t current = _current.xml_current();
		if (current.is_empty())
		{
			if (xml_root() && node->xml_is_type_of(xml_type::element))
				return false;
			current = last ? xml_tail() : xml_head();
		}

		if (current->xml_has_children())
		{
			xml_collection_t children = current->xml_children();
			children->append(node, last);
		}
		else if (current->xml_parent())
		{
			xml_collection_t children = current->xml_parent()->xml_children();
			if (children.is_empty()) //for debug
				throw(exception_t((dword)xml_exception_code::unexpected_error, "exception: unexpected error calling the function xml_builder::append"_s));
			children->insert(node, children->find(current), last);
		}
		else
		{
			if (xml_root() == null)
				xml_root(node);

			if (last)
			{
				if (xml_tail() == current)
					insert_in_tail(node);
				else
					insert_next_to(current, node);
			}
			else
			{
				if (_xml_head == current.get())
				{
					if (_xml_head->xml_is_type_of(xml_type::header))
					{
						if (_xml_head == _xml_tail)
							insert_in_tail(node);
						else
							insert_next_to(xml_head(), node);
					}
					else
						insert_in_head(node);
				}
				else
					insert_prev_to(current, node);
			}
		}
	}
	return true;
}

bool xml_builder::insert(xml_node_t node, xml_iterator_t at, bool nextTo)
{
	return false; if (!at.is_valid())
		return append(node, nextTo);

	if (node == null || (!node->xml_is_type_of(xml_type::element) && !node->xml_is_type_of(xml_type::comment)))
		return false;

	xml_node_t current = (xml_node*)at;

	if (current->xml_parent())
	{
		xml_collection_t children = current->xml_parent()->xml_children();
		if (children.is_empty())
			throw(exception_t(xml_exception_code::unexpected_error, "exception: unexpected error calling the function xml_builder::insert"_s));
		children->insert(node, xml_iterator_t(children, current.get()), nextTo);
	}
	else
	{
		if (!xml_root())
			xml_root(node);

		if (nextTo)
		{
			if (xml_tail() == current.get())
				insert_in_tail(node);
			else
				insert_next_to(current, node);
		}
		else
		{
			if (xml_head() == current.get())
			{
				if (xml_head()->xml_is_type_of(xml_type::header))
				{
					if (xml_head() == xml_tail())
						insert_in_tail(node);
					else
						insert_next_to(xml_head(), node);
				}
				else
					insert_in_head(node);
			}
			else
				insert_prev_to(current, node);
		}
	}
	return true;
}

bool xml_builder::pop(xml_node** out, bool last)
{
	return false;
}

bool xml_builder::pop_at(xml_iterator_t it, xml_node** out)
{
	return false;
}



xml_element_t xml_builder::xml_current()const
{
	if (!_current.current().is_valid())
		return null;
	return _current.current()->xml_as<xml_element>();
}

xml_iterator_t xml_builder::xml_current_pos()const
{
	return _current.current();
}

bool xml_builder::move_to(xml_iterator_t current)
{
	if (!current.is_valid() || current.parent() != static_cast<ixml_items*>(this))
		return false;
	_current.xml_current((xml_node*)current);
	_current.end_child();
	return true;
}

bool xml_builder::move_to_child(xml_iterator_t child)
{
	if (!child.is_valid() || child.parent() != static_cast<ixml_items*>(this))
		return false;
	if (child->xml_parent() == null)//IS a root
		return false;
	_current.xml_current(child->xml_parent());
	_current.xml_child((xml_node*)child);
	return true;
}

bool xml_builder::move_up()
{
	return _current.prev_Child();
}

bool xml_builder::move_down()
{
	return _current.next_child();
}

bool xml_builder::move_forward()
{
	return _current.forward();
}

bool xml_builder::move_backward()
{
	return _current.backward();
}

void xml_builder::push_header(xml_header_t obj)
{
	if (obj.is_empty())
		return;

	if (xml_head() == null)
	{
		xml_head(obj.get());
		xml_tail(obj.get());
	}
	else
	{
		if (xml_head()->xml_is_type_of(xml_type::header))
		{
			xml_node_t old = xml_head();
			xml_head(obj.get());
			if (old->xml_next())
			{
				old->xml_next()->xml_prev(obj.get());
				((xml_node*)obj.get())->xml_next(old->xml_next());
			}
			else
			{
				xml_tail(obj.get());
			}
		}
		else
		{
			xml_node_t old = xml_head();
			xml_head(obj.get());
			old->xml_prev(obj.get());
			((xml_node*)obj.get())->xml_next(old);
		}
	}
}

bool xml_builder::begin_element(wstring name)
{
	if (xml_tail() == null)
	{
		xml_element_t element = NEW xml_element(name, xml_collection_t());
		xml_head(element.get());
		xml_tail(element.get());
		xml_root(element.get());
		_current.xml_current(element.get());
	}
	else if (xml_root() == null)
	{
		xml_root(NEW xml_element(name, xml_collection_t()));
		_current.xml_current(xml_root());
		xml_tail()->xml_next(xml_root());
		xml_root()->xml_prev(xml_tail());
		xml_tail(xml_root());
	}
	else if (_current.xml_current().get() == null)
	{
		return false;
	}
	else
	{
		xml_node_t current = _current.xml_current();
		if (!current->xml_is_type_of(xml_type::element))
			return false;

		if (_current.xml_child().get() == null || !_current.end_child())
		{
			xml_collection_t children = current->xml_children();
			if (children == null)
			{
				children = NEW xml_element_list();
				current->xml_children(children);
			}
			children->append(NEW xml_element(name, xml_collection_t()));
			_current.end_child();
		}
		else
		{
			xml_collection_t children = current->xml_children();
			if (children->end().current() == _current.child().current())
			{
				children->append(NEW xml_element(name, xml_collection_t()));
				_current.end_child();
			}
			else
			{
				children->insert(NEW xml_element(name, xml_collection_t()), xml_iterator_t(children, _current.child().current()));
				_current.next_child();
			}
		}
		_current.forward();
	}
	return true;
}

bool xml_builder::end_element()
{
	if (_current.xml_current() == null)
		return false;
	_current.backward();
	return true;
}

bool xml_builder::element(wstring name, wstring value)
{
	if (xml_tail() == null)
	{
		xml_root(NEW xml_element(ang::move(name), ang::move(value)));
		xml_head(xml_root());
		xml_tail(xml_head());
	}
	else if (xml_root() == null)
	{
		xml_root(NEW xml_element(ang::move(name), ang::move(value)));

		xml_tail()->xml_next(xml_root());
		xml_root()->xml_prev(xml_tail());
		xml_tail(xml_root());
	}
	else
	{
		if (_current.xml_current().get() == null)
		{
			return false;
		}
		else
		{
			xml_node_t current = _current.xml_current();
			if (!current->xml_is_type_of(xml_type::element))
				return false;

			if (_current.xml_child().get() == null || !_current.end_child())
			{
				xml_collection_t children = current->xml_children();
				if (children.get() == null)
				{
					children = NEW xml_element_list();
					current->xml_children(children);
				}
				children->append(NEW xml_element(ang::move(name), ang::move(value)));
				_current.end_child();
			}
			else
			{
				xml_collection_t children = current->xml_children();
				if (children->end().current() == _current.child().current())
				{
					children->append(NEW xml_element(ang::move(name), ang::move(value)));
					_current.end_child();
				}
				else
				{
					children->insert(NEW xml_element(ang::move(name), ang::move(value)), _current.child());
					_current.next_child();
				}
			}
		}
	}
	return true;
}

bool xml_builder::element(xml_element_t element)
{
	if (xml_tail() == null)
	{
		xml_root(element.get());
		xml_head(xml_root());
		xml_tail(xml_head());
	}
	else if (xml_root() == null)
	{
		xml_root(element.get());

		xml_tail()->xml_next(xml_root());
		xml_root()->xml_prev(xml_tail());
		xml_tail(xml_root());
	}
	else
	{
		if (_current.xml_current().get() == null)
		{
			return false;
		}
		else
		{
			xml_node_t current = _current.xml_current();
			if (!current->xml_is_type_of(xml_type::element))
				return false;

			if (_current.xml_child().get() == null || !_current.end_child())
			{
				xml_collection_t children = current->xml_children();
				if (children.get() == null)
				{
					children = NEW xml_element_list();
					current->xml_children(children);
				}
				children->append(element.get());
				_current.end_child();
			}
			else
			{
				xml_collection_t children = current->xml_children();
				if (children->end().current() == _current.child().current())
				{
					children->append(element.get());
					_current.end_child();
				}
				else
				{
					children->insert(element.get(), _current.child());
					_current.next_child();
				}
			}
		}
	}
	return true;
}

bool xml_builder::value(wstring value)
{
	if (_current.xml_current().get() == null)
		return false;
	xml_node_t current = _current.xml_current();
	current->xml_value(ang::move(value));
	return true;
}

bool xml_builder::attribute(wstring name, wstring value)
{
	if (_current.xml_current() == null)
		return false;
	xml_node_t current = _current.xml_current();
	xml_collection_t attribs = current->xml_attributes().get();
	if (attribs.get() == null)
	{
		attribs = NEW xml_attribute_list();
		current->xml_attributes(attribs);
	}
	attribs->append(NEW xml_attribute(ang::move(name), ang::move(value)));
	return true;
}

bool xml_builder::comment(wstring value)
{
	if (xml_tail() == null)
	{
		xml_head(NEW xml_comment(ang::move(value)));
		xml_tail(xml_head());
	}
	else
	{
		if (_current.xml_current().get() == null)
		{
			xml_tail()->xml_next(new xml_comment(ang::move(value)));
			xml_tail()->xml_next()->xml_prev(xml_tail());
			xml_tail(xml_tail()->xml_next());
		}
		else
		{
			xml_node_t current = _current.xml_current();
			if (!current->xml_is_type_of(xml_type::element))
				return false;

			if (_current.xml_child().get() == null || !_current.end_child())
			{
				xml_collection_t children = current->xml_children();
				if (children.get() == null)
				{
					children = NEW xml_element_list();
					current->xml_children(children);
				}
				children->append(NEW xml_comment(ang::move(value)));
				_current.end_child();
			}
			else
			{
				xml_collection_t children = current->xml_children();
				if (children->end().current() == _current.child().current())
				{
					children->append(NEW xml_comment(ang::move(value)));
					_current.end_child();
				}
				else
				{
					children->insert(NEW xml_comment(ang::move(value)), _current.child());
					_current.next_child();
				}
			}
		}
	}
	return true;
}

