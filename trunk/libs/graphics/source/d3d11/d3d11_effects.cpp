#include "pch.h"
#include "d3d11/d3d11_driver.h"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

#if DIRECTX_SUPPORT

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::d3d11;


/////////////////////////////////////////////////////////////////////////

d3d11_pass::d3d11_pass()
{

}

d3d11_pass::~d3d11_pass()
{

}

COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_pass);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_pass, object, effects::ipass);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_pass, object, effects::ipass);


bool d3d11_pass::load(d3d11_effect_library_t, dom::xml::xml_node_t)
{
	return false;
}

windex d3d11_pass::pass_index()const
{
	return 0;
}

text::istring_view_t d3d11_pass::render_target()const
{
	return null;
}

effects::itechnique_t d3d11_pass::tenchnique()const
{
	return null;
}


//////////////////////////////////////////////////////////////////

d3d11_effect::d3d11_effect()
{
	m_fbo_map["default"] = frame_buffer_data();
}

d3d11_effect::~d3d11_effect()
{

}

COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_effect);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_effect, object, effects::ieffect);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_effect, object, effects::ieffect);

void d3d11_effect::clear()
{
	m_effect_name = null;
	m_square_model.indices = null;
	m_square_model.vertices = null;
	m_fx_library = null;
	m_passes = null;
	m_fbo_map = null;
}

bool d3d11_effect::load(d3d11_effect_library_t fxlib, dom::xml::xml_node_t node)
{
	m_fx_library = fxlib;
	if (node.is_empty() || !node->xml_has_children())
		return false;
	
	m_effect_name = node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>();
	for (dom::xml::xml_node_t node : node->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "render_targets"_s) 
			for (dom::xml::xml_node_t node : node->xml_children())
				if (node->xml_name()->xml_as<text::raw_cstr>() == "render_target"_s)
					load_render_target(fxlib, node);
		else if (name == "pass"_s)
			load_pass(fxlib, node);
	}
	return true;
}

bool d3d11_effect::load_pass(d3d11_effect_library_t, dom::xml::xml_node_t)
{
	return false;
}

bool d3d11_effect::load_render_target(d3d11_effect_library_t, dom::xml::xml_node_t)
{
	return false;
}

text::istring_view_t d3d11_effect::resource_sid()const 
{
	return m_effect_name.get();
}

void d3d11_effect::draw(effects::draw_context_t& context)
{
	d3d11_effect_library_t library = m_fx_library.lock();
	d3d11_driver_t driver = library->driver();
	driver->execute_on_thread_safe([&]()
	{
		bool first = true;
		bool second = true;
		m_fbo_map["default"].fbo = interface_cast<d3d11_frame_buffer>(context.frame_buffer.get());

		uint pass_count = 0;

		for (d3d11_pass_t pass : m_passes)
		{
			if (first) {
				first = false;
				context.first_pass(pass->tenchnique(), find_render_target(pass->render_target()).get());
				continue;
			}//end if

			if (second) {
				second = false;	
				driver->bind_index_buffer(m_square_model.indices);
				driver->bind_vertex_buffer(m_square_model.vertices);
			}//end if

			effects::itechnique_t technique = pass->tenchnique();
			driver->bind_frame_buffer(find_render_target(pass->render_target()).get());
			driver->bind_technique(technique);
			driver->clear(colors::gray);
			
			auto uniform = technique->map_vs_uniform(driver.get(), "w"_s);
			uniform["world"].cast<maths::float4x4>() = ang::maths::matrix::identity();
			technique->unmap_ps_uniform(driver.get(), uniform);

			
			if (context.vertex_shader_uniforms.data() != null &&
				context.vertex_shader_uniforms[pass_count].raw_data().data() != null)
			{
				auto& src_data = context.vertex_shader_uniforms[pass_count];
				
				uniform = technique->map_vs_uniform(driver.get(), (text::raw_cstr)src_data.descriptor().var_name());
				if (uniform.raw_data().data() != null)
				{
					auto data = uniform.raw_data();
					memcpy(data.data(), src_data.raw_data().data(), min(data.size(), src_data.raw_data().size()));
					technique->unmap_vs_uniform(driver.get(), uniform);
				}

			}

			if (context.pixel_shader_uniforms.data() != null &&
				context.pixel_shader_uniforms[pass_count].raw_data().data() != null)
			{
				auto& src_data = context.pixel_shader_uniforms[pass_count];

				uniform = technique->map_ps_uniform(driver.get(), (text::raw_cstr)src_data.descriptor().var_name());
				if (uniform.raw_data().data() != null)
				{
					auto data = uniform.raw_data();
					memcpy(data.data(), src_data.raw_data().data(), min(data.size(), src_data.raw_data().size()));
					technique->unmap_ps_uniform(driver.get(), uniform);
				}
			}


			uint i = 0;
			for (text::istring_view_t tex_sid : technique->textures()) {
				auto fbo = find_render_target(tex_sid, false);
				for (uint j = 0, c = fbo->color_buffer_count(); j < c; j++) 
					driver->bind_texture(fbo->color_buffer(j), i++);
			}

			driver->draw_indexed(m_square_model.indices->counter(), primitive::triangle);

			for (; i > 0; i--) {
				driver->bind_texture(null, i - 1);
			}
			pass_count++;
		}//end for
	});

	m_fbo_map["default"].fbo = null;
}

