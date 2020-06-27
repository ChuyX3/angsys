#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::meshes;

struct vertex
{
	maths::float4 position;
	maths::float4 normal;
	maths::float4 texcoord;

	static array<reflect::attribute_desc> descriptor() {
		return initializer_list<reflect::attribute_desc>{
			{ reflect::var_type::f32, reflect::var_class::vec3, "position"_r, reflect::var_semantic::position, 0, 0},
			{ reflect::var_type::f32, reflect::var_class::vec3, "normal"_r, reflect::var_semantic::normal, 0, 16 },
			{ reflect::var_type::f32, reflect::var_class::vec2, "texcoord"_r, reflect::var_semantic::tex_coord, 0, 32 }
		};
	}
};

struct raw_material
{
	maths::float4 Ka = { 1,1,1,0};
	maths::float4 Kd = { 1,1,1,0 };
	maths::float4 Ks = { 1,1,1,0 };
	uint ilum = 0;
	float d = 1;
	float Ns = 0;
	string map_Kd;
	string map_bump;
	string map_d;

	static vector<reflect::varying_desc> descriptor() {
		return initializer_list<reflect::varying_desc>{
			{reflect::var_type::f32, reflect::var_class::vec3, "Ka"_r, 1, 16},
			{ reflect::var_type::f32, reflect::var_class::vec3, "Kd"_r, 1, 16 },
			{ reflect::var_type::f32, reflect::var_class::vec3, "Ks"_r, 1, 16 },
			{ reflect::var_type::u32, reflect::var_class::scalar, "ilum"_r, 1, 4 },
			{ reflect::var_type::f32, reflect::var_class::scalar, "d"_r, 1, 4 },
			{ reflect::var_type::f32, reflect::var_class::scalar, "Ns"_r, 1, 4 }
		};
	}
};

struct mesh_element
{
	string material;
	smart_vector<vertex, memory::aligned16_allocator> vertices;

	mesh_element()
		: material()
		, vertices(new collections::vector_buffer<vertex, memory::aligned16_allocator>()) {
	}
	mesh_element(mesh_element && element)
		: material(ang::move(element.material))
		, vertices(ang::move(element.vertices)) {
	}
	mesh_element(mesh_element const& element)
		: material((cstr_t)element.material)
		, vertices(new collections::vector_buffer<vertex, memory::aligned16_allocator>()) {
		vertices->copy(element.vertices.get());
	}
	mesh_element& operator = (mesh_element const& element) {
		material = (cstr_t)element.material;
		vertices->copy(element.vertices.get());
		return*this;
	}
	mesh_element& operator = (mesh_element && element) {
		material = move(element.material);
		vertices = move(element.vertices);
		return*this;
	}
	~mesh_element() {
		material = null;
		vertices = null;
	}
};

vector<geometry_data> mesh_loader::load_data(ilibrary_t lib, dom::xml::ixml_node_t node, string_ptr sid)
{
	if (node.is_empty() || !node->has_children())
		return null;

	vector<geometry_data> data;

	if (!sid.is_empty() && (*sid).is_empty())
		*sid = node->attributes()["sid"]->as<cstr_t>();

	for (auto child : node->children())
	{
		auto name = child->name()->as<cstr_t>();
		if (name == "element")
		{
			load_element(lib, child, data);
		}
		else if (name == "file")
		{
			core::files::input_text_file_t file;
			if (!lib->file_system()->open(child->value()->as<cstr_t>(), &file))
				return null;
			return load_data(lib, file, sid);
		}
	}
	return ang::move(data);
}

