#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_XML_TEXT_H__
#define __ANG_DOM_XML_XML_TEXT_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<xml_encoding E>
			class xml_cdata final : public xml_node<E>
			{
			public:
				static xml_cdata_t create_new(const xml_cdata&);
				static xml_cdata_t create_new(xml_document_ptr<E>, const xml_cdata*);
				static xml_cdata_t create_new(xml_document_ptr<E>, strings::basic_string<E>);

			protected:
				xml_cdata(xml_document_ptr<E>);

			public:
				ANG_DECLARE_INTERFACE();

				ixml_node_t xml_clone(ixml_document_t)const override;
				bool xml_has_value()const override;

			private:
				using xml_node::xml_has_name;
				using xml_node::xml_has_children;
				using xml_node::xml_has_attributes;

				using xml_node::xml_children;
				using xml_node::xml_attributes;

				using xml_node::xml_parent;
				using xml_node::xml_first_child;
				using xml_node::xml_last_child;
				using xml_node::xml_prev_sibling;
				using xml_node::xml_next_sibling;

				virtual~xml_cdata();
				friend xml_document;
			};
		}
	}
}


#endif//__ANG_DOM_XML_XML_TEXT_H__
