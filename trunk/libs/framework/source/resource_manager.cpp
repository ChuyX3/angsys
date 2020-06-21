#include "pch.h"
#include <ang/framework/framework.h>

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;

resource_manager::resource_manager()
	: base(null)
	, m_factory(null)
	, m_file_map()
{
	m_file_system = core::files::ifile_system::instance();
}

resource_manager::~resource_manager()
{

}

void resource_manager::dispose()
{
	library::dispose();
	m_factory = null;
	m_file_system = null;
}

void resource_manager::clear()
{
	library::clear();
}

vector<string> resource_manager::paths(core::files::path_access_type_t type)const
{
	return m_file_system->paths(type);
}

void resource_manager::push_path(cstr_t path, core::files::path_access_type_t type, cstr_t macro)
{
	m_file_system->push_path(path, type, macro);
}

vector<string> resource_manager::find_paths(cstr_t macro)const
{
	return m_file_system->find_paths(macro);
}

core::files::path_access_type_t resource_manager::path_access_type(cstr_t path)const
{
	return m_file_system->path_access_type(path);
}

bool resource_manager::create_handle(cstr_t sid, core::files::open_flags_t flags, core::files::ifile_ptr_t file, cstr_t macro)
{
	auto path = find_source(sid);
	if (path != null)
		return m_file_system->create_handle(path, flags, file, macro);
	else
		return m_file_system->create_handle(sid, flags, file, macro);
}

core::async::iasync_op<core::files::ifile> resource_manager::create_handle_async(cstr_t path_, core::files::open_flags_t flags, cstr_t macro_) {
	string path = path_;
	string macro = macro_;

	return m_async_worker->run_async<optional<core::files::ifile>>([=](core::async::iasync_op<core::files::ifile>)->optional<core::files::ifile>
	{
		core::files::ifile_t file;
		if (!this->create_handle(path, flags, &file, macro))
			return error(error_code::file_not_found);
		return file;
	});
}

bool resource_manager::open(cstr_t sid, core::files::input_text_file_ptr_t file, cstr_t macro)
{
	auto path = find_source(sid);
	if (path != null)
		return m_file_system->open(path, file, macro);
	else
		return m_file_system->open(sid, file, macro);
}

bool resource_manager::open(cstr_t sid, core::files::output_text_file_ptr_t file, cstr_t macro)
{
	auto path = find_source(sid);
	if (path != null)
		return m_file_system->open(path, file, macro);
	else
		return m_file_system->open(sid, file, macro);
}

bool resource_manager::open(cstr_t sid, core::files::input_binary_file_ptr_t file, cstr_t macro)
{
	auto path = find_source(sid);
	if (path != null)
		return m_file_system->open(path, file, macro);
	else
		return m_file_system->open(sid, file, macro);
}

bool resource_manager::open(cstr_t sid, core::files::output_binary_file_ptr_t file, cstr_t macro)
{
	auto path = find_source(sid);
	if (path != null)
		return m_file_system->open(path, file, macro);
	else
		return m_file_system->open(sid, file, macro);
}

ifactory_t resource_manager::factory()const
{
	return m_factory.get();
}

core::files::ifile_system_t resource_manager::file_system()const
{
	return const_cast<self*>(this);
}

string resource_manager::find_source(cstr_t sid)const
{
	core::async::scope_locker<core::async::mutex_t> lock = m_fs_mutex;
	auto it = m_file_map.find(sid);
	if (it.is_valid())
		return it->value;	
	return null;
}

bool resource_manager::load_sources(dom::xml::ixml_node_t sources)
{
	if (sources.is_empty() || !sources->has_children())
		return false;
	for (auto source : sources->children())
	{
		if (source->name()->as<cstr_t>() == "file")
		{
			auto sid = source->attributes()["sid"]->as<cstr_t>();
			core::async::scope_locker<core::async::mutex_t> lock = m_fs_mutex;
			m_file_map[sid] = source->value()->as<cstr_t>();
		}
	}
	return true;
}

bool resource_manager::load(cstr_t fileName)
{
	core::files::input_text_file_t file;
	if (!open(fileName, &file))
		return false;
	auto doc = dom::xml::xml_document_from_file(file);
	if (doc.failed())
		return false;
	return load(doc->root_element());
}

core::async::iasync<bool> resource_manager::load_async(cstr_t fileName_)
{
	string fileName = fileName_;
	return create_task<bool>([=](core::async::iasync<bool>)->bool
	{
		core::files::input_text_file_t file;
		if (!open(fileName, &file))
			return false;
		auto doc = dom::xml::xml_document_from_file(file);
		if (doc.failed())
			return false;
		return load(doc->root_element());
	});
}

optional<ilibrary> resource_manager::load_library(cstr_t fileName)
{
	core::files::input_text_file_t file;
	if (!open(fileName, &file))
		return error(error_code::invalid_param);
	auto doc = dom::xml::xml_document_from_file(file);
	if (doc.failed())
		return error(error_code::invalid_param);
	return load_library(doc->root_element());
}

core::async::iasync_op<ilibrary> resource_manager::load_library_async(cstr_t fileName_)
{
	string fileName = fileName_;
	return create_task<optional<ilibrary>>([=](core::async::iasync_op<ilibrary>)->optional<ilibrary>
	{
		core::files::input_text_file_t file;
		if (!open(fileName, &file))
			return error(error_code::invalid_param);
		auto doc = dom::xml::xml_document_from_file(file);
		if (doc.failed())
			return error(error_code::invalid_param);
		return load_library(doc->root_element());
	});
}