/*********************************************************************************************************************/
/*   File Name: xml_comment.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <ang/dom/xml.h>

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

using namespace ang;
using namespace ang::dom::xml;


#define MY_TYPE ang::dom::xml::xml_comment
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

xml_comment_t xml_comment::create_new(xml_document_t doc, const xml_comment* comment) {
	xml_comment_t node = NEW xml_comment(doc);
	if (comment)
	{
		if (!comment->m_content.is_empty() && comment->m_content->xml_is_type_of(xml_type::text))
			node->push_value(doc->create_pcdata((xml_cstr_t)comment->m_content.get()->xml_as<xml_text>()));
	}
	return node;
}

xml_comment_t xml_comment::create_new(xml_document_t doc, ixml_text_t value) {
	xml_comment_t node = NEW xml_comment(doc);
	node->push_value(value);
	return node;
}

xml_comment::xml_comment(xml_document_t doc)
	: base(doc, xml_type::comment)
{

}

xml_comment::~xml_comment()
{
	clear();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(xml_comment);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dom::xml::xml_comment, xml_node);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(xml_comment, xml_node);

bool xml_comment::xml_has_value()const
{
	return !m_content.is_empty() && m_content->xml_is_type_of(xml_type::text);
}

xml_node_t xml_comment::xml_clone(xml_document_t doc)const
{
	return xml_comment::create_new(doc, this);
}
