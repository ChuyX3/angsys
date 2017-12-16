/*********************************************************************************************************************/
/*   File Name: output_text_file.cpp                                                                                 */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/files.hpp"
#include "file_system.h"
#include "string_helper.hpp"

using namespace ang;
using namespace ang::streams;
using namespace ang::core::files;

output_text_file::output_text_file()
	: file()
{
}

output_text_file::output_text_file(path_view path, text::encoding_t encoding)
	: output_text_file()
{
	open(path, encoding);
}

output_text_file::~output_text_file()
{
	close();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::files::output_text_file, file);



bool output_text_file::open(path_view path, text::encoding_t encoding)
{
	if (is_valid())
		return false;
	auto flags = open_flags::access_out + open_flags::open_alway;

	flags += get_format(encoding);

	if (!create(path, flags))
		return false;
	return true;
}

text::encoding_t output_text_file::format()const
{
	return hfile.is_empty() ? text::encoding::none : hfile->format().get();
}

void output_text_file::format(text::encoding_t value)
{
	if (hfile.is_empty())
		return;
	hfile->format(value);
}

file_cursor_t output_text_file::cursor()const
{
	return hfile.is_empty() ? 0 : hfile->position();
}

void output_text_file::cursor(file_cursor_t offset, file_reference_t ref)
{
	if (!hfile.is_empty())	
		hfile->move_to(offset, ref);
}

bool output_text_file::write(core::delegates::function<bool(streams::itext_output_stream_t)> func, file_cursor_t offset, wsize size)
{
		ibuffer_t buff = map(min<wsize>((wsize)size, wsize(hfile->stream_size() - offset)), offset);
		if (buff.get() == null)	return false;	
		switch (hfile->format())
		{
		case text::encoding::ascii: return func(new streams::text_buffer_output_stream(new text::text_buffer_wrapper<text::encoding::ascii>(buff)));
		case text::encoding::utf8: return func(new streams::text_buffer_output_stream(new text::text_buffer_wrapper<text::encoding::utf8>(buff)));
		case text::encoding::utf16_le: return func(new streams::text_buffer_output_stream(new text::text_buffer_wrapper<text::encoding::utf16_le>(buff)));
		case text::encoding::utf16_be: return func(new streams::text_buffer_output_stream(new text::text_buffer_wrapper<text::encoding::utf16_be>(buff)));
		case text::encoding::utf32_le: return func(new streams::text_buffer_output_stream(new text::text_buffer_wrapper<text::encoding::utf32_le>(buff)));
		case text::encoding::utf32_be: return func(new streams::text_buffer_output_stream(new text::text_buffer_wrapper<text::encoding::utf32_be>(buff)));
		default: return nullptr;
		}		
}

//ang::core::async::iasync_t<bool> output_text_file::write_async(core::delegates::function<bool(streams::itext_output_stream_t)> func, file_cursor_t offset, wsize size)
//{
//	add_ref();
//	return core::async::async_task<bool>::run_async([=](async::iasync<bool>*, var_args_t)->bool
//	{
//		output_text_file_t _this = this;
//		release();
//
//		ibuffer_t buff = map(size, offset);
//		if (buff.get() == null)
//			return false;
//		return func(new streams::text_buffer_output_stream(buff, hfile->encoding()));
//	});
//}

wsize output_text_file::write(raw_str_t cstr)
{
	if (hfile.is_empty())
		return 0U;

	if (hfile->format() == text::encoding::binary)
	{
		hfile->write(cstr.ptr(), cstr.size(), text::text_format());
	}
	//else if (hfile->format() == cstr.encoding())
	//{
	//	hfile->write(cstr.ptr(), cstr.size() + cstr.char_size(), text::text_format());
	//}
	else
	{
		text::encoder_interface encoder;
		text::encoder_interface::initialize_interface(&encoder, hfile->format());
	
		alignas(16) stack_array<byte, 100> buffer;
		raw_str_t dest = { buffer.begin(), 100, hfile->format() };
		wsize t = 0, i = 0, c = 0, idx = 0, count = cstr.count(), cs = cstr.char_size(), cs2 = dest.char_size();

		while (idx <= count)
		{
			i = encoder._convert_string(dest.ptr(), dest.count() - 1, (byte*)cstr.ptr() + idx*cs, c, cstr.encoding(), true);
			idx += c; c = 0; t += i;
			hfile->write(dest.ptr(), i * cs2, text::text_format());
			
		}
		return t * cs2;
	}
}

