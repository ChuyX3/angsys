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
	, _head(null)
	, _tail(null)
{
	allocator = memory::allocator_manager::get_allocator(0);
}

template<typename T>
inline ang::collections::list_data<T>::list_data(ang::initializer_list_t<T> list)
	: list_data()
{
	for (auto it = list.begin(); it != list.end(); ++it)
		append(*it);
}

template<typename T>
inline ang::collections::list_data<T>::list_data(const ang::nullptr_t&)
	: list_data()
{

}

template<typename T>
inline ang::collections::list_data<T>::list_data(ang::collections::list_data<T>&& ar)
	: list_data()
{
	_count = ar._count;
	_head = ar._head;
	_tail = ar._tail;
	allocator = ar.allocator;
	ar._count = 0;
	ar._head = null;
	ar._tail = null;
}

template<typename T>
inline ang::collections::list_data<T>::list_data(const ang::collections::list_data<T>& ar)
	: list_data()
{
	copy(static_cast<const ienum_t&>(ar));
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
inline ang::collections::list_data<T>::~list_data()
{
	clean();
}

template<typename T>
inline bool ang::collections::list_data<T>::is_empty()const
{
	return size() == 0;
}

template<typename T>
inline uint ang::collections::list_data<T>::size()const
{
	return _count;
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
inline bool ang::collections::list_data<T>::has_items()const
{
	return bool(size() != 0);
}

template<typename T>
inline bool ang::collections::list_data<T>::move(ang::collections::list_data<T>& ar)
{
	if (&ar == this)
		return false;
	clean();
	_count = ar._count;
	_head = ar._head;
	_tail = ar._tail;
	allocator = ar.allocator;
	ar._head = null;
	ar._tail = null;
	return true;
}

template<typename T>
inline bool ang::collections::list_data<T>::copy(const ang::collections::ienum<T>* _items)
{
	if (!_items)
		return false;
	for (auto it = _items->begin(); it.is_valid(); ++it)
		append(const_cast<T&>(*it));	
	return true;
}

template<typename T>
inline T& ang::collections::list_data<T>::at(const ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	return reinterpret_cast<node_ptr_t>(it.current())->value();
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::find_index(index idx)const
{
	if (idx >= _count)
		return  iterator_t(const_cast<self_t*>(this), null, 0);

	node_ptr_t node = _head;
	for (index i = 0; node != null && i < idx; node = node->next(), ++i);
	return iterator_t(const_cast<self_t*>(this), (pointer)node, 0);
}

template<typename T>
inline index ang::collections::list_data<T>::index_of(ang::collections::iterator<T> it)const
{
	if (!it.is_valid() || it.parent() != this)
		return invalid_index;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	index i = 0;
	while (node) { node->prev(); i++; };
	return i;
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::find(T const& datum, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (iterator_t it = begin(); it.is_valid(); ++it)
		{
			if (*it == datum)
				return it;
		}
		else for (iterator_t it = end(); it.is_valid(); --it)
		{
			if (*it == datum)
				return it;
		}
	}
	return iterator_t(const_cast<self_t*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::find(T const& datum, ang::collections::iterator<T> nextTo, bool invert)const
{
	if (!is_empty() && nextTo.parent() == this)
	{
		if (invert)for (iterator_t it = nextTo.is_valid() ? nextTo : begin(); it.is_valid(); ++it)
		{
			if ((T const&)*it == datum)
				return it;
		}

		else for (iterator_t it = nextTo.is_valid() ? nextTo : end(); it.is_valid(); --it)
		{
			if ((T const&)*it == datum)
				return it;
		}
	}
	return iterator_t(const_cast<list_data*>(this), 0);
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::begin()const
{
	return iterator_t(const_cast<list_data*>(this), _head);
}

template<typename T>
inline ang::collections::iterator<T> ang::collections::list_data<T>::end()const
{
	return iterator_t(const_cast<list_data*>(this), _tail);
}

template<typename T>
inline bool ang::collections::list_data<T>::next(ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current())->next();
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::list_data<T>::next(ang::collections::iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	for (int i = 0; i < val && node != null; node = node->next());
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::list_data<T>::prev(ang::collections::iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current())->prev();
	it.current(node);
	return node != null;
}

template<typename T>
inline bool ang::collections::list_data<T>::prev(ang::collections::iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	for (int i = 0; i < val && node != null; node = node->prev());
	it.current(node);
	return node != null;
}

template<typename T>
inline uint ang::collections::list_data<T>::counter()const
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
inline void ang::collections::list_data<T>::append(T const& value, bool last)
{
	node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
	allocator->construct(_new_node, value);
	if (last)
	{	
		node_ptr_t _old_node = _tail;
		_tail = _new_node;
		if (_old_node) {
			_tail->prev(_old_node);
			_old_node->next(_tail);
		}
		else
			_head = _new_node;
	}
	else
	{
		node_ptr_t _old_node = _head;
		_head = _new_node;
		if (_old_node) {
			_head->next(_old_node);
			_old_node->prev(_head);
		}
		else
			_tail = _new_node;
	
	}
	_count++;
}

template<typename T>
inline bool ang::collections::list_data<T>::insert(index idx, T const& value)
{
	if (idx == 0U)
	{
		append(value, false);
		return true;
	}
	else if (idx >= _count)
	{
		append(value, true);
		return true;
	}
	else if (idx <= (_count / 2))
	{
		node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
		allocator->construct(_new_node, value);
		node_ptr_t node = _head;
		for (index i = 0; i < idx; ++i)
			node = node->next();
		_new_node->next(node->next());
		_new_node->prev(node);
		node->next()->prev(_new_node);
		node->next(_new_node);
	}
	else
	{
		idx = _count - idx;

		node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
		allocator->construct(_new_node, value);
		node_ptr_t node = _tail;
		for (index i = 0; i <= idx; ++i)
			node = node->prev();
		_new_node->next(node->next());
		_new_node->prev(node);
		node->next()->prev(_new_node);
		node->next(_new_node);
	}
	_count++;
	return true;
}

template<typename T>
inline bool ang::collections::list_data<T>::insert(ang::collections::iterator<T> it, T const& value)
{
	if (it.parent() != this || it.current() == null)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	node_ptr_t _new_node = allocator->object_alloc<node_t>(1);
	allocator->construct(_new_node, value);
	_new_node->next(node);
	_new_node->prev(node->prev());
	if (node->prev() == null)
		_head = _new_node;
	else
		node->prev()->next(_new_node);
	node->prev(_new_node);
	_count++;
	return true;
}

template<typename T>
inline bool ang::collections::list_data<T>::pop(bool last)
{
	if (_count == 0)
		return false;
	if (!last) {
		node_ptr_t node = _tail;
		_tail = node->prev();
		if (_tail) _tail->next(null);
		else _head = null;
		allocator->destruct(node);
		allocator->memory_release(node);
	}
	else
	{
		node_ptr_t node = _head;
		_head = node->next();
		if (_head) _head->prev(null);
		else _tail = null;
		allocator->destruct(node);
		allocator->memory_release(node);
	}
	_count--;
	return true;
}

template<typename T>
inline bool ang::collections::list_data<T>::pop(T& value, bool last)
{
	if (_count == 0)
		return false;
	if (!last) {
		node_ptr_t node = _tail;
		_tail = node->prev();
		if (_tail) _tail->next(null);
		else _head = null;
		value = ang::move(node->value());
		allocator->destruct(node);
		allocator->memory_release(node);
	}
	else
	{
		node_ptr_t node = _head;
		_head = node->next();
		if (_head) _head->prev(null);
		else _tail = null;
		value = ang::move(node->value());
		allocator->destruct(node);
		allocator->memory_release(node);
	}
	_count--;
	return true;
}

template<typename T>
inline bool ang::collections::list_data<T>::pop_at(index idx)
{
	if (idx >= _count)
		return false;
	else if (idx <= (_count / 2))
	{
		node_ptr_t node = _head;
		for (index i = 0; i < idx; ++i)
			node = node->next();
		if(node->prev()) node->prev()->next(node->next());
		else _head = node->next();
		if (node->next()) node->next()->prev(node->prev());
		else _tail = node->prev();
		allocator->destruct(node);
		allocator->memory_release(node);
	}
	else
	{
		idx = _count - idx;
		node_ptr_t node = _tail;
		for (index i = 0; i <= idx; ++i)
			node = node->prev();
		if (node->prev()) node->prev()->next(node->next());
		else _head = node->next();
		if (node->next()) node->next()->prev(node->prev());
		else _tail = node->prev();
		allocator->destruct(node);
		allocator->memory_release(node);
	}
	_count--;
}

template<typename T>
inline bool ang::collections::list_data<T>::pop_at(ang::collections::iterator<T> it)
{
	if (it.parent() != this || it.current() == null)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	if (node->prev()) node->prev()->next(node->next());
	else _head = node->next();
	if (node->next()) node->next()->prev(node->prev());
	else _tail = node->prev();
	allocator->destruct(node);
	allocator->memory_release(node);
	return true;
}

template<typename T>
inline bool ang::collections::list_data<T>::pop_at(index idx, T& value)
{
	if (idx >= _count)
		return false;
	else if (idx <= (_count / 2))
	{
		node_ptr_t node = _head;
		for (index i = 0; i < idx; ++i)
			node = node->next();
		if (node->prev()) node->prev()->next(node->next());
		else _head = node->next();
		if (node->next()) node->next()->prev(node->prev());
		else _tail = node->prev();
		value = ang::move(node->value());
		allocator->destruct(node);
		allocator->memory_release(node);
	}
	else
	{
		idx = _count - idx;
		node_ptr_t node = _tail;
		for (index i = 0; i <= idx; ++i)
			node = node->prev();
		if (node->prev()) node->prev()->next(node->next());
		else _head = node->next();
		if (node->next()) node->next()->prev(node->prev());
		else _tail = node->prev();
		value = ang::move(node->value());
		allocator->destruct(node);
		allocator->memory_release(node);
	}
	_count--;
}

template<typename T>
inline bool ang::collections::list_data<T>::pop_at(ang::collections::iterator<T> it, T& value)
{
	if (it.parent() != this || it.current() == null)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	if (node->prev()) node->prev()->next(node->next());
	else _head = node->next();
	if (node->next()) node->next()->prev(node->prev());
	else _tail = node->prev();
	value = ang::move(node->value());
	allocator->destruct(node);
	allocator->memory_release(node);
	return true;
}

template<typename T>
inline bool ang::collections::list_data<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_name<list_data<T>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::ilist<T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T>
inline ang::type_name_t ang::collections::list_data<T>::class_name()
{
	static string className = string("ang::collections::list<") + type_name<T>() + ">";
	return className->cstr();
}

template<typename T>
inline ang::type_name_t ang::collections::list_data<T>::object_name()const
{
	return ang::collections::list_data<T>::class_name();
}

template<typename T>
inline bool ang::collections::list_data<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
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
	else if (ang::collections::ilist<T>::query_object(className, out))
	{
		return true;
	}
	return false;
}

template<typename T>
inline void ang::collections::list_data<T>::clean()
{
	if (_head != null)
	{
		node_ptr_t node = _head;
		while (node != null)
		{
			node_ptr_t del = node;
			node = node->next();
			allocator->destruct(del);
			allocator->memory_release(del);
		}
	}
	_head = null;
	_tail = null;
	_count = 0;
}

template<typename T>
inline bool ang::collections::list_data<T>::operator == (const ang::collections::list_data<T>& other)
{
	if (_count != other._count)
		return false;
	for (node_ptr_t n1 = _head, n2 = other._head; n1 != null && n2 != null; n1 = n1->next(), n2 = n2->next())
		if (n1->value() != n2->value())
			return false;
	return true;
}

template<typename T>
inline bool ang::collections::list_data<T>::operator != (const ang::collections::list_data<T>& other)
{
	return !operator == (ar);
}

template<typename T>
inline bool ang::collections::list_data<T>::realloc(ang::memory::iallocator* alloc)
{
	memory::iallocator* this_alloc = get_allocator();
	node_ptr_t _temp, node = _head;
	_head = null;
	_tail = null;
	_count = 0;
	while(node!= null)
	{
		_temp = alloc->object_alloc<node_t>(1);
		alloc->construct(_temp, ang::move(node->value()));
		_temp->prev(_tail);
		if (_tail) _tail->next(_temp);
		else _head = _temp;
		_tail = _temp;
		_temp = node;
		node = node->next();
		this_alloc->destruct(_temp);
		this_alloc->memory_release(_temp);
		_count++;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T>
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(ang::nullptr_t const&)
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
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(ang::initializer_list_t<data_type> list)
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
inline ang::object_wrapper<ang::collections::list_data<T>>::object_wrapper(ang::static_array<data_type> arr)
	: object_wrapper<ang::collections::list_data<T>>()
{
	set(new collections::list_data<T>());
	for (index i = 0; i < arr->size(); ++i)
		_ptr->append(arr[i]);
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
inline bool ang::object_wrapper<ang::collections::list_data<T>>::is_empty()const
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
inline ang::object_wrapper<ang::collections::list_data<T>>& ang::object_wrapper<ang::collections::list_data<T>>::operator = (const ang::nullptr_t&)
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
inline ang::object_wrapper<ang::collections::list_data<T>>& ang::object_wrapper<ang::collections::list_data<T>>::operator += (T const& value)
{
	if (is_empty())
		set(new ang::collections::list_data<T>());
	get()->append(value);
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


#endif//__ANG_LIST_HPP__

