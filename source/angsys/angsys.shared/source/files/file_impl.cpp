/*********************************************************************************************************************/
/*   File Name: file_impl.cpp                                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <ang/core/files.hpp>
#include "file_system.h"
#include "string_helper.hpp"

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

using namespace ang;
using namespace ang::core;
using namespace ang::core::files;


////////////////////////////////////////////////////////////////////////

mapped_file_buffer::mapped_file_buffer(system_file_t file, file_flags_t access, wsize size, ulong64 offset)
	: _original_source(null)
	, _buffer_ptr(null)
	, _buffer_size(0)
	, _buffer_offset(0)
	, _access_flag(file_flags::null)
{
	_original_source = file;
	map(access, size, offset);
}

mapped_file_buffer::~mapped_file_buffer()
{
	_original_source->unmap(this);
}

ANG_IMPLEMENT_CLASSNAME(ang::core::files::mapped_file_buffer);
ANG_IMPLEMENT_OBJECTNAME(ang::core::files::mapped_file_buffer);

bool mapped_file_buffer::is_inherited_of(type_name_t name)
{
	return name == type_of<file_impl>()
		|| object::is_inherited_of(name)
		|| name == type_of<ibuffer>();
}

bool mapped_file_buffer::is_kind_of(type_name_t name)const
{
	return name == type_of<file_impl>()
		|| object::is_kind_of(name)
		|| name == type_of<ibuffer>();
}

bool mapped_file_buffer::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_of<mapped_file_buffer>())
	{
		*out = static_cast<mapped_file_buffer*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_of<ibuffer>())
	{
		*out = static_cast<ibuffer*>(this);
		return true;
	}
	return false;
}

bool mapped_file_buffer::map(file_flags_t access, wsize size, ulong64 offset)
{
	_access_flag = access.is_active(file_flags::access_out) ? file_flags::access_out : file_flags::access_in;
	_access_flag += (file_flags)(access.value() & 0X1F);
#ifdef WINDOWS_PLATFORM
	dword accessFlags = access.is_active(file_flags::access_out) ? PAGE_READWRITE : PAGE_READONLY;
	pointer mapp_handle = _original_source->map_handle(size + offset);

#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	LARGE_INTEGER lint;
	lint.QuadPart = offset;
	_buffer_ptr = MapViewOfFile(mapp_handle, access.is_active(file_flags::access_out) ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ, lint.HighPart, lint.LowPart, size);
#else//STOREAPP
	_buffer_ptr = MapViewOfFileFromApp(mapp_handle, access.is_active(file_flags::access_out) ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ, offset, size);
#endif//DESKTOP

	if (_buffer_ptr)
	{	
		_buffer_size = size;
		_buffer_offset = offset;
	}

#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	int prot = access.is_active(file_flags::access_out) ? PROT_WRITE | PROT_READ : PROT_READ;
	int flags = MAP_SHARED;

	_buffer_ptr = mmap(0, size, prot, flags, _original_source->handle(), offset);
	_buffer_size = size;
	_buffer_offset = offset;
	if (_buffer_ptr == MAP_FAILED)
	{
		_buffer_ptr = null;
		_buffer_size = 0;
		_buffer_offset = 0;
	}
#endif

	return _buffer_ptr ? true : false;
}

bool mapped_file_buffer::unmap()
{
	if (_buffer_ptr == null)
		return false;
#ifdef WINDOWS_PLATFORM
	UnmapViewOfFile(_buffer_ptr);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	munmap(_buffer_ptr, _buffer_size);
#endif
	_buffer_ptr = null;
	_buffer_size = 0;
	return true;
}

system_file_t mapped_file_buffer::original_source()const
{
	return _original_source;
}

pointer mapped_file_buffer::buffer_ptr()const
{
	return _buffer_ptr;
}

wsize mapped_file_buffer::buffer_size()const
{
	return _buffer_size;
}

text::encoding_t mapped_file_buffer::encoding()const
{
	return get_format(_access_flag);
}

wsize mapped_file_buffer::mem_copy(wsize size, pointer data, text::encoding_t encoding)
{
	if (!_access_flag.is_active(file_flags::access_out))
		return 0;
	if (_access_flag.is_active(file_flags::format_text))
	{
		if (encoding == text::encoding::binary)
			return 0; 

		raw_str_t dest(_buffer_ptr, _buffer_size, this->encoding());
		raw_str_t src(data, size, encoding);
		windex written = 0;

		text::encoder_interface encoder;
		text::encoder_interface::initialize_interface(&encoder, dest.encoding());
		return dest.char_size() * encoder._convert_string(dest.ptr(), dest.count(), src.ptr(), written, src.encoding(), true);
	}
	else
	{
		auto max_size = min(size, _buffer_size);
		memcpy(_buffer_ptr, data, max_size);
		return max_size;
	}
}

ibuffer_view_t mapped_file_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) >= _buffer_size)
		return null;
	return new buffer_view(this, start, size);
}

bool mapped_file_buffer::unmap_buffer(ibuffer_view_t& buffer, wsize)
{
	if (buffer.is_empty())
		return false;
	auto pos = wsize(buffer->buffer_ptr());
	auto start = wsize(_buffer_ptr);

	if (pos < start || pos >(start + _buffer_size))
		return false;
	buffer = null;
	return true;
}

bool mapped_file_buffer::can_realloc_buffer()const
{
	return true;
}

bool mapped_file_buffer::realloc_buffer(wsize size)
{
	if (_buffer_size >= size)
		return false;
	if (size <= _buffer_size)
		return false;

	file_size_t new_size = 128UL;
	while (new_size < (file_size_t)size)
		new_size *= 2;

	auto _old_ptr = _buffer_ptr;
	auto _old_offset = _buffer_offset;
	auto _old_size = _buffer_size;

	if (map(_access_flag, (wsize)new_size, (ulong64)_old_offset))
	{
#ifdef WINDOWS_PLATFORM
		UnmapViewOfFile(_old_ptr);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		munmap(_old_ptr, _old_size);
#endif
		return true;
	}
	else
	{
		_buffer_ptr = _old_ptr;
		_buffer_offset = _old_offset;
		_buffer_size = _old_size;
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////

file_size_t file_impl::get_file_size(file_handle_t h)
{
	ulong64 size = 0;
#ifdef WINDOWS_PLATFORM
	LARGE_INTEGER lint;
	lint.QuadPart = 0;
	SetFilePointerEx(h, lint, (PLARGE_INTEGER)&size, FILE_END);
	SetFilePointerEx(h, lint, 0, FILE_BEGIN);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	size = lseek(h, 0, SEEK_END);
	lseek(h, 0, SEEK_SET);
#endif
	return size;
}

text::encoding_t file_impl::get_file_encoding(file_handle_t handle)
{
	dword bom[2] = { 0,0 };
	dword offset = 0;
	text::encoding_t result = text::encoding::ascii;
#ifdef WINDOWS_PLATFORM
	dword out;
	LARGE_INTEGER lint;
	lint.QuadPart = 0;
	SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
	ReadFile(handle, bom, 4, &out, NULL);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	lseek(handle, 0, SEEK_SET);
	::read(handle, bom, 4);
#endif

	if (load_bom<text::encoding::utf8>(bom) > 0)
	{
		offset = 3;
		result = text::encoding::utf8;
	}
	else if (load_bom<text::encoding::utf32_be>(bom) > 0)
	{
		offset = 4;
		result = text::encoding::utf32_be;
	}
	else if (load_bom<text::encoding::utf32_le>(bom) > 0)
	{
		offset = 4;
		result = text::encoding::utf32_le;
	}
	else if (load_bom<text::encoding::utf16_be>(bom) > 0)
	{
		offset = 2;
		result = text::encoding::utf16_be;
	}
	else if (load_bom<text::encoding::utf16_le>(bom) > 0)
	{
		offset = 2;
		result = text::encoding::utf16_le;
	}
#ifdef WINDOWS_PLATFORM
	lint.QuadPart = offset;
	SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	::lseek(handle, offset, SEEK_SET);
#endif
	return result;
}

void file_impl::set_file_encoding(file_handle_t handle, text::encoding_t encoding)
{
	alignas(4) static byte utf8_bom[4] = { 0xef, 0xbb, 0xbf, 0x0 };
	alignas(4) static byte utf16_le_bom[4] = { 0xff, 0xfe, 0x0, 0x0 };
	alignas(4) static byte utf16_be_bom[4] = { 0xfe, 0xff, 0x0, 0x0 };
	alignas(4) static byte utf32_le_bom[8] = { 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	alignas(4) static byte utf32_be_bom[8] = { 0x0, 0x0, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0 };
	pointer ptr = null;
	wsize sz = 0;
	switch (encoding)
	{
	case text::encoding::utf8:
		ptr = utf8_bom;
		sz = 3;
		break;
	case text::encoding::utf16_le:
		ptr = utf16_le_bom;
		sz = 2;
		break;
	case text::encoding::utf16_be:
		ptr = utf16_be_bom;
		sz = 2;
		break;
	case text::encoding::utf32_le:
		ptr = utf32_le_bom;
		sz = 4;
		break;
	case text::encoding::utf32_be:
		ptr = utf32_be_bom;
		sz = 4;
		break;
	}
	
	if (ptr)
	{
#ifdef WINDOWS_PLATFORM
		dword overlapped = { 0 };
		LARGE_INTEGER lint;
		LARGE_INTEGER cur;
		lint.QuadPart = 0;
		SetFilePointerEx(handle, lint, &cur, FILE_BEGIN);
		WriteFile(handle, ptr, (dword)sz, &overlapped, NULL);
		lint.QuadPart = sz;
		SetFilePointerEx(handle, lint, &cur, FILE_BEGIN);
		SetEndOfFile(handle);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::lseek(handle, 0, SEEK_SET);
		::write(handle, ptr, sz);
		::lseek(handle, sz, SEEK_SET);
#endif
	}
	
}

file_impl::file_impl()
	: _hfile(0)
	, _hmap_size(0)
	, _hmap(0)
	, _path("")
	, _flags()
	, _size(0)
	, _cursor(0)
	, _map_counter(0)
{

}

file_impl::~file_impl()
{
	close();
}

ANG_IMPLEMENT_CLASSNAME(ang::core::files::file_impl);
ANG_IMPLEMENT_OBJECTNAME(ang::core::files::file_impl);

bool file_impl::is_inherited_of(type_name_t name)
{
	return name == type_of<file_impl>()
		|| object::is_inherited_of(name)
		|| name == type_of<ifile>();
}

bool file_impl::is_kind_of(type_name_t name)const
{
	return name == type_of<file_impl>()
		|| object::is_kind_of(name)
		|| name == type_of<ifile>();
}

bool file_impl::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_of<file_impl>())
	{
		*out = static_cast<file_impl*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_of<ifile>())
	{
		*out = static_cast<ifile*>(this);
		return true;
	}
	return false;
}

bool file_impl::create(path_view path, open_flags_t flags)
{
	_flags = open_flags::null;

#ifdef WINDOWS_PLATFORM

	DWORD dwDesiredAccess, dwCreationDisposition, dwShareMode = FILE_SHARE_READ;

	dwDesiredAccess = flags.is_active(open_flags::access_out) ? GENERIC_READ | GENERIC_WRITE
		: flags.is_active(open_flags::access_in) ? GENERIC_READ : 0;

	if (dwDesiredAccess == 0)
		return false;

	dwCreationDisposition = flags.is_active(open_flags::open_exist) ? OPEN_EXISTING
		: flags.is_active(open_flags::create_alway) ? CREATE_ALWAYS
		: flags.is_active(open_flags::open_alway) ? OPEN_ALWAYS
		: /*args->createMode == CreateMode::CreateNew ?*/ CREATE_NEW;

