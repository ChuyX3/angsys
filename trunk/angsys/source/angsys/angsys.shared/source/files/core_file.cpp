/*********************************************************************************************************************/
/*   File Name: core_file.cpp                                                                                        */
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


////////////////////////////////////////////////////////////////////////

mapped_file_buffer::mapped_file_buffer(system_file_t file, open_flags_t access, wsize size, ulong64 offset)
	: _original_source(null)
	, _buffer_ptr(null)
	, _buffer_size(0)
	, _buffer_offset(0)
	, _access_flag(open_flags::null)
{
	_original_source = file;
	map(access, size, offset);
}

mapped_file_buffer::~mapped_file_buffer()
{
	_original_source->unmap(this);
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::files::mapped_file_buffer);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::files::mapped_file_buffer, object, ibuffer);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::files::mapped_file_buffer, object, ibuffer, ibuffer_view);


bool mapped_file_buffer::map(open_flags_t access, wsize size, ulong64 offset)
{
	_access_flag = bool(access & open_flags::access_out) ? open_flags::access_out : open_flags::access_in;
	_access_flag += access.get() & (open_flags)0X1F;
#ifdef WINDOWS_PLATFORM
	dword accessFlags = bool(access & open_flags::access_out) ? PAGE_READWRITE : PAGE_READONLY;
	pointer mapp_handle = _original_source->map_handle(size + offset);

#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
	LARGE_INTEGER lint;
	lint.QuadPart = offset;
	_buffer_ptr = MapViewOfFile(mapp_handle, bool(access & open_flags::access_out) ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ, lint.HighPart, lint.LowPart, size);
#else//STOREAPP
	_buffer_ptr = MapViewOfFileFromApp(mapp_handle, bool(access & open_flags::access_out) ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ, offset, size);
#endif//DESKTOP

	if (_buffer_ptr)
	{	
		_buffer_size = size;
		_buffer_offset = offset;
	}

#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	int prot = bool(access & open_flags::access_out) ? PROT_WRITE | PROT_READ : PROT_READ;
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

bool mapped_file_buffer::is_readonly()const { return false; }

pointer mapped_file_buffer::buffer_ptr()
{
	return _buffer_ptr;
}

const_pointer mapped_file_buffer::buffer_ptr()const
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
	if (!(_access_flag & open_flags::access_out))
		return 0;
	if (bool(_access_flag & open_flags::format_text))
	{
		if (encoding == text::encoding::binary)
			return 0; 

		raw_str_t dest(_buffer_ptr, _buffer_size, this->encoding());
		raw_str_t src(data, size, encoding);
		windex written = 0;

		text::iencoder_t encoder = text::iencoder::get_encoder(dest.encoding());
		return encoder->convert(dest.ptr(), src.ptr(), src.encoding(), true, dest.count(), src.count()).count();
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

file_size_t core_file::get_file_size(file_handle_t handle)
{
	ulong64 size = 0;
	if (handle == invalid_handle)
		return 0;
#ifdef WINDOWS_PLATFORM

	LARGE_INTEGER li;
	size = GetFileSizeEx(handle, &li) == FALSE ? 0ll : li.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	size = lseek(h, 0, SEEK_END);
	lseek(h, 0, SEEK_SET);
#endif
	return size;
}

text::encoding_t core_file::get_file_encoding(file_handle_t handle)
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

	if (text::load_bom<text::encoding::utf8>(bom) > 0)
	{
		offset = 3;
		result = text::encoding::utf8;
	}
	else if (text::load_bom<text::encoding::utf32_be>(bom) > 0)
	{
		offset = 4;
		result = text::encoding::utf32_be;
	}
	else if (text::load_bom<text::encoding::utf32_le>(bom) > 0)
	{
		offset = 4;
		result = text::encoding::utf32_le;
	}
	else if (text::load_bom<text::encoding::utf16_be>(bom) > 0)
	{
		offset = 2;
		result = text::encoding::utf16_be;
	}
	else if (text::load_bom<text::encoding::utf16_le>(bom) > 0)
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

void core_file::set_file_encoding(file_handle_t handle, text::encoding_t encoding)
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

core_file::core_file()
	: _hfile(invalid_handle)
	, _hmap_size(0)
	, _hmap(0)
	, _path("")
	, _flags()
	//, _size(0)
	//, _cursor(0)
	, _map_counter(0)
{

}

core_file::~core_file()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::files::core_file);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::files::core_file, object, ifile);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::files::core_file, object, ifile);

