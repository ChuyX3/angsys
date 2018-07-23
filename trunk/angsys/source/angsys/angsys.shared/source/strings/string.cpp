#include "pch.h"
#include "angsys.h"

using namespace ang;
using namespace ang::strings;


basic_string_buffer_base::basic_string_buffer_base()
{
	memset(&_data, 0, sizeof(_data));
	_map_index = invalid_index;
	_map_size = invalid_index;
}

basic_string_buffer_base::~basic_string_buffer_base()
{
	_map_index = invalid_index;
	_map_size = invalid_index;
	//clear();
}

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::strings::basic_string_buffer_base, object, itext_buffer);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::strings::basic_string_buffer_base);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::strings::basic_string_buffer_base, object, itext_buffer, ibuffer, ibuffer_view);

/////////////////////////////////////////////////////////////////////////

wstring basic_string_buffer_base::to_string()const
{
	return text_buffer();
}

wstring basic_string_buffer_base::to_string(text::text_format_t format)const
{
	//TODO:
	return text_buffer(); 
}

rtti_t const& basic_string_buffer_base::value_type()const
{
	return runtime_info();
}

bool basic_string_buffer_base::set_value(rtti_t const& id, unknown_t ptr)
{
	if (id.is_type_of(type_of<cstr_t>()))
	{
		cstr_t& str = *reinterpret_cast<cstr_t*>(ptr);
		copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cwstr_t>()))
	{
		cwstr_t& str = *reinterpret_cast<cwstr_t*>(ptr);
		copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cmstr_t>()))
	{
		cmstr_t& str = *reinterpret_cast<cmstr_t*>(ptr);
		copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cstr16_t>()))
	{
		cstr16_t& str = *reinterpret_cast<cstr16_t*>(ptr);
		copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cstr32_t>()))
	{
		cstr32_t& str = *reinterpret_cast<cstr32_t*>(ptr);
		copy(str);
		return true;
	}

	return false;
}

bool basic_string_buffer_base::get_value(rtti_t const& id, unknown_t)const
{
	return false;
}

pointer basic_string_buffer_base::buffer_ptr()
{
	return text_buffer().ptr();
}

const_pointer basic_string_buffer_base::buffer_ptr()const
{
	return text_buffer().ptr();
}

wsize basic_string_buffer_base::buffer_size()const
{
	return capacity() * _encoder->char_type().size();
}

wsize basic_string_buffer_base::mem_copy(wsize _size, pointer _ptr, text::encoding_t format)
{
	//if (format == encoding::binary
	//	|| format == encoding::auto_detect)
	//	throw exception_t(except_code::unsupported);
	auto char_size = _encoder->char_type().size();
	if (_map_index != (wsize)invalid_index || _map_size != (wsize)invalid_index)
	{
		copy_at(raw_str(_ptr, min(_map_size, _size) / char_size, format), _map_index / char_size);
		return min(_map_size, _size);
	}
	else {
		copy(raw_str(_ptr, _size / char_size, format));
		return length() * char_size;
	}
}

ibuffer_view_t basic_string_buffer_base::map_buffer(windex start, wsize size)
{
	if (_map_index != (wsize)invalid_index || _map_size != (wsize)invalid_index)
		return null;
	if ((start + size) > (capacity() * _encoder->char_type().size()))
		return null;
	_map_index = start;
	_map_size = size;
	return this;
}

bool basic_string_buffer_base::unmap_buffer(ibuffer_view_t& view, wsize used)
{
	if (view.get() != static_cast<ibuffer_view*>(this))
		return false;
	length((_map_index + used) / _encoder->char_type().size());
	view = null;
	return true;
}

bool basic_string_buffer_base::can_realloc_buffer()const { return (_map_index == (wsize)invalid_index && _map_size == (wsize)invalid_index); };

bool basic_string_buffer_base::realloc_buffer(wsize size) { return realloc(size / _encoder->char_type().size(), true); };

