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
	return m_hfile.is_empty() ? true : m_hfile->is_eof();
}

text::encoding_t input_text_file::format()const
{
	return m_hfile.is_empty() ? text::encoding::none : m_hfile->format().get();
}

file_offset_t input_text_file::cursor()const
{
	return m_hfile.is_empty() ? 0 : m_hfile->cursor();
}

bool input_text_file::cursor(file_offset_t offset, stream_reference_t ref)
{
	return m_hfile.is_empty() ? false : m_hfile->cursor(offset, ref);
}

file_offset_t input_text_file::size()const
{
	return m_hfile.is_empty() ? 0 : m_hfile->size();
}

stream_mode_t input_text_file::mode()const
{
	return stream_mode::out;
}


template<text::encoding E_OUT>
wsize read_format(ifile_t file, str_view<typename const text::char_type_by_encoding<E_OUT>::char_t, E_OUT> format, var_args_t& args)
{
	constexpr bool SWAP_IN = text::is_endian_swapped<text::native_encoding<E_IN>::value>::value;
	constexpr bool SWAP_OUT = text::is_endian_swapped<text::native_encoding<E_OUT>::value>::value;

	typedef typename text::char_type_by_encoding<E_IN>::char_t in_char_t;
	typedef typename text::char_type_by_encoding<E_OUT>::char_t out_char_t;

	char32 c1, c2;
	in_char_t buffer[201];
	wsize format_idx = 0, buffer_idx = 0, temp;
	file_offset_t cursor = file->cursor();
	file_offset_t forward = 0;
	int arg_idx = 0;
	text::text_format_flags_t tff;

	args = new var_args();

	wsize readed = 0;
	wsize size1 = format.size() / sizeof(out_char_t);
	wsize size2 = 0;
	enum {
		detect = 0,
		reading_spaces,
		read_file,
		formating,
	}state;
	state = detect;
	while (!file->is_eof() && format_idx < size1)
	{
	
		if (state == formating)
		{
			forward += buffer_idx * sizeof(in_char_t);
			file->cursor(cursor + forward, stream_reference::begin);
			buffer_idx = 0;
			size2 = 0;
		}

		if (buffer_idx >= size2) //reading data from file
		{
			forward += readed;
			size2 = (readed = file->read(200 * sizeof(in_char_t), buffer) - cursor) / sizeof(in_char_t);
			buffer[size2] = 0;
			buffer_idx = 0;
		}
		
		
		if (state == reading_spaces ||
			char32(text::char_to_dword(format[format_idx])) == U' ' || 
			char32(text::char_to_dword(format[format_idx])) == U'\t') //character '\t' or ' ' means "read all spaces"
		{
			while (char32(text::char_to_dword(format[format_idx])) == U' '
				|| char32(text::char_to_dword(format[format_idx])) == U'\t') //read all spaces
				format_idx++;

			while (char32(text::char_to_dword(buffer[buffer_idx])) == U' '
				|| char32(text::char_to_dword(buffer[buffer_idx])) == U'\t') //read all spaces
				buffer_idx++;

			if (char32(text::char_to_dword(format[buffer_idx])) == 0) // if it is end of string then
			{
				state = reading_spaces; // set the state to read spaces
				continue; // and continue to read more data from file
			}
			else
			{
				state = detect;
			}
		}

		if (state == formating || 
			char32(text::char_to_dword(format[format_idx])) == U'{') //parsing data
		{
				
			if (state != formating && (size2 - buffer_idx) < 20)
				// the data to parse is less that 20 characters, witch means that it can generate a parsing error 
			{
				state = formating;
			}
			else
			{
				var val = nullptr;
				tff.value = format_parser<E_OUT>::parse(format, format_idx, arg_idx);
				switch (tff.value)
				{
				case text::text_format::target::signed_:
					val =(uint)str_to_signed(buffer, buffer_idx, tff.base);
					break;
				case text::text_format::target::unsigned_:
					val = (uint)str_to_unsigned(buffer, buffer_idx, tff.base);
					break;
				case text::text_format::target::float_:
					val = (uint)str_to_floating(buffer, buffer_idx, tff.exponent);
					break;
				case 0: //no format readed
					temp = buffer_idx;
					if(text::to_char32<false, SWAP_OUT>(format, format_idx) != text::to_char32<false, SWAP_IN>(buffer, buffer_idx)) //diferent text
					{
						forward += temp2 * sizeof(in_char_t);
						file->cursor(cursor + forward, stream_reference::begin);
						return forward;
					}
				default:
					break;
				}
				if (arg_idx > (int)args->size())
					args->size(arg_idx + 1, true);
				args[arg_idx] = val;
			}
			continue;
		}

		temp = buffer_idx;
		if (text::to_char32<false, SWAP_OUT>(format, format_idx) != text::to_char32<false, SWAP_IN>(buffer, buffer_idx)) //diferent text
		{
			forward += temp2 * sizeof(in_char_t);
			file->cursor(cursor + forward, stream_reference::begin);
			return forward;
		}
	}

	forward += buffer_idx * sizeof(in_char_t);
	file->cursor(cursor + forward, stream_reference::begin);

	return forward;
}

