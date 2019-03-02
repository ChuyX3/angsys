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

			ang_begin_interface(LINK ixml_document, ixml_object, collections::ienum<xml_node>)
				visible vcall ixml_text_view_t xml_version()const pure;
				visible vcall xml_encoding_t xml_encoding()const pure;
				visible vcall bool xml_stand_alone()const pure;
				visible vcall xml_node_t xml_data_type()const pure;
				visible vcall xml_node_t xml_root_element()const pure;
				visible vcall xml_document_t xml_clone()const pure;

				visible vcall xml_iterator_t find(xml_cstr_t, bool invert = false)const pure;
				visible vcall xml_iterator_t find(xml_cstr_t, xml_iterator_t next_to, bool invert = false)const pure;

				visible vcall xml_iterator_t xml_current()const pure;
				visible vcall xml_node_t xml_current_element()const pure;

				visible vcall ixml_text_t create_cdata(xml_cstr_t)const pure;
				visible vcall ixml_text_t create_pcdata(xml_cstr_t)const pure;

				visible vcall bool move_to(xml_iterator_t current) pure;
				visible vcall bool move_to_child(xml_iterator_t child) pure;

				visible vcall bool move_up() pure;
				visible vcall bool move_down() pure;
				visible vcall bool move_forward() pure;
				visible vcall bool move_backward() pure;

				visible vcall void push_header(xml_cstr_t version = "1.0"_s, nullable<bool> standalone = null) pure;
				visible vcall bool begin_element(xml_cstr_t name) pure;
				visible vcall bool end_element() pure;
				visible vcall bool push_element(xml_cstr_t name, xml_cstr_t value) pure;
				visible vcall bool push_element(xml_cstr_t element) pure;
				visible vcall bool push_data(xml_cstr_t value) pure;
				visible vcall bool push_value(xml_cstr_t value) pure;
				visible vcall bool push_attribute(xml_cstr_t name, xml_cstr_t value) pure;
				visible vcall bool push_namespace(xml_cstr_t name, xml_cstr_t value) pure;
				visible vcall bool push_comment(xml_cstr_t value) pure;

				visible vcall void load(core::files::input_text_file_t) pure;
				visible vcall void save(core::files::output_text_file_t)const pure;

				visible vcall void parse(ibuffer_view_t data)pure;
				visible vcall void parse(ixml_text_view_t data)pure;

				template<typename T, text::encoding E> inline void parse(str_view<T, E> const& text) { parse(xml_cstr(text)); }
				template<text::encoding E, template<typename>class A> inline void parse(text::basic_string<E, A> const& text) { parse(text.is_empty() ? raw_str() : raw_str(text->cstr())); }

			ang_end_interface();

			//ixml_document_t xml_create_document(xml_encoding_t, core::files::input_text_file_t = null);
		}
	}

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_document)
	ANG_END_INTF_WRAPPER();
}


#endif//__ANG_DOM_XML_IXML_DOCUMENT_H__
