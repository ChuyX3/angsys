/*********************************************************************************************************************/
/*   File Name: xml_node.cpp                                                                                           */
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


////////////////////////////////////////////////////////////////////////

ang::object_wrapper<xml_node>::object_wrapper() 
	: _ptr(null)
{
}

ang::object_wrapper<xml_node>::object_wrapper(xml_node* ptr) 
	: _ptr(null)
{
	set(ptr);
}

ang::object_wrapper<xml_node>::object_wrapper(object_wrapper && other)
	: _ptr(null)
{
	xml_node * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_node>::object_wrapper(object_wrapper const& other)
	: _ptr(null)
{
	set(other._ptr);
}

ang::object_wrapper<xml_node>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_node>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<xml_node>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<xml_node>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<xml_node>::is_empty()const
{
	return _ptr == null;
}

xml_node* ang::object_wrapper<xml_node>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_node>::set(xml_node* ptr)
{
	xml_node * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<xml_node>& ang::object_wrapper<xml_node>::operator = (xml_node* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_node>& ang::object_wrapper<xml_node>::operator = (ang::object_wrapper<xml_node> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_node>& ang::object_wrapper<xml_node>::operator = (ang::object_wrapper<xml_node> const& other)
{
	set(other._ptr);
	return*this;
}

xml_node ** ang::object_wrapper<xml_node>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_node> ang::object_wrapper<xml_node>::operator& (void)
{
	return this;
}

xml_node * ang::object_wrapper<xml_node>::operator -> (void)
{
	return get();
}

xml_node const* ang::object_wrapper<xml_node>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_node>::operator xml_node * (void)
{
	return get();
}

ang::object_wrapper<xml_node>::operator xml_node const* (void)const
{
	return get();
}

ang::object_wrapper<xml_node>::operator xml::xml_value_t()const
{
	return _ptr ? _ptr->xml_value() : null;
}

xml_node_t ang::object_wrapper<xml_node>::operator[](cstr_t value)const
{
	xml_iterator_t it;
	if (_ptr)
	{
		if (_ptr->xml_has_attributes())
			it = _ptr->xml_attributes()->find_first(value);
		if (!it.is_valid() && _ptr->xml_has_children())
			it = _ptr->xml_children()->find_first(value);
	}
	return it.is_valid() ? (xml_node*)it : null;
}

xml_node_t ang::object_wrapper<xml_node>::operator[](cwstr_t value)const
{
	xml_iterator_t it;
	if (_ptr)
	{
		if (_ptr->xml_has_attributes())
			it = _ptr->xml_attributes()->find_first(value);
		if (!it.is_valid() && _ptr->xml_has_children())
			it = _ptr->xml_children()->find_first(value);
	}
	return it.is_valid() ? (xml_node*)it : null;
}

ANG_IMPLEMENT_WEAK_PTR(xml_node);

////////////////////////////////////////////////////////////////////////////////


xml_node::xml_node(xml_type_t type)
	: _xml_type(type)
	, _xml_parent()
	, _xml_prev(null)
	, _xml_next(null)
	, _xml_name(null)
	, _xml_attributes(null)
{
	_xml_content.data_type = xml_type_t::abstract;
	_xml_content.unknown = null;
}

xml_node::~xml_node()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::xml::xml_node);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::xml_node);

bool xml_node::is_child_of(type_name_t className)
{
	if (className == type_name<xml_node>()
		|| object::is_child_of(className)
		|| ixmlobject::is_child_of(className))
		return true;
	return false;
}

bool xml_node::is_kind_of(type_name_t className)const
{
	if (className == type_name<xml_node>()
		|| object::is_kind_of(className)
		|| ixmlobject::is_kind_of(className))
		return true;
	return false;
}

bool xml_node::query_object(type_name_t className, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == type_name<xml_node>())
	{
		*out = static_cast<xml_node*>(this);
		return true;
	}
	else if (object::query_object(className, out))
	{
		return true;
	}
	else if (ixmlobject::is_kind_of(className))
	{
		*out = static_cast<ixmlobject*>(this);
		return true;
	}
	return false;
}

void xml_node::clean()
{
	//_xml_parent;
	//_xml_prev;
	//_xml_next;
	_xml_name = null;
	_xml_attributes = null;
	_xml_content.data_type = xml_type::abstract;
	_xml_content.unknown = null;
}

template<> xml_header_t xml_node::xml_as<xml_header>() { return xml_is_type_of(xml_type::header) ? static_cast<xml_header*>(this) : null; }
template<> xml_comment_t xml_node::xml_as<xml_comment>() { return xml_is_type_of(xml_type::comment) ? static_cast<xml_comment*>(this) : null; }
template<> xml_element_t xml_node::xml_as<xml_element>() { return xml_is_type_of(xml_type::element) ? static_cast<xml_element*>(this) : null; }
template<> xml_attribute_t xml_node::xml_as<xml_attribute>() { return xml_is_type_of(xml_type::attribute) ? static_cast<xml_attribute*>(this) : null; }

xml_type_t xml_node::xml_type()const
{
	return _xml_type;
}

bool xml_node::xml_is_type_of(xml_type_t type)const
{
	return (_xml_type == type || xml_type_t::node == type);
}

wstring& xml_node::xml_print(wstring& stream, xml_format_t const& flag, ushort level)const
{
	switch (_xml_type)
	{
	case xml_type_t::header:
	{
		auto version = (xml_value_t)_xml_attributes["version"];
		auto encoding = (xml_value_t)_xml_attributes["encoding"];
		auto standalone = (xml_value_t)_xml_attributes["standalone"];
		stream << L"<?xml "_s << L"version=\""_s << version << L"\""_s;
		stream << L" encoding="_s << L"\""_s << (encoding.is_empty() ? L"utf-8"_s : encoding->cstr()) << L"\""_s;
		if (!standalone.is_empty())
			stream << L" standalone=\""_s << standalone << L"\""_s;
		stream << L"?>"_s;
	}
	break;
	case xml_type_t::element:
		if (xml_has_name())
		{
			if (flag.is_active(xml_format::wrap_text_tab))
			{
				for (index i = 0; i < level; i++)
					stream << L"\t"_s;
			}
			else if (flag.is_active(xml_format::wrap_text_space))
			{
				for (index i = 0; i < level; i++)
					stream << L"  "_s;
			}

			stream << L"<"_s << _xml_name;

			if (xml_has_attributes())
			{
				stream << L" "_s;
				_xml_attributes->xml_print(stream, flag, level);
			}

			if (_xml_content.data_type != xml_type_t::abstract && !_xml_content.unknown.is_empty())
			{
				if (_xml_content.data_type == xml_type_t::value)
				{
					stream << L">"_s;
					xml_value_t(static_cast<strings::wstring_buffer*>(_xml_content.unknown.get())).xml_print(stream, flag, level);
				}
				else
				{
					stream << L">"_s;
					static_cast<xml_node*>(_xml_content.unknown.get())->xml_print(stream, flag, level + 1);
					if (flag.is_active(xml_format::wrap_text_tab))
					{
						stream << L"\n"_s;
						for (index i = 0; i < level; i++)
							stream << L"\t"_s;
					}
					else if (flag.is_active(xml_format::wrap_text_space))
					{
						stream << L"\n"_s;
						for (index i = 0; i < level; i++)
							stream << L"  "_s;
					}
				}
				stream << L"</"_s << _xml_name << L">"_s;
			}
			else
			{
				stream << L"/>"_s;
			}
		}
		break;
	case xml_type_t::comment:
		if (flag.is_active(xml_format::wrap_text_tab))
		{
			for (index i = 0; i < level; i++)
				stream << L"\t"_s;
		}
		else if (flag.is_active(xml_format::wrap_text_space))
		{
			for (index i = 0; i < level; i++)
				stream << L"  "_s;
		}
		stream << L"<!--"_s;
		if (xml_has_value())
			stream << xml_value_t(static_cast<strings::wstring_buffer*>(_xml_content.unknown.get()));
		stream << L"-->"_s;
		break;
	case xml_type_t::attribute:
		if (xml_has_name() && xml_has_value())
		{
			stream << _xml_name;
			xml_value_t val = static_cast<strings::wstring_buffer*>(_xml_content.unknown.get());
			if (val->find("\""_s, val->begin()).is_valid())
			{
				stream << L"='"_s;
				val.xml_print(stream, xml_format::none, 0);
				stream << L"'"_s;
			}
			else
			{
				stream << L"=\""_s;
				val.xml_print(stream, xml_format::none, 0);
				stream << L"\""_s;
			}
		}
		break;
	default:break;
	}
	return stream;
}

bool xml_node::xml_has_name()const { return false; }
bool xml_node::xml_has_value()const { return false; }
bool xml_node::xml_has_children()const { return false; }
bool xml_node::xml_has_attributes()const { return false; }

xml_node* xml_node::xml_parent()const
{
	return _xml_parent.lock().get();
}

xml_node* xml_node::xml_prev()const
{
	return _xml_prev.get();
}

xml_node* xml_node::xml_next()const
{
	return _xml_next.get();
}

void xml_node::xml_parent(xml_node* node)
{
	_xml_parent = node;
}

void xml_node::xml_prev(xml_node* node)
{
	_xml_prev = node;
}

void xml_node::xml_next(xml_node* node)
{
	_xml_next = node;
}

xml_value_t xml_node::xml_name()const
{
	return _xml_name.get();
}

xml_value_t xml_node::xml_value()const
{
	if (_xml_content.data_type == xml_type_t::value)
		return xml_value_t(static_cast<strings::wstring_buffer*>(_xml_content.unknown.get()));
	return xml_value_t();
}

xml_collection_t xml_node::xml_children()const
{
	if (_xml_content.data_type == xml_type_t::element_list)
		return static_cast<xml_collection*>(_xml_content.unknown.get());
	return null;
}

xml_attributes_t xml_node::xml_attributes()const
{
	return (xml_attribute_list*)_xml_attributes.get();
}

bool xml_node::xml_name(wstring value)
{
	if (_xml_type != xml_type_t::attribute
		&& _xml_type != xml_type_t::element)
		return false;
	_xml_name = ang::move(value);
	return true;
}

bool xml_node::xml_value(wstring value)
{
	if (_xml_type != xml_type_t::attribute
		&& _xml_type != xml_type_t::element
		&& _xml_type != xml_type_t::comment)
		return false;
	_xml_content.data_type = value.is_empty() ? xml_type_t::abstract : xml_type_t::value;
	_xml_content.unknown = value.get();
	return true;
}

bool xml_node::xml_children(xml_collection_t store)
{
	if (_xml_type != xml_type_t::element
		|| (!store.is_empty() && !store->xml_is_type_of(xml_type_t::element_list)))
		return false;
	_xml_content.data_type = xml_type_t::element_list;
	_xml_content.unknown = store.get();
	store->xml_parent(this);
	return true;
}

bool xml_node::xml_attributes(xml_collection_t store)
{
	if (_xml_type != xml_type_t::element
		|| (!store.is_empty() && !store->xml_is_type_of(xml_type_t::attribute_list)))
		return false;
	_xml_attributes = store.get();
	store->xml_parent(this);
	return true;
}