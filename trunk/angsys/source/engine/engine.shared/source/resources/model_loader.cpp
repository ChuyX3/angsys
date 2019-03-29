#include "pch.h"
#include "resources/resources.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;

struct vertex
{
	maths::float3 position;
	maths::float3 normal;
	maths::float2 texcoord;

	static reflect::varying_desc_t descriptor() {
		return ang::move(reflect::varying_desc{
			"vertex"_r,
			reflect::uniform_fields_t {
				reflect::varying_desc{ reflect::var_type::f32, reflect::var_class::vec3, "position"_r },
				reflect::varying_desc{ reflect::var_type::f32, reflect::var_class::vec3, "normal"_r },
				reflect::varying_desc{ reflect::var_type::f32, reflect::var_class::vec2, "texcoord"_r },
			}
		});
	}
};


model_loader::model_loader(core::files::ifile_system* fs)
	: m_file_system(fs)
{

}

model_loader::~model_loader()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::resources::model_loader);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::resources::model_loader, object, imodel_loader);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::resources::model_loader, object, imodel_loader);

void model_loader::dispose()
{
	clear();
	m_elements = null;
}

void model_loader::clear()
{ 
	m_elements->clear();
}

bool model_loader::load(dom::xml::xml_node_t node)
{
	if (node.is_empty())
		return false;

	if (node->xml_has_value()) 
	{
		auto fs = m_file_system.lock();
		if (fs.is_empty())
			fs = core::files::ifile_system::instance();

		core::files::input_binary_file_t fbin;
		if (fs->open(node->xml_value(), &fbin))
		{
			if (load(fbin))
				return true;
			fbin->dispose();

			core::files::input_text_file_t ftext;
			if (fs->open(node->xml_value(), &ftext))
			{
				if (load(ftext))
					return true;
			}
		}
		return false;
	}
	else if (node->xml_has_children())
	{
		if (node.is_empty() || !node->xml_has_children())
			return null;
		m_elements->empty();
		for (dom::xml::xml_node_t node : node->xml_children())
		{
			auto name = node->xml_name()->xml_as<cstr_t>();
			model_element element;
			if (name == "element"_s && load_element(node, element))
				m_elements += element;
		}
		return m_elements.get();

		return true;
	}
	return false;
}

bool model_loader::save(dom::xml::xml_document_t)const
{
	//TODO:
	return false;
}


