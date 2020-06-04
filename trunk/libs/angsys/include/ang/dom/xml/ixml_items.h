/*********************************************************************************************************************/
/*   File Name: ang/dom/xml/ixml_items.h                                                                             */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes classes for the use of xml files                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_IXML_ITEMS_H__
#define __ANG_DOM_XML_IXML_ITEMS_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			struct ixml_items
				: intf<ixml_items
				, iid("ang::dom::xml::ixml_items")
				, ixml_finder
				, ixml_enum>
			{
				virtual xml_type_t type()const = 0;
				virtual bool is_type_of(xml_type_t)const = 0;
				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level = 0)const = 0;
			};

			class LINK xml_attribs
			{
			public:
				typedef xml::ixml_enum type;

			private:
				xml::ixml_items* m_ptr;

			public:
				xml_attribs();
				xml_attribs(xml::ixml_items*);
				xml_attribs(xml_attribs &&);
				xml_attribs(xml_attribs const&);
				xml_attribs(std::nullptr_t const&);
				~xml_attribs();

			public:
				void reset();
				void reset_unsafe();
				bool is_empty()const;
				xml::ixml_items* get(void)const;
				void set(xml::ixml_items*);
				xml::ixml_items ** addres_of(void);

				xml::xml_forward_iterator_t begin() {
					return m_ptr ? m_ptr->begin() : xml::xml_forward_iterator_t(null);
				}
				xml::xml_forward_iterator_t end() {
					return m_ptr ? m_ptr->end() : xml::xml_forward_iterator_t(null);
				}

				xml::xml_const_forward_iterator_t begin()const {
					return m_ptr ? ((xml::ixml_items const*)m_ptr)->begin() : xml::xml_const_forward_iterator_t(null);
				}
				xml::xml_const_forward_iterator_t end()const {
					return m_ptr ? ((xml::ixml_items const*)m_ptr)->end() : xml::xml_const_forward_iterator_t(null);
				}

			public:
				ixml_attbs_t& operator = (xml::ixml_items*);
				ixml_attbs_t& operator = (nullptr_t const&);
				ixml_attbs_t& operator = (ixml_attbs_t &&);
				ixml_attbs_t& operator = (ixml_attbs_t const&);

				//intf_wrapper_ptr<xml::ixml_items> operator & (void);
				xml::ixml_items * operator -> (void);
				xml::ixml_items const* operator -> (void)const;

				operator xml::ixml_items * (void);
				operator xml::ixml_items const* (void)const;

				ixml_text_t operator[](cstr_t)const;
			};
		
		}
	}

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_enum)
		dom::xml::xml_forward_iterator_t begin();
		dom::xml::xml_forward_iterator_t end();
		dom::xml::xml_const_forward_iterator_t begin()const;
		dom::xml::xml_const_forward_iterator_t end()const;

		dom::xml::xml_backward_iterator_t rbegin();
		dom::xml::xml_backward_iterator_t rend();
		dom::xml::xml_const_backward_iterator_t rbegin()const;
		dom::xml::xml_const_backward_iterator_t rend()const;
	ANG_END_INTF_WRAPPER();

	ANG_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_items)
		dom::xml::xml_forward_iterator_t begin();
		dom::xml::xml_forward_iterator_t end();
		dom::xml::xml_const_forward_iterator_t begin()const;
		dom::xml::xml_const_forward_iterator_t end()const;

		dom::xml::xml_backward_iterator_t rbegin();
		dom::xml::xml_backward_iterator_t rend();
		dom::xml::xml_const_backward_iterator_t rbegin()const;
		dom::xml::xml_const_backward_iterator_t rend()const;
		dom::xml::ixml_node_t operator[](cstr_t cstr)const;
	ANG_END_INTF_WRAPPER();

}

#endif//__ANG_DOM_XML_IXML_ITEMS_H__