vector<geometry_data> mesh_loader::load_data(ilibrary_t lib, core::files::input_text_file_t file, string_ptr sid)
{
	if (file.is_empty())
		return null;
	auto doc = dom::xml::xml_document_from_file(file);
	if (doc.successed())
		return ang::move(load_data(lib, doc->root_element(), sid));

	vector<mesh_element> elements = null;
	vector<string> material_lib = null;

	cstr_t v_s = "v "_r;
	cstr_t vt_s = "vt "_r;
	cstr_t vn_s = "vn "_r;
	cstr_t f_s = "f "_r;
	cstr_t mtllib_s = "mtllib "_r;
	cstr_t usemtl_s = "usemtl "_r;

	try
	{
		string line = ""_s;

		string material;
		mesh_element model;
		
		//model.vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();
		//maths::mat4 rotation = maths::matrix::rotation_x(-3.141592f / 2.0f);

		maths::ul_float3 vec;
		vector<maths::ul_float3> vertices;
		vector<maths::ul_float3> normals;
		vector<maths::ul_float2> texcoords;

		while (!file->is_eos())
		{
			windex i = 0;
			file->read_line(line);

			if (line->find(v_s, 0, v_s.size() + 1) == 0)
			{
				text::parser::seek(line, i, v_s);
				vec.set<0>(text::parser::to_value<float>(line, i));
				vec.set<1>(text::parser::to_value<float>(line, i));
				vec.set<2>(text::parser::to_value<float>(line, i));

				auto vec2 = maths::float4(vec.get<0>(), vec.get<1>(), vec.get<2>(), 1);// *rotation;
				vertices += maths::ul_float3{ vec2.get<0>(), vec2.get<1>(), vec2.get<2>() };
			}
			else if (line->find(vn_s, 0, vn_s.size() + 1) == 0)
			{
				text::parser::seek(line, i, vn_s);
				vec.set<0>(text::parser::to_value<float>(line, i));
				vec.set<1>(text::parser::to_value<float>(line, i));
				vec.set<2>(text::parser::to_value<float>(line, i));

				auto vec2 = maths::float4(vec.get<0>(), vec.get<1>(), vec.get<2>(), 0);// *rotation;
				normals += maths::ul_float3{ vec2.get<0>(), vec2.get<1>(), vec2.get<2>() };
			}
			else  if (line->find(vt_s, 0, vt_s.size() + 1) == 0)
			{
				text::parser::seek(line, i, vt_s);
				vec.set<0>(text::parser::to_value<float>(line, i));
				vec.set<1>(text::parser::to_value<float>(line, i));

				texcoords += maths::ul_float2{ vec.get<0>(), vec.get<1>() };
			}
			else  if (line->find(f_s, 0, f_s.size() + 1) == 0)
			{
				windex face[3] = { 0,0,0 };
				vertex vertex[3];
				bool has_normal = false;

				text::parser::seek(line, i, f_s);
				face[0] = text::parser::to_value<float>(line, i);
				text::parser::seek(line, i, "/"_r);
				face[1] = text::parser::to_value<float>(line, i);
				text::parser::seek(line, i, "/"_r);
				face[2] = text::parser::to_value<float>(line, i);

				if (face[0] > 0)
				{
					vec = vertices[face[0] - 1];
					vertex[0].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
				}
				if (face[1] > 0)
				{
					vec = { texcoords[face[1] - 1], 0 };
					vertex[0].texcoord = { vec.get<0>(), vec.get<1>(), 0, 0 };
				}
				if (face[2] > 0)
				{
					has_normal = true;
					vec = normals[face[2] - 1];
					vertex[0].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
				}

				face[0] = text::parser::to_value<float>(line, i);
				text::parser::seek(line, i, "/"_r);
				face[1] = text::parser::to_value<float>(line, i);
				text::parser::seek(line, i, "/"_r);
				face[2] = text::parser::to_value<float>(line, i);

				if (face[0] > 0)
				{
					vec = vertices[face[0] - 1];
					vertex[1].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
				}
				if (face[1] > 0)
				{
					vec = { texcoords[face[1] - 1], 0 };
					vertex[1].texcoord = { vec.get<0>(), vec.get<1>(), 0, 0 };
				}
				if (face[2] > 0)
				{
					has_normal = true;
					vec = normals[face[2] - 1];
					vertex[1].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
				}

				face[0] = text::parser::to_value<float>(line, i);
				text::parser::seek(line, i, "/"_r);
				face[1] = text::parser::to_value<float>(line, i);
				text::parser::seek(line, i, "/"_r);
				face[2] = text::parser::to_value<float>(line, i);

				if (face[0] > 0)
				{
					vec = vertices[face[0] - 1];
					vertex[2].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
				}
				if (face[1] > 0)
				{
					vec = { texcoords[face[1] - 1], 0 };
					vertex[2].texcoord = { vec.get<0>(), vec.get<1>(), 0, 0 };
				}
				if (face[2] > 0)
				{
					has_normal = true;
					vec = normals[face[2] - 1];
					vertex[2].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
				}

				if (!has_normal)
				{
					maths::float3 normal = maths::float3(vertex[2].position - vertex[1].position) % maths::float3(vertex[1].position - vertex[0].position);
					vertex[0].normal = vertex[1].normal = vertex[2].normal = (maths::float3)maths::float4(maths::norm(normal), 0);
				}

				model.vertices += vertex[0];
				model.vertices += vertex[1];
				model.vertices += vertex[2];
			}
			else if (line->find(usemtl_s, 0, usemtl_s.size() + 1) == 0)
			{
				if (!material.is_empty())
				{
					model.material = ang::move(material);
					elements += ang::move(model);
					model.vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();
				}
				material = line->sub_string(usemtl_s.size(), -1);
			}
			else if (line->find(mtllib_s, 0, mtllib_s.size() + 1) == 0)
			{
				material_lib += line->sub_string(mtllib_s.size(), -1);
			}
		}
		if (!material.is_empty())
		{
			model.material = ang::move(material);
			elements += ang::move(model);
		}
	}
	catch (exception const& e)
	{
		return null;
	}

	collections::hash_map<string, material_data> material_map;

	for (auto mat : material_lib)
	{
		core::files::input_text_file_t file;
		if (!lib->file_system()->open(mat, &file))
			continue;
		load_material_data(lib, file, material_map);
		file->dispose();
	}

	vector<geometry_data> geometries = null;
	string technique_name = "default"_r;
	for (auto& element : elements)
	{
		geometry_data_t geometry;

		geometry.technique_name = technique_name;
		try {
			geometry.material = material_map[element.material];
		}
		catch (...) {}
		
		geometry.index_data.set<0>(reflect::var_type::none);
		geometry.index_data.set<1>(0);
		geometry.index_data.set<2>(null);

		geometry.vertex_data.set<0>(vertex::descriptor());
		geometry.vertex_data.set<1>(element.vertices->size());
		geometry.vertex_data.set<2>(element.vertices.get());

		geometries += move(geometry);
	}

	return move(geometries);
}

