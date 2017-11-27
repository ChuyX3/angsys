/*********************************************************************************************************************/
/*   File Name: xml_text.cpp                                                                                           */
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

xml_entity_values_t xml::xml_entity_values[5] = {
	{ xml_entity::amp, "&amp;"_s, "&"_s },
	{ xml_entity::lt, "&lt;"_s, "<"_s },
	{ xml_entity::gt, "&gt;"_s, ">" },
	{ xml_entity::quot, "&quot;"_s, "\""_s },
	{ xml_entity::apos, "&apos;"_s, "'"_s }
};

/////////////////////////////////////////////////////////////////////////

ang::object_wrapper<xml_text>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<xml_text>::object_wrapper(xml_text* ptr)
	: _ptr(null)
{
	set(ptr);
}

ang::object_wrapper<xml_text>::object_wrapper(object_wrapper && other)
	: _ptr(null)
{
	xml_text * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<xml_text>::object_wrapper(object_wrapper const& other)
	: _ptr(null)
{
	set(other._ptr);
}

ang::object_wrapper<xml_text>::object_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::object_wrapper<xml_text>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<xml_text>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<xml_text>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<xml_text>::is_empty()const
{
	return _ptr == null;
}

xml_text* ang::object_wrapper<xml_text>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<xml_text>::set(xml_text* ptr)
{
	xml_text * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<xml_text>& ang::object_wrapper<xml_text>::operator = (nullptr_t const&)
{
	clean();
	return*this;
}

ang::object_wrapper<xml_text>& ang::object_wrapper<xml_text>::operator = (xml_text* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<xml_text>& ang::object_wrapper<xml_text>::operator = (ang::object_wrapper<xml_text> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<xml_text>& ang::object_wrapper<xml_text>::operator = (ang::object_wrapper<xml_text> const& other)
{
	set(other._ptr);
	return*this;
}

xml_text ** ang::object_wrapper<xml_text>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<xml_text> ang::object_wrapper<xml_text>::operator& (void)
{
	return this;
}

xml_text * ang::object_wrapper<xml_text>::operator -> (void)
{
	return get();
}

xml_text const* ang::object_wrapper<xml_text>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<xml_text>::operator xml_text * (void)
{
	return get();
}

ang::object_wrapper<xml_text>::operator xml_text const* (void)const
{
	return get();
}

ang::object_wrapper<xml_text>::operator wstring (void)const
{
	return get();
}

ang::object_wrapper<xml_text>::operator wstr_t (void)
{
	return is_empty()? wstr_t(): _ptr->str();
}

ang::object_wrapper<xml_text>::operator cwstr_t (void)const
{
	return is_empty() ? cwstr_t() : _ptr->cstr();
}

/////////////////////////////////////////////////////////////////////////

xml_text::xml_text()
	: strings::string_buffer<text::encoding::unicode>()
	, _xml_parent(null) 
{

}

xml_text::xml_text(xml_text const* value)
	: strings::string_buffer<text::encoding::unicode>()
	, _xml_parent(null) 
{
	if (value)copy(value->cstr());
}

xml_text::xml_text(xml_text const& value)
	: strings::string_buffer<text::encoding::unicode>()
	, _xml_parent(null)
{
	copy(value.cstr());
}

xml_text::xml_text(wstring && value)
	: strings::string_buffer<text::encoding::unicode>()
	, _xml_parent(null)
{
	if (!value.is_empty())move(value);
}

xml_text::~xml_text()
{
	clean();
}

ANG_IMPLEMENT_CLASSNAME(ang::xml::xml_text);
ANG_IMPLEMENT_OBJECTNAME(ang::xml::xml_text);

bool ang::xml::xml_text::is_kind_of(ang::type_name_t name)const
{
	return name == ang::xml::xml_text::class_name()
		|| ang::strings::unicode_string_buffer::is_kind_of(name)
		|| ang::xml::ixml_object::is_kind_of(name);
}

bool ang::xml::xml_text::is_inherited_of(ang::type_name_t name)
{
	return name == ang::xml::xml_text::class_name()
		|| ang::strings::unicode_string_buffer::is_inherited_of(name)
		|| ang::xml::ixml_object::is_inherited_of(name);
}

bool ang::xml::xml_text::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::xml::xml_text*>(this);
		return true;
	}
	else if (ang::strings::unicode_string_buffer::query_object(name, out))
	{
		return true;
	}
	else if (xml::ixml_node::query_object(name, out))
	{
		return true;
	}
	return false;
}

xml_type_t xml_text::xml_type()const { return xml_type::text; }

bool xml_text::xml_is_type_of(xml_type_t type)const {return type == xml_type::text; }

wstring& xml_text::xml_print(wstring& out, xml_format_t const& format, ushort)const
{
	if (is_empty()) return out;
	out << this->cstr();
	return out;
}

ixml_node_t xml_text::xml_parent()const
{
	return _xml_parent.lock();
}

ixml_document_t xml_text::xml_parent_doc()const
{
	return _xml_parent_doc.lock();
}

ixml_node_t xml_text::xml_clone(ixml_document_t doc)const 
{
	auto text = NEW xml_text(this);
	text->xml_parent_doc(doc);
	return text;
}

void xml_text::xml_parent(ixml_node_t node)
{ 
	_xml_parent = node;
	_xml_parent_doc = (node) ? node->xml_parent_doc() : null;
}

void xml_text::xml_parent_doc(ixml_document_t doc)
{
	_xml_parent_doc = doc;
}

void xml_text::xml_prev_sibling(ixml_node_t node)
{
}

void xml_text::xml_next_sibling(ixml_node_t node)
{
}

ixml_node_t xml_text::xml_first_child()const { return null; }
ixml_node_t xml_text::xml_last_child()const { return null; }
ixml_node_t xml_text::xml_prev_sibling()const { return null; }
ixml_node_t xml_text::xml_next_sibling()const { return null; }

bool xml_text::xml_has_name()const { return false; }
bool xml_text::xml_has_value()const { return false; }
bool xml_text::xml_has_children()const { return false; }
bool xml_text::xml_has_attributes()const { return false; }

xml_text_t xml_text::xml_name()const { return null; }
xml_text_t xml_text::xml_value()const { return null; }
ixml_store_t xml_text::xml_children()const { return null; }
ixml_attributes_t xml_text::xml_attributes()const { return null; }



template<> cstr_t xml::xml_text::xml_as<cstr_t>()const { return null; }
template<> cwstr_t xml::xml_text::xml_as<cwstr_t>()const { return this? this->cstr() : null; }
template<> string xml::xml_text::xml_as<string>()const { return this ? this->cstr() : null; } //make a copy
template<> wstring xml::xml_text::xml_as<wstring>()const { return this ? this->cstr() : null; } //make a copy
template<> mstring xml::xml_text::xml_as<mstring>()const { return this ? this->cstr() : null; } //make a copy

template<> short xml::xml_text::xml_as<short>()const { return integer::parse(xml_as<cwstr_t>()); }
template<> ushort xml::xml_text::xml_as<ushort>()const { return uinteger::parse(xml_as<cwstr_t>()); }
template<> int xml::xml_text::xml_as<int>()const { return integer::parse(xml_as<cwstr_t>()); }
template<> uint xml::xml_text::xml_as<uint>()const { return uinteger::parse(xml_as<cwstr_t>()); }
template<> long xml::xml_text::xml_as<long>()const { return integer::parse(xml_as<cwstr_t>()); }
template<> ulong xml::xml_text::xml_as<ulong>()const { return uinteger::parse(xml_as<cwstr_t>()); }
template<> long64 xml::xml_text::xml_as<long64>()const { return integer64::parse(xml_as<cwstr_t>()); }
template<> ulong64 xml::xml_text::xml_as<ulong64>()const { return uinteger64::parse(xml_as<cwstr_t>()); }
template<> float xml::xml_text::xml_as<float>()const { return floating::parse(xml_as<cwstr_t>()); }
template<> double xml::xml_text::xml_as<double>()const { return floating64::parse(xml_as<cwstr_t>()); }
template<> bool xml::xml_text::xml_as<bool>()const { return boolean::parse(xml_as<cwstr_t>()); }
template<> xml::xml_encoding_t xml::xml_text::xml_as<xml::xml_encoding_t>()const { return xml::xml_encoding_t::parse(xml_as<cwstr_t>()); }

/////////////////////////////////////////////////////////////////////////////

xml_pcdata::xml_pcdata()
	: xml_text()
{
}

xml_pcdata::xml_pcdata(xml_pcdata const* value)
	: xml_text(value)
{
	
}

xml_pcdata::xml_pcdata(xml_pcdata const& value)
	: xml_text(value)
{
}

xml_pcdata::xml_pcdata(wstring && value)
	: xml_text(value)
{

}

xml_pcdata::xml_pcdata(wstring const& value)
	: xml_text(value)
{
}


xml_pcdata::~xml_pcdata()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_pcdata, xml_text);

xml_type_t xml_pcdata::xml_type()const { return xml_type::pcdata; }

bool xml_pcdata::xml_is_type_of(xml_type_t type)const { 
	return type == xml_type::pcdata
	|| xml_text::xml_is_type_of(type);
}

ixml_node_t xml_pcdata::xml_clone(ixml_document_t doc)const
{
	auto text = NEW xml_pcdata(this);
	text->xml_parent_doc(doc);
	return text;
}

wstring& xml_pcdata::xml_print(wstring& out, xml_format_t const& format, ushort)const
{
	if (is_empty())
		return out;

	wstring str = this->cstr(); //make a copy
	cwstr_t cstr = str;
	index idx = 0;
	while (cstr[idx] != 0 && idx < cstr.size())
	{
		for (int i = 0; i < 5; i++)
		{
			if (xml_entity_values[i].code[0] == cstr[idx])
			{
				//TODO: str->replace(xml_entity_values[i].code, str->at(idx), str->at(idx + 1));
				cstr = str;
				break;
			}
		}
		idx++;
	}
	out << str;
	return out;
}


/////////////////////////////////////////////////////////////////////////////

xml_cdata::xml_cdata()
	: xml_text()
{
}

xml_cdata::xml_cdata(xml_cdata const* value)
	: xml_text(value)
{
}

xml_cdata::xml_cdata(xml_cdata const& value)
	: xml_text(value)
{
}

xml_cdata::xml_cdata(wstring && value)
	: xml_text(value)
{

}

xml_cdata::xml_cdata(wstring const& value)
	: xml_text(value)
{
}


xml_cdata::~xml_cdata()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::xml::xml_cdata, xml_text);

xml_type_t xml_cdata::xml_type()const { return xml_type::cdata; }

bool xml_cdata::xml_is_type_of(xml_type_t type)const { 
	return type == xml_type::pcdata
	|| xml_text::xml_is_type_of(type);
}

ixml_node_t xml_cdata::xml_clone(ixml_document_t doc)const
{
	auto text = NEW xml_cdata(this);
	text->xml_parent_doc(doc);
	return text;
}