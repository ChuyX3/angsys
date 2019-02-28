#pragma once
namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<xml_encoding E>
			class xml_attribute final : public xml_node<E>
			{
			public:
				static xml_attribute_ptr<E> create_new(const xml_attribute&);
				static xml_attribute_ptr<E> create_new(xml_document_ptr<E>, const xml_attribute*);
				static xml_attribute_ptr<E> create_new(xml_document_ptr<E>, strings::basic_string<E>, strings::basic_string<E>);

			protected:
				xml_attribute(xml_document_ptr<E>);

			public:
				ANG_DECLARE_INTERFACE();

				ixml_node_t xml_clone(ixml_document_t)const override;
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

			template<xml_encoding E>
			class xml_namespace final : public xml_node<E>
			{
			public:
				static xml_namespace_ptr<E> create_new(const xml_namespace&);
				static xml_namespace_ptr<E> create_new(xml_document_ptr<E>, const xml_namespace*);
				static xml_namespace_ptr<E> create_new(xml_document_ptr<E>, strings::basic_string<E>, strings::basic_string<E>);

			protected:
				xml_namespace(xml_document_ptr<E>);

			public:
				ANG_DECLARE_INTERFACE();

				ixml_node_t xml_clone(ixml_document_t)const override;
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
				friend xml_document<E>;
			};

			
		}
	}
}

