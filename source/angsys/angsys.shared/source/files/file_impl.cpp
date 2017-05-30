/*********************************************************************************************************************/
/*   File Name: file_impl.cpp                                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <ang/core/files.h>
#include "file_system.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::files;


////////////////////////////////////////////////////////////////////////

mapped_file_buffer::mapped_file_buffer(system_file_t file, file_flags_t access, uint size, ulong64 offset)
	: _original_source(null)
	, _handle(null)
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

bool mapped_file_buffer::is_child_of(type_name_t name)
{
	return name == type_name<file_impl>()
		|| object::is_child_of(name)
		|| name == type_name<ibuffer>();
}

bool mapped_file_buffer::is_kind_of(type_name_t name)const
{
	return name == type_name<file_impl>()
		|| object::is_kind_of(name)
		|| name == type_name<ibuffer>();
}

bool mapped_file_buffer::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<mapped_file_buffer>())
	{
		*out = static_cast<mapped_file_buffer*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<ibuffer>())
	{
		*out = static_cast<ibuffer*>(this);
		return true;
	}
	return false;
}

bool mapped_file_buffer::map(file_flags_t access, uint size, ulong64 offset)
{
	_access_flag = access.is_active(file_flags::access_out) ? file_flags::access_out : file_flags::access_in;
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

wsize mapped_file_buffer::mem_copy(wsize size, pointer data)
{
	if (!_access_flag.is_active(file_flags::access_out))
		return 0;
	auto max_size = min(size, _buffer_size);
	memcpy(_buffer_ptr, data, max_size);
	return max_size;
}

pointer mapped_file_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) >= _buffer_size)
		return null;
	return (byte_t*)_buffer_ptr + start;
}

void mapped_file_buffer::unmap_buffer(pointer, wsize)
{
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
	while (new_size < size)
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

dword file_impl::add_ref()
{
	return object::add_ref();
}
dword file_impl::release()
{
	return object::release();
}

file_size_t file_impl::get_file_size(file_handle_t h)
{
	ulong64 size;
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
	mbyte bom;
	dword out;
#ifdef WINDOWS_PLATFORM
	LARGE_INTEGER lint;
	lint.QuadPart = 0;
	SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
	ReadFile(handle, bom.bytes, 3, &out, NULL);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	::read(handle, bom.bytes, 3);
#endif

	if ((bom.value & 0X0000FFFF) == mbyte::ubom)
	{
#ifdef WINDOWS_PLATFORM
		lint.QuadPart = 2;
		SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::lseek(handle, 2, SEEK_SET);
#endif
	//	file->cursor = 2;
	//	flag += open_flags::EncodingUnicode;
		return text::encoding::iso_10646; //unicode
	}
	else if ((bom.value & 0X00FFFFFF) == mbyte::inv_mbom)
	{
#ifdef WINDOWS_PLATFORM
		lint.QuadPart = 3;
		SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::lseek(handle, 3, SEEK_SET);
#endif
		//file->cursor = 3;
		return text::encoding::utf_8; //mbyte
	}
	else
	{
#ifdef WINDOWS_PLATFORM
		lint.QuadPart = 0;
		SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::lseek(handle, 0, SEEK_SET);
#endif
		return text::encoding::iso_8859_1;
	}
}

void file_impl::set_file_encoding(file_handle_t handle, text::encoding_t encoding)
{
	if (text::encoding::iso_10646 == encoding)
	{
		uint bom = mbyte::ubom;
		dword overlapped = { 0 };
#ifdef WINDOWS_PLATFORM
		LARGE_INTEGER lint;
		LARGE_INTEGER cur;
		lint.QuadPart = 2;
		WriteFile(handle, &bom, 2, &overlapped, NULL);
		SetFilePointerEx(handle, lint, &cur, FILE_BEGIN);
		SetEndOfFile(handle);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::write(handle, &bom, 2);
		::lseek(handle, 2, SEEK_SET);
#endif
	}
	else if (text::encoding::utf_8 == encoding)
	{
		uint bom = mbyte::inv_mbom;
		dword overlapped = { 0 };
#ifdef WINDOWS_PLATFORM
		LARGE_INTEGER lint;
		LARGE_INTEGER cur;
		lint.QuadPart = 3;
		WriteFile(handle, &bom, 3, &overlapped, NULL);
		SetFilePointerEx(handle, lint, &cur, FILE_BEGIN);
		SetEndOfFile(handle);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::write(handle, &bom, 3);
		::lseek(handle, 3, SEEK_SET);
#endif
	}
	else //Ascii
	{
#ifdef WINDOWS_PLATFORM
		LARGE_INTEGER lint;
		lint.QuadPart = 0;
		SetFilePointerEx(handle, lint, 0, FILE_BEGIN);
		SetEndOfFile(handle);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::lseek(handle, 0, SEEK_SET);
#endif
	}
}

file_impl::file_impl()
	: _hfile(0)
	, _hmap(0)
	, _hmap_size(0)
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

bool file_impl::is_child_of(type_name_t name)
{
	return name == type_name<file_impl>()
		|| object::is_child_of(name)
		|| name == type_name<ifile>();
}

bool file_impl::is_kind_of(type_name_t name)const
{
	return name == type_name<file_impl>()
		|| object::is_kind_of(name)
		|| name == type_name<ifile>();
}

bool file_impl::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<file_impl>())
	{
		*out = static_cast<file_impl*>(this);
		return true;
	}
	else if (ang::object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<ifile>())
	{
		*out = static_cast<ifile*>(this);
		return true;
	}
	return false;
}

bool file_impl::create(cwstr_t path, open_flags_t flags)
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

	string _path = path;
	_hfile = open((cstr_t)_path
		, dwOpenFlags
		, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (_hfile < 0)
	{
		_hfile = -1;
		return false;
	}
#endif

	_path = path;
	_size = get_file_size(_hfile);

	if (flags.is_active(open_flags::type_text)) //text file
	{
		_flags += open_flags::type_text;
		if (_size > 0) 
		{
			switch (get_file_encoding(_hfile).get())
			{
			case text::encoding::iso_10646:
				_flags += open_flags::encoding_unicode;
				break;
			case text::encoding::utf_8:
				_flags += open_flags::encoding_mbyte;
				break;
			case text::encoding::iso_8859_1:
				_flags += open_flags::encoding_ascii;
				break;
			}
		}
		else //new file
		{
			if (flags.is_active(open_flags::encoding_unicode))
			{
				set_file_encoding(_hfile, text::encoding::iso_10646);
				_flags += open_flags::encoding_unicode;
				_cursor = 2;
			}
			else if (flags.is_active(open_flags::encoding_mbyte))
			{
				set_file_encoding(_hfile, text::encoding::utf_8);
				_flags += open_flags::encoding_mbyte;
				_cursor = 3;
			}
			else //if (flags.is_active(open_flags::encoding_ascii))
			{
				set_file_encoding(_hfile, text::encoding::iso_8859_1);
				_flags += open_flags::encoding_ascii;
				_cursor = 0;
			}
		}
	}
	else //binary file
	{
		_flags += open_flags::type_binary;
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

	core::async::scope_locker lock = _mutex;
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
#else//LINUX|ANDROID
		_hmap = 0;//not used
#endif//PLATFORM
		_hmap_size = _hmap ? _size : 0;
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
	core::async::scope_locker lock = _mutex;
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

wstring file_impl::file_path()const
{
	return _path;
}

file_size_t file_impl::file_size()const
{
	return _size;
}

bool file_impl::file_size(file_size_t size)
{
	core::async::scope_locker lock = _mutex;
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
	::lseek(_hfile, cur, 0);
#endif
}

text::encoding_t file_impl::encoding()const
{
	return _flags.is_active(open_flags::encoding_unicode) ? text::encoding::iso_10646
		: _flags.is_active(open_flags::encoding_mbyte) ? text::encoding::utf_8
		: _flags.is_active(open_flags::encoding_ascii) ? text::encoding::ascii
		: text::encoding::unknown; // if it is binary must return text::encoding::unknown
}

void file_impl::encoding(text::encoding_t value)
{
	if (_flags.is_active(open_flags::type_text) && value.get() != encoding().get())
		set_file_encoding(_hfile, value);
}

void file_impl::cursor(file_reference_t ref, file_cursor_t cur)
{
	core::async::scope_locker lock = _mutex;
#ifdef WINDOWS_PLATFORM
	LARGE_INTEGER lint;
	_cursor = cur;
	lint.QuadPart = cur;
	SetFilePointerEx(_hfile, lint, null, (uint)ref.get());
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	::lseek(_hfile, cur, (uint)ref.get());
#endif
}

file_size_t file_impl::cursor()const
{
	return _cursor;
}

wsize file_impl::read(wsize size, pointer buffer)
{
	core::async::scope_locker lock = _mutex;
	if (buffer == null || size == 0 || !is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_in))
		return 0;
	dword readed = 0;
#ifdef WINAPI_FAMILY
	if (!ReadFile(_hfile, buffer, (dword)size, &readed, NULL))
		return 0;
	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
	_cursor = cur.QuadPart;

#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	readed = ::read(_hfile, buffer, size);
	_cursor = ::lseek(_hfile, 0, SEEK_CUR);
#endif
	return readed;
}

wsize file_impl::write(wsize size, pointer buffer)
{
	core::async::scope_locker lock = _mutex;
	if (buffer == null || size == 0 || !is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_out))
		return 0;
	dword written = 0;
#ifdef WINAPI_FAMILY
	if (!WriteFile(_hfile, buffer, size, &written, NULL))
		return 0;
	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
	_cursor = cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	written = ::read(_hfile, buffer, size);
	_cursor = ::lseek(_hfile, 0, SEEK_CUR);
#endif
	return written;
}

wsize file_impl::read(ibuffer_t buffer)
{
	core::async::scope_locker lock = _mutex;
	if (buffer == null || !is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_in))
		return 0;
	dword readed = 0;
#ifdef WINAPI_FAMILY
	if (!ReadFile(_hfile, buffer->buffer_ptr(), buffer->buffer_size(), &readed, NULL))
		return 0;
	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
	_cursor = cur.QuadPart;

#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	readed = ::read(_hfile, buffer->buffer_ptr(), buffer->buffer_size());
	_cursor = ::lseek(_hfile, 0, SEEK_CUR);
#endif
	return readed;
}

wsize file_impl::write(ibuffer_t buffer)
{
	core::async::scope_locker lock = _mutex;
	if (buffer == null || !is_created())
		return 0;
	if (!_flags.is_active(open_flags::access_in))
		return 0;
	dword written = 0;
#ifdef WINAPI_FAMILY
	if (!WriteFile(_hfile, buffer->buffer_ptr(), buffer->buffer_size(), &written, NULL))
		return 0;
	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
	_cursor = cur.QuadPart;

#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	written = ::read(_hfile, buffer->buffer_ptr(), buffer->buffer_size());
	_cursor = ::lseek(_hfile, 0, SEEK_CUR);
#endif
	return written;
}

ibuffer_t file_impl::map(wsize size, file_cursor_t offset)
{
	core::async::scope_locker lock = _mutex;

	if (map_handle(size + offset) == 0)
		return null;
	_map_counter++;
	return new mapped_file_buffer(this, _flags, size, offset);
}

bool file_impl::unmap(ibuffer_t buffer)
{
	core::async::scope_locker lock = _mutex;
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
	core::async::scope_locker lock = _mutex;
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

bool file_impl::set_mutex(core::async::mutex_t mutex)
{
	core::async::scope_locker lock = _mutex;
	_mutex = mutex;
	return true;
}
