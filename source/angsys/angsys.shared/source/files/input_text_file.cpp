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

	if (!create(path, open_flags::access_in + open_flags::open_exist + open_flags::type_text))
		return false;
	return true;
}

text::encoding_t input_text_file::format()const
{
	return hfile.is_empty() ? text::encoding::unknown : hfile->encoding().get();
}

file_cursor_t input_text_file::cursor()const
{
	return hfile.is_empty() ? 0 : hfile->cursor();
}

void input_text_file::cursor(file_cursor_t offset, file_reference_t ref)
{
	if (!hfile.is_empty())	
		hfile->cursor(ref, offset);
}

bool input_text_file::read(core::delegates::function<bool(streams::itext_input_stream_t)> func, file_cursor_t offset, wsize size)
{
		ibuffer_t buff = map(min<wsize>((wsize)size, wsize(hfile->file_size() - offset)), offset);
		if (buff.get() == null)
			return false;	
		return func(new streams::text_buffer_input_stream(buff, hfile->encoding()));
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

wsize input_text_file::read(wstring& out, wsize count)
{
	if (hfile.is_empty())
		return 0U;
	switch (hfile->encoding())
	{
		case text::encoding::ascii: {
			string temp = new strings::string_buffer(count);
			temp->length(hfile->read(count, temp->str()));
			out = temp;
		} return out->length();
		case text::encoding::unicode: {
			if (out.is_empty())
				out = new strings::wstring_buffer(count);
			else
				out->realloc(count, false);
			out->length(hfile->read(count * sizeof(wchar), out->str()) / sizeof(wchar));
		} return out->length();
		case text::encoding::utf_8: {
			mstring temp = new strings::mstring_buffer(count);
			temp->length(hfile->read(count, temp->str()));
			out = temp;
		} return out->length();
	default:
		return 0U;
	}
}

wsize input_text_file::read(string& out, wsize count)
{
	if (hfile.is_empty())
		return 0U;
	switch (hfile->encoding())
	{
	case text::encoding::unicode: {
		wstring temp = new strings::wstring_buffer(count);
		temp->length(hfile->read(count * sizeof(wchar), temp->str()) / sizeof(wchar));
		out = temp;
	} return out->length();
	case text::encoding::ascii: {
		if (out.is_empty())
			out = new strings::string_buffer(count);
		else
			out->realloc(count, false);
		out->length(hfile->read(count , out->str()));
	} return out->length();
	case text::encoding::utf_8: {
		mstring temp = new strings::mstring_buffer(count);
		temp->length(hfile->read(count, temp->str()));
		out = temp;
	} return out->length();
	default:
		return 0U;
	}
}

wsize input_text_file::read(mstring& out, wsize count)
{
	if (hfile.is_empty())
		return 0U;
	switch (hfile->encoding())
	{
	case text::encoding::unicode: {
		wstring temp = new strings::wstring_buffer(count);
		temp->length(hfile->read(count * sizeof(char_t), temp->str()) / sizeof(char_t));
		out = temp;
	} return out->length();
	case text::encoding::utf_8: {
		if (out.is_empty())
			out = new strings::mstring_buffer(count);
		else
			out->realloc(count, false);
		out->length(hfile->read(count, out->str()));
	} return out->length();
	case text::encoding::ascii: {
		mstring temp = new strings::mstring_buffer(count);
		temp->length(hfile->read(count, temp->str()));
		out = temp;
	} return out->length();
	default:
		return 0U;
	}
}