bool model_loader::load(core::files::input_text_file_t file)
{
	//trying parse file as a xml document
	auto doc = dom::xml::xml_document::from_file(file);
	if (!doc.is_empty())
		return load(doc->xml_root_element());
	else /*no xml document*/ {		
		try
		{
			string line = ""_s;
			streams::itext_input_stream_t stream;
			streams::string_input_stream_t wrapper = new streams::string_input_stream(null);
			stream = wrapper.get();

			cstr_t v_s = "v ";
			cstr_t vt_s = "vt ";
			cstr_t vn_s = "vn ";
			cstr_t f_s = "f ";
			cstr_t mtllib_s = "mtllib ";
			cstr_t usemtl_s = "usemtl ";

			auto fs = m_file_system.lock();
			if (fs.is_empty())
				fs = core::files::ifile_system::instance();

			string material;
			model_element model;
			vector<string> material_lib;
			vector<vertex> vertices;

			maths::matrix4 rotation = maths::matrix::rotation_x(-3.141592f / 2.0f);

			maths::unaligned_float3 vec;
			vector<maths::unaligned_float3> position;
			vector<maths::unaligned_float3> normals;
			vector<maths::unaligned_float2> texcoords;

			while (!file->is_eos())
			{
				file->read_line(line);
				wrapper->attach(line.get());

				if (line->find(v_s, 0, v_s.size() + 1) == 0)
				{
					stream >> v_s >> vec.get<0>() >> vec.get<1>() >> vec.get<2>();
					auto vec2 = maths::float4(vec.get<0>(), vec.get<1>(), vec.get<2>(), 1) * rotation;
					position += maths::unaligned_float3{ vec2.get<0>(), vec2.get<1>(), vec2.get<2>() };
				}
				else if (line->find(vn_s, 0, vn_s.size() + 1) == 0)
				{
					stream >> vn_s >> vec.get<0>() >> vec.get<1>() >> vec.get<2>();
					auto vec2 = maths::float4(vec.get<0>(), vec.get<1>(), vec.get<2>(), 0) * rotation;
					normals += maths::unaligned_float3{ vec2.get<0>(), vec2.get<1>(), vec2.get<2>() };
				}
				else  if (line->find(vt_s, 0, vt_s.size() + 1) == 0)
				{
					stream >> vt_s >> vec.get<0>() >> vec.get<1>();
					texcoords += maths::unaligned_float2{ vec.get<0>(), vec.get<1>() };
				}
				else  if (line->find(f_s, 0, f_s.size() + 1) == 0)
				{
					windex face[3] = { 0,0,0 };
					vertex vertex[3];
					bool has_normal = false;
					stream >> f_s >> face[0] >> "/"_s >> face[1] >> "/"_s >> face[2];
					if (face[0] > 0)
					{
						vec = position[face[0] - 1];
						vertex[0].position = { vec.get<0>(), vec.get<1>(), vec.get<2>() };
					}
					if (face[1] > 0)
					{
						vec = { texcoords[face[1] - 1], 0 };
						vertex[0].texcoord = { vec.get<0>(), vec.get<1>() };
					}
					if (face[2] > 0)
					{
						has_normal = true;
						vec = normals[face[2] - 1];
						vertex[0].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>() };
					}

					stream >> face[0] >> "/"_s >> face[1] >> "/"_s >> face[2];
					if (face[0] > 0)
					{
						vec = position[face[0] - 1];
						vertex[1].position = { vec.get<0>(), vec.get<1>(), vec.get<2>() };
					}
					if (face[1] > 0)
					{
						vec = { texcoords[face[1] - 1], 0 };
						vertex[1].texcoord = { vec.get<0>(), vec.get<1>() };
					}
					if (face[2] > 0)
					{
						has_normal = true;
						vec = normals[face[2] - 1];
						vertex[1].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>() };
					}

					stream >> face[0] >> "/"_s >> face[1] >> "/"_s >> face[2];
					if (face[0] > 0)
					{
						vec = position[face[0] - 1];
						vertex[2].position = { vec.get<0>(), vec.get<1>(), vec.get<2>() };
					}
					if (face[1] > 0)
					{
						vec = { texcoords[face[1] - 1], 0 };
						vertex[2].texcoord = { vec.get<0>(), vec.get<1>() };
					}
					if (face[2] > 0)
					{
						has_normal = true;
						vec = normals[face[2] - 1];
						vertex[2].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>() };
					}

					if (!has_normal)
					{
						maths::float3 normal = maths::float3(vertex[2].position - vertex[1].position) % maths::float3(vertex[1].position - vertex[0].position);
						vertex[0].normal = vertex[1].normal = vertex[2].normal = maths::norm(normal);
					}

					vertices += vertex[0];
					vertices += vertex[1];
					vertices += vertex[2];
				}
				else if (line->find(usemtl_s, 0, usemtl_s.size() + 1) == 0)
				{
					if (!material.is_empty())
					{
						core::files::input_text_file_t mtl;
						if (fs->open(material, &mtl))
						{
							load_material(mtl, model);
							mtl->dispose();
						}
						model.vertex_data = new reflect::struct_buffer();
						auto desc = vertex::descriptor();
						model.vertex_data->make_array(desc, vertices->counter(), 16);
						memcpy(model.vertex_data->raw_data().data(), vertices->data(), model.vertex_data->raw_data().size());

						m_elements += ang::move(model);
						vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();
					}
					line->sub_string(&material, usemtl_s.size(), -1);
				}
				else if (line->find(mtllib_s, 0, mtllib_s.size() + 1) == 0)
				{
					istring_t mtl;
					line->sub_string(&mtl, mtllib_s.size(), -1);
					material_lib += mtl;
				}
			}
			if (!material.is_empty())
			{
				core::files::input_text_file_t mtl;
				if (fs->open(material, &mtl))
				{
					load_material(mtl, model);
					mtl->dispose();
				}
				m_elements += ang::move(model);
			}
			return true;
		}
		catch (exception_t e)
		{
			clear();
			return false;
		}
	}
	return false;
}

bool model_loader::save(core::files::output_text_file_t file)const
{
	//TODO:
	return false;
}

bool model_loader::load(core::files::input_binary_file_t file)
{
	//TODO:
	return false;
}

bool model_loader::save(core::files::output_binary_file_t file)const
{
	//TODO:
	return false;
}

core::async::iasync<imodel_loader_t> model_loader::load_async(dom::xml::xml_node_t node)
{
	auto aw = m_async_worker.lock();
	if (aw.is_empty())
		return null;
	model_loader_t auto_save = const_cast<model_loader*>(this);
	return aw->run_async<imodel_loader_t>([=](core::async::iasync<imodel_loader_t>)->imodel_loader_t
	{
		return auto_save->load(node) ? auto_save.get() : null;
	});
}

core::async::iasync<imodel_loader_t> model_loader::save_async(dom::xml::xml_document_t doc)const
{
	auto aw = m_async_worker.lock();
	if (aw.is_empty())
		return null;
	model_loader_t auto_save = const_cast<model_loader*>(this);
	return aw->run_async<imodel_loader_t>([=](core::async::iasync<imodel_loader_t>)->imodel_loader_t
	{
		return auto_save->save(doc) ? auto_save.get() : null;
	});
}

