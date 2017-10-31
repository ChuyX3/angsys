
/////////////////////////////////////////////////////////////////////

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper()
	: _ptr(null)
{

}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper(object_wrapper<strings::string_buffer<ENCODING>> && other)
	: object_wrapper()
{
	_ptr = other._ptr;
	other._ptr = null;
}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper(object_wrapper<strings::string_buffer<ENCODING>> const& other)
	: object_wrapper()
{
	set(new strings::string_buffer<CURRENT_ENCODING>((strings::string_buffer<CURRENT_ENCODING>::cstr_t)other));
}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper(strings::string_buffer<ENCODING>* ptr)
	: object_wrapper()
{
	set(ptr);
}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{
}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::~object_wrapper()
{
	clean();
}

void object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::clean_unsafe()
{
	_ptr = null;
}

bool object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::is_empty()const
{
	return _ptr == null;
}

strings::string_buffer<CURRENT_ENCODING>* object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::get(void)const
{
	return _ptr;
}

void object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::set(strings::string_buffer<CURRENT_ENCODING>* ptr)
{
	strings::string_buffer<CURRENT_ENCODING> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

strings::string_buffer<CURRENT_ENCODING> ** object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::addres_of(void)
{
	return &_ptr;
}


object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator = (strings::string_buffer<ENCODING>* other)
{
	set(other);
	return*this;
}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator = (object_wrapper<strings::string_buffer<ENCODING>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator = (object_wrapper<strings::string_buffer<ENCODING>> const& other)
{
	if (is_empty()) set(new strings::string_buffer<CURRENT_ENCODING>((typename strings::string_buffer<CURRENT_ENCODING>::cstr_t)other));
	else get()->copy((typename strings::string_buffer<CURRENT_ENCODING>::cstr_t)other);
	return*this;
}

object_wrapper_ptr<strings::string_buffer<CURRENT_ENCODING>> object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator & (void) { return this; }

strings::string_buffer<CURRENT_ENCODING> * object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator -> (void) { return get(); }

strings::string_buffer<CURRENT_ENCODING> const* object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator -> (void)const { return get(); }

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator strings::string_buffer<CURRENT_ENCODING> * (void) { return get(); }

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator strings::string_buffer<CURRENT_ENCODING> const* (void)const { return get(); }

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator safe_str<typename text::char_type_by_encoding<CURRENT_ENCODING>::char_t>(void)
{
	return is_empty() ? safe_str<typename text::char_type_by_encoding<CURRENT_ENCODING>::char_t>(null, 0) : get()->str();
}

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator safe_str<typename text::char_type_by_encoding<CURRENT_ENCODING>::char_t const>(void)const
{
	return is_empty() ? safe_str<typename text::char_type_by_encoding<CURRENT_ENCODING>::char_t const>(null, 0) : get()->cstr();
}

typename text::char_type_by_encoding<CURRENT_ENCODING>::char_t& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator [] (int idx)
{
	static typename string_buffer<CURRENT_ENCODING>::char_t dummy = 0;
	if (_ptr && _ptr->length() > (wsize)idx)
		return _ptr->str()[idx];
	return dummy;
}

typename text::char_type_by_encoding<CURRENT_ENCODING>::char_t object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator [] (int idx)const
{
	static char dummy = 0;
	if (_ptr && _ptr->length() > (wsize)idx)
		return _ptr->str()[idx];
	return dummy;
}

/////////////////////////////////////////////////////////////////////

string_buffer<CURRENT_ENCODING>::string_buffer()
{
	memset(&_data, 0, sizeof(_data));
	_map_index = invalid_index;
	_map_size = invalid_index;
}

string_buffer<CURRENT_ENCODING>::string_buffer(wsize reserv)
{
	memset(&_data, 0, sizeof(_data));
	_map_index = invalid_index;
	_map_size = invalid_index;
	realloc(reserv, false);
}

string_buffer<CURRENT_ENCODING>::~string_buffer()
{
	_map_index = invalid_index;
	_map_size = invalid_index;
	clean();
}

type_name_t string_buffer<CURRENT_ENCODING>::class_name() {
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::string<"_o += encoding_t(CURRENT_ENCODING).to_string()) += ">"_s));
	return name;
}
type_name_t string_buffer<CURRENT_ENCODING>::object_name()const { return class_name(); }

bool string_buffer<CURRENT_ENCODING>::is_child_of(type_name_t name)
{
	return name == type_of<string_buffer<CURRENT_ENCODING>>()
		|| object::is_child_of(name) || itext_buffer<CURRENT_ENCODING>::is_child_of(name);
}

bool string_buffer<CURRENT_ENCODING>::is_kind_of(type_name_t name)const
{
	return name == type_of<string_buffer<CURRENT_ENCODING>>()
		|| object::is_kind_of(name) || itext_buffer<CURRENT_ENCODING>::is_kind_of(name);
}

bool string_buffer<CURRENT_ENCODING>::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null) return false;
	if (name == type_of(*this)) {
	
		*out = static_cast<string_buffer<CURRENT_ENCODING>*>(this);
		return true;
	}
	else if (object::query_object(name, out)) {
		return true;
	}
	else if (itext_buffer<CURRENT_ENCODING>::query_object(name, out)) {
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////

pointer string_buffer<CURRENT_ENCODING>::buffer_ptr()const
{
	return pointer(cstr().cstr());
}

wsize string_buffer<CURRENT_ENCODING>::buffer_size()const
{
	return capacity() * sizeof(typename string_buffer<CURRENT_ENCODING>::char_t);
}

wsize string_buffer<CURRENT_ENCODING>::mem_copy(wsize _size, pointer _ptr, text::encoding_t format)
{
	if (format == encoding::binary
		|| format == encoding::auto_detect)
		throw exception_t(except_code::unsupported);
	if (_map_index != invalid_index || _map_size != invalid_index)
	{
		copy_at(_map_index / sizeof(typename string_buffer<CURRENT_ENCODING>::char_t), _ptr, min(_map_size, _size) / sizeof(typename string_buffer<CURRENT_ENCODING>::char_t), format);
		return min(_map_size, _size);
	}
	else {
		copy(_ptr, _size / sizeof(typename string_buffer<CURRENT_ENCODING>::char_t), format);
		return length() * sizeof(typename string_buffer<CURRENT_ENCODING>::char_t);
	}
}

ibuffer_view_t string_buffer<CURRENT_ENCODING>::map_buffer(windex start, wsize size)
{
	if (_map_index != invalid_index || _map_size != invalid_index)
		return null;
	if ((start + size) > (capacity() * sizeof(typename string_buffer<CURRENT_ENCODING>::char_t)))
		return null;
	_map_index = start;
	_map_size = size;
	return this;
}

bool string_buffer<CURRENT_ENCODING>::unmap_buffer(ibuffer_view_t& view, wsize used)
{
	if (view.get() != static_cast<ibuffer_view*>(this))
		return false;
	length((_map_index + used) / sizeof(typename string_buffer<CURRENT_ENCODING>::char_t));
	view = null;
	return true;
}

bool string_buffer<CURRENT_ENCODING>::can_realloc_buffer()const { return true; };

bool string_buffer<CURRENT_ENCODING>::realloc_buffer(wsize size) { return realloc(size / sizeof(typename string_buffer<CURRENT_ENCODING>::char_t), true); };

text::encoding_t string_buffer<CURRENT_ENCODING>::encoding()const
{
	return ENCODING;
}

typename string_buffer<CURRENT_ENCODING>::str_t  string_buffer<CURRENT_ENCODING>::text_buffer() { return str(); }

typename string_buffer<CURRENT_ENCODING>::cstr_t  string_buffer<CURRENT_ENCODING>::text_buffer()const { return cstr(); }

/////////////////////////////////////////////////////////////////////////

comparision_result_t string_buffer<CURRENT_ENCODING>::compare(object const& obj)const
{
	itext_buffer_t<CURRENT_ENCODING> buffer = interface_cast<itext_buffer<CURRENT_ENCODING>>(&obj);
	if (buffer.is_empty())
		return comparision_result::diferent;
	auto encoder = get_encoder<ENCODING>();
	auto _cstr = cstr().cstr();
	auto _ptr = buffer->buffer_ptr();
	auto _for = buffer->encoding();
	return (comparision_result)encoder.compare(_cstr, _ptr, _for);
	return comparision_result::diferent;
}

bool string_buffer<CURRENT_ENCODING>::is_local_data()const
{
	return  bool(_data._storage_type != 0XFFFF);
}

bool string_buffer<CURRENT_ENCODING>::realloc(wsize new_size, bool save)
{
	if (capacity() >= new_size)
		return true;
	
	wsize size = 32U;
	while (size <= new_size)
		size *= 2U;
	typename string_buffer<CURRENT_ENCODING>::char_t* new_buffer = memory::buffer_allocator<typename string_buffer<CURRENT_ENCODING>::char_t>::alloc(size);
	if (new_buffer == null)
		return false;
	wsize len = 0U;
	new_buffer[0] = 0;
	if (save)
		len = get_encoder<ENCODING>().convert(new_buffer, size - 1, cstr().get(), true);

	clean();
	_data._allocated_length = len;
	_data._allocated_capacity = size - 1;
	_data._allocated_buffer = new_buffer;
	_data._storage_type = invalid_index;
	return true;
}

void string_buffer<CURRENT_ENCODING>::length(wsize len)
{
	if (is_local_data()) {
		len = min(len, LOCAL_CAPACITY - 1);
		_data._stack_length = len;
		_data._stack_buffer[len] = 0;
	}
	else {
		len = min(len, _data._allocated_capacity - 1);
		_data._allocated_length = len;
		_data._allocated_buffer[len] = 0;
	}
}

bool string_buffer<CURRENT_ENCODING>::is_empty()const
{
	return (_data._stack_length == 0 || _data._allocated_length == 0) ? true : false;
}

typename string_buffer<CURRENT_ENCODING>::str_t string_buffer<CURRENT_ENCODING>::str()
{
	return is_local_data() ? typename string_buffer<CURRENT_ENCODING>::str_t(_data._stack_buffer, LOCAL_CAPACITY - 1) : typename string_buffer<CURRENT_ENCODING>::str_t(_data._allocated_buffer, _data._allocated_capacity - 1);
}

typename string_buffer<CURRENT_ENCODING>::cstr_t string_buffer<CURRENT_ENCODING>::cstr() const
{
	return is_local_data() ? typename string_buffer<CURRENT_ENCODING>::cstr_t(_data._stack_buffer, LOCAL_CAPACITY - 1) : typename string_buffer<CURRENT_ENCODING>::cstr_t(_data._allocated_buffer, _data._allocated_capacity - 1);
}

wsize string_buffer<CURRENT_ENCODING>::length() const
{
	return is_local_data() ? _data._stack_length : _data._allocated_length;
}

wsize string_buffer<CURRENT_ENCODING>::capacity() const
{
	return is_local_data() ? LOCAL_CAPACITY - 1 : _data._allocated_capacity - 1;
}

void string_buffer<CURRENT_ENCODING>::clean()
{
	if (!is_local_data())
		memory::buffer_allocator<typename string_buffer<CURRENT_ENCODING>::char_t>::free(_data._allocated_buffer); //ANG_ALLOCATOR_RELEASE(_data._buffer_ptr);
	memset(&_data, 0, sizeof(_data));
}

void string_buffer<CURRENT_ENCODING>::copy(pointer raw, wsize sz, encoding_t encoding)
{
	if (sz == 0U)
		return;
	if (sz < LOCAL_CAPACITY)
	{
		clean();
		_data._stack_length = get_encoder<ENCODING>().convert(_data._stack_buffer, min(sz, LOCAL_CAPACITY - 1), raw, encoding, true);
	}
	else
	{
		realloc(sz, false);
		_data._stack_length = get_encoder<ENCODING>().convert(_data._allocated_buffer, min(_data._allocated_capacity - 1, sz), raw, encoding, true);
	}
}

void string_buffer<CURRENT_ENCODING>::copy_at(windex at, pointer raw, wsize sz, encoding_t encoding)
{
	if (sz == 0U)
		return;

	wsize my_len = min(length(), at);
	if ((my_len + sz) < LOCAL_CAPACITY)
	{
		_data._stack_length = my_len + get_encoder<ENCODING>().convert(&_data._stack_buffer[my_len], min(LOCAL_CAPACITY - my_len - 1, sz), raw, encoding, true);
	}
	else
	{
		realloc(my_len + sz, true);
		_data._stack_length = my_len + get_encoder<ENCODING>().convert(&_data._allocated_buffer[my_len], min(_data._allocated_capacity - my_len - 1, sz), raw, encoding, true);
	}
}

typename string_buffer<CURRENT_ENCODING>::char_t& string_buffer<CURRENT_ENCODING>::at(windex idx)
{
	if (idx >= length())
		throw exception_t(except_code::array_overflow);
	return str()[idx];
}

typename string_buffer<CURRENT_ENCODING>::str_t string_buffer<CURRENT_ENCODING>::begin()
{
	return  str();
}

typename string_buffer<CURRENT_ENCODING>::str_t string_buffer<CURRENT_ENCODING>::end()
{
	return typename string_buffer<CURRENT_ENCODING>::str_t(&str()[length()], 0);
}

typename string_buffer<CURRENT_ENCODING>::char_t const& string_buffer<CURRENT_ENCODING>::at(windex idx)const
{
	if (idx >= length())
		throw exception_t(except_code::array_overflow);
	return cstr()[idx];
}

typename string_buffer<CURRENT_ENCODING>::cstr_t string_buffer<CURRENT_ENCODING>::begin()const
{
	return  cstr();
}

typename string_buffer<CURRENT_ENCODING>::cstr_t string_buffer<CURRENT_ENCODING>::end()const
{
	return typename string_buffer<CURRENT_ENCODING>::cstr_t(&cstr()[length()], 0);
}

//void string_buffer<CURRENT_ENCODING>::format(cstr_t format, ...)
//{
//	va_list arg_list;
//	va_start(arg_list, format);
//
//	wsize size = _vscprintf(format, arg_list);
//	if (size <= 0U)
//		return clean();
//
//	realloc(size, false);
//	if (is_local_data()) {
//		vsprintf_s(_data._stack_buffer, LOCAL_CAPACITY - 1, format, arg_list);
//		_data._stack_length = size;
//	}
//	else
//	{
//		vsprintf_s(_data._allocated_buffer, _data._allocated_capacity - 1, format, arg_list);
//		_data._allocated_length = size;
//	}
//	va_end(arg_list);
//}
//
//void string_buffer<CURRENT_ENCODING>::concat_format(cstr_t format, ...)
//{
//	va_list arg_list;
//	va_start(arg_list, format);
//
//	wsize size = _vscprintf(format, arg_list);
//	if (size <= 0U)
//		return clean();
//
//	realloc(size + length(), true);
//	if (is_local_data())
//	{
//		vsprintf_s(&_data._stack_buffer[_data._stack_length], LOCAL_CAPACITY - _data._stack_length - 1, format, arg_list);
//		_data._stack_length += size;
//	}
//	else
//	{
//		vsprintf_s(&_data._allocated_buffer[_data._allocated_length], _data._allocated_capacity - _data._allocated_length - 1, format, arg_list);
//		_data._allocated_length += size;
//	}
//	va_end(arg_list);
//}
//

void string_buffer<CURRENT_ENCODING>::invert()
{
	if (is_empty())
		return;

	typename string_buffer<CURRENT_ENCODING>::char_t* _beg = begin();
	typename string_buffer<CURRENT_ENCODING>::char_t* _end = end();
	typename string_buffer<CURRENT_ENCODING>::char_t temp;
	while (_beg < _end)
	{
		temp = *_beg;
		*_beg = *_end;
		*_end = temp;
		_end--;
		_beg++;
	}
}

void string_buffer<CURRENT_ENCODING>::invert(windex b, windex e)
{
	if (is_empty() || length() <= e || e <= b)
		return;

	char* _beg = &(char&)b;
	char* _end = &(char&)e;
	char temp;
	while (_beg < _end)
	{
		temp = *_beg;
		*_beg = *_end;
		*_end = temp;
		_end--;
		_beg++;
	}
}

void string_buffer<CURRENT_ENCODING>::uppercase()
{
	if (is_empty())
		return;
	wsize l = length();
	typename string_buffer<CURRENT_ENCODING>::char_t* buff = str();
	for (wsize i = 0; i < l; i++)
	{
		if ((buff[i] >= 97) && (buff[i] <= 122))
			buff[i] = char((long)buff[i] + 65 - 97);
	}
}

void string_buffer<CURRENT_ENCODING>::lowercase()
{
	if (is_empty())
		return;
	wsize l = length();
	typename string_buffer<CURRENT_ENCODING>::char_t* buff = str();
	for (wsize i = 0; i < l; i++)
	{
		if ((buff[i] >= 65) && (buff[i] <= 90))
			buff[i] = char((long)buff[i] + 97 - 65);
	}
}