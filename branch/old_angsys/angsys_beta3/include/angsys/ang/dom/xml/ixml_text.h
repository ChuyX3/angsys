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
			ang_begin_interface(LINK ixml_text, ixml_node)
				visible vcall text::itext_buffer_t xml_data()pure;
			ang_end_interface();
		}
	}
}


#endif//__ANG_DOM_XML_IXML_TEXT_H__
