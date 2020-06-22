#include "pch.h"
#include <ang/framework/framework.h>


using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;
using namespace ang::graphics::meshes;

geometry::geometry()
	: m_transform()
	, m_index_buffer()
	, m_vertex_buffer()
	, m_material()
{

}

geometry::~geometry()
{

}

void geometry::dispose()
{
	m_index_buffer = null;
	m_vertex_buffer = null;
	m_material = null;
}

bool geometry::load(ilibrary_t lib, geometry_data_t const& data)
{
	auto vertex = lib->factory()->create_vertex_buffer(
		graphics::buffers::buffer_usage::def, 
		data.vertex_data.get<0>(),
		data.vertex_data.get<1>(),
		data.vertex_data.get<2>(), 
		data.vertex_data.get<3>());

	auto index = lib->factory()->create_index_buffer(
		graphics::buffers::buffer_usage::def,
		data.index_data.get<0>(),
		data.index_data.get<1>(),
		data.index_data.get<2>(),
		data.index_data.get<3>());
	if(index.successed())
		m_index_buffer = index.get();
	if(vertex.successed())
		m_vertex_buffer = vertex.get();

	meshes::material_t mat = new meshes::material();
	if (mat->load(lib, data.material))
		m_material = mat;

	return true;
}

string geometry::technique() const
{
	return m_material.is_empty()? "default"_s : m_material->technique();
}

buffers::iindex_buffer_t geometry::index_buffer() const
{
	return m_index_buffer;
}

buffers::ivertex_buffer_t geometry::vertex_buffer() const
{
	return m_vertex_buffer;
}

void geometry::transform(maths::mat4 const& m)
{
	m_transform = m;
}

maths::mat4 const& geometry::transform() const
{
	return m_transform;
}

imaterial_t geometry::material() const
{
	return m_material.get();
}

////////////////////////////////////////////////////////////////////////////////

quad_geometry::quad_geometry()
	: m_transform()
	, m_material(new quad_material())
	, m_vertex_buffer()
{

}

quad_geometry::~quad_geometry()
{

}


void quad_geometry::dispose()
{
	clear();
	m_material = null;
}


void quad_geometry::clear()
{
	m_material->clear();
	m_vertex_buffer = null;
}


			
string quad_geometry::technique() const
{
	return "default"_s;
}

buffers::iindex_buffer_t quad_geometry::index_buffer() const
{
	return null;
}

buffers::ivertex_buffer_t quad_geometry::vertex_buffer() const
{
	return m_vertex_buffer.get();
}

meshes::imaterial_t quad_geometry::material() const
{
	return m_material.get();
}

void quad_geometry::transform(maths::mat4 const& m)
{
	m_transform = m;
}

maths::mat4 const& quad_geometry::transform() const
{
	return m_transform;
}

void quad_geometry::push_texture(cstr_t name, textures::itexture_t tex)
{
	m_material->push_texture(name, tex);
}
