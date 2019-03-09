#include "pch.h"
#include "scenes/model_loader.h"

#if defined _DEBUG
#define new new(__FILE__,__LINE__)
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;

#define MY_TYPE indexed_model
#include <ang/inline/object_wrapper_specialization.inl>
#undef MY_TYPE

indexed_model::indexed_model()
{

}

indexed_model::~indexed_model()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::scenes::indexed_model);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::scenes::indexed_model, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::scenes::indexed_model, object);

collections::vector<indexed_model::model_element> indexed_model::load(dom::xml::xml_node_t material_info)
{
	if (material_info.is_empty() || !material_info->xml_has_children())
		return null;
	m_elements->empty();
	for(dom::xml::xml_node_t node : material_info->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		indexed_model::model_element element;
		if (name == "element"_s && load_element(node, element))
			m_elements += element;
	}
	return m_elements.get();
}

core::async::iasync<collections::vector<indexed_model::model_element>> indexed_model::load_async(dom::xml::xml_node_t node)
{
	indexed_model_t auto_save = this;
	return core::async::task::run_async<collections::vector<indexed_model::model_element>>(
		[this, auto_save, node](core::async::iasync<collections::vector<indexed_model::model_element>> async) ->collections::vector<indexed_model::model_element>
	{
		try
		{
			collections::vector<indexed_model::model_element> elements = load(node).get();
			if (elements.is_empty())
				async->cancel();
			return elements.get();
		}
		catch (exception_t const& e) {
			e->what();
			return null;
		}
		
	});
}

bool indexed_model::load_element(dom::xml::xml_node_t node, indexed_model::model_element& out)
{
	indexed_model::model_element element_info;
	if (node.is_empty() || !node->xml_has_children())
		return null;
	bool res = true;
	for(dom::xml::xml_node_t node : node->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "material"_s)
			load_material(node, out);
		else if (name == "index_buffer"_s)
			res &= load_index_buffer(node, out);
		else if (name == "vertex_buffer"_s)
			res &= load_vertex_buffer(node, out);
	}
	return res;
}

bool indexed_model::load_material(dom::xml::xml_node_t material, indexed_model::model_element& out)
{
	if (material.is_empty() || !material->xml_has_children())
		return false;
	out.textures = null;
	for(dom::xml::xml_node_t node : material->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "fx"_s)
			out.technique_name = node->xml_value()->xml_as<text::raw_cstr>();
		else if (name == "texture"_s)
			out.textures += node->xml_value()->xml_as<text::raw_cstr>();
	}
	return true;
}

bool indexed_model::load_index_buffer(dom::xml::xml_node_t indices, indexed_model::model_element& out)
{
	if (!indices->xml_has_children())
		return false;
	auto type_node = indices["data_type"_s];
	auto data_node = indices["data"_s];

	reflect::var_type_t  type = type_node ? type_node["type"_s]->xml_value()->xml_as<reflect::var_type_t>() : (reflect::var_type_t)reflect::var_type::u32;
	uint count = data_node["count"_s]->xml_value()->xml_as<uint>();

	streams::itext_input_stream_t stream = new streams::string_input_stream(data_node->xml_value().get());
	if (stream->is_eos())
		return false;

	switch (type.get())
	{
	case reflect::var_type::u16: {
		out.index_type = reflect::var_type::u16;
		array<ushort> arr = new collections::array_buffer<ushort>(count);
		for (ushort& item : arr)
			stream >> item >> " , "_s;
		out.index_data = arr.get();
	}break;

	case reflect::var_type::s16: {
		out.index_type = reflect::var_type::s16;
		array<short> arr = new collections::array_buffer<short>(count);
		for (short& item : arr)
			stream >> item >> " , "_s;
		out.index_data = arr.get();
	}break;
	case reflect::var_type::s32: {
		out.index_type = reflect::var_type::s32;
		array<int> arr = new collections::array_buffer<int>(count);
		for (int& item : arr)
			stream >> item >> " , "_s;
		out.index_data = arr.get();
	}break;
	default:
	case reflect::var_type::u32: {
		out.index_type = reflect::var_type::u32;
		array<uint> arr = new collections::array_buffer<uint>(count);
		for (uint& item : arr)
			stream >> item >> " , "_s;
		out.index_data = arr.get();
	}break;
	}

	return true;
}

bool indexed_model::load_vertex_buffer(dom::xml::xml_node_t vertices, indexed_model::model_element& out)
{
	if (!vertices->xml_has_children())
		return false;
	auto data_layout = vertices["data_layout"_s];
	auto data_node = vertices["data"_s];

	if (!reflect::attribute_desc::load(data_layout, out.vertex_desc))
		return false;

	wsize stride = reflect::attribute_desc::get_size_in_bytes(out.vertex_desc, 16U);
	wsize stride4 = reflect::attribute_desc::get_size_in_bytes(out.vertex_desc, 4U);
	if (stride == stride4)
	{
		wsize count = data_node["vertex_count"_s]->xml_value()->xml_as<uint>() * stride / sizeof(float);
		
		streams::itext_input_stream_t stream = new streams::string_input_stream(data_node->xml_value().get());
		if (stream->is_eos())
			return false;

		array<float, memory::aligned16_allocator> data = new collections::array_buffer<float, memory::aligned16_allocator>();
		data->size(count);
		out.vertex_data = data.get();
		for (float& item : data)
			stream >> item >> " , "_s;
	}
	else
	{
		wsize count = data_node["vertex_count"_s]->xml_value()->xml_as<uint>() * stride / sizeof(float);
		
		streams::itext_input_stream_t stream = new streams::string_input_stream(data_node->xml_value().get());
		if (stream->is_eos())
			return false;

		array<float, memory::aligned16_allocator> data = new collections::array_buffer<float, memory::aligned16_allocator>();
		data->size(count);

		wsize in_size = stride4 / sizeof(float);
		wsize out_size = stride / sizeof(float);

		wsize aligment = 16u;
		out.vertex_data = data.get();
		for (index i = 0U; i < count; )
		{
			wsize total = 0;
			wsize size = 0;
			wsize temp = 0;
			wsize res = 0;
	
			for(index j = 0u, c = out.vertex_desc->counter(); j < c; j++)
			{		
				reflect::attribute_desc const& desc = out.vertex_desc[j];
				size = desc.get_size_in_bytes();
				if (size == 0)continue;//next item
				temp = (total % aligment);
				res = aligment - temp;
				if (res < aligment)
				{
					if (res > size)
						total += reflect::get_memory_size_aligned(temp, size) - temp;
					else if (res < size)
						total += res;
				}
				for (index k = 0U, d = size / sizeof(float); k < d; ++k)
				{
					stream >> data[i + total / sizeof(float) + k] >> " , "_s;
				}
				total += size;		
			}
			i += reflect::get_memory_size_aligned(total, aligment) / sizeof(float);
		}
	}
	
	return true;
}