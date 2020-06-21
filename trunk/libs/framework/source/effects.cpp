#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::effects;

/////////////////////////////////////////////////////////////////////////////////////////////

bool load(ilibrary_t lib, dom::xml::ixml_node_t);
bool load_shader(dom::xml::ixml_node_t, shader_info_t& out);

/////////////////////////////////////////////////////////////////////////////////////////////

pass::pass()
{
}

pass::~pass()
{
}

void pass::dispose()
{
	m_resources = null;
	m_shaders = null;
}

effects::ishaders_t pass::shaders()const {
	return m_shaders.get();
}

string pass::frame_buffer()const
{
	return m_frame_buffer;
}

string pass::geometry()const
{
	return m_geometry;
}

array<string> pass::resources()const
{
	return array<string>(m_resources.begin(), m_resources.end());
}

bool pass::load(ilibrary_t lib, dom::xml::ixml_node_t node)
{
	if (node.is_empty() || !node->has_children())
		return false;

	for (auto node : node->children())
	{
		auto name = node->name()->as<cstr_t>();
		if (name == "geometry")
		{
			m_geometry = node->value()->as<cstr_t>();
		}
		else if (name == "render_target")
		{
			m_frame_buffer = node->value()->as<cstr_t>();
		}
		else if (name == "resources")
		{
			if (!node->has_children())
				continue;
			for (auto node : node->children())
			{
				if (name == "resource")
				{
					m_resources += node->value()->as<cstr_t>();
				}
			}
		}
		else if (name == "technique")
		{
			m_shaders = lib->load_resource(node)->cast<resources::resource_type::shaders>();
		}
	}
	return true;
}

bool pass::save(ilibrary_t lib, dom::xml::ixml_document_t doc)const
{
	return false;
}


void pass::draw(idriver_t driver, scenes::iscene_t scene, meshes::igeometry_t geo)
{
	driver_locker_t lock = driver;

	auto indices = geo->index_buffer();
	auto vertices = geo->vertex_buffer();

	driver->bind_shaders(shaders());
	auto transform = scene->active_camera()->transform();
	auto uniforms = m_shaders->map_vs_uniform(driver, "wvp");//world, view and projection matrices
	if (transform->is_stereo())
	{
		auto vpm0 = transform->view_matrix(0) * transform->projection_matrix(0);
		auto vpm1 = transform->view_matrix(1) * transform->projection_matrix(1);
		if (driver->matrix_transpose_needed())//for directx family
		{
			uniforms[0 /*"wm"*/].try_copy<maths::float4x4>(ang::maths::matrix::transpose(geo->transform()));
			uniforms[1 /*"vpm[0]"*/][0].try_copy<maths::float4x4>(ang::maths::matrix::transpose(vpm0));
			uniforms[1 /*"vpm[1]"*/][1].try_copy<maths::float4x4>(ang::maths::matrix::transpose(vpm1));
		}
		else
		{
			uniforms[0 /*"wm"*/].try_copy<maths::float4x4>(geo->transform());
			uniforms[1 /*"vpm[0]"*/][0].try_copy<maths::float4x4>(vpm0);
			uniforms[1 /*"vpm[1]"*/][1].try_copy<maths::float4x4>(vpm1);
		}
	}
	else
	{
		auto vpm = transform->view_matrix() * transform->projection_matrix();
		if (driver->matrix_transpose_needed())//for directx family
		{
			uniforms[0 /*"wm"*/].try_copy<maths::float4x4>(ang::maths::matrix::transpose(geo->transform()));
			uniforms[1 /*"vpm"*/].try_copy<maths::float4x4>(ang::maths::matrix::transpose(vpm));
		}
		else
		{
			uniforms[0 /*"wm"*/].try_copy<maths::float4x4>(geo->transform());
			uniforms[1 /*"vpm"*/].try_copy<maths::float4x4>(vpm);
		}
	}
		
	m_shaders->unmap_vs_uniform(driver, uniforms);

	uniforms = m_shaders->map_ps_uniform(driver, "ligths");
	if (uniforms.raw_data().get() != null)
	{
		auto scene_lights = scene->ligths();

		uniforms[0 /*"ambient"*/].try_copy<maths::float4>(scene->ambient_color());
		uniforms[1 /*"lights_count"*/].try_copy<uint>(scene_lights.size());
		//in material: uniforms[2 /*"specular_power"*/].try_copy<float>(material->field("Ns")); 
		auto lights_uniforms = uniforms[3 /*"lights"*/];//
		if (lights_uniforms.raw_data().data() != null)
		{
			auto lights = lights_uniforms.cast<array_view<reflect::varying>>();
			for (windex i = 0; i < scene_lights.size(); ++i)
			{
				lights[i][0 /*"color"*/].try_copy<maths::float4>(maths::float4(scene_lights[i].color, 1.0f));
				lights[i][1 /*"position"*/].try_copy<maths::float3>(scene_lights[i].info);
				lights[i][2 /*"type"*/].try_copy<uint>((uint)scene_lights[i].type.get());
			}
		}
		m_shaders->unmap_ps_uniform(driver, uniforms);
	}

	windex c = 0;
	auto material = geo->material();
	if (!material.is_empty())
	{
		uniforms = m_shaders->map_ps_uniform(driver, "material");
		for (auto const& field : material->fields())
		{
			auto data1 = field.raw_data();
			auto data2 = uniforms[(cstr_t)field.descriptor().var_name()].raw_data();
			memcpy(data2.data(), data1.data(), min(data1.size(), data2.size()));
		}
		m_shaders->unmap_vs_uniform(driver, uniforms);
	
		for (textures::itexture_t tex : material->textures())
		{
			driver->bind_texture(tex, c);
			m_shaders->bind_texture(driver, c, c);
			c++;
		}
	}
	
	driver->bind_vertex_buffer(vertices);
	m_shaders->bind_vertex_buffer(driver, vertices);
	if (indices.is_empty())
	{
		driver->bind_index_buffer(null);
		driver->draw(vertices->block_counter(), primitive::triangle);
	}
	else
	{
		driver->bind_index_buffer(indices);
		driver->draw_indexed(indices->counter(), primitive::triangle);
	}

	if (!material.is_empty())
	{
		for (windex i = 0; i < c; ++i)
			driver->bind_texture(null, i);
	}

	driver->bind_index_buffer(null);
	driver->bind_vertex_buffer(null);
	driver->bind_shaders(null);
}

