/*********************************************************************************************************************/
/*   File Name: input_text_file.cpp                                                                                  */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/files.hpp"

using namespace ang;
using namespace ang::streams;
using namespace ang::core::files;

input_text_file::input_text_file()
	: file()
{
}

input_text_file::input_text_file(path_view path)
	: input_text_file()
{
	open(path);
}

input_text_file::~input_text_file()
{
	close();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::files::input_text_file, file);

bool input_text_file::open(path_view path)
{
	if (is_valid())
		return false;

	if (!create(path, open_flags::access_in + open_flags::open_exist + open_flags::format_text))
		return false;
	return true;
}

text::encoding_t input_text_file::format()const
{
	return hfile.is_empty() ? text::encoding::none : hfile->format().get();
}

file_cursor_t input_text_file::cursor()const
{
	return hfile.is_empty() ? 0 : hfile->position();
}

void input_text_file::cursor(file_cursor_t offset, file_reference_t ref)
{
	if (!hfile.is_empty())	
		hfile->move_to(offset, ref);
}

bool input_text_file::read(core::delegates::function<bool(streams::itext_input_stream_t)> func, file_cursor_t offset, wsize size)
{
		ibuffer_t buff = map(min<wsize>((wsize)size, wsize(hfile->stream_size() - offset)), offset);
		if (buff.get() == null)
			return false;	
		switch (hfile->format())
		{
		case text::encoding::ascii: return func(new streams::text_buffer_input_stream(new text::text_buffer_wrapper<text::encoding::ascii>(buff)));
		case text::encoding::utf8: return func(new streams::text_buffer_input_stream(new text::text_buffer_wrapper<text::encoding::utf8>(buff)));
		case text::encoding::utf16_le: return func(new streams::text_buffer_input_stream(new text::text_buffer_wrapper<text::encoding::utf16_le>(buff)));
		case text::encoding::utf16_be: return func(new streams::text_buffer_input_stream(new text::text_buffer_wrapper<text::encoding::utf16_be>(buff)));
		case text::encoding::utf32_le: return func(new streams::text_buffer_input_stream(new text::text_buffer_wrapper<text::encoding::utf32_le>(buff)));
		case text::encoding::utf32_be: return func(new streams::text_buffer_input_stream(new text::text_buffer_wrapper<text::encoding::utf32_be>(buff)));
		default: return nullptr;
		}
}

//ang::core::async::iasync_t<bool> input_text_file::read_async(core::delegates::function<bool(streams::itext_input_stream_t)> func, file_cursor_t offset, wsize size)
//{
//	add_ref();
//	return core::async::async_task<bool>::run_async([=](async::iasync<bool>*, var_args_t)->bool
//	{
//		input_text_file_t _this = this;
//		release();
//
//		ibuffer_t buff = map(min<wsize>((wsize)size, wsize(hfile->file_size() - offset)), offset);
//		if (buff.get() == null)
//			return false;
//		return func(new streams::text_buffer_input_stream(buff, hfile->encoding()));
//	});
//}

wsize input_text_file::read(raw_str_t& out)
{
	if (hfile.is_empty())
		return 0U;
	return hfile->read(out.ptr(), out.size(), out.encoding());
}
