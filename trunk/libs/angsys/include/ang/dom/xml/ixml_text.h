/*********************************************************************************************************************/
/*   File Name: ang/dom/xml/ixml_text.h                                                                              */
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
#elif !defined __ANG_DOM_XML_IXML_TEXT_H__
#define __ANG_DOM_XML_IXML_TEXT_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			struct nvt LINK ixml_text
				: intf<ixml_text
				, iid("ang::dom::xml::ixml_text")
				, text::istring>
			{
				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level = 0)const = 0;
				template<typename T> T as()const;
			};
		}
	}

	namespace text
	{
		template<typename T, text::encoding E>
		struct str_view_compare_helper<str_view<T, E>, dom::xml::ixml_text_t>
		{
			static int compare(const str_view<T, E>& value1, dom::xml::ixml_text_t value2) {
				return str_view_compare_helper<cstr_t, cstr_t>::compare(value1, value2);
			}
		};

		template<typename T, text::encoding E>
		struct str_view_compare_helper<dom::xml::ixml_text_t, str_view<T, E>>
		{
			static int compare(dom::xml::ixml_text_t value1, const str_view<T, E>& value2) {
				return str_view_compare_helper<cstr_t, cstr_t>::compare(value1, value2);
			}
		};
	}
}


#endif//__ANG_DOM_XML_IXML_TEXT_H__
