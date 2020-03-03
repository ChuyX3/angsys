#ifndef __COFFE_DOM_XML_H__
#error ...
#elif !defined __COFFE_DOM_XML_XML_NODE_H__
#define __COFFE_DOM_XML_XML_NODE_H__

namespace coffe
{
	namespace dom
	{
		namespace xml
		{
			struct nvt LINK ixml_node
				: intf<ixml_node
				, iid("coffe::dom::xml::ixml_node")>
			{
				virtual xml_type_t type()const = 0;
				virtual bool is_type_of(xml_type_t)const = 0;
				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level = 0)const = 0;

				virtual ixml_node_t parent()const = 0;
				virtual ixml_document_t document()const = 0;
				virtual ixml_node_t clone(ixml_document_t)const = 0;

				virtual bool has_name()const = 0;
				virtual bool has_value()const = 0;
				virtual bool has_namespace()const = 0;
				virtual bool has_children()const = 0;
				virtual bool has_attributes()const = 0;

				virtual ixml_text_t name()const = 0;
				virtual ixml_text_t value()const = 0;
				virtual ixml_text_t xmlns()const = 0;
				virtual ixml_node_t xmlns(cstr_t)const = 0;
				virtual ixml_items_t children()const = 0;
				virtual ixml_attbs_t attributes()const = 0;
			};

			struct nvt LINK ixml_streamer
				: intf<ixml_streamer
				, iid("coffe::dom::xml::ixml_streamer")>
			{
				virtual bool load(ixml_node_t) = 0;
				virtual bool save(ixml_document_t)const = 0;
				virtual core::async::iasync<bool> load_async(ixml_node_t) = 0;
				virtual core::async::iasync<bool> save_async(ixml_document_t)const = 0;
			};
		}
	}
}


#endif//__COFFE_DOM_XML_XML_NODE_H__
