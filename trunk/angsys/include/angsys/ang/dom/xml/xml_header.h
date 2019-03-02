#pragma once

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			class LINK xml_header final
				: public xml_node
				, public ixml_header
			{
			public:
				static xml_header_t create_new(xml_document_t);
				static xml_header_t create_new(xml_document_t, const xml_header*);
				static xml_header_t create_new(xml_document_t, xml_cstr_t version, bool standalone = true);

			protected:
				xml_header(xml_document_t);

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				xml_node_t xml_clone(xml_document_t)const override;

			public:
				ixml_text_view_t xml_version()const;
				xml_encoding_t xml_encoding()const;
				bool xml_stand_alone()const;

				void version(ixml_text_t);
				void encoding(xml_encoding_t);
				void is_stand_alone(bool);

				void encoding(ixml_text_t);
				void is_stand_alone(ixml_text_t);

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