/*********************************************************************************************************************/
/*   File Name: xml_document.h                                                                                       */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes classes for the use of xml files                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#pragma once
namespace ang
{
	namespace dom
	{
		namespace xml
		{
			class xml_document_iterator
			{
			protected:
				xml_document* m_doc;
				xml_node_t	m_current;
				xml_node_t	m_child;

			public:
				xml_document_iterator();
				xml_document_iterator(xml_document*);
				xml_document_iterator(const xml_document_iterator&);
				~xml_document_iterator();

			private:
				xml_node_t current_node()const;
				xml_node_t child_node()const;
				void current_node(xml_node_t);
				void child_node(xml_node_t);

			public: //Properties
				void document(xml_document*);
				xml_document* document()const;
				xml_iterator_t current()const;
				xml_iterator_t child()const;

			public: //Methods
				void reset();
				bool begin_child();
				bool end_child();
				bool next();
				bool prev();
				bool next_child();
				bool prev_child();

				bool backward();
				bool forward();

				friend xml_document;
			};

			class xml_document
				: public implement<xml_document
				, iid("ang::dom::xml_document")
				, ixml_document
				, ixml_document_builder
				, ixml_list
				, ixml_items
				, ixml_finder
				, ixml_enum>
			{
			private:
				friend  xml_document_iterator;
				enum stand_alone_t {
					stand_alone_none = -1,
					stand_alone_false = 0,
					stand_alone_true = 1,
				};

				stand_alone_t m_stand_alone;
				ixml_text_t m_version;
				xml_encoding_t m_encoding;
				wsize m_count;
				xml_list m_items;
				xml_node_t m_root;
				xml_document_iterator m_cursor;

			public:
				xml_document(xml_encoding_t = xml_encoding::auto_detect);
				xml_document(xml_document const*);

				template<typename T> T* to_intf()const {
					return static_cast<T*>(const_cast<xml_document*>(this));
				}

			public: /*overrides*/
				virtual void dispose()override;

			public:
				static xml_document_t from_file(core::files::input_text_file_t);

			public: /*ixml_document override*/
				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level)const override;
				virtual string version()const override;
				virtual xml_encoding_t encoding()const override;
				virtual bool stand_alone()const override;
				virtual ixml_node_t data_type()const override;
				virtual ixml_items_t elements()const override;
				virtual ixml_node_t root_element()const override;
				virtual ixml_document_t clone()const override;
				virtual ixml_document_builder_t builder()override;
				virtual void clear() override;

			public: /*ixml_document_builder overrides*/
				virtual xml_iterator_t current()const override;
				virtual ixml_node_t current_element()const override;

				virtual bool move_to(xml_iterator_t current) override;
				virtual bool move_to_child(xml_iterator_t child) override;

				virtual bool move_up() override;
				virtual bool move_down() override;
				virtual bool move_forward() override;
				virtual bool move_backward() override;

				virtual ixml_text_t create_cdata(cstr_t)const override;
				virtual ixml_text_t create_pcdata(cstr_t)const override;

				virtual void push_header(xml_encoding_t, cstr_t version = "1.0"_sv, nullable<bool> standalone = null) override;
				virtual bool begin_element(cstr_t name) override;
				virtual bool end_element() override;
				virtual bool push_element(cstr_t name, cstr_t value) override;
				virtual bool push_element(cstr_t element) override;
				virtual bool push_data(cstr_t value) override;
				virtual bool push_value(cstr_t value) override;
				virtual bool push_attribute(cstr_t name, cstr_t value) override;
				virtual bool push_namespace(cstr_t name, cstr_t value) override;
				virtual bool push_comment(cstr_t value) override;

				virtual error load(core::files::input_text_file_t) override;
				virtual void save(core::files::output_text_file_t)const override;

				virtual error parse(cstr_t data) override;
				virtual error parse(string data) override;

			private: /*ixml_enum*/
				virtual wsize counter()const  override;
				virtual ixml_node_t at(xml_base_iterator_t const&)  override;
				virtual bool increase(xml_base_iterator_t&)const  override;
				virtual bool increase(xml_base_iterator_t&, int offset)const  override;
				virtual bool decrease(xml_base_iterator_t&)const  override;
				virtual bool decrease(xml_base_iterator_t&, int offset)const  override;

				virtual xml_forward_iterator_t begin()  override;
				virtual xml_forward_iterator_t end()  override;
				virtual xml_const_forward_iterator_t begin()const  override;
				virtual xml_const_forward_iterator_t end()const  override;
				virtual xml_forward_iterator_t last()  override;
				virtual xml_const_forward_iterator_t last()const  override;

				virtual xml_backward_iterator_t rbegin()  override;
				virtual xml_backward_iterator_t rend()  override;
				virtual xml_const_backward_iterator_t rbegin()const  override;
				virtual xml_const_backward_iterator_t rend()const  override;

			private: /*ixml_finder overrides*/
				virtual xml_iterator_t find(cstr_t, bool invert = false)const  override;
				virtual xml_iterator_t find(cstr_t , xml_base_iterator_t next_to, bool invert = false)const  override;
				virtual ixml_enum_t find_all(cstr_t)const  override;

			private: /*ixml_items overrides*/
				virtual xml_type_t type()const override;
				virtual bool is_type_of(xml_type_t)const override;

			private: /*ixml_list overrides*/
				virtual xml_node_t front()const override;
				virtual xml_node_t back()const override;
				virtual void push_front(xml_node_t) override;
				virtual void push_back(xml_node_t) override;
				virtual xml_iterator_t insert(xml_base_iterator_t it, xml_node_t) override;
				virtual bool pop_front(xml_node_ptr_t = null) override;
				virtual bool pop_back(xml_node_ptr_t = null) override;
				virtual xml_iterator_t remove(xml_base_iterator_t it, xml_node_ptr_t = null) override;
			};
		}
	}
}
