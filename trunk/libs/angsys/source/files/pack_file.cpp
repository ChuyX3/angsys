/*********************************************************************************************************************/
/*   File Name: pack_file.cpp                                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/files.hpp"

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
				: public bean
				, public ifile
			{
			private:
				path _path;
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

			public: //overrides
				bool create(path_view path, open_flags_t flags, ifile_t original_source, ulong64 offset, ulong64 size);
				bool is_created()const;
				bool close();

			public: //Methos
				virtual streams::stream_mode_t mode()const override;
				virtual path file_path()const override;
				virtual file_size_t stream_size()const override;
				virtual bool stream_size(file_size_t) override;
				virtual void format(text::encoding_t)override;
				virtual text::encoding_t format()const override;
				virtual bool move_to(file_cursor_t, file_reference_t) override;
				virtual file_size_t position()const override;
				virtual wsize read(pointer buffer, wsize size, text::text_format_t)override;
				virtual wsize write(pointer buffer, wsize size, text::text_format_t)override;
				virtual wsize read(pointer buffer, wsize size, text::encoding_t)override;
				virtual wsize write(pointer buffer, wsize size, text::encoding_t)override;
				virtual wsize read(ibuffer_view_t buffer, text::encoding_t = text::encoding::binary)override;
				virtual wsize write(ibuffer_view_t buffer, text::encoding_t = text::encoding::binary)override;
				virtual ibuffer_t map(wsize size, file_cursor_t offset)override;
				virtual bool unmap(ibuffer_t, wsize)override;
				virtual bool set_mutex(core::async::mutex_ptr_t)override;

			};

		}
	}
}


bool load_directory_recursive(files::path const& root_path, files::path const& subpath, file_cursor_t& header_size, file_cursor_t& offset, collections::smart_vector<collections::pair<files::path, pack_file_info>>& files)
{
#if defined WINDOWS_PLATFORM
	WIN32_FIND_DATAW ffd;
	LARGE_INTEGER filesize;

	files::path path = root_path;

	if (!subpath.is_empty() && subpath->length() > 0)
		path += "/"_sv + subpath;

	path += "/*"_sv;

	HANDLE  hFind = FindFirstFileW((path_view)path, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		HRESULT hr = GetLastError();
		return false;
	}

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (ffd.cFileName != L"."_sv && ffd.cFileName != L".."_sv)
			{
				files::path _subpath = subpath;
				if (!subpath.is_empty() && subpath->length() > 0)
					_subpath += "/"_sv;
				_subpath += path_view(ffd.cFileName);
				load_directory_recursive(root_path, _subpath, header_size, offset, files);
			}
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;

			files::path _subpath = subpath;
			if (!subpath.is_empty() && subpath->length() > 0)
				_subpath += "/"_sv;
			_subpath += path_view(ffd.cFileName);

			files += {_subpath, { (file_size_t)filesize.QuadPart, offset } };
			offset += filesize.QuadPart;
			header_size += sizeof(pack_file_info) + _subpath->length() * sizeof(wchar) + sizeof(uint);
		}
	} while (FindNextFileW(hFind, &ffd) != 0);
	FindClose(hFind);
#else
#endif
	return true;
}


bool pack_file::create_pack_from_folder(path_view in_path, path_view out_path)
{
	collections::smart_vector<collections::pair<files::path, pack_file_info>> paths;
#if defined WINDOWS_PLATFORM
	file_cursor_t offset = 0;
	file_cursor_t header_size = 0;

	load_directory_recursive(in_path, L""_sv, header_size, offset, paths);

	output_binary_file_t out_file = new output_binary_file(out_path);
	if (!out_file->is_valid())
		return false;

	ulong64 count = paths->counter();
	//out_file->write(&count, sizeof(count));
	out_file->write([&](streams::ibinary_output_stream_t stream)->bool 
	{
		stream << paths->counter();
		for(collections::pair<files::path, pack_file_info>& info : paths)
		{
			info.value.offset += header_size;
			stream << info.value.offset << info.value.size << info.key;
		}
		files::path path = in_path;
		path += "/*"_sv;
		auto end = path->end();
		for(collections::pair<files::path, pack_file_info>& info : paths)
		{
			path->replace(info.key, end, collections::iterator<wchar>(path.get(), null, path->length()));
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

		}
		return true;
	}, 0, offset + header_size + sizeof(ulong64));
	
#endif
	return true;
}


pack_file::pack_file()
{

}

pack_file::~pack_file()
{

}


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

bool pack_file::create(path_view path, open_flags_t flags)
{
	return false;
}

smart_array<path> pack_file::paths()const
{
	return null;
}

bool pack_file::register_paths(path_view path)
{
	return false;
}

bool pack_file::create_file_handle(path_view path, open_flags_t flags, ifile_ptr_t out)
{
	if (flags.is_active(open_flags::access_out))
		return false;

	return true;
}

bool pack_file::open(path_view path, input_text_file_t& out)
{
	out = null;
	return false;
}

bool pack_file::open(path_view, output_text_file_t& out)
{
	out = null;
	return false;
}

bool pack_file::open(path_view path, input_binary_file_t& out)
{
	out = null;
	return false;
}

bool pack_file::open(path_view, output_binary_file_t& out)
{
	out = null;
	return false;
}
