/*********************************************************************************************************************/
/*   File Name: file_system.cpp                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <ang/core/files.h>
#include "file_system.h"


//#if defined _DEBUG
//#define new new(__FILE__, __LINE__)
//#endif

using namespace ang;
using namespace ang::core;
using namespace ang::core::files;

static inline path_t view_to_path(cstr_t const& str) {
	return str;
	/*path_t path;
	switch (str.encoding())
	{
	case text::encoding::ascii: path = str.cstr<text::encoding::ascii>(); break;
	case text::encoding::unicode: path = str.cstr<text::encoding::unicode>(); break;
	case text::encoding::utf8: path = str.cstr<text::encoding::utf8>(); break;
	case text::encoding::utf16: path = str.cstr<text::encoding::utf16>(); break;
	case text::encoding::utf16_le: path = str.cstr<text::encoding::utf16_le>(); break;
	case text::encoding::utf16_be: path = str.cstr<text::encoding::utf16_be>(); break;
	case text::encoding::utf16_se: path = str.cstr<text::encoding::utf16_se>(); break;
	case text::encoding::utf32: path = str.cstr<text::encoding::utf32>(); break;
	case text::encoding::utf32_le: path = str.cstr<text::encoding::utf32_le>(); break;
	case text::encoding::utf32_be: path = str.cstr<text::encoding::utf32_be>(); break;
	case text::encoding::utf32_se: path = str.cstr<text::encoding::utf32_se>(); break;
	}
	return ang::move(path);*/
}

//bool ifile_system::register_file_system(ifile_system* fs, file_system_priority_t prio)
//{
//	return file_system::instance()->register_file_system(fs, prio);
//}

file_system::file_system()
	: m_paths()
	, m_highest_priority()
	, m_lowest_priority()
	, m_async_worker(null)
{
	//m_async_worker = async::thread::create_dispatcher_thread();
}

file_system::~file_system()
{

}

//COFFE_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::files::file_system);
//COFFE_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::files::file_system);
//COFFE_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::files::file_system, bean, ifile_system);

void file_system::dispose()
{
	m_lowest_priority.clear();
	m_highest_priority.clear();
	m_paths.clear();
	m_macros.clear();
	//m_async_worker->join();
	if(!m_async_worker.is_empty())
		m_async_worker->exit();
	m_async_worker = null;
}

bool file_system::register_file_system(ifile_system* fs, file_system_priority_t prio)
{
	auto scope = m_mutex.scope();
	if (fs == null)
		return false;

	if (prio == file_system_priority::highest)
	{
		m_highest_priority += fs;
		return true;
	}
	else if (prio == file_system_priority::lowest)
	{
		m_highest_priority += fs;
		return true;
	}	
	return false;
}

vector<string> file_system::paths(path_access_type_t access)const
{
	auto scope = m_mutex.scope();
	vector<string> paths;
	for (collections::pair<const path_t, path_access_type_t> const & pair  : m_paths)
	{
		if (pair.value == access)
			paths += new text::basic_string_buffer<path_encoding>(pair.key); //makes a copy to prevent external modifications
	}
	return ang::move(paths);
}

void file_system::push_path(cstr_t path_, path_access_type_t access, cstr_t macro_)
{
	auto scope = m_mutex.scope();
	path_t path = view_to_path(path_);
	m_paths[path] = access;
	if (macro_ != null)
	{
		path_t macro = view_to_path(macro_);
		m_macros[macro] += path;
	}
}

vector<string> file_system::find_paths(cstr_t macro_)const
{
	auto scope = m_mutex.scope();
	path_t macro = view_to_path(macro_);
	vector<string> paths;
	try { 
		for (auto const& path : m_macros[macro])
			paths += new text::basic_string_buffer<path_encoding>(path);
	}
	catch (...) { }
	return ang::move(paths);
}

path_access_type_t file_system::path_access_type(cstr_t path_)const
{
	auto scope = m_mutex.scope();
	path_t path = view_to_path(path_);
	try { return m_paths[path]; }
	catch (...) { return path_access_type::none; }
}

