#ifndef __COFFE_DOM_XML_H__
#error ...
#elif !defined __COFFE_DOM_XML_IXML_OBJECT_H__
#define __COFFE_DOM_XML_IXML_OBJECT_H__

namespace coffe
{
	namespace dom
	{
		namespace xml
		{
			struct nvt LINK ixml_object : intf<ixml_object>
			{
				virtual xml_type_t type()const = 0;
				virtual bool is_type_of(xml_type_t)const = 0;
				virtual streams::itext_output_stream_t& print(streams::itext_output_stream_t& stream, const xml_format_t& flag, word level = 0)const = 0;
				template<typename T> typename smart_ptr_type<T>::smart_ptr_t as()const { return null; }
			};

		}
	}

	COFFE_BEGIN_INTF_WRAPPER(LINK, dom::xml::ixml_object)
		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as()const { return get() ? get()->as<T>() : null; }
	COFFE_END_INTF_WRAPPER();
}


#endif//__COFFE_DOM_XML_IXML_OBJECT_H__
