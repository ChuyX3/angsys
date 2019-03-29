// test.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <ang/streams.h>
#include <ang/maths/vectors.h>
#include <ang/dom/xml.h>
#include <ang/collections/list.h>

#include <ang/core/timer.h>

using namespace ang;
using namespace ang::dom;

struct vertex
{
	maths::float3 position;
	maths::float4 color;
	maths::float2 texcoord;
};

int main(int argc, char* argv[])
{	
	core::files::input_text_file_t file;
	core::files::ifile_system::instance()->open("./vertex_buffer.xml", &file);

	auto last_time = core::time::get_performance_time_us();

	auto doc = dom::xml::xml_document::from_file(file);
	
	auto xml_doc_time = core::time::get_performance_time_us();

	graphics::reflect::struct_buffer_t data = new graphics::reflect::struct_buffer();
	data->load(doc->xml_root_element());

	auto current_time = core::time::get_performance_time_us();
	array_view<vertex> data_view = to_array((vertex*)data->raw_data().data(), data->raw_data().size() / size_of<vertex>());
	printf("parse time: %d - %d\n", xml_doc_time - last_time, current_time - xml_doc_time);

	return 0;
}
