#ifndef __ANG_BASE_ARRAY_HPP__
#error ...
#elif !defined __ANG_BASE_ARRAY_INL__
#define __ANG_BASE_ARRAY_INL__

template<typename T, template <typename> class allocator>
ang::collections::scope_array<T, allocator>::scope_array()
	: _size(0)
	, _data(null) {

}

template<typename T, template <typename> class allocator>
ang::collections::scope_array<T, allocator>::scope_array(ang::nullptr_t const&)
	: _size(0)
	, _data(null) {

}

template<typename T, template <typename> class allocator>
ang::collections::scope_array<T, allocator>::scope_array(wsize sz, T* val)
	: _size(0)
	, _data(ang::null) {
	_size = min(wsize(-1) / sizeof(T), sz);
	if (_size > 0) {
		_data = allocator<T>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			allocator<T>::template construct<T const&>((T*)&_data[i], val[i]);
	}
}

template<typename T, template <typename> class allocator>
ang::collections::scope_array<T, allocator>::scope_array(scope_array const& other)
	: _size(other._size)
	, _data(null) {
	if (_size > 0) {
		_data = allocator<T>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			allocator<T>::template construct<T const&>((T*)&_data[i], other._data[i]);
	}
}

template<typename T, template <typename> class allocator>
ang::collections::scope_array<T, allocator>::scope_array(scope_array&& other)
	: _size(ang::move(other._size))
	, _data(ang::move(other._data)) {
}

template<typename T, template <typename> class allocator>
template<typename U, wsize N>
ang::collections::scope_array<T, allocator>::scope_array(U(&ar)[N])
	: _size(N)
	, _data(null) {
	if (_size > 0) {
		_data = allocator<T>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			allocator<T>::template construct<U const&>((T*)&_data[i], ar[i]);
	}
}

template<typename T, template <typename> class allocator>
template<typename U, wsize N>
ang::collections::scope_array<T, allocator>::scope_array(const U(&ar)[N])
	: _size(N)
	, _data(null) {
	if (_size > 0) {
		_data = allocator<T>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			allocator<T>::template construct<U const&>((T*)&_data[i], ar[i]);
	}
}

template<typename T, template <typename> class allocator>
void ang::collections::scope_array<T, allocator>::clean()
{
	if (_data)
	{
		for (index i = 0; i < _size; ++i)
			allocator<T>::destruct((T*)&_data[i]);
		allocator<T>::free(_data);
	}
	_size = 0;
	_data = null;
}


template<typename T, template <typename> class allocator>
T* ang::collections::scope_array<T, allocator>::alloc(wsize size)
{
	clean();
	if (size > 0)
	{
		_size = size;
		_data = allocator<T>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			allocator<T>::template construct<T const&>((T*)&_data[i], T());
	}
	return _data;
}

template<typename T, template <typename> class allocator>
void ang::collections::scope_array<T, allocator>::set(T* val, wsize sz)
{
	clean();
	_size = min(wsize(-1) / sizeof(T), sz);
	if (_size > 0) {
		_data = allocator<T>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			allocator<T>::template construct<T const&>((T*)&_data[i], val[i]);
	}
}

template<typename T, template <typename> class allocator>
void ang::collections::scope_array<T, allocator>::move(scope_array& other)
{
	if (&other == this)
		return;
	clean();
	_size = other._size;
	_data = other._data;
	other._size = 0;
	other._data = null;
}

template<typename T, template <typename> class allocator>
void ang::collections::scope_array<T, allocator>::move(ang::array_view<T>& other)
{
	clean();
	if(other.size() > 0)
		_data = allocator<T>::alloc(other.size());
	_size = other.size();
	
	for (windex i = 0; i < _size; ++i)
		allocator<T>::template construct<T&&>(&_data[i], ang::move(other.get()[i]));
	other.set(null, 0);
}

template<typename T, template <typename> class allocator> template<typename U, template<typename> class allocator2>
void ang::collections::scope_array<T, allocator>::copy(ang::scope_array<U, allocator2> const& other)
{
	clean();
	if (other.size() > 0)
		_data = allocator<T>::alloc(other.size());
	_size = other.size();

	for (windex i = 0; i < _size; ++i)
		allocator<T>::template construct<U const&>(&_data[i], other[i]);
	other.set(null, 0);
}

template<typename T, template <typename> class allocator> template<typename U>
void ang::collections::scope_array<T, allocator>::copy(ang::array_view<U> const& other)
{
	clean();
	if (other.size() > 0)
		_data = allocator<T>::alloc(other.size());
	_size = other.size();

	for (windex i = 0; i < _size; ++i)
		allocator<T>::template construct<U const&>(&_data[i], other[i]);
	//other.set(null, 0);
}

template<typename T, template <typename> class allocator>
template<typename U, wsize N> void ang::collections::scope_array<T, allocator>::copy(U(&ar)[N]) {
	clean();
	_size = N;
	if (_size > 0) {
		_data = allocator<T>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			allocator<T>::template construct<U const&>((T*)&_data[i], ar[i]);
	}
}

template<typename T, template <typename> class allocator>
template<typename U, wsize N> void ang::collections::scope_array<T, allocator>::copy(const U(&ar)[N]) {
	clean();
	_size = N;
	if (_size > 0) {
		_data = allocator<T>::alloc(_size);
		for (index i = 0; i < _size; ++i)
			allocator<T>::template construct<U const&>((T*)&_data[i], ar[i]);
	}
}

#endif//__ANG_BASE_ARRAY_INL__