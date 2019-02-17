#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_XML_DOCUMENT_H__
#define __ANG_DOM_XML_XML_DOCUMENT_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			template<xml_encoding E>
			class xml_document final: public object, public ixml_document
			{
			public:
				static xml_document_ptr<E> from_file(core::files::input_text_file_t);

			private:
				class xml_document_iterator
				{
				protected:
					mutable weak_ptr<xml_document> _xml_doc;
					xml_node_ptr<E>	_xml_current;
					xml_node_ptr<E>	_xml_child;

				public:
					xml_document_iterator();
					xml_document_iterator(xml_document*);
					xml_document_iterator(const xml_document_iterator&);
					~xml_document_iterator();

				private:
					xml_node_ptr<E> xml_current()const;
					xml_node_ptr<E> xml_child()const;
					void xml_current(xml_node_ptr<E>);
					void xml_child(xml_node_ptr<E>);

				public: //Properties
					void xml_parent(xml_document_ptr<E>);
					xml_document_ptr<E> xml_parent()const;

					xml_iterator_t current()const;
					xml_iterator_t child()const;

				public: //Methods
					bool begin_child();
					bool end_child();
					bool next();
					bool prev();
					bool next_child();
					bool prev_Child();

					bool backward();
					bool forward();

					friend xml_document;
				};

				wsize _count;
				xml_node* _xml_first;
				xml_node* _xml_root;
				xml_node* _xml_last;
				xml_document_iterator _current;
				string _last_parsing_error;

			public:
				xml_document();
				xml_document(xml_document const*);

			protected:

				virtual~xml_document();

			public:
				ANG_DECLARE_INTERFACE();

				virtual xml_type_t xml_type()const override;
				virtual bool xml_is_type_of(xml_type_t)const override;
				virtual streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const override;

				virtual wsize counter()const override;
				virtual xml::ixml_node_t at(xml_base_iterator_t const&) override;
				virtual bool increase(xml_base_iterator_t&)const override;
				virtual bool increase(xml_base_iterator_t&, int offset)const override;
				virtual bool decrease(xml_base_iterator_t&)const override;
				virtual bool decrease(xml_base_iterator_t&, int offset)const override;

				virtual xml_forward_iterator_t begin() override;
				virtual xml_forward_iterator_t end() override;
				virtual xml_const_forward_iterator_t begin()const override;
				virtual xml_const_forward_iterator_t end()const override;
				inline xml_forward_iterator_t last() override;
				inline xml_const_forward_iterator_t last()const override;

				virtual xml_backward_iterator_t rbegin() override;
				virtual xml_backward_iterator_t rend() override;
				virtual xml_const_backward_iterator_t rbegin()const override;
				virtual xml_const_backward_iterator_t rend()const override;

				virtual ixml_header_t xml_header()const override;
				virtual ixml_node_t xml_data_type()const override;
				virtual ixml_node_t xml_root_element()const override;
				virtual ixml_document_t xml_clone()const override;

				virtual xml_iterator_t find(raw_str_t, bool invert = false)const override;
				virtual xml_iterator_t find(raw_str_t, xml_iterator_t next_to, bool invert = false)const override;

				virtual xml_iterator_t xml_current()const override;
				virtual ixml_node_t xml_current_element()const override;

				virtual bool move_to(xml_iterator_t current) override;
				virtual bool move_to_child(xml_iterator_t child) override;

				virtual bool move_up() override;
				virtual bool move_down() override;
				virtual bool move_forward() override;
				virtual bool move_backward() override;

				virtual void push_default_header() override;
				virtual void push_header(xml_document_ptr<E> version, xml_encoding_t encoding, bool standalone) override;
				virtual bool begin_element(xml_document_ptr<E> name) override;
				virtual bool end_element() override;
				virtual bool push_element(xml_document_ptr<E> name, xml_document_ptr<E> value) override;
				virtual bool push_element(xml_document_ptr<E> element) override;
				virtual bool push_data(xml_document_ptr<E> value) override;
				virtual bool push_value(xml_document_ptr<E> value) override;
				virtual bool push_attribute(xml_document_ptr<E> name, xml_document_ptr<E> value) override;
				virtual bool push_comment(xml_document_ptr<E> value) override;

				virtual void load(core::files::input_text_file_t) override;
				virtual void save(core::files::output_text_file_t)const override;
				virtual void parse(raw_str_t text)override;
				using ixml_document::parse;

			public: //Properties
				bool is_empty()const;
				void clean();

			protected:
				static bool decode_header(raw_str_t& text, windex& idx, xml_document_ptr<E>& version, xml_encoding_t& encoding, bool& standalone);
				static bool decode_dtd(xml_document_ptr<E> doc, raw_str_t& text, windex& idx);
				static bool decode_elements(xml_document_ptr<E> doc, raw_str_t& text, windex& idx);

				ixml_node* xml_first()const;
				ixml_node* xml_root()const;
				ixml_node* xml_last()const;

				void insert_next_to(ixml_node*, ixml_node*);
				void insert_prev_to(ixml_node*, ixml_node*);
				void insert_first(ixml_node*);
				void insert_last(ixml_node*);
				void xml_first(ixml_node_t);
				void xml_root(ixml_node_t);
				void xml_last(ixml_node_t);
			};
		}
	}
}


#endif//__ANG_DOM_XML_XML_DOCUMENT_H__