#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	_hfile = CreateFileW(
		path,
		dwDesiredAccess,
		dwShareMode,
		NULL,
		dwCreationDisposition,
		0,
		NULL
	);

#else
	_hfile = CreateFile2(
		path,
		dwDesiredAccess,
		dwShareMode,
		dwCreationDisposition,
		NULL
	);
#endif

	if (_hfile == INVALID_HANDLE_VALUE)
	{
		HRESULT err = GetLastError();
		_hfile = null;
		return false;
	}

#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM

	dword dwOpenFlags = 0;

	dwOpenFlags = flags.is_active(open_flags::access_out) ? O_RDWR
		: flags.is_active(open_flags::access_in) ? O_RDONLY : 0;

	dwOpenFlags |= flags.is_active(open_flags::open_exist) ? 0
		: flags.is_active(open_flags::create_alway) ? O_CREAT | O_TRUNC
		: flags.is_active(open_flags::open_alway) ? O_CREAT
		: /*args->createMode == CreateMode::CreateNew ?*/ O_CREAT | O_EXCL;

	_path = path;
	_hfile = open((path_view)_path
		, dwOpenFlags
		, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (_hfile < 0)
	{
		_hfile = -1;
		return false;
	}
#endif

	_size = get_file_size(_hfile);

	if (flags.is_active(open_flags::format_text)) //text file
	{
		if (_size > 0) 
		{
			switch (get_file_encoding(_hfile).get())
			{
			case text::encoding::utf8:
				_flags += open_flags::format_utf8;
				break;
			case text::encoding::utf16_le:
				_flags += open_flags::format_utf16_le;
				break;
			case text::encoding::utf16_be:
				_flags += open_flags::format_utf16_be;
				break;
			case text::encoding::utf32_le:
				_flags += open_flags::format_utf32_le;
				break;
			case text::encoding::utf32_be:
				_flags += open_flags::format_utf32_be;
				break;
			default:
				if ((flags.value() & 0X1F) != (uint)open_flags::format_text)
					_flags += (open_flags)(flags.value() & 0X1F);
				else
					_flags += open_flags::format_ascii;
				break;
			}
		}
		else //new file
		{
			uint f = (flags.value() & 0X1F);
			switch (get_format(flags))
			{
			case text::encoding::ascii:
				_flags += open_flags::format_ascii;
				_cursor = 0;
				break;
			case text::encoding::utf16_le:
				set_file_encoding(_hfile, text::encoding::utf16_le);
				_flags += open_flags::format_utf16_le;
				_cursor = 2;
				break;
			case text::encoding::utf16_be:
				set_file_encoding(_hfile, text::encoding::utf16_be);
				_flags += open_flags::format_utf16_be;
				_cursor = 2;
				break;
			case text::encoding::utf32_le:
				set_file_encoding(_hfile, text::encoding::utf32_le);
				_flags += open_flags::format_utf32_le;
				_cursor = 4;
				break;
			case text::encoding::utf32_be:
				set_file_encoding(_hfile, text::encoding::utf32_be);
				_flags += open_flags::format_utf32_be;
				_cursor = 4;
				break;
			default:// case text::encoding::utf8:
				set_file_encoding(_hfile, text::encoding::utf8);
				_flags += open_flags::format_utf8;
				_cursor = 3;
				break;
			}
			
		}
	}
	else if (flags.is_active(open_flags::format_packfile)) //text file
	{
		_flags += open_flags::format_packfile;
		_cursor = 0;
	}
	else //binary file
	{
		_flags += open_flags::format_binary;
		_cursor = 0;
	}

	_flags += (flags.is_active(open_flags::access_inout) ? open_flags::access_inout
		: flags.is_active(open_flags::access_in) ? open_flags::access_in
		: open_flags::access_out);

	return true;
}

