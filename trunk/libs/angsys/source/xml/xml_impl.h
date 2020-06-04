/*********************************************************************************************************************/
/*   File Name: xml_impl.h                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes classes for the use of xml files                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#pragma once
#include <ang/dom/xml.h>

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			ang_declare_object(xml_node);
			ang_declare_object(xml_document);
			ang_declare_interface(ixml_list);

			typedef enum xml_entity : uint
			{
				amp,	//	&
				lt,		//	<
				gt,		//	>
				quot,	//	"
				apos,	//	'
			}xml_entity_t;

			typedef	struct xml_entity_values
			{
				xml_entity_t key;
				castr_t	code;
				castr_t	value;
			}xml_entity_values_t;

			extern xml_entity_values_t xml_entity_values[5];

		
			struct nvt ixml_list
				: intf<ixml_list
				, iid("ang::dom::xml::ixml_list")
				, ixml_items>
			{
				virtual xml_node_t front()const = 0;
				virtual xml_node_t back()const = 0;
				virtual void push_front(xml_node_t) = 0;
				virtual void push_back(xml_node_t) = 0;
				virtual xml_iterator_t insert(xml_base_iterator_t it, xml_node_t) = 0;
				virtual bool pop_front(xml_node_ptr_t = null) = 0;
				virtual bool pop_back(xml_node_ptr_t = null) = 0;
				virtual xml_iterator_t remove(xml_base_iterator_t it, xml_node_ptr_t = null) = 0;
			};



			template<typename EP = typename value_pack_from_range<xml_encoding, xml_encoding::ascii, xml_encoding::utf32_se>::type>
			struct xml_utils_base;

			using xml_utils = xml_utils_base<typename value_pack_from_range<xml_encoding, xml_encoding::ascii, xml_encoding::utf32_se>::type>;

			template<typename T, xml_encoding E>
			struct xml_utils_impl
			{

				static int xml_skip_space(str_view<T, E> const& code, windex begin);
				static int xml_get_element_name(str_view<T, E> const& code, windex begin);
				static int xml_get_attibute_name(str_view<T, E> const& code, windex begin);
				static error xml_parse(ixml_document_builder_t doc, str_view<T, E> const& code, windex& begin);
			};

			template<xml_encoding...ES>
			struct xml_utils_base<value_pack<xml_encoding, ES...>>
			{
				using xml_parse_type = error(*)(ixml_document_builder_t, void*, wsize, windex&);

				using pack_type = value_pack<xml_encoding, ES...>;
				static constexpr xml_encoding FIRST = pack_type::get<0>();
				static constexpr wsize PACK_SIZE = value_pack_size<pack_type>::value;

				static error parse(ixml_document_builder_t doc, void* code, wsize sz, xml_encoding e, windex& begin)
				{
					wsize i = get_index(e);
					return i < PACK_SIZE ? s_xml_parse_array[i](doc, code, sz, begin) : error(error_code::invalid_param);
				}

			private:
				static wsize get_index(xml_encoding E) { return wsize(E) - wsize(FIRST); }

				template<xml_encoding E>
				static error xml_parse(ixml_document_builder_t doc, void* ptr, wsize sz, windex& idx)
				{
					using char_type = typename text::char_type_by_encoding<E>::char_type;
					return xml_utils_impl<const char_type, E>::xml_parse(doc, str_view<const char_type, E>((char_type*)ptr, sz / size_of<char_type>()), idx);
				}
				
				static constexpr xml_parse_type s_xml_parse_array[] = { &xml_parse<ES>... };
			};

		}
	}

	template<> struct is_base_of<object, dom::xml::xml_node> : true_type {};
	template<> struct is_base_of<iintf, dom::xml::xml_node> : true_type {};

	template<> struct is_base_of<object, dom::xml::xml_document> : true_type {};
	template<> struct is_base_of<iintf, dom::xml::xml_document> : true_type {};
}

#include "xml/xml_text.h"
#include "xml/xml_list.h"
#include  "xml/xml_node.h"
#include  "xml/xml_document.h"

#include "xml/inline/xml_text.inl"
#include "xml/inline/xml_impl.inl"