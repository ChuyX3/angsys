#include "pch.h"
#include "scenes/model_loader.h"

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::scenes;


static const uint _magic_word = 'MDLO';


ordered_model::ordered_model()
{

}

ordered_model::~ordered_model()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::graphics::scenes::ordered_model, object)

collections::vector<ordered_model::model_element> ordered_model::load(core::files::input_text_file_t file)
{
	_elements = null;
	try
	{
		return file->read([&](streams::itext_input_stream_t stream)->bool
		{
			ibuffer_t _buffer = interface_cast<ibuffer>(stream.get());
			cstr_t data;
			if (_buffer.is_empty())
				return false;
			data.set((char*)_buffer->buffer_ptr(), _buffer->buffer_size() - 1);

			index idx = 0;
			maths::unaligned_float3 temp1;

			cstr_t v_s = "v ";
			cstr_t vt_s = "vt ";
			cstr_t vn_s = "vn ";
			cstr_t f_s = "f ";
			cstr_t mtllib_s = "mtllib ";
			cstr_t usemtl_s = "usemtl ";

			string material;
			model_element model;
			model.vertices = new collections::vector_buffer<vertex>();
			model.vertices->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));

			collections::vector<maths::unaligned_float3> vertices;
			collections::vector<maths::unaligned_float3> normals;
			collections::vector<maths::unaligned_float2> texcoords;

			collections::vector<model_element> out = new collections::vector_buffer<model_element>();

			cstr_t debug_view;

			while (data.size() > idx)
			{
				//debug_view.set(&data[idx], data.size() - idx);
				if (data.find(v_s, idx, idx + v_s.size()) == idx)
				{
					stream->move_to(idx + v_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y >> temp1._vector.z;
					vertices += temp1;
				}
				else if (data.find(vn_s, idx, idx + vn_s.size()) == idx)
				{
					stream->move_to(idx + vn_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y >> temp1._vector.z;
					normals += temp1;
				}
				else if (data.find(vt_s, idx, idx + vt_s.size()) == idx)
				{
					stream->move_to(idx + vt_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y;
					texcoords += {temp1._vector.x, temp1._vector.y};
				}
				else if (data.find(f_s, idx, idx + f_s.size()) == idx)
				{
					stream->move_to(idx + f_s.size(), streams::stream_reference::begin);
					index face[3] = { 0,0,0 };
					vertex vertex;
					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.vertices += vertex;

					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.vertices += vertex;

					stream >> face[0] >> "/" >> face[1] >> "/" >> face[2];
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 };
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = { temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 };
					}
					model.vertices += vertex;

				}
				else if (data.find(usemtl_s, idx, idx + usemtl_s.size()) == idx)
				{
					if (!material.is_empty())
					{
						model.material = ang::move(material);
						_elements += ang::move(model);
						model.vertices = new collections::vector_buffer<vertex>();
						model.vertices->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
					}

					auto end = data.find('\n', idx);
					if (end == invalid_index)
						break; //error

					data.sub_string(material, idx + usemtl_s.size(), end - idx - usemtl_s.size() - 1);
				}

				idx = data.find('\n', idx);
				if (idx != invalid_index)idx++;
			}
			if (!material.is_empty())
			{
				model.material = ang::move(material);
				_elements += ang::move(model);
				model.vertices = new collections::vector_buffer<vertex>();
				model.vertices->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
			}
			return true;
		}) ? _elements.get() : null;
	}
	catch (exception_t)
	{
		return null;
	}
}


