#include "pch.h"
#include "resources/resources.h"

using namespace ang;
using namespace ang::graphics;
using namespace ang::graphics::resources;

resource_manager::resource_manager()
	: base(null)
{
	m_worker_tread = core::async::thread::create_thread();
	m_async_worker = m_worker_tread.get();
}

resource_manager::~resource_manager()
{

}

ANG_IMPLEMENT_INTERFACE_RUNTIME_INFO(ang::graphics::resources::resource_manager);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::graphics::resources::resource_manager, library, core::files::ifile_system);
ANG_IMPLEMENT_INTERFACE_QUERY_INTERFACE(ang::graphics::resources::resource_manager, library, core::files::ifile_system);

void resource_manager::clear()
{
	m_factory = null;
	m_file_map = null;
	m_resource_map = null;
	m_fx_library = null;
	m_tex_loader = null;
}

array_view<string> resource_manager::paths(core::files::file_system_priority_t priority)const
{
	return core::files::ifile_system::fs_instance()->paths(priority);
}

void resource_manager::push_path(cstr_t path, core::files::file_system_priority_t priority)
{
	return core::files::ifile_system::fs_instance()->push_path(path, priority);
}

bool resource_manager::open_file(cstr_t path, core::files::open_flags_t flags, core::files::ifile_ptr_t file)
{
	try {
		return core::files::ifile_system::fs_instance()->open_file((cstr_t)m_file_map[path], flags, file);
	}
	catch (...) {
		return core::files::ifile_system::fs_instance()->open_file(path, flags, file);
	}
}

bool resource_manager::open(cstr_t path, core::files::input_text_file_ptr_t file)
{
	try {
		return core::files::ifile_system::fs_instance()->open((cstr_t)m_file_map[path], file);
	}
	catch (...) {
		return core::files::ifile_system::fs_instance()->open(path, file);
	}
}

bool resource_manager::open(cstr_t path, core::files::output_text_file_ptr_t file)
{
	try {
		return core::files::ifile_system::fs_instance()->open((cstr_t)m_file_map[path], file);
	}
	catch (...) {
		return core::files::ifile_system::fs_instance()->open(path, file);
	}
}

bool resource_manager::open(cstr_t path, core::files::input_binary_file_ptr_t file)
{
	try {
		return core::files::ifile_system::fs_instance()->open((cstr_t)m_file_map[path], file);
	}
	catch (...) {
		return core::files::ifile_system::fs_instance()->open(path, file);
	}
}

bool resource_manager::open(cstr_t path, core::files::output_binary_file_ptr_t file)
{
	try {
		return core::files::ifile_system::fs_instance()->open((cstr_t)m_file_map[path], file);
	}
	catch (...) {
		return core::files::ifile_system::fs_instance()->open(path, file);
	}
}

ifactory_t resource_manager::factory()const
{
	return m_factory.lock();
}

bool resource_manager::load_sources(dom::xml::xml_node_t sources)
{
	using namespace ang::dom;

	if (!sources->xml_has_children())
		return false;

	for (xml::xml_node_t file : sources->xml_children())
	{
		auto name = file->xml_name()->xml_as<cstr_t>();
		if (name != "file")
			continue;
		m_mutex.lock();
		m_file_map += {file->xml_attributes()["sid"]->xml_as<cstr_t>(), file->xml_value()->xml_as<cstr_t>()};
		m_mutex.unlock();
	}
	return true;
}

string resource_manager::find_source(cstr_t sid)const
{
	core::async::scope_locker<core::async::mutex_t> locker = m_mutex;
	try {
		return m_file_map[sid];
	}
	catch (...) {
		return null;
	}
}

iresource_t resource_manager::find_resource(cstr_t sid)const
{
	core::async::scope_locker<core::async::mutex_t> locker = m_mutex;
	auto it = m_resource_map->find(sid);
	if (it.is_valid())
		return it->value.lock();
	return null;
}

