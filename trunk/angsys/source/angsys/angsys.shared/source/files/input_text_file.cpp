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
	: base()
{
}

input_text_file::input_text_file(path_view_t path)
	: base()
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

bool input_text_file::map(function<bool(ibuffer_view_t)> func, wsize sz, file_offset_t offset)
{
	ibuffer_t buff =  file::map(min(size(), sz), min(size(), offset));
	if (buff.is_empty())
		return false;
	func(buff.get());
	unmap(buff, min(size(), sz));
	return true;
}

//bool input_text_file::map(function<bool(text::istring_view_t)> func, wsize sz, file_offset_t offset)
//{
//	ibuffer_t buff = file::map(min(size(), sz), min(size(), offset));
//	if (buff.is_empty())
//		return false;
//	func(text::istring_factory::get_factory(format())->create_wrapper((ibuffer_view*)buff.get()));
//	unmap(buff, min(size(), sz));
//	return true;
//}

bool input_text_file::read(function<bool(streams::itext_input_stream_t)> func)
{
	return func(this);
}

uint input_text_file::seek(text::raw_cstr_t format)
{
	if (m_hfile.is_empty())
		throw_exception(except_code::invalid_access);

	text::iencoder_t ie = text::iencoder::get_encoder(m_hfile->format());
	text::iencoder_t oe = text::iencoder::get_encoder(format.encoding());
	//text::iformat_parser_t ip = text::iformat_parser::get_parser(m_hfile->format());
	//text::iformat_parser_t op = text::iformat_parser::get_parser(format.encoding());

	wsize format_idx = 0, buffer_idx = 0, temp,
		ics = ie->char_type().size(),
		ocs = oe->char_type().size();
	scope_array<byte> buffer(2001 * ics);

	file_offset_t cursor = m_hfile->cursor();
	file_offset_t forward = 0;
	int arg_idx = 0;

	wsize readed = 0;
	wsize size1 = format.size() / ocs;
	wsize size2 = 0;
	enum {
		detect = 0,
		reading_spaces,
		reading_endl,
	}state;
	state = detect;
	while (!m_hfile->is_eof() && format_idx < size1)
	{
		if (buffer_idx >= size2) //reading data from file
		{
			forward += readed;
			size2 = (readed = (wsize)(m_hfile->read(200 * ics, buffer.get()) - cursor)) / ics;
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

		temp = buffer_idx;
		if (oe->to_char32(format.ptr(), format_idx) != ie->to_char32(buffer.get(), buffer_idx)) //diferent text
		{
			forward += temp * ics;
			m_hfile->cursor(cursor + forward, stream_reference::begin);
			return format_idx;
		}
	}

	forward += buffer_idx * ics;
	m_hfile->cursor(cursor + forward, stream_reference::begin);

	return format_idx;
}


#define FUNCX_TEXT_READ(A0) \
if (type.type_id() == ang::type_of<A0>().type_id()) { \
	*(A0*)ptr = (A0)parser->to_value<A0>(text::raw_cstr(buffer.data(), readed, e), i); \
	m_hfile->cursor(current + i * cs, stream_reference::begin); \
	return i * cs; \
}

#define TEXT_READ_SWITCH(...) { ANG_EXPAND(APPLY_FUNCX_N(FUNCX_TEXT_READ, ELSE_SEPARATOR,##__VA_ARGS__)) }


wsize input_text_file::read(pointer ptr, const rtti_t& type)
{
	if (m_hfile.is_empty())
		throw_exception(except_code::invalid_access);
	text::encoding_t e = format();
	text::iparser_t parser = text::iparser::get_parser(e);
	wsize cs = text::encoder<text::encoding::auto_detect>::char_size_by_encoding(e);
	scope_array<byte> buffer(2001 * cs);
	auto current = m_hfile->cursor();
	wsize i = 0, readed = (wsize)(m_hfile->read(buffer.size(), buffer.data()) - current);

	TEXT_READ_SWITCH(
		char,
		byte,
		wchar_t,
		char16_t,
		char32_t,
		short,
		ushort,
		int,
		uint,
		long,
		ulong,
		long64,
		ulong64,
		float,
		double
	);
	return 0;
}

uint input_text_file::read_format(text::raw_cstr_t format, var_args_t& args)
{
	if (m_hfile.is_empty())
		throw_exception(except_code::invalid_access);

	text::iencoder_t ie = text::iencoder::get_encoder(m_hfile->format());
	text::iencoder_t oe = text::iencoder::get_encoder(format.encoding());
	text::iparser_t ip = text::iparser::get_parser(m_hfile->format());
	text::iparser_t op = text::iparser::get_parser(format.encoding());

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
			size2 = (readed = (wsize)(m_hfile->read(200 * ics, buffer.get()) - cursor)) / ics;
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
				tff.value = op->parse(format, format_idx, arg_idx).format_flags();
				switch (tff.value)
				{
				case text::text_format::target::signed_:
					val = (int) ip->to_signed(text::raw_cstr(buffer.get(), size2, ie->format()), buffer_idx, true, tff.base);
					break;
				case text::text_format::target::unsigned_:
					val = (uint)ip->to_unsigned(text::raw_cstr(buffer.get(), size2, ie->format()), buffer_idx, true, tff.base);
					break;
				case text::text_format::target::float_:
					val = (float)ip->to_floating(text::raw_cstr(buffer.get(), size2, ie->format()), buffer_idx, true, tff.exponent);
					break;
				case 0: //no format readed
					temp = buffer_idx;
					if (oe->to_char32(format.ptr(), format_idx) != ie->to_char32(buffer.get(), buffer_idx)) //diferent text
					{
						forward += temp * ics;
						m_hfile->cursor(cursor + forward, stream_reference::begin);
						return (uint)args->counter();
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
			return (uint)args->counter();
		}
	}

	forward += buffer_idx * ics;
	m_hfile->cursor(cursor + forward, stream_reference::begin);

	return (uint)args->counter();
}

wsize input_text_file::read(text::istring_t str, wsize sz, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || str.is_empty() || sz == 0)
		return 0;

	text::iencoder_t encoder = text::iencoder::get_encoder(m_hfile->format());
	wsize cs = encoder->char_type().size();

	windex eos = str->length();
	wsize readed, total = 0;
	auto cur = cursor();
	scope_array<byte> buff(100 * cs);
	
	while (!m_hfile->is_eof() && sz > 0)
	{
		readed = (wsize)(m_hfile->read(99 * cs, buff.data()) - cur);
		encoder->set_eos(buff.data(), readed / cs);
		str->concat(text::raw_str(buff.data(), min(readed, sz * cs), encoder->format()));
		sz -= min(readed / cs, sz);
		total += min(readed, sz * cs);
	}
	if (written)*written = str->length() - eos;
	cursor(cur + total, stream_reference::begin);
	return total;
}

