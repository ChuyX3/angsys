#pragma once

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			class LINK xml_element final
				: public smart<xml_element, xml_node>
			{
			public:;
				static xml_element_t create_new(xml_document_t, const xml_element*);
				static xml_element_t create_new(xml_document_t, string);
				static xml_element_t create_new(xml_document_t, string, string);

			protected:
				xml_element(xml_document_t);

			public:
				ANG_DECLARE_INTERFACE();

				xml_node_t xml_clone(xml_document_t)const override;
				bool xml_has_name()const override;
				bool xml_has_value()const override;
				bool xml_has_namespace()const override;
				bool xml_has_children()const override;
				bool xml_has_attributes()const override;

			private:
				using xml_node::xml_parent;
				using xml_node::xml_prev_sibling;
				using xml_node::xml_next_sibling;

				virtual~xml_element();
				friend xml_document;
			};
		}
	}
}