vector<geometry_data> mesh_loader::load_data(ilibrary_t, core::files::input_binary_file_t file, string_ptr)
{
	return null;
}

void mesh_loader::load_element(ilibrary_t lib, dom::xml::ixml_node_t node, vector<geometry_data>& elements)
{
	if (node.is_empty() || !node->has_children())
		return;

	geometry_data_t data;
	for (auto child : node->children())
	{
		auto name = child->name()->as<cstr_t>();
		if (name == "material")
		{
			for (auto mat : child->children())
			{
				auto name = mat->name()->as<cstr_t>();
				if (name == "technique")
				{
					data.technique_name = mat->value()->as<cstr_t>();
				}
				else if (name == "texture")
				{
					data.material.samplers[mat->attributes()["name"]->as<cstr_t>()] = mat->value()->as<cstr_t>();
				}
			}
		}
		else if (name == "buffer")
		{
			auto t = child->attributes()["target"]->as<cstr_t>();
			if (t == "vertices") {
				reflect::vertex_buffer_t buffer = new reflect::vertex_buffer();
				if (!buffer->load(child))
					continue;
				auto desc = buffer->descriptor();
				data.vertex_data.set<0>(array<reflect::attribute_desc_t>(desc.begin(), desc.end()));
				data.vertex_data.set<1>(buffer->array_count());
				data.vertex_data.set<2>(buffer.get());
				data.vertex_data.set<3>(null);
			}
			else if (t == "indices") {
				reflect::struct_buffer_t buffer = new reflect::struct_buffer();
				if (!buffer->load(child))
					continue;
				auto desc = buffer->descriptor();
				data.index_data.set<0>(desc.var_type());
				data.index_data.set<1>(desc.array_count());
				data.index_data.set<2>(buffer.get());
				data.index_data.set<3>(null);
			}
		}
	}
	elements += move(data);

	return;
}