iframe_buffer_t d3d11_effect::render_target(text::istring_view_t)const
{
	return null;
}

effects::ipass_t d3d11_effect::pass(index)const
{
	return null;
}

void d3d11_effect::trim()
{
	for (collections::pair<astring, frame_buffer_data_t> pair : m_fbo_map)
	{
		if (!pair.value.fbo.is_empty())
			pair.value.fbo->clear();
	}
}

d3d11_frame_buffer_t d3d11_effect::find_render_target(text::raw_cstr_t key, bool force, size<uint> size)const
{
	auto it = m_fbo_map->find(key);
	if (!it.is_valid())
		return null;

	d3d11_frame_buffer_t fbo = it->value.fbo;

	if (fbo.is_empty() && force)
	{
		d3d11_effect_library_t library = m_fx_library.lock();
		d3d11_driver_t driver = library->driver();
		fbo = new d3d11_frame_buffer(driver.get());
		auto dim = it->value.dimentions;
		if (dim.width == uint(-1) || dim.height == uint(-1))
			dim = size;
		if (dim.width == uint(-1) || dim.height == uint(-1))
			dim = { 512,512 }; //fixed size render taget, low resolution
		fbo->create(it->value.color_format, it->value.depth_stencil_format, dim);
		it->value.fbo = null;
		return fbo.get();
	}
	else if (fbo->D3DRenderTargetView(0) == null && force) {
		auto dim = it->value.dimentions;
		if (dim.width == uint(-1) || dim.height == uint(-1))
			dim = size;
		if (dim.width == uint(-1) || dim.height == uint(-1))
			dim = { 512,512 }; //fixed size render taget, low resolution
		fbo->create(it->value.color_format, it->value.depth_stencil_format, dim);
		return fbo.get();
	}
	return null;
}

//////////////////////////////////////////////////////////////////


d3d11_effect_library::d3d11_effect_library(d3d11_driver_t parent)
	: m_mutex(make_shared<core::async::mutex>())
	, m_driver(ang::move(parent))
{

}

d3d11_effect_library::~d3d11_effect_library()
{

}

COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::d3d11::d3d11_effect_library);
COFFE_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::d3d11::d3d11_effect_library, object, effects::ieffect_library);
COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::d3d11::d3d11_effect_library, object, effects::ieffect_library);

void d3d11_effect_library::clear()
{
	clear_data();
	m_driver = null;
	m_fs = null;
	m_mutex = null;
}

