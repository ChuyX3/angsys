/*********************************************************************************************************************/
/*   File Name: ang/doc/xml.h                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes classes for the use of Xml files                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_XML_H__
#define __ANG_XML_H__

#include <angsys.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif

namespace ang
{
	namespace doc
	{
		namespace xml
		{
			ang_interface(ixml_node);
			ang_interface(ixml_document);

			safe_enum(LINK, xml_type, uint)
			{
				value,
				node,
				attribute,
				element,
				comment,
				list,
				document,
			};

			typedef text::encoding xml_encoding;
			typedef text::encoding_t xml_encoding_t;

			ang_begin_interface(ixml_node)
				visible vcall ixml_node_t xml_parent()const pure;
				visible vcall ixml_document xml_parent_document()const pure;
				visible vcall ixml_node_t xml_first_child()const pure;
				visible vcall ixml_node_t xml_last_child()const pure;
				visible vcall ixml_node_t xml_prev_sibling()const pure;
				visible vcall ixml_node_t xml_next_sibling()const pure;
				visible vcall ixml_node_t xml_clone(ixml_document_t)const pure;

				visible vcall bool xml_has_name()const pure;
				visible vcall bool xml_has_value()const pure;
				visible vcall bool xml_has_children()const pure;
				visible vcall bool xml_has_attributes()const pure;

				visible vcall Ixml_TextPtr xml_Name()const pure;
				visible vcall Ixml_TextPtr xml_Value()const pure;
				visible vcall Ixml_CollectionPtr xml_Children()const pure;
				visible vcall Ixml_Attributes xml_Attributes()const pure;

				visible vcall void xml_Parent(ixml_node_t) pure;
				visible vcall void xml_PrevSibling(ixml_node_t) pure;
				visible vcall void xml_NextSibling(ixml_node_t) pure;
				allow Ixml_Collection;
				allow Ixml_Document;
			ang_end_interface();
			
		}
	}
}


#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_XML_H__
