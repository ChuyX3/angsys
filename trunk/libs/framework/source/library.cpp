#include "pch.h"
#include <ang/framework/framework.h>


using namespace ang;
using namespace ang::dom;
using namespace ang::graphics;
using namespace ang::graphics::resources;

library::library(ilibrary* parent)
	: m_parent(parent)
	, m_fx_library(new effects::effect_library(this))
	, m_tex_loader(new textures::texture_loader(this))
	, m_mesh_loader(new meshes::mesh_loader(this))
{
	//throw_exception_if(parent == null, error_code::invalid_param);
	m_async_worker = core::async::thread::create_dispatcher_thread();
}

library::~library()
{

}

bool library::query_interface(rtti_t const& id, unknown_ptr_t out)
{
	if (id.type_id() == type_of<library>().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<library*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == type_of<ilibrary>().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<ilibrary*>(this);
		return true;
	}
	else if (id.type_id() == type_of<xml::ixml_streamer>().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<xml::ixml_streamer*>(this);
		return true;
	}
	else if (id.type_id() == type_of<textures::itexture_loader>().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<textures::itexture_loader*>(m_tex_loader.get());
		return true;
	}
	else if (id.type_id() == type_of<effects::ieffect_library>().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<effects::ieffect_library*>(m_fx_library.get());
		return true;
	}
	else if (id.type_id() == type_of<meshes::imesh_loader>().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<meshes::imesh_loader*>(m_mesh_loader.get());
		return true;
	}
	return false;
}


void library::dispose()
{
	m_async_worker->exit();
	m_async_worker = null;

	clear();

	m_fx_library = null;
	m_tex_loader = null;
	m_parent = null;

}

void library::clear()
{
	m_fx_library->clear();
	m_tex_loader->clear();
}

ifactory_t library::factory()const
{
	return m_parent.lock()->factory();
}
core::files::ifile_system_t library::file_system()const
{
	return parent()->file_system();
}

core::async::idispatcher_t library::dispatcher()const
{
	return m_async_worker.get();
}

bool library::load(xml::ixml_node_t data)
{
	if (data.is_empty())
		return false;

	auto name = data->name()->as<cstr_t>();

	if (name == "effect_library")
	{
		m_fx_library->load(data);
	}
	else if (name == "texture_library")
	{
		m_tex_loader->load(data);
	}
	else if (name == "mesh_library")
	{
		m_mesh_loader->load(data);
	}
	else if (name == "library")
	{
		for (xml::ixml_node_t node : data->children())
		{
			name = node->name()->as<cstr_t>();
			if (name == "sources")
			{
				load_sources(node);
			}
			else if (name == "effect_library")
			{
				m_fx_library->load(node);
			}
			else if (name == "texture_library")
			{
				m_tex_loader->load(node);
			}
			else if (name == "mesh_library")
			{
				m_mesh_loader->load(node);
			}
		}
	}
	return true;
}

bool library::save(xml::ixml_document_t doc)const
{
	//TODO:
	return false;
}

core::async::iasync<bool> library::load_async(xml::ixml_node_t data)
{
	library_t auto_save = this;
	return create_task<bool>([=](core::async::iasync<bool>)->bool
	{
		return auto_save->load(data);
	});
}

core::async::iasync<bool> library::save_async(xml::ixml_document_t doc)const
{
	return null;
}

bool library::load_sources(xml::ixml_node_t data)
{
	return m_parent.lock()->load_resource(data);
}

optional<ilibrary> library::load_library(xml::ixml_node_t data)
{
	auto name = data->name()->as<cstr_t>();
	if (name == "library")
	{
		ilibrary_t lib = new library(this);
		if (lib->load(data))
			return lib;
	}
	else if (name == "effect_library")
	{
		ilibrary_t lib = new effects::effect_library(this);
		if (lib->load(data))
			return lib;
	}
	else if (name == "texture_library")
	{
		ilibrary_t lib = new textures::texture_loader(this);
		if (lib->load(data))
			return lib;
	}
	else if (name == "mesh_library")
	{
		ilibrary_t lib = new meshes::mesh_loader(this);
		if (lib->load(data))
			return lib;
	}
	return error(error_code::invalid_param);
}

optional<iresource> library::load_resource(xml::ixml_node_t data)
{
	if (data.is_empty())
		return error(error_code::invalid_param);

	auto name = data->name()->as<cstr_t>();

	if (name == "shader"
		|| name == "technique"
		|| name == "effect")
	{
		core::async::scope_locker<core::async::mutex> locker = m_mutex;
		return m_fx_library->load_resource(data);
	}
	else if (name == "texture")
	{
		core::async::scope_locker<core::async::mutex> locker = m_mutex;
		return m_tex_loader->load_resource(data);
	}
	else if (name == "mesh")
	{
		core::async::scope_locker<core::async::mutex> locker = m_mutex;
		return m_mesh_loader->load_resource(data);
	}
	return error(error_code::invalid_param);
}

core::async::iasync_op<ilibrary> library::load_library_async(xml::ixml_node_t data)
{
	library_t auto_save = this;

	return create_task<optional<ilibrary>>([=](core::async::iasync_op<ilibrary>)->optional<ilibrary>
	{
		return auto_save->load_library(data);
	});
}

core::async::iasync_op<iresource> library::load_resource_async(xml::ixml_node_t data)
{
	library_t auto_save = this;
	return create_task<optional<iresource>>([=](core::async::iasync_op<iresource>)->optional<iresource>
	{
		return auto_save->load_resource(data);
	});
}

string library::find_source(cstr_t sid)const
{
	return m_parent.lock()->find_source(sid);
}

iresource_t library::find_resource(cstr_t sid)const
{
	core::async::scope_locker<core::async::mutex> locker = m_mutex;
	auto res = m_tex_loader->find_resource(sid);
	if (!res.is_empty())
		return res;
	res = m_mesh_loader->find_resource(sid);
	if (!res.is_empty())
		return res;
	res = m_fx_library->find_resource(sid);
	if (!res.is_empty())
		return res;
	if (auto p = m_parent.lock())
		return p->find_resource(sid);
	return null;
}

core::async::iasync<iresource_t> library::find_resource_async(cstr_t sid_)
{
	library_t auto_save = this;
	string sid = sid_;
	return create_task<iresource_t>([=](core::async::iasync<iresource_t>)->iresource_t
	{
		core::async::scope_locker<core::async::mutex> locker = m_mutex;
		auto res = m_fx_library->find_resource(sid);
		if (!res.is_empty())
			return res;
		res = m_mesh_loader->find_resource(sid);
		if (!res.is_empty())
			return res;
		res = m_tex_loader->find_resource(sid);
		if (!res.is_empty())
			return res;
		auto tex = m_tex_loader->load_texture(sid);
		if (tex.successed())
			return tex->resource();
		if (auto p = m_parent.lock())
			return p->find_resource(sid);
		return null;
	});
}
