#pragma once

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<xml_encoding E>
			class xml_header final : public xml_node<E>, public ixml_header
			{
			public:
				static xml_header_ptr<E> create_new(const xml_header&);
				static xml_header_ptr<E> create_new(xml_document_ptr<E>, const xml_header*);
				static xml_header_ptr<E> create_new(xml_document_ptr<E>, strings::basic_string<E> version = "1.0"_s, bool standalone = true);

			protected:
				xml_header(xml_document_ptr<E>);

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				ixml_node_t xml_clone(ixml_document_t)const override;

			public:
				ixml_text_t xml_version()const;
				xml_encoding_t xml_encoding()const;
				bool xml_stand_alone()const;

				void version(strings::basic_string<E>);
				void encoding(xml_encoding_t);
				void is_stand_alone(bool);

				void encoding(strings::basic_string<E>);
				void is_stand_alone(strings::basic_string<E>);

			private:
				using xml_node::xml_has_name;
				using xml_node::xml_has_value;
				using xml_node::xml_has_children;
				using xml_node::xml_has_attributes;

				using xml_node::xml_name;
				using xml_node::xml_value;
				using xml_node::xml_children;

				using xml_node::xml_parent;
				using xml_node::xml_first_child;
				using xml_node::xml_last_child;
				using xml_node::xml_prev_sibling;
				using xml_node::xml_next_sibling;

				virtual~xml_header();
				friend xml_document;
			};
		}
	}
}