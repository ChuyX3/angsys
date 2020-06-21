#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::effects;

/////////////////////////////////////////////////////////////////////////////////////////////

effect_library::effect_library(ilibrary* parent)
	: m_parent(parent)
	, m_effect_map()
	, m_technique_map()
{
	throw_exception_if(parent == null, error_code::invalid_param);
}

effect_library::~effect_library()
{
}

void effect_library::dispose()
{
	clear();
	m_parent = null;
}

void effect_library::clear()
{
	m_mutex.lock();
	m_effect_map.clear();
	m_technique_map.clear();
	m_mutex.unlock();
}

ifactory_t effect_library::factory()const
{
	return m_parent.lock()->factory();
}

core::files::ifile_system_t effect_library::file_system()const
{
	return parent()->file_system();
}

core::async::idispatcher_t effect_library::dispatcher()const
{
	return parent()->dispatcher();
}

bool effect_library::load(dom::xml::ixml_node_t data)
{
	if (data.is_empty() || !data->has_children())
		return false;

	for (dom::xml::ixml_node_t node : data->children())
	{
		auto name = node->name()->as<cstr_t>();
		if (name == "sources")
		{
			load_sources(node);
		}
		else if (name == "technique")
		{
			load_technique(node);
		}
		else if (name == "effect")
		{
			load_effect(node);
		}
		else if (name == "file")
		{
			core::files::input_text_file_t file;
			optional<dom::xml::ixml_document> doc;
			if (file_system()->open(node->value()->as<cstr_t>(), &file)
				&& (doc = dom::xml::xml_document_from_file(file)).successed())
			{
				file->dispose();
				load(doc->root_element());
			}
		}
	}
	return true;
}

bool effect_library::save(dom::xml::ixml_document_t doc)const
{
	dom::xml::ixml_document_builder_t builde = doc->builder();
	builde->begin_element("effect_library");
	{
		//TODO:
	}builde->end_element();

	return true;
}

core::async::iasync<bool> effect_library::load_async(dom::xml::ixml_node_t node)
{
	return create_task<bool>([=](core::async::iasync<bool>)->bool
	{
		return this->load(node);
	});
}

core::async::iasync<bool> effect_library::save_async(dom::xml::ixml_document_t doc)const
{
	return create_task<bool>([=](core::async::iasync<bool>)->bool
	{
		return this->save(doc);
	});
}

bool effect_library::load_sources(dom::xml::ixml_node_t node)
{
	return m_parent.lock()->load_sources(node);
}

optional<ilibrary> effect_library::load_library(dom::xml::ixml_node_t node)
{
	effect_library_t loader = new effect_library(this);
	if (!loader->load(node))
		return error(error_code::unknown);
	return loader.get();
}

optional<iresource> effect_library::load_resource(dom::xml::ixml_node_t node)
{
	if (node.is_empty())
		return error(error_code::invalid_param);

	auto name = node->name()->as<cstr_t>();
	if (name == "effect")
	{
		auto fx = load_effect(node);
		if (fx.successed())
			return fx->resource();
	}
	else if (name == "technique")
	{
		auto sh = load_technique(node);
		if (sh.successed())
			return sh->resource();
	}
	return error(error_code::invalid_param);
}

core::async::iasync_op<ilibrary> effect_library::load_library_async(dom::xml::ixml_node_t node)
{
	return create_task<optional<ilibrary>>([=](core::async::iasync_op<ilibrary>)->optional<ilibrary>
	{
		return load_library(node);
	});
}

core::async::iasync_op<iresource> effect_library::load_resource_async(dom::xml::ixml_node_t node)
{
	return create_task<optional<iresource>>([=](core::async::iasync_op<iresource>)->optional<iresource>
	{
		return load_resource(node);
	});
}

string effect_library::find_source(cstr_t sid)const 
{
	return parent()->find_source(sid);
}

iresource_t effect_library::find_resource(cstr_t sid)const 
{
	auto fx = find_effect(sid);
	if (!fx.is_empty())
		return fx->resource();

	auto sh = find_technique(sid);
	if (!sh.is_empty())
		return sh->resource();
	return null;
//	return m_parent.lock()->find_resource(sid);
}

