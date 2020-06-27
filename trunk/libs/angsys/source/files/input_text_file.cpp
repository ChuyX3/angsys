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
//#include "format_parser.h"

using namespace ang;
using namespace ang::streams;
using namespace ang::core::files;

input_text_file::input_text_file()
{
}

input_text_file::input_text_file(cstr_t path)
{
	open(path);
}

input_text_file::~input_text_file()
{

}

bool input_text_file::open(cstr_t path)
{
	if (is_valid())
		return false;
	if (!create(path, open_flags_t(open_flags::access_in) + open_flags::open_exist + open_flags::format_text))
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
	
	file_offset_t _sz = sz == invalid_index ? size() : min(size(), file_offset_t(sz));
	file_offset_t _off = offset == invalid_index ? size() : min(size(), file_offset_t(offset));
	ibuffer_t buff =  file::map(_sz, _off);
	if (buff.is_empty())
		return false;
	auto result = func(buff.get());
	unmap(buff, _sz);
	return result;
}


static text::encoding_t  discard_bom(pointer bom, text::encoding_t e, windex& idx)
{
	if (bom == null)
		return text::encoding::none;

	switch (e.get())
	{
	case text::encoding::auto_detect:
		if (text::load_bom<text::encoding::utf8>(bom) > 0)
		{
			idx += 3;
			return text::encoding::utf8;
		}
		else if (text::load_bom<text::encoding::utf32_le>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf32_le>();
		}
		else  if (text::load_bom<text::encoding::utf32_be>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf32_be>();
		}
		else if (text::load_bom<text::encoding::utf16_le>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_le>();
		}
		else  if (text::load_bom<text::encoding::utf16_be>(bom) > 0)
		{
			idx += 1;
			return text::native_encoding<text::encoding::utf16_be>();
		}
		return text::encoding::ascii;
	case text::encoding::utf8:
		if (text::load_bom<text::encoding::utf8>(bom) > 0)
			idx += 3;
		return text::encoding::utf8;
	case text::encoding::utf16_le:
		if(text::load_bom<text::encoding::utf16_le>(bom) > 0)
			idx += 1;
		return text::native_encoding<text::encoding::utf16_le>();
	case text::encoding::utf16_be:
		if (text::load_bom<text::encoding::utf16_be>(bom) > 0)
			idx += 1;
		return text::native_encoding<text::encoding::utf16_be>();
	case text::encoding::utf32_le:
		if (text::load_bom<text::encoding::utf32_le>(bom) > 0)
			idx += 1;
		return text::native_encoding<text::encoding::utf32_le>();
	case text::encoding::utf32_be: 
		if (text::load_bom<text::encoding::utf32_be>(bom) > 0)
			idx += 1;
		return text::native_encoding<text::encoding::utf32_be>();
	default:
		return e;
	}
}

template<text::encoding E>
string from_buffer(ibuffer_t buff, wsize i) {
	using type = typename text::char_type_by_encoding<E>::char_type;
	text::basic_string<E> str = new text::basic_string_buffer<E>();
	str_view<const type, E> code(((const type*)buff->buffer_ptr()) + i, (buff->buffer_size() / size_of<type>()) - i);
	str->attach(code);
	return str.get();
}

