/*********************************************************************************************************************/
/*   File Name: ang/dom/xml.h                                                                                        */
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
#include <ang/collections/hash_map.h>

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
			ang_interface(ixml_document);
			ang_interface(ixml_collection);
			ang_interface(ixml_serializable);

			ang_object(xml_document);
			ang_object(xml_collection);
			ang_object(xml_node);
			ang_object(xml_text);
			ang_object(xml_cdata);
			ang_object(xml_attribute);
			ang_object(xml_namespace);
			ang_object(xml_comment);
			ang_object(xml_element);

			typedef class xml_attributes xml_attributes_t;
			typedef collections::ienum<xml_node> ixml_items;
			typedef intf_wrapper<ixml_items> ixml_items_t;
			typedef collections::iterator<xml_node> xml_iterator_t;
			typedef collections::base_iterator<xml_node> xml_base_iterator_t;
			typedef collections::forward_iterator<xml_node> xml_forward_iterator_t;
			typedef collections::backward_iterator<xml_node> xml_backward_iterator_t;
			typedef collections::const_iterator<xml_node> xml_const_iterator_t;
			typedef collections::const_forward_iterator<xml_node> xml_const_forward_iterator_t;
			typedef collections::const_backward_iterator<xml_node> xml_const_backward_iterator_t;

			typedef text::encoding xml_encoding;
			typedef text::encoding_t xml_encoding_t;

			safe_enum(LINK, xml_type, byte)
			{
				abstract = 0,
				text,
				cdata,
				node,
				collection,
				tree,
				element,
				comment,
				attribute,
				name_space,
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

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_text)
		operator cstr_t()const;
	ANG_END_OBJECT_WRAPPER();
}

#include <ang/dom/xml/ixml_object.h>
#include <ang/dom/xml/ixml_collection.h>
#include <ang/dom/xml/ixml_node.h>
#include <ang/dom/xml/ixml_document.h>


namespace ang
{

	template<> struct is_inherited_from<dom::xml::xml_node, object> : true_type {};
	template<> struct is_inherited_from<dom::xml::xml_node, intf> : true_type {};

	template<> struct is_inherited_from<dom::xml::xml_document, object> : true_type {};
	template<> struct is_inherited_from<dom::xml::xml_document, intf> : true_type {};

	

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_document)
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_collection)
		dom::xml::xml_forward_iterator_t begin();
		dom::xml::xml_forward_iterator_t end();
		dom::xml::xml_const_forward_iterator_t begin()const;
		dom::xml::xml_const_forward_iterator_t end()const;
		operator dom::xml::xml_text_t()const;
		dom::xml::xml_node_t operator[](str_t)const;
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_node)
		dom::xml::xml_node_t operator[] (cstr_t)const;
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_cdata)
		inline operator ang::dom::xml::xml_node_t()const;
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_attribute)
		inline operator ang::dom::xml::xml_node_t()const;
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_namespace)
		inline operator ang::dom::xml::xml_node_t()const;
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_comment)
		inline operator ang::dom::xml::xml_node_t()const;
	ANG_END_OBJECT_WRAPPER();

	ANG_BEGIN_OBJECT_WRAPPER(LINK, dom::xml::xml_element)
		inline operator ang::dom::xml::xml_node_t()const;
	ANG_END_OBJECT_WRAPPER();


}