wsize input_text_file::read(text::unknown_str_t buff, wsize sz, text::encoding_t e, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || buff == null || sz == 0)
		return 0;
	text::iencoder_t encoder = text::iencoder::get_encoder(e);

	wsize cs = encoder->char_type().size();
	pointer ptr = buff;
	wsize out_size = (sz / cs) - 1;
	wsize readed, total = 0;
	stack_array<byte, 200> _buffer;
	auto cur = cursor();
	if (written)*written = out_size;
	while (!m_hfile->is_eof() && out_size > 0)
	{
		readed = (wsize)(m_hfile->read(200, _buffer.data()) - cur);
		auto str = encoder->convert(text::raw_str(ptr, sz, text::encoding::auto_detect), text::raw_cstr(_buffer.data(), readed, format()), true);
		ptr = &((byte*)str.ptr())[str.size()];
		out_size -= str.size() / cs;
		total += str.size();
	}
	if (written)*written -= out_size;
	cursor(cur + total, stream_reference::begin);
	return total;
}


wsize input_text_file::read_line(text::istring_t str, array_view<const char32_t> token, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || str.is_empty())
		return 0;

	text::encoding_t e = format();
	text::iencoder_t encoder = text::iencoder::get_encoder(e);
	wsize cs = encoder->char_type().size();
	str->clear();
	wsize readed, total = 0;
	auto cur = cursor();
	scope_array<byte> buff(100 * cs);

	bool endl = false;
	while (!m_hfile->is_eof() && !endl)
	{
		wsize beg = 0, end = 0, temp = 0;
		char32 ch = 0;
		readed = (wsize)(m_hfile->read(buff.size() - cs, buff.data()) - cur);
		
		while ((ch = encoder->to_char32(buff.data(), temp)) && (ch == U' ' || ch == U'\t') && beg < (readed / cs)) {
			beg = temp;
		}

		end = encoder->find_any(text::raw_cstr(buff.data(), readed, text::encoding::auto_detect), beg, token);
		if (end != invalid_index)
			endl = true;
		else
			end = readed / cs;
		str->concat(text::raw_str(buff.data() + (beg * cs), (end - beg) * cs, e));

		if (endl) //removing all endline tokens
		{
			temp = end;
			while ((ch = encoder->to_char32(buff.data(), temp)) && beg < (readed / cs)) {
				bool b = false;
				for (char32 ch2 : token) {
					if (ch == ch2)
						b = true;
				}
				if (!b)break;
				end = temp;
			}
		}
		total += end * cs;	
	}
	if (written)*written = str->length();
	cursor(cur + total, stream_reference::begin);
	return total;
}

wsize input_text_file::read_line(text::unknown_str_t buff, wsize sz, text::encoding_t e, array_view<const char32_t> token, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || buff == null || sz == 0)
		return 0;
	text::iencoder_t encoder = text::iencoder::get_encoder(e);
	text::iencoder_t my_encoder = text::iencoder::get_encoder(format());
	wsize mycs = my_encoder->char_type().size();
	wsize cs = encoder->char_type().size();
	pointer ptr = buff;
	wsize out_size = (sz / cs) - 1;
	wsize readed, total = 0;
	scope_array<byte> _buffer(100 * mycs);
	auto cur = cursor();
	bool endl = false;
	if (written)*written = out_size;
	while (!m_hfile->is_eof() && out_size > 0 && !endl)
	{
		wsize beg = 0, end = 0, temp = 0;
		char32 ch = 0;
		readed = (wsize)(m_hfile->read(_buffer.size() - mycs, _buffer.data()) - cur);
	
		while (ch = my_encoder->to_char32(_buffer.data(), beg) && (ch == U' ' || ch == U'\t') && beg < (readed / mycs)) {
			beg = temp;
		}

		end = my_encoder->find_any(text::raw_cstr(_buffer.data(), readed, text::encoding::auto_detect), beg, token);

		if (end != invalid_index)
			endl = true;
		else
			end = readed / mycs;

		auto str = encoder->convert(text::raw_str(ptr, out_size * cs, e), text::raw_cstr(_buffer.data() + (beg * mycs), (end - beg) * mycs, format()), true);

		ptr = &((byte*)str.ptr())[str.size()];
		out_size -= str.size() / cs;
		total += end * mycs;

	}
	if (written)*written -= out_size;
	cursor(cur + total, stream_reference::begin);
	return total;
}