file_handle_t file_impl::handle()const
{
	return _hfile;
}

file_handle_t file_impl::map_handle(ulong64 _min)
{

	if ((ulong64)_size < _min && _flags.is_active(file_flags::access_out))
	{
#ifdef WINDOWS_PLATFORM
		LARGE_INTEGER lint;
		lint.QuadPart = _min;
		SetFilePointerEx(_hfile, lint, null, FILE_BEGIN);
		SetEndOfFile(_hfile);
		lint.QuadPart = _cursor;
		SetFilePointerEx(_hfile, lint, null, FILE_BEGIN);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::lseek(_hfile, _min, 0);
		::lseek(_hfile, _cursor, 0);
#endif
		_size = get_file_size(_hfile);
	}

	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (!is_created())
		return 0;

	if (_hmap == 0)
	{
#ifdef WINAPI_FAMILY
		dword accessFlags = _flags.is_active(file_flags::access_out) ? PAGE_READWRITE : PAGE_READONLY;

#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
		LARGE_INTEGER lint;
		lint.QuadPart = _size;
		
		_hmap = CreateFileMappingW(_hfile, NULL, accessFlags, 0,0, NULL);
#else//STOREAPP
		_hmap = CreateFileMappingFromApp(_hfile, NULL, accessFlags, _size, NULL);
#endif//DESKTOP
		_hmap_size = _hmap ? _size : 0;
#else//LINUX|ANDROID
		_hmap = 0;//not used
#endif//PLATFORM		
	}
	else if (_min > _hmap_size)
	{
#ifdef WINAPI_FAMILY
		dword accessFlags = _flags.is_active(file_flags::access_out) ? PAGE_READWRITE : PAGE_READONLY;

#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
		LARGE_INTEGER lint;
		lint.QuadPart = _size;

		auto hmap = CreateFileMappingW(_hfile, NULL, accessFlags, 0, 0, NULL);
#else//STOREAPP
		auto hmap = CreateFileMappingFromApp(_hfile, NULL, accessFlags, _size, NULL);
#endif//DESKTOP
		if (hmap)
		{
			_hmap_size = _size;
			CloseHandle(_hmap);
			_hmap = hmap;
		}
#else//LINUX|ANDROID
		_hmap = 0;//not used
#endif//PLATFORM

	}
	return _hmap;
}