raw_str_t  basic_string_buffer_base::text_buffer() {
	auto char_size = _encoder->char_type().size();
	if (_map_index != (wsize)invalid_index || _map_size != (wsize)invalid_index)
		return storage_type_stack == storage_type() ? raw_str((pointer)(_data._stack_buffer + _map_index), _map_size, encoding()) : raw_str(((byte*)_data._allocated_buffer) + _map_index, _map_size, encoding());
	else
		return storage_type_stack == storage_type() ? raw_str((pointer)_data._stack_buffer, _data._stack_length * char_size, encoding()) 
		: storage_type_allocated == storage_type() ? raw_str(_data._allocated_buffer, _data._allocated_length * char_size, encoding())
		: _data._const_string->text_buffer();
}

raw_cstr_t  basic_string_buffer_base::text_buffer()const {
	auto char_size = _encoder->char_type().size();
	if (_map_index != (wsize)invalid_index || _map_size != (wsize)invalid_index)
		return storage_type_stack == storage_type() ? raw_cstr((pointer)(_data._stack_buffer + _map_index), _map_size, encoding()) : raw_cstr(((byte*)_data._allocated_buffer) + _map_index, _map_size, encoding());
	else
		return storage_type_stack == storage_type() ? raw_cstr((pointer)_data._stack_buffer, _data._stack_length * char_size, encoding())
		: storage_type_allocated == storage_type() ? raw_cstr(_data._allocated_buffer, _data._allocated_length * char_size, encoding())
		: ((basic_const_string_buffer_base const*)_data._const_string)->text_buffer();
}

/////////////////////////////////////////////////////////////////////////

comparision_result_t basic_string_buffer_base::compare(object const* obj)const
{
	itext_buffer_t buffer = interface_cast<itext_buffer>(const_cast<object*>(obj));
	if (buffer.is_empty())
		return comparision_result::diferent;
	return (comparision_result)_encoder->compare(text_buffer().ptr(), buffer->text_buffer().ptr(), buffer->encoding());
}

basic_string_buffer_base::storage_type_t basic_string_buffer_base::storage_type()const
{
	return _data._storage_type == storage_type_allocated ? storage_type_allocated 
		: _data._storage_type == storage_type_string_pool ? storage_type_string_pool
		: storage_type_stack;
}

void basic_string_buffer_base::length(wsize len)
{
	dword end = 0; wsize i = 0;
	auto char_size = _encoder->char_type().size();
	if (storage_type_stack == storage_type()) {
		len = min(len, 128u / char_size - 1);
		_data._stack_length = len;
		_encoder->set_eos(_data._stack_buffer, len); //set end of string
	}
	else if (storage_type_allocated == storage_type()) {
		len = min(len, _data._allocated_capacity - 1);
		_data._allocated_length = len;
		_encoder->set_eos(_data._allocated_buffer, len); //set end of string
	}
}

bool basic_string_buffer_base::is_empty()const
{
	return (_data._stack_length == 0 || _data._allocated_length == 0) ? true : false;
}

wsize basic_string_buffer_base::length() const
{
	if (_map_index != (wsize)invalid_index || _map_size != (wsize)invalid_index)
		return _map_size;
	else
		return storage_type_stack == storage_type() ? _data._stack_length 
		: storage_type_allocated == storage_type() ? _data._allocated_length
		: _data._const_string->text_buffer().count();
}

wsize basic_string_buffer_base::capacity() const
{
	if (_map_index != (wsize)invalid_index || _map_size != (wsize)invalid_index)
		return _map_size;
	else
		return storage_type_stack == storage_type() ? 128u / _encoder->char_type().size() - 1
		: storage_type_allocated == storage_type() ? _data._allocated_capacity - 1
		: _data._const_string->text_buffer().count();
}

void basic_string_buffer_base::set(basic_const_string_buffer_base* ptr)
{
	clear();
	if (ptr != null && encoding() == ptr->encoding())
	{
		_data._storage_type = storage_type_string_pool;
		_data._const_string = ptr;
		_data._const_string->add_ref();
		_data._const_string_view = ptr->text_buffer().ptr();
	}
}

