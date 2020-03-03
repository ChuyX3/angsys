#include "pch.h"
#include "scenes/model_loader.h"

#if defined _DEBUG
#define new new(__FILE__,__LINE__)
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;


static const uint _magic_word = 'MDLO';

#define MY_TYPE ordered_model
#include <ang/inline/object_wrapper_specialization.inl>
#undef MY_TYPE

ordered_model::ordered_model()
{

}

ordered_model::~ordered_model()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::scenes::ordered_model);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::scenes::ordered_model, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::scenes::ordered_model, object);

vector<ordered_model::model_element> ordered_model::load(core::files::input_text_file_t file)
{
	m_elements = null;
	try
	{
		istring_t line = ""_s;
		streams::itext_input_stream_t stream;
		streams::string_input_stream_t wrapper = new streams::string_input_stream(null);
		stream = wrapper.get();

		cstr_t v_s = "v ";
		cstr_t vt_s = "vt ";
		cstr_t vn_s = "vn ";
		cstr_t f_s = "f ";
		cstr_t mtllib_s = "mtllib ";
		cstr_t usemtl_s = "usemtl ";

		istring_t material;
		model_element model;
		vector<istring_t> material_lib;
		model.vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();

		maths::matrix4 rotation = maths::matrix::rotation_x(-3.141592f / 2.0f);

		maths::unaligned_float3 vec;
		vector<maths::unaligned_float3> vertices;
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
				vertices += maths::unaligned_float3{ vec2.get<0>(), vec2.get<1>(), vec2.get<2>() };
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
				index face[3] = { 0,0,0 };
				vertex vertex[3];
				bool has_normal = false;
				stream >> f_s >> face[0] >> "/"_s >> face[1] >> "/"_s >> face[2];
				if (face[0] > 0)
				{
					vec = vertices[face[0] - 1];
					vertex[0].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
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
					vertex[0].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
				}

				stream >> face[0] >> "/"_s >> face[1] >> "/"_s >> face[2];
				if (face[0] > 0)
				{
					vec = vertices[face[0] - 1];
					vertex[1].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
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
					vertex[1].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
				}

				stream >> face[0] >> "/"_s >> face[1] >> "/"_s >> face[2];
				if (face[0] > 0)
				{
					vec = vertices[face[0] - 1];
					vertex[2].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
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
					vertex[2].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
				}

				if (!has_normal)
				{
					maths::float3 normal = maths::float3(vertex[2].position - vertex[1].position) % maths::float3(vertex[1].position - vertex[0].position);
					vertex[0].normal = vertex[1].normal = vertex[2].normal = maths::float4(maths::norm(normal), 0);
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
					m_elements += ang::move(model);
					model.vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();
				}
				line->sub_string(&material, usemtl_s.size(), -1);
			}
			else if (line->find(mtllib_s, 0, mtllib_s.size() + 1) == 0)
			{
				istring_t mtl;
				line->sub_string(&mtl, mtllib_s.size(), - 1);
				material_lib += mtl;
			}
		}
		if (!material.is_empty())
		{
			model.material = ang::move(material);
			m_elements += ang::move(model);
			//model.vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();
		}
		return m_elements;
	}
	catch (exception_t e)
	{
		return null;
	}
}


core::async::iasync<vector<ordered_model::model_element>> ordered_model::load_async(core::files::input_text_file_t file)
{
	return core::async::task::run_async<vector<ordered_model::model_element>>(
		[&](core::async::iasync<vector<ordered_model::model_element>> task)-> vector<ordered_model::model_element>
	{
		m_elements = null;
		try
		{
			istring_t line = ""_s;
			streams::itext_input_stream_t stream;
			streams::string_input_stream_t wrapper = new streams::string_input_stream(null);
			stream = wrapper.get();

			cstr_t v_s = "v ";
			cstr_t vt_s = "vt ";
			cstr_t vn_s = "vn ";
			cstr_t f_s = "f ";
			cstr_t mtllib_s = "mtllib ";
			cstr_t usemtl_s = "usemtl ";

			istring_t material;
			model_element model;
			vector<istring_t> material_lib;
			model.vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();

			maths::matrix4 rotation = maths::matrix::rotation_x(-3.141592f / 2.0f);

			maths::unaligned_float3 vec;
			vector<maths::unaligned_float3> vertices;
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
					vertices += maths::unaligned_float3{ vec2.get<0>(), vec2.get<1>(), vec2.get<2>() };
				}
				else if (line->find(vn_s, 0, vn_s.size() + 1) == 0)
				{
					stream >> vn_s >> vec.get<0>() >> vec.get<1>() >> vec.get<2>();
					normals += vec;
				}
				else  if (line->find(vt_s, 0, vt_s.size() + 1) == 0)
				{
					stream >> vt_s >> vec.get<0>() >> vec.get<1>();
					texcoords += maths::unaligned_float2{ vec.get<0>(), vec.get<1>() };
				}
				else  if (line->find(f_s, 0, f_s.size() + 1) == 0)
				{
					index face[3] = { 0,0,0 };
					vertex vertex[3];
					bool has_normal = false;
					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						vec = vertices[face[0] - 1];
						vertex[0].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
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
						vertex[0].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
					}

					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						vec = vertices[face[0] - 1];
						vertex[1].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
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
						vertex[1].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
					}

					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						vec = vertices[face[0] - 1];
						vertex[2].position = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 1 };
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
						vertex[2].normal = { vec.get<0>(), vec.get<1>(), vec.get<2>(), 0 };
					}

					if (!has_normal)
					{
						maths::float3 normal = maths::float3(vertex[2].position - vertex[1].position) % maths::float3(vertex[1].position - vertex[0].position);
						vertex[0].normal = vertex[1].normal = vertex[2].normal = maths::float4(maths::norm(normal), 0);
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
						m_elements += ang::move(model);
						model.vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();
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
				model.material = ang::move(material);
				m_elements += ang::move(model);
				//model.vertices = new collections::vector_buffer<vertex, memory::aligned16_allocator>();
			}
			return m_elements;
		}
		catch (exception_t e)
		{
			return null;
		}
	});
}


vector<ordered_model::model_element> ordered_model::load(core::files::input_binary_file_t file)
{
	return null;
}

core::async::iasync<vector<ordered_model::model_element>> ordered_model::load_async(core::files::input_binary_file_t file)
{
	return null;
}


bool ordered_model::save(core::files::output_binary_file_t file)
{
	//ulong64 _size = 0;
	//if (!file->write([&](streams::ibinary_output_stream_t stream)
	//{
	//	if (m_elements.is_empty())
	//		return false;
	//	stream << _magic_word << m_elements->counter();
	//
	//	for(model_element& element : m_elements)
	//	{
	//		stream << element.material;
	//		if (element.vertices.is_empty())
	//			stream << 0U;
	//		else {
	//			stream << element.vertices->counter();
	//			for(vertex& v : element.vertices)
	//			{
	//				stream->write(&v, sizeof(vertex));
	//			}
	//		}
	//	}
	//	_size = (ulong64)stream->position();
	//	return true;
	//}, 0, 8)) return false;
	//
	//file->cu(_size);

	return true;
}