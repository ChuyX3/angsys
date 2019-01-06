#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_IXML_OBJECT_H__
#define __ANG_DOM_XML_IXML_OBJECT_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			ang_begin_interface(LINK ixml_object)
				visible vcall xml_type_t xml_type()const pure;
				visible vcall bool xml_is_type_of(xml_type_t)const pure;
				visible vcall streams::itext_output_stream_t& xml_print(streams::itext_output_stream_t& stream, const xml_format_t& flag, ushort level = 0)const pure;
				template<typename T> typename smart_ptr_type<T>::smart_ptr_t xml_as() { return null; }
			ang_end_interface();


		}
	}
}


#endif//__ANG_DOM_XML_IXML_OBJECT_H__