bool file_impl::is_created()const
{
	return _hfile != 0;
}

bool file_impl::close()
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
#ifdef WINAPI_FAMILY
	if (_hfile == null)
		return false;
	CloseHandle(_hfile);
	_hfile = null;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	if (_hfile < 0)
		return false;
	::close(_hfile);
	_hfile = -1;
#endif
	_path = null;
	_flags = open_flags::null;
	_size = 0;
	_cursor = 0;

	return true;
}


streams::stream_mode_t file_impl::mode()const
{
	return _flags.is_active(open_flags::access_inout) ? streams::stream_mode::inout
		: _flags.is_active(open_flags::access_out) ? streams::stream_mode::out
		: _flags.is_active(open_flags::access_in) ? streams::stream_mode::in
		: streams::stream_mode::unknow;
}

path file_impl::file_path()const
{
	return _path;
}

file_size_t file_impl::stream_size()const
{
	return _size;
}

bool file_impl::stream_size(file_size_t size)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (_size == size)
		return true;
	if (!_flags.is_active(open_flags::access_out))
		return false;
#ifdef WINDOWS_PLATFORM
	LARGE_INTEGER lint;
	_cursor = max(0, min<long64>(size, _cursor));
	lint.QuadPart = size;
	SetFilePointerEx(_hfile, lint, null, 0);
	auto res = SetEndOfFile(_hfile);
	lint.QuadPart = _cursor;
	SetFilePointerEx(_hfile, lint, null, 0);
	return res ? true : false;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	auto res = ::lseek(_hfile, size, SEEK_SET) == size;
	::lseek(_hfile, _cursor, SEEK_SET);
	return res;
