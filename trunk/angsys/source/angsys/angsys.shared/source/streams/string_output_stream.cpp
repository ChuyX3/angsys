#include "pch.h"
#include <ang/streams.h>

using namespace ang;
using namespace ang::streams;

string_output_stream::string_output_stream(text::istring_t str)
	: m_cursor(0)
	, m_string(ang::move(str))
	, m_parser(null)
{
	if (!m_string.is_empty())
		//throw_exception(except_code::invalid_param);
		m_parser = text::iparser::get_parser(m_string->encoding());
}

string_output_stream::~string_output_stream()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::streams::string_output_stream);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::streams::string_output_stream, object, itext_output_stream);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::streams::string_output_stream, object, itext_output_stream, ioutput_stream);

void string_output_stream::attach(text::istring_t str)
{
	m_cursor = 0;
	m_string = str;
	if (m_string.is_empty()) 
		m_parser = null;
	else
		m_parser = text::iparser::get_parser(m_string->encoding());
}

text::istring_t string_output_stream::data()const
{
	return m_string;
}

void string_output_stream::clear()
{
	m_string = null;
}

text::encoding_t string_output_stream::format()const
{
	return m_string.is_empty() ? text::encoding::auto_detect : m_string->encoding().get();
}

stream_index_t string_output_stream::cursor()const
{
	return m_cursor;
}

stream_size_t string_output_stream::size()const
{
	return m_string.is_empty() ? 0 : m_string->length();
}

bool string_output_stream::cursor(stream_index_t offset, stream_reference_t ref)
{
	switch (ref)
	{
	case stream_reference::begin:
		m_cursor = (wsize)min(max(offset, 0), size());
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

bool string_output_stream::command(special_command_t cmd)
{
	switch (cmd)
	{
	default://TODO
		return false;
	}
}

wsize string_output_stream::write(cstr_t str)
{
	if (m_string.is_empty())
		return 0;
	auto l = m_string->length();
	m_string->concat(str);
	m_cursor += m_string->length() - l;
	return  m_string->length() - l;
}

wsize string_output_stream::write_line(cstr_t cstr)
{
	if (m_string.is_empty())
		return 0;
	auto l = m_string->length();
	m_string->concat(cstr);
	m_cursor += m_string->length() - l;
	return  m_string->length() - l;
}

wsize string_output_stream::write_format(cstr_t format, var_args_t args)
{
	if (m_string.is_empty())
		return 0;
	auto l = m_string->length();
	m_string->concat_format(format, args);
	m_cursor += m_string->length() - l;
	return  m_string->length() - l;
}

