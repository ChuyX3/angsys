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


#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::core;
using namespace ang::core::files;

bool_t ifile_system::register_file_system(ifile_system* fs, file_system_priority_t prio)
{
	return file_system::instance()->register_file_system(fs, prio);
}


file_system::file_system()
{
	_paths = new collections::vector_buffer<wstring>();
	highest_priority = new collections::vector_buffer<intf_wrapper<ifile_system>>();
	lowest_priority = new collections::vector_buffer<intf_wrapper<ifile_system>>();
}

file_system::~file_system()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::core::files::file_system);
ANG_IMPLEMENT_OBJECTNAME(ang::core::files::file_system);

bool_t file_system::is_child_of(type_name_t name)
{
	return name == type_name<file_impl>()
		|| object::is_child_of(name)
		|| name == type_name<ifile_system>();
}

bool_t file_system::is_kind_of(type_name_t name)const
{
	return name == type_name<file_impl>()
		|| object::is_kind_of(name)
		|| name == type_name<ifile_system>();
}

bool_t file_system::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<file_system>())
	{
		*out = static_cast<file_system*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<ifile_system>())
	{
		*out = static_cast<ifile_system*>(this);
		return true;
	}
	return false;
}

bool_t file_system::register_file_system(ifile_system* fs, file_system_priority_t prio)
{
	if (fs == null)
		return false;

	if (prio == file_system_priority::highest)
	{
		highest_priority += fs;
		return true;
	}
	else if (prio == file_system_priority::lowest)
	{
		highest_priority += fs;
		return true;
	}
	
	return false;
}

array<wstring> file_system::paths()const
{
	return static_cast<collections::ienum<wstring> const*>(_paths.get());
}

bool_t file_system::register_paths(cwstr_t path)
{
	auto it = _paths->find(path);
	if (it.is_valid())
		return false;
	_paths += path;
	return true;
}

bool_t file_system::create_file_handle(cwstr_t path, open_flags_t flags, ifile_ptr_t out)
{
	if (out.is_empty())
		return false;

	for(auto it = highest_priority->begin(); it.is_valid(); ++it) 
	{
		if ((*it)->create_file_handle(path, flags, out))
			return true;
	}

	file_impl* file = new file_impl();
	file->add_ref();
	file->create(path, flags);
	if (file->is_created())
	{
		*out = file;
		file->release();
		return true;
	}
	else
	{
		for (auto it = _paths->begin(); it.is_valid(); ++it)
		{
			wstring _path = (*it) + "/" + path;
			file->create(_path, flags);
			if (file->is_created())
			{
				*out = static_cast<ifile*>(file);
				file->release();
				return true;
			}
		}
		file->release();
	}

	for (auto it = lowest_priority->begin(); it.is_valid(); ++it)
	{
		if ((*it)->create_file_handle(path, flags, out))
			return true;
	}
	
	return false;
}

bool_t file_system::open(cwstr_t path, input_text_file_t& out)
{
	ifile_t _hfile;
	if (!create_file_handle(path, open_flags::access_in + open_flags::type_text + open_flags::open_exist, &_hfile))
		return false;
	out = new input_text_file();
	out->attach(_hfile);
	return true;
}

bool_t file_system::open(cwstr_t, output_text_file_t&)
{
	return false;
}

bool_t file_system::open(cwstr_t, input_binary_file_t&)
{
	return false;
}

bool_t file_system::open(cwstr_t, output_binary_file_t&)
{
	return false;
}

//bool_t file_system::OpenFile(Path path, OpenFlags flags, IFile** out)
//{
//	if (out == null)
//		return false;
//	_FileArgs args = { path, flags };
//	File* file = new File(&args);
//	file->AddRef();
//	if (!file->IsValid())
//	{
//		for (auto it = paths.Begin(); it.IsValid(); ++it)
//		{
//			args.pathName = (*it) / path;
//			file->Open(&args);
//			if (file->IsValid())
//			{
//				*out = file;
//				return true;
//			}
//		}
//
//		file->Release();
//		return false;
//	}
//	*out = file;
//	return true;
//}