#endif
}

text::encoding_t file_impl::format()const
{
	return get_format(_flags); // if it is binary must return text::encoding::unknown
}

void file_impl::format(text::encoding_t value)
{
	if (_flags.is_active(open_flags::format_text) && value.get() != format().get())
	{
		switch (value)
		{
		case text::encoding::ascii:
			_flags += open_flags::format_ascii;
			_cursor = 0;
			break;
		case text::encoding::utf16_le:
			set_file_encoding(_hfile, text::encoding::utf16_le);
			_flags += open_flags::format_utf16_le;
			_cursor = 2;
			break;
		case text::encoding::utf16_be:
			set_file_encoding(_hfile, text::encoding::utf16_be);
			_flags += open_flags::format_utf16_be;
			_cursor = 2;
			break;
		case text::encoding::utf32_le:
			set_file_encoding(_hfile, text::encoding::utf32_le);
			_flags += open_flags::format_utf32_le;
			_cursor = 4;
			break;
		case text::encoding::utf32_be:
			set_file_encoding(_hfile, text::encoding::utf32_be);
			_flags += open_flags::format_utf32_be;
			_cursor = 4;
			break;
		default:// case text::encoding::utf8:
			set_file_encoding(_hfile, text::encoding::utf8);
			_flags += open_flags::format_utf8;
			_cursor = 3;
			break;
		}
	}
}

bool file_impl::move_to(file_cursor_t cur, file_reference_t ref)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
#ifdef WINDOWS_PLATFORM
	LARGE_INTEGER lint;
	_cursor = cur;
	lint.QuadPart = cur;
	SetFilePointerEx(_hfile, lint, null, (uint)ref.get());
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	::lseek(_hfile, cur, (uint)ref.get());
#endif
	return true;
}

file_size_t file_impl::position()const
{
	return _cursor;
}

static file_cursor_t read_file(file_handle_t hfile, pointer buffer, wsize size, dword& readed)
{
#ifdef WINAPI_FAMILY
	if (!ReadFile(hfile, buffer, (dword)size, &readed, NULL)) {
		readed = 0;
		return 0;
	}

	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(hfile, lint, &cur, FILE_CURRENT);
	return cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	readed = ::read(hfile, buffer, size);
	return ::lseek(hfile, 0, SEEK_CUR);
#endif
}

wsize file_impl::read(pointer buffer, wsize size, text::text_format_t f)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (buffer == null || size == 0 || !is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_in))
		return 0;
	dword readed = 0;
	if (format() == text::encoding::binary)
	{
		auto cursor = read_file(_hfile, buffer, size, readed);
		_cursor = readed ? cursor : _cursor;
		return readed;
	}
	else
	{
		return 0;
	}
//
//#ifdef WINAPI_FAMILY
//	if (!ReadFile(_hfile, buffer, (dword)size, &readed, NULL))
//		return 0;
//	LARGE_INTEGER lint;
//	LARGE_INTEGER cur;
//	lint.QuadPart = 0;
//	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
//	_cursor = cur.QuadPart;
//
//#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
//	readed = ::read(_hfile, buffer, size);
//	_cursor = ::lseek(_hfile, 0, SEEK_CUR);
//#endif
//	return readed;
}