bool file_system::create_handle(cstr_t path_, open_flags_t flags, ifile_ptr_t out, cstr_t macro)
{
	if (out.is_empty())
		return false;

	if(m_mutex.sync([&]()
	{
		for (auto fs : m_highest_priority)
		{
			if (fs->create_handle(path_, flags, out, macro))
				return true;
		}
		return false;
	}))
		return true;

	if (macro != null) // if not null tries create file here only
	{
		try {
			path_t path;// = view_to_path(path_);
			//path_t macro = view_to_path(macro_);
			auto scope = m_mutex.scope();
			auto& macro_paths = m_macros[macro];
			if (flags.is_active(open_flags::access_inout))
			{
				system_file_t file = new core_file();
				for (path_t const& macro_path : macro_paths)
				{
					if (m_paths[macro_path].is_active(path_access_type::all))
					{
						path = macro_path;
						path << "/"_sv << view_to_path(path_);
						file->create(ang::move(path), flags);
						if (file->is_created())
						{
							*out = file;
							return true;
						}
					}
				}
			}
			else if (flags.is_active(open_flags::access_out))
			{
				system_file_t file = new core_file();
				for (path_t const& macro_path : macro_paths)
				{
					if (m_paths[macro_path].is_active(path_access_type::write))
					{
						path = macro_path;
						path << "/"_sv << view_to_path(path_);
						file->create(ang::move(path), flags);
						if (file->is_created())
						{
							*out = file;
							return true;
						}
					}
				}
			}
			else if (flags.is_active(open_flags::access_in))
			{
				system_file_t file = new core_file();
				for (path_t const& macro_path : macro_paths)
				{
					if (m_paths[macro_path].is_active(path_access_type::read))
					{
						path = macro_path;
						path << "/"_sv  << view_to_path(path_);
						file->create(ang::move(path), flags);
						if (file->is_created())
						{
							*out = file;
							return true;
						}
					}
				}
			}
			return false;
		}
		catch (...) {
			return false;
		}
	}

	system_file_t file = new core_file();
	file->create(view_to_path(path_), flags);
	if (file->is_created())
	{
		*out = file;
		return true;
	}
	else if (flags.is_active(open_flags::access_inout))
	{
		path_t path;
		auto scope = m_mutex.scope();
		for (auto const& pair : m_paths)
		{
			if (pair.value.is_active(path_access_type::all))
			{
				path = pair.key;
				path << "/"_sv << view_to_path(path_);
				file->create(ang::move(path), flags);
				if (file->is_created())
				{
					*out = static_cast<ifile*>(file);
					return true;
				}
			}	
		}
	}
	else if (flags.is_active(open_flags::access_out))
	{
		path_t path;
		auto scope = m_mutex.scope();
		for (auto const& pair : m_paths)
		{
			if (pair.value.is_active(path_access_type::write))
			{
				path = pair.key;
				path << "/"_sv << view_to_path(path_);
				file->create(ang::move(path), flags);
				if (file->is_created())
				{
					*out = static_cast<ifile*>(file);
					return true;
				}
			}
		}
	}
	else if (flags.is_active(open_flags::access_in))
	{
		path_t path;
		auto scope = m_mutex.scope();
		for (auto const& pair : m_paths)
		{
			if (pair.value.is_active(path_access_type::read))
			{
				path = pair.key;
				path << "/"_sv << view_to_path(path_);
				file->create(ang::move(path), flags);
				if (file->is_created())
				{
					*out = static_cast<ifile*>(file);
					return true;
				}
			}
		}
	}
	
	return m_mutex.sync([&]() 
	{
		for (auto fs : m_lowest_priority)
		{
			if (fs->create_handle(path_, flags, out))
				return true;
		}
		return false;
	});
}

async::iasync_op<ifile> file_system::create_handle_async(cstr_t path_, open_flags_t flags, cstr_t macro_) {
	string path = path_;
	string macro = macro_;
	if(m_async_worker.is_empty())
		m_async_worker = async::thread::create_dispatcher_thread();
	return m_async_worker->run_async<optional<ifile>>([=](async::iasync_op<ifile>)->optional<ifile>
	{
		ifile_t file;
		if (!this->create_handle(path, flags, &file, macro))
			return error(error_code::file_not_found);
		return file;
	});
}

bool file_system::open(cstr_t path, input_text_file_ptr_t out, cstr_t macro)
{
	ifile_t _hfile;
	if (out.is_empty()) 
		return false;
	if (!create_handle(path, open_flags_t(open_flags::access_in) + open_flags::format_text + open_flags::open_exist, &_hfile, macro))
		return false;
	*out = new input_text_file();
	(*out)->attach(_hfile);
	return true;
}

bool file_system::open(cstr_t path, output_text_file_ptr_t out, cstr_t macro)
{
	ifile_t _hfile;
	if (out.is_empty())
		return false;
	if (!create_handle(path, open_flags_t(open_flags::access_out) + open_flags::format_text + open_flags::open_alway, &_hfile, macro))
		return false;
	//*out = new output_text_file();
	//(*out)->attach(_hfile);
	return true;
}

