/*********************************************************************************************************************/
/*   File Name: binary_file.cpp                                                                                */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/files.h"

using namespace ang;
using namespace ang::streams;
using namespace ang::core::files;

binary_file::binary_file()
	: file()
{
}

binary_file::binary_file(path_view path)
	: binary_file()
{
	open(path);
}

//binary_file::~binary_file()
//{
//	close();
//}
//
//ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::files::binary_file, file);

bool binary_file::open(path_view path)
{
	if (is_valid())
		return false;

	if (!create(path, open_flags::access_inout + open_flags::open_alway + open_flags::type_binary))
		return false;
	return true;
}

file_cursor_t binary_file::cursor()const
{
	return hfile.is_empty() ? 0 : hfile->cursor();
}

void binary_file::cursor(file_cursor_t offset, file_reference_t ref)
{
	if (!hfile.is_empty())
		hfile->cursor(ref, offset);
}

bool binary_file::write(core::delegates::function<bool(streams::ibinary_output_stream_t)> func, file_cursor_t offset, wsize size)
{
	ibuffer_t buff = map(size, offset);
	if (buff.get() == null)
		return false;
	streams::ibinary_output_stream_t stream = new streams::binary_buffer_output_stream(buff);
	auto val = func(stream);
	buff = null;
	return val;
}

wsize binary_file::write(pointer out, wsize count)
{
	if (hfile.is_empty() || out == null || count == 0)
		return 0U;
	return hfile->write(count, out);
}

bool binary_file::read(core::delegates::function<bool(streams::ibinary_input_stream_t)> func, file_cursor_t offset, wsize size)
{
	ibuffer_t buff = map(min<wsize>((wsize)size, wsize(hfile->file_size() - offset)), offset);
	if (buff.get() == null)
		return false;
	return func(new streams::binary_buffer_input_stream(buff));
}

wsize binary_file::read(pointer out, wsize count)
{
	if (hfile.is_empty() || out == null || count == 0)
		return 0U;
	return hfile->read(count, out);
}

