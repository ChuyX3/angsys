#include "pch.h"
#include <ang/streams.h>

using namespace ang;
using namespace ang::streams;

string_input_stream::string_input_stream(text::istring_view_t str)
	: m_cursor(0)
	, m_string(ang::move(str))
	, m_parser(null)
{
	if (!m_string.is_empty())
		//throw_exception(except_code::invalid_param);
		m_parser = text::iparser::get_parser(m_string->encoding());
}

string_input_stream::~string_input_stream()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::streams::string_input_stream);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::streams::string_input_stream, object, itext_input_stream);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::streams::string_input_stream, object, itext_input_stream, iinput_stream);

void string_input_stream::attach(text::istring_view_t str)
{
	m_cursor = 0;
	m_string = str;
	if (m_string.is_empty()) 
		m_parser = null;
	else
		m_parser = text::iparser::get_parser(m_string->encoding());
}

text::istring_view_t string_input_stream::data()const
{
	return m_string;
}

void string_input_stream::clear()
{
	m_string = null;
}

text::encoding_t string_input_stream::format()const
{
	return m_string.is_empty() ? text::encoding::auto_detect : m_string->encoding().get();
}

stream_index_t string_input_stream::cursor()const
{
	return m_cursor;
}

stream_size_t string_input_stream::size()const
{
	return m_string.is_empty() ? 0 : m_string->length();
}

bool string_input_stream::is_eos()const
{
	return m_string.is_empty() ? true : m_string->length() == m_cursor;
}

bool string_input_stream::cursor(stream_index_t offset, stream_reference_t ref)
{
	switch (ref)
	{
	case stream_reference::begin:
		m_cursor = (wsize) min(max(offset, 0), size());
		break;
	case stream_reference::current:
		m_cursor = (wsize)min(max(m_cursor + offset, 0), size());
		break;
	case stream_reference::end:
		m_cursor = (wsize)min(max(size() + offset, 0), size());
		break;
	}
	return true;
}

wsize string_input_stream::seek(cstr_t format)
{
	wsize c = m_cursor;
	m_parser->seek(m_string, m_cursor, format);
	return m_cursor - c;
}

#define FUNCX_TEXT_READ(A0) \
	if (type.type_id() == ang::type_of<A0>().type_id()) { \
		*(A0*)ptr = (A0)m_parser->to_value<A0>(data, i); \
		m_cursor += i; \
		return i * cs; \
	}

#define TEXT_READ_SWITCH(...) { ANG_EXPAND(APPLY_FUNCX_N(FUNCX_TEXT_READ, ELSE_SEPARATOR,##__VA_ARGS__)) }


wsize string_input_stream::read(pointer ptr, ang::rtti_t const& type)
{
	if (m_string.is_empty())
		return 0;
	wsize cs = text::encoder<text::encoding::auto_detect>::char_size_by_encoding(format());
	wsize i = 0;
	cstr_t data = m_string;
	data = cstr_t((byte*)data.ptr() + (m_cursor * cs), data.size() - (m_cursor * cs), data.encoding());

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

wsize string_input_stream::read_format(cstr_t format, var_args_t& args)
{
	//TODO:
	return 0;
}

wsize string_input_stream::read(text::istring_t out, wsize sz, wsize*written)
{
	if (out.is_empty())
		return 0;
	
	text::encoding_t e = format();
	wsize cs = text::encoder<text::encoding::auto_detect>::char_size_by_encoding(e);
	cstr_t cstr = m_string;
	wsize size = min(sz * cs, cstr.size() - m_cursor * cs);
	
	out->copy(cstr_t((byte*)cstr.ptr() + (m_cursor * cs), size, e));
	m_cursor += size / cs;
	if (written)*written = size;
	return size;
}

wsize string_input_stream::read(text::unknown_str_t out, wsize sz, text::encoding_t e, wsize*written)
{
	if (out == null || sz == 0)
		return 0;

}

wsize string_input_stream::read_line(text::istring_t, array_view<const char32_t>, wsize*written)
{
	return 0;
}

wsize string_input_stream::read_line(text::unknown_str_t, wsize, text::encoding_t, array_view<const char32_t>, wsize*written)
{
	return 0;
}





