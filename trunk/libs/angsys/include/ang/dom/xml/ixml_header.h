#ifndef __COFFE_DOM_XML_H__
#error ...
#elif !defined __COFFE_DOM_XML_IXML_HEADER_H__
#define __COFFE_DOM_XML_IXML_HEADER_H__

namespace coffe
{
	namespace dom
	{
		namespace xml
		{
			coffe_begin_interface(LINK ixml_header)
				visible virtual ixml_text_view_t xml_version()const = 0;
				visible virtual xml_encoding_t xml_encoding()const = 0;
				visible virtual bool xml_stand_alone()const = 0;
			coffe_end_interface();


		}
	}
}


#endif//__COFFE_DOM_XML_IXML_HEADER_H__
