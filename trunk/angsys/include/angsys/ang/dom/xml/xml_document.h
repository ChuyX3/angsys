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
			
			class LINK xml_document final
				: public smart<xml_document, ixml_document>
			{
			public:
				static xml_document_t from_file(core::files::input_text_file_t);

			private:
				class xml_document_iterator
				{
				protected:
					mutable weak_ptr<xml_document> m_doc;
					xml_node_t	m_current;
					xml_node_t	m_child;

				public:
					xml_document_iterator();
					xml_document_iterator(xml_document*);
					xml_document_iterator(const xml_document_iterator&);
					~xml_document_iterator();

				private:
					xml_node_t xml_current()const;
					xml_node_t xml_child()const;
					void xml_current(xml_node_t);
					void xml_child(xml_node_t);

				public: //Properties
					void xml_parent(xml_document_t);
					xml_document_t xml_parent()const;

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

				nullable<bool> m_stand_alone;
				string m_version;
				xml_encoding_t m_encoding;
				text::istring_factory_t m_factory;
				wsize m_count;
				xml_node* m_first;
				xml_node* m_root;
				xml_node* m_last;
				xml_document_iterator m_current;

				astring m_last_parsing_error;

			public:
				xml_document(xml_encoding_t = xml_encoding::utf8);
				xml_document(xml_document const*);

			protected:

				virtual~xml_document();

			protected: //overrides
				virtual void dispose()override;
			public:
				ANG_DECLARE_INTERFACE();

				bool is_empty()const;

				virtual void clear()override;

				virtual xml_type_t xml_type()const override;
				virtual bool xml_is_type_of(xml_type_t)const override;
				virtual streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag = xml_format::wrap_text_space + xml_format::fix_entity, ushort level = 0)const override;

				virtual wsize counter()const override;
				virtual xml::xml_node_t at(xml_base_iterator_t const&) override;
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

				virtual string xml_version()const override;
				virtual xml_encoding_t xml_encoding()const override;
				virtual bool xml_stand_alone()const override;

				virtual xml_node_t xml_data_type()const override;
				virtual xml_node_t xml_root_element()const override;
				virtual xml_document_t xml_clone()const override;

				virtual xml_iterator_t find(cstr_t, bool invert = false)const override;
				virtual xml_iterator_t find(cstr_t, xml_iterator_t next_to, bool invert = false)const override;

				virtual xml_iterator_t xml_current()const override;
				virtual xml_node_t xml_current_element()const override;

				virtual string create_cdata(cstr_t)const;
				virtual string create_pcdata(cstr_t)const;

				virtual bool move_to(xml_iterator_t current) override;
				virtual bool move_to_child(xml_iterator_t child) override;

				virtual bool move_up() override;
				virtual bool move_down() override;
				virtual bool move_forward() override;
				virtual bool move_backward() override;

				virtual void push_header(cstr_t version = "1.0"_s, nullable<bool> standalone = null) override;
				virtual bool begin_element(cstr_t name) override;
				virtual bool end_element() override;
				virtual bool push_element(cstr_t name, cstr_t value) override;
				virtual bool push_element(cstr_t element) override;
				virtual bool push_data(cstr_t value) override;
				virtual bool push_value(cstr_t value) override;
				virtual bool push_attribute(cstr_t name, cstr_t value) override;
				virtual bool push_namespace(cstr_t name, cstr_t value) override;
				virtual bool push_comment(cstr_t value) override;

				virtual void load(core::files::input_text_file_t) override;
				virtual void save(core::files::output_text_file_t)const override;
				//virtual void parse(cstr_t data)override;
				virtual void parse(ibuffer_view_t data)override;
				virtual void parse(text::istring_view_t data)override;
				using ixml_document::parse;

			private:
				bool begin_element(string name);
				bool push_element(string name, string value);
				bool push_element(string element);
				bool push_data(string value);
				bool push_value(string value);
				bool push_attribute(string name, string value);
				bool push_comment(string value);

			protected:
				static bool decode_header(text::istring_view_t data, windex& idx, string& version, xml_encoding_t& encoding, nullable<bool>& standalone);
				static bool decode_dtd(xml_document_t doc, text::istring_view_t data, windex& idx);
				static bool decode_elements(xml_document_t doc, text::istring_view_t data, windex& idx);

				xml_node* xml_first()const;
				xml_node* xml_root()const;
				xml_node* xml_last()const;

				void insert_next_to(xml_node*, xml_node*);
				void insert_prev_to(xml_node*, xml_node*);
				void insert_first(xml_node*);
				void insert_last(xml_node*);
				void xml_first(xml_node_t);
				void xml_root(xml_node_t);
				void xml_last(xml_node_t);
			};
		}
	}
}


#endif//__ANG_DOM_XML_XML_DOCUMENT_H__
