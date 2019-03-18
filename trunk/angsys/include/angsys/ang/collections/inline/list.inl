#ifndef __ANG_COLLECTION_LIST_H__
#error Can't include binary_tree.h, please include binary_tree.h inside
#elif !defined __ANG_COLLECTION_LIST_INL__
#define __ANG_COLLECTION_LIST_INL__

template<typename T, template<typename> class A>
inline ang::collections::list_object<T, A>::list_object()
	: m_count(0)
	, m_head(null)
	, m_tail(null)
{

}

template<typename T, template<typename> class A>
inline ang::collections::list_object<T, A>::list_object(const ang::nullptr_t&)
	: list_object()
{

}

template<typename T, template<typename> class A>
inline ang::collections::list_object<T, A>::list_object(ang::collections::list_object<T, A>&& other)
	: list_object()
{
	m_count = other.m_count;
	m_head = other.m_head;
	m_tail = other.m_tail;

	other.m_count = 0;
	other.m_head = null;
	other.m_tail = null;
}

template<typename T, template<typename> class A>
inline ang::collections::list_object<T, A>::list_object(ang::collections::list_object<T, A>const& other)
	: list_object()
{
	copy((ienum_t const*)&other);
}

template<typename T, template<typename> class A>
inline ang::collections::list_object<T, A>::list_object(ang::collections::list_object<T, A>const* other)
	: list_object()
{
	if(other != null)
		copy((ienum_t const*)other);
}

template<typename T, template<typename> class A>
inline ang::collections::list_object<T, A>::list_object(ang::collections::ienum<T>const* other)
	: list_object()
{
	if (other != null)
		copy(other);
}

template<typename T, template<typename> class A> template<typename U>
inline ang::collections::list_object<T, A>::list_object(ang::initializer_list<U> list)
	: list_object()
{
	for (U const& item : list) {
		push(item);
	}
}

template<typename T, template<typename> class A> template<typename U>
inline ang::collections::list_object<T, A>::list_object(ang::array_view<U> const& list)
	: list_object()
{
	for (U const& item : list) {
		push(item);
	}
}

template<typename T, template<typename> class A> template<typename U, template<typename> class A2>
inline ang::collections::list_object<T, A>::list_object(ang::scope_array<U,A2> const& list)
	: list_object()
{
	for (U const& item : list) {
		push(item);
	}
}

template<typename T, template<typename> class A> template<typename U, wsize SIZE>
inline ang::collections::list_object<T, A>::list_object(ang::stack_array<U, SIZE> const& list)
	: list_object()
{
	for (U const& item : list) {
		push(item);
	}
}


template<typename T, template<typename> class A>
inline ang::collections::list_object<T, A>::~list_object()
{
}

template<typename T, template <typename> class A>
inline ang::rtti_t const& ang::collections::list_object<T, A>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::collections::list<"_s) += ang::type_of<T>().type_name()) += ">"_s);
	static ang::rtti_t const* parents[] = { &runtime::type_of<ilist<T>>() };
	static ang::rtti_t const& info = rtti::regist(name, genre::class_type, size_of<ang::collections::list_object<T, A>>(), align_of<ang::collections::list_object<T, A>>(), parents, &intf::default_query_interface);
	return info;
}


template<typename T, template <typename> class A>
inline ang::rtti_t const& ang::collections::list_object<T, A>::runtime_info()const
{
	return class_info();
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<list_object<T, A>*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == ang::type_of<ilist<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ilist<T>*>(this);
		return true;
	}
	else if (id.type_id() == ang::type_of<iarray<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<iarray<T>*>(this);
		return true;
	}
	else if (id.type_id() == ang::type_of<ienum<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum<T>*>(this);
		return true;
	}
	return false;
}

