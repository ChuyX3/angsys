#ifndef __ANG_STACK_HPP__
#error Can't include stack.inl, please include stack.hpp inside
#elif !defined __ANG_STACK_INL__
#define __ANG_STACK_INL__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// stack_object<T, allocator> class implementation //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template<typename> class allocator>
inline ang::collections::stack_object<T, allocator>::stack_object()
	: object()
	, _size(0)
	, _head(null)
	, _tail(null)
{

}

template<typename T, template<typename> class allocator>  template<typename U>
inline ang::collections::stack_object<T, allocator>::stack_object(ang::initializer_list_t<U> list)
	: stack_object()
{
	for (auto it = list.begin(); it != list.end(); ++it)
		push(*it);
}


template<typename T, template<typename> class allocator>
inline ang::collections::stack_object<T, allocator>::stack_object(const ang::nullptr_t&)
	: stack_object()
{

}

template<typename T, template<typename> class allocator>
inline ang::collections::stack_object<T, allocator>::stack_object(ang::collections::stack_object<T, allocator>&& st)
	: stack_object()
{
	_size = st._size;
	_head = st._head;
	_tail = st._tail;
	st._size = 0;
	st._head = null;
	st._tail = null;
}

template<typename T, template<typename> class allocator>
inline ang::collections::stack_object<T, allocator>::stack_object(const ang::collections::stack_object<T, allocator>& ar)
	: stack_object()
{
	extend(&ar);
}

template<typename T, template<typename> class allocator>
inline ang::collections::stack_object<T, allocator>::stack_object(const ang::collections::stack_object<T, allocator>* ar)
	: stack_object()
{
	extend(ar);
}

template<typename T, template<typename> class allocator>
inline ang::collections::stack_object<T, allocator>::stack_object(const ang::collections::ienum<T>* items)
	: stack_object()
{
	extend(items);
}

template<typename T, template<typename> class allocator>
inline ang::collections::stack_object<T, allocator>::~stack_object()
{
	clean();
}

template<typename T, template <typename> class allocator>
inline ang::type_name_t ang::collections::stack_object<T, allocator>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::collections::stack<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::is_inherited_of(ang::type_name_t name)
{
	if (name == type_of<stack_object<T, allocator>>()
		|| ang::object::is_inherited_of(name)
		|| ang::collections::isequence<T>::is_inherited_of(name))
		return true;
	return false;
}

