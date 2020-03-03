#ifndef __COFFE_DOM_XML_H__
#error ...
#elif !defined __COFFE_DOM_XML_IXML_TEXT_H__
#define __COFFE_DOM_XML_IXML_TEXT_H__

namespace coffe
{
	namespace dom
	{
		namespace xml
		{
			struct nvt LINK ixml_text
				: intf<ixml_text
				, iid("coffe::dom::xml::ixml_text")
				, text::istring>
			{
				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level = 0)const = 0;
				template<typename T> T as()const;
			};
		}
	}
	COFFE_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_text)
		operator cstr_t()const;
		char32_t operator [](windex)const;
		inline bool operator == (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this == (cstr_t)cstr; }
		inline bool operator != (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this != (cstr_t)cstr; }
		inline bool operator >= (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this >= (cstr_t)cstr; }
		inline bool operator <= (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this <= (cstr_t)cstr; }
		inline bool operator > (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
		inline bool operator < (dom::xml::ixml_text_t cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
		inline bool operator == (coffe::nullptr_t const&)const { return m_ptr == null; }
		inline bool operator != (coffe::nullptr_t const&)const { return m_ptr != null; }
	COFFE_END_INTF_WRAPPER();

	namespace text
	{
		template<typename T, text::encoding E>
		struct str_view_compare_helper<str_view<T, E>, dom::xml::ixml_text_t>
		{
			static int compare(const str_view<T, E>& value1, dom::xml::ixml_text_t value2) {
				return str_view_compare_helper<cstr_t, cstr_t>::compare(value1, value2);
			}
		};

		template<typename T, text::encoding E>
		struct str_view_compare_helper<dom::xml::ixml_text_t, str_view<T, E>>
		{
			static int compare(dom::xml::ixml_text_t value1, const str_view<T, E>& value2) {
				return str_view_compare_helper<cstr_t, cstr_t>::compare(value1, value2);
			}
		};
	}
}


#endif//__COFFE_DOM_XML_IXML_TEXT_H__