bool core_file::create(path_view_t path, open_flags_t flags)
{
	_flags = open_flags::null;

#ifdef WINDOWS_PLATFORM

	DWORD dwDesiredAccess, dwCreationDisposition, dwShareMode = FILE_SHARE_READ;

	dwDesiredAccess = bool(flags & open_flags::access_out) ? GENERIC_READ | GENERIC_WRITE
		: bool(flags & open_flags::access_in) ? GENERIC_READ : 0;

	if (dwDesiredAccess == 0)
		return false;

	dwCreationDisposition = bool(flags & open_flags::open_exist) ? OPEN_EXISTING
		: bool(flags & open_flags::create_alway) ? CREATE_ALWAYS
		: bool(flags & open_flags::open_alway) ? OPEN_ALWAYS
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

	dwOpenFlags = bool(flags & open_flags::access_out) ? O_RDWR
		: bool(flags & open_flags::access_in) ? O_RDONLY : 0;

	dwOpenFlags |= bool(flags & open_flags::open_exist) ? 0
		: bool(flags & open_flags::create_alway) ? O_CREAT | O_TRUNC
		: bool(flags & open_flags::open_alway) ? O_CREAT
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

	wsize size = (wsize)get_file_size(_hfile);

	if (bool(flags & open_flags::format_text)) //text file
	{
		if (size > 0) 
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
				if ((flags.get() & (open_flags)0X1F) != open_flags::format_text)
					_flags += flags.get() & (open_flags)0X1F;
				else
					_flags += open_flags::format_ascii;
				break;
			}
		}
		else //new file
		{
			switch (get_format(flags))
			{
			case text::encoding::ascii:
				_flags += open_flags::format_ascii;
				break;
			case text::encoding::utf16_le:
				set_file_encoding(_hfile, text::encoding::utf16_le);
				_flags += open_flags::format_utf16_le;
				break;
			case text::encoding::utf16_be:
				set_file_encoding(_hfile, text::encoding::utf16_be);
				_flags += open_flags::format_utf16_be;
				break;
			case text::encoding::utf32_le:
				set_file_encoding(_hfile, text::encoding::utf32_le);
				_flags += open_flags::format_utf32_le;
				break;
			case text::encoding::utf32_be:
				set_file_encoding(_hfile, text::encoding::utf32_be);
				_flags += open_flags::format_utf32_be;
				break;
			default:// case text::encoding::utf8:
				set_file_encoding(_hfile, text::encoding::utf8);
				_flags += open_flags::format_utf8;
				break;
			}
		}
	}
	else if (bool(flags & open_flags::format_packfile)) //text file
	{
		_flags += open_flags::format_packfile;
	}
	else //binary file
	{
		_flags += open_flags::format_binary;
	}

	_flags += (bool(flags & open_flags::access_inout) ? open_flags::access_inout
		: bool(flags & open_flags::access_in) ? open_flags::access_in
		: open_flags::access_out);

	return true;
}

file_handle_t core_file::handle()const
{
	return _hfile;
}

bool core_file::is_eof()const {
	return cursor() == size();
}

