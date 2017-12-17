#include "pch.h"
#include <angsys.hpp>
#include <ang/streams.hpp>
#include "string_helper.hpp"
#include "format_parser.h"

using namespace ang;
using namespace ang::strings;

ANG_IMPLEMENT_BASIC_INTERFACE(ang::text::itext_buffer, ang::ibuffer);


namespace ang
{
	extern char hexl[] = { "0123456789abcdefx" };
	extern char hexu[] = { "0123456789ABCDEFX" };
}

template<> wsize ang::load_bom<text::encoding::utf8>(pointer ptr) {
	alignas(4) static byte utf8_bom[4] = { 0xef, 0xbb, 0xbf, 0x0 };
	return (text::UTF8().compare_until((utf8_char_t const*)ptr, (utf8_char_t const*)utf8_bom) == 3) ? 3 : 0;
}

template<> wsize ang::load_bom<text::encoding::utf16_le>(pointer ptr) {
	alignas(4) static byte utf16_le_bom[4] = { 0xff, 0xfe, 0x0, 0x0 };
	return (text::UTF16_LE().compare_until((utf16_char_t const*)ptr, (utf16_char_t const*)utf16_le_bom) == 1) ? 2 : 0;
}

template<> wsize ang::load_bom<text::encoding::utf16_be>(pointer ptr) {
	alignas(4) static byte utf16_be_bom[4] = { 0xfe, 0xff, 0x0, 0x0 };
	return (text::UTF16_BE().compare_until((utf16_char_t const*)ptr, (utf16_char_t const*)utf16_be_bom) == 1) ? 2 : 0;
}

