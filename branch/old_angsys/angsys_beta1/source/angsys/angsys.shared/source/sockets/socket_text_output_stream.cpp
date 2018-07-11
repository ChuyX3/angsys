#include "pch.h"
#include "ang/core/sockets.h"
#include "socket_manager.h"


using namespace ang;
using namespace ang::streams;

socket_text_output_stream::socket_text_output_stream(core::connectivity::socket_handle_t socket)
	: _socket(socket)
	, _format(text::encoding::unknown)
	, _is_formating_enabled(false)
	, char_format("{char:}")
	, string_format("{string:n}")
	, signed_format("{signed:}")
	, unsigned_format("{unsigned:}")
	, floating_format("{floating:n.4}")
{

}

socket_text_output_stream::~socket_text_output_stream()
{
}

ANG_IMPLEMENT_CLASSNAME(ang::streams::socket_text_output_stream);
ANG_IMPLEMENT_OBJECTNAME(ang::streams::socket_text_output_stream);

bool socket_text_output_stream::is_kind_of(type_name_t type)const throw()
{
	return (type == type_name<socket_text_output_stream>())
		|| object::is_kind_of(type)
		|| (type == type_name<itext_output_stream>());
}

bool socket_text_output_stream::is_child_of(type_name_t type)throw()
{
	return (type == type_name<socket_text_output_stream>())
		|| object::is_child_of(type)
		|| (type == type_name<itext_output_stream>());
}

bool socket_text_output_stream::query_object(type_name_t type, unknown_ptr_t out) throw()
{
	if (out == null)
		return false;
	if (type == type_name<socket_text_output_stream>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<itext_output_stream>())
	{
		*out = static_cast<itext_output_stream*>(this);
		return true;
	}
	return false;
}

stream_index_t socket_text_output_stream::position()const
{
	return -1;
}

stream_size_t socket_text_output_stream::stream_size()const
{
	return -1;
}

bool socket_text_output_stream::command(special_command_t command)
{
	switch (command)
	{
	case special_command::end_line:
		write('\n');
		break;
	case special_command::space:
		write(' ');
		break;
	case special_command::tab:
		write('\t');
		break;
	default:return false;
	}
	return true;
}

void socket_text_output_stream::enable_text_format(bool value)
{
	_is_formating_enabled = value;
}

bool socket_text_output_stream::is_text_format_enabled()const
{
	return _is_formating_enabled;
}

void socket_text_output_stream::text_format(cstr_t format)
{
	text_format(text::text_format(format));
}

void socket_text_output_stream::text_format(text::text_format_t format)
{
	switch (format.format_target())
	{
	case text::text_format::character:
		char_format = format;
		break;
	case text::text_format::text:
		string_format = format;
		break;
	case text::text_format::signed_interger:
		signed_format = format;
		break;
	case text::text_format::usigned_interger:
		unsigned_format = format;
		break;
	case text::text_format::floating:
		floating_format = format;
		break;
	default:break;
	}
}

bool socket_text_output_stream::move_to(stream_index_t size, stream_reference_t ref)
{
	return false;
}

text::encoding_t socket_text_output_stream::text_encoding()const
{
	return _format;
}

bool socket_text_output_stream::write(pointer ptr, wsize sz)
{
	send(_socket, (const char*)ptr, sz, 0);
	return true;
}