file_handle_t core_file::map_handle(ulong64 _min)
{

	if ((ulong64)size() < _min && bool(_flags & open_flags::access_out))
	{
#ifdef WINDOWS_PLATFORM
		LARGE_INTEGER lint;
		lint.QuadPart = _min;
		SetFilePointerEx(_hfile, lint, null, FILE_BEGIN);
		SetEndOfFile(_hfile);
		lint.QuadPart = cursor();
		SetFilePointerEx(_hfile, lint, null, FILE_BEGIN);
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
		::lseek(_hfile, _min, 0);
		::lseek(_hfile, _cursor, 0);
#endif
	}

	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (!is_created())
		return 0;

	if (_hmap == 0)
	{
#ifdef WINAPI_FAMILY
		dword accessFlags = bool(_flags & open_flags::access_out) ? PAGE_READWRITE : PAGE_READONLY;

#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
		LARGE_INTEGER lint;
		lint.QuadPart = size();
		
		_hmap = CreateFileMappingW(_hfile, NULL, accessFlags, 0,0, NULL);
#else//STOREAPP
		_hmap = CreateFileMappingFromApp(_hfile, NULL, accessFlags, 0, NULL);
#endif//DESKTOP
		_hmap_size = _hmap ? size() : 0;
#else//LINUX|ANDROID
		_hmap = 0;//not used
#endif//PLATFORM		
	}
	else if (_min > _hmap_size)
	{
#ifdef WINAPI_FAMILY
		dword accessFlags = bool(_flags & open_flags::access_out) ? PAGE_READWRITE : PAGE_READONLY;

#if WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
		LARGE_INTEGER lint;
		lint.QuadPart = size();

		auto hmap = CreateFileMappingW(_hfile, NULL, accessFlags, 0, 0, NULL);
#else//STOREAPP
		auto hmap = CreateFileMappingFromApp(_hfile, NULL, accessFlags, 0, NULL);
#endif//DESKTOP
		if (hmap)
		{
			_hmap_size = size();
			CloseHandle(_hmap);
			_hmap = hmap;
		}
#else//LINUX|ANDROID
		_hmap = 0;//not used
#endif//PLATFORM

	}
	return _hmap;
}

bool core_file::is_created()const
{
	return _hfile != 0;
}

void core_file::clear()
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
#ifdef WINAPI_FAMILY
	if (_hfile == invalid_handle)
		return;
	CloseHandle(_hfile);
	_hfile = null;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	if (_hfile < 0)
		return;
	::close(_hfile);
	_hfile = -1;
#endif
	_path = null;
	_flags = open_flags::null;
	
	return;
}


streams::stream_mode_t core_file::mode()const
{
	return bool(_flags & open_flags::access_inout) ? streams::stream_mode::inout
		: bool(_flags & open_flags::access_out) ? streams::stream_mode::out
		: bool(_flags & open_flags::access_in) ? streams::stream_mode::in
		: streams::stream_mode::unknow;
}

path_t core_file::path()const
{
	return _path;
}

file_size_t core_file::size()const
{
	return get_file_size(_hfile);
}

bool core_file::size(file_size_t sz)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (size() == sz)
		return true;
	if (!(_flags & open_flags::access_out))
		return false;
#ifdef WINDOWS_PLATFORM
	LARGE_INTEGER lint;
	file_offset_t cur = max(0, min<long64>(sz, cursor()));
	lint.QuadPart = sz;
	SetFilePointerEx(_hfile, lint, null, 0);
	auto res = SetEndOfFile(_hfile);
	lint.QuadPart = cur;
	SetFilePointerEx(_hfile, lint, null, 0);
	return res ? true : false;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	auto res = ::lseek(_hfile, sz, SEEK_SET) == sz;
	::lseek(_hfile, cur, SEEK_SET);
	return res;
#endif
}

text::encoding_t core_file::format()const
{
	return get_format(_flags); // if it is binary must return text::encoding::unknown
}

void core_file::format(text::encoding_t value)
{
	if (bool(_flags & open_flags::format_text) && value.get() != format().get())
	{
		switch (value)
		{
		case text::encoding::ascii:
			_flags += open_flags::format_ascii;
			break;
		case text::encoding::utf16_le:
			set_file_encoding(_hfile, text::encoding::utf16_le);
			_flags += open_flags::format_utf16_le;
			break;
		case text::encoding::utf16_be:
			set_file_encoding(_hfile, text::encoding::utf16_be);
			_flags += open_flags::format_utf16_be;
			break;
		case text::encoding::utf32_le:
			set_file_encoding(_hfile, text::encoding::utf32_le);
			_flags += open_flags::format_utf32_le;
			break;
		case text::encoding::utf32_be:
			set_file_encoding(_hfile, text::encoding::utf32_be);
			_flags += open_flags::format_utf32_be;
			break;
		default:// case text::encoding::utf8:
			set_file_encoding(_hfile, text::encoding::utf8);
			_flags += open_flags::format_utf8;
			break;
		}
	}
}