void d3d11_effect_library::trim()
{
	for (collections::pair<astring,d3d11_effect_t> pair : m_effects)
	{
		if (!pair.value.is_empty())
			pair.value->trim();
	}
}

void d3d11_effect_library::clear_data(void)
{
	m_source_map.reset();
	m_shaders.reset();
	m_effects.reset();
}

void d3d11_effect_library::set_file_system(core::files::ifile_system_t fs) { m_fs = fs; }

bool d3d11_effect_library::load_library(dom::xml::xml_node_t library)
{
	if (library.is_empty() || !library->xml_has_children())
		return false;

	for(dom::xml::xml_node_t node : library->xml_children())
	{
		auto name = node->xml_name()->xml_as<text::raw_cstr>();
		if (name == "sources"_s)
			load_sources(node);
		else if (name == "effect"_s)
			load_effect(node);
		else if (name == "technique"_s)
			load_technique(node);
	}
	return true;
}

core::async::iasync<effects::ieffect_library_t> d3d11_effect_library::load_library_async(dom::xml::xml_node_t library)
{
	return core::async::task::run_async<effects::ieffect_library_t>([=](core::async::iasync<effects::ieffect_library_t> async)->effects::ieffect_library_t
	{
		if (library.is_empty() || !library->xml_has_children())
		{
			async->cancel();
			return null;
		}	

		for(dom::xml::xml_node_t node : library->xml_children())
		{
			auto name = node->xml_name()->xml_as<text::raw_cstr>();
			if (name == "sources"_s)
				load_sources(node);
			else if (name == "effect"_s)
				load_effect(node);
			else if (name == "technique"_s)
				load_technique(node);
		}
		return this;
	});
}

bool d3d11_effect_library::load_sources(dom::xml::xml_node_t sources)
{
	if (!sources->xml_has_children())
		return false;

	for(dom::xml::xml_node_t file : sources->xml_children())
	{
		auto name = file->xml_name()->xml_as<text::raw_cstr>();
		if (name != "file"_s)
			continue;
		m_mutex->lock();
		m_source_map += {file->xml_attributes()["sid"_s]->xml_as<text::raw_cstr>(), file->xml_value()->xml_as<text::raw_cstr>()};
		m_mutex->unlock();
	}
	return true;
}

effects::ieffect_t d3d11_effect_library::load_effect(dom::xml::xml_node_t node)
{
	//TODO:
	return null;
}

core::async::iasync<effects::ieffect_t> d3d11_effect_library::load_effect_async(dom::xml::xml_node_t node)
{
	//TODO:
	return null;
}

effects::itechnique_t d3d11_effect_library::load_technique(dom::xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return null;
	d3d11_technique_t shaders = new d3d11_technique();
	if (!shaders->load(this, node))
		return null;
	m_mutex->lock();
	m_shaders += {node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>(), shaders.get() };
	m_mutex->unlock();
	return shaders.get();
}


core::async::iasync<effects::itechnique_t> d3d11_effect_library::load_technique_async(dom::xml::xml_node_t node)
{
	return core::async::task::run_async<effects::itechnique_t>([=](core::async::iasync<effects::itechnique_t> async)->effects::itechnique_t
	{
		if (!node->xml_has_children())
			return null;
		d3d11_technique_t shaders = new d3d11_technique();
		if (!shaders->load(this, node))
			return null;
		m_mutex->lock();
		m_shaders += {node->xml_attributes()["name"_s]->xml_as<text::raw_cstr>(), shaders.get() };
		m_mutex->unlock();
		return shaders.get();
	});
}


effects::ieffect_t d3d11_effect_library::find_effect(text::raw_cstr_t name)const
{
	return null;
}

effects::itechnique_t d3d11_effect_library::find_technique(text::raw_cstr name)const
{
	if (m_shaders.is_empty())
		return null;
	auto it = m_shaders->find(name);
	return it.is_valid() ? it->value.get() : null;
}

#endif