wsize file_impl::read(pointer buffer, wsize size, text::encoding_t encoding)
{
	raw_str_t dest = { buffer , size, encoding };

	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (buffer == null || size == 0 || !is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_in))
		return 0;
	dword readed = 0;

	if (format() == text::encoding::binary)
	{
		auto cursor = read_file(_hfile, buffer, size, readed);
		_cursor = readed ? cursor : _cursor;
		return readed;
	}
	else
	{
		text::encoder_interface encoder;
		text::encoder_interface::initialize_interface(&encoder, encoding);

		alignas(16) stack_array<byte, 128> buffer;
		raw_str_t src = { buffer.begin(), 124, format() };
		wsize t = 0, c = 0, idx = 0, count = dest.count() - 1, cs = dest.char_size(), cs2 = src.char_size();

		while (idx <= count)
		{
#ifdef WINAPI_FAMILY
			if (!ReadFile(_hfile, src.ptr(), (dword)src.size(), &readed, NULL))
				break;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
			readed = ::read(_hfile, src.ptr(), src.size());
#endif
			idx += encoder._convert_string((byte*)dest.ptr() + (idx * cs), (dest.size() / cs) - idx - 1, src.ptr(), c, src.encoding(), true);

			c = min(c * cs2, readed);
			t += c;
#ifdef WINAPI_FAMILY
		
			LARGE_INTEGER lint;
			LARGE_INTEGER cur;
			lint.QuadPart = _cursor + c;
			SetFilePointerEx(_hfile, lint, &cur, FILE_BEGIN);
			_cursor = cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
			_cursor = ::lseek(_hfile, _cursor + c, SEEK_SET);
#endif
		}
		return t;
	}
}

wsize file_impl::read(ibuffer_view_t buffer, text::encoding_t encoding)
{
	if (buffer.is_empty())
		return 0;
	raw_str_t dest = { buffer->buffer_ptr() , buffer->buffer_size(), encoding };

	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;

	if (!_flags.is_active(open_flags::access_in))
		return 0;
	dword readed = 0;

	if (format() == text::encoding::binary)
	{
		auto cursor = read_file(_hfile, buffer->buffer_ptr(), buffer->buffer_size(), readed);
		_cursor = readed ? cursor : _cursor;
		return readed;
	}
	else
	{
		text::encoder_interface encoder;
		text::encoder_interface::initialize_interface(&encoder, encoding);

		alignas(16) stack_array<byte, 128> buffer;
		raw_str_t src = { buffer.begin(), 124, format() };
		wsize t = 0, c = 0, idx = 0, count = dest.count() - 1, cs = dest.char_size(), cs2 = src.char_size();

		while (idx <= count)
		{
#ifdef WINAPI_FAMILY
			if (!ReadFile(_hfile, src.ptr(), (dword)src.size(), &readed, NULL))
				break;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
			readed = ::read(_hfile, src.ptr(), src.size());
#endif
			idx += encoder._convert_string((byte*)dest.ptr() + (idx * cs), (dest.size() / cs) - idx - 1, src.ptr(), c, src.encoding(), true);

			c = min(c * cs2, readed);
			t += c;
#ifdef WINAPI_FAMILY

			LARGE_INTEGER lint;
			LARGE_INTEGER cur;
			lint.QuadPart = _cursor + c;
			SetFilePointerEx(_hfile, lint, &cur, FILE_BEGIN);
			_cursor = cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
			_cursor = ::lseek(_hfile, _cursor + c, SEEK_SET);
#endif
		}
		return t;
	}
}


static file_cursor_t write_file(file_handle_t hfile,  pointer buffer, wsize size, dword& written)
{
#ifdef WINAPI_FAMILY
	if (!WriteFile(hfile, buffer, (dword)size, &written, NULL)) {
		written = 0;
		return 0;
	}
		
	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(hfile, lint, &cur, FILE_CURRENT);
	return cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	written = ::write(hfile, buffer, size);
	return ::lseek(hfile, 0, SEEK_CUR);
#endif
}

