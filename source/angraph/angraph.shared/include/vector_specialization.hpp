#pragma once

#define ANG_IMPLEMENT_OBJECT_VECTOR_SPECIALIZATION(_TYPE)	 \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper() \
	: _ptr(null) { } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(ang::collections::vector_buffer<object_wrapper<_TYPE>>* ptr) \
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>() { set(ptr); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(ang::initializer_list_t<ang::object_wrapper<_TYPE>> list) \
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>() { set(new collections::vector_buffer<ang::object_wrapper<_TYPE>>(ang::move(list))); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(const ang::collections::ienum<ang::object_wrapper<_TYPE>>* store) \
	: object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>() { set(new collections::vector_buffer<ang::object_wrapper<_TYPE>>(store)); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> && other) \
	: object_wrapper<collections::vector_buffer<ang::object_wrapper<_TYPE>>>() {  \
	collections::vector_buffer<ang::object_wrapper<_TYPE>> * temp = other._ptr; \
	other._ptr = null; \
	_ptr = temp; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> const& other) \
	: object_wrapper<collections::vector_buffer<ang::object_wrapper<_TYPE>>>() { set(other.get()); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::~object_wrapper() { clean(); } \
void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::clean() { \
	if (_ptr)_ptr->release(); \
	_ptr = null; \
} \
void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::clean_unsafe() { _ptr = null; } \
bool ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::is_empty()const { return _ptr == null; } \
ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>* ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::get(void)const { return _ptr; } \
void ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::set(ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>* ptr) { \
	ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> * temp = _ptr; \
	if (ptr == _ptr) return; \
	_ptr = ptr; \
	if (_ptr)_ptr->add_ref(); \
	if (temp)temp->release(); \
} \
ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>** ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::addres_of(void) { return &_ptr; } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>* ptr) { \
	set(ptr); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (const ang::nullptr_t&) { \
	clean(); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (ang::collections::ienum<ang::object_wrapper<_TYPE>> const* items) { \
	if (_ptr == null) set(new collections::vector_buffer<ang::object_wrapper<_TYPE>>(items)); \
	else _ptr->copy(items); \
	return *this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> && other) { \
	if (this == &other) return *this; \
	clean(); \
	_ptr = other._ptr; \
	other._ptr = null; \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> const& other) { \
	set(other._ptr); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator += (ang::object_wrapper<_TYPE> value) { \
	if (is_empty()) set(new ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>()); \
	get()->append(ang::move(value)); \
	return*this; \
} \
ang::object_wrapper_ptr<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>> ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator & (void) { return this; } \
ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> * ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator -> (void) { return get(); } \
ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> const* ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator -> (void)const { return get(); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> * (void) { return get(); } \
ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator ang::collections::vector_buffer<ang::object_wrapper<_TYPE>> const* (void)const { return get(); } \
ang::object_wrapper<_TYPE> const& ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator[](int idx)const { return _ptr->data()[idx]; } \
ang::object_wrapper<_TYPE> & ang::object_wrapper<ang::collections::vector_buffer<ang::object_wrapper<_TYPE>>>::operator[](int idx) { return _ptr->data()[idx]; } 



#define ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(_TYPE)	 \
ang::collections::vector_buffer<_TYPE>::vector_buffer() \
	: object(), _size(0) , _capacity(0) , _data(null) \
{ allocator = memory::allocator_manager::get_allocator(0); } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(ang::initializer_list_t<_TYPE> list) \
	: vector_buffer() { \
	capacity((uint)list.size()); \
	for (auto it = list.begin(); it != list.end(); ++it) append(*it); \
} \
ang::collections::vector_buffer<_TYPE>::vector_buffer(const ang::nullptr_t&) \
	: vector_buffer() { } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(ang::collections::vector_buffer<_TYPE>&& ar) \
	: vector_buffer() { \
	_size = ar._size; \
	_data = ar._data; \
	_capacity = ar._capacity; \
	allocator = ar.allocator; \
	ar._size = 0; \
	ar._capacity = 0; \
	ar._data = null; \
	ar.allocator = 0; \
} \
ang::collections::vector_buffer<_TYPE>::vector_buffer(const ang::collections::vector_buffer<_TYPE>& ar) \
	: vector_buffer() { copy(static_cast<const ienum_t*>(&ar)); } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(const ang::collections::vector_buffer<_TYPE>* ar) \
	: vector_buffer() { if (ar) copy(static_cast<const ienum_t*>(ar)); } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(const ang::collections::ienum<_TYPE>* store) \
	: vector_buffer() { copy(store); } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(uint sz, _TYPE const* ar) \
	: vector_buffer() { \
	if (ar != null) copy(sz, ar); \
	else capacity(sz); \
} \
ang::collections::vector_buffer<_TYPE>::~vector_buffer() { clean(); } \
bool ang::collections::vector_buffer<_TYPE>::is_empty()const { return size() == 0U; } \
_TYPE* ang::collections::vector_buffer<_TYPE>::data()const { \
	if (_data) return _data; \
	return null; \
} \
uint ang::collections::vector_buffer<_TYPE>::size()const { return _size; } \
uint ang::collections::vector_buffer<_TYPE>::capacity()const { return _capacity; } \
void ang::collections::vector_buffer<_TYPE>::capacity(uint size, bool save) { realloc(size, save); } \
bool ang::collections::vector_buffer<_TYPE>::copy(uint s, _TYPE const* ar) { \
	capacity(s); \
	for (uint i = 0U; i < s; i++)	append(ar[i]); \
	return true; \
} \
void ang::collections::vector_buffer<_TYPE>::set_allocator(memory::iallocator* alloc) { \
	if (alloc == null && alloc == allocator) return; \
	if (!alloc->can_delete_from(allocator)) realloc(alloc); \
	allocator = alloc; \
} \
ang::memory::iallocator* ang::collections::vector_buffer<_TYPE>::get_allocator()const { return allocator; } \
pointer ang::collections::vector_buffer<_TYPE>::buffer_ptr()const { return (pointer)_data; } \
wsize ang::collections::vector_buffer<_TYPE>::buffer_size()const { 	return _capacity * sizeof(_TYPE); } \
uint ang::collections::vector_buffer<_TYPE>::mem_copy(wsize _s, pointer _p) { \
	auto s = min((_s / sizeof(_TYPE)) * sizeof(_TYPE), (_capacity - 1) * sizeof(_TYPE)); \
	memcpy(_data, _p, s); \
	return s; \
} \
pointer ang::collections::vector_buffer<_TYPE>::map_buffer(windex start, wsize size) { \
	if ((start + size) >= capacity() * sizeof(_TYPE)) return null; \
	return (byte_t*)data() + start; \
} \
void ang::collections::vector_buffer<_TYPE>::unmap_buffer(pointer ptr, wsize used) { \
	if (ptr == ((byte_t*)data() + size() * sizeof(_TYPE)) \
		&& (size() + used / sizeof(_TYPE)) < capacity()) { \
		_size = size() + used / sizeof(_TYPE); \
	} \
} \
bool ang::collections::vector_buffer<_TYPE>::can_realloc_buffer()const { return true; }; \
bool ang::collections::vector_buffer<_TYPE>::realloc_buffer(wsize size) { return realloc(size, true); }; \
bool ang::collections::vector_buffer<_TYPE>::has_items()const { return bool(size() != 0); } \
bool ang::collections::vector_buffer<_TYPE>::move(ang::collections::vector_buffer<_TYPE>& ar) { \
	if (&ar == this) return false; \
	clean(); \
	_size = ar._size; \
	_data = ar._data; \
	_capacity = ar._capacity; \
	allocator = ar.allocator; \
	ar._data = null; \
	ar._size = 0; \
	ar.allocator = 0; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::copy(const ang::collections::ienum<_TYPE>* _items) { \
	if(_items == nullptr) return false; \
	capacity(_items->counter(), false); \
	for (auto it = _items->begin(); it.is_valid(); ++it) append(const_cast<_TYPE&>(*it)); \
	return true; \
} \
_TYPE& ang::collections::vector_buffer<_TYPE>::at(const ang::collections::iterator<_TYPE>& it)const { return _data[it.offset()]; } \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::find_index(index idx)const { \
	if (idx >= _size) return  iterator_t(const_cast<self_t*>(this), null, 0); \
	return iterator_t(const_cast<self_t*>(this), (pointer)_data, idx); \
} \
index ang::collections::vector_buffer<_TYPE>::index_of(ang::collections::iterator<_TYPE> it)const { \
	if (!it.is_valid() || it.parent() != this) return invalid_index; \
	return it.offset(); \
} \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::find(_TYPE const& datum, bool invert)const { \
	if (!is_empty()) { \
		if (invert) for (iterator_t it = begin(); it.is_valid(); ++it) { \
			if (*it == datum) return it; \
		} \
		else for (iterator_t it = end(); it.is_valid(); --it) { \
			if (*it == datum) return it; \
		} \
	} \
	return iterator_t(const_cast<self_t*>(this), 0); \
} \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::find(_TYPE const& datum, ang::collections::iterator<_TYPE> nextTo, bool invert)const { \
	if (!is_empty() && nextTo.parent() == this) { \
		if (invert)for (iterator_t it = nextTo.is_valid() ? nextTo : begin(); it.is_valid(); ++it) { \
			if (*it == datum) return it; \
		} \
		else for (iterator_t it = nextTo.is_valid() ? nextTo : end(); it.is_valid(); --it) { \
			if (*it == datum) return it; \
		} \
	} \
	return iterator_t(const_cast<vector_buffer*>(this), 0); \
} \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::begin()const { \
	return iterator_t(const_cast<vector_buffer*>(this), position_t((is_empty()) ? null : _data)); \
} \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::end()const { \
	return iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1); \
} \
bool ang::collections::vector_buffer<_TYPE>::next(ang::collections::iterator<_TYPE>& it)const { \
	it.offset(it.offset() + 1); \
	if (it.offset() == _size) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::next(ang::collections::iterator<_TYPE>& it, int val)const { \
	it.offset(it.offset() + val); \
	if (it.offset() == _size) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::prev(ang::collections::iterator<_TYPE>& it)const { \
	it.offset(it.offset() - 1); \
	if (it.offset() == uint(-1)) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::prev(ang::collections::iterator<_TYPE>& it, int val)const { \
	it.offset((uint(val) > it.offset()) ? uint(-1) : it.offset() - val); \
	if (it.offset() == uint(-1)) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
uint ang::collections::vector_buffer<_TYPE>::counter()const { return _size; } \
void ang::collections::vector_buffer<_TYPE>::extend(const ang::collections::ienum<_TYPE>* items) { \
	if (!items) return; \
	capacity(counter() + items->counter()); \
	for (auto it = items->begin(); it.is_valid(); ++it) append(*it); \
} \
void ang::collections::vector_buffer<_TYPE>::append(_TYPE const& value, bool last) { \
	if (((long_t)capacity() - (long_t)counter()) <= 1) capacity(capacity() + 1, true); \
	if (last) { _data[_size++] = value; } \
	else { \
		for (auto i = _size; i > 0U; --i) _data[i] = _data[i - 1U]; \
		_data[0] = value; \
		_size++; \
	} \
} \
bool ang::collections::vector_buffer<_TYPE>::insert(index idx, _TYPE const& value) { \
	if (idx == 0U) { \
		append(value, false); \
		return true; \
	} \
	else if (idx >= _size) { \
		append(value, true); \
		return true; \
	} \
	if ((capacity() - counter()) <= 1U) \
		capacity(capacity() + 1, true); \
	for (auto i = _size; i > idx; --i) _data[i] = _data[i - 1U]; \
	_data[idx] = value; \
	_size++; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::insert(ang::collections::iterator<_TYPE> it, _TYPE const& value) { \
	if (it.parent() != this || it.current() != _data) return false; \
	return insert((index)it.offset(), value); \
} \
bool ang::collections::vector_buffer<_TYPE>::pop(bool last) { \
	if (_size == 0) return false; \
	if (!last) for (index i = 1U; i < _size; ++i) \
		_data[i - 1] = ang::move(_data[i]); \
	--_size; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::pop(_TYPE& value, bool last) { \
	if (_size == 0) return false; \
	if (!last) { \
		value = ang::move(_data[0]); \
		for (index i = 1U; i < _size; ++i) _data[i - 1] = ang::move(_data[i]); \
	} \
	else { value = ang::move(_data[_size - 1U]); } \
	--_size; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::pop_at(index idx) { \
	if (_size == 0U) return false; \
	if (idx == 0U) return pop(false); \
	for (index i = idx; i < _size; ++i) _data[i - 1] = ang::move(_data[i]); \
	--_size; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::pop_at(ang::collections::iterator<_TYPE> it) { \
	if (it.parent() != this || it.current() != _data) return false; \
	return pop_at((index)it.offset()); \
} \
bool ang::collections::vector_buffer<_TYPE>::pop_at(index idx, _TYPE& value) { \
	if (_size == 0U) return false; \
	if (idx == 0U) return pop(value, false); \
	value = ang::move(_data[0]); \
	for (index i = idx; i < _size; ++i) _data[i - 1] = ang::move(_data[i]); \
	--_size; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::pop_at(ang::collections::iterator<_TYPE> it, _TYPE& value) { \
	if (it.parent() != this || it.current() != _data) return false; \
	return pop_at((index)it.offset(), value); \
} \
bool ang::collections::vector_buffer<_TYPE>::is_kind_of(ang::type_name_t name)const { \
	if (name == type_name<vector_buffer<_TYPE>>() \
		|| ang::object::is_kind_of(name) \
		|| ang::collections::icollection<_TYPE>::is_kind_of(name) \
		|| ang::collections::ilist<_TYPE>::is_kind_of(name)) \
		return true; \
	return false; \
} \
ang::type_name_t ang::collections::vector_buffer<_TYPE>::class_name() { return "ang::collections::vector<"#_TYPE">"; } \
ang::type_name_t ang::collections::vector_buffer<_TYPE>::object_name()const { return ang::collections::vector_buffer<_TYPE>::class_name(); } \
bool ang::collections::vector_buffer<_TYPE>::query_object(ang::type_name_t className, ang::unknown_ptr_t out) { \
	if (out == null) return false; \
	if (className == type_name<vector_buffer<_TYPE>>()) { \
		*out = static_cast<ang::collections::vector_buffer<_TYPE>*>(this); \
		return true; \
	} \
	else if (ang::object::query_object(className, out)) { return true; 	} \
	else if (ang::collections::icollection<_TYPE>::query_object(className, out)) { return true; } \
	else if (ang::collections::ilist<_TYPE>::query_object(className, out)) { return true; } \
	return false; \
} \
void ang::collections::vector_buffer<_TYPE>::clean() { \
	if (_data != null) { \
		for (uint i = 0; i < _size; ++i) allocator->destruct<_TYPE>(&_data[i]); \
		_size = 0U; \
		_capacity = 0U; \
		allocator->memory_release(_data); \
	} \
	_data = null; \
} \
bool ang::collections::vector_buffer<_TYPE>::operator == (const ang::collections::vector_buffer<_TYPE>& ar) { \
	if (size() != ar.size()) return false; \
	for (index i = 0, l = size(); i < l; i++) \
		if (_data[i] != ar._data[i]) return false; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::operator != (const ang::collections::vector_buffer<_TYPE>& ar) { return !operator == (ar); } \
ang::collections::vector_buffer<_TYPE>::operator _TYPE*()const { return _data; } \
bool ang::collections::vector_buffer<_TYPE>::realloc(uint new_size, bool save) { \
	new_size++; \
	if (capacity() >= new_size) return true; \
	uint sz = 8U, temp = 0U; \
	while (sz <= new_size) sz *= 2U; \
	memory::iallocator* alloc = get_allocator(); \
	_TYPE* new_buffer = alloc->object_alloc<_TYPE>(sz); \
	if (save) { \
		temp = _size; \
		for (index i = 0U; i < _size; ++i) new_buffer[i] = ang::move(_data[i]); \
		alloc->memory_release(_data); \
	} \
	else { clean(); } \
	_data = new_buffer; \
	_size = temp; \
	_capacity = sz; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::realloc(ang::memory::iallocator* alloc) { \
	if (capacity() == 0U) return true; \
	memory::iallocator* this_alloc = get_allocator(); \
	_TYPE* new_buffer = alloc->object_alloc<_TYPE>(_capacity); \
	for (index i = 0U; i < _size; ++i) new_buffer[i] = ang::move(_data[i]); \
	this_alloc->memory_release(_data); \
	_data = new_buffer; \
	return true; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper() \
	: _ptr(null) { } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(ang::collections::vector_buffer<_TYPE>* ptr) \
	: object_wrapper<ang::collections::vector_buffer<_TYPE>>() { set(ptr); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(ang::initializer_list_t<data_type> list) \
	: object_wrapper<ang::collections::vector_buffer<_TYPE>>() { set(new collections::vector_buffer<_TYPE>(ang::move(list))); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(const ang::collections::ienum<data_type>* store) \
	: object_wrapper<ang::collections::vector_buffer<_TYPE>>() { set(new collections::vector_buffer<_TYPE>(store)); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(uint size, data_type const* ar) \
	: object_wrapper<ang::collections::vector_buffer<_TYPE>>() { set(new collections::vector_buffer<_TYPE>(size, ar)); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<_TYPE>> && other) \
	: object_wrapper<collections::vector_buffer<_TYPE>>() { \
	collections::vector_buffer<_TYPE> * temp = other._ptr; \
	other._ptr = null; \
	_ptr = temp; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<_TYPE>> const& other) \
	: object_wrapper<collections::vector_buffer<_TYPE>>() { set(other.get()); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::~object_wrapper() { clean(); } \
void ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::clean() { if (_ptr)_ptr->release(); _ptr = null; } \
void ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::clean_unsafe() { _ptr = null; } \
bool ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::is_empty()const { return _ptr == null; } \
ang::collections::vector_buffer<_TYPE>* ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::get(void)const { return _ptr; } \
void ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::set(ang::collections::vector_buffer<_TYPE>* ptr) { \
	ang::collections::vector_buffer<_TYPE> * temp = _ptr; \
	if (ptr == _ptr) return; \
	_ptr = ptr; \
	if (_ptr)_ptr->add_ref(); \
	if (temp)temp->release(); \
} \
ang::collections::vector_buffer<_TYPE>** ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::addres_of(void) { return &_ptr; } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (ang::collections::vector_buffer<_TYPE>* ptr) { \
	set(ptr); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (const ang::nullptr_t&) { \
	clean(); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (ang::collections::ienum<data_type> const* items) { \
	if (_ptr == null) set(new collections::vector_buffer<_TYPE>(items)); \
	else _ptr->copy(items); \
	return *this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<_TYPE>> && other) { \
	if (this == &other) return *this; \
	clean(); \
	_ptr = other._ptr; \
	other._ptr = null; \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<_TYPE>> const& other) { \
	set(other._ptr); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator += (_TYPE value) { \
	if (is_empty()) set(new ang::collections::vector_buffer<_TYPE>()); \
	get()->append(ang::move(value)); \
	return*this; \
} \
ang::object_wrapper_ptr<ang::collections::vector_buffer<_TYPE>> ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator & (void) { return this; } \
ang::collections::vector_buffer<_TYPE> * ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator -> (void) { return get(); } \
ang::collections::vector_buffer<_TYPE> const* ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator -> (void)const { return get(); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator ang::collections::vector_buffer<_TYPE> * (void) { return get(); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator ang::collections::vector_buffer<_TYPE> const* (void)const { return get(); } \
typename ang::collections::vector_buffer<_TYPE>::type const& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator[](int idx)const { return _ptr->data()[idx]; } \
typename ang::collections::vector_buffer<_TYPE>::type & ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator[](int idx) { return _ptr->data()[idx]; }


#define ANG_IMPLEMENT_VECTOR_DATA_OBJECT_SPECIALIZATION(_TYPE) \
ang::collections::vector_buffer<_TYPE>::vector_buffer() \
	: object(), _size(0) , _capacity(0) , _data(null) \
{ allocator = memory::allocator_manager::get_allocator(0); } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(ang::initializer_list_t<_TYPE> list) \
	: vector_buffer() { \
	capacity((uint)list.size()); \
	for (auto it = list.begin(); it != list.end(); ++it) append(*it); \
} \
ang::collections::vector_buffer<_TYPE>::vector_buffer(const ang::nullptr_t&) \
	: vector_buffer() { } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(ang::collections::vector_buffer<_TYPE>&& ar) \
	: vector_buffer() { \
	_size = ar._size; \
	_data = ar._data; \
	_capacity = ar._capacity; \
	allocator = ar.allocator; \
	ar._size = 0; \
	ar._capacity = 0; \
	ar._data = null; \
	ar.allocator = 0; \
} \
ang::collections::vector_buffer<_TYPE>::vector_buffer(const ang::collections::vector_buffer<_TYPE>& ar) \
	: vector_buffer() { copy(static_cast<const ienum_t*>(&ar)); } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(const ang::collections::vector_buffer<_TYPE>* ar) \
	: vector_buffer() { if (ar) copy(static_cast<const ienum_t*>(ar)); } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(const ang::collections::ienum<_TYPE>* store) \
	: vector_buffer() { copy(store); } \
ang::collections::vector_buffer<_TYPE>::vector_buffer(uint sz, _TYPE const* ar) \
	: vector_buffer() { \
	if (ar != null) copy(sz, ar); \
	else capacity(sz); \
} \
ang::collections::vector_buffer<_TYPE>::~vector_buffer() { clean(); } \
bool ang::collections::vector_buffer<_TYPE>::is_empty()const { return size() == 0U; } \
_TYPE* ang::collections::vector_buffer<_TYPE>::data()const { \
	if (_data) return _data; \
	return null; \
} \
uint ang::collections::vector_buffer<_TYPE>::size()const { return _size; } \
uint ang::collections::vector_buffer<_TYPE>::capacity()const { return _capacity; } \
void ang::collections::vector_buffer<_TYPE>::capacity(uint size, bool save) { realloc(size, save); } \
bool ang::collections::vector_buffer<_TYPE>::copy(uint s, _TYPE const* ar) { \
	capacity(s); \
	for (uint i = 0U; i < s; i++)	append(ar[i]); \
	return true; \
} \
void ang::collections::vector_buffer<_TYPE>::set_allocator(memory::iallocator* alloc) { \
	if (alloc == null && alloc == allocator) return; \
	if (!alloc->can_delete_from(allocator)) realloc(alloc); \
	allocator = alloc; \
} \
ang::memory::iallocator* ang::collections::vector_buffer<_TYPE>::get_allocator()const { return allocator; } \
pointer ang::collections::vector_buffer<_TYPE>::buffer_ptr()const { return (pointer)_data; } \
wsize ang::collections::vector_buffer<_TYPE>::buffer_size()const { 	return _capacity * sizeof(_TYPE); } \
uint ang::collections::vector_buffer<_TYPE>::mem_copy(wsize _s, pointer _p) { \
	auto s = min((_s / sizeof(_TYPE)) * sizeof(_TYPE), (_capacity - 1) * sizeof(_TYPE)); \
	memcpy(_data, _p, s); \
	return s; \
} \
pointer ang::collections::vector_buffer<_TYPE>::map_buffer(windex start, wsize size) { \
	if ((start + size) >= capacity() * sizeof(_TYPE)) return null; \
	return (byte_t*)data() + start; \
} \
void ang::collections::vector_buffer<_TYPE>::unmap_buffer(pointer ptr, wsize used) { \
	if (ptr == ((byte_t*)data() + size() * sizeof(_TYPE)) \
		&& (size() + used / sizeof(_TYPE)) < capacity()) { \
		_size = size() + used / sizeof(_TYPE); \
	} \
} \
bool ang::collections::vector_buffer<_TYPE>::can_realloc_buffer()const { return true; }; \
bool ang::collections::vector_buffer<_TYPE>::realloc_buffer(wsize size) { return realloc(size, true); }; \
bool ang::collections::vector_buffer<_TYPE>::has_items()const { return bool(size() != 0); } \
bool ang::collections::vector_buffer<_TYPE>::move(ang::collections::vector_buffer<_TYPE>& ar) { \
	if (&ar == this) return false; \
	clean(); \
	_size = ar._size; \
	_data = ar._data; \
	_capacity = ar._capacity; \
	allocator = ar.allocator; \
	ar._data = null; \
	ar._size = 0; \
	ar.allocator = 0; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::copy(const ang::collections::ienum<_TYPE>* _items) { \
	if(_items == nullptr) return false; \
	capacity(_items->counter(), false); \
	for (auto it = _items->begin(); it.is_valid(); ++it) append(const_cast<_TYPE&>(*it)); \
	return true; \
} \
_TYPE& ang::collections::vector_buffer<_TYPE>::at(const ang::collections::iterator<_TYPE>& it)const { return _data[it.offset()]; } \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::find_index(index idx)const { \
	if (idx >= _size) return  iterator_t(const_cast<self_t*>(this), null, 0); \
	return iterator_t(const_cast<self_t*>(this), (pointer)_data, idx); \
} \
index ang::collections::vector_buffer<_TYPE>::index_of(ang::collections::iterator<_TYPE> it)const { \
	if (!it.is_valid() || it.parent() != this) return invalid_index; \
	return it.offset(); \
} \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::find(_TYPE const& datum, bool invert)const { \
	if (!is_empty()) { \
		if (invert) for (iterator_t it = begin(); it.is_valid(); ++it) { \
			if (*it == datum) return it; \
		} \
		else for (iterator_t it = end(); it.is_valid(); --it) { \
			if (*it == datum) return it; \
		} \
	} \
	return iterator_t(const_cast<self_t*>(this), 0); \
} \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::find(_TYPE const& datum, ang::collections::iterator<_TYPE> nextTo, bool invert)const { \
	if (!is_empty() && nextTo.parent() == this) { \
		if (invert)for (iterator_t it = nextTo.is_valid() ? nextTo : begin(); it.is_valid(); ++it) { \
			if (*it == datum) return it; \
		} \
		else for (iterator_t it = nextTo.is_valid() ? nextTo : end(); it.is_valid(); --it) { \
			if (*it == datum) return it; \
		} \
	} \
	return iterator_t(const_cast<vector_buffer*>(this), 0); \
} \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::begin()const { \
	return iterator_t(const_cast<vector_buffer*>(this), position_t((is_empty()) ? null : _data)); \
} \
ang::collections::iterator<_TYPE> ang::collections::vector_buffer<_TYPE>::end()const { \
	return iterator_t(const_cast<vector_buffer*>(this), position_t(_data), _size - 1); \
} \
bool ang::collections::vector_buffer<_TYPE>::next(ang::collections::iterator<_TYPE>& it)const { \
	it.offset(it.offset() + 1); \
	if (it.offset() == _size) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::next(ang::collections::iterator<_TYPE>& it, int val)const { \
	it.offset(it.offset() + val); \
	if (it.offset() == _size) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::prev(ang::collections::iterator<_TYPE>& it)const { \
	it.offset(it.offset() - 1); \
	if (it.offset() == uint(-1)) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::prev(ang::collections::iterator<_TYPE>& it, int val)const { \
	it.offset((uint(val) > it.offset()) ? uint(-1) : it.offset() - val); \
	if (it.offset() == uint(-1)) { \
		it.current(null); \
		it.offset(0); \
	} \
	return true; \
} \
uint ang::collections::vector_buffer<_TYPE>::counter()const { return _size; } \
void ang::collections::vector_buffer<_TYPE>::extend(const ang::collections::ienum<_TYPE>* items) { \
	if (!items) return; \
	capacity(counter() + items->counter()); \
	for (auto it = items->begin(); it.is_valid(); ++it) append(*it); \
} \
void ang::collections::vector_buffer<_TYPE>::append(_TYPE const& value, bool last) { \
	if (((long_t)capacity() - (long_t)counter()) <= 1) capacity(capacity() + 1, true); \
	memory::iallocator* alloc = get_allocator(); \
	if (last) { alloc->template construct<_TYPE>(&_data[_size++], value); } \
	else { \
		for (auto i = _size; i > 0U; --i) { \
			alloc->construct<_TYPE>(&_data[i], ang::move(_data[i - 1U])); \
			alloc->destruct<_TYPE>(&_data[i - 1U]); \
		} \
		alloc->construct<_TYPE>(&_data[0], value); \
		_size++; \
	} \
} \
bool ang::collections::vector_buffer<_TYPE>::insert(index idx, _TYPE const& value) { \
	if (idx == 0U) { \
		append(value, false); \
		return true; \
	} \
	else if (idx >= _size) { \
		append(value, true); \
		return true; \
	} \
	if ((capacity() - counter()) <= 1U) \
		capacity(capacity() + 1, true); \
	memory::iallocator* alloc = get_allocator();\
	for (auto i = _size; i > idx; --i) { \
		alloc->construct<_TYPE>(&_data[i], ang::move(_data[i - 1U])); \
		alloc->destruct<_TYPE>(&_data[i - 1U]); \
	} \
	alloc->construct<_TYPE>(&_data[idx], value); \
	_size++; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::insert(ang::collections::iterator<_TYPE> it, _TYPE const& value) { \
	if (it.parent() != this || it.current() != _data) return false; \
	return insert((index)it.offset(), value); \
} \
bool ang::collections::vector_buffer<_TYPE>::pop(bool last) { \
	if (_size == 0) return false; \
	if (!last) for (index i = 1U; i < _size; ++i) \
		_data[i - 1] = ang::move(_data[i]); \
	--_size; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::pop(_TYPE& value, bool last) { \
	if (_size == 0) return false; \
	if (!last) { \
		value = ang::move(_data[0]); \
		for (index i = 1U; i < _size; ++i) _data[i - 1] = ang::move(_data[i]); \
	} \
	else { value = ang::move(_data[_size - 1U]); } \
	--_size; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::pop_at(index idx) { \
	if (_size == 0U) return false; \
	if (idx == 0U) return pop(false); \
	for (index i = idx; i < _size; ++i) _data[i - 1] = ang::move(_data[i]); \
	--_size; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::pop_at(ang::collections::iterator<_TYPE> it) { \
	if (it.parent() != this || it.current() != _data) return false; \
	return pop_at((index)it.offset()); \
} \
bool ang::collections::vector_buffer<_TYPE>::pop_at(index idx, _TYPE& value) { \
	if (_size == 0U) return false; \
	if (idx == 0U) return pop(value, false); \
	value = ang::move(_data[0]); \
	for (index i = idx; i < _size; ++i) _data[i - 1] = ang::move(_data[i]); \
	--_size; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::pop_at(ang::collections::iterator<_TYPE> it, _TYPE& value) { \
	if (it.parent() != this || it.current() != _data) return false; \
	return pop_at((index)it.offset(), value); \
} \
bool ang::collections::vector_buffer<_TYPE>::is_kind_of(ang::type_name_t name)const { \
	if (name == type_name<vector_buffer<_TYPE>>() \
		|| ang::object::is_kind_of(name) \
		|| ang::collections::icollection<_TYPE>::is_kind_of(name) \
		|| ang::collections::ilist<_TYPE>::is_kind_of(name)) \
		return true; \
	return false; \
} \
ang::type_name_t ang::collections::vector_buffer<_TYPE>::class_name() { return "ang::collections::vector<"#_TYPE">"; } \
ang::type_name_t ang::collections::vector_buffer<_TYPE>::object_name()const { return ang::collections::vector_buffer<_TYPE>::class_name(); } \
bool ang::collections::vector_buffer<_TYPE>::query_object(ang::type_name_t className, ang::unknown_ptr_t out) { \
	if (out == null) return false; \
	if (className == type_name<vector_buffer<_TYPE>>()) { \
		*out = static_cast<ang::collections::vector_buffer<_TYPE>*>(this); \
		return true; \
	} \
	else if (ang::object::query_object(className, out)) { return true; 	} \
	else if (ang::collections::icollection<_TYPE>::query_object(className, out)) { return true; } \
	else if (ang::collections::ilist<_TYPE>::query_object(className, out)) { return true; } \
	return false; \
} \
void ang::collections::vector_buffer<_TYPE>::clean() { \
	if (_data != null) { \
		for (uint i = 0; i < _size; ++i) allocator->destruct<_TYPE>(&_data[i]); \
		_size = 0U; \
		_capacity = 0U; \
		allocator->memory_release(_data); \
	} \
	_data = null; \
} \
bool ang::collections::vector_buffer<_TYPE>::operator == (const ang::collections::vector_buffer<_TYPE>& ar) { \
	if (size() != ar.size()) return false; \
	for (index i = 0, l = size(); i < l; i++) \
		if (_data[i] != ar._data[i]) return false; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::operator != (const ang::collections::vector_buffer<_TYPE>& ar) { return !operator == (ar); } \
ang::collections::vector_buffer<_TYPE>::operator _TYPE*()const { return _data; } \
bool ang::collections::vector_buffer<_TYPE>::realloc(uint new_size, bool save) { \
	new_size++; \
	if (capacity() >= new_size) return true; \
	uint sz = 8U, temp = 0U; \
	while (sz <= new_size) sz *= 2U; \
	memory::iallocator* alloc = get_allocator(); \
	_TYPE* new_buffer = alloc->object_alloc<_TYPE>(sz); \
	if (save) { \
		temp = _size; \
		for (index i = 0U; i < _size; ++i) { \
			alloc->construct(&new_buffer[i], ang::move(_data[i])); \
			alloc->destruct(&_data[i]);	\
		} \
		alloc->memory_release(_data); \
	} \
	else { clean(); } \
	_data = new_buffer; \
	_size = temp; \
	_capacity = sz; \
	return true; \
} \
bool ang::collections::vector_buffer<_TYPE>::realloc(ang::memory::iallocator* alloc) { \
	if (capacity() == 0U) return true; \
	memory::iallocator* this_alloc = get_allocator(); \
	_TYPE* new_buffer = alloc->object_alloc<_TYPE>(_capacity); \
	for (index i = 0U; i < _size; ++i) { \
		alloc->construct(&new_buffer[i], ang::move(_data[i])); \
		alloc->destruct(&_data[i]);	\
	} \
	this_alloc->memory_release(_data); \
	_data = new_buffer; \
	return true; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper() \
	: _ptr(null) { } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(ang::collections::vector_buffer<_TYPE>* ptr) \
	: object_wrapper<ang::collections::vector_buffer<_TYPE>>() { set(ptr); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(ang::initializer_list_t<data_type> list) \
	: object_wrapper<ang::collections::vector_buffer<_TYPE>>() { set(new collections::vector_buffer<_TYPE>(ang::move(list))); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(const ang::collections::ienum<data_type>* store) \
	: object_wrapper<ang::collections::vector_buffer<_TYPE>>() { set(new collections::vector_buffer<_TYPE>(store)); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(uint size, data_type const* ar) \
	: object_wrapper<ang::collections::vector_buffer<_TYPE>>() { set(new collections::vector_buffer<_TYPE>(size, ar)); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<_TYPE>> && other) \
	: object_wrapper<collections::vector_buffer<_TYPE>>() { \
	collections::vector_buffer<_TYPE> * temp = other._ptr; \
	other._ptr = null; \
	_ptr = temp; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<_TYPE>> const& other) \
	: object_wrapper<collections::vector_buffer<_TYPE>>() { set(other.get()); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::~object_wrapper() { clean(); } \
void ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::clean() { if (_ptr)_ptr->release(); _ptr = null; } \
void ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::clean_unsafe() { _ptr = null; } \
bool ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::is_empty()const { return _ptr == null; } \
ang::collections::vector_buffer<_TYPE>* ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::get(void)const { return _ptr; } \
void ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::set(ang::collections::vector_buffer<_TYPE>* ptr) { \
	ang::collections::vector_buffer<_TYPE> * temp = _ptr; \
	if (ptr == _ptr) return; \
	_ptr = ptr; \
	if (_ptr)_ptr->add_ref(); \
	if (temp)temp->release(); \
} \
ang::collections::vector_buffer<_TYPE>** ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::addres_of(void) { return &_ptr; } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (ang::collections::vector_buffer<_TYPE>* ptr) { \
	set(ptr); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (const ang::nullptr_t&) { \
	clean(); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (ang::collections::ienum<data_type> const* items) { \
	if (_ptr == null) set(new collections::vector_buffer<_TYPE>(items)); \
	else _ptr->copy(items); \
	return *this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<_TYPE>> && other) { \
	if (this == &other) return *this; \
	clean(); \
	_ptr = other._ptr; \
	other._ptr = null; \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<_TYPE>> const& other) { \
	set(other._ptr); \
	return*this; \
} \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator += (_TYPE value) { \
	if (is_empty()) set(new ang::collections::vector_buffer<_TYPE>()); \
	get()->append(ang::move(value)); \
	return*this; \
} \
ang::object_wrapper_ptr<ang::collections::vector_buffer<_TYPE>> ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator & (void) { return this; } \
ang::collections::vector_buffer<_TYPE> * ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator -> (void) { return get(); } \
ang::collections::vector_buffer<_TYPE> const* ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator -> (void)const { return get(); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator ang::collections::vector_buffer<_TYPE> * (void) { return get(); } \
ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator ang::collections::vector_buffer<_TYPE> const* (void)const { return get(); } \
typename ang::collections::vector_buffer<_TYPE>::type const& ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator[](int idx)const { return _ptr->data()[idx]; } \
typename ang::collections::vector_buffer<_TYPE>::type & ang::object_wrapper<ang::collections::vector_buffer<_TYPE>>::operator[](int idx) { return _ptr->data()[idx]; }
