#include "pch.h"
#include <ang/framework/framework.h>


using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;
using namespace ang::graphics::meshes;

geometry::geometry()
{

}

void geometry::dispose()
{
	m_index_buffer = null;
	m_vertex_buffer = null;
	m_material = null;
}

bool geometry::load(ifactory_t lib, geometry_data_t const& data)
{
	//data.index_data->descriptor();

	return false;
}

string geometry::technique() const
{
	return "default"_s;
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
	return m_material;
}

geometry::~geometry()
{

}