template<> wsize ang::load_bom<text::encoding::utf32_le>(pointer ptr) {
	alignas(4) static byte utf32_le_bom[8] = { 0xff, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	return (text::UTF32_LE().compare_until((utf32_char_t const*)ptr, (utf32_char_t const*)utf32_le_bom) == 1) ? 4 : 0;
}

template<> wsize ang::load_bom<text::encoding::utf32_be>(pointer ptr) {
	alignas(4) static byte utf32_be_bom[8] = { 0x0, 0x0, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0 };
	return (text::UTF32_BE().compare_until((utf32_char_t const*)ptr, (utf32_char_t const*)utf32_be_bom) == 1) ? 4 : 0;
}

template<> wsize ang::load_bom<text::encoding::utf16>(pointer ptr) {
	return text::is_little_endian() ? load_bom<text::encoding::utf16_le>(ptr) : load_bom<text::encoding::utf16_be>(ptr);
}

template<> wsize ang::load_bom<text::encoding::utf16_se>(pointer ptr) {
	return text::is_little_endian() ? load_bom<text::encoding::utf16_be>(ptr) : load_bom<text::encoding::utf16_le>(ptr);
}

template<> wsize ang::load_bom<text::encoding::utf32>(pointer ptr) {
	return text::is_little_endian() ? load_bom<text::encoding::utf32_le>(ptr) : load_bom<text::encoding::utf32_be>(ptr);
}

template<> wsize ang::load_bom<text::encoding::utf32_se>(pointer ptr) {
	return text::is_little_endian() ? load_bom<text::encoding::utf32_be>(ptr) : load_bom<text::encoding::utf32_le>(ptr);
}

template<> wsize ang::load_bom<text::encoding::unicode>(pointer ptr) {
	return load_bom<text::native_encoding<text::encoding::unicode>()>(ptr);
}


long64 ang::str_to_integer(raw_str_t str, windex& i, int base) {
	switch (str.encoding())
	{
	case text::encoding::ascii: return str_to_integer<false>(str.to_cstr<char>(), i, base);
	case text::encoding::utf8: return str_to_integer<false>(str.to_cstr<mchar>(), i, base);
	case text::encoding::utf16_le: return text::is_little_endian() ? str_to_integer<false>(str.to_cstr<char16_t>(), i, base) : str_to_integer<true>(str.to_cstr<char16_t>(), i, base);
	case text::encoding::utf16_be:return text::is_little_endian() ? str_to_integer<true>(str.to_cstr<char16_t>(), i, base) : str_to_integer<false>(str.to_cstr<char16_t>(), i, base);
	case text::encoding::utf16_se:return str_to_integer<true>(str.to_cstr<char16_t>(), i, base);
	case text::encoding::utf16: return str_to_integer<false>(str.to_cstr<char16_t>(), i, base);
	case text::encoding::utf32_le: return text::is_little_endian() ? str_to_integer<false>(str.to_cstr<char32_t>(), i, base) : str_to_integer<true>(str.to_cstr<char32_t>(), i, base);
	case text::encoding::utf32_be:return text::is_little_endian() ? str_to_integer<true>(str.to_cstr<char32_t>(), i, base) : str_to_integer<false>(str.to_cstr<char32_t>(), i, base);
	case text::encoding::utf32_se:return str_to_integer<true>(str.to_cstr<char32_t>(), i, base);
	case text::encoding::utf32: return str_to_integer<false>(str.to_cstr<char32_t>(), i, base);
	default: return 0;
	}
}

ulong64 ang::str_to_uinteger(raw_str_t str, windex& i, int base) {
	switch (str.encoding())
	{
	case text::encoding::ascii: return str_to_uinteger<false>(str.to_cstr<char>(), i, base);
	case text::encoding::utf8: return str_to_uinteger<false>(str.to_cstr<mchar>(), i, base);
	case text::encoding::utf16_le: return text::is_little_endian() ? str_to_uinteger<false>(str.to_cstr<char16_t>(), i, base) : str_to_uinteger<true>(str.to_cstr<char16_t>(), i, base);
	case text::encoding::utf16_be:return text::is_little_endian() ? str_to_uinteger<true>(str.to_cstr<char16_t>(), i, base) : str_to_uinteger<false>(str.to_cstr<char16_t>(), i, base);
	case text::encoding::utf16_se:return str_to_uinteger<true>(str.to_cstr<char16_t>(), i, base);
	case text::encoding::utf16: return str_to_uinteger<false>(str.to_cstr<char16_t>(), i, base);
	case text::encoding::utf32_le: return text::is_little_endian() ? str_to_uinteger<false>(str.to_cstr<char32_t>(), i, base) : str_to_uinteger<true>(str.to_cstr<char32_t>(), i, base);
	case text::encoding::utf32_be:return text::is_little_endian() ? str_to_uinteger<true>(str.to_cstr<char32_t>(), i, base) : str_to_uinteger<false>(str.to_cstr<char32_t>(), i, base);
	case text::encoding::utf32_se:return str_to_uinteger<true>(str.to_cstr<char32_t>(), i, base);
	case text::encoding::utf32: return str_to_uinteger<false>(str.to_cstr<char32_t>(), i, base);
	default: return 0;
	}
}

double ang::str_to_float(raw_str_t str, windex& i, bool exp) {
	switch (str.encoding())
	{
	case text::encoding::ascii: return str_to_floating<false>(str.to_cstr<char>(), i, exp);
	case text::encoding::utf8: return str_to_floating<false>(str.to_cstr<mchar>(), i, exp);
	case text::encoding::utf16_le: return text::is_little_endian() ? str_to_floating<false>(str.to_cstr<char16_t>(), i, exp) : str_to_floating<true>(str.to_cstr<char16_t>(), i, exp);
	case text::encoding::utf16_be:return text::is_little_endian() ? str_to_floating<true>(str.to_cstr<char16_t>(), i, exp) : str_to_floating<false>(str.to_cstr<char16_t>(), i, exp);
	case text::encoding::utf16_se:return str_to_floating<true>(str.to_cstr<char16_t>(), i, exp);
	case text::encoding::utf16: return str_to_floating<false>(str.to_cstr<char16_t>(), i, exp);
	case text::encoding::utf32_le: return text::is_little_endian() ? str_to_floating<false>(str.to_cstr<char32_t>(), i, exp) : str_to_floating<true>(str.to_cstr<char32_t>(), i, exp);
	case text::encoding::utf32_be:return text::is_little_endian() ? str_to_floating<true>(str.to_cstr<char32_t>(), i, exp) : str_to_floating<false>(str.to_cstr<char32_t>(), i, exp);
	case text::encoding::utf32_se:return str_to_floating<true>(str.to_cstr<char32_t>(), i, exp);
	case text::encoding::utf32: return str_to_floating<false>(str.to_cstr<char32_t>(), i, exp);
	default: return 0;
	}
}

char32_t ang::char_format(char32_t c, text::text_format_t f) {
	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != text::text_format::character)
		return c;

	if ((FLAG_CHAR_LOWERCASE & format) == FLAG_CHAR_LOWERCASE)
	{
		if (c >= 'A' && c <= 'Z')
			return 'a' + c - 'A';
	}
	else if ((FLAG_CHAR_UPPERCASE & format) == FLAG_CHAR_UPPERCASE)
	{
		if (c >= 'a' && c <= 'z')
			return 'A' + c - 'a';
	}
	return c;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

string_base_buffer::string_base_buffer()
{
	memset(&_data, 0, sizeof(_data));
	_map_index = invalid_index;
	_map_size = invalid_index;
}

string_base_buffer::~string_base_buffer()
{
	_map_index = invalid_index;
	_map_size = invalid_index;
	clean();
}

type_name_t string_base_buffer::class_name() { return "ang::string_base_buffer"_s; }

type_name_t string_base_buffer::object_name()const { return class_name(); }

bool string_base_buffer::is_inherited_of(type_name_t name)
{
	return name == type_of<string_base_buffer>()
		|| object::is_inherited_of(name) || itext_buffer::is_inherited_of(name);
}

bool string_base_buffer::is_kind_of(type_name_t name)const
{
	return name == type_of<string_base_buffer>()
		|| object::is_kind_of(name) || itext_buffer::is_kind_of(name);
}

bool string_base_buffer::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null) return false;
	if (name == type_of(*this)) {

		*out = static_cast<string_base_buffer*>(this);
		return true;
	}
	else if (object::query_object(name, out)) {
		return true;
	}
	else if (itext_buffer::query_object(name, out)) {
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////

pointer string_base_buffer::buffer_ptr()const
{
	return text_buffer().ptr();
}

wsize string_base_buffer::buffer_size()const
{
	return capacity() * char_size();
}

wsize string_base_buffer::mem_copy(wsize _size, pointer _ptr, text::encoding_t format)
{
	if (format == encoding::binary
		|| format == encoding::auto_detect)
		throw exception_t(except_code::unsupported);
	if (_map_index != invalid_index || _map_size != invalid_index)
	{
		copy_at(raw_str(_ptr, min(_map_size, _size) / char_size(), format), _map_index / char_size());
		return min(_map_size, _size);
	}
	else {	
		copy(raw_str(_ptr, _size / char_size(), format));
		return length() * char_size();
	}
}

ibuffer_view_t string_base_buffer::map_buffer(windex start, wsize size)
{
	if (_map_index != invalid_index || _map_size != invalid_index)
		return null;
	if ((start + size) > (capacity() * char_size()))
		return null;
	_map_index = start;
	_map_size = size;
	return this;
}

bool string_base_buffer::unmap_buffer(ibuffer_view_t& view, wsize used)
{
	if (view.get() != static_cast<ibuffer_view*>(this))
		return false;
	length((_map_index + used) / char_size());
	view = null;
	return true;
}

bool string_base_buffer::can_realloc_buffer()const { return (_map_index == invalid_index && _map_size == invalid_index); };

bool string_base_buffer::realloc_buffer(wsize size) { return realloc(size / char_size(), true); };

raw_str_t  string_base_buffer::text_buffer()const {

	if (_map_index != invalid_index || _map_size != invalid_index)
		return is_local_data() ? raw_str((pointer)(_data._stack_buffer + _map_index), _map_size, encoding()) : raw_str(((byte*)_data._allocated_buffer) +_map_index, _map_size, encoding());
	else
		return is_local_data() ? raw_str((pointer)_data._stack_buffer, _data._stack_length * char_size(), encoding()) : raw_str(_data._allocated_buffer, _data._allocated_length * char_size(), encoding());
}

wsize string_base_buffer::serialize(streams::ibinary_output_stream_t stream)const
{
	return ibuffer::serialize(const_cast<string_base_buffer*>(this), stream);
}

wsize string_base_buffer::serialize(streams::itext_output_stream_t stream)const
{
	return ibuffer::serialize(const_cast<string_base_buffer*>(this), stream);
}


/////////////////////////////////////////////////////////////////////////

comparision_result_t string_base_buffer::compare(object const& obj)const
{
	itext_buffer_t buffer = interface_cast<itext_buffer>(&obj);

	if (buffer.is_empty())
		return comparision_result::diferent;
	auto _cstr = text_buffer();
	auto _ptr = buffer->buffer_ptr();
	auto _for = buffer->encoding();
	return (comparision_result)encoder()._compare_string(_cstr.ptr(), _ptr, _for);
	return comparision_result::diferent;
}

bool string_base_buffer::is_local_data()const
{
	return  bool(_data._storage_type != invalid_index);
}

bool string_base_buffer::realloc(wsize new_size, bool save)
{
	if (_map_index != invalid_index || _map_size != invalid_index)
		return false;

	if (capacity() >= new_size)
		return true;

	wsize cs = char_size(), size = 32U, i = 0;
	while (size <= new_size)
		size *= 2U;
	auto new_buffer = memory::buffer_allocator<byte>::alloc(size * cs);
	if (new_buffer == null)
		return false;
	wsize len = 0U;
	new_buffer[0] = 0;
	if (save)
		len = encoder()._convert_string(new_buffer, size - 1, text_buffer().ptr(), i,encoding(), true);

	clean();
	_data._allocated_length = len;
	_data._allocated_capacity = size - 1;
	_data._allocated_buffer = new_buffer;
	_data._storage_type = invalid_index;
	return true;
}

void string_base_buffer::length(wsize len)
{
	dword end = 0; wsize i = 0;
	if (is_local_data()) {
		len = min(len, 128u / char_size() - 1);
		_data._stack_length = len;

		encoder()._convert_string(_data._stack_buffer + (len * char_size()), 1, &end, i, text::encoding::utf32, false); //set end of string
	}
	else {
		len = min(len, _data._allocated_capacity - 1);
		_data._allocated_length = len;
		encoder()._convert_string((byte*)_data._allocated_buffer + (len * char_size()), 1, &end, i, text::encoding::utf32, false); //set end of string
	}
}

bool string_base_buffer::is_empty()const
{
	return (_data._stack_length == 0 || _data._allocated_length == 0) ? true : false;
}

wsize string_base_buffer::length() const
{
	if (_map_index != invalid_index || _map_size != invalid_index)
		return _map_size;
	else
		return is_local_data() ? _data._stack_length : _data._allocated_length;
}

wsize string_base_buffer::capacity() const
{
	if (_map_index != invalid_index || _map_size != invalid_index)
		return _map_size;
	else
		return is_local_data() ? 128u / char_size() - 1 : _data._allocated_capacity - 1;
}

void string_base_buffer::clean()
{
	if (!is_local_data())
		memory::buffer_allocator<byte>::free((byte*)_data._allocated_buffer); //ANG_ALLOCATOR_RELEASE(_data._buffer_ptr);
	memset(&_data, 0, sizeof(_data));
}

void string_base_buffer::copy(raw_str_t str)
{
	wsize sz = str.count(), i = 0U;
	if (sz == 0U)
		return;
	auto lc = 128u / char_size();
	if (sz < lc)
	{
		clean();
		_data._stack_length = encoder()._convert_string(_data._stack_buffer, min(sz, lc - 1), str.ptr(), i, str.encoding(), true);
	}
	else
	{
		realloc(sz, false);
		_data._allocated_length = encoder()._convert_string(_data._allocated_buffer, min(_data._allocated_capacity - 1, sz), str.ptr(), i, str.encoding(), true);
	}
}

void string_base_buffer::concat(raw_str_t str)
{
	wsize sz = str.count(), i = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / char_size();
	wsize my_len = length();
	if ((my_len + sz) < lc)
	{
		_data._stack_length = my_len + encoder()._convert_string(_data._stack_buffer + (my_len * char_size()), min(lc - my_len - 1, sz), str.ptr(), i, str.encoding(), true);
	}
	else
	{
		realloc(my_len + sz, true);
		_data._allocated_length = my_len + encoder()._convert_string((byte*)_data._allocated_buffer + (my_len * char_size()), min(_data._allocated_capacity - my_len - 1, sz), str.ptr(), i, str.encoding(), true);
	}
}

void string_base_buffer::copy_at(raw_str_t str, windex at)
{
	wsize sz = str.count(), i = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / char_size();
	wsize my_len = min(length(), at);
	if ((my_len + sz) < lc)
	{
		_data._stack_length = my_len + encoder()._convert_string(_data._stack_buffer + (my_len * char_size()), min(lc - my_len - 1, sz), str.ptr(), i, str.encoding(), true);
	}
	else
	{
		realloc(my_len + sz, true);
		_data._allocated_length = my_len + encoder()._convert_string((byte*)_data._allocated_buffer + (my_len * char_size()), min(_data._allocated_capacity - my_len - 1, sz), str.ptr(), i, str.encoding(), true);
	}
}

int string_base_buffer::compare(raw_str_t str)const
{
	return encoder()._compare_string(text_buffer().ptr(), str.ptr(), str.encoding());
}

windex string_base_buffer::compare_until(raw_str_t str)const
{
	return encoder()._compare_string_until(text_buffer().ptr(), str.ptr(), str.encoding());
}

windex string_base_buffer::find(raw_str_t str, windex start, windex end)const
{
	auto my_data = text_buffer();
	return encoder()._find(my_data.ptr(), min(my_data.count(), end), str.ptr(), str.count(), str.encoding(), start);
}

windex string_base_buffer::find_revert(raw_str_t str, windex start, windex end)const
{
	auto my_data = text_buffer();
	return encoder()._find_revert(my_data.ptr(), min(my_data.count(), end), str.ptr(), str.count(), str.encoding(), start);
}

wsize string_base_buffer::sub_string(raw_str_t raw, windex start, windex end)const
{
	if (start >= end || start > length()) return 0;
	text::encoder_interface _encoder; wsize i = 0;
	text::encoder_interface::initialize_interface(&_encoder, raw._encoding);

	if (!_encoder._convert_string)
		return 0;
	auto my_data = text_buffer();
	return _encoder._convert_string(raw.ptr(), end - start, my_data.ptr(), i, my_data.encoding(), true);
}

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::ascii
#include "string_impl.inl"
#undef CURRENT_ENCODING

void string_buffer<encoding::ascii>::format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	wsize size = _vscprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size, false);
	if (is_local_data()) {
		vsprintf_s((char*)_data._stack_buffer, LOCAL_CAPACITY - 1, format, arg_list);
		_data._stack_length = size;
	}
	else
	{
		vsprintf_s((char*)_data._allocated_buffer, _data._allocated_capacity - 1, format, arg_list);
		_data._allocated_length = size;
	}
	va_end(arg_list);
}

