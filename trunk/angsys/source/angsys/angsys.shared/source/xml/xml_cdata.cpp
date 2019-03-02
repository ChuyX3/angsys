/*********************************************************************************************************************/
/*   File Name: xml_cdata.cpp                                                                                           */
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

#define MY_TYPE ang::dom::xml::xml_cdata
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

/////////////////////////////////////////////////////////////////////////

xml_cdata_t xml_cdata::create_new(xml_document_t doc, const xml_cdata* data)
{
	xml_cdata_t node = NEW xml_cdata(doc);
	if (data)
	{
		if (!data->m_content.is_empty() && data->m_content->xml_is_type_of(xml_type::text))
			node->push_value(doc->create_pcdata((xml_cstr_t)data->m_content->xml_as<xml_text>()));
	}
	return node;
}

xml_cdata_t xml_cdata::create_new(xml_document_t doc, ixml_text_t text)
{
	xml_cdata_t node = NEW xml_cdata(doc);
	node->push_value(text);
	return node;
}

xml_cdata::xml_cdata(xml_document_t doc)
	: xml_node(doc, xml_type::cdata)
{
}

xml_cdata::~xml_cdata()
{
	clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_cdata);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_cdata, xml_node);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_cdata, xml_node);

xml_node_t xml_cdata::xml_clone(xml_document_t doc)const
{
	return create_new(xml_parent_doc(), this);
}

bool xml_cdata::xml_has_value()const
{
	return m_content.is_empty() ? false : m_content->xml_is_type_of(xml_type::text);
}

