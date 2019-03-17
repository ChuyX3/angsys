#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_XML_CDATA_H__
#define __ANG_DOM_XML_XML_CDATA_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{

			class LINK xml_cdata final
				: public smart<xml_cdata, xml_node>
			{
			public:
				static xml_cdata_t create_new(const xml_cdata&);
				static xml_cdata_t create_new(xml_document_t, const xml_cdata*);
				static xml_cdata_t create_new(xml_document_t, string);

			public:
				xml_cdata(xml_document_t);

			public:
				ANG_DECLARE_INTERFACE();

				xml_node_t xml_clone(xml_document_t)const override;
				bool xml_has_name()const override;
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


#endif//__ANG_DOM_XML_XML_CDATA_H__
