#pragma once
namespace ang
{
	namespace dom
	{
		namespace xml
		{
			class LINK xml_attribute final
				: public xml_node
			{
			public:
				static xml_attribute_t create_new(xml_document_t, const xml_attribute*);
				static xml_attribute_t create_new(xml_document_t, ixml_text_t, ixml_text_t);

			protected:
				xml_attribute(xml_document_t);

			public:
				ANG_DECLARE_INTERFACE();

				xml_node_t xml_clone(xml_document_t)const override;
				bool xml_has_name()const override;
				bool xml_has_value()const override;

			private:
				using xml_node::xml_has_children;
				using xml_node::xml_has_namespace;
				using xml_node::xml_has_attributes;

				using xml_node::xml_children;
				using xml_node::xml_namespace;
				using xml_node::xml_attributes;

				using xml_node::xml_parent;
				using xml_node::xml_first_child;
				using xml_node::xml_last_child;
				using xml_node::xml_prev_sibling;
				using xml_node::xml_next_sibling;

				virtual~xml_attribute();
				friend xml_document;
			};

			class LINK xml_namespace final
				: public xml_node
			{
			public:
				static xml_namespace_t create_new(xml_document_t, const xml_namespace*);
				static xml_namespace_t create_new(xml_document_t, ixml_text_t, ixml_text_t);

			protected:
				xml_namespace(xml_document_t);

			public:
				ANG_DECLARE_INTERFACE();

				xml_node_t xml_clone(xml_document_t)const override;
				bool xml_has_name()const override;
				bool xml_has_value()const override;

			private:
				using xml_node::xml_has_children;
				using xml_node::xml_has_namespace;
				using xml_node::xml_has_attributes;

				using xml_node::xml_children;
				//using xml_node::xml_namespace;
				using xml_node::xml_attributes;

				using xml_node::xml_parent;
				using xml_node::xml_first_child;
				using xml_node::xml_last_child;
				using xml_node::xml_prev_sibling;
				using xml_node::xml_next_sibling;

				virtual~xml_namespace();
				friend xml_document;
			};

			
		}
	}
}

