// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <angsys.h>
#include <ang/dom/xml.h>

using namespace ang;

int main()
{
	dom::xml::xml_type_t type = dom::xml::xml_type::element;

	core::files::input_text_file_t file = new core::files::input_text_file();
	file->open("./resources/fx_library.xml");
	auto res = dom::xml::xml_document_from_file(file);
	if (res.successed()) 
	{
		dom::xml::ixml_document_t doc = res.get();
		dom::xml::ixml_node_t node = doc->root_element();
	}

	return 0;
}