///////////////////////////////////////////////////////////////////////////

effect::effect()
{
}

effect::~effect()
{
}

void effect::dispose()
{
	m_resource_sid = null;
	m_passes = null;
	m_fbo_map = null;
}

resources::resource_type_t effect::resource_type()const 
{ 
	return resources::resource_type::effect;
}

string effect::resource_sid()const 
{ 
	return m_resource_sid.get();
}

void effect::resource_sid(cstr_t sid)
{ 
	m_resource_sid = sid;
}

intfptr effect::fast_cast(resources::resource_type_t type)
{
	return type == resources::resource_type::effect ? static_cast<effects::ieffect*>(this) : null;
}

bool effect::load(ilibrary_t lib, dom::xml::ixml_node_t node)
{
	if (node.is_empty() || !node->has_children())
		return false;
	m_resource_sid = node->attributes()["sid"]->as<cstr_t>();
	for (auto node : node->children())
	{
		auto name = node->name()->as<cstr_t>();
		if (name == "file")
		{
			core::files::input_text_file_t file;
			if (!lib->file_system()->open(node->value(), &file))
				return false;
			auto doc = dom::xml::xml_document_from_file(file);
			if (doc.failed())
				return false;
			return load(lib, doc->root_element());
		}
		else if (name == "sources")
		{
			lib->load_sources(node);
		}
		else if (name == "technique")
		{
			lib->load_resource(node);
		}
		else if (name == "render_targets")
		{
			if (!node->has_children())
				continue;
			for (auto node : node->children())
			{
				if(node->name()->as<cstr_t>() == "render_target")
					load_frame_buffer(node);
			}
		}
		else if (name == "pass")
		{
			pass_t pass = new effects::pass();
			if (pass->load(lib, node))
			{
				//auto lock = m_mutex.scope();
				m_passes += pass;
			}
		}
	}

	return true;
}

bool effect::save(ilibrary_t lib, dom::xml::ixml_document_t node)const
{
	return false;
}

bool effect::load_frame_buffer(dom::xml::ixml_node_t node)
{
	if (node.is_empty() || !node->has_children())
		return false;
	frame_buffer_desc_t desc = {
		null,
		textures::tex_format::null,
		{0,0},
		false
	};

	for (auto node : node->children())
	{
		auto name = node->name()->as<cstr_t>();
		if (name == "color_formats")
		{
			for (auto node : node->children())
				if (node->name()->as<cstr_t>() == "color_format")
					desc.color_format += node->value()->as<textures::tex_format_t>();	
		}
		else if (name == "depth_stencil_format")
		{
			desc.depth_stencil_format = node->value()->as<textures::tex_format_t>();
		}
		else if (name == "dimentions")
		{
			desc.dimentions = node->value()->as<size<uint>>();
		}
	}
	return true;
}