core::async::iasync<imodel_loader_t> model_loader::load_async(core::files::input_text_file_t file)
{
	auto aw = m_async_worker.lock();
	if (aw.is_empty())
		return null;
	model_loader_t auto_save = const_cast<model_loader*>(this);
	return aw->run_async<imodel_loader_t>([=](core::async::iasync<imodel_loader_t>)->imodel_loader_t
	{
		return auto_save->load(file) ? auto_save.get() : null;
	});
}

core::async::iasync<imodel_loader_t> model_loader::save_async(core::files::output_text_file_t file)const
{
	auto aw = m_async_worker.lock();
	if (aw.is_empty())
		return null;
	model_loader_t auto_save = const_cast<model_loader*>(this);
	return aw->run_async<imodel_loader_t>([=](core::async::iasync<imodel_loader_t>)->imodel_loader_t
	{
		return auto_save->save(file) ? auto_save.get() : null;
	});
}

core::async::iasync<imodel_loader_t> model_loader::load_async(core::files::input_binary_file_t file)
{
	auto aw = m_async_worker.lock();
	if (aw.is_empty())
		return null;
	model_loader_t auto_save = const_cast<model_loader*>(this);
	return aw->run_async<imodel_loader_t>([=](core::async::iasync<imodel_loader_t>)->imodel_loader_t
	{
		return auto_save->load(file) ? auto_save.get() : null;
	});
}

core::async::iasync<imodel_loader_t> model_loader::save_async(core::files::output_binary_file_t file)const
{
	auto aw = m_async_worker.lock();
	if (aw.is_empty())
		return null;
	model_loader_t auto_save = const_cast<model_loader*>(this);
	return aw->run_async<imodel_loader_t>([=](core::async::iasync<imodel_loader_t>)->imodel_loader_t
	{
		return auto_save->save(file) ? auto_save.get() : null;
	});
}

collections::ienum_ptr<model_element> model_loader::elements()const
{
	return m_elements.get();
}

bool model_loader::load_material(core::files::input_text_file_t file, model_element& out)
{
	//loading materia from mtl file
}

bool model_loader::load_element(dom::xml::xml_node_t node, model_element& out)
{
	if (node.is_empty() || !node->xml_has_children())
		return false;
	bool res = true;
	for (dom::xml::xml_node_t node : node->xml_children())
	{
		auto name = node->xml_name()->xml_as<cstr_t>();
		if (name == "material"_s)
			load_material(node, out);
		else if (name == "technique"_s)
			out.technique_name = node->xml_value()->xml_as<cstr_t>();
		else if (name == "buffer"_s)
		{
			auto usage = node["usage"];
			if (usage.is_empty())
				continue;
			auto usage_value = usage->xml_value()->xml_as<cstr_t>();
			if (usage_value == "index")
				out.index_data = load_buffer(node);
			else if (usage_value == "vertex")
				out.vertex_data = load_buffer(node);
		}
	}
	return res;
}

bool model_loader::load_material(dom::xml::xml_node_t node, model_element& out)
{
	if (node.is_empty() || !node->xml_has_children())
		return false;
	out.material.samplers = null;
	if (out.material.fields.is_empty())
		out.material.fields = new reflect::struct_buffer();

	out.material.fields->make_struct();

	reflect::text_data_loader_context_t parser;
	reflect::text_data_loader_input_context_t input = {
		text::iparser::get_parser(node->xml_parent_doc()->xml_encoding()),
		""_r,
		0
	};
	reflect::text_data_loader_output_context_t output;

	for (dom::xml::xml_node_t node : node->xml_children())
	{
		auto name = node->xml_name()->xml_as<cstr_t>();
		if (name == "var"_s)
		{
			reflect::varying_desc_t desc;
			if (desc.load(node))
			{
				reflect::text_data_loader::create_context(desc, parser);
				input.data = node->xml_value();
				input.idx = 0;
				reflect::varying_t va = out.material.fields->push_var(desc);
				output.data = va.raw_data().get();
				output.idx = 0;
				parser.load_data(parser, input, output);
			}
		}
		else if (name == "texture"_s)
			out.material.samplers += { node["name"]->xml_value()->xml_as<cstr_t>(), node->xml_value()->xml_as<cstr_t>() };
		
	}
	return true;
}

reflect::struct_buffer_t model_loader::load_buffer(dom::xml::xml_node_t node)
{
	reflect::struct_buffer_t buffer = new reflect::struct_buffer();
	if (!buffer->load(node))
		return null;
	return ang::move(buffer);
}
