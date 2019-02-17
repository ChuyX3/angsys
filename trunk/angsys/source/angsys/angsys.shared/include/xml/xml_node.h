#pragma once 
namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<xml_encoding E>
			class xml_node : public object, public ixml_node
			{
			public:
				friend xml_document;

				class xml_collection
					: public object
					, public ixml_collection
				{
				protected:
					friend xml_node;
					const xml_type_t _xml_type;
					wsize _count;
					xml_node* _xml_first;
					xml_node* _xml_last;
					mutable weak_ptr<ixml_node> _xml_parent;

				public:
					typedef ixml_node* node_ptr_t;
					typedef collections::ienum<ixml_node> ienum_t;

				protected:
					xml_collection(xml_node_ptr<E>, xml_type_t);
					virtual~xml_collection();

				public:
					ANG_DECLARE_INTERFACE();
					virtual void clean();

					xml_type_t xml_type()const override;
					bool xml_is_type_of(xml_type_t)const override;
					streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;
					ixml_node_t xml_parent()const;
					ixml_document_t xml_parent_doc()const;

				public: //ienum overrides
					wsize counter()const override;

					xml::ixml_node_t at(xml_base_iterator_t const&) override;
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
					xml_iterator_t find(raw_str_t, bool invert = false)const override;
					xml_iterator_t find(raw_str_t, xml_iterator_t next_to, bool invert = false)const override;

				public:
					virtual bool push(ixml_node_t node, bool last = true);
					virtual bool insert(ixml_node_t node, xml_iterator_t at, bool nextTo = true);
					virtual bool pop(ixml_node_ptr_t out = null, bool last = true);
					virtual bool pop_at(xml_iterator_t it, xml::ixml_node_ptr_t out = null);

				protected:
					ixml_node* xml_first()const;
					ixml_node* xml_last()const;
					void xml_first(ixml_node*);
					void xml_last(ixml_node*);
				};

			private:
				const xml_type_t _xml_type;
				mutable weak_ptr<ixml_node> _xml_parent;
				mutable weak_ptr<ixml_document> _xml_parent_doc;

				xml_node* _xml_prev;
				xml_node* _xml_next;

			protected:
				strings::basic_string<E> _xml_name;
				strings::basic_string<E> _xml_value;
				xml_namespace_ptr<E> _xml_namespace;
				ixml_collection_t _xml_attributes; //attributes and namespaces 
				object_wrapper<xml_collection> _xml_children;

			protected:
				xml_node(ixml_document_t, xml_type_t);
				virtual~xml_node();

			public:
				ixml_node_t xml_parent()const override;
				ixml_document_t xml_parent_doc()const override;
				ixml_node_t xml_first_child()const override;
				ixml_node_t xml_last_child()const override;
				ixml_node_t xml_prev_sibling()const override;
				ixml_node_t xml_next_sibling()const override;

			protected:
				void xml_parent(ixml_node_t) override;
				void xml_prev_sibling(ixml_node_t) override;
				void xml_next_sibling(ixml_node_t) override;

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				virtual void clean();

				xml_type_t xml_type()const override;
				bool xml_is_type_of(xml_type_t)const override;
				streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

				virtual bool xml_has_name()const override;
				virtual bool xml_has_value()const override;
				virtual bool xml_has_namespace()const override;
				virtual bool xml_has_children()const override;
				virtual bool xml_has_attributes()const override;

				virtual ixml_text_t xml_name()const override;
				virtual ixml_text_t xml_value()const override;
				virtual xml_namespace_ptr<E> xml_namespace()const override;
				virtual ixml_collection_t xml_children()const override;
				virtual xml_attributes_t xml_attributes()const override;

			protected:
				bool push_name(wstring value);
				bool push_data(wstring);
				bool push_value(wstring);
				xml_iterator_t push_attribute(xml_attribute_ptr<E> att);
				xml_iterator_t push_attribute(xml_attribute_ptr<E> att, xml_iterator_t next);
				bool push_attributes(ixml_collection_t attributes);
				xml_iterator_t push_child(xml_node_ptr<E> element);
				xml_iterator_t push_child(xml_node_ptr<E> element, xml_iterator_t next);
				bool push_children(ixml_collection_t children);

			};
		}
	}
}

