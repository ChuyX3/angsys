#include "pch.h"
#include "ang/system.h"

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
	clean();
}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::strings::basic_string_buffer_base, object, itext_buffer);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::strings::basic_string_buffer_base);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::strings::basic_string_buffer_base, object, itext_buffer, ibuffer, ibuffer_view);

/////////////////////////////////////////////////////////////////////////

pointer basic_string_buffer_base::buffer_ptr()const
{
	return (void*)text_buffer().ptr();
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
	if (_map_index != invalid_index || _map_size != invalid_index)
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
	if (_map_index != invalid_index || _map_size != invalid_index)
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

bool basic_string_buffer_base::can_realloc_buffer()const { return (_map_index == invalid_index && _map_size == invalid_index); };

bool basic_string_buffer_base::realloc_buffer(wsize size) { return realloc(size / _encoder->char_type().size(), true); };

raw_str_t  basic_string_buffer_base::text_buffer() {
	auto char_size = _encoder->char_type().size();
	if (_map_index != invalid_index || _map_size != invalid_index)
		return is_local_data() ? raw_str((pointer)(_data._stack_buffer + _map_index), _map_size, encoding()) : raw_str(((byte*)_data._allocated_buffer) + _map_index, _map_size, encoding());
	else
		return is_local_data() ? raw_str((pointer)_data._stack_buffer, _data._stack_length * char_size, encoding()) : raw_str(_data._allocated_buffer, _data._allocated_length * char_size, encoding());
}

raw_cstr_t  basic_string_buffer_base::text_buffer()const {
	auto char_size = _encoder->char_type().size();
	if (_map_index != invalid_index || _map_size != invalid_index)
		return is_local_data() ? raw_cstr((pointer)(_data._stack_buffer + _map_index), _map_size, encoding()) : raw_cstr(((byte*)_data._allocated_buffer) + _map_index, _map_size, encoding());
	else
		return is_local_data() ? raw_cstr((pointer)_data._stack_buffer, _data._stack_length * char_size, encoding()) : raw_cstr(_data._allocated_buffer, _data._allocated_length * char_size, encoding());
}

/////////////////////////////////////////////////////////////////////////

comparision_result_t basic_string_buffer_base::compare(object const* obj)const
{
	itext_buffer_t buffer = dyn_cast<itext_buffer>(const_cast<object*>(obj));
	if (buffer.is_empty())
		return comparision_result::diferent;
	return (comparision_result)_encoder->compare(text_buffer().ptr(), buffer->buffer_ptr(), buffer->encoding());
}

bool basic_string_buffer_base::is_local_data()const
{
	return  bool(_data._storage_type != invalid_index);
}

void basic_string_buffer_base::length(wsize len)
{
	dword end = 0; wsize i = 0;
	auto char_size = _encoder->char_type().size();
	if (is_local_data()) {
		len = min(len, 128u / char_size - 1);
		_data._stack_length = len;
		_encoder->set_eos(_data._stack_buffer, len); //set end of string
	}
	else {
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
	if (_map_index != invalid_index || _map_size != invalid_index)
		return _map_size;
	else
		return is_local_data() ? _data._stack_length : _data._allocated_length;
}

wsize basic_string_buffer_base::capacity() const
{
	if (_map_index != invalid_index || _map_size != invalid_index)
		return _map_size;
	else
		return is_local_data() ? 128u / _encoder->char_type().size() - 1 : _data._allocated_capacity - 1;
}

void basic_string_buffer_base::copy(raw_cstr_t str)
{
	wsize sz = str.count(), j = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / _encoder->char_type().size();
	if (sz < lc)
	{
		clean();
		_encoder->convert(_data._stack_buffer, _data._stack_length, str.ptr(), j, str.encoding(), true, min(sz, lc - 1));
	}
	else
	{
		realloc(sz, false);
		_encoder->convert(_data._allocated_buffer, _data._allocated_length, str.ptr(), j, str.encoding(), true, min(sz, _data._allocated_capacity - 1));
	}
}

void basic_string_buffer_base::concat(raw_cstr_t str)
{
	wsize sz = str.count(), j = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / _encoder->char_type().size();
	wsize my_len = length();
	if ((my_len + sz) < lc)
	{
		_encoder->convert(_data._stack_buffer, _data._stack_length, str.ptr(), j, str.encoding(), true, min(lc - my_len - 1, sz));
	}
	else
	{
		realloc(my_len + sz, true);
		_encoder->convert(_data._allocated_buffer, _data._allocated_length, str.ptr(), j, str.encoding(), true, min(_data._allocated_capacity - my_len - 1, sz));
	}
}

void basic_string_buffer_base::copy_at(raw_str_t str, windex at)
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

int basic_string_buffer_base::compare(raw_str_t str)const
{
	return encoder()._compare_string(text_buffer().ptr(), str.ptr(), str.encoding());
}

windex basic_string_buffer_base::compare_until(raw_str_t str)const
{
	return encoder()._compare_string_until(text_buffer().ptr(), str.ptr(), str.encoding());
}

windex basic_string_buffer_base::find(raw_str_t str, windex start, windex end)const
{
	auto my_data = text_buffer();
	return encoder()._find(my_data.ptr(), min(my_data.count(), end), str.ptr(), str.count(), str.encoding(), start);
}

windex basic_string_buffer_base::find_revert(raw_str_t str, windex start, windex end)const
{
	auto my_data = text_buffer();
	return encoder()._find_revert(my_data.ptr(), min(my_data.count(), end), str.ptr(), str.count(), str.encoding(), start);
}

wsize basic_string_buffer_base::sub_string(raw_str_t raw, windex start, windex end)const
{
	if (start >= end || start > length()) return 0;
	text::encoder_interface _encoder; wsize i = start;
	text::encoder_interface::initialize_interface(&_encoder, raw._encoding);

	if (!_encoder._convert_string)
		return 0;
	auto my_data = text_buffer();
	return _encoder._convert_string(raw.ptr(), end - start, my_data.ptr(), i, my_data.encoding(), true);
}