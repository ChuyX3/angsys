#pragma once 


namespace ang
{
	
	namespace dom
	{
		namespace xml
		{

			class LINK xml_collection
				: public smart<xml_collection, ixml_collection>
			{
			public:
				typedef xml_collection self_t;

			protected:
				friend xml_node;
				const xml_type_t m_type;
				wsize m_count;
				xml_node* m_first;
				xml_node* m_last;
				mutable weak_ptr<xml_node> m_parent;

			public:
				typedef ixml_node* node_ptr_t;
				typedef collections::ienum<ixml_node> ienum_t;

				xml_collection(xml_node_t, xml_type_t);

			protected:
				virtual~xml_collection();

			public:
				ANG_DECLARE_INTERFACE();
				virtual void clear();

				xml_type_t xml_type()const override;
				bool xml_is_type_of(xml_type_t)const override;
				streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;
				xml_node_t xml_parent()const;
				xml_document_t xml_parent_doc()const;

			public: //ienum overrides
				wsize counter()const override;

				xml::xml_node_t at(xml_base_iterator_t const&) override;
				bool increase(xml_base_iterator_t&)const override;
				bool increase(xml_base_iterator_t&, int offset)const override;
				bool decrease(xml_base_iterator_t&)const override;
				bool decrease(xml_base_iterator_t&, int offset)const override;

				xml_forward_iterator_t begin() override;
				xml_forward_iterator_t end() override;
				xml_const_forward_iterator_t begin()const override;
				xml_const_forward_iterator_t end()const override;
				xml_forward_iterator_t last() override;
				xml_const_forward_iterator_t last()const override;

				xml_backward_iterator_t rbegin() override;
				xml_backward_iterator_t rend() override;
				xml_const_backward_iterator_t rbegin()const override;
				xml_const_backward_iterator_t rend()const override;

			public: //icollection overrides
				xml_iterator_t find(xml_cstr_t, bool invert = false)const override;
				xml_iterator_t find(xml_cstr_t, xml_iterator_t next_to, bool invert = false)const override;

			public:
				virtual bool push(xml_node_t node, bool last = true);
				virtual bool insert(xml_node_t node, xml_iterator_t at, bool nextTo = true);
				virtual bool pop(xml_node_ptr_t out = null, bool last = true);
				virtual bool pop_at(xml_iterator_t it, xml_node_ptr_t out = null);

			protected:
				xml_node* xml_first()const;
				xml_node* xml_last()const;
				void xml_first(xml_node*);
				void xml_last(xml_node*);
			};

		
			class LINK xml_node 
				: public smart<xml_node, ixml_node>
			{
			public:
				friend xml_document;
				friend xml_collection;
				
			private:
				const xml_type_t m_type;
				mutable weak_ptr<xml_node> m_parent;
				mutable weak_ptr<xml_document> m_doc;

				xml_node* m_prev;
				xml_node* m_next;

			protected:
				xml_text_t m_name;
				xml_text_t m_namespace;
				ixml_object_t m_content;
				xml_attributes_t m_attributes; //attributes and namespaces 
				collections::hash_map<ixml_text_view_t, weak_ptr<xml_node>> m_ns_map;
				//object_wrapper<xml_collection> m_children;

			protected:
				xml_node(xml_document_t, xml_type_t);
				virtual~xml_node();

			public:
				xml_node_t xml_parent()const override;
				xml_document_t xml_parent_doc()const override;
				xml_node_t xml_first_child()const override;
				xml_node_t xml_last_child()const override;
				xml_node_t xml_prev_sibling()const override;
				xml_node_t xml_next_sibling()const override;

			protected:
				void xml_parent(xml_node_t) override;
				void xml_prev_sibling(xml_node_t) override;
				void xml_next_sibling(xml_node_t) override;

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				virtual void clear();

				xml_type_t xml_type()const override;
				bool xml_is_type_of(xml_type_t)const override;
				streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

				virtual bool xml_has_name()const override;
				virtual bool xml_has_value()const override;
				virtual bool xml_has_namespace()const override;
				virtual bool xml_has_children()const override;
				virtual bool xml_has_attributes()const override;

				virtual xml_text_t xml_name()const override;
				virtual xml_text_t xml_value()const override;
				virtual xml_text_t xml_namespace()const override;
				virtual xml_node_t xml_namespace(xml_cstr_t)const override;
				virtual ixml_collection_t xml_children()const override;
				virtual xml_attributes_t xml_attributes()const override;

			protected:
				bool push_name(xml_cstr_t);
				bool push_data(xml_cstr_t);
				bool push_value(xml_cstr_t);
				bool push_namespace(xml_cstr_t);
				

				bool push_name(ixml_text_t);
				bool push_data(ixml_text_t);
				bool push_value(ixml_text_t);
				bool push_namespace(ixml_text_t);
				bool push_namespace(xml_namespace_t);
				xml_iterator_t push_attribute(xml_node_t att);
				xml_iterator_t push_attribute(xml_node_t att, xml_iterator_t next);
				bool push_attributes(ixml_collection_t attributes);
				xml_iterator_t push_child(xml_node_t element);
				xml_iterator_t push_child(xml_node_t element, xml_iterator_t next);
				bool push_children(ixml_collection_t children);

			};
		}
	}
}