core::async::iasync<iresource_t> effect_library::find_resource_async(cstr_t sid_)
{
	effect_library_t auto_save = this;
	string sid = sid_;
	return create_task<iresource_t>([=](core::async::iasync<iresource_t>)->iresource_t
	{
		auto res = find_resource(sid);
		if (!res.is_empty())
			return res;
		return null;
	});
}

optional<ieffect> effect_library::load_effect(dom::xml::ixml_node_t node) 
{
	effect_t fx = new effect();
	if(!fx->load(this, node))
		return error(error_code::invalid_param);
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	m_effect_map[fx->resource_sid()] = fx.get();
	return fx.get();
}

core::async::iasync_op<ieffect> effect_library::load_effect_async(dom::xml::ixml_node_t node)
{
	return create_task<optional<ieffect>>([=](core::async::iasync_op<ieffect>)->optional<ieffect>
	{
		return load_effect(node);
	});
}

optional<ishaders> effect_library::load_technique(dom::xml::ixml_node_t data) 
{
	using namespace ang::dom;
	
	if (data.is_empty())
		return error(error_code::invalid_param);

	string sid = data->attributes()["sid"]->as<cstr_t>();
	if (auto shader = find_technique(sid))
		return shader;
	
	if (data->has_value())
	{
		sid = data->value()->as<cstr_t>();
		if (auto shader = find_technique(sid))
			return shader;
	}
	else if (data->children())
	{
		string log;
		shader_info vertex;
		shader_info pixel;
		bool res = true;

		for (xml::ixml_node_t node : data->children())
		{
			auto name = node->name()->as<cstr_t>();
			if (name == "vertex_shader")
				res &= load_shader_info(node, vertex);
			else if (name == "pixel_shader")
				res &= load_shader_info(node, pixel);
		}
		if (!res)
			return error(error_code::invalid_param);

		auto shaders = factory()->compile_shaders_async(vertex, pixel, sid)->result();
		if (shaders.successed())
		{
			core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
			m_technique_map[sid] = shaders.get();
		}
		else
		{
			//logs:
		}
		return shaders;
	}
	return error(error_code::invalid_param);
}

bool effect_library::load_shader_info(dom::xml::ixml_node_t data, shader_info_t& info)
{
	using namespace ang::dom;
	if (data->has_children())
	{
		for (xml::ixml_node_t node : data->children())
		{
			auto name = node->name()->as<cstr_t>();
			if (name == "entry")
				info.entry = node->value()->as<cstr_t>();
			else if (name == "file")
				info.file = node->value()->as<cstr_t>();
			else if (name == "code") {
				if (node->has_children())
				{
					try {
						info.code = node->children()["CDATA"]->value()->as<cstr_t>();
					}
					catch (...) {}
				}
				else
				{
					info.code = node->value()->as<cstr_t>();
				}
			}
			else if (name == "compile_config")
				info.compile_config = node->value()->as<cstr_t>();
		}
		return true;
	}
	else if (data->has_value())
	{
		int i = 0;
		for (string& str : data->value()->split(";"_r))
		{
			switch (i)
			{
			case 0: info.file = str->cstr(); break;
			case 1: info.entry = str->cstr(); break;
			case 2: info.compile_config = str->cstr(); break;
			}
			i++;
		}
		return true;
	}
	return false;
}

core::async::iasync_op<ishaders> effect_library::load_technique_async(dom::xml::ixml_node_t node) 
{
	return create_task<optional<ishaders>>([=](core::async::iasync_op<ishaders>)->optional<ishaders>
	{
		return load_technique(node);
	});
}

ieffect_t effect_library::find_effect(cstr_t sid)const
{
	auto lock = m_mutex.scope();
	auto it = m_effect_map.find(sid);
	if (it.is_valid()) {
		auto fx = it->value;
		if (!fx.is_empty())
			return fx;
	}
	return null;
}

ishaders_t effect_library::find_technique(cstr_t sid)const
{
	auto lock = m_mutex.scope();
	auto it = m_technique_map.find(sid);
	if (it.is_valid()) {
		auto shader = it->value;
		if (!shader.is_empty())
			return shader;
	}
	return null;
}


/////////////////////////