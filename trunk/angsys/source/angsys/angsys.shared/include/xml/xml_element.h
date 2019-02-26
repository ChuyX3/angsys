#pragma once

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<xml_encoding E>
			class xml_element final : public xml_node<E>
			{
			public:
				static xml_element_ptr<E> create_new(const xml_element&);
				static xml_element_ptr<E> create_new(xml_document_ptr<E>, const xml_element*);
				static xml_element_ptr<E> create_new(xml_document_ptr<E>, strings::basic_string<E>);
				static xml_element_ptr<E> create_new(xml_document_ptr<E>, strings::basic_string<E>, strings::basic_string<E>);

			protected:
				xml_element(xml_document_ptr<E>);

			public:
				ANG_DECLARE_INTERFACE();

				ixml_node_t xml_clone(ixml_document_t)const override;
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
				friend xml_document<E>;
			};
		}
	}
}