core::async::iasync_t<collections::vector<ordered_model::model_element>> ordered_model::load_async(core::files::input_text_file_t file)
{
	_elements = null;
	try
	{
		return file->read_async<collections::vector<ordered_model::model_element>>([&](streams::itext_input_stream_t stream)->collections::vector<ordered_model::model_element>
		{
			ibuffer_t _buffer = interface_cast<ibuffer>(stream.get());
			cstr_t data;
			if (_buffer.is_empty())
				return null;
			data.set((char*)_buffer->buffer_ptr(), _buffer->buffer_size() - 1);

			index idx = 0;
			maths::unaligned_float3 temp1;

			cstr_t v_s = "v ";
			cstr_t vt_s = "vt ";
			cstr_t vn_s = "vn ";
			cstr_t f_s = "f ";
			cstr_t mtllib_s = "mtllib ";
			cstr_t usemtl_s = "usemtl ";

			string material;
			collections::vector<string> material_lib;
			model_element model;
			model.vertices = new collections::vector_buffer<vertex>();
			model.vertices->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));

			collections::vector<maths::unaligned_float3> vertices;
			collections::vector<maths::unaligned_float3> normals;
			collections::vector<maths::unaligned_float2> texcoords;

			collections::vector<model_element> out = new collections::vector_buffer<model_element>();

			cstr_t debug_view;

			maths::matrix4 world = maths::matrix::identity() * maths::matrix::rotation_x(3.141592f * -90.0f / 180.0f);

			while (data.size() > idx)
			{
				//debug_view.set(&data[idx], data.size() - idx);
				if (data.find(v_s, idx, idx + v_s.size()) == idx)
				{
					stream->move_to(idx + v_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y >> temp1._vector.z;
					vertices += temp1;
				}
				else if (data.find(vn_s, idx, idx + vn_s.size()) == idx)
				{
					stream->move_to(idx + vn_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y >> temp1._vector.z;
					normals += temp1;
				}
				else if (data.find(vt_s, idx, idx + vt_s.size()) == idx)
				{
					stream->move_to(idx + vt_s.size(), streams::stream_reference::begin);
					stream >> temp1._vector.x >> temp1._vector.y;
					texcoords += {temp1._vector.x, temp1._vector.y};
				}
				else if (data.find(f_s, idx, idx + f_s.size()) == idx)
				{
					stream->move_to(idx + f_s.size(), streams::stream_reference::begin);
					index face[3] = { 0,0,0 };
					vertex vertex;

					stream->read(face[0]);
					stream->read("/");
					stream->read(face[1]);
					if(stream->read("/"))
						stream->read(face[2]);
			
					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = maths::float4{ temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 } * world;
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = maths::float4{ temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 } * world;
					}
					model.vertices += vertex;

					stream->read(face[0]);
					stream->read("/");
					stream->read(face[1]);
					if (stream->read("/"))
						stream->read(face[2]);

					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = maths::float4{ temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 } * world;
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = maths::float4{ temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 } * world;
					}
					model.vertices += vertex;

					stream->read(face[0]);
					stream->read("/");
					stream->read(face[1]);
					if (stream->read("/"))
						stream->read(face[2]);

					if (face[0] > 0)
					{
						temp1 = vertices[face[0] - 1];
						vertex.position = maths::float4{ temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 1 } * world;
					}
					if (face[1] > 0)
					{
						temp1 = { texcoords[face[1] - 1], 0 };
						vertex.texcoord = { temp1.get<0>(), temp1.get<1>() };
					}
					if (face[2] > 0)
					{
						temp1 = normals[face[2] - 1];
						vertex.normal = maths::float4{ temp1.get<0>(), temp1.get<1>(), temp1.get<2>(), 0 } * world;
					}
					model.vertices += vertex;

				}
				else if (data.find(usemtl_s, idx, idx + usemtl_s.size()) == idx)
				{
					if (!material.is_empty())
					{
						model.material = ang::move(material);
						_elements += ang::move(model);
						model.vertices = new collections::vector_buffer<vertex>();
						model.vertices->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
					}

					auto end = data.find('\n', idx);
					if (end == invalid_index)
						break; //error

					data.sub_string(material, idx + usemtl_s.size(), end - idx - usemtl_s.size() - 1);
				}
				else if (data.find(mtllib_s, idx, idx + mtllib_s.size()) == idx)
				{
					auto end = data.find('\n', idx);
					if (end == invalid_index)
						break; //error
					material_lib += ""_s;
					data.sub_string(*material_lib->end(), idx + mtllib_s.size(), end - idx - usemtl_s.size() - 1);
				}

				idx = data.find('\n', idx);
				if (idx != invalid_index)idx++;
			}
			if (!material.is_empty())
			{
				model.material = ang::move(material);
				_elements += ang::move(model);
				model.vertices = new collections::vector_buffer<vertex>();
				model.vertices->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
			}
			return _elements.get();
		});
	}
	catch (exception_t)
	{
		return null;
	}
}


collections::vector<ordered_model::model_element> ordered_model::load(core::files::input_binary_file_t file)
{
	ulong64 _size = 0;
	file->read([&](streams::ibinary_input_stream_t stream) 
	{
		uint info;
		_elements = null;

		stream >> info;
		if (info != _magic_word)
			return false;
		uint element_count = 0;
		stream >> element_count;
		model_element element;
		for (index i = 0; i < element_count; ++i)
		{
			vertex _v;
			uint vertex_count = 0;
			stream >> element.material;
			stream >> vertex_count;
			element.vertices = new collections::vector_buffer<vertex>();
			element.vertices->set_allocator(memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator));
			for (index j = 0; j < vertex_count; ++j)
			{
				stream->read(&_v, sizeof(vertex));
				element.vertices += _v;
			}
			_elements += element;
			element.material = null;
			element.vertices = null;
		}
	});

	return _elements;
}


bool ordered_model::save(core::files::output_binary_file_t file)
{
	ulong64 _size = 0;
	if (!file->write([&](streams::ibinary_output_stream_t stream)
	{
		if (_elements.is_empty())
			return false;
		stream << _magic_word << _elements->counter();

		foreach(_elements, [&](model_element& element)
		{
			stream << element.material;
			if (element.vertices.is_empty())
				stream << 0U;
			else {
				stream << element.vertices->counter();
				foreach(element.vertices, [&](vertex& v)
				{
					stream->write(&v, sizeof(vertex));
				});
			}
		});
		_size = (ulong64)stream->position();
		return true;
	}, 0, 8)) return false;

	file->file_size(_size);

	return true;
}