void basic_string_buffer_base::copy(raw_cstr_t str)
{
	wsize sz = str.count(), j = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / _encoder->char_type().size();
	if (sz < lc)
	{
		clear();
		_encoder->convert(_data._stack_buffer, _data._stack_length, str.ptr(), j, str.encoding(), true, lc - 1, sz);
	}
	else
	{
		realloc(sz, false);
		_encoder->convert(_data._allocated_buffer, _data._allocated_length, str.ptr(), j, str.encoding(), true, _data._allocated_capacity - 1, sz);
	}
}

void basic_string_buffer_base::concat(raw_cstr_t str)
{
	wsize sz = str.count(), j = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / _encoder->char_type().size();
	wsize my_len = length();
	realloc(my_len + sz, true);
	if (storage_type_stack == storage_type())
	{
		_encoder->convert(_data._stack_buffer, _data._stack_length, str.ptr(), j, str.encoding(), true, lc - my_len - 1, sz);
	}
	else if (storage_type_allocated == storage_type())
	{
		_encoder->convert(_data._allocated_buffer, _data._allocated_length, str.ptr(), j, str.encoding(), true, _data._allocated_capacity - my_len - 1, sz);
	}
	else
	{	
		basic_const_string_buffer_base* _const_string = _data._const_string;
		auto txt = _const_string->text_buffer();
		memset(&_data, 0, sizeof(_data));
		realloc(txt.count() + str.count());
		copy(txt);
		concat(str);
		_const_string->release();
	}
}

void basic_string_buffer_base::copy_at(raw_str_t str, windex at)
{
	wsize sz = str.count(), j = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / _encoder->char_type().size();
	wsize my_len = min(length(), at);
	realloc(my_len + sz, true);
	if (storage_type_stack == storage_type())
	{
		_data._stack_length = my_len;
		_encoder->convert(_data._stack_buffer, _data._stack_length, str.ptr(), j, str.encoding(), true, lc - my_len - 1, sz);
	}
	else if (storage_type_allocated == storage_type())
	{
		_data._allocated_length = my_len;
		_encoder->convert(_data._allocated_buffer, _data._allocated_length, str.ptr(), j, str.encoding(), true, _data._allocated_capacity - my_len - 1, sz);
	}
	else
	{
		basic_const_string_buffer_base* _const_string = _data._const_string;
		auto txt = _const_string->text_buffer();
		memset(&_data, 0, sizeof(_data));
		realloc(txt.count() + str.count());
		copy(txt);
		copy_at(str, at);
		_const_string->release();
	}
}

int basic_string_buffer_base::compare(raw_cstr_t str)const
{
	return _encoder->compare(text_buffer().ptr(), str.ptr(), str.encoding());
}

windex basic_string_buffer_base::compare_until(raw_cstr_t str)const
{
	return _encoder->compare_until(text_buffer().ptr(), str.ptr(), str.encoding());
}

windex basic_string_buffer_base::find(raw_cstr_t str, windex start, windex end)const
{
	auto my_data = text_buffer();
	return _encoder->find(my_data.ptr(), min(my_data.count(), end), str.ptr(), str.count(), str.encoding(), start);
}

windex basic_string_buffer_base::find_reverse(raw_cstr_t str, windex start, windex end)const
{
	auto my_data = text_buffer();
	return _encoder->find_reverse(my_data.ptr(), min(my_data.count(), end), str.ptr(), str.count(), str.encoding(), start);
}

raw_str_t basic_string_buffer_base::sub_string(raw_str_t raw, windex start, windex end)const
{
	if (start >= end || start > length()) 
		return raw_str_t();

	auto encoder = iencoder::get_encoder(raw.encoding());
	auto my_data = text_buffer();
	wsize i = 0, j = start, cs = encoder->char_type().size();

	encoder->convert(raw.ptr(), i, my_data.ptr(), j, my_data.encoding(), true, raw.size() / cs, end);
	return raw_str_t(raw.ptr(), i * cs, raw.encoding());
}

///////////////////////////////////////////////////////////////////////////////////
#define MY_ALLOCATOR ang::memory::buffer_allocator

#define MY_ENCODING ang::text::encoding::ascii
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::unicode
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf8
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_se
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_le
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_be
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_se
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_le
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_be
#include "inline/string.inl"
#undef MY_ENCODING
