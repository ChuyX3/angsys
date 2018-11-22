/*********************************************************************************************************************/
/*   File Name: input_text_file.cpp                                                                                  */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <ang/core/files.h>
#include "format_parser.h"

using namespace ang;
using namespace ang::streams;
using namespace ang::core::files;

input_text_file::input_text_file()
	: file()
{
}

input_text_file::input_text_file(path_view_t path)
	: input_text_file()
{
	open(path);
}

input_text_file::~input_text_file()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::files::input_text_file)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::files::input_text_file, file, streams::itext_input_stream);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::files::input_text_file, file, streams::itext_input_stream);

bool input_text_file::open(path_view_t path)
{
	if (is_valid())
		return false;
	if (!create(path, open_flags::access_in + open_flags::open_exist + open_flags::format_text))
		return false;
	return true;
}

bool input_text_file::is_eos()const
{
	return hfile.is_empty() ? true : hfile->is_eof();
}

text::encoding_t input_text_file::format()const
{
	return hfile.is_empty() ? text::encoding::none : hfile->format().get();
}

file_offset_t input_text_file::cursor()const
{
	return hfile.is_empty() ? 0 : hfile->cursor();
}

bool input_text_file::cursor(file_offset_t offset, stream_reference_t ref)
{
	return hfile.is_empty() ? false : hfile->cursor(offset, ref);
}

file_offset_t input_text_file::size()const
{
	return hfile.is_empty() ? 0 : hfile->size();
}

stream_mode_t input_text_file::mode()const
{
	return stream_mode::out;
}


template<text::encoding E_IN, text::encoding E_OUT>
wsize read_format(ifile_t file, str_view<typename const text::char_type_by_encoding<E_OUT>::char_t, E_OUT> format, var_args_t& args)
{
	constexpr bool SWAP_IN = text::is_endian_swapped<text::native_encoding<E_IN>::value>::value;
	constexpr bool SWAP_OUT = text::is_endian_swapped<text::native_encoding<E_OUT>::value>::value;

	typedef typename text::char_type_by_encoding<E_IN>::cahr_t in_char_t;
	typedef typename text::char_type_by_encoding<E_OUT>::cahr_t out_char_t;

	char32 c1, c2;
	in_char_t buffer[201];
	wsize i = 0, j = 0;
	file_offset_t cursor = file->cursor();
	file_offset_t forward = 0;
	wsize readed;
	wsize size1 = format.size() / sizeof(out_char_t);
	wsize size2 = 0;
	enum {
		detect = 0,
		reading_spaces,
		read_file,
		formating,
	}state;
	state = detect;
	while (!file->is_eof() && i < size1)
	{
	
		if (state == formating)
		{
			forward += j;
			file->cursor(cursor + forward, stream_reference::begin);
			j = 0;
			size2 = 0;
		}

		if (j >= size2) //reading data from file
		{
			forward += readed;
			size2 = (readed = file->read(200 * sizeof(in_char_t), buffer)) / sizeof(in_char_t);
			buffer[size2] = 0;
			j = 0;
		}
		
		
		if (state == reading_spaces ||
			char32(text::char_to_dword(format[i])) == U' ' || 
			char32(text::char_to_dword(format[i])) == U'\t') //character '\t' or ' ' means "read all spaces"
		{
			while (char32(text::char_to_dword(format[i])) == U' '
				|| char32(text::char_to_dword(format[i])) == U'\t') //read all spaces
				i++;

			while (char32(text::char_to_dword(buffer[j])) == U' '
				|| char32(text::char_to_dword(buffer[j])) == U'\t') //read all spaces
				j++;

			if (char32(text::char_to_dword(format[j])) == 0) // if it is end of string then
			{
				state = reading_spaces; // set the state to read spaces
				continue; // and continue to read more data from file
			}
			else
			{
				state = none;
			}
		}

		if (state == formating || 
			char32(text::char_to_dword(format[i])) == U'{') //parsing data
		{
			int args = 0;
			text::text_format_flags_t f;
			f.value = format_parser<E_OUT>::parse(format, i, args);
			if (state != formating && (size2 - j) < 20)
				// the data to parse is less that 20 characters, witch means that it can generate a parsing error 
			{
				state = read_file;
				continue;
			}
		}
		
	
	}

	return 0;
}

