#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_IXML_NODE_H__
#define __ANG_DOM_XML_IXML_NODE_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			ang_begin_interface(LINK ixml_node, ixml_object)
				visible vcall ixml_node_t xml_parent()const pure;
				visible vcall ixml_document_t xml_parent_doc()const pure;
				visible vcall ixml_node_t xml_first_child()const pure;
				visible vcall ixml_node_t xml_last_child()const pure;
				visible vcall ixml_node_t xml_prev_sibling()const pure;
				visible vcall ixml_node_t xml_next_sibling()const pure;
				visible vcall ixml_node_t xml_clone(ixml_document_t)const pure;

				visible vcall bool xml_has_name()const pure;
				visible vcall bool xml_has_value()const pure;
				visible vcall bool xml_has_namespace()const pure;
				visible vcall bool xml_has_children()const pure;
				visible vcall bool xml_has_attributes()const pure;

				visible vcall ixml_string_t xml_name()const pure;
				visible vcall ixml_string_t xml_value()const pure;
				visible vcall ixml_node_t xml_namespace()const pure;
				visible vcall ixml_collection_t xml_children()const pure;
				visible vcall xml_attributes_t xml_attributes()const pure;

				visible vcall void xml_parent(ixml_node_t) pure;
				visible vcall void xml_prev_sibling(ixml_node_t) pure;
				visible vcall void xml_next_sibling(ixml_node_t) pure;
				allow ixml_collection;
				allow ixml_document;
			ang_end_interface();
		}
	}

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_node)
		operator dom::xml::ixml_string_t()const;
		dom::xml::ixml_node_t operator[](raw_str_t)const;
		template<typename T, text::encoding E> dom::xml::ixml_node_t operator[](str_view<T, E> str)const;
	ANG_END_INTF_WRAPPER();


}


#endif//__ANG_DOM_XML_IXML_NODE_H__
