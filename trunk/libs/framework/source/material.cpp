#include "pch.h"
#include <ang/framework/framework.h>


using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;
using namespace ang::graphics::meshes;

material::material()
{

}

material::~material()
{

}

void material::dispose()
{
	m_tectnique_name = null;
	m_textures.clear();
}

string material::technique()const
{
	return m_tectnique_name;
}

array<reflect::varying_desc> material::fields_layout()const
{
	int i = 0;
	array<reflect::varying_desc> desc(m_data.size());
	for (reflect::varying var : m_data)
		desc[i++] = var.descriptor();
	return move(desc);
}

array<reflect::varying> material::fields()
{
	return array<reflect::varying>(m_data.begin(), m_data.end());
}

reflect::varying material::field(windex idx)
{
	return m_data.size() < idx ? m_data[idx] : reflect::varying();
}

reflect::varying material::field(cstr_t name)
{
	for (reflect::varying var : m_data) {
		var.find_field(name);
	}
}

array_view<textures::itexture_t> material::textures() const
{

}
