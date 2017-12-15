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
#include "ang/core/files.hpp"
#include "file_system.h"

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
		return "lowest"_s;
	case file_system_priority::highest:
		return "highest"_s;
	default:
		return "unknown"_s;
	}
}

template<text::encoding> open_flags_t get_encoding();

template<> open_flags_t get_encoding<text::encoding::utf16>() {
	return text::is_little_endian() ? open_flags::format_utf16_le : open_flags::format_utf16_be;
}

template<> open_flags_t get_encoding<text::encoding::utf16_se>() {
	return !text::is_little_endian() ? open_flags::format_utf16_le : open_flags::format_utf16_be;
}

template<> open_flags_t get_encoding<text::encoding::utf32>() {
	return text::is_little_endian() ? open_flags::format_utf32_le : open_flags::format_utf32_be;
}

template<> open_flags_t get_encoding<text::encoding::utf32_se>() {
	return !text::is_little_endian() ? open_flags::format_utf32_le : open_flags::format_utf32_be;
}

template<> open_flags_t get_encoding<text::encoding::unicode>() {
	return get_encoding<text::native_encoding<text::encoding::unicode>()>();
}


open_flags_t ang::core::files::get_format(text::encoding_t encoding)
{
	switch (encoding.get())
	{
	case text::encoding::ascii: return open_flags::format_ascii;
	case text::encoding::utf8: return open_flags::format_utf8;
	case text::encoding::utf16_le: return open_flags::format_utf16_le;
	case text::encoding::utf16_be: return open_flags::format_utf16_be;
	case text::encoding::utf32_le: return open_flags::format_utf32_le;
	case text::encoding::utf32_be: return open_flags::format_utf32_be;
	case text::encoding::unicode: return get_encoding<text::encoding::unicode>();
	case text::encoding::utf16: return get_encoding<text::encoding::utf16>();
	case text::encoding::utf32: return get_encoding<text::encoding::utf32>();
	default:  return open_flags::format_utf8; //utf8 for default
	}
}


text::encoding_t ang::core::files::get_format(open_flags_t encoding)
{
	if (!encoding.is_active(open_flags::format_text))
		return text::encoding::binary;

	switch (encoding.value() & 0X1F)
	{
	case open_flags::format_ascii: return text::encoding::ascii;
	case open_flags::format_utf8: return text::encoding::utf8;
	case open_flags::format_utf16_le: return text::encoding::utf16_le;
	case open_flags::format_utf16_be: return text::encoding::utf16_be;
	case open_flags::format_utf32_le: return text::encoding::utf32_le;
	case open_flags::format_utf32_be: return text::encoding::utf32_be;
	default:  return text::encoding::utf8; //utf8 for default
	}
}

/////////////////////////////////////////////////////////////////////////////

ang::intf_wrapper<ifile>::intf_wrapper() : _ptr(null) {

}

ang::intf_wrapper<ifile>::~intf_wrapper() {
	clean();
}

ang::intf_wrapper<ifile>::intf_wrapper(ifile* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<ifile>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ifile * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<ifile>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

void ang::intf_wrapper<ifile>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::intf_wrapper<ifile>::is_empty()const
{
	return _ptr == null;
}

ifile* ang::intf_wrapper<ifile>::get(void)const
{
	return _ptr;
}

void ang::intf_wrapper<ifile>::set(ifile* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::intf_wrapper<ifile>& ang::intf_wrapper<ifile>::operator = (ifile* ptr)
{
	set(ptr);
	return*this;
}

ang::intf_wrapper<ifile>& ang::intf_wrapper<ifile>::operator = (ang::intf_wrapper<ifile> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::intf_wrapper<ifile>& ang::intf_wrapper<ifile>::operator = (ang::intf_wrapper<ifile> const& other)
{
	set(other._ptr);
	return*this;
}

ifile ** ang::intf_wrapper<ifile>::addres_of(void)
{
	return &_ptr;
}

ang::intf_wrapper_ptr<ifile> ang::intf_wrapper<ifile>::operator & (void)
{
	return this;
}

ifile * ang::intf_wrapper<ifile>::operator -> (void)
{
	return get();
}

ifile const* ang::intf_wrapper<ifile>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<ifile>::operator ifile * (void)
{
	return get();
}

ang::intf_wrapper<ifile>::operator ifile const* (void)const
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

bool file::is_inherited_of(type_name_t name)
{
	return name == type_of<file>()
		|| object::is_inherited_of(name);
}

bool file::is_kind_of(type_name_t name)const
{
	return name == type_of<file>()
		|| object::is_kind_of(name);
}

bool file::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_of<file>())
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

bool file::unmap(ibuffer_t buffer, wsize size)
{
	if (buffer.is_empty())
		return false;
	return hfile->unmap(buffer, size);
}

bool file::is_valid()const
{
	return !hfile.is_empty();
}

file_size_t file::file_size()const
{
	return is_valid() ? hfile->stream_size() : 0;
}

bool file::file_size(file_size_t size)
{
	if(hfile.is_empty())
		return false;

	if (hfile->stream_size() > size)
		return hfile->stream_size(size);
	file_size_t pos = hfile->position();
	switch (hfile->mode().get())
	{
	case streams::stream_mode::out:
	case streams::stream_mode::inout: 		
		hfile->move_to(size, file_reference::begin);
		hfile->move_to(pos, file_reference::begin);
		return true;
	default:
		return false;
	}
}

streams::stream_mode_t file::mode()const
{
	return is_valid() ? hfile->mode().get() : streams::stream_mode::unknow;
}

bool file::set_mutex(core::async::mutex_t& mutex)
{
	return is_valid() ? hfile->set_mutex(mutex) : false;
}

bool file::close()
{
	hfile = null;
	return true;
}