bool core_file::cursor(file_offset_t cur, stream_reference_t ref)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
#ifdef WINDOWS_PLATFORM
	LARGE_INTEGER lint;
	lint.QuadPart = cur;
	SetFilePointerEx(_hfile, lint, null, (uint)ref.get());
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	::lseek(_hfile, cur, (uint)ref.get());
#endif
	return true;
}

file_size_t core_file::cursor()const
{
#ifdef WINDOWS_PLATFORM
	if (_hfile == invalid_handle)
		return 0;
	LARGE_INTEGER cur;
	LARGE_INTEGER pos = { 0 };
	SetFilePointerEx(_hfile, pos, &cur, FILE_CURRENT);
	return cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return ::lseek(_hfile, 0, SEEK_CUR);
#endif
}

static file_offset_t read_file(file_handle_t hfile, pointer buffer, wsize size, dword& readed)
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

wsize core_file::read(wsize size, pointer buffer)
{
	dword readed;
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (buffer == null || size == 0 || !is_created())
		return 0;
	if (!(_flags & open_flags::access_in))
		return 0;

#ifdef WINAPI_FAMILY
	if (!ReadFile(_hfile, buffer, (dword)size, &readed, NULL)) {
		readed = 0;
		return 0;
	}

	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
	return (wsize)cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	readed = ::read(_hfile, buffer, size);
	return ::lseek(_hfile, 0, SEEK_CUR);
#endif
}

wsize core_file::read(ibuffer_view_t buffer)
{
	dword readed;
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (buffer == null  || !is_created())
		return 0;
	if (!(_flags & open_flags::access_in))
		return 0;

#ifdef WINAPI_FAMILY
	if (!ReadFile(_hfile, buffer->buffer_ptr(), (dword)buffer->buffer_size(), &readed, NULL)) {
		readed = 0;
		return 0;
	}

	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
	return (wsize)cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	readed = ::read(_hfile, buffer->buffer_ptr(), (dword)buffer->buffer_size());
	return ::lseek(_hfile, 0, SEEK_CUR);
#endif
}

wsize core_file::write(wsize size, pointer buffer)
{
	dword written;
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (buffer == null || size == 0 || !is_created())
		return 0;
	if (!(_flags & open_flags::access_out))
		return 0;

#ifdef WINAPI_FAMILY
	if (!WriteFile(_hfile, buffer, (dword)size, &written, NULL)) {
		written = 0;
		return 0;
	}

	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
	return (wsize)cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	written = ::write(_hfile, buffer, size);
	return ::lseek(_hfile, 0, SEEK_CUR);
#endif
}

wsize core_file::write(ibuffer_view_t buffer)
{
	dword written;
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	if (buffer == null || !is_created())
		return 0;
	if (!(_flags & open_flags::access_out))
		return 0;

#ifdef WINAPI_FAMILY
	if (!WriteFile(_hfile, buffer->buffer_ptr(), (dword)buffer->buffer_size(), &written, NULL)) {
		written = 0;
		return 0;
	}

	LARGE_INTEGER lint;
	LARGE_INTEGER cur;
	lint.QuadPart = 0;
	SetFilePointerEx(_hfile, lint, &cur, FILE_CURRENT);
	return (wsize)cur.QuadPart;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	written = ::write(_hfile, buffer->buffer_ptr(), (dword)buffer->buffer_size());
	return ::lseek(_hfile, 0, SEEK_CUR);
#endif
}


ibuffer_t core_file::map(wsize size, file_offset_t offset)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;

	if (map_handle(size + offset) == 0)
		return null;
	_map_counter++;
	return new mapped_file_buffer(this, _flags, size, offset);
}

bool core_file::unmap(ibuffer_t buffer, wsize size)
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

bool core_file::unmap(mapped_file_buffer* mapped_buffer)
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

bool core_file::set_mutex(core::async::mutex_ptr_t mutex)
{
	core::async::scope_locker<core::async::mutex_ptr_t> lock = _mutex;
	_mutex = mutex;
	return true;
}


////////////////////////////////////////////////////////////////////////////////