template<typename T, template <typename> class A>
inline ang::comparision_result_t ang::collections::list_object<T, A>::compare(const ang::object* obj)const
{
	windex i = 0;
	intf_wrapper<ienum<T>> items = ang::interface_cast<ienum<T>>(const_cast<object*>(obj));

	if (!items.is_empty())
	{
		if (counter() > items->counter())
			return comparision_result::mayor;
		else if (counter() < items->counter())
			return comparision_result::minor;
		else for (const_forward_iterator_t it1 = begin(), it2 = items->begin(); it1 != end() && it2 != items->end(); ++it1, ++it2)
			if (logic_operation<T, T, logic_operation_type::same>::operate(*it1, *it2))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}

template<typename T, template <typename> class A>
inline void ang::collections::list_object<T, A>::clear()
{
	while (pop());
	m_head = null;
	m_tail = null;
	m_count = 0;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::is_empty()const
{
	return m_count == 0;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::move(list_object<T, A>& other)
{
	if (&other == this)
		return false;
	clear();

	m_count = other.m_count;
	m_head = other.m_head;
	m_tail = other.m_tail;

	other.m_count = 0;
	other.m_head = null;
	other.m_tail = null;
	
	return true;
}

template<typename T, template <typename> class A>template<typename U> 
inline void ang::collections::list_object<T, A>::copy(ang::array_view<U>const& arr)
{
	clear();
	for (U const& item : arr)
		push(item);
}

template<typename T, template <typename> class A>template<typename U, template<typename> class A2>
inline void ang::collections::list_object<T, A>::copy(ang::scope_array<U, A2>const& arr)
{
	clear();
	for (U const& item : arr)
		push(item);
}

template<typename T, template <typename> class A>template<typename U, wsize SIZE> 
inline void ang::collections::list_object<T, A>::copy(ang::stack_array<U, SIZE> const& arr)
{
	clear();
	for (U const& item : arr)
		push(item);
}

template<typename T, template <typename> class A>template<typename U> 
inline void ang::collections::list_object<T, A>::extend(ang::array_view<U>const& arr)
{
	for (U const& item : arr)
		push(item);
}

template<typename T, template <typename> class A>template<typename U, template<typename> class A2> 
inline void ang::collections::list_object<T, A>::extend(ang::scope_array<U, A2>const& arr)
{
	for (U const& item : arr)
		push(item);
}

template<typename T, template <typename> class A> template<typename U, wsize SIZE>
inline void ang::collections::list_object<T, A>::extend(ang::stack_array<U, SIZE> const& arr)
{
	for (U const& item : arr)
		push(item);
}


template<typename T, template <typename> class A>
inline wsize ang::collections::list_object<T, A>::counter()const
{
	return m_count;
}

template<typename T, template <typename> class A>
inline T& ang::collections::list_object<T, A>::at(ang::collections::base_iterator<T> const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty() || it.parent() != static_cast<ienum_t*>(this))
		throw_exception(except_code::invalid_memory);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	return node->data;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::increase(base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	it.current(node->next);
	return it.current() != null;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::increase(base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	while (node && offset)
	{
		node = node->next;
		offset--;
	}
	it.current(node);
	return it.current() != null;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::decrease(base_iterator_t& it)const 
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	it.current(node->prev);
	return it.current() != null;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::decrease(base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	while (node && offset)
	{
		node = node->prev;
		offset--;
	}
	it.current(node);
	return it.current() != null;
}

template<typename T, template <typename> class A>
inline ang::collections::forward_iterator<T> ang::collections::list_object<T, A>::begin()
{
	return forward_iterator_t(const_cast<list_object*>(this), pointer(m_head));
}

template<typename T, template <typename> class A>
inline ang::collections::forward_iterator<T> ang::collections::list_object<T, A>::end()
{
	return forward_iterator_t(const_cast<list_object*>(this), null);
}

template<typename T, template <typename> class A>
inline ang::collections::const_forward_iterator<T> ang::collections::list_object<T, A>::begin()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), pointer(m_head));
}

template<typename T, template <typename> class A>
inline ang::collections::const_forward_iterator<T> ang::collections::list_object<T, A>::end()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), null);
}

