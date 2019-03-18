#include "pch.h"
#include "resources/resources.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::effects;
using namespace ang::graphics::resources;


effect::effect(ilibrary* parent)
	: m_parent(parent)
{

}

effect::~effect()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::effects::effect);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::effects::effect, object, ieffect, iresource, dom::xml::ixml_serializable);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::effects::effect, object, ieffect, iresource, dom::xml::ixml_serializable);

bool effect::load(dom::xml::xml_node_t node)
{
	return false;
}

bool effect::save(dom::xml::xml_document_t doc)const
{
	//TODO:
	return false;
}

core::async::iasync<bool> effect::load_async(dom::xml::xml_node_t)
{
	throw_exception(except_code::unsupported);
	return null; 
}

core::async::iasync<bool> effect::save_async(dom::xml::xml_document_t)const
{
	throw_exception(except_code::unsupported);
	return null;
}

iresource_t effect::resource()const
{
	return const_cast<effect*>(this);
}

wsize effect::pass_count()const
{
	return m_passes->counter();
}

ipass_t effect::pass(windex i)const
{
	return m_passes[i].get();
}

iframe_buffer_t effect::frame_buffer(cstr_t sid)const
{
	try { return m_fbo_map[sid]; }
	catch (...) { return null; }
}

resource_type_t effect::resource_type()const
{
	return resource_type::effect;
}

string effect::resource_sid()const
{
	return m_resource_sid;
}
	
effects::ieffect_t effect::to_effect()
{
	return this;
}

effects::ishaders_t effect::to_shaders()
{
	return null;
}

textures::itexture_t effect::to_texture()
{
	return null;
}

iframe_buffer_t effect::to_frame_buffer()
{
	return null;
}

buffers::iindex_buffer_t effect::to_index_buffer()
{
	return null;
}

buffers::ivertex_buffer_t effect::to_vertex_buffer()
{
	return null;
}


////////////////////////////////////////////////////////////////////////////////////////////////

effect_library::effect_library(ilibrary* parent)
	: m_parent(parent)
{
	if (parent == null)
		throw_exception(except_code::invalid_param);
	m_async_worker = parent->dispatcher();
}

effect_library::~effect_library()
{
	m_parent = null;
	m_async_worker = null;
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::graphics::effects::effect_library);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::graphics::effects::effect_library, object, ieffect_library);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::graphics::effects::effect_library, object, ieffect_library, ilibrary, dom::xml::ixml_serializable);

void effect_library::clear()
{
	m_effect_map = null;
	m_technique_map = null;
}

ifactory_t effect_library::factory()const  
{
	return parent()->factory();
}

core::async::idispatcher_t effect_library::dispatcher()const
{
	return m_async_worker.get();
}

bool effect_library::load(dom::xml::xml_node_t node)
{
	if (!node->xml_has_children() || 
		(node->xml_name()->xml_as<cstr_t>() != "effect_library" &&
			node->xml_name()->xml_as<cstr_t>() != "effects"))
		return false;

	for (dom::xml::xml_node_t node : node->xml_children())
	{
		cstr_t name = node->xml_name();
		if (name == "sources")
			load_sources(node);
		else if (name == "technique")
			load_technique(node);
		else if (name == "effect")
			load_effect(node);
	}
	return true;
}

bool effect_library::save(dom::xml::xml_document_t doc)const 
{
	//TODO: needed for engine editor
	return false;
}

core::async::iasync<bool> effect_library::load_async(dom::xml::xml_node_t node) 
{
	effect_library_t auto_save = this;
	return dispatcher()->run_async<bool>([=](core::async::iasync<bool> task)->bool
	{
		return auto_save.get()->load(node);
	});
}

core::async::iasync<bool> effect_library::save_async(dom::xml::xml_document_t doc)const
{
	effect_library_t auto_save = const_cast<effect_library*>(this);
	return dispatcher()->run_async<bool>([=](core::async::iasync<bool> task)->bool
	{
		return auto_save.get()->save(doc);
	});
}

bool effect_library::load_sources(dom::xml::xml_node_t node)  
{
	return parent()->load_sources(node);
}

