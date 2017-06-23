#ifndef __ANG_LIST_H__
#error Can't include list.hpp, please include list.h inside
#elif !defined __ANG_LIST_HPP__
#define __ANG_LIST_HPP__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// list_data<T> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::collections::list_data<T>::list_data()
	: object()
	, _count(0)
	, _first(null)
	, _last(null)
{
	allocator = memory::allocator_manager::get_allocator(0);
}

template<typename T>
inline ang::collections::list_data<T>::list_data(std::initializer_list<T> list)
	: list_data()
{
	for (auto it = list.begin(); it != list.end(); ++it)
		append(*it);
}

template<typename T>
inline ang::collections::list_data<T>::list_data(const std::nullptr_t&)
	: list_data()
{
}

template<typename T>
inline ang::collections::list_data<T>::list_data(ang::collections::list_data<T>&& ar)
	: list_data()
{
	_count = ar._count;
	_first = ar._first;
	_last = ar._last;
	allocator = ar.allocator;
	ar._count = 0;
	ar._first = null;
	ar._last = null;
	ar.allocator = null;
}

template<typename T>
inline ang::collections::list_data<T>::list_data(const ang::collections::list_data<T>& ar)
	: list_data()
{
	copy(static_cast<const ienum_t*>(&ar));
}

template<typename T>
inline ang::collections::list_data<T>::list_data(const ang::collections::list_data<T>* ar)
	: list_data()
{
	if (ar) copy(static_cast<const ienum_t*>(ar));
}

template<typename T>
inline ang::collections::list_data<T>::list_data(const ang::collections::ienum<T>* store)
	: list_data()
{
	copy(store);
}


template<typename T>
inline ang::collections::list_data<T>::list_data(ang::uint_t sz, T const* ar)
	: list_data()
{
	if (ar != null)
	{
		for (auto i = 0U; i < sz; ++i)
			append(ar[i]);
	}
}