template<typename T, template <typename> class A>
inline ang::collections::forward_iterator<T> ang::collections::list_object<T, A>::last()
{
	return forward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

template<typename T, template <typename> class A>
inline ang::collections::const_forward_iterator<T> ang::collections::list_object<T, A>::last()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

template<typename T, template <typename> class A>
inline ang::collections::backward_iterator<T> ang::collections::list_object<T, A>::rbegin()
{
	return backward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

template<typename T, template <typename> class A>
inline ang::collections::backward_iterator<T> ang::collections::list_object<T, A>::rend()
{
	return backward_iterator_t(const_cast<list_object*>(this), null);
}

template<typename T, template <typename> class A>
inline ang::collections::const_backward_iterator<T> ang::collections::list_object<T, A>::rbegin()const
{
	return const_backward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

template<typename T, template <typename> class A>
inline ang::collections::const_backward_iterator<T> ang::collections::list_object<T, A>::rend()const
{
	return const_backward_iterator_t(const_cast<list_object*>(this), null);
}


template<typename T, template <typename> class A>
inline ang::collections::iterator<T> ang::collections::list_object<T, A>::at(windex idx)
{
	if (idx >= m_count)
		return end();

	if (idx <= (m_count / 2)) {
		node_ptr_t node = m_head;
		for (windex i = 0; i < idx; i++)
			node = node->next;
		return iterator_t(const_cast<list_object*>(this), pointer(node));
	}
	else
	{
		node_ptr_t node = m_tail;
		idx = m_count - idx - 1;
		for (windex i = 0; i < idx; i++)
			node = node->prev;
		return iterator_t(const_cast<list_object*>(this), pointer(node));
	}
}

template<typename T, template <typename> class A>
inline ang::collections::const_iterator<T> ang::collections::list_object<T, A>::at(windex idx)const
{
	if (idx >= m_count)
		return end();

	if (idx <= (m_count / 2)) {
		node_ptr_t node = m_head;
		for (windex i = 0; i < idx; i++)
			node = node->next;
		return const_iterator_t(const_cast<list_object*>(this), pointer(node));
	}
	else
	{
		node_ptr_t node = m_tail;
		idx = m_count - idx - 1;
		for (windex i = 0; i < idx; i++)
			node = node->prev;
		return const_iterator_t(const_cast<list_object*>(this), pointer(node));
	}
}

template<typename T, template <typename> class A>
inline void ang::collections::list_object<T, A>::copy(const ang::collections::ienum<T>* items)
{
	if (items == static_cast<ienum_t*>(this))
		return;
	clear();
	if(items != null) for (T const& item : *items) {
		push(item);
	}
}

template<typename T, template <typename> class A>
inline ang::collections::iterator<T> ang::collections::list_object<T, A>::find(core::delegates::function<bool(T const&)> func, bool invert)const
{
	if (invert) for (node_ptr_t node = m_tail; node != null; node = node->prev) {
		if(func(node->data))
			return iterator_t(const_cast<list_object*>(this), pointer(node));
	}
	else for (node_ptr_t node = m_head; node != null; node = node->next) {
		if (func(node->data))
			return iterator_t(const_cast<list_object*>(this), pointer(node));
	}

	return end();
}

template<typename T, template <typename> class A>
inline ang::collections::iterator<T> ang::collections::list_object<T, A>::find(core::delegates::function<bool(T const&)> func, base_iterator_t next_to, bool invert)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty() || next_to.parent() != static_cast<ienum_t const*>(this))
		throw_exception(except_code::invalid_memory);
#endif
	if (invert) 
	{
		if (next_to.current() == null)
			next_to.current(m_tail);
		for (node_ptr_t node = reinterpret_cast<node_ptr_t>(next_to.current()); node != null; node = node->prev) {
			if (func(node->data))
				return iterator_t(const_cast<list_object*>(this), pointer(node));
		}
	}
	else 
	{
		if (next_to.current() == null)
			next_to.current(m_head);
		for (node_ptr_t node = reinterpret_cast<node_ptr_t>(next_to.current()); node != null; node = node->next) {
			if (func(node->data))
				return iterator_t(const_cast<list_object*>(this), pointer(node));
		}
	}
	return end();
}

template<typename T, template <typename> class A>
inline ang::collections::ienum_ptr<T> ang::collections::list_object<T, A>::find_all(core::delegates::function<bool(T const&)> func)const
{
	list<T,A> list;
	for (node_ptr_t node = m_head; node != null; node = node->next) {
		if (func(node->data))
			list->push(node->data);
	}
	return list.get();
}

template<typename T, template <typename> class A>
inline void ang::collections::list_object<T, A>::extend(const ienum<T>* items)
{
	if (items == static_cast<ienum_t*>(this))
		return;
	if (items != null) for (T const& item : *items) {
		push(item);
	}
}

template<typename T, template <typename> class A>
inline void ang::collections::list_object<T, A>::push(T const& value, bool last) 
{
	node_ptr_t new_node = m_alloc.allocate(1);
	m_alloc.template construct<node_t, T const&>(new_node, value);

	if (last)
	{
		node_ptr_t old_node = m_tail;
		m_tail = new_node;
		if (old_node) {
			m_tail->prev = old_node;
			old_node->next = m_tail;
		}
		else
			m_head = new_node;
	}
	else
	{
		node_ptr_t old_node = m_head;
		m_head = new_node;
		if (old_node) {
			m_head->next = old_node;
			old_node->prev = m_head;
		}
		else
			m_tail = new_node;
	}
	m_count++;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::insert(windex idx, T const& value) 
{
	if (idx == 0U)
	{
		push(value, false);
		return true;
	}
	else if (idx >= m_count)
	{
		push(value, true);
		return true;
	}
	else if (idx <= (m_count / 2))
	{
		node_ptr_t new_node = m_alloc.allocate(1);
		m_alloc.template construct<node_t, T const&>(new_node, value);
		idx--;
		node_ptr_t node = m_head;
		for (index i = 0; i < idx; ++i)
			node = node->next;
		new_node->next = node->next;
		new_node->prev = node;
		node->next->prev = new_node;
		node->next = new_node;
	}
	else
	{
		idx = m_count - idx;

		node_ptr_t new_node = m_alloc.allocate(1);
		m_alloc.template construct<node_t, T const&>(new_node, value);

		node_ptr_t node = m_tail;
		for (index i = 0; i < idx; ++i)
			node = node->prev;
		new_node->next = node->next;
		new_node->prev = node;
		node->next->prev = new_node;
		node->next = new_node;
	}
	m_count++;
	return true;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::insert(base_iterator_t it, T const& value)
{
	if (it.parent() != this || it.current() == null)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	node_ptr_t new_node = m_alloc.allocate(1);
	m_alloc.template construct<node_t, T const&>(new_node, value);

	new_node->next = node;
	new_node->prev = node->prev;
	if (node->prev == null)
		m_head = new_node;
	else
		node->prev->next = new_node;
	node->prev = new_node;
	m_count++;
	return true;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::pop(bool last)
{
	if (m_count == 0)
		return false;
	if (last) {
		node_ptr_t node = m_tail;
		m_tail = node->prev;
		if (m_tail) m_tail->next = null;
		else m_head = null;

		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	else
	{
		node_ptr_t node = m_head;
		m_head = node->next;
		if (m_head) m_head->prev = null;
		else m_tail = null;
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	m_count--;
	return true;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::pop(T& value, bool last)
{
	if (m_count == 0)
		return false;
	if (last) {
		node_ptr_t node = m_tail;
		m_tail = node->prev;
		if (m_tail) m_tail->next = null;
		else m_head = null;
		value = ang::move(node->data);
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	else
	{
		node_ptr_t node = m_head;
		m_head = node->next;
		if (m_head) m_head->prev = null;
		else m_tail = null;
		value = ang::move(node->data);
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	m_count--;
	return true;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::pop_at(windex idx)
{
	if (idx >= m_count)
		return false;
	else if (idx <= (m_count / 2))
	{
		node_ptr_t node = m_head;
		for (windex i = 0; i < idx; ++i)
			node = node->next;
		if (node->prev) node->prev->next = node->next;
		else m_head = node->next;
		if (node->next) node->next->prev = node->prev;
		else m_tail = node->prev;
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	else
	{
		idx = m_count - idx - 1;
		node_ptr_t node = m_tail;
		for (windex i = 0; i < idx; ++i)
			node = node->prev;
		if (node->prev) node->prev->next = node->next;
		else m_head = node->next;
		if (node->next) node->next->prev = node->prev;
		else m_tail = node->prev;
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	m_count--;
	return true;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::pop_at(base_iterator_t it)
{
	if (it.parent() != this || it.current() == null)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	if (node->prev) node->prev->next = node->next;
	else m_head = node->next;
	if (node->next) node->next->prev = node->prev;
	else m_tail = node->prev;
	m_alloc.template destroy<node_t>(node);
	m_alloc.deallocate(node);
	m_count--;
	return true;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::pop_at(windex idx, T& value)
{
	if (idx >= m_count)
		return false;
	else if (idx <= (m_count / 2))
	{
		node_ptr_t node = m_head;
		for (windex i = 0; i < idx; ++i)
			node = node->next;
		if (node->prev) node->prev->next = node->next;
		else m_head = node->next;
		if (node->next) node->next->prev = node->prev;
		else m_tail = node->prev;
		value = ang::move(node->data);
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	else
	{
		idx = m_count - idx - 1;
		node_ptr_t node = m_tail;
		for (windex i = 0; i <= idx; ++i)
			node = node->prev;
		if (node->prev) node->prev->next = node->next;
		else m_head = node->next;
		if (node->next) node->next->prev = node->prev;
		else m_tail = node->prev;
		value = ang::move(node->data);
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	m_count--;
	return true;
}

template<typename T, template <typename> class A>
inline bool ang::collections::list_object<T, A>::pop_at(base_iterator_t it, T& value)
{
	if (it.parent() != this || it.current() == null)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	if (node->prev) node->prev->next = node->next;
	else m_head = node->next;
	if (node->next) node->next->prev = node->prev;
	else m_tail = node->prev;
	value = ang::move(node->data);
	m_alloc.template destroy<node_t>(node);
	m_alloc.deallocate(node);
	m_count--;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::list_object<T, A>());
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::object_wrapper(ang::collections::list_object<T, A>* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new ang::collections::list_object<T, A>());
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T, template <typename> class A> template<typename U>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::object_wrapper(std::initializer_list<U> list)
	: m_ptr(null)
{
	set(new collections::list_object<T, A>(ang::move(list)));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: m_ptr(null)
{
	set(new collections::list_object<T, A>(store));
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::object_wrapper(ang::object_wrapper<ang::collections::list_object<T, A>> && ptr)
	: object_wrapper<collections::list_object<T, A>>()
{
	if (ptr.is_empty())
	{
		set(new ang::collections::list_object<T, A>());
	}
	else
	{
		auto temp = ptr.m_ptr;
		ptr.m_ptr = null;
		m_ptr = temp;
	}
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::object_wrapper(ang::object_wrapper<ang::collections::list_object<T, A>> const& ptr)
	: object_wrapper<collections::list_object<T, A>>()
{
	set(!ptr.is_empty() ? ptr.get() : new ang::collections::list_object<T, A>());
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::~object_wrapper()
{
	reset();
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::list_object<T, A>>::reset()
{
	if (m_ptr)
		m_ptr->release();
	m_ptr = null;
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::list_object<T, A>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T, template <typename> class A>
inline bool ang::object_wrapper<ang::collections::list_object<T, A>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T, template <typename> class A>
inline ang::collections::list_object<T, A>* ang::object_wrapper<ang::collections::list_object<T, A>>::get(void)const
{
	return m_ptr;
}

template<typename T, template <typename> class A>
inline void ang::object_wrapper<ang::collections::list_object<T, A>>::set(ang::collections::list_object<T, A>* ptr)
{
	ang::collections::list_object<T, A> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class A>
inline ang::collections::list_object<T, A>** ang::object_wrapper<ang::collections::list_object<T, A>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T, template <typename> class A>
inline ang::collections::list_object<T, A>** ang::object_wrapper<ang::collections::list_object<T, A>>::addres_for_init(void)
{
	reset();
	return &m_ptr;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>& ang::object_wrapper<ang::collections::list_object<T, A>>::operator = (ang::collections::list_object<T, A>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>& ang::object_wrapper<ang::collections::list_object<T, A>>::operator = (const std::nullptr_t&)
{
	reset();
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>& ang::object_wrapper<ang::collections::list_object<T, A>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (m_ptr == null)
		set(new collections::list_object<T, A>(items));
	else
		m_ptr->copy(items);
	return *this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>& ang::object_wrapper<ang::collections::list_object<T, A>>::operator = (ang::object_wrapper<ang::collections::list_object<T, A>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>& ang::object_wrapper<ang::collections::list_object<T, A>>::operator = (ang::object_wrapper<ang::collections::list_object<T, A>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>& ang::object_wrapper<ang::collections::list_object<T, A>>::operator += (T item)
{
	if (is_empty())
		set(new collections::list_object<T, A>());
	m_ptr->push(item);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>& ang::object_wrapper<ang::collections::list_object<T, A>>::operator += (collections::ienum<data_type> const* items)
{
	if (is_empty())
		set(new collections::list_object<T, A>(items));
	else m_ptr->extend(items);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>& ang::object_wrapper<ang::collections::list_object<T, A>>::operator += (object_wrapper const& items)
{
	if (is_empty())
		set(new collections::list_object<T, A>(items.get()));
	else m_ptr->extend((array_view<T>)items);
	return*this;
}

template<typename T, template <typename> class A>
inline ang::object_wrapper_ptr<ang::collections::list_object<T, A>> ang::object_wrapper<ang::collections::list_object<T, A>>::operator & (void)
{
	return this;
}

template<typename T, template <typename> class A>
inline ang::collections::list_object<T, A> * ang::object_wrapper<ang::collections::list_object<T, A>>::operator -> (void)
{
	return get();
}

template<typename T, template <typename> class A>
inline ang::collections::list_object<T, A> const* ang::object_wrapper<ang::collections::list_object<T, A>>::operator -> (void)const
{
	return get();
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::operator ang::collections::list_object<T, A> * (void)
{
	return get();
}

template<typename T, template <typename> class A>
inline ang::object_wrapper<ang::collections::list_object<T, A>>::operator ang::collections::list_object<T, A> const* (void)const
{
	return get();
}

template<typename T, template <typename> class A> template<typename I>
inline T const& ang::object_wrapper<ang::collections::list_object<T, A>>::operator[](I const& idx)const
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename T, template <typename> class A> template<typename I>
inline T & ang::object_wrapper<ang::collections::list_object<T, A>>::operator[](I const& idx)
{
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}


template<typename I>
inline char& ang::collections::list<char, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline char const& ang::collections::list<char, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline byte& ang::collections::list<byte, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline byte const& ang::collections::list<byte, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline short& ang::collections::list<short, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline short const& ang::collections::list<short, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ushort& ang::collections::list<ushort, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ushort const& ang::collections::list<ushort, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline int& ang::collections::list<int, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline int const& ang::collections::list<int, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline uint& ang::collections::list<uint, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline uint const& ang::collections::list<uint, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline long& ang::collections::list<long, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline long const& ang::collections::list<long, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ulong& ang::collections::list<ulong, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ulong const& ang::collections::list<ulong, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}


template<typename I>
inline long64& ang::collections::list<long64, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline long64 const& ang::collections::list<long64, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ulong64& ang::collections::list<ulong64, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ulong64 const& ang::collections::list<ulong64, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline float& ang::collections::list<float, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline float const& ang::collections::list<float, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline double& ang::collections::list<double, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline double const& ang::collections::list<double, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ang::intfptr& ang::collections::list<ang::intfptr, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ang::intfptr const& ang::collections::list<ang::intfptr, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ang::objptr& ang::collections::list<ang::objptr, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ang::objptr const& ang::collections::list<ang::objptr, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ang::string& ang::collections::list<ang::string, ang::memory::default_allocator>::operator[](I const& idx) {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}

template<typename I>
inline ang::string const& ang::collections::list<ang::string, ang::memory::default_allocator>::operator[](I const& idx)const {
	static_assert(is_integer_value<I>::value, "no integer value is no accepted");
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if ((idx >= (I)m_ptr->counter()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return *m_ptr->at(idx);
}


#endif//__ANG_COLLECTION_LIST_INL__