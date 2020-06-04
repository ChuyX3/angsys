/*********************************************************************************************************************/
/*   File Name: xml_node.h                                                                                           */
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
			class xml_node_base
				: public implement<xml_node_base
				, iid("ang::dom::xml::xml_node_base")>
			{
			public:
				xml_node_base(xml_document*, xml_type_t type);

				void dispose()override;
				
				xml_type type()const;
				xml_node_t to_node()const;

				void parent(xml_node*);
				xml_node_t parent()const;
				void document(xml_document*);
				xml_document_t document()const;
				
				void prev_sibling(xml_node*);
				xml_node_t prev_sibling()const;
				void next_sibling(xml_node*);
				xml_node_t next_sibling()const;


				ixml_text_t name()const;
				void name(ixml_text_t);

				ixml_text_t value()const;
				void value(ixml_text_t);

				ixml_list_t children()const;
				ixml_list_t attributes()const;
				void attributes(ixml_list_t);

				xml_node_t first_child()const;
				xml_node_t last_child()const;

			protected:
				xml_type_t m_type;
				ixml_text_t m_name;
				ixml_list_t m_attribs;
				anyone<ixml_text_t, xml_list> m_content;
				weak_ptr<xml_node> m_parent;
				weak_ptr<xml_document> m_document;
				xml_node_t m_prev_sibling;
				xml_node_t m_next_sibling;
			};

			class xml_node
				: public implement<xml_node
				, iid("ang::dom::xml::xml_node")
				, xml_node_base
				, ixml_node
				, ixml_list
				, ixml_items
				, ixml_finder
				, ixml_enum>
			{
			public:
				static xml_node_t create_element(xml_document*, cstr_t);
				static xml_node_t create_element(xml_document*, cstr_t, cstr_t);
				static xml_node_t create_attribute(xml_document*, cstr_t, cstr_t);
				static xml_node_t create_comment(xml_document*, cstr_t);
				static xml_node_t create_cdata(xml_document*, cstr_t);
				static xml_node_t create_attribute_list(xml_document*);

			public:
				xml_node(xml_document* doc, xml_type_t type);

				template<typename T>
				T* to_intf()const;

			 public: /*overrides*/
				virtual void dispose()override;

			public: /*ixml_node override*/
				virtual xml_type_t type()const override;
				virtual bool is_type_of(xml_type_t)const override;
				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level)const override;

				virtual ixml_node_t parent()const override;
				virtual ixml_document_t document()const override;
				virtual ixml_node_t clone(ixml_document_t)const override;

				virtual bool has_name()const override;
				virtual bool has_value()const override;
				virtual bool has_namespace()const override;
				virtual bool has_children()const override;
				virtual bool has_attributes()const override;

				virtual ixml_text_t name()const override;
				virtual ixml_text_t value()const override;
				virtual ixml_text_t xmlns()const override;
				virtual ixml_node_t xmlns(cstr_t)const override;
				virtual ixml_items_t children()const override;
				virtual ixml_attbs_t attributes()const override;

			private: /*ixml_items override*/
				virtual wsize counter()const override;
				virtual ixml_node_t at(xml_base_iterator_t const&) override;
				virtual bool increase(xml_base_iterator_t&)const override;
				virtual bool increase(xml_base_iterator_t&, int offset)const override;
				virtual bool decrease(xml_base_iterator_t&)const override;
				virtual bool decrease(xml_base_iterator_t&, int offset)const override;

				virtual xml_forward_iterator_t begin() override;
				virtual xml_forward_iterator_t end() override;
				virtual xml_const_forward_iterator_t begin()const override;
				virtual xml_const_forward_iterator_t end()const override;
				virtual xml_forward_iterator_t last() override;
				virtual xml_const_forward_iterator_t last()const override;

				virtual xml_backward_iterator_t rbegin() override;
				virtual xml_backward_iterator_t rend() override;
				virtual xml_const_backward_iterator_t rbegin()const override;
				virtual xml_const_backward_iterator_t rend()const override;

				virtual xml_iterator_t find(cstr_t, bool invert)const override;
				virtual xml_iterator_t find(cstr_t, xml_base_iterator_t next_to, bool invert)const override;
				virtual ixml_enum_t find_all(cstr_t)const override;

			private: /*ixml_list override*/
				virtual xml_node_t front()const override;
				virtual xml_node_t back()const override;
				virtual void push_front(xml_node_t) override;
				virtual void push_back(xml_node_t) override;
				virtual xml_iterator_t insert(xml_base_iterator_t it, xml_node_t) override;
				virtual bool pop_front(xml_node_ptr_t = null) override;
				virtual bool pop_back(xml_node_ptr_t = null) override;
				virtual xml_iterator_t remove(xml_base_iterator_t it, xml_node_ptr_t = null) override;
			};
			
			
			template<> inline xml_node_base* xml_node::to_intf()const {
				return static_cast<xml_node_base*>(const_cast<xml_node*>(this));
			}

			template<> inline ixml_node* xml_node::to_intf()const {
				return static_cast<ixml_node*>(const_cast<xml_node*>(this));
			}

			template<> inline ixml_list* xml_node::to_intf()const {
				return is_type_of(xml_type::element) || is_type_of(xml_type::attribute_list) ?
					static_cast<ixml_list*>(const_cast<xml_node*>(this)) : null;
			}

			template<> inline ixml_items* xml_node::to_intf()const {
				return is_type_of(xml_type::element) || is_type_of(xml_type::attribute_list) ?
					static_cast<ixml_items*>(const_cast<xml_node*>(this)) : null;
			}

			template<> inline ixml_enum* xml_node::to_intf()const {
				return is_type_of(xml_type::element) || is_type_of(xml_type::attribute_list) ?
					static_cast<ixml_enum*>(const_cast<xml_node*>(this)) : null;
			}
		}
	}
}