wsize file_impl::write(pointer buffer, wsize size, text::text_format_t format)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (buffer == null || size == 0 || !is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_out))
		return 0;

	dword written = 0;
	file_cursor_t cursor = 0;
	if (this->format() == text::encoding::binary)
	{
		cursor = write_file(_hfile, buffer, size, written);
		_cursor = written ? cursor : _cursor;
		return written;
	}
	else
	{
		text::encoder_interface encoder;
		text::encoder_interface::initialize_interface(&encoder, this->format());

		wsize i = 0, cs = raw_str(0, 0, encoder._format()).char_size();
		char32_t _char[2] = { 0 };

		cstr_t value;
		alignas(16) stack_array<char, 128> _buff;
		raw_str_t dest = { _buff.begin(), 124, this->format() };

		switch (format.format_target())
		{
		case text::text_format::character:
			switch (size) {
			case 1: _char[0] = text::ASCII().to_utf32((char const*)buffer, i); break;//ascii - utf8
			case 2: _char[0] = text::UTF16().to_utf32((char16_t const*)buffer, i); break; //utf16
			case 4: _char[0] = text::UTF32().to_utf32((char32_t const*)buffer, i); break; //utf32
			default: return 0;
			}

			i = encoder._convert_string(dest.ptr(), dest.count(), _char, i, text::encoding::utf32, false) * cs;
			cursor = write_file(_hfile, dest.ptr(), i, written);
			_cursor = written ? cursor : _cursor;

			break;

		case text::text_format::signed_integer:
			switch (size) {
			case 1: dest = encoder._integer_to_string(*(char*)buffer, dest.ptr(), dest.size() / cs, format); break;
			case 2: dest = encoder._integer_to_string(*(short*)buffer, dest.ptr(), dest.size() / cs, format); break;
			case 4: dest = encoder._integer_to_string(*(int*)buffer, dest.ptr(), dest.size() / cs, format); break;
			case 8: dest = encoder._integer_to_string(*(long64*)buffer, dest.ptr(), dest.size() / cs, format); break;
			}
			cursor = write_file(_hfile, dest.ptr(), dest.size(), written);
			_cursor = written ? cursor : _cursor;
			break;

		case text::text_format::usigned_integer:
			switch (size) {
			case 1: dest = encoder._uinteger_to_string(*(uchar*)buffer, dest.ptr(), dest.size() / cs, format); break;
			case 2: dest = encoder._uinteger_to_string(*(ushort*)buffer, dest.ptr(), dest.size() / cs, format); break;
			case 4: dest = encoder._uinteger_to_string(*(uint*)buffer, dest.ptr(), dest.size() / cs, format); break;
			case 8: dest = encoder._uinteger_to_string(*(ulong64*)buffer, dest.ptr(), dest.size() / cs, format); break;
			}
			cursor = write_file(_hfile, dest.ptr(), dest.size(), written);
			_cursor = written ? cursor : _cursor;
			break;

		case text::text_format::floating:
			switch (size) {
			case 4: dest = encoder._floating_to_string(*(float*)buffer, dest.ptr(), dest.size() / cs, format); break;
			case 8: dest = encoder._floating_to_string(*(double*)buffer, dest.ptr(), dest.size() / cs, format); break;
			}
			cursor = write_file(_hfile, dest.ptr(), dest.size(), written);
			_cursor = written ? cursor : _cursor;
			break;
		}

		return i;
	}
}

wsize file_impl::write(pointer buffer, wsize size, text::encoding_t encoding)
{
	raw_str_t cstr = { buffer , size, encoding };

	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (buffer == null || size == 0 || !is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_out))
		return 0;
	dword written = 0;

	if (format() == text::encoding::binary)
	{
		auto cursor = write_file(_hfile, buffer, size, written);
		_cursor = written ? cursor : _cursor;
		return written;
	}
	else
	{
		text::encoder_interface encoder;
		text::encoder_interface::initialize_interface(&encoder, format());

		alignas(16) stack_array<byte, 128> buffer;
		raw_str_t dest = { buffer.begin(), 124, format() };
		wsize t = 0, i = 0, c = 0, idx = 0, count = cstr.count(), cs = cstr.char_size(), cs2 = dest.char_size();

		while (idx <= count)
		{
			i = encoder._convert_string(dest.ptr(), dest.count() - 1, (byte*)cstr.ptr() + idx*cs, c, cstr.encoding(), true);
			write(dest.ptr(), i * cs2, text::text_format());
#ifdef WINAPI_FAMILY
			if (!WriteFile(_hfile, dest.ptr(), (dword)(i * cs2), &written, NULL))
				return t * cs2;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
			written = ::write(_hfile, dest.ptr(), i * cs2);
#endif
			idx += c; c = 0; t += i;
		}

#ifdef WINAPI_FAMILY

		LARGE_INTEGER lint;
		LARGE_INTEGER cur;
		lint.QuadPart = 0;
		SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
		_cursor = cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		_cursor = ::lseek(_hfile, 0, SEEK_CUR);
#endif

		return t * cs2;
	}
}