bool file_system::open(cstr_t path, input_binary_file_ptr_t out, cstr_t macro)
{
	ifile_t _hfile;
	if (!create_handle(path, open_flags_t(open_flags::access_in) + open_flags::format_binary + open_flags::open_exist, &_hfile, macro))
		return false;
	*out = new input_binary_file();
	(*out)->attach(_hfile);
	return true;
}

bool file_system::open(cstr_t path, output_binary_file_ptr_t out, cstr_t macro)
{
	ifile_t _hfile;
	if (!create_handle(path, open_flags_t(open_flags::access_out) + open_flags::format_binary + open_flags::open_alway, &_hfile, macro))
		return false;
//	*out = new output_binary_file();
//	(*out)->attach(_hfile);
	return true;
}



////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
//ifile_system_t ifile_system::create_file_system(path root)
//{
//	return new folder_file_system(root);
//}
//
//folder_file_system::folder_file_system(path root)
//{
//	_root_path = root;
//	m_paths = new collections::vector_buffer<path>();
//}
//
//folder_file_system::~folder_file_system()
//{
//	_root_path = null;
//	m_paths = null;
//}
//
//COFFE_IMPLEMENT_CLASSNAME(ang::core::files::folder_file_system);
//COFFE_IMPLEMENT_OBJECTNAME(ang::core::files::folder_file_system);
//
//bool folder_file_system::is_inherited_of(type_name_t name)
//{
//	return name == type_of<core_file>()
//		|| object::is_inherited_of(name)
//		|| ifile_system::is_inherited_of(name);
//}
//
//bool folder_file_system::is_kind_of(type_name_t name)const
//{
//	return name == type_of<core_file>()
//		|| object::is_kind_of(name)
//		|| ifile_system::is_kind_of(name);
//}
//
//bool folder_file_system::query_object(type_name_t name, unknown_ptr_t out)
//{
//	if (out == null)
//		return false;
//
//	if (name == type_of<folder_file_system>())
//	{
//		*out = static_cast<folder_file_system*>(this);
//		return true;
//	}
//	else if (ang::object::query_object(name, out))
//	{
//		return true;
//	}
//	else if (name == type_of<ifile_system>())
//	{
//		*out = static_cast<ifile_system*>(this);
//		return true;
//	}
//	return false;
//}
//
//array<path> folder_file_system::paths()const
//{
//	return static_cast<collections::ienum<path> const*>(m_paths.get());
//}
//
//bool folder_file_system::register_paths(cstr_t path)
//{
//	auto it = m_paths->find([&](string const& p) { return path == (cstr_t)p; });
//	if (it.is_valid())
//		return false;
//	m_paths += path;
//	return true;
//}
//
//bool folder_file_system::create_handle(cstr_t path, open_flags_t flags, ifile_ptr_t out)
//{
//	if (out.is_empty())
//		return false;
//
//	files::path _path = (_root_path + "\\"_sv) += path;
//
//	system_file_t file = new core_file();
//	file->create(_path, flags);
//	if (file->is_created())
//	{
//		*out = file;
//		return true;
//	}
//	else
//	{
//		for (auto it = m_paths->begin(), end = m_paths->end(); it != end; ++it)
//		{
//			_path = (((_root_path + "\\"_sv) += (*it)) += "\\"_sv) += path;
//			file->create(_path, flags);
//			if (file->is_created())
//			{
//				*out = static_cast<ifile*>(file);
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//bool folder_file_system::open(cstr_t path, input_text_file_t& out)
//{
//	ifile_t _hfile;
//	if (!create_handle(path, open_flags::access_in + open_flags::format_text + open_flags::open_exist, &_hfile))
//		return false;
//	out = new input_text_file();
//	out->attach(_hfile);
//	return true;
//}
//
//bool folder_file_system::open(cstr_t path, output_text_file_t& out)
//{
//	ifile_t _hfile;
//	if (!create_handle(path, open_flags::access_out + open_flags::format_text + open_flags::open_alway, &_hfile))
//		return false;
//	out = new output_text_file();
//	out->attach(_hfile);
//	return true;
//}
//
//bool folder_file_system::open(cstr_t path, input_binary_file_t& out)
//{
//	ifile_t _hfile;
//	if (!create_handle(path, open_flags::access_in + open_flags::format_binary + open_flags::open_exist, &_hfile))
//		return false;
//	out = new input_binary_file();
//	out->attach(_hfile);
//	return true;
//}
//
//bool folder_file_system::open(cstr_t path, output_binary_file_t& out)
//{
//	ifile_t _hfile;
//	if (!create_handle(path, open_flags::access_out + open_flags::format_binary + open_flags::open_alway, &_hfile))
//		return false;
//	out = new output_binary_file();
//	out->attach(_hfile);
//	return true;
//}

///////////////////////////////////////////////////////////////
