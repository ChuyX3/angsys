

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer()
	: base()
{
	m_encoder = iencoder::get_encoder(MY_ENCODING);
	m_parser = iparser::get_parser(MY_ENCODING);
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(wsize reserv)
	: base()
{
	m_encoder = iencoder::get_encoder(MY_ENCODING);
	m_parser = iparser::get_parser(MY_ENCODING);
	realloc(reserv, false);
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(raw_cstr_t const& str)
	: base()
{
	m_encoder = iencoder::get_encoder(MY_ENCODING);
	m_parser = iparser::get_parser(MY_ENCODING);
	copy(str);
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(basic_string_buffer<MY_ENCODING> const* str)
	: base()
{
	m_encoder = iencoder::get_encoder(MY_ENCODING);
	m_parser = iparser::get_parser(MY_ENCODING);
	copy(str->cstr());
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(basic_const_string_buffer<MY_ENCODING>* str)
	: base()
{
	m_encoder = iencoder::get_encoder(MY_ENCODING);
	m_parser = iparser::get_parser(MY_ENCODING);
	set(str);
}


basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::~basic_string_buffer()
{
	//clear();
}

ang::rtti_t const& basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::class_info()
{
	static const char name[] = ANG_UTILS_TO_STRING(ang::string<MY_ENCODING>);
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

variant basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::clone()const
{
	return (ivariant*)new self_t(cstr());
}

bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::is_readonly(void) const
{
	return false;
}

void basic_string_buffer<MY_ENCODING>::clear()
{
	allocator_t alloc;
	if(m_data.m_storage_type == storage_type_allocated)
		alloc.deallocate((unsafe_str_t)m_data.m_allocated_buffer);
	else if (m_data.m_storage_type == storage_type_string_pool)
		m_data.m_const_string->release();	
	memset(&m_data, 0, sizeof(m_data));
}

raw_str_t basic_string_buffer<MY_ENCODING>::alloc(wsize new_size)
{
	allocator_t alloc;
	wsize size = 2 * RAW_CAPACITY / size_of<char_t>(), i = 0;
	while (size <= new_size)
		size *= 2U;
	auto new_buffer = alloc.allocate(size);
	return raw_str_t(new_buffer, size * size_of<char_t>(), MY_ENCODING);
}

bool basic_string_buffer<MY_ENCODING>::realloc(wsize new_size, bool save)
{
	allocator_t alloc;
	if (m_map_index != (wsize)invalid_index || m_map_size != (wsize)invalid_index)
		return false;
	if (m_data.m_storage_type == storage_type_string_pool)
		new_size = save ? max(m_data.m_const_string->cstr().count(), new_size) : new_size;
	else if (capacity() > new_size)
		return true;
	wsize size = 2 * RAW_CAPACITY / size_of<char_t>(), i = 0;
	while (size <= new_size)
		size *= 2U;
	auto new_buffer = alloc.allocate(size);
	if (new_buffer == null)
		return false;
	wsize len = 0U, j = 0;
	new_buffer[0] = 0;
	str_view<const char_t, MY_ENCODING> data = cstr();
	if (save)
		text::encoder<ENCODING>::convert(new_buffer, len, data, j, true, size - 1, data.size());

	clear();
	m_data.m_allocated_length = len;
	m_data.m_allocated_capacity = size - 1;
	m_data.m_allocated_buffer = new_buffer;
	m_data.m_storage_type = storage_type_allocated;
	return true;
}

raw_str_t basic_string_buffer<MY_ENCODING>::str(int) {
	return basic_string_buffer_base::str();
}

raw_cstr_t basic_string_buffer<MY_ENCODING>::cstr(int)const {
	return basic_string_buffer_base::cstr();
}

str_view<typename text::char_type_by_encoding<MY_ENCODING>::char_t, MY_ENCODING> basic_string_buffer<MY_ENCODING>::str()
{ 
	return this ? str(0).str<ENCODING>() : str_view<typename text::char_type_by_encoding<MY_ENCODING>::char_t, MY_ENCODING>();
}

cstr_view<typename text::char_type_by_encoding<MY_ENCODING>::char_t, MY_ENCODING> basic_string_buffer<MY_ENCODING>::cstr()const
{
	return this ? cstr(0).cstr<ENCODING>() : cstr_view<typename text::char_type_by_encoding<MY_ENCODING>::char_t, MY_ENCODING>();
}

//////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::object_wrapper() : m_ptr(null) {
	set(new basic_string_buffer<MY_ENCODING>());
}

ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::object_wrapper(basic_string_buffer<MY_ENCODING>* ptr) : m_ptr(null) {
	set(ptr);
}

ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::object_wrapper(object_wrapper && other) : m_ptr(null) {
	basic_string_buffer<MY_ENCODING> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::object_wrapper(object_wrapper const& other) : m_ptr(null) {
	set(new basic_string_buffer<MY_ENCODING>(other.m_ptr));
}


ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::object_wrapper(std::nullptr_t const&) : m_ptr(null) {
}


ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::~object_wrapper() { reset(); }


void ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}


void ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::reset_unsafe()
{
	m_ptr = null;
}


bool ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::is_empty()const
{
	return m_ptr == null || m_ptr->is_empty();
}


basic_string_buffer<MY_ENCODING>* ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::get(void)const
{
	return m_ptr;
}


void ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::set(basic_string_buffer<MY_ENCODING>* ptr)
{
	basic_string_buffer<MY_ENCODING> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}


ang::object_wrapper<basic_string_buffer<MY_ENCODING>>& ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator = (basic_string_buffer<MY_ENCODING>* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<basic_string_buffer<MY_ENCODING>>& ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator = (ang::nullptr_t const&)
{
	reset();
	return*this;
}

ang::object_wrapper<basic_string_buffer<MY_ENCODING>>& ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator = (ang::object_wrapper<basic_string_buffer<MY_ENCODING>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}


ang::object_wrapper<basic_string_buffer<MY_ENCODING>>& ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator = (ang::object_wrapper<basic_string_buffer<MY_ENCODING>> const& other)
{
	set(other.m_ptr);
	return*this;
}


basic_string_buffer<MY_ENCODING> ** ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::addres_of(void)
{
	return &m_ptr;
}


ang::object_wrapper_ptr<basic_string_buffer<MY_ENCODING>> ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator& (void)
{
	return this;
}


basic_string_buffer<MY_ENCODING>* ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator -> (void)
{
	return get();
}

basic_string_buffer<MY_ENCODING>const* ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator basic_string_buffer<MY_ENCODING>* (void)
{
	return get();
}

ang::object_wrapper<basic_string_buffer<MY_ENCODING>>::operator basic_string_buffer<MY_ENCODING>const* (void)const
{
	return get();
}

