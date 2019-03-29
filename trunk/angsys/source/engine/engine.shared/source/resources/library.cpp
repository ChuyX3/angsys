#include "pch.h"
#include "resources/resources.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;


library::library(ilibrary* parent)
	: m_parent(parent)
	, m_fx_library(new effects::effect_library(this))
	, m_tex_loader(new textures::texture_loader(this))
{
	m_async_worker = parent ? parent->dispatcher() : null;
}

library::~library()
{
	m_fx_library = null;
	m_tex_loader = null;
	m_parent = null;
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::resources::library);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::resources::library, object, ilibrary);
//ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::resources::library, object, ilibrary);

bool library::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == library::class_info().type_id())
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
	else if (id.type_id() == ilibrary::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<ilibrary*>(this);
		return true;
	}
	else if (id.type_id() == effects::ieffect_library::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<effects::ieffect_library*>(m_fx_library.get());
		return true;
	}
	else if (id.type_id() == textures::itexture_loader::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<textures::itexture_loader*>(m_tex_loader.get());
		return true;
	}
	return false;
}


void library::clear()
{
	m_fx_library->clear();
	m_tex_loader->clear();
}

ifactory_t library::factory()const
{
	return parent()->factory();
}

core::async::idispatcher_t library::dispatcher()const
{
	return m_async_worker.get();
}

bool library::load(dom::xml::xml_node_t node)
{
	cstr_t name = node->xml_name();
	if (name == "library")
	{
		for (dom::xml::xml_node_t node : node->xml_children())
		{
			cstr_t type = node->xml_name();
			if (type == "sources")
				load_sources(node);
			else if (type == "effect_library" || type == "effects")
				m_fx_library->load(node);
			else if (type == "texture_library" || type == "textures")
				m_tex_loader->load(node);
		}
		return true;
	}
	else if (name == "effect_library" || name == "effects")
	{
		return m_fx_library->load(node);
	}
	else if (name == "texture_library" || name == "textures")
	{
		return m_tex_loader->load(node);
	}
	return false;
}

bool library::save(dom::xml::xml_document_t doc)const
{
	//TODO: needed for engine editor
	return false;
}

bool library::load_sources(dom::xml::xml_node_t sources)
{
	/*call parent recursively to find resource manager*/
	return parent()->load_sources(sources);
}

core::async::iasync<bool> library::load_async(dom::xml::xml_node_t node)
{
	library_t auto_save = this;
	return m_async_worker->run_async<bool>(
		[=](core::async::iasync<bool> task)
	{
		return auto_save->load(node);
	});
}

core::async::iasync<bool> library::save_async(dom::xml::xml_document_t doc)const
{

}

ilibrary_t library::load_library(dom::xml::xml_node_t  node)
{
	cstr_t name = node->xml_name();
	if (name == "library")
	{
		/*creating child library*/
		library_t lib = new library(this);
		if (!lib->load(node))
			return null;
		return lib.get();
	}
	else if (name == "effect_library" || name == "effects")
	{
		return m_fx_library->load_library(node);
	}
	else if (name == "texture_library" || name == "textures")
	{
		return m_tex_loader->load_library(node);
	}
	return null;
}

iresource_t library::load_resource(dom::xml::xml_node_t  node)
{
	cstr_t name = node->xml_name();
	iresource_t res;
	if (name == "technique")
	{
		res = m_fx_library->load_resource(node);
	}
	else if (name == "effect")
	{
		res = m_fx_library->load_resource(node);
	}
	else if (name == "texture")
	{
		res = m_tex_loader->load_resource(node);
	}
	if(res.is_empty())
		return null;
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	m_resource_map[res->resource_sid()] = res;

	return res;
}

core::async::iasync<ilibrary_t> library::load_library_async(dom::xml::xml_node_t  node)
{
	library_t auto_save = this;
	return dispatcher()->run_async<ilibrary_t>(
		[=](core::async::iasync<ilibrary_t> task)->ilibrary_t
	{
		return auto_save->load_library(node);
	});
}

core::async::iasync<iresource_t> library::load_resource_async(dom::xml::xml_node_t  node)
{
	library_t auto_save = this;
	return dispatcher()->run_async<iresource_t>(
		[=](core::async::iasync<iresource_t> task)->iresource_t
	{
		return auto_save->load_resource(node);
	});
}

string library::find_source(cstr_t sid)const
{
	return parent()->find_source(sid);
}

iresource_t library::find_resource(cstr_t sid)const
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	try {
		return m_resource_map[sid].lock().get();
	}
	catch (...)
	{
		return parent()->find_resource(sid);
	}
}