bool input_text_file::map2(function<bool(string)> func, wsize sz, file_offset_t offset)
{
	file_offset_t _sz = sz == invalid_index ? size() : min(size(), file_offset_t(sz));
	file_offset_t _off = offset == invalid_index ? size() : min(size(), file_offset_t(offset));
	ibuffer_t buff = file::map(_sz, _off);
	if (buff.is_empty())
		return false;

	auto e = buff->encoding();
	wsize i = 0;
	string code;
	e = discard_bom(buff->buffer_ptr(), e, i);
	switch (e)
	{
	case text::encoding::ascii:
		code = from_buffer<text::encoding::ascii>(buff, i);
		break;
	case text::encoding::unicode:
		code = from_buffer<text::encoding::unicode>(buff, i);
		break;
	case text::encoding::utf8:
		code = from_buffer<text::encoding::utf8>(buff, i);
		break;
	case text::encoding::utf16_le:
		code = from_buffer<text::encoding::utf16_le>(buff, i);
		break;
	case text::encoding::utf16_be:
		code = from_buffer<text::encoding::utf16_be>(buff, i);
		break;
	case text::encoding::utf32_le:
		code = from_buffer<text::encoding::utf32_le>(buff, i);
		break;
	case text::encoding::utf32_be:
		code = from_buffer<text::encoding::utf32_be>(buff, i);
		break;
	default:
		break;
	}
	auto result = func(code.get());
	unmap(buff, _sz);
	return result;
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

wsize input_text_file::seek(cstr_t format)
{
	if (m_hfile.is_empty())
		throw_exception(error_code::invalid_access);

	using encoder = text::encoder;

	wsize format_idx = 0, buffer_idx = 0, temp,
		ics = encoder::char_size_by_encoding(m_hfile->format()),
		ocs = encoder::char_size_by_encoding(m_hfile->format());
	array<byte> buffer_(2001 * ics);
	str_t buffer(buffer_.get(), buffer_.size(), m_hfile->format());

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
			size2 = (readed = (wsize)(m_hfile->read(200 * ics, buffer.ptr()) - (cursor + forward))) / ics;
			encoder::set_eos(buffer, size2);
			buffer_idx = 0;
		}

		if (state == reading_spaces ||
			encoder::to_char32(format, format_idx, false) == U' ' ||
			encoder::to_char32(format, format_idx, false) == U'\t') //character '\t' or ' ' means "read all spaces"
		{
			while (encoder::to_char32(format, format_idx, false) == U' '
				|| encoder::to_char32(format, format_idx, false) == U'\t') //read all spaces
				format_idx++;

			while (encoder::to_char32(buffer, buffer_idx, false) == U' '
				|| encoder::to_char32(buffer, buffer_idx, false) == U'\t') //read all spaces
				buffer_idx++;

			if (encoder::to_char32(buffer, buffer_idx, false) == 0) // if it is end of string then
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
			encoder::to_char32(format, format_idx, false) == U'\r' ||
			encoder::to_char32(format, format_idx, false) == U'\n') //character '\t' or ' ' means "read all spaces"
		{
			while (encoder::to_char32(format, format_idx, false) == U'\r'
				|| encoder::to_char32(format, format_idx, false) == U'\n') //read all spaces
				format_idx++;

			while (encoder::to_char32(buffer, buffer_idx, false) == U'\r'
				|| encoder::to_char32(buffer, buffer_idx, false) == U'\n') //read all spaces
				buffer_idx++;

			if (encoder::to_char32(buffer, buffer_idx, false) == 0) // if it is end of string then
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
		if (encoder::to_char32(format, format_idx) != encoder::to_char32(buffer, buffer_idx)) //diferent text
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
	*(A0*)ptr = (A0)text::parser::to_value<A0>(cstr_t(buffer.data(), readed, e), i); \
	m_hfile->cursor(current + i * cs, stream_reference::begin); \
	return i * cs; \
}

#define TEXT_READ_SWITCH(...) { ANG_EXPAND(APPLY_FUNCX_N(FUNCX_TEXT_READ, ELSE_SEPARATOR,##__VA_ARGS__)) }


wsize input_text_file::read(pointer ptr, const rtti_t& type)
{
	if (m_hfile.is_empty())
		throw_exception(error_code::invalid_access);
	text::encoding_t e = format();
	wsize cs = text::encoder::char_size_by_encoding(e);
	array<byte> buffer(2001 * cs);
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

wsize input_text_file::read_format(cstr_t format, var_args_t& args)
{
	if (m_hfile.is_empty())
		throw_exception(error_code::invalid_access);
	using encoder = text::encoder;

	wsize format_idx = 0, buffer_idx = 0, temp,
		ics = encoder::char_size_by_encoding(m_hfile->format()),
		ocs = encoder::char_size_by_encoding(format.encoding());
	array<byte> buffer_(2001 * ics);
	str_t buffer(buffer_.get(), buffer_.size(), m_hfile->format());
	
	file_offset_t cursor = m_hfile->cursor();
	file_offset_t forward = 0;
	int arg_idx = 0;
	text::text_format_flags_t tff;

//	args.clear();

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
			size2 = (readed = (wsize)(m_hfile->read(200 * ics, buffer.ptr()) - (cursor + forward))) / ics;
			encoder::set_eos(buffer, size2);
			buffer_idx = 0;
		}
	
		if (state == reading_spaces ||
			encoder::to_char32(format, format_idx, false) == U' ' ||
			encoder::to_char32(format, format_idx, false) == U'\t') //character '\t' or ' ' means "read all spaces"
		{
			while (encoder::to_char32(format, format_idx, false) == U' '
				|| encoder::to_char32(format, format_idx, false) == U'\t') //read all spaces
				format_idx++;

			while (encoder::to_char32(buffer, buffer_idx, false) == U' '
				|| encoder::to_char32(buffer, buffer_idx, false) == U'\t') //read all spaces
				buffer_idx++;

			if (encoder::to_char32(buffer, buffer_idx, false) == 0) // if it is end of string then
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
			encoder::to_char32(format, format_idx, false) == U'\r' ||
			encoder::to_char32(format, format_idx, false) == U'\n') //character '\t' or ' ' means "read all spaces"
		{
			while (encoder::to_char32(format, format_idx, false) == U'\r'
				|| encoder::to_char32(format, format_idx, false) == U'\n') //read all spaces
				format_idx++;

			while (encoder::to_char32(buffer, buffer_idx, false) == U'\r'
				|| encoder::to_char32(buffer, buffer_idx, false) == U'\n') //read all spaces
				buffer_idx++;

			if (encoder::to_char32(buffer, buffer_idx, false) == 0) // if it is end of string then
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
			encoder::to_char32(format, format_idx, false) == U'{') //parsing data
		{

			if (state != formating && (size2 - buffer_idx) < 20)
				// the data to parse is less that 20 characters, witch means that it can generate a parsing error 
			{
				state = formating;
			}
			else
			{
				var val;
				tff.value = text::parser::parse(format, format_idx, arg_idx).format_flags();
				switch (tff.target)
				{
				case text::text_format::target::signed_:
					val = (int) text::parser::to_signed(cstr_t(buffer.ptr(), size2, buffer.encoding()), buffer_idx, true, tff.base);
					break;
				case text::text_format::target::unsigned_:
					val = (uint)text::parser::to_unsigned(cstr_t(buffer.ptr(), size2, buffer.encoding()), buffer_idx, true, tff.base);
					break;
				case text::text_format::target::float_:
					val = (float)text::parser::to_floating(cstr_t(buffer.ptr(), size2, buffer.encoding()), buffer_idx, true, tff.exponent);
					break;
				case 0: //no format readed
					temp = buffer_idx;
					if (encoder::to_char32(format, format_idx) != encoder::to_char32(buffer, buffer_idx)) //diferent text
					{
						forward += temp * ics;
						m_hfile->cursor(cursor + forward, stream_reference::begin);
						return (uint)args.size();
					}
				default:
					break;
				}
				if (arg_idx > (int)args.size())
					args.capacity(arg_idx + 1, true);
				if (arg_idx < 0)
					args += val;
				else
					args[arg_idx] = val;
			}
			continue;
		}

		temp = buffer_idx;
		if (encoder::to_char32(format, format_idx) != encoder::to_char32(buffer, buffer_idx)) //diferent text
		{
			forward += temp * ics;
			m_hfile->cursor(cursor + forward, stream_reference::begin);
			return (uint)args.size();
		}
	}

	forward += buffer_idx * ics;
	m_hfile->cursor(cursor + forward, stream_reference::begin);

	return (uint)args.size();
}

