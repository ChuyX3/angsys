#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::meshes;

/////////////////////////////////////////////////////////////////////////////////////////////

mesh_loader::mesh_loader(ilibrary* parent)
	: m_parent(parent)
	, m_mesh_map()
{
	throw_exception_if(parent == null, error_code::invalid_param);
}

mesh_loader::~mesh_loader()
{
}

void mesh_loader::dispose()
{
	clear();
	//m_async_worker = null;
	m_parent = null;
	m_mesh_map = null;
}

void mesh_loader::clear()
{
	m_mutex.lock();
	m_mesh_map.clear();
	m_mutex.unlock();
}

ifactory_t mesh_loader::factory()const
{
	return m_parent.lock()->factory();
}

core::files::ifile_system_t mesh_loader::file_system()const
{
	return parent()->file_system();
}

core::async::idispatcher_t mesh_loader::dispatcher()const
{
	return parent()->dispatcher();
}

bool mesh_loader::load(dom::xml::ixml_node_t data)
{
	if (data.is_empty() || !data->has_children())
		return false;

	for (dom::xml::ixml_node_t node : data->children())
	{
		auto name = node->name()->as<cstr_t>();
		if (name == "sources")
			load_sources(node);
		else if (name == "mesh")
			load_mesh(node);
	}
	return true;
}

bool mesh_loader::save(dom::xml::ixml_document_t doc)const
{
	auto builder = doc->builder();
	builder->begin_element("mesh_loader");
	{
		//TODO:
	}builder->end_element();

	return true;
}

core::async::iasync<bool> mesh_loader::load_async(dom::xml::ixml_node_t node)
{
	return create_task<bool>([=](core::async::iasync<bool>)->bool
	{
		return this->load(node);
	});
}

core::async::iasync<bool> mesh_loader::save_async(dom::xml::ixml_document_t doc)const
{
	return create_task<bool>([=](core::async::iasync<bool>)->bool
	{
		return this->save(doc);
	});
}

bool mesh_loader::load_sources(dom::xml::ixml_node_t node)
{
	return m_parent.lock()->load_sources(node);
}

optional<ilibrary> mesh_loader::load_library(dom::xml::ixml_node_t node)
{
	mesh_loader_t loader = new mesh_loader(this);
	if (!loader->load(node))
		return error(error_code::unknown);
	return loader.get();
}

optional<iresource> mesh_loader::load_resource(dom::xml::ixml_node_t node)
{
	if (node.is_empty())
		return error(error_code::invalid_param);

	auto name = node->name()->as<cstr_t>();
	if (name == "mesh")
	{
		auto mesh = load_mesh(node);
		if (mesh.successed())
			return mesh->resource();
	}
	return error(error_code::invalid_param);
}

core::async::iasync_op<ilibrary> mesh_loader::load_library_async(dom::xml::ixml_node_t node)
{
	return create_task<optional<ilibrary>>([=](core::async::iasync_op<ilibrary>)->optional<ilibrary>
	{
		return load_library(node);
	});
}

core::async::iasync_op<iresource> mesh_loader::load_resource_async(dom::xml::ixml_node_t node)
{
	return create_task<optional<iresource>>([=](core::async::iasync_op<iresource>)->optional<iresource>
	{
		return load_resource(node);
	});
}

string mesh_loader::find_source(cstr_t sid)const
{
	return parent()->find_source(sid);
}

iresource_t mesh_loader::find_resource(cstr_t sid)const
{
	auto ms = find_mesh(sid);
	if (!ms.is_empty())
		return ms->resource();
	return null;
}

core::async::iasync<iresource_t> mesh_loader::find_resource_async(cstr_t sid_)
{
	mesh_loader_t auto_save = this;
	string sid = sid_;
	return create_task<iresource_t>([=](core::async::iasync<iresource_t>)->iresource_t
	{
		auto res = find_resource(sid);
		if (!res.is_empty())
			return res;
		return null;
	});
}

optional<imesh> mesh_loader::load_mesh(dom::xml::ixml_node_t node)
{
	return load_mesh_async(node)->result();
}

core::async::iasync_op<imesh> mesh_loader::load_mesh_async(dom::xml::ixml_node_t node)
{
	mesh_loader_t auto_save = this;
	return create_task<optional<imesh>>([=](core::async::iasync_op<imesh>)->optional<imesh>
	{
		mesh_t ms = new mesh();
		if (!ms->load(auto_save->parent(), node))
			return error(error_code::invalid_param);
		auto lock = m_mutex.scope();
		m_mesh_map[ms->resource_sid()] = ms;
		return ms.get();
	});
}

imesh_t mesh_loader::find_mesh(cstr_t sid)const
{
	auto lock = m_mutex.scope();
	auto it = m_mesh_map.find(sid);
	if (it.is_valid()) {
		auto ms = it->value;
		if (!ms.is_empty())
			return ms;
	}
	return null;
}

/////////////////////////