template<typename T, template<typename> class allocator>
inline ang::type_name_t ang::collections::stack_object<T, allocator>::object_name()const
{
	return ang::collections::stack_object<T, allocator>::class_name();
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::is_kind_of(ang::type_name_t name)const
{
	if (name == type_of<stack_object<T, allocator>>()
		|| ang::object::is_kind_of(name)
		|| ang::collections::isequence<T>::is_kind_of(name))
		return true;
	return false;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (className == type_of<stack_object<T, allocator>>())
	{
		*out = static_cast<ang::collections::stack_object<T, allocator>*>(this);
		return true;
	}
	else if (ang::object::query_object(className, out))
	{
		return true;
	}
	else if (ang::collections::isequence<T>::query_object(className, out))
	{
		return true;
	}
	return false;
}


template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::is_empty()const
{
	return size() == 0;
}



template<typename T, template<typename> class allocator>
inline wsize ang::collections::stack_object<T, allocator>::size()const
{
	return _size;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::move(ang::collections::stack_object<T, allocator>& st)
{
	if (&st == this)
		return false;
	clean();
	_size = st._size;
	_head = st._head;
	_tail = st._tail;
	st._size = 0;
	st._head = null;
	st._tail = null;
	return true;
}

template<typename T, template<typename> class allocator>
inline wsize ang::collections::stack_object<T, allocator>::counter()const
{
	return _size;
}

template<typename T, template<typename> class allocator>
inline T& ang::collections::stack_object<T, allocator>::at(ang::collections::base_iterator<T> const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw(exception_t(except_code::invalid_memory));
	if (this != it.parent() || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	return node_ptr_t(it.offset())->value;
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::stack_object<T, allocator>::begin()
{
	return forward_iterator_t(const_cast<stack_object*>(this), position_t(_head));
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::stack_object<T, allocator>::end()
{
	return forward_iterator_t(const_cast<stack_object*>(this), position_t(null));
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::stack_object<T, allocator>::begin()const
{
	return const_forward_iterator_t(const_cast<stack_object*>(this), position_t(_head));
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::stack_object<T, allocator>::end()const
{
	return const_forward_iterator_t(const_cast<stack_object*>(this), position_t(null));
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<T> ang::collections::stack_object<T, allocator>::last()
{
	return forward_iterator_t(const_cast<stack_object*>(this), position_t(_tail));
}

template<typename T, template<typename> class allocator>
inline ang::collections::forward_iterator<const T> ang::collections::stack_object<T, allocator>::last()const
{
	return const_forward_iterator_t(const_cast<stack_object*>(this), position_t(_tail));
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::increase(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = node_ptr_t(it.current());
	if (node == null)return false;
	it.current(node->next);
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::increase(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw(exception_t(except_code::invalid_param));
#endif
	node_ptr_t node = node_ptr_t(it.current());
	while (node != null && val > 0) {
		node = node->next;
		val--;
	}
	it.current(node);
	return node ? true : false;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::decrease(ang::collections::base_iterator<T>& it)const
{
#ifdef DEBUG_SAFE_CODE
	throw(exception_t(except_code::invalid_access));
#endif
	return false;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::decrease(ang::collections::base_iterator<T>& it, int val)const
{
#ifdef DEBUG_SAFE_CODE
	throw(exception_t(except_code::invalid_access));
#endif
	return false;
}

template<typename T, template<typename> class allocator>
inline ang::comparision_result_t ang::collections::stack_object<T, allocator>::compare(const ang::object& obj)const
{
	if (obj.is_kind_of(class_name()))
	{
		stack_object<T, allocator>const& other = static_cast<stack_object<T, allocator>const&>(obj);
		if (counter() != other.counter())
			return comparision_result::diferent;
		else for (node_ptr_t it1 = _head, it2 = other._head; it1 != null && it2 != null; it1 = it1->next, it2 = it2->next)
			if (comparision_operations<T, T>::template compare<comparision_diferent>(it1->value, it2->value))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}

template<typename T, template<typename> class allocator>
inline void ang::collections::stack_object<T, allocator>::extend(const ang::collections::ienum<T>* items)
{
	if (!items) return;
	for (T const& value : *items)
		push(value);
}

template<typename T, template<typename> class allocator>
inline void ang::collections::stack_object<T, allocator>::push(T const& value)
{
	node_ptr_t node = allocator<node_t>::template alloc_and_construct<T const&>(value);

	if (_head == null)
	{
		_head = node;
		_tail = node;
	}
	else
	{
		node->next = _head;
		_head = node;
	}
	_size++;
}


template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::pop()
{
	if (_head == null)
		return false;
	node_ptr_t node = _head;
	_head = node->next;
	allocator<node_t>::destruct_and_free(node);
	if (_head == null) _tail = null;
	_size--;
	return true;
}

template<typename T, template<typename> class allocator>
inline bool ang::collections::stack_object<T, allocator>::pop(T& value)
{
	if (_head == null)
		return false;
	node_ptr_t node = _head;
	_head = node->next;
	value = ang::move(node->value);
	allocator<node_t>::destruct_and_free(node);
	if (_head == null) _tail = null;
	_size--;
	return true;
}

template<typename T, template<typename> class allocator>
inline void ang::collections::stack_object<T, allocator>::clean()
{
	node_ptr_t temp, node = _head;
	while (node != null) {
		temp = node;
		node = node->next;
		allocator<node_t>::destruct_and_free(temp);
	}
	_head = null;
	_tail = null;
	_size = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::object_wrapper()
	: _ptr(null)
{

}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::object_wrapper(ang::collections::stack_object<T, allocator>* ptr)
	: object_wrapper<ang::collections::stack_object<T, allocator>>()
{
	set(ptr);
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper<ang::collections::stack_object<T, allocator>>()
{
}

template<typename T, template <typename> class allocator> template<typename U>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::object_wrapper(std::initializer_list<U> list)
	: object_wrapper<ang::collections::stack_object<T, allocator>>()
{
	set(new collections::stack_object<T, allocator>(ang::move(list)));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: object_wrapper<ang::collections::stack_object<T, allocator>>()
{
	set(new collections::stack_object<T, allocator>(store));
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::stack_object<T, allocator>> && other)
	: object_wrapper<collections::stack_object<T, allocator>>()
{
	collections::stack_object<T, allocator> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::object_wrapper(ang::object_wrapper<ang::collections::stack_object<T, allocator>> const& other)
	: object_wrapper<collections::stack_object<T, allocator>>()
{
	set(other.get());
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::~object_wrapper()
{
	clean();
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::stack_object<T, allocator>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::stack_object<T, allocator>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T, template <typename> class allocator>
inline bool ang::object_wrapper<ang::collections::stack_object<T, allocator>>::is_empty()const
{
	return _ptr == null;
}

template<typename T, template <typename> class allocator>
inline ang::collections::stack_object<T, allocator>* ang::object_wrapper<ang::collections::stack_object<T, allocator>>::get(void)const
{
	return _ptr;
}

template<typename T, template <typename> class allocator>
inline void ang::object_wrapper<ang::collections::stack_object<T, allocator>>::set(ang::collections::stack_object<T, allocator>* ptr)
{
	ang::collections::stack_object<T, allocator> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class allocator>
inline ang::collections::stack_object<T, allocator>** ang::object_wrapper<ang::collections::stack_object<T, allocator>>::addres_of(void)
{
	return &_ptr;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>& ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator = (ang::collections::stack_object<T, allocator>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>& ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator = (const std::nullptr_t&)
{
	clean();
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>& ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (_ptr == null)
		set(new collections::stack_object<T, allocator>(items));
	else
		_ptr->copy(items);
	return *this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>& ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator = (ang::object_wrapper<ang::collections::stack_object<T, allocator>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>& ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator = (ang::object_wrapper<ang::collections::stack_object<T, allocator>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper_ptr<ang::collections::stack_object<T, allocator>> ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator & (void)
{
	return this;
}

template<typename T, template <typename> class allocator>
inline ang::collections::stack_object<T, allocator> * ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator -> (void)
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::collections::stack_object<T, allocator> const* ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator -> (void)const
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator ang::collections::stack_object<T, allocator> * (void)
{
	return get();
}

template<typename T, template <typename> class allocator>
inline ang::object_wrapper<ang::collections::stack_object<T, allocator>>::operator ang::collections::stack_object<T, allocator> const* (void)const
{
	return get();
}

#endif//__ANG_STACK_INL__
