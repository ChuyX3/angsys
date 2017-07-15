/*********************************************************************************************************************/
/*   File Name: pack_file.cpp                                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/files.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::files;

namespace ang
{
	namespace core
	{
		namespace files
		{

			class packed_file_impl
				: public object
				, public ifile
			{
			private:
				wstring _path;
				ifile_t _original_source;
				file_flags_t _flags;
				ulong64 _size;
				ulong64 _offset;
				ulong64 _cursor;

				ibuffer_t _mapped_buffer;

			public:
				packed_file_impl();

			private:
				virtual~packed_file_impl();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				bool create(cwstr_t path, open_flags_t flags, ifile_t original_source, ulong64 offset, ulong64 size);
				bool is_created()const;
				bool close();

			public: //Methos
				virtual streams::stream_mode_t mode()const override;
				virtual wstring file_path()const override;
				virtual file_size_t file_size()const override;
				virtual text::encoding_t encoding()const override;
				virtual void cursor(file_reference_t, file_cursor_t) override;
				virtual file_size_t cursor()const override;
				virtual wsize read(wsize size, pointer buffer)override;
				virtual wsize write(wsize size, pointer buffer)override;
				virtual wsize read(ibuffer_t buffer)override;
				virtual wsize write(ibuffer_t buffer)override;
				virtual ibuffer_t map(wsize size, file_size_t offset)override;
				virtual bool unmap(ibuffer_t)override;

			};

		}
	}
}


bool load_directory_recursive(wstring const& root_path, wstring const& subpath, file_cursor_t& header_size, file_cursor_t& offset, collections::vector<collections::pair<wstring, pack_file_info>>& files)
{
#if defined WINDOWS_PLATFORM
	WIN32_FIND_DATAW ffd;
	LARGE_INTEGER filesize;

	wstring path = root_path;

	if (!subpath.is_empty() && subpath->counter() > 0)
		path += "/"_s + subpath;

	path += "/*"_s;

	HANDLE  hFind = FindFirstFileW((cwstr_t)path, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		HRESULT hr = GetLastError();
		return false;
	}

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (ffd.cFileName != L"."_s && ffd.cFileName != L".."_s)
			{
				wstring _subpath = subpath;
				if (!subpath.is_empty() && subpath->counter() > 0)
					_subpath += "/"_s;
				_subpath += cwstr_t(ffd.cFileName);
				load_directory_recursive(root_path, _subpath, header_size, offset, files);
			}
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;

			wstring _subpath = subpath;
			if (!subpath.is_empty() && subpath->counter() > 0)
				_subpath += "/"_s;
			_subpath += cwstr_t(ffd.cFileName);

			files += {_subpath, { (file_size_t)filesize.QuadPart, offset } };
			offset += filesize.QuadPart;
			header_size += sizeof(pack_file_info) + _subpath->counter() * sizeof(wchar) + sizeof(uint);
		}
	} while (FindNextFileW(hFind, &ffd) != 0);
	FindClose(hFind);
#elif defined ANDROID_PLATFORM
#endif
	return true;
}


bool pack_file::create_pack_from_folder(cwstr_t in_path, cwstr_t out_path)
{
	collections::vector<collections::pair<wstring, pack_file_info>> paths;
#if defined WINDOWS_PLATFORM
	file_cursor_t offset = 0;
	file_cursor_t header_size = 0;

	load_directory_recursive(in_path, L""_s, header_size, offset, paths);

	output_binary_file_t out_file = new output_binary_file(out_path);
	if (!out_file->is_valid())
		return false;

	ulong64 count = paths->counter();
	//out_file->write(&count, sizeof(count));
	out_file->write([&](streams::ibinary_output_stream_t stream)->bool 
	{
		stream << paths->counter();
		foreach(paths, [&](collections::pair<wstring, pack_file_info>& info)
		{
			info.value().offset += header_size;
			stream << info.value().offset << info.value().size << info.key_value();
		});
		wstring path = in_path;
		path += "/*"_s;
		auto end = path->end();
		foreach(paths, [&](collections::pair<wstring, pack_file_info>& info)
		{
			path->replace(info.key_value(), end, collections::iterator<wchar>(path.get(), null, path->counter()));
			input_binary_file_t file = new input_binary_file(path);
			if (file->is_valid())
			{
				buffer_t buff = new(file->file_size()) buffer();
				file->read(buff->buffer_ptr(), buff->buffer_size());
				stream->write(buff->buffer_ptr(), buff->buffer_size());
			}
			else
			{
				stream->move_to(info.value().size, ang::streams::stream_reference::current);
			}

		});
		return true;
	}, 0, offset + header_size + sizeof(ulong64));
	
#elif defined ANDROID_PLATFORM

#endif
	return true;
}


pack_file::pack_file()
{

}

pack_file::~pack_file()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::core::files::pack_file);
ANG_IMPLEMENT_OBJECTNAME(ang::core::files::pack_file);

bool pack_file::is_child_of(type_name_t name)
{
	return name == type_name<pack_file>()
		|| file::is_child_of(name)
		|| name == type_name<ifile_system>();
}

bool pack_file::is_kind_of(type_name_t name)const
{
	return name == type_name<pack_file>()
		|| file::is_kind_of(name)
		|| name == type_name<ifile_system>();
}

bool pack_file::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == type_name<pack_file>())
	{
		*out = static_cast<pack_file*>(this);
		return true;
	}
	else if (file::query_object(name, out))
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

bool pack_file::create(cwstr_t path, open_flags_t flags)
{
	return false;
}

array<wstring> pack_file::paths()const
{
	return null;
}

bool pack_file::register_paths(cwstr_t path)
{
	return false;
}

bool pack_file::create_file_handle(cwstr_t path, open_flags_t flags, ifile_ptr_t out)
{
	if (flags.is_active(open_flags::access_out))
		return false;

	return true;
}

bool pack_file::open(cwstr_t path, input_text_file_t& out)
{
	out = null;
	return false;
}

bool pack_file::open(cwstr_t, output_text_file_t& out)
{
	out = null;
	return false;
}

bool pack_file::open(cwstr_t path, input_binary_file_t& out)
{
	out = null;
	return false;
}

bool pack_file::open(cwstr_t, output_binary_file_t& out)
{
	out = null;
	return false;
}
