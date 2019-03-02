// test.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/streams.h>
#include <ang/maths/vectors.h>
#include <ang/dom/xml.h>
using namespace ang;
using namespace ang::dom;

int main(int argc, char* argv[])
{	
	xml::xml_document_t doc = new xml::xml_document();

	core::files::input_text_file_t file = new core::files::input_text_file();
	file->open(L".\\texture_apex.xml"_s);
	doc->load(file);
	return 0;
}