wsize input_text_file::read_format(raw_cstr_t format, var_args_t& args)
{
	if (m_hfile.is_empty())
		throw_exception(except_code::invalid_access);

	text::iencoder_t ie = text::iencoder::get_encoder(m_hfile->format());
	text::iencoder_t oe = text::iencoder::get_encoder(format.encoding());
	text::iformat_parser_t ip = text::iformat_parser::get_parser(m_hfile->format());
	text::iformat_parser_t op = text::iformat_parser::get_parser(format.encoding());

	char32 c1, c2;
	wsize format_idx = 0, buffer_idx = 0, temp,
		ics = ie->char_type().size(),
		ocs = oe->char_type().size();
	scope_array<byte> buffer(2001 * ics);
	
	file_offset_t cursor = m_hfile->cursor();
	file_offset_t forward = 0;
	int arg_idx = 0;
	text::text_format_flags_t tff;

	args = new var_args();

	wsize readed = 0;
	wsize size1 = format.size() / ocs;
	wsize size2 = 0;
	enum {
		detect = 0,
		reading_spaces,
		reading_endl,
		read_file,
		formating,
	}state;
	state = detect;
	while (!m_hfile->is_eof() && format_idx < size1)
	{

		if (state == formating)
		{
			forward += buffer_idx * ics;
			m_hfile->cursor(cursor + forward, stream_reference::begin);
			buffer_idx = 0;
			size2 = 0;
		}

		if (buffer_idx >= size2) //reading data from file
		{
			forward += readed;
			size2 = (readed = m_hfile->read(200 * ics, buffer.get()) - cursor) / ics;
			ie->set_eos(buffer.get(), size2);
			buffer_idx = 0;
		}
	
		if (state == reading_spaces ||
			ie->to_char32(format.ptr(), format_idx, false) == U' ' ||
			ie->to_char32(format.ptr(), format_idx, false) == U'\t') //character '\t' or ' ' means "read all spaces"
		{
			while (ie->to_char32(format.ptr(), format_idx, false) == U' '
				|| ie->to_char32(format.ptr(), format_idx, false) == U'\t') //read all spaces
				format_idx++;

			while (ie->to_char32(buffer.get(), buffer_idx, false) == U' '
				|| ie->to_char32(buffer.get(), buffer_idx, false) == U'\t') //read all spaces
				buffer_idx++;

			if (ie->to_char32(buffer.get(), buffer_idx, false) == 0) // if it is end of string then
			{
				state = reading_spaces; // set the state to read spaces
				continue; // and continue to read more data from file
			}
			else
			{
				state = detect;
			}
		}

		if (state == reading_endl ||
			ie->to_char32(format.ptr(), format_idx, false) == U'\r' ||
			ie->to_char32(format.ptr(), format_idx, false) == U'\n') //character '\t' or ' ' means "read all spaces"
		{
			while (ie->to_char32(format.ptr(), format_idx, false) == U'\r'
				|| ie->to_char32(format.ptr(), format_idx, false) == U'\n') //read all spaces
				format_idx++;

			while (ie->to_char32(buffer.get(), buffer_idx, false) == U'\r'
				|| ie->to_char32(buffer.get(), buffer_idx, false) == U'\n') //read all spaces
				buffer_idx++;

			if (ie->to_char32(buffer.get(), buffer_idx, false) == 0) // if it is end of string then
			{
				state = reading_endl; // set the state to read spaces
				continue; // and continue to read more data from file
			}
			else
			{
				state = detect;
			}
		}

		if (state == formating ||
			ie->to_char32(format.ptr(), format_idx, false) == U'{') //parsing data
		{

			if (state != formating && (size2 - buffer_idx) < 20)
				// the data to parse is less that 20 characters, witch means that it can generate a parsing error 
			{
				state = formating;
			}
			else
			{
				var val = nullptr;
				tff.value = op->parse(format.ptr(), format.size() / ocs, format_idx, arg_idx).format_flags();
				switch (tff.value)
				{
				case text::text_format::target::signed_:
					val = (int) ip->to_signed(buffer.get(), size2, buffer_idx, true, tff.base);
					break;
				case text::text_format::target::unsigned_:
					val = (uint)ip->to_unsigned(buffer.get(), size2, buffer_idx, true, tff.base);
					break;
				case text::text_format::target::float_:
					val = (float)ip->to_floating(buffer.get(), size2, buffer_idx, true, tff.exponent);
					break;
				case 0: //no format readed
					temp = buffer_idx;
					if (oe->to_char32(format.ptr(), format_idx) != ie->to_char32(buffer.get(), buffer_idx)) //diferent text
					{
						forward += temp * ics;
						m_hfile->cursor(cursor + forward, stream_reference::begin);
						return forward;
					}
				default:
					break;
				}
				if (arg_idx > (int)args->size())
					args->size(arg_idx + 1, true);
				if (arg_idx < 0)
					args += val;
				else
					args[arg_idx] = val;
			}
			continue;
		}

		temp = buffer_idx;
		if (oe->to_char32(format.ptr(), format_idx) != ie->to_char32(buffer.get(), buffer_idx)) //diferent text
		{
			forward += temp * ics;
			m_hfile->cursor(cursor + forward, stream_reference::begin);
			return forward;
		}
	}

	forward += buffer_idx * ics;
	m_hfile->cursor(cursor + forward, stream_reference::begin);

	return forward;
}

wsize input_text_file::read(ibuffer_view_t buff, text::encoding_t e, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || buff == null || buff->buffer_ptr() == null)
		return 0;
	text::iencoder_t encoder = text::iencoder::get_encoder(e);
	wsize mycs = text::iencoder::get_encoder(m_hfile->format())->char_type().size();
	wsize cs = encoder->char_type().size();
	pointer ptr = buff->buffer_ptr();
	wsize out_size = (buff->buffer_size() / cs) - 1;
	wsize readed, total = 0;
	byte _buffer[200];
	auto cur = cursor();
	if (written)*written = out_size;
	while (!m_hfile->is_eof() && out_size > 0)
	{
		readed = m_hfile->read(200, _buffer) - cur;
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
	if (m_hfile.is_empty() || m_hfile->is_eof() || buff.is_empty())
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
	while (!m_hfile->is_eof() && out_size > 0 && !endl)
	{
		readed = m_hfile->read(200, _buffer) - cur;
		
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
