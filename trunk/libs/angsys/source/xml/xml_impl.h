#pragma once
#include <coffe/dom/xml.h>

namespace coffe
{
	namespace dom
	{
		namespace xml
		{
			coffe_declare_object(xml_node);
			coffe_declare_object(xml_document);
			coffe_declare_interface(ixml_list);

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

			template<typename T, xml_encoding E>
			class xml_utils
			{
			public:
				typedef text::text_encoder<E> encoder;

				static int xml_skip_space(str_view<T, E> const& code, windex begin);
				static int xml_get_element_name(str_view<T, E> const& code, windex begin);
				static int xml_get_attibute_name(str_view<T, E> const& code, windex begin);
			};

			struct nvt ixml_list
				: intf<ixml_list
				, iid("coffe::dom::xml::ixml_list")
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
		}
	}

	template<> struct is_base_of<bean, dom::xml::xml_node> : true_type {};
	template<> struct is_base_of<iintf, dom::xml::xml_node> : true_type {};

	template<> struct is_base_of<bean, dom::xml::xml_document> : true_type {};
	template<> struct is_base_of<iintf, dom::xml::xml_document> : true_type {};
}

#include "xml/xml_text.h"
#include "xml/xml_list.h"
#include  "xml/xml_node.h"
#include  "xml/xml_document.h"

#include "xml/inline/xml_text.inl"
#include "xml/inline/xml_impl.inl"