wsize input_text_file::read_format(raw_cstr_t format, var_args_t& args)
{
	/*if (hfile.is_empty() || hfile->is_eof() || format.ptr() == null)
		return 0;
	text::iencoder_t encoder = text::iencoder::get_encoder(format.encoding());
	wsize mycs = text::iencoder::get_encoder(hfile->format())->char_type().size();
	wsize cs = encoder->char_type().size();
	pointer ptr = buff->buffer_ptr();
	wsize out_size = (buff->buffer_size() / cs) - 1;
	wsize readed, total = 0;
	byte _buffer[200];
	auto cur = cursor();

	while (!hfile->is_eof() && out_size > 0)
	{
		readed = hfile->read(200, _buffer);
		auto str = encoder->convert(ptr, _buffer, this->format(), true, out_size, readed / mycs);
		ptr = &((byte*)str.ptr())[str.size()];
		out_size -= str.size() / cs;
		total += str.size();
	}
	cursor(cur + total, stream_reference::begin);
	return total;*/
	return 0;
}

wsize input_text_file::read(ibuffer_view_t buff, text::encoding_t e, wsize* written)
{
	if (hfile.is_empty() || hfile->is_eof() || buff == null || buff->buffer_ptr() == null)
		return 0;
	text::iencoder_t encoder = text::iencoder::get_encoder(e);
	wsize mycs = text::iencoder::get_encoder(hfile->format())->char_type().size();
	wsize cs = encoder->char_type().size();
	pointer ptr = buff->buffer_ptr();
	wsize out_size = (buff->buffer_size() / cs) - 1;
	wsize readed, total = 0;
	byte _buffer[200];
	auto cur = cursor();
	if (written)*written = out_size;
	while (!hfile->is_eof() && out_size > 0)
	{
		readed = hfile->read(200, _buffer);
		auto str = encoder->convert(ptr, _buffer, this->format(), true, out_size, readed / mycs);
		ptr = &((byte*)str.ptr())[str.size()];
		out_size -= str.size() / cs;
		total += str.size();
	}
	if (written)*written -= out_size;
	cursor(cur + total, stream_reference::begin);
	return total;
}

wsize input_text_file::read_line(ibuffer_view_t buff, text::encoding_t e, array_view<const char32_t> end, wsize* written)
{
	if (hfile.is_empty() || hfile->is_eof() || buff.is_empty())
		return 0;
	text::iencoder_t encoder = text::iencoder::get_encoder(e);
	text::iencoder_t my_encoder = text::iencoder::get_encoder(format());
	wsize mycs = my_encoder->char_type().size();
	wsize cs = encoder->char_type().size();
	pointer ptr = buff->buffer_ptr();
	wsize out_size = (buff->buffer_size() / cs) - 1;
	wsize readed, total = 0;
	byte _buffer[200];
	auto cur = cursor();
	bool endl = false;
	if (written)*written = out_size;
	while (!hfile->is_eof() && out_size > 0 && !endl)
	{
		readed = hfile->read(200, _buffer);
		
		auto at = my_encoder->find_any(_buffer, readed / mycs, 0, end);

		if (at != invalid_index)
			endl = true;
		else
			at = readed / mycs;

		auto str = encoder->convert(ptr, _buffer, format(), true, out_size, at - 1);

		ptr = &((byte*)str.ptr())[str.size()];
		out_size -= str.size() / cs;
		total += at * mycs;
	
	}
	if (written)*written -= out_size;
	cursor(cur + total, stream_reference::begin);
	return total;
}