bool socket_text_output_stream::write(wchar value)
{
	switch (_format)
	{
	case text::encoding::ascii: {
		char c = (char)(byte)value;
		write(&c, 1);
	}break;
	case text::encoding::unicode: {
		write(&value, 1);
	}break;
	case text::encoding::utf_8: {
		mbyte c = value;
		write(&c.value, c.size());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(char value)
{
	switch (_format)
	{
	case text::encoding::ascii: {
		write(&value, 1);
	}break;
	case text::encoding::unicode: {
		wchar c = (wchar)(byte)value;
		write(&c, 1);
	}break;
	case text::encoding::utf_8: {
		mbyte c = value;
		write(&c.value, c.size());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(byte value)
{
	switch (_format)
	{
	case text::encoding::ascii: {
		write(&value, 1);
	}break;
	case text::encoding::unicode: {
		wchar c = (wchar)value;
		write(&c, 1);
	}break;
	case text::encoding::utf_8: {
		mbyte c = (wchar_t)value;
		write(&c.value, c.size());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(short value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(interger::to_string(value, signed_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(interger::to_wstring(value, signed_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(ushort value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(uinterger::to_string(value, unsigned_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(uinterger::to_wstring(value, unsigned_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(int value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(interger::to_string(value, signed_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(interger::to_wstring(value, signed_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(uint value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(uinterger::to_string(value, unsigned_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(uinterger::to_wstring(value, unsigned_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(long value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(interger::to_string(value, signed_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(interger::to_wstring(value, signed_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(ulong value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(uinterger::to_string(value, unsigned_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(uinterger::to_wstring(value, unsigned_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(long64 value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(interger64::to_string(value, signed_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(interger64::to_wstring(value, signed_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(ulong64 value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(uinterger64::to_string(value, unsigned_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(uinterger64::to_wstring(value, unsigned_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(float value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(floating::to_string(value, floating_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(floating::to_wstring(value, floating_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(double value)
{
	switch (_format)
	{
	case text::encoding::utf_8:
	case text::encoding::ascii: {
		string str = ang::move(floating64::to_string(value, floating_format));
		write((pointer)str->cstr(), str->length());
	}break;
	case text::encoding::unicode: {
		wstring str = ang::move(floating64::to_wstring(value, floating_format));
		write((pointer)str->cstr(), str->length());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(cwstr_t value)
{
	switch (_format)
	{
	case text::encoding::ascii: {
		string text = value;
		auto buff = (cstr_t)text;
		write((pointer)buff.cstr(), buff.size());
	}break;
	case text::encoding::unicode: {
		write((pointer)value.cstr(), value.size() * sizeof(wchar));
	}break;
	case text::encoding::utf_8: {
		mstring text = value;
		auto buff = (cmstr_t)text;
		write((pointer)buff.cstr(), buff.size());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(cstr_t value)
{
	switch (_format)
	{
	case text::encoding::unicode: {
		wstring text = value;
		auto buff = (cwstr_t)text;
		write((pointer)buff.cstr(), buff.size() * sizeof(wchar));
	}break;
	case text::encoding::ascii: {
		write((pointer)value.cstr(), value.size());
	}break;
	case text::encoding::utf_8: {
		mstring text = value;
		auto buff = (cmstr_t)text;
		write((pointer)buff.cstr(), buff.size());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write(cmstr_t value)
{
	switch (_format)
	{
	case text::encoding::unicode: {
		wstring text = value;
		auto buff = (cwstr_t)text;
		write((pointer)buff.cstr(), buff.size() * sizeof(wchar));
	}break;
	case text::encoding::ascii: {
		string text = value;
		auto buff = (cstr_t)text;
		write((pointer)buff.cstr(), buff.size());
	}break;
	case text::encoding::utf_8: {
		write((pointer)value.cstr(), value.size());
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write_line(cwstr_t value)
{
	switch (_format)
	{
	case text::encoding::ascii: {
		string text = value;
		auto buff = (cstr_t)text;
		write((pointer)buff.cstr(), buff.size());
		write((pointer)"\n", 1);
	}break;
	case text::encoding::unicode: {
		write((pointer)value.cstr(), value.size() * sizeof(wchar));
		write((pointer)L"\n", 2);
	}break;
	case text::encoding::utf_8: {
		mstring text = value;
		auto buff = (cmstr_t)text;
		write((pointer)buff.cstr(), buff.size());
		write((pointer)"\n", 1);
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write_line(cstr_t value)
{
	switch (_format)
	{
	case text::encoding::unicode: {
		wstring text = value;
		auto buff = (cwstr_t)text;
		write((pointer)buff.cstr(), buff.size() * sizeof(wchar));
		write((pointer)L"\n", 2);
	}break;
	case text::encoding::ascii: {
		write((pointer)value.cstr(), value.size());
		write((pointer)"\n", 1);
	}break;
	case text::encoding::utf_8: {
		mstring text = value;
		auto buff = (cmstr_t)text;
		write((pointer)buff.cstr(), buff.size());
		write((pointer)"\n", 1);
	}break;
	default:return false;
	}
	return true;
}

bool socket_text_output_stream::write_line(cmstr_t value)
{
	switch (_format)
	{
	case text::encoding::unicode: {
		wstring text = value;
		auto buff = (cwstr_t)text;
		write((pointer)buff.cstr(), buff.size() * sizeof(wchar));
		write((pointer)L"\n", 2);
	}break;
	case text::encoding::ascii: {
		string text = value;
		auto buff = (cstr_t)text;
		write((pointer)buff.cstr(), buff.size());
		write((pointer)"\n", 1);
	}break;
	case text::encoding::utf_8: {
		write((pointer)value.cstr(), value.size());
		write((pointer)"\n", 1);
	}break;
	default:return false;
	}
	return true;
}