void string_buffer<encoding::ascii>::concat_format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	wsize size = _vscprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size + length(), true);
	if (is_local_data())
	{
		vsprintf_s((char*)_data._stack_buffer + _data._stack_length, LOCAL_CAPACITY - _data._stack_length - 1, format, arg_list);
		_data._stack_length += size;
	}
	else
	{
		vsprintf_s((char*)_data._allocated_buffer + _data._allocated_length, _data._allocated_capacity - _data._allocated_length - 1, format, arg_list);
		_data._allocated_length += size;
	}
	va_end(arg_list);
}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::unicode
#include "string_impl.inl"
#undef CURRENT_ENCODING

void string_buffer<encoding::unicode>::format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	wsize size = _vscwprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size, false);
	if (is_local_data()) {
		vswprintf_s((wchar*)_data._stack_buffer, LOCAL_CAPACITY - 1, format, arg_list);
		_data._stack_length = size;
	}
	else
	{
		vswprintf_s((wchar*)_data._allocated_buffer, _data._allocated_capacity - 1, format, arg_list);
		_data._allocated_length = size;
	}
	va_end(arg_list);
}

void string_buffer<encoding::unicode>::concat_format(cstr_t format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	wsize size = _vscwprintf(format, arg_list);
	if (size <= 0U)
		return clean();

	realloc(size + length(), true);
	if (is_local_data())
	{
		vswprintf_s((wchar*)_data._stack_buffer + _data._stack_length, LOCAL_CAPACITY - _data._stack_length - 1, format, arg_list);
		_data._stack_length += size;
	}
	else
	{
		vswprintf_s((wchar*)_data._allocated_buffer + _data._allocated_length, _data._allocated_capacity - _data._allocated_length - 1, format, arg_list);
		_data._allocated_length += size;
	}
	va_end(arg_list);
}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf8
#include "string_impl.inl"
#undef CURRENT_ENCODING

void string_buffer<encoding::utf8>::format(cstr_t format, ...)
{
}

void string_buffer<encoding::utf8>::concat_format(cstr_t format, ...)
{
}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf16
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf16_le
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf16_be
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf32
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf32_le
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING encoding::utf32_be
#include "string_impl.inl"
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