wsize input_text_file::read(string str, wsize sz, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || str == null || sz == 0)
		return 0;

	using encoder = text::encoder;
	wsize cs = encoder::char_size_by_encoding(m_hfile->format());

	windex eos = str->length();
	wsize readed, total = 0;
	auto cur = cursor();
	array<byte> buffer_(100 * cs);
	str_t buffer(buffer_.get(), buffer_.size(), m_hfile->format());
	
	while (!m_hfile->is_eof() && sz > 0)
	{
		readed = (wsize)(m_hfile->read(99 * cs, buffer.ptr()) - (cur + total));
		encoder::set_eos(buffer, readed / cs);
		str->append(cstr_t(buffer.ptr(), min(readed, sz * cs), buffer.encoding()));
		sz -= min(readed / cs, sz);
		total += min(readed, sz * cs);
	}
	if (written)*written = str->length() - eos;
	cursor(cur + total, stream_reference::begin);
	return total;
}

wsize input_text_file::read(pointer buff, wsize sz, text::encoding_t e, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || buff == null || sz == 0)
		return 0;
	using encoder = text::encoder;

	auto my_e = m_hfile->format();
	wsize cs = encoder::char_size_by_encoding(e);
	pointer ptr = buff;
	wsize out_size = (sz / cs) - 1;
	wsize readed, total = 0;
	stack_array<byte, 200> _buffer;
	auto cur = cursor();
	if (written)*written = out_size;
	while (!m_hfile->is_eof() && out_size > 0)
	{
		readed = (wsize)(m_hfile->read(200, _buffer.data()) - (cur + total));
		auto str = encoder::convert(str_t(ptr, sz, my_e), cstr_t(_buffer.data(), readed, format()), true, sz, readed);
		ptr = &((byte*)str.ptr())[str.size()];
		out_size -= str.size() / cs;
		total += str.size();
	}
	if (written)*written -= out_size;
	cursor(cur + total, stream_reference::begin);
	return total;
}