ilibrary_t effect_library::load_library(dom::xml::xml_node_t node) 
{
	cstr_t name = node->xml_name();
	if (name == "effect_library" || name == "effects") {
		effect_library_t library = new effect_library(this);
		if (!library->load(node))
			return null;
		return library.get();
	}
}

iresource_t effect_library::load_resource(dom::xml::xml_node_t node)
{
	cstr_t name = node->xml_name();
	resource_t res = new resource(this);
	if (!res->load(node))
		return null;
	switch (res->resource_type())
	{
	case resource_type::technique:
		m_mutex.lock();
		m_technique_map[res->resource_sid()] = res->to_shaders();
		m_mutex.unlock();
		return res->handle();
	case resource_type::effect:
		m_mutex.lock();
		m_effect_map[res->resource_sid()] = static_cast<effect*>(res->to_effect().get());
		m_mutex.unlock();
		return res.get();
	case resource_type::frame_buffer:
		return res.get(); //needed for effects, no storage object
	}
	return null;//unknown resource (can be a frame buffer)
}

core::async::iasync<ilibrary_t> effect_library::load_library_async(dom::xml::xml_node_t node) 
{
	effect_library_t auto_save = const_cast<effect_library*>(this);
	return dispatcher()->run_async<ilibrary_t>([=](core::async::iasync<ilibrary_t> task)->ilibrary_t
	{
		return auto_save.get()->load_library(node);
	});
}

core::async::iasync<iresource_t> effect_library::load_resource_async(dom::xml::xml_node_t node)
{
	effect_library_t auto_save = const_cast<effect_library*>(this);
	return dispatcher()->run_async<iresource_t>([=](core::async::iasync<iresource_t> task)->iresource_t
	{
		return auto_save.get()->load_resource(node);
	});
}

string effect_library::find_source(cstr_t sid)const
{
	return parent()->find_source(sid);
}

iresource_t effect_library::find_resource(cstr_t sid)const
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;

	auto it1 = m_technique_map->find(sid);
	if (it1.is_valid())
		return it1->value->resource();

	auto it2 = m_effect_map->find(sid);
	if (it2.is_valid())
		return it2->value->resource();

	return parent()->find_resource(sid);
}

ieffect_t effect_library::load_effect(dom::xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return null;
	effect_t effect = new effects::effect(this);
	if (!effect->load(node))
		return null;
	m_mutex.lock();
	m_effect_map[effect->resource_sid()] = effect.get();
	m_mutex.unlock();
	return effect.get();
}

core::async::iasync<ieffect_t> effect_library::load_effect_async(dom::xml::xml_node_t  node) 
{
	effect_library_t auto_save = const_cast<effect_library*>(this);
	return dispatcher()->run_async<ieffect_t>([=](core::async::iasync<ieffect_t> task)->ieffect_t
	{
		return auto_save.get()->load_effect(node);
	});
}

itechnique_t effect_library::load_technique(dom::xml::xml_node_t node)
{
	if (!node->xml_has_children())
		return null;
	resource_t res = new resource(this);
	if (!res->load(node) || res->resource_type() != resource_type::technique)
		return null;
	m_mutex.lock();
	m_technique_map[res->resource_sid()] = res->to_shaders();
	m_mutex.unlock();
	return  res->to_shaders();
}

core::async::iasync<itechnique_t> effect_library::load_technique_async(dom::xml::xml_node_t node) 
{
	effect_library_t auto_save = const_cast<effect_library*>(this);
	return dispatcher()->run_async<itechnique_t>([=](core::async::iasync<itechnique_t> task)->itechnique_t
	{
		return auto_save.get()->load_technique(node);
	});
}

ieffect_t effect_library::find_effect(cstr_t sid)const  
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	auto it2 = m_effect_map->find(sid);
	if (it2.is_valid())
		return it2->value;
	auto res = parent()->find_resource(sid);
	if (res.is_empty())
		return null;
	return res->to_effect();
}

itechnique_t effect_library::find_technique(cstr_t sid)const 
{
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	auto it1 = m_technique_map->find(sid);
	if (it1.is_valid())
		return it1->value;
	auto res = parent()->find_resource(sid);
	if (res.is_empty())
		return null;
	return res->to_shaders();
}
