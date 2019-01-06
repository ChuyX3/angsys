/*********************************************************************************************************************/
/*   File Name: ang/dom/xml.h                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes classes for the use of xml files                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_DOM_XML_H__
#define __ANG_DOM_XML_H__

#include <angsys.h>
#include <ang/core/files.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace dom
	{
		namespace xml
		{

			ang_interface(ixml_object);
			ang_interface(ixml_node);
			ang_interface(ixml_text);
			ang_interface(ixml_document);
			ang_interface(ixml_collection);

			ang_object(xml_document);
			ang_object(xml_node);
			//ang_object(xml_store);
			//ang_object(xml_text);
			//ang_object(xml_cdata);
			//ang_object(xml_pcdata);
			ang_object(xml_attribute);
			ang_object(xml_header);
			ang_object(xml_comment);
			ang_object(xml_element);

			typedef class ixml_attributes ixml_attributes_t;
			typedef collections::ienum<struct ixml_node > ixml_items;
			typedef collections::iterator<struct ixml_node > xml_iterator_t;
			typedef collections::base_iterator<struct ixml_node > xml_base_iterator_t;
			typedef collections::forward_iterator<struct ixml_node > xml_forward_iterator_t;
			typedef collections::backward_iterator<struct ixml_node > xml_backward_iterator_t;
			typedef collections::const_iterator<struct ixml_node > xml_const_iterator_t;
			typedef collections::const_forward_iterator<struct ixml_node > xml_const_forward_iterator_t;
			typedef collections::const_backward_iterator<struct ixml_node > xml_const_backward_iterator_t;

			typedef text::encoding xml_encoding;
			typedef text::encoding_t xml_encoding_t;

			safe_enum(LINK, xml_type, byte)
			{
				abstract = 0,
				text,
				cdata,
				pcdata,
				node,
				store,
				tree,
				header,
				element,
				comment,
				attribute,
				element_list,
				attribute_list,
				finder,
				builder,
				document
			};

			safe_enum(LINK, xml_exception_code, uint)
			{
				unknown,
				not_implemented = 0X200,
				parsing_error,
				unexpected_error,
			};

			safe_flags(LINK, xml_format, uint)
			{
				none = 0X0000,
				fix_entity = 0X0100,
				wrap_text_tab = 0X0200, //has more priority that WrapTextSpace
				wrap_text_space = 0X0400,
			};
		}
	}
}

#include <ang/dom/xml/ixml_object.h>
#include <ang/dom/xml/ixml_collection.h>
#include <ang/dom/xml/ixml_node.h>
#include <ang/dom/xml/ixml_text.h>
#include <ang/dom/xml/ixml_document.h>

#include <ang/dom/xml/xml_text.h>
#include <ang/dom/xml/xml_node.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_DOM_XML_H__