template<typename T>
inline ang::collections::list_data<T>::~list_data()
{
	clean();
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::is_empty()const
{
	return 0 == _count;
}


template<typename T>
inline ang::bool_t ang::collections::list_data<T>::copy(ang::uint_t s, T const* ar)
{
	clean();
	if (ar != null)
	{
		for (auto i = 0U; i < sz; ++i)
			append(ar[i]);
	}
	return true;
}

template<typename T>
inline void ang::collections::list_data<T>::set_allocator(memory::iallocator* alloc)
{
	if (alloc == null && alloc == allocator)
		return;

	if (!alloc->can_delete_from(allocator))
		realloc(alloc);

	allocator = alloc;
}

template<typename T>
inline ang::memory::iallocator* ang::collections::list_data<T>::get_allocator()const
{
	return allocator;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::has_items()const
{
	return bool_t(_count != 0);
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::move(ang::collections::list_data<T>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_count = ar._count;
	_first = ar._first;
	_last = ar._last;
	allocator = ar.allocator;
	ar._count = 0;
	ar._first = null;
	ar._last = null;
	ar.allocator = null;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::copy(const ang::collections::ienum<T>* _items)
{
	for (auto it = _items->begin(); it.is_valid(); ++it)
		append(const_cast<T&>(*it));	
	return true;
}

template<typename T>
inline T& ang::collections::list_data<T>::at(const ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty() || it.current() == null)
		throw(exception_t(except_code::invalid_memory));
	if (this != it.parent())
		throw(exception_t(except_code::invalid_param));
#endif
	auto node = reinterpret_cast<node_ptr_t>(it.current());
	return node->value();
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::find_index(ang::index_t index)const
{
	if (index >= _count)
		return  iterator_t(const_cast<self_t*>(this), null, 0);
	node_ptr_t node = null;
	if (index <= _count / 2)
	{
		node = _first;
		for (auto i = 0U; i < index; ++i)
			node = node->next();
	}
	else
	{
		node = _last;
		for (auto i = _count - 1; i > index; --i)
			node = node->prev();
	}
	return iterator_t(const_cast<self_t*>(this), (void_ptr_t)node, 0);
}

template<typename T>
inline ang::index_t ang::collections::list_data<T>::index_of(ang::collections::iterator<T> it)const
{
	if (!it.is_valid() || it.parent() != this)
		return invalid_index;
	auto node = reinterpret_cast<node_ptr_t>(it.current());
	index_t i = 0;
	while (node) {
		node = node->prev();
		++i;
	}
	return i;
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::find(T const& datum, bool_t invert)const
{
	if (!is_empty())
	{
		
		if (invert) for (node_ptr_t node = _first; node != null; node = node->next())
		{
			if (node->value() == datum)
				return iterator_t(const_cast<self_t*>(this), (void_ptr_t)node, 0);
		}
		else for (node_ptr_t node = _last; node != null; node = node->prev())
		{
			if (node->value() == datum)
				return iterator_t(const_cast<self_t*>(this), (void_ptr_t)node, 0);
		}
	}
	return iterator_t(const_cast<self_t*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::find(T const& datum, ang::collections::iterator<T> nextTo, bool_t invert)const
{
	if (!is_empty() && nextTo.parent() == this)
	{
		if (invert)for (node_ptr_t node = nextTo.is_valid()? node_ptr_t(nextTo.current())->next() : _first; node != null; node = node->next())
		{
			if (node->value() == datum)
				return iterator_t(const_cast<self_t*>(this), (void_ptr_t)node, 0);
		}

		else for (node_ptr_t node = nextTo.is_valid() ? node_ptr_t(nextTo.current())->prev() : _last; node != null; node = node->prev())
		{
			if (node->value() == datum)
				return iterator_t(const_cast<self_t*>(this), (void_ptr_t)node, 0);
		}
	}
	return iterator_t(const_cast<list_data*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::begin()const
{
	return iterator_t(const_cast<list_data*>(this), position_t(_first));
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::end()const
{
	return iterator_t(const_cast<list_data*>(this), position_t(_last));
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::next(ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	auto node = reinterpret_cast<node_ptr_t>(it.current());
	node = node->next();
	it.current(node);
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::next(ang::collections::iterator<T>& it, ang::int_t val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	
	auto node = reinterpret_cast<node_ptr_t>(it.current());
	int i = 0;
	while (node && i < val) {
		node = node->next();
		++i;
	}
	it.current(node);
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::prev(ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	auto node = reinterpret_cast<node_ptr_t>(it.current());
	node = node->prev();
	it.current(node);
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::prev(ang::collections::iterator<T>& it, ang::int_t val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif

	auto node = reinterpret_cast<node_ptr_t>(it.current());
	int i = 0;
	while (node && i < val) {
		node = node->prev();
		++i;
	}
	it.current(node);
	return true;
}

template<typename T>
inline ang::uint_t ang::collections::list_data<T>::counter()const
{
	return _count;
}

template<typename T>
inline void ang::collections::list_data<T>::extend(const ang::collections::ienum<T>* items)
{
	if (!items) return;
	for (auto it = items->begin(); it.is_valid(); ++it)
		append(*it);
}

template<typename T>
inline void ang::collections::list_data<T>::append(T value, ang::bool_t last)
{
	memory::iallocator* alloc = get_allocator();
	node_ptr_t node = alloc->object_alloc<node_t>(1);
	alloc->construct<node_t>(node, ang::move(value));
	if (last)
	{
		if (_last) _last->next(node);
		else _first = node;
		node->prev(_last);
		_last = node;
	}
	else
	{
		if(_first)_first->prev(node);
		else _last = node;
		node->next(_first);
		_first = node;
	}
	_count++;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::insert(ang::index_t idx, T value)
{
	if (idx == 0U)
	{
		append(ang::move(value), false);
		return true;
	}
	else if (idx >= _count)
	{
		append(ang::move(value), true);
		return true;
	}

	memory::iallocator* alloc = get_allocator();
	node_ptr_t node, new_node = alloc->object_alloc<node_t>(1);
	alloc->construct<node_t>(new_node, ang::move(value));

	if (idx <= _count / 2)
	{
		node = _first;
		for (auto i = 0U; i < idx; ++i)
			node = node->next();
	}
	else
	{
		node = _last;
		for (auto i = _count - 1; i > idx; --i)
			node = node->prev();
	}

	if (node->prev()) node->prev()->next(new_node);
	else _first = new_node;
	new_node->prev(node->prev());
	new_node->next(node);
	node->prev(new_node);

	_count++;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::insert(ang::collections::iterator<T> it, T value)
{
	if (it.parent() != this || it.current() == null)
		return false;
	memory::iallocator* alloc = get_allocator();
	node_ptr_t node, new_node = alloc->object_alloc<node_t>(1);
	alloc->construct<node_t>(new_node, ang::move(value));
	node = reinterpret_cast<node_ptr_t>(it.current());

	if (node->prev())
		node->prev()->next(new_node);
	else _first = new_node;
	new_node->prev(node->prev());
	new_node->next(node);
	node->prev(new_node);

	_count++;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::pop(ang::bool_t last)
{
	if (_count == 0)
		return false;
	node_ptr_t node;

	if (last)
	{
		node = _last;
		_last = _last->prev();
		if (_last)_last->next(null);
		else _first = null;
	}
	else
	{
		node = _first;
		_first = _first->next();
		if (_first)_first->prev(null);
		else _last = null;
	}

	get_allocator()->destruct(node);
	get_allocator()->memory_release(node);
	_count--;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::pop(T& value, ang::bool_t last)
{
	if (_count == 0)
		return false;
	node_ptr_t node;

	if (last)
	{
		node = _last;
		_last = _last->prev();
		if (_last)_last->next(null);
		else _first = null;
	}
	else
	{
		node = _first;
		_first = _first->next();
		if (_first)_first->prev(null);
		else _last = null;
	}
	value = ang::move(node->value());
	get_allocator()->destruct(node);
	get_allocator()->memory_release(node);
	_count--;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::pop_at(ang::index_t idx)
{
	if (idx == 0U)
	{
		pop(false);
		return true;
	}
	else if (idx >= _count)
	{
		pop(true);
		return true;
	}

	memory::iallocator* alloc = get_allocator();
	node_ptr_t node;
	if (idx <= _count / 2)
	{
		node = _first;
		for (auto i = 0U; i < idx; ++i)
			node = node->next();
	}
	else
	{
		node = _last;
		for (auto i = _count - 1; i > idx; --i)
			node = node->prev();
	}


	if (node->next()) node->next()->prev(node->prev());
	else _last = node->prev();
	node->prev()->next(node->next());

	alloc->destruct(node);
	alloc->memory_release(node);
	_count--;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::pop_at(ang::collections::iterator<T> it)
{
	if (!it.is_valid() || it.parent() != this)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	node_ptr_t _next = node->next();
	node_ptr_t _prev = node->prev();
	if (_prev) _prev->next(_next);
	else _first = _next;
	if (_next) _next->prev(_prev);
	else _last = _prev;
	get_allocator()->destruct(node);
	get_allocator()->memory_release(node);
	_count--;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::pop_at(ang::index_t idx, T& value)
{
	if (idx == 0U)
	{
		pop(false);
		return true;
	}
	else if (idx >= _count)
	{
		pop(true);
		return true;
	}

	memory::iallocator* alloc = get_allocator();
	node_ptr_t node, new_node = alloc->object_alloc<node_t>(1);
	alloc->construct<node_t>(new_node, ang::move(value));

	if (idx <= _count / 2)
	{
		node = _first;
		for (auto i = 0U; i < idx; ++i)
			node = node->next();
	}
	else
	{
		node = _last;
		for (auto i = _count - 1; i > idx; --i)
			node = node->prev();
	}


	if (node->next()) node->next()->prev(node->prev());
	else _last = node->prev();
	node->prev()->next(node->next());

	value = ang::move(node->value());
	get_allocator()->destruct(node);
	get_allocator()->memory_release(node);
	_count--;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::pop_at(ang::collections::iterator<T> it, T& value)
{
	if (!it.is_valid() || it.parent() != this)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	node_ptr_t _next = node->next();
	node_ptr_t _prev = node->prev();
	if (_prev) _prev->next(_next);
	else _first = _next;
	if (_next) _next->prev(_prev);
	else _last = _prev;
	value = ang::move(node->value());
	get_allocator()->destruct(node);
	get_allocator()->memory_release(node);
	_count--;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<list_data<T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::icollection<T>::is_kind_of(name)
		|| ang::collections::ilist<T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::list_data<T>::class_name()
{
	static ang::astring className = ang::astring("ang::collections::list<") + type_name<T>() + ">";
	return className->cstr();
}

template<typename T>
inline ang::type_name_t ang::collections::list_data<T>::object_name()const
{
	return ang::collections::list_data<T>::class_name();
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_name<list_data<T>>())
	{
		*out = static_cast<ang::collections::list_data<T>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	else if (ang::collections::icollection<T>::query_object(className, out))
	{
		return true;
	}
	else if (ang::collections::ilist<T>::query_object(className, out))
	{
		return true;
	}
	return false;
}

template<typename T>
inline void ang::collections::list_data<T>::clean()
{
	node_ptr_t del, node = _first;
	auto alloc = get_allocator();
	while (node) {
		del = node;
		node = node->next();
		alloc->destruct(node);
		alloc->memory_release(node);
	}
	_first = null;
	_last = null;
	_count = 0;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::operator == (const ang::collections::list_data<T>& ar)
{
	if (conuter() != ar.conuter())
		return false;

	for (node_ptr_t n1 = _first, n2 = ar._first; n1 && n2; n1=n1->next(), n2 = n2->next())
		if (n1->value() != n2->value())
			return false;
	return true;
}

template<typename T>
inline ang::bool_t ang::collections::list_data<T>::operator != (const ang::collections::list_data<T>& ar)
{
	return !operator == (ar);
}


template<typename T>
inline ang::bool_t ang::collections::list_data<T>::realloc(ang::memory::iallocator* alloc)
{
	if (counter() == 0U)
		return true;

	memory::iallocator* this_alloc = get_allocator();

	node_ptr_t node, last, del;
	del = _first;
	node = del->next();
	_first = last = alloc->object_alloc<node_t>(1);
	alloc->construct(last, ang::move(del->value()));
	this_alloc->destruct(del);
	this_alloc->memory_release(del);

	while (node)
	{
		del = node;
		node = node->next();
		last->next(alloc->object_alloc<node_t>(1));
		alloc->construct(last->next(), ang::move(del->value()));
		last->next()->prev(last);
		this_alloc->destruct(del);
		this_alloc->memory_release(del);
		last = last->next();
	}

	_last = last;
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// list_data<object_wrapper<T>> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// list_data<intf_wrapper<T>> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(ang::collections::list_data<T>* ptr)
	: object_wrapper<ang::collections::list_data<T>>()
{
	set(ptr);
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(std::initializer_list<data_type> list)
	: object_wrapper<ang::collections::list_data<T>>()
{
	set(new collections::list_data<T>(ang::move(list)));
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: object_wrapper<ang::collections::list_data<T>>()
{
	set(new collections::list_data<T>(store));
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(ang::uint_t size, data_type const* ar)
	: object_wrapper<ang::collections::list_data<T>>()
{
	set(new collections::list_data<T>(size, ar));
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(ang::object_wrapper<ang::collections::list_data<T>> && other)
	: object_wrapper<collections::list_data<T>>()
{
	collections::list_data<T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(ang::object_wrapper<ang::collections::list_data<T>> const& other)
	: object_wrapper<collections::list_data<T>>()
{
	set(other.get());
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::~object_wrapper()
{
	clean();
}

template<typename T>
inline void ang::object_wrapper<ang::collections::list_data<T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::list_data<T>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T>
inline ang::bool_t ang::object_wrapper<ang::collections::list_data<T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::list_data<T>* ang::object_wrapper<ang::collections::list_data<T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::collections::list_data<T>>::set(ang::collections::list_data<T>* ptr)
{
	ang::collections::list_data<T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::collections::list_data<T>** ang::object_wrapper<ang::collections::list_data<T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>& ang::object_wrapper<ang::collections::list_data<T>>::operator = (ang::collections::list_data<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>& ang::object_wrapper<ang::collections::list_data<T>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>& ang::object_wrapper<ang::collections::list_data<T>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (_ptr == null)
		set(new collections::list_data<T>(items));
	else
		_ptr->copy(items);
	return *this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>& ang::object_wrapper<ang::collections::list_data<T>>::operator = (ang::object_wrapper<ang::collections::list_data<T>> && other)
{
	collections::list_data<T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>& ang::object_wrapper<ang::collections::list_data<T>>::operator = (ang::object_wrapper<ang::collections::list_data<T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>& ang::object_wrapper<ang::collections::list_data<T>>::operator += (T value)
{
	if (is_empty())
		set(new ang::collections::list_data<T>());
	get()->append(ang::move(value));
	return*this;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::collections::list_data<T>> ang::object_wrapper<ang::collections::list_data<T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::list_data<T> * ang::object_wrapper<ang::collections::list_data<T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::list_data<T> const* ang::object_wrapper<ang::collections::list_data<T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::operator ang::collections::list_data<T> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::operator ang::collections::list_data<T> const* (void)const
{
	return get();
}

template<typename T>
inline typename ang::collections::list_data<T>::type const& ang::object_wrapper<ang::collections::list_data<T>>::operator[](ang::index_t index)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (index >= _ptr->counter()) throw(exception_t(except_code::array_overflow));
#endif
	return  collections::list_node_t<T>(_ptr->find_index(index).current())->value();
}

template<typename T>
inline typename ang::collections::list_data<T>::type & ang::object_wrapper<ang::collections::list_data<T>>::operator[](ang::index_t index)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (index >= _ptr->counter()) throw(exception_t(except_code::array_overflow));
#endif
	return  collections::list_node_t<T>(_ptr->find_index(index).current())->value();
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__ANG_LIST_HPP__

