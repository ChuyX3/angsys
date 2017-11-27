// task1.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/streams.hpp>
#include <ang/xml.hpp>

#include <type_traits>

using namespace ang;

//static cwstr_t _code =
//L"<?xml version=\"1.0\" encoding=\"utf-16\"?>\n"
//"<Type Name = \"ang::xml::xml_value\">\n"
//"	<DisplayString>{_value}</DisplayString>\n"
//"	<Expand>\n"
//"		<ExpandedItem>_value</ExpandedItem>\n"
//"	</Expand>\n"
//"</Type>"_s;

int main(int argc, char* argv[])
{
	FILE* file = fopen("angxml.xml", "r");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		wsize sz = ftell(file);
		fseek(file, 0, SEEK_SET);

		ibuffer_t buff = new(sz) buffer();

		fread(buff->buffer_ptr(), 1, buff->buffer_size(), file);


		xml::xml_document_t doc = new xml::xml_document();
		raw_str_t code(buff->buffer_ptr(), buff->buffer_size(), text::encoding::auto_detect);
		doc->parse(code);


		fclose(file);
	}


    return 0;
}

