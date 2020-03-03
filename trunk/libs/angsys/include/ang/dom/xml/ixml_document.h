#ifndef __COFFE_DOM_XML_H__
#error ...
#elif !defined __COFFE_DOM_XML_IXML_DOCUMENT_H__
#define __COFFE_DOM_XML_IXML_DOCUMENT_H__

namespace coffe
{
	namespace dom
	{
		namespace xml
		{

			struct nvt LINK ixml_document_builder
				: intf<ixml_document_builder
				, iid("coffe::dom::xml::ixml_document_builder")>
			{
				virtual xml_iterator_t current()const = 0;
				virtual ixml_node_t current_element()const = 0;

				virtual bool move_to(xml_iterator_t current) = 0;
				virtual bool move_to_child(xml_iterator_t child) = 0;

				virtual bool move_up() = 0;
				virtual bool move_down() = 0;
				virtual bool move_forward() = 0;
				virtual bool move_backward() = 0;

				virtual ixml_text_t create_cdata(cstr_t)const = 0;
				virtual ixml_text_t create_pcdata(cstr_t)const = 0;

				virtual void push_header(cstr_t version = "1.0"_sv, nullable<bool> standalone = null) = 0;
				virtual bool begin_element(cstr_t name) = 0;
				virtual bool end_element() = 0;
				virtual bool push_element(cstr_t name, cstr_t value) = 0;
				virtual bool push_element(cstr_t element) = 0;
				virtual bool push_data(cstr_t value) = 0;
				virtual bool push_value(cstr_t value) = 0;
				virtual bool push_attribute(cstr_t name, cstr_t value) = 0;
				virtual bool push_namespace(cstr_t name, cstr_t value) = 0;
				virtual bool push_comment(cstr_t value) = 0;
			};

			struct nvt LINK ixml_document 
				: intf<ixml_document
				, iid("coffe::dom::xml::ixml_document")>
			{
				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level = 0)const = 0;

				virtual string version()const = 0;
				virtual xml_encoding_t encoding()const = 0;
				virtual bool stand_alone()const = 0;
				virtual ixml_node_t data_type()const = 0;
				virtual ixml_items_t elements()const = 0;
				virtual ixml_node_t root_element()const = 0;
				virtual ixml_document_t clone()const = 0;
				
				virtual void clear() = 0;
				virtual ixml_document_builder_t builder() = 0;
				virtual error load(core::files::input_text_file_t) = 0;
				virtual void save(core::files::output_text_file_t)const = 0;
				virtual error parse(cstr_t data) = 0;
				virtual error parse(string data) = 0;
			};

			LINK optional<ixml_document> xml_document_from_file(core::files::input_text_file_t);
		}
	}


}


#endif//__COFFE_DOM_XML_IXML_DOCUMENT_H__
