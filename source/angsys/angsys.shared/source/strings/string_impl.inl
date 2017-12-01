
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

object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator objptr (void)const { return get(); }

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
	: string_base_buffer()
{
}

string_buffer<CURRENT_ENCODING>::string_buffer(wsize reserv)
	: string_base_buffer()
{
	realloc(reserv, false);
}

string_buffer<CURRENT_ENCODING>::string_buffer(raw_str_t const& str)
	: string_base_buffer()
{
	copy(str);
}

string_buffer<CURRENT_ENCODING>::string_buffer(string_base_buffer const* str)
	: string_base_buffer()
{
	copy(str ? str->text_buffer() : raw_str());
}

string_buffer<CURRENT_ENCODING>::~string_buffer()
{
}

type_name_t string_buffer<CURRENT_ENCODING>::class_name() {
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::string<"_o += encoding_t(CURRENT_ENCODING).to_string()) += ">"_s));
	return name;
}
type_name_t string_buffer<CURRENT_ENCODING>::object_name()const { return class_name(); }

bool string_buffer<CURRENT_ENCODING>::is_inherited_of(type_name_t name)
{
	return name == type_of<string_buffer<CURRENT_ENCODING>>()
		|| string_base_buffer::is_inherited_of(name);
}

bool string_buffer<CURRENT_ENCODING>::is_kind_of(type_name_t name)const
{
	return name == type_of<string_buffer<CURRENT_ENCODING>>()
		|| string_base_buffer::is_kind_of(name);
}

bool string_buffer<CURRENT_ENCODING>::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null) return false;
	if (name == type_of(*this)) {
	
		*out = static_cast<string_buffer<CURRENT_ENCODING>*>(this);
		return true;
	}
	else if (string_base_buffer::query_object(name, out)) {
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////

text::encoding_t string_buffer<CURRENT_ENCODING>::encoding()const
{
	return ENCODING;
}

wsize string_buffer<CURRENT_ENCODING>::char_size()const
{
	return sizeof(char_t);
}

text::encoder_interface& string_buffer<CURRENT_ENCODING>::encoder()const
{
	static text::encoder<CURRENT_ENCODING> _encoder;
	return *(text::encoder_interface*)&_encoder;
}

/////////////////////////////////////////////////////////////////////////

void string_buffer<CURRENT_ENCODING>::move(string_base<CURRENT_ENCODING>& other)
{
	if (other.is_empty() || this == other.get())
		return;
	clean();
	memcpy(&_data, &other->_data, sizeof(_data));
	memset(&other->_data, 0, sizeof(_data));
}

void string_buffer<CURRENT_ENCODING>::move(string_buffer<CURRENT_ENCODING>* other)
{
	if (other == null || this == other)
		return;
	clean();
	memcpy(&_data, &other->_data, sizeof(_data));
	memset(&other->_data, 0, sizeof(_data));
}

typename string_buffer<CURRENT_ENCODING>::str_t string_buffer<CURRENT_ENCODING>::str()
{
	auto ptr = text_buffer();
	return str_t((char_t*)ptr.ptr(), ptr.size() / sizeof(char_t));
}

typename string_buffer<CURRENT_ENCODING>::cstr_t string_buffer<CURRENT_ENCODING>::cstr() const
{
	auto ptr = text_buffer();
	return cstr_t((char_t*)ptr.ptr(), ptr.size() / sizeof(char_t));
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