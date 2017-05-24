/*********************************************************************************************************************/
/*   File Name: input_binary_file.cpp                                                                                */
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

input_binary_file::input_binary_file()
	: file()
{
}

input_binary_file::input_binary_file(cwstr_t path)
	: input_binary_file()
{
	open(path);
}

input_binary_file::~input_binary_file()
{
	close();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::files::input_binary_file, file);

bool input_binary_file::open(cwstr_t path)
{
	if (is_valid())
		return false;

	if (!create(path, open_flags::access_in + open_flags::open_exist + open_flags::type_binary))
		return false;
	return true;
}

file_cursor_t input_binary_file::cursor()const
{
	return hfile.is_empty() ? 0 : hfile->cursor();
}

void input_binary_file::cursor(file_cursor_t offset, file_reference_t ref)
{
	if (!hfile.is_empty())	
		hfile->cursor(ref, offset);
}

bool input_binary_file::read(core::delegates::function<bool(streams::ibinary_input_stream_t)> func, file_cursor_t offset, wsize size)
{
		ibuffer_t buff = map(min<wsize>((wsize)size, wsize(hfile->file_size() - offset)), offset);
		if (buff.get() == null)
			return false;	
		return func(new streams::binary_buffer_input_stream(buff));
}

//ang::core::async::iasync_t<bool> input_binary_file::read_async(core::delegates::function<bool(streams::ibinary_input_stream_t)> func, file_cursor_t offset, wsize size)
//{
//	add_ref();
//	return core::async::async_task<bool>::run_async([=](async::iasync<bool>*, var_args_t)->bool
//	{
//		input_binary_file_t _this = this;
//		release();
//
//		ibuffer_t buff = map(min<wsize>((wsize)size, wsize(hfile->file_size() - offset)), offset);
//		if (buff.get() == null)
//			return false;
//		return func(new streams::binary_buffer_input_stream(buff));
//	});
//}

wsize input_binary_file::read(pointer out, wsize count)
{
	if (hfile.is_empty() || out == null || count == 0)
		return 0U;
	return hfile->read(count, out);
}
