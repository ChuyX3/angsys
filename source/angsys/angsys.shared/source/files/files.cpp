/*********************************************************************************************************************/
/*   File Name: files.cpp                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "ang/core/files.h"

using namespace ang;
using namespace ang::core::files;

ANG_IMPLEMENT_FLAGS(ang::core::files, open_flags, uint);
ANG_IMPLEMENT_INTERFACE(ang::core::files, ifile);
ANG_IMPLEMENT_INTERFACE(ang::core::files, ifile_system);

ANG_IMPLEMENT_ENUM(ang::core::files, file_system_priority, uint, file_system_priority::lowest);

cstr_t file_system_priority_t::to_string()const
{
	switch (_value)
	{
	case file_system_priority::lowest:
		return "lowest";
	case file_system_priority::highest:
		return "highest";
	default:
		return "unknown";
	}
}



/////////////////////////////////////////////////////////////////////////////

inline ang::intf_wrapper<ifile>::intf_wrapper() : _ptr(null) {

}

inline ang::intf_wrapper<ifile>::~intf_wrapper() {
	clean();
}

inline ang::intf_wrapper<ifile>::intf_wrapper(ifile* ptr) : _ptr(null) {
	set(ptr);
}

inline ang::intf_wrapper<ifile>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ifile * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

inline ang::intf_wrapper<ifile>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

inline void ang::intf_wrapper<ifile>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

inline bool ang::intf_wrapper<ifile>::is_empty()const
{
	return _ptr == null;
}

inline ifile* ang::intf_wrapper<ifile>::get(void)const
{
	return _ptr;
}

inline void ang::intf_wrapper<ifile>::set(ifile* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

inline ang::intf_wrapper<ifile>& ang::intf_wrapper<ifile>::operator = (ifile* ptr)
{
	set(ptr);
	return*this;
}

inline ang::intf_wrapper<ifile>& ang::intf_wrapper<ifile>::operator = (ang::intf_wrapper<ifile> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

inline ang::intf_wrapper<ifile>& ang::intf_wrapper<ifile>::operator = (ang::intf_wrapper<ifile> const& other)
{
	set(other._ptr);
	return*this;
}

inline ifile ** ang::intf_wrapper<ifile>::addres_of(void)
{
	return &_ptr;
}

inline ang::intf_wrapper_ptr<ifile> ang::intf_wrapper<ifile>::operator & (void)
{
	return this;
}

inline ifile * ang::intf_wrapper<ifile>::operator -> (void)
{
	return get();
}

inline ifile const* ang::intf_wrapper<ifile>::operator -> (void)const
{
	return get();
}

inline ang::intf_wrapper<ifile>::operator ifile * (void)
{
	return get();
}

inline ang::intf_wrapper<ifile>::operator ifile const* (void)const
{
	return get();
}

////////////////////////////////////////////////////////////////////////////////////

file::file()
	: hfile(null)

{

}

file::~file()
{
	close();
}


ANG_IMPLEMENT_CLASSNAME(ang::core::files::file);
ANG_IMPLEMENT_OBJECTNAME(ang::core::files::file);

bool file::is_child_of(type_name_t name)
{
	return name == type_name<file>()
		|| object::is_child_of(name);
}

bool file::is_kind_of(type_name_t name)const
{
	return name == type_name<file>()
		|| object::is_kind_of(name);
}

bool file::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<file>())
	{
		*out = static_cast<file*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	return false;
}

bool file::create(path_view path, open_flags_t flags)
{
	auto fs = ifile_system::get_file_system();
	if (fs == null)
		return false;
	if (!fs->create_file_handle(path , flags, &hfile))
		return false;

	return true;
}

bool file::attach(ifile* f)
{
	close();
	hfile = f;
	return true;
}

ibuffer_t file::map(wsize size, file_cursor_t offset)
{
	if (hfile.is_empty())
		return null;
	return hfile->map(size, offset);
}

bool file::unmap(ibuffer_t buffer)
{
	if (buffer.is_empty())
		return false;
	return hfile->unmap(buffer);
}

bool file::is_valid()const
{
	return !hfile.is_empty();
}

file_size_t file::file_size()const
{
	return is_valid() ? hfile->file_size() : 0;
}

bool file::file_size(file_size_t size)
{
	if(hfile.is_empty())
		return false;

	if (hfile->file_size() > size)
		return hfile->file_size(size);
	file_size_t pos = hfile->cursor();
	switch (hfile->mode().get())
	{
	case streams::stream_mode::out:
	case streams::stream_mode::inout: 		
		hfile->cursor(file_reference::begin, size);
		hfile->cursor(file_reference::begin, pos);
		return true;
	default:
		return false;
	}
}

streams::stream_mode_t file::mode()const
{
	return is_valid() ? hfile->mode().get() : streams::stream_mode::unknow;
}

bool file::set_mutex(core::async::mutex_t mutex)
{
	return is_valid() ? hfile->set_mutex(mutex) : false;
}

bool file::close()
{
	hfile = null;
	return true;
}