void mesh_loader::load_material_data(ilibrary_t lib, core::files::input_text_file_t file, collections::hash_map<string, material_data>& materials)
{
	cstr_t newmtl_s = "newmtl "_r;
	cstr_t Ka_s = "Ka "_r;
	cstr_t Kd_s = "Kd "_r;
	cstr_t Ks_s = "Ks "_r;
	cstr_t ilum_s = "ilum "_r;
	cstr_t d_s = "d "_r;
	cstr_t Ns_s = "Ns "_r;
	cstr_t map_Kd_s = "map_Kd "_r;
	cstr_t map_bump_s = "map_bump "_r;
	cstr_t map_d_s = "map_d "_r;

	try
	{
		string line = ""_s;
		string name;	
		raw_material mat;
		maths::float3 vec;
		windex i = 0;
		while (!file->is_eos())
		{
			file->read_line(line);
			i = 0;
			text::parser::seek(line, i, " "_r);
			if (line->find(newmtl_s, i, i + newmtl_s.size() + 1) == i)
			{
				if (name != null)
				{
					material_data_t material;
					material.fields = new reflect::struct_buffer();
					auto var = material.fields->make_struct(raw_material::descriptor(), 16);
					var[0].cast<maths::float3>() = (maths::float3)mat.Ka;
					var[1].cast<maths::float3>() = (maths::float3)mat.Kd;
					var[2].cast<maths::float3>() = (maths::float3)mat.Ks;
					var[3].cast<uint>() = mat.ilum;
					var[4].cast<float>() = mat.d;
					var[5].cast<float>() = mat.Ns;

					mat.Ka = { 1,1,1,0 };
					mat.Kd = { 1,1,1,0 };
					mat.Ks = { 1,1,1,0 };
					mat.ilum = 0;
					mat.d = 1;
					mat.Ns = 0;

					if(!mat.map_Kd.is_empty())
						material.samplers["map_Kd"] = ang::move(mat.map_Kd);
					if (!mat.map_Kd.is_empty())
						material.samplers["map_bump"] = ang::move(mat.map_bump);
					if (!mat.map_Kd.is_empty())
						material.samplers["map_d"] = ang::move(mat.map_d);

					materials[name] = move(material);
					name = null;
				}
				name = line->sub_string(newmtl_s.size(), -1);
			}
			else if (line->find(Ka_s, i, i + Ka_s.size() + 1) == i)
			{
				text::parser::seek(line, i, Ka_s);
				vec.set<0>(text::parser::to_value<float>(line, i));
				vec.set<1>(text::parser::to_value<float>(line, i));
				vec.set<2>(text::parser::to_value<float>(line, i));
				mat.Ka = vec;
			}
			else if (line->find(Kd_s, i, i + Kd_s.size() + 1) == i)
			{
				text::parser::seek(line, i, Kd_s);
				vec.set<0>(text::parser::to_value<float>(line, i));
				vec.set<1>(text::parser::to_value<float>(line, i));
				vec.set<2>(text::parser::to_value<float>(line, i));
				mat.Kd = vec;
			}
			else if (line->find(Ks_s, i, i + Ks_s.size() + 1) == i)
			{
				text::parser::seek(line, i, Ks_s);
				vec.set<0>(text::parser::to_value<float>(line, i));
				vec.set<1>(text::parser::to_value<float>(line, i));
				vec.set<2>(text::parser::to_value<float>(line, i));
				mat.Ks = vec;
			}
			else if (line->find(ilum_s, i, i + ilum_s.size() + 1) == i)
			{
				text::parser::seek(line, i, ilum_s);
				mat.ilum = text::parser::to_value<uint>(line, i);
			}
			else if (line->find(d_s, i, i + d_s.size() + 1) == i)
			{
				text::parser::seek(line, i, d_s);
				mat.d = text::parser::to_value<float>(line, i);		
			}
			else if (line->find(Ns_s, i, i + Ns_s.size() + 1) == i)
			{
				text::parser::seek(line, i, Ns_s);
				mat.Ns = text::parser::to_value<float>(line, i);
			}
			else if (line->find(map_Kd_s, i, i + map_Kd_s.size() + 1) == i)
			{
				text::parser::seek(line, i, map_Kd_s);
				mat.map_Kd = line->sub_string(i, -1);
			}
			else if (line->find(map_bump_s, i, i + map_bump_s.size() + 1) == i)
			{
				text::parser::seek(line, i, map_bump_s);
				mat.map_bump = line->sub_string(i, -1);
			}
			else if (line->find(map_d_s, i, i + map_d_s.size() + 1) == i)
			{
				text::parser::seek(line, i, map_d_s);
				mat.map_d = line->sub_string(i, -1);
			}
		}

		if (name != null)
		{
			material_data_t material;
			material.fields = new reflect::struct_buffer();
			auto var = material.fields->make_struct(raw_material::descriptor(), 16);
			var[0].cast<maths::float3>() = (maths::float3)mat.Ka;
			var[1].cast<maths::float3>() = (maths::float3)mat.Kd;
			var[2].cast<maths::float3>() = (maths::float3)mat.Ks;
			var[3].cast<uint>() = mat.ilum;
			var[4].cast<float>() = mat.d;
			var[5].cast<float>() = mat.Ns;

			if (!mat.map_Kd.is_empty())
				material.samplers["map_Kd"] = ang::move(mat.map_Kd);
			if (!mat.map_Kd.is_empty())
				material.samplers["map_bump"] = ang::move(mat.map_bump);
			if (!mat.map_Kd.is_empty())
				material.samplers["map_d"] = ang::move(mat.map_d);

			materials[name] = move(material);
			name = null;
		}

	}
	catch (...)
	{

	}
}