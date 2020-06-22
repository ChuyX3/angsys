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

bool material::load(ilibrary_t lib, material_data_t const& data)
{
	ibuffer_t buff = data.fields.get();
	if (!buff.is_empty())
	{
		m_data = to_array((byte*)buff->buffer_ptr(), (byte*)buff->buffer_ptr() + buff->buffer_size());
		m_fields = reflect::varying((array_view<byte> const&)m_data, data.fields->descriptor(), 16);
	}
	//lib->query_interface();
	
	textures::itexture_loader_t tex_loader = interface_cast<textures::itexture_loader>(lib);
	for (auto pair : data.samplers) {
		auto res = tex_loader->find_resource_async(pair.value)->result();
		if (!res.is_empty() && res->resource_type() == resources::resource_type::texture)
			m_textures += {pair.key, res->cast<resources::resource_type::texture>()};
	}
	return true;
}

string material::technique()const
{
	return m_tectnique_name;
}

array<reflect::varying_desc> material::fields_layout()const
{
	int i = 0;
	array<reflect::varying_desc> desc(m_fields.counter());
	for (reflect::varying var : m_fields)
		desc[i++] = var.descriptor();
	return move(desc);
}

array<reflect::varying> material::fields()
{
	return array<reflect::varying>(m_fields.begin(), m_fields.end());
}

reflect::varying material::field(windex idx)
{
	return m_data.size() < idx ? m_fields[idx] : reflect::varying();
}

reflect::varying material::field(cstr_t name)
{
	return m_fields[name];
}

textures::itexture_t material::texture(cstr_t name) const
{
	collections::pair<string, textures::itexture_t> pair;
	if(algorithms::search((array_view<decltype(pair)> const&)m_textures, pair, [&](auto pair)->bool
	{
		return pair.key == name;
	}))
		return pair.value;
	return null;
}

array<textures::itexture_t> material::textures() const
{
	array<textures::itexture_t> res;
	res.size(m_textures.size());
	for (int i = 0; i < res.size(); i++)
		res[i] = m_textures[i].value;
	return move(res);
}

///////////////////////////////////////////////////////////////////////////////////////

quad_material::quad_material()
{

}

quad_material::~quad_material()
{

}


void quad_material::clear()
{

}

void quad_material::dispose()
{
	clear();
}


string quad_material::technique()const
{
	return "default"_s;
}

array<reflect::varying_desc> quad_material::fields_layout()const
{
	return null;
}

array<reflect::varying> quad_material::fields()
{
	return null;
}

reflect::varying quad_material::field(windex)
{
	return reflect::varying();
}

reflect::varying quad_material::field(cstr_t)
{
	return reflect::varying();
}

textures::itexture_t quad_material::texture(cstr_t name) const
{
	collections::pair<string, textures::itexture_t> pair;
	if (algorithms::search((array_view<decltype(pair)> const&)m_textures, pair, [&](auto pair)->bool
	{
		return pair.key == name;
	}))
		return pair.value;
	return null;
}

array<textures::itexture_t> quad_material::textures() const
{
	array<textures::itexture_t> res;
	res.size(m_textures.size());
	for (int i = 0; i < res.size(); i++)
		res[i] = m_textures[i].value;
	return move(res);
}

void quad_material::push_texture(cstr_t name, textures::itexture_t tex)
{
	m_textures += {name, tex};
}

	
			