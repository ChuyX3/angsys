

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer()
	: basic_string_buffer_base()
{
	_encoder = iencoder::get_encoder(MY_ENCODING);
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(wsize reserv)
	: basic_string_buffer()
{
	realloc(reserv, false);
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(raw_cstr_t const& str)
	: basic_string_buffer()
{
	copy(str);
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(basic_string_buffer<MY_ENCODING> const* str)
	: basic_string_buffer()
{
	copy(str->cstr());
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(basic_const_string_buffer<MY_ENCODING>* str)
	: basic_string_buffer()
{
	set(str);
}


basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::~basic_string_buffer()
{
	clear();
}

ang::rtti_t const& basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::class_info()
{
	static const char name[] = ANG_UTILS_TO_STRING(ang::string<MY_ENCODING COMA MY_ALLOCATOR>);
	static rtti_t const* parents[] = TYPE_OF_PTR_ARRAY(basic_string_buffer_base);
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>), alignof(basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>), parents, &default_query_interface);
	return info; 
}

ang::rtti_t const& basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::runtime_info()const
{ 
	return class_info();
}

bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::class_info().type_id()) 
	{
		if (out == null) return false;
		*out = static_cast<basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>*>(this);
		return true;
	}
	else if (basic_string_buffer_base::query_interface(id, out))
	{ 
		return true;
	}
	return false;
}


bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::is_readonly(void) const
{
	return false;
}

text::encoding_t basic_string_buffer<MY_ENCODING>::encoding(void) const
{
	return MY_ENCODING;
}

void basic_string_buffer<MY_ENCODING>::clear()
{
	if(_data._storage_type == storage_type_allocated)
		alloc.deallocate((unsafe_str_t)_data._allocated_buffer);
	else if (_data._storage_type == storage_type_string_pool)
		_data._const_string->release();	
	memset(&_data, 0, sizeof(_data));
}

bool basic_string_buffer<MY_ENCODING>::realloc(wsize new_size, bool save)
{
	if (_map_index != invalid_index || _map_size != invalid_index)
		return false;
	if (_data._storage_type == storage_type_string_pool)
		new_size = save ? max(_data._const_string->text_buffer().count(), new_size) : new_size;
	else if (capacity() > new_size)
		return true;

	wsize cs = sizeof(char_t), size = 32U, i = 0;
	while (size <= new_size)
		size *= 2U;
	auto new_buffer = alloc.allocate(size);
	if (new_buffer == null)
		return false;
	wsize len = 0U, j = 0;
	new_buffer[0] = 0;
	auto data = cstr();
	if (save)
		text::encoder<ENCODING>::convert(new_buffer, len, data.cstr(), j, true, size - 1, data.size());

	clear();
	_data._allocated_length = len;
	_data._allocated_capacity = size - 1;
	_data._allocated_buffer = new_buffer;
	_data._storage_type = storage_type_allocated;
	return true;
}

str_view<typename text::char_type_by_encoding<MY_ENCODING>::char_t, MY_ENCODING> basic_string_buffer<MY_ENCODING>::str()
{ 
	return this ? text_buffer().to_str<ENCODING>() : str_view<typename text::char_type_by_encoding<MY_ENCODING>::char_t, MY_ENCODING>();
}

cstr_view<typename text::char_type_by_encoding<MY_ENCODING>::char_t, MY_ENCODING> basic_string_buffer<MY_ENCODING>::cstr()const
{
	return this ? text_buffer().to_cstr<ENCODING>() : cstr_view<typename text::char_type_by_encoding<MY_ENCODING>::char_t, MY_ENCODING>();
}

//////////////////////////////////////////////////////////////////////////////

#define MY_TYPE basic_string_buffer<MY_ENCODING>
#include "ang/inline/object_wrapper_specialization.inl"
#undef MY_TYPE