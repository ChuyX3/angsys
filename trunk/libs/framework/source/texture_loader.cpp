#include "pch.h"
#include <ang/framework/framework.h>


using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;
using namespace ang::graphics::textures;

texture_loader::texture_loader(ilibrary* parent)
	: m_parent(parent)
	, m_tex_info_map()
	, m_texture_map()
	, m_async_worker(core::async::thread::create_dispatcher_thread())
{
	throw_exception_if(parent == null, error_code::invalid_param);
}

texture_loader::~texture_loader()
{
}

void texture_loader::dispose()
{
	clear();
	m_async_worker->exit();
	m_async_worker = null;
	m_parent = null;
	m_texture_map = null;
}

void texture_loader::clear()
{
	m_mutex.lock();
	m_texture_map.clear();
	m_mutex.unlock();
}

ifactory_t texture_loader::factory()const
{
	return m_parent.lock()->factory();
}

core::files::ifile_system_t texture_loader::file_system()const
{
	return parent()->file_system();
}

core::async::idispatcher_t texture_loader::dispatcher()const
{
	return m_async_worker;
}

bool texture_loader::load(dom::xml::ixml_node_t node)
{
	if (node.is_empty() || !node->has_children())
		return false;

	for (dom::xml::ixml_node_t node : node->children())
	{
		auto name = node->name()->as<cstr_t>();
		if (name == "sources")
		{
			load_sources(node);
		}
		else if (name == "texture")
		{
			auto att = node->attributes();
			tex_info_t info;
			string sid = att["sid"]->as<cstr_t>();
			info.sid = sid;
			info.type = att["type"]->as<tex_type_t>();

			if (node->has_children())
			{
				for (dom::xml::ixml_node_t node : node->children())
				{
					if (node->name()->as<cstr_t>() != "texture")
						continue;
					info.files += node->value()->as<cstr_t>();
				}
			}
			else
			{
				info.files += node->value()->as<cstr_t>();
			}

			m_mutex.lock();
			m_tex_info_map[sid] = ang::move(info);
			m_mutex.unlock();
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

bool texture_loader::save(dom::xml::ixml_document_t doc)const
{
	auto builder = doc->builder();
	builder->begin_element("texture_apex");
	{
		//TODO:
	}builder->end_element();

	return true;
}

core::async::iasync<bool> texture_loader::load_async(dom::xml::ixml_node_t node)
{
	return create_task<bool>([=](core::async::iasync<bool>)
	{
		return this->load(node);
	});
}

core::async::iasync<bool> texture_loader::save_async(dom::xml::ixml_document_t doc)const
{
	return create_task<bool>([=](core::async::iasync<bool>)
	{
		return this->save(doc);
	});
}

bool texture_loader::load_sources(dom::xml::ixml_node_t node)
{
	return m_parent.lock()->load_sources(node);
}

optional<ilibrary> texture_loader::load_library(dom::xml::ixml_node_t node)
{
	texture_loader_t loader = new texture_loader(this);
	if (!loader->load(node))
		return error(error_code::unknown);
	return loader.get();
}

optional<iresource> texture_loader::load_resource(dom::xml::ixml_node_t node)
{
	auto tex = load_texture(node);
	if (tex.failed())
		return tex.error();
	return tex->resource();
}

core::async::iasync_op<ilibrary> texture_loader::load_library_async(dom::xml::ixml_node_t node)
{
	return create_task<optional<ilibrary>>([=](core::async::iasync_op<ilibrary>)
	{
		return load_library(node);
	});
}

core::async::iasync_op<iresource> texture_loader::load_resource_async(dom::xml::ixml_node_t node)
{
	return create_task<optional<iresource>>([=](core::async::iasync_op<iresource>)
	{
		return load_resource(node);
	});
}

string texture_loader::find_source(cstr_t sid)const
{
	return m_parent.lock()->find_source(sid);
}

iresource_t texture_loader::find_resource(cstr_t sid)const
{
	m_mutex.lock();
	auto it = m_texture_map.find(sid);
	if (it.is_valid()) {
		auto res = it->value.lock();
		m_mutex.unlock();
		return res.get();
	}
	m_mutex.unlock();
	return null;// m_parent.lock()->find_resource(sid);
}


core::async::iasync<iresource_t> texture_loader::find_resource_async(cstr_t sid_)
{
	texture_loader_t auto_save = this;
	string sid = sid_;
	return create_task<iresource_t>([=](core::async::iasync<iresource_t>)->iresource_t
	{
		auto res = find_resource(sid);
		if (!res.is_empty())
			return res;
		auto tex = load_texture(sid);
		if (tex.successed())
			return tex->resource();
		return null;
	});
}


optional<itexture> texture_loader::load_texture(cstr_t sid)
{
	auto itex = find_texture(sid);
	if (!itex.is_empty())
		return itex;
	tex_info_t info;
	{//scope
		core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
		if (m_tex_info_map.is_empty())
			return error(error_code::invalid_param);
		auto it = m_tex_info_map.find(sid);
		if (!it.is_valid())
			return error(error_code::invalid_param);
		info = it->value;
	}

	itexture_t tex;
	if (info.files.size() > 1)
	{
		auto result = factory()->load_texture_async(info.files, info.type, info.sid)->result();
		if (result.failed())
			return result.error();
		tex = result.get();
	}
	else
	{
		auto result = factory()->load_texture_async(info.files[0], info.type, info.sid)->result();
		if (result.failed())
			return result.error();
		tex = result.get();
	}

	auto lock = m_mutex.scope();
	m_texture_map[sid] = tex->resource();
	return tex;
}

bool texture_loader::load_texture_info(dom::xml::ixml_node_t node)
{
	using namespace ang::dom;
	itexture_t tex;

	if (node.is_empty())
		return false;

	tex_info_t info;
	info.sid = node->attributes()["sid"]->as<cstr_t>();
	if (node->has_children())
	{
		bool res = true;
		auto att = node->attributes();

		info.type = att["type"]->as<textures::tex_type_t>();

		for (xml::ixml_node_t tex_node : node->children())
		{
			cstr_t name = tex_node->name();
			if (name != "texture")
				continue;
			info.files += tex_node->value()->as<cstr_t>();
		}
	}
	else
	{
		bool res = true;
		auto att = node->attributes();

		info.type = att["type"]->as<textures::tex_type_t>();
		info.files += node->value()->as<cstr_t>();
	}

	auto lock = m_mutex.scope();
	m_tex_info_map[info.sid] = info;

	return true;
}

optional<itexture> texture_loader::load_texture(dom::xml::ixml_node_t node)
{
	using namespace ang::dom;
	itexture_t tex;

	if (node.is_empty())
		return error(error_code::invalid_param);

	tex_info_t info;
	info.sid = node->attributes()["sid"]->as<cstr_t>();
	if (node->has_children())
	{
		bool res = true;
		auto att = node->attributes();

		info.type = att["type"]->as<textures::tex_type_t>();

		for (xml::ixml_node_t tex_node : node->children())
		{
			cstr_t name = tex_node->name();
			if (name != "texture")
				continue;
			info.files += tex_node->value()->as<cstr_t>();
		}

		auto result = factory()->load_texture_async(info.files, info.type, info.sid)->result();
		if (result.failed())
			return result.error();
		tex = result.get();
	}
	else
	{
		bool res = true;
		auto att = node->attributes();

		info.type = att["type"]->as<textures::tex_type_t>();
		info.files += node->value()->as<cstr_t>();

		auto result = factory()->load_texture_async(info.files[0], info.type, info.sid)->result();
		if (result.failed())
			return result.error();
		tex = result.get();
	}

	auto lock = m_mutex.scope();
	m_tex_info_map[info.sid] = info;
	m_texture_map[info.sid] = tex->resource();

	return tex;
}

core::async::iasync_op<itexture> texture_loader::load_texture_async(cstr_t sid)
{
	return create_task<optional<itexture>>([=](core::async::iasync_op<itexture>)
	{
		return load_texture(sid);
	});
}

core::async::iasync_op<itexture> texture_loader::load_texture_async(dom::xml::ixml_node_t node)
{
	return create_task<optional<itexture>>([=](core::async::iasync_op<itexture>)
	{
		return load_texture(node);
	});
}

core::async::iasync<bool> texture_loader::load_texture_info_async(dom::xml::ixml_node_t node)
{
	return create_task<bool>([=](core::async::iasync<bool>)
	{
		return load_texture_info(node);
	});
}

itexture_t texture_loader::find_texture(cstr_t sid)const
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;

	auto it = m_texture_map.find(sid);
	if (it.is_valid()) {
		auto res = it->value.lock();
		if (!res.is_empty())
			return res->cast<resources::resource_type::texture>();
	}
	return null;
}
