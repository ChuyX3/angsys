#include <angsys.h>
#include <ang/dom/xml.h>

namespace ang
{
	namespace dom
	{
		namespace xml
		{
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

			template<>
			class xml_utils<void, xml_encoding::auto_detect>
			{
			public:
				static int xml_skip_space(text::istring_view_t const& code, windex begin);
				static int xml_get_element_name(text::istring_view_t const& code, windex begin);
				static int xml_get_attibute_name(text::istring_view_t const& code, windex begin);
			};

		}
	}
}


#include "xml/inline/xml_internal.inl"