resources::iresource_t effect::resource()const
{
	return const_cast<effect*>(this);
}

void effect::draw(idriver_t driver, iframe_buffer_t def_fbo, scenes::iscene_t scene, scenes::inode_t node)
{
	default_fbo(def_fbo);
	//auto lock = m_mutex.scope();
	for (pass_t pass : m_passes) {
		auto geo = (cstr_t)pass->geometry();
		if (geo == "default") {
			iframe_buffer_t fbo	= frame_buffer(pass->frame_buffer(), scene->clip_size());
			if (fbo.is_empty())
				fbo = def_fbo; //default
			driver->bind_frame_buffer(fbo);
			node->draw(driver, scene, fbo, pass.get());
		}
		else// if (geo == "quad"_r) 
		{
			iframe_buffer_t fbo = frame_buffer(pass->frame_buffer(), scene->clip_size());
			if (fbo.is_empty())
				fbo = def_fbo; //default
			
			m_quad_model->clear();
			for (string res : pass->resources()) {
				auto tex = frame_buffer(res);
				if(!tex.is_empty()) for(windex i = 0; i < tex->color_buffer_count(); i++)
					m_quad_model->push_texture(res, tex->color_buffer(i));
			}

			driver->bind_frame_buffer(fbo);
			pass->draw(driver, scene, m_quad_model.get());
		}
	}
	default_fbo(null);
}

wsize effect::pass_count()const
{
	return m_passes.is_empty() ? 0 : m_passes.size();
}

ipass_t effect::pass(windex idx)const
{
	//auto lock = m_mutex.scope();
	return m_passes[idx].get();
}

iframe_buffer_t effect::frame_buffer(cstr_t sid)const
{
	//auto lock = m_mutex.scope();
	auto it = m_fbo_map.find(sid);
	if (!it.is_valid())
		return null;
	if (it->value.fbo == null)
	{
		auto factory = m_library.lock()->factory();
		if (it->value.dimentions == size<uint>(0, 0))
		{
			factory->create_frame_buffer(it->value.color_format, it->value.depth_stencil_format, { 512,512 }, factory->driver()->vr_extension_support(), it->key);
		}
		else
		{
			factory->create_frame_buffer(it->value.color_format, it->value.depth_stencil_format, it->value.dimentions, factory->driver()->vr_extension_support(), it->key);
		}
	}
	return it->value.fbo;
}


iframe_buffer_t effect::frame_buffer(cstr_t sid, size<uint> resize)const
{
	//auto lock = m_mutex.scope();
	auto it = m_fbo_map.find(sid);
	if (!it.is_valid())
		return null;
	if (it->value.fbo == null)
	{
		auto factory = m_library.lock()->factory();
		if (it->value.dimentions == size<uint>(0, 0))
		{
			if (resize.width == 0 || resize.height == 0)
				resize = { 512,512 };//for default
			factory->create_frame_buffer(it->value.color_format, it->value.depth_stencil_format, resize, factory->driver()->vr_extension_support(), it->key);
		}
		else
		{
			factory->create_frame_buffer(it->value.color_format, it->value.depth_stencil_format, it->value.dimentions, factory->driver()->vr_extension_support(), it->key);
		}
	}
	else
	{
		if (it->value.dimentions == size<uint>(0, 0) && resize != size<uint>(0, 0))//use screen size
		{
			if (it->value.fbo->dimentions() != resize)
				if (!it->value.fbo->try_resize(resize)) {
					it->value.fbo = null;
					auto factory = m_library.lock()->factory();
					factory->create_frame_buffer(it->value.color_format, it->value.depth_stencil_format, resize, factory->driver()->vr_extension_support(), it->key);
				}
		}
	}

	return it->value.fbo;
}

iframe_buffer_t effect::default_fbo()const
{
	return effect::frame_buffer("default");
}

void effect::default_fbo(iframe_buffer_t fbo)
{
	if (fbo.is_empty()) {
		//auto lock = m_mutex.scope();
		try {
			m_fbo_map["default"].fbo = null;
		}
		catch(...) {}
	}
	else
	{
		frame_buffer_data_t data;
		//auto lock = m_mutex.scope();
		data.color_format += textures::tex_format::R8G8B8A8;
		data.depth_stencil_format = textures::tex_format::D24S8;
		data.dimentions = fbo->dimentions();
		data.fbo = fbo;
		m_fbo_map["default"] = data;
	}
}

///////////////////////////////////////////////////////////////////////////