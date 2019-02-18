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
			ang_interface(ixml_header);
			ang_interface(ixml_document);
			ang_interface(ixml_collection);

			typedef class xml_attributes xml_attributes_t;
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

			typedef text::itext_buffer ixml_text;
			typedef text::itext_buffer_t ixml_text_t;

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
				unexpected_error
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
#include <ang/dom/xml/ixml_header.h>
#include <ang/dom/xml/ixml_collection.h>
#include <ang/dom/xml/ixml_node.h>
#include <ang/dom/xml/ixml_document.h>

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			class xml_attributes
			{
			public:
				typedef xml::ixml_collection type;

			private:
				xml::ixml_collection* _ptr;

			public:
				xml_attributes();
				xml_attributes(xml::ixml_collection*);
				xml_attributes(xml_attributes_t &&);
				xml_attributes(xml_attributes_t const&);
				xml_attributes(std::nullptr_t const&);
				~xml_attributes();

			public:
				void reset();
				void reset_unsafe();
				bool is_empty()const;
				xml::ixml_collection* get(void)const;
				void set(xml::ixml_collection*);
				xml::ixml_collection ** addres_of(void);

				xml::xml_forward_iterator_t begin() {
					return _ptr ? _ptr->begin() : xml::xml_forward_iterator_t(null);
				}
				xml::xml_forward_iterator_t end() {
					return _ptr ? _ptr->end() : xml::xml_forward_iterator_t(null);
				}

				xml::xml_const_forward_iterator_t begin()const {
					return _ptr ? ((xml::ixml_collection const*)_ptr)->begin() : xml::xml_const_forward_iterator_t(null);
				}
				xml::xml_const_forward_iterator_t end()const {
					return _ptr ? ((xml::ixml_collection const*)_ptr)->end() : xml::xml_const_forward_iterator_t(null);
				}

				//xml_attribute_t xml_attribute(raw_str_t);
				//xml_namespace_t xml_namespace(raw_str_t);

				//template<typename T, xml_encoding E> xml_attribute_t xml_attribute(str_view<T, E> str)const { return xml_attribute(raw_str(str)); }
				//template<typename T, xml_encoding E> xml_namespace_t xml_namespace(str_view<T, E> str)const { return xml_namespace(raw_str(str)); }

			public:
				xml_attributes_t& operator = (xml::ixml_collection*);
				xml_attributes_t& operator = (xml_attributes_t &&);
				xml_attributes_t& operator = (xml_attributes_t const&);

				//intf_wrapper_ptr<xml::ixml_collection> operator & (void);
				xml::ixml_collection * operator -> (void);
				xml::ixml_collection const* operator -> (void)const;

				operator xml::ixml_collection * (void);
				operator xml::ixml_collection const* (void)const;

				ixml_text_t operator[](raw_str_t)const;
				template<typename T, xml_encoding E> ixml_text_t operator[](str_view<T, E> str)const { return operator[](raw_str(str)); }
			};
		}
	}
}

//#include <ang/dom/xml/xml_node.h>
//#include <ang/dom/xml/xml_text.h>
//#include <ang/dom/xml/xml_attribute.h>
//#include <ang/dom/xml/xml_comment.h>
//#include <ang/dom/xml/xml_element.h>
//#include <ang/dom/xml/xml_document.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_DOM_XML_H__