wsize input_text_file::read_line(string str, array_view<const char32_t> token, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || str == null)
		return 0;

	text::encoding_t e = format();
	using encoder = text::encoder;
	wsize cs = encoder::char_size_by_encoding(e);
	str->clear();
	wsize readed, total = 0;
	auto cur = cursor();
	array<byte> buffer_(100 * cs);
	str_t buffer(buffer_.get(), buffer_.size(), m_hfile->format());

	bool endl = false;
	while (!m_hfile->is_eof() && !endl)
	{
		wsize beg = 0, end = 0, temp = 0;
		char32 ch = 0;
		readed = (wsize)(m_hfile->read(buffer.size() - cs, buffer.ptr()) - (cur + total));
		
	//	while ((ch = encoder::to_char32(buffer, temp)) && (ch == U' ' || ch == U'\t') && beg < (readed / cs)) {
	//		beg = temp;
	//	}

		end = encoder::find_any(cstr_t(buffer.ptr(), readed, buffer.encoding()), beg, token);
		if (end != invalid_index)
			endl = true;
		else
			end = readed / cs;

		str->append(cstr_t((byte*)buffer.ptr() + (beg * cs), (end - beg) * cs, e));

		if (endl) //removing all endline tokens
		{
			list<char32_t> t = token;
			temp = end;
			while ((ch = encoder::to_char32(buffer, temp)) && beg < (readed / cs)) {
				bool b = false;
				for (auto it = t.begin(); it != t.end(); it++) {
					if (ch == *it)
						b = true;
					t.remove(it);
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

wsize input_text_file::read_line(pointer buff, wsize sz, text::encoding_t e, array_view<const char32_t> token, wsize* written)
{
	if (m_hfile.is_empty() || m_hfile->is_eof() || buff == null || sz == 0)
		return 0;	 
	using encoder = text::encoder;
	wsize mycs = encoder::char_size_by_encoding(e);
	wsize cs = encoder::char_size_by_encoding(format());
	pointer ptr = buff;
	wsize out_size = (sz / cs) - 1;
	wsize readed, total = 0;
	array<byte> buffer_(100 * mycs);
	str_t buffer(buffer_.get(), buffer_.size(), m_hfile->format());

	auto cur = cursor();
	bool endl = false;
	if (written)*written = out_size;
	while (!m_hfile->is_eof() && out_size > 0 && !endl)
	{
		wsize beg = 0, end = 0, temp = 0;
		char32 ch = 0;
		readed = (wsize)(m_hfile->read(buffer.size() - mycs, buffer.ptr()) - (cur + total));
	
		/*while (ch = encoder::to_char32(buffer, beg) && (ch == U' ' || ch == U'\t') && beg < (readed / mycs)) {
			beg = temp;
		}*/

		end = encoder::find_any(cstr_t(buffer.ptr(), readed, buffer.encoding()), beg, token);

		if (end != invalid_index)
			endl = true;
		else
			end = readed / mycs;

		auto str = encoder::convert(str_t(ptr, out_size * cs, e), cstr_t((byte*)buffer.ptr() + (beg * mycs), (end - beg) * mycs, format()), true);

		ptr = &((byte*)str.ptr())[str.size()];
		out_size -= str.size() / cs;
	
		if (endl) //removing all endline tokens
		{
			list<char32_t> t = token;
			temp = end;
			while ((ch = encoder::to_char32(buffer, temp)) && beg < (readed / cs)) {
				bool b = false;
				for (auto it = t.begin(); it != t.end(); it++) {
					if (ch == *it)
						b = true;
					t.remove(it);
				}
				if (!b)break;
				end = temp;
			}
		}

		total += end * mycs;
	}
	if (written)*written -= out_size;
	cursor(cur + total, stream_reference::begin);
	return total;
}

