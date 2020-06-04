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

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY

#ifdef ANG_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			ang_declare_interface(ixml_node);
			ang_declare_interface(ixml_text);
			ang_declare_interface(ixml_items);
			ang_declare_interface(ixml_document);
			ang_declare_interface(ixml_document_builder);
			ang_declare_interface(ixml_streamer);

			typedef collections::ienum<ixml_node> ixml_enum;
			typedef collections::ienum_ptr<ixml_node> ixml_enum_t;
			typedef collections::ifinder<ixml_node, cstr_t> ixml_finder;
			typedef collections::ifinder_ptr<ixml_node, cstr_t> ixml_finder_t;

			typedef class xml_attribs ixml_attbs_t;
			typedef collections::iterator<ixml_node> xml_iterator_t;
			typedef collections::base_iterator<ixml_node> xml_base_iterator_t;
			typedef collections::forward_iterator<ixml_node> xml_forward_iterator_t;
			typedef collections::backward_iterator<ixml_node> xml_backward_iterator_t;
			typedef collections::const_iterator<ixml_node> xml_const_iterator_t;
			typedef collections::const_forward_iterator<ixml_node> xml_const_forward_iterator_t;
			typedef collections::const_backward_iterator<ixml_node> xml_const_backward_iterator_t;

			typedef text::encoding xml_encoding;
			typedef text::encoding_t xml_encoding_t;

			declare_enum(LINK, xml_type, byte)
			{
				abstract = 0,
				document, //= 1
				text, //= 2
				cdata, // =3
				node, //= 4
				element, //= 5
				comment, //= 6
				attribute, //= 7
				name_space, //= 8
				element_list, //= 9
				attribute_list, //= 10
				
			};

			declare_flags(LINK, xml_format, uint)
			{
				none = 0X0000,
				fix_entity = 0X0100,
				wrap_text_tab = 0X0200, //has more priority that WrapTextSpace
				wrap_text_space = 0X0400,
			};
		}
	}

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_text)
		operator cstr_t()const;
		char32_t operator [](windex)const;
		inline bool operator == (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this == (cstr_t)cstr; }
		inline bool operator != (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this != (cstr_t)cstr; }
		inline bool operator >= (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this >= (cstr_t)cstr; }
		inline bool operator <= (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this <= (cstr_t)cstr; }
		inline bool operator > (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
		inline bool operator < (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
		inline bool operator == (ang::nullptr_t const&)const { return m_ptr == null; }
		inline bool operator != (ang::nullptr_t const&)const { return m_ptr != null; }
	ANG_END_INTF_WRAPPER();

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_document)
		dom::xml::ixml_node_t operator[](cstr_t)const;
	ANG_END_INTF_WRAPPER();

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_node)
		operator dom::xml::ixml_text_t()const;
		dom::xml::ixml_node_t operator[](cstr_t)const;
	ANG_END_INTF_WRAPPER();
}

#include <ang/dom/xml/ixml_text.h>
#include <ang/dom/xml/ixml_node.h>
#include <ang/dom/xml/ixml_items.h>
#include <ang/dom/xml/ixml_document.h>

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<typename T, typename = void>
			struct xml_text_as_helper : false_type
			{
				static inline T parse(ixml_text const* cstr) {
					throw_exception(error_code::unimplemented);
					return T();
				}
			};

			template<typename T> inline T ixml_text::as()const {
				return xml_text_as_helper<T>::parse(this);
			}

			template<typename T>
			struct xml_text_as_helper<T, void_t<decltype(&T::parse)>> : true_type
			{
				static inline T parse(ixml_text const* cstr) {
					return T::parse(cstr ? cstr->cstr() : cstr_t());
				}
			};

			template<> LINK cstr_t ixml_text::as<cstr_t>()const;
			template<> LINK string ixml_text::as<string>()const;

			template<> LINK short ixml_text::as<short>()const;
			template<> LINK ushort ixml_text::as<ushort>()const;
			template<> LINK int ixml_text::as<int>()const;
			template<> LINK uint ixml_text::as<uint>()const;
			template<> LINK long ixml_text::as<long>()const;
			template<> LINK ulong ixml_text::as<ulong>()const;
			template<> LINK long64 ixml_text::as<long64>()const;
			template<> LINK ulong64 ixml_text::as<ulong64>()const;
			template<> LINK float ixml_text::as<float>()const;
			template<> LINK double ixml_text::as<double>()const;
			template<> LINK bool ixml_text::as<bool>()const;
		}
	}
}

ANG_ENUM_DECLARATION(LINK, ang::dom::xml::xml_type);
ANG_FLAGS_DECLARATION(LINK, ang::dom::xml::xml_format);

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_DOM_XML_H__