#include <ang/dom/xml/xml_text.h>
#include <ang/dom/xml/xml_node.h>
#include <ang/dom/xml/xml_cdata.h>
#include <ang/dom/xml/xml_attribute.h>
#include <ang/dom/xml/xml_comment.h>
#include <ang/dom/xml/xml_element.h>
#include <ang/dom/xml/xml_document.h>

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<typename T, typename = void>
			struct xml_text_as_helper : false_type
			{
				static inline T parse(xml_text const* cstr) {
					throw_exception(except_code::unsupported);
					return T();
				}
			};

			template<typename T> inline T xml_text::xml_as()const {
				return xml_text_as_helper<T>::parse(this);
			}

			template<typename T>
			struct xml_text_as_helper<T, void_t<decltype(&T::parse)>> : true_type
			{
				static inline T parse(xml_text const* cstr) {
					return T::parse(cstr ? cstr->cstr() : cstr_t());
				}
			};

			template<> string  LINK ixml_object::xml_as<string_buffer>()const;
			template<> xml_text_t  LINK ixml_object::xml_as<xml_text>()const;
			template<> ixml_node_t  LINK ixml_object::xml_as<ixml_node>()const;
			template<> xml_node_t  LINK ixml_object::xml_as<xml_node>()const;
			template<> xml_attribute_t  LINK ixml_object::xml_as<xml_attribute>()const;
			template<> xml_comment_t  LINK ixml_object::xml_as<xml_comment>()const;
			template<> xml_element_t  LINK ixml_object::xml_as<xml_element>()const;
			template<> ixml_collection_t  LINK ixml_object::xml_as<ixml_collection>()const;
			template<> xml_collection_t  LINK ixml_object::xml_as<xml_collection>()const;
			template<> ixml_document_t  LINK ixml_object::xml_as<ixml_document>()const;
			template<> xml_document_t  LINK ixml_object::xml_as<xml_document>()const;

			template<> LINK cstr_t xml_text::xml_as<cstr_t>()const;
			template<> LINK astring xml_text::xml_as<astring>()const;
			template<> LINK wstring xml_text::xml_as<wstring>()const;
			template<> LINK mstring xml_text::xml_as<mstring>()const;

			template<> LINK short xml_text::xml_as<short>()const;
			template<> LINK ushort xml_text::xml_as<ushort>()const;
			template<> LINK int xml_text::xml_as<int>()const;
			template<> LINK uint xml_text::xml_as<uint>()const;
			template<> LINK long xml_text::xml_as<long>()const;
			template<> LINK ulong xml_text::xml_as<ulong>()const;
			template<> LINK long64 xml_text::xml_as<long64>()const;
			template<> LINK ulong64 xml_text::xml_as<ulong64>()const;
			template<> LINK float xml_text::xml_as<float>()const;
			template<> LINK double xml_text::xml_as<double>()const;
			template<> LINK bool xml_text::xml_as<bool>()const;
		}
	}
}

inline ang::object_wrapper<ang::dom::xml::xml_text>::operator ang::cstr_t()const { return is_empty() ? cstr_t() : get()->cstr(); }
inline ang::object_wrapper<ang::dom::xml::xml_attribute>::operator ang::dom::xml::xml_node_t()const { return get(); }
inline ang::object_wrapper<ang::dom::xml::xml_namespace>::operator ang::dom::xml::xml_node_t()const { return get(); }
inline ang::object_wrapper<ang::dom::xml::xml_cdata>::operator ang::dom::xml::xml_node_t()const { return get(); }
inline ang::object_wrapper<ang::dom::xml::xml_comment>::operator ang::dom::xml::xml_node_t()const { return get(); }
inline ang::object_wrapper<ang::dom::xml::xml_element>::operator ang::dom::xml::xml_node_t()const { return get(); }


inline ang::collections::iterator<ang::dom::xml::xml_node>::operator ang::dom::xml::xml_node*()const {
	return reinterpret_cast<ang::dom::xml::xml_node*>(current());
}
inline ang::dom::xml::xml_node_t ang::collections::iterator<ang::dom::xml::xml_node>::operator -> ()const {
	return reinterpret_cast<ang::dom::xml::xml_node*>(current());
}
inline ang::dom::xml::xml_node_t ang::collections::iterator<ang::dom::xml::xml_node>::operator * ()const {
	return reinterpret_cast<ang::dom::xml::xml_node*>(current());
}

inline ang::collections::iterator<const ang::dom::xml::xml_node>::operator ang::dom::xml::xml_node const*()const {
	return reinterpret_cast<ang::dom::xml::xml_node*>(current());
}
inline const ang::dom::xml::xml_node_t ang::collections::iterator<const ang::dom::xml::xml_node>::operator -> ()const {
	return reinterpret_cast<ang::dom::xml::xml_node*>(current());
}
inline const ang::dom::xml::xml_node_t ang::collections::iterator<const ang::dom::xml::xml_node>::operator * ()const {
	return reinterpret_cast<ang::dom::xml::xml_node*>(current());
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_DOM_XML_H__
