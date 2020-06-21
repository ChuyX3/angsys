#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::meshes;

/////////////////////////////////////////////////////////////////////////////////////////////

mesh::mesh()
{
}

mesh::~mesh()
{
}

void mesh::dispose()
{

}

bool mesh::load(ilibrary_t lib, dom::xml::ixml_node_t node)
{
	m_resource_id = null;
	auto data = mesh_loader::load_data(lib, node, &m_resource_id);
	if (data.is_empty())
		return false;
	
	auto factory = lib->factory();
	for (auto& data : data)
	{
		geometry_t geo = new geometry();
		if (geo->load(lib, data))
			m_geometries += geo.get();
	}

	return true;
}

string mesh::resource_sid()const
{
	return m_resource_id;
}

void mesh::resource_sid(cstr_t sid)
{
	m_resource_id = sid;
}

array<igeometry_t> mesh::elements()const
{
	return array<igeometry_t>(m_geometries.begin(), m_geometries.end());
}

resources::iresource_t mesh::resource()const
{
	return const_cast<mesh*>(this);
}

resource_type_t mesh::resource_type()const
{
	return resource_type::mesh;
}

intfptr mesh::fast_cast(resource_type_t type)
{
	return type == resource_type::mesh ? (imesh*)this : null;
}
