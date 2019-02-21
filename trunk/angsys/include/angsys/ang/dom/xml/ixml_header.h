#ifndef __ANG_DOM_XML_H__
#error ...
#elif !defined __ANG_DOM_XML_IXML_HEADER_H__
#define __ANG_DOM_XML_IXML_HEADER_H__

namespace ang
{
	namespace dom
	{
		namespace xml
		{
			ang_begin_interface(LINK ixml_header)
				visible vcall ixml_text_t xml_version()const pure;
				visible vcall xml_encoding_t xml_encoding()const pure;
				visible vcall bool xml_stand_alone()const pure;
			ang_end_interface();


		}
	}
}


#endif//__ANG_DOM_XML_IXML_HEADER_H__