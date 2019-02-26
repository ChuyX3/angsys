#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_IXML_DOCUMENT_H__
#define __ANG_DOM_XML_IXML_DOCUMENT_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{

			ang_begin_interface(LINK ixml_document, ixml_object, collections::ienum<ixml_node>)
				visible vcall xml_encoding_t xml_encoding()const pure;
				visible vcall ixml_header_t xml_header()const pure;
				visible vcall ixml_node_t xml_data_type()const pure;
				visible vcall ixml_node_t xml_root_element()const pure;
				visible vcall ixml_document_t xml_clone()const pure;

				visible vcall xml_iterator_t find(raw_str_t, bool invert = false)const pure;
				visible vcall xml_iterator_t find(raw_str_t, xml_iterator_t next_to, bool invert = false)const pure;

				visible vcall xml_iterator_t xml_current()const pure;
				visible vcall ixml_node_t xml_current_element()const pure;

				visible vcall bool move_to(xml_iterator_t current) pure;
				visible vcall bool move_to_child(xml_iterator_t child) pure;

				visible vcall bool move_up() pure;
				visible vcall bool move_down() pure;
				visible vcall bool move_forward() pure;
				visible vcall bool move_backward() pure;

				visible vcall void push_default_header() pure;
				visible vcall void push_header(raw_str_t version, bool standalone) pure;
				visible vcall bool begin_element(raw_str_t name) pure;
				visible vcall bool end_element() pure;
				visible vcall bool push_element(raw_str_t name, raw_str_t value) pure;
				visible vcall bool push_element(raw_str_t element) pure;
				visible vcall bool push_data(raw_str_t value) pure;
				visible vcall bool push_value(raw_str_t value) pure;
				visible vcall bool push_attribute(raw_str_t name, raw_str_t value) pure;
				visible vcall bool push_comment(raw_str_t value) pure;

				visible vcall void load(core::files::input_text_file_t) pure;
				visible vcall void save(core::files::output_text_file_t)const pure;

				visible vcall void parse(raw_str_t text)pure;
				template<typename T, text::encoding E> inline void parse(str_view<T, E> const& text) { parse(raw_str(text)); }
				template<text::encoding E, template<typename>class A> inline void parse(strings::basic_string<E, A> const& text) { parse(text.is_empty() ? raw_str() : raw_str(text->cstr())); }

			ang_end_interface();

			ixml_document_t xml_create_document(xml_encoding_t, core::files::input_text_file_t = null);
		}
	}
}


#endif//__ANG_DOM_XML_IXML_DOCUMENT_H__
