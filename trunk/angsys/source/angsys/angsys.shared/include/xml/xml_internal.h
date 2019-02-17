#include <angsys.h>
#include <ang/dom/xml.h>

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<xml_encoding E> class xml_document;
			template<xml_encoding E> class xml_node;
			template<xml_encoding E> class xml_cdata;
			template<xml_encoding E> class xml_attribute;
			template<xml_encoding E> class xml_namespace;
			template<xml_encoding E> class xml_header;
			template<xml_encoding E> class xml_comment;
			template<xml_encoding E> class xml_element;

			template<xml_encoding E> using xml_document_ptr = xml_document<E>;
			template<xml_encoding E> using xml_node_ptr = xml_node<E>;
			template<xml_encoding E> using xml_cdata_ptr = xml_cdata<E>;
			template<xml_encoding E> using xml_attribute_ptr = xml_attribute<E>;
			template<xml_encoding E> using xml_namespace_ptr = xml_namespace<E>;
			template<xml_encoding E> using xml_header_ptr = xml_header<E>;
			template<xml_encoding E> using xml_comment_ptr = xml_comment<E>;
			template<xml_encoding E> using xml_element_ptr = xml_element<E>;

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
				cstr_t	code;
				cstr_t	value;
			}xml_entity_values_t;
			extern xml_entity_values_t xml_entity_values[5];

			//////////////////////////////////////////////////////////////////////////////////////////////

			template<typename T, xml_encoding E>
			class xml_utils
			{
			public:
				typedef text::encoder<E> encoder;

				static int xml_skip_space(str_view<T, E> const& code, windex begin);
				static int xml_get_element_name(str_view<T, E> const& code, windex begin);
				static int xml_get_attibute_name(str_view<T, E> const& code, windex begin);
			};
			

		}
	}
}

#include "xml/xml_node.h"
#include "xml/xml_text.h"
#include "xml/xml_attribute.h"
#include "xml/xml_comment.h"
#include "xml/xml_header.h"
#include "xml/xml_element.h"

#include "xml/xml_document.h"


#include "xml/inline/xml_internal.inl"