wsize file_impl::write(ibuffer_view_t buffer, text::encoding_t encoding)
{
	if (buffer.is_empty())
		return 0;

	raw_str_t cstr = { buffer->buffer_ptr() , buffer->buffer_size(), encoding };

	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (!is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_out))
		return 0;
	dword written = 0;

	if (format() == text::encoding::binary)
	{
#ifdef WINAPI_FAMILY
		if (!WriteFile(_hfile, cstr.ptr(), (dword)cstr.size(), &written, NULL))
			return 0;
		LARGE_INTEGER lint;
		LARGE_INTEGER cur;
		lint.QuadPart = 0;
		SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
		_cursor = cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		written = ::write(_hfile, cstr.ptr(), cstr.size());
		_cursor = ::lseek(_hfile, 0, SEEK_CUR);
#endif
		return written;
	}
	else
	{
		text::encoder_interface encoder;
		text::encoder_interface::initialize_interface(&encoder, format());

		alignas(16) stack_array<byte, 128> buffer;
		raw_str_t dest = { buffer.begin(), 124, format() };
		wsize t = 0, i = 0, c = 0, idx = 0, count = cstr.count(), cs = cstr.char_size(), cs2 = dest.char_size();

		while (idx <= count)
		{
			i = encoder._convert_string(dest.ptr(), dest.count() - 1, (byte*)cstr.ptr() + idx*cs, c, cstr.encoding(), true);
			write(dest.ptr(), i * cs2, text::text_format());
#ifdef WINAPI_FAMILY
			if (!WriteFile(_hfile, dest.ptr(), dword(i * cs2), &written, NULL))
				return t * cs2;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
			written = ::write(_hfile, dest.ptr(), dest.size());
#endif
			idx += c; c = 0; t += i;
		}

#ifdef WINAPI_FAMILY

		LARGE_INTEGER lint;
		LARGE_INTEGER cur;
		lint.QuadPart = 0;
		SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
		_cursor = cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		_cursor = ::lseek(_hfile, 0, SEEK_CUR);
#endif

		return t * cs2;
	}
}

ibuffer_t file_impl::map(wsize size, file_cursor_t offset)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;

	if (map_handle(size + offset) == 0)
		return null;
	_map_counter++;
	return new mapped_file_buffer(this, _flags, size, offset);
}

bool file_impl::unmap(ibuffer_t buffer, wsize size)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	mapped_file_buffer_t mapped_buffer = interface_cast<mapped_file_buffer>(buffer.get());
	if (mapped_buffer == null || mapped_buffer->original_source().get() != this)
		return false;
	if (mapped_buffer->unmap())
	{
		_map_counter--;
		if (_map_counter <= 0)
		{
#ifdef WINDOWS_PLATFORM
			CloseHandle(_hmap);
			_hmap = NULL;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
			_hmap = NULL;
#endif
		}
	}
	return true;
}

bool file_impl::unmap(mapped_file_buffer* mapped_buffer)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (mapped_buffer == null || mapped_buffer->original_source().get() != this)
		return false;
	if (mapped_buffer->unmap())
	{
		_map_counter--;
		if (_map_counter <= 0)
		{
#ifdef WINDOWS_PLATFORM
			CloseHandle(_hmap);
			_hmap = NULL;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
			_hmap = NULL;
#endif
		}
	}
	return true;
}

bool file_impl::set_mutex(core::async::mutex_ptr_t mutex)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	_mutex = mutex;
	return true;
}


////////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<file_impl>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<file_impl>::object_wrapper(file_impl* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<file_impl>::object_wrapper(object_wrapper && other) : _ptr(null) {
	file_impl * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<file_impl>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<file_impl>::object_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

ang::object_wrapper<file_impl>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<file_impl>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<file_impl>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<file_impl>::is_empty()const
{
	return _ptr == null;
}

file_impl* ang::object_wrapper<file_impl>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<file_impl>::set(file_impl* ptr)
{
	file_impl * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<file_impl>& ang::object_wrapper<file_impl>::operator = (file_impl* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<file_impl>& ang::object_wrapper<file_impl>::operator = (ang::object_wrapper<file_impl> && other)
{
	if (this == (object_wrapper<file_impl>*)&other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<file_impl>& ang::object_wrapper<file_impl>::operator = (ang::object_wrapper<file_impl> const& other)
{
	set(other._ptr);
	return*this;
}

file_impl ** ang::object_wrapper<file_impl>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<file_impl> ang::object_wrapper<file_impl>::operator& (void)
{
	return this;
}

file_impl * ang::object_wrapper<file_impl>::operator -> (void)
{
	return get();
}

file_impl const* ang::object_wrapper<file_impl>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<file_impl>::operator file_impl * (void)
{
	return get();
}

ang::object_wrapper<file_impl>::operator file_impl const* (void)const
{
	return get();
}