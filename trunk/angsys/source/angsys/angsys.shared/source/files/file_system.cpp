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


core::files::ifile_system_t core::files::ifile_system::fs_instance() {
	return ang::singleton<core::files::file_system_t>::instance().get();
}

//bool ifile_system::register_file_system(ifile_system* fs, file_system_priority_t prio)
//{
//	return file_system::instance()->register_file_system(fs, prio);
//}


file_system::file_system()
	: m_paths(null)
	, m_highest_priority(null)
	, m_lowest_priority(null)
{
	m_paths = new collections::vector_buffer<path_t>();
	m_highest_priority = new collections::vector_buffer<intf_wrapper<ifile_system>>();
	m_lowest_priority = new collections::vector_buffer<intf_wrapper<ifile_system>>();
}

file_system::~file_system()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::files::file_system);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::files::file_system, object, ifile_system);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::files::file_system, object, ifile_system);

bool file_system::register_file_system(ifile_system* fs, file_system_priority_t prio)
{
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

array_view<path_t> file_system::paths(file_system_priority_t p)const
{
	return m_paths;
}

void file_system::push_path(path_view_t path, file_system_priority_t p)
{
	auto it = m_paths->find([&](path_t const& p)
	{ 
		return path == (path_view_t)p;
	});

	if (it.is_valid())
		return;

	if (p == file_system_priority::highest)
	{
		m_paths->push(path, false);
	}
	else if (p == file_system_priority::lowest)
	{
		m_paths->push(path, true);
	}
}

bool file_system::open_file(path_view_t path, open_flags_t flags, ifile_ptr_t out)
{
	if (out.is_empty())
		return false;

	for(auto fs : m_highest_priority)
	{
		if (fs->open_file(path, flags, out))
			return true;
	}

	system_file_t file = new core_file();
	file->create(path, flags);
	if (file->is_created())
	{
		*out = file;
		return true;
	}
	else
	{
		for (wstring p : m_paths)
		{
			path_t _path = (p + "/"_s) += path;
			file->create(_path, flags);
			if (file->is_created())
			{
				*out = static_cast<ifile*>(file);
				return true;
			}
		}
	}

	for (auto fs : m_lowest_priority)
	{
		if (fs->open_file(path, flags, out))
			return true;
	}
	
	return false;
}

bool file_system::open(path_view_t path, input_text_file_ptr_t out)
{
	ifile_t _hfile;
	if (out.is_empty()) 
		return false;
	if (!open_file(path, open_flags::access_in + open_flags::format_text + open_flags::open_exist, &_hfile))
		return false;
	*out = new input_text_file();
	(*out)->attach(_hfile);
	return true;
}

bool file_system::open(path_view_t path, output_text_file_ptr_t out)
{
	ifile_t _hfile;
	if (out.is_empty())
		return false;
	if (!open_file(path, open_flags::access_out + open_flags::format_text + open_flags::open_alway, &_hfile))
		return false;
	//*out = new output_text_file();
	//(*out)->attach(_hfile);
	return true;
}

bool file_system::open(path_view_t path, input_binary_file_ptr_t out)
{
	ifile_t _hfile;
	if (!open_file(path, open_flags::access_in + open_flags::format_binary + open_flags::open_exist, &_hfile))
		return false;
	*out = new input_binary_file();
	(*out)->attach(_hfile);
	return true;
}

bool file_system::open(path_view_t path, output_binary_file_ptr_t out)
{
	ifile_t _hfile;
	if (!open_file(path, open_flags::access_out + open_flags::format_binary + open_flags::open_alway, &_hfile))
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
//ANG_IMPLEMENT_CLASSNAME(ang::core::files::folder_file_system);
//ANG_IMPLEMENT_OBJECTNAME(ang::core::files::folder_file_system);
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
//bool folder_file_system::register_paths(path_view_t path)
//{
//	auto it = m_paths->find([&](path_t const& p) { return path == (path_view_t)p; });
//	if (it.is_valid())
//		return false;
//	m_paths += path;
//	return true;
//}
//
//bool folder_file_system::open_file(path_view_t path, open_flags_t flags, ifile_ptr_t out)
//{
//	if (out.is_empty())
//		return false;
//
//	files::path _path = (_root_path + "\\"_s) += path;
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
//			_path = (((_root_path + "\\"_s) += (*it)) += "\\"_s) += path;
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
//bool folder_file_system::open(path_view_t path, input_text_file_t& out)
//{
//	ifile_t _hfile;
//	if (!open_file(path, open_flags::access_in + open_flags::format_text + open_flags::open_exist, &_hfile))
//		return false;
//	out = new input_text_file();
//	out->attach(_hfile);
//	return true;
//}
//
//bool folder_file_system::open(path_view_t path, output_text_file_t& out)
//{
//	ifile_t _hfile;
//	if (!open_file(path, open_flags::access_out + open_flags::format_text + open_flags::open_alway, &_hfile))
//		return false;
//	out = new output_text_file();
//	out->attach(_hfile);
//	return true;
//}
//
//bool folder_file_system::open(path_view_t path, input_binary_file_t& out)
//{
//	ifile_t _hfile;
//	if (!open_file(path, open_flags::access_in + open_flags::format_binary + open_flags::open_exist, &_hfile))
//		return false;
//	out = new input_binary_file();
//	out->attach(_hfile);
//	return true;
//}
//
//bool folder_file_system::open(path_view_t path, output_binary_file_t& out)
//{
//	ifile_t _hfile;
//	if (!open_file(path, open_flags::access_out + open_flags::format_binary + open_flags::open_alway, &_hfile))
//		return false;
//	out = new output_binary_file();
//	out->attach(_hfile);
//	return true;
//}

///////////////////////////////////////////////////////////////
