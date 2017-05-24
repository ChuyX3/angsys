/*********************************************************************************************************************/
/*   File Name: output_text_file.cpp                                                                                 */
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

output_text_file::output_text_file()
	: file()
{
}

output_text_file::output_text_file(cwstr_t path)
	: output_text_file()
{
	open(path);
}

output_text_file::~output_text_file()
{
	close();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::files::output_text_file, file);

bool output_text_file::open(cwstr_t path)
{
	if (is_valid())
		return false;

	if (!create(path, open_flags::access_out + open_flags::open_alway + open_flags::type_text))
		return false;
	return true;
}

text::encoding_t output_text_file::format()const
{
	return hfile.is_empty() ? text::encoding::unknown : hfile->encoding().get();
}

void output_text_file::format(text::encoding_t value)
{
	if (hfile.is_empty())
		return;
	hfile->encoding(value);
}

file_cursor_t output_text_file::cursor()const
{
	return hfile.is_empty() ? 0 : hfile->cursor();
}

void output_text_file::cursor(file_cursor_t offset, file_reference_t ref)
{
	if (!hfile.is_empty())	
		hfile->cursor(ref, offset);
}

bool output_text_file::write(core::delegates::function<bool(streams::itext_output_stream_t)> func, file_cursor_t offset, wsize size)
{
		ibuffer_t buff = map(min<wsize>((wsize)size, wsize(hfile->file_size() - offset)), offset);
		if (buff.get() == null)
			return false;	
		return func(new streams::text_buffer_output_stream(buff, hfile->encoding()));
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

wsize output_text_file::write(cwstr_t cstr)
{
	if (hfile.is_empty())
		return 0U;
	switch (hfile->encoding())
	{
		case text::encoding::ascii: {
			string temp = cstr;
			hfile->write(temp->length(), temp->str());
		} return cstr.size();
		case text::encoding::unicode: {
			hfile->write(cstr.size() * sizeof(wchar), (pointer)cstr);
		} return cstr.size();
		case text::encoding::utf_8: {
			mstring temp = cstr;
			hfile->write(temp->length(), temp->str());
		} return cstr.size();
	default:
		return 0U;
	}
}

wsize output_text_file::write(cstr_t cstr)
{
	if (hfile.is_empty())
		return 0U;
	switch (hfile->encoding())
	{
	case text::encoding::ascii: {
		hfile->write(cstr.size(), (pointer)cstr);
	} return cstr.size();
	case text::encoding::unicode: {
		wstring temp = cstr;
		hfile->write(temp->length() * sizeof(wchar), (pointer)temp->str());
	} return cstr.size();
	case text::encoding::utf_8: {
		mstring temp = cstr;
		hfile->write(temp->length(), (pointer)temp->str());
	} return cstr.size();
	default:
		return 0U;
	}
}

wsize output_text_file::write(cmstr_t cstr)
{
	if (hfile.is_empty())
		return 0U;
	switch (hfile->encoding())
	{
	case text::encoding::ascii: {
		string temp = cstr;
		hfile->write(temp->length(), (pointer)temp->str());
	} return cstr.size();
	case text::encoding::unicode: {
		string temp = cstr;
		hfile->write(temp->length() * sizeof(wchar), (pointer)temp->str());
	} return cstr.size();
	case text::encoding::utf_8: {
		hfile->write(cstr.size(), (pointer)cstr);
	} return cstr.size();
	default:
		return 0U;
	}
}
