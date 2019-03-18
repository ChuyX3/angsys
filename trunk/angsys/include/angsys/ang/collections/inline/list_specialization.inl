#ifndef __ANG_COLLECTION_LIST_H__
#error Can't include binary_tree.h, please include binary_tree.h inside
#else

//ang::collections::list_object<MY_TYPE, MY_ALLOC>::allocator_t ang::collections::list_object<MY_TYPE, MY_ALLOC>::m_alloc;

ang::collections::list_object<MY_TYPE, MY_ALLOC>::list_object()
	: m_count(0)
	, m_head(null)
	, m_tail(null)
{

}

ang::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(const ang::nullptr_t&)
	: list_object()
{

}

ang::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(ang::collections::list_object<MY_TYPE, MY_ALLOC>&& other)
	: list_object()
{
	m_count = other.m_count;
	m_head = other.m_head;
	m_tail = other.m_tail;

	other.m_count = 0;
	other.m_head = null;
	other.m_tail = null;
}

ang::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(ang::collections::list_object<MY_TYPE, MY_ALLOC>const& other)
	: list_object()
{
	copy((ienum_t const*)&other);
}

ang::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(ang::collections::list_object<MY_TYPE, MY_ALLOC>const* other)
	: list_object()
{
	if(other != null)
		copy((ienum_t const*)other);
}

ang::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(ang::collections::ienum<MY_TYPE>const* other)
	: list_object()
{
	if (other != null)
		copy(other);
}


ang::collections::list_object<MY_TYPE, MY_ALLOC>::~list_object()
{
}

ang::rtti_t const& ang::collections::list_object<MY_TYPE, MY_ALLOC>::class_info()
{
	static const char name[] = ANG_UTILS_TO_STRING(ANG_EXPAND_ARGS(ang::collections::list<MY_TYPE,MY_ALLOC>));
	static rtti_t const* parents[] = { &runtime::type_of<ilist<MY_TYPE>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<ang::collections::list_object<MY_TYPE, MY_ALLOC>>(), align_of<ang::collections::list_object<MY_TYPE, MY_ALLOC>>(), parents, &default_query_interface);
	return info;
}

ang::rtti_t const& ang::collections::list_object<MY_TYPE, MY_ALLOC>::runtime_info()const
{
	return class_info();
}

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<list_object<MY_TYPE, MY_ALLOC>*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == type_of<ilist<MY_TYPE>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ilist<MY_TYPE>*>(this);
		return true;
	}
	else if (id.type_id() == type_of<iarray<MY_TYPE>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<iarray<MY_TYPE>*>(this);
		return true;
	}
	else if (id.type_id() == type_of<ienum<MY_TYPE>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum<MY_TYPE>*>(this);
		return true;
	}
	return false;
}

ang::comparision_result_t ang::collections::list_object<MY_TYPE, MY_ALLOC>::compare(const ang::object* obj)const
{
	windex i = 0;
	intf_wrapper<ienum<MY_TYPE>> items = interface_cast<ienum<MY_TYPE>>(const_cast<object*>(obj));

	if (!items.is_empty())
	{
		if (counter() > items->counter())
			return comparision_result::mayor;
		else if (counter() < items->counter())
			return comparision_result::minor;
		else for (const_forward_iterator_t it1 = begin(), it2 = items->begin(); it1 != end() && it2 != items->end(); ++it1, ++it2)
			if (logic_operation<MY_TYPE, MY_TYPE, logic_operation_type::same>::operate(*it1, *it2))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}

void ang::collections::list_object<MY_TYPE, MY_ALLOC>::clear()
{
	while (pop());
	m_head = null;
	m_tail = null;
	m_count = 0;
}

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::is_empty()const
{
	return m_count == 0;
}

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::move(list_object<MY_TYPE, MY_ALLOC>& other)
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

wsize ang::collections::list_object<MY_TYPE, MY_ALLOC>::counter()const
{
	return m_count;
}


MY_TYPE& ang::collections::list_object<MY_TYPE, MY_ALLOC>::at(ang::collections::base_iterator<MY_TYPE> const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty() || it.parent() != static_cast<ienum_t*>(this))
		throw_exception(except_code::invalid_memory);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	return node->data;
}

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::increase(base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	it.current(node->next);
	return it.current() != null;
}

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::increase(base_iterator_t& it, int offset)const
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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::decrease(base_iterator_t& it)const 
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(except_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	it.current(node->prev);
	return it.current() != null;
}

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::decrease(base_iterator_t& it, int offset)const
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

ang::collections::forward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::begin()
{
	return forward_iterator_t(const_cast<list_object*>(this), pointer(m_head));
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::end()
{
	return forward_iterator_t(const_cast<list_object*>(this), null);
}

ang::collections::const_forward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::begin()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), pointer(m_head));
}

ang::collections::const_forward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::end()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), null);
}

ang::collections::forward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::last()
{
	return forward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

ang::collections::const_forward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::last()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

ang::collections::backward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::rbegin()
{
	return backward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

ang::collections::backward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::rend()
{
	return backward_iterator_t(const_cast<list_object*>(this), null);
}

ang::collections::const_backward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::rbegin()const
{
	return const_backward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

ang::collections::const_backward_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::rend()const
{
	return const_backward_iterator_t(const_cast<list_object*>(this), null);
}

ang::collections::iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::at(windex idx)
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

ang::collections::const_iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::at(windex idx)const
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

void ang::collections::list_object<MY_TYPE, MY_ALLOC>::copy(const ang::collections::ienum<MY_TYPE>* items)
{
	if (items == static_cast<ienum_t*>(this))
		return;
	clear();
	if(items != null) for (MY_TYPE const& item : *items) {
		push(item);
	}
}

ang::collections::iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::find(core::delegates::function<bool(MY_TYPE const&)> func, bool invert)const
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

ang::collections::iterator<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::find(core::delegates::function<bool(MY_TYPE const&)> func, base_iterator_t next_to, bool invert)const
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

ang::collections::ienum_ptr<MY_TYPE> ang::collections::list_object<MY_TYPE, MY_ALLOC>::find_all(core::delegates::function<bool(MY_TYPE const&)> func)const
{
	list<MY_TYPE,MY_ALLOC> list;
	for (node_ptr_t node = m_head; node != null; node = node->next) {
		if (func(node->data))
			list->push(node->data);
	}
	return list.get();
}

void ang::collections::list_object<MY_TYPE, MY_ALLOC>::extend(const ienum<MY_TYPE>* items)
{
	if (items == static_cast<ienum_t*>(this))
		return;
	if (items != null) for (MY_TYPE const& item : *items) {
		push(item);
	}
}

void ang::collections::list_object<MY_TYPE, MY_ALLOC>::push(MY_TYPE const& value, bool last) 
{
	static allocator_t m_alloc;
	node_ptr_t new_node = m_alloc.allocate(1);
	m_alloc.template construct<node_t, MY_TYPE const&>(new_node, value);

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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::insert(windex idx, MY_TYPE const& value) 
{
	static allocator_t m_alloc;

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
		m_alloc.template construct<node_t, MY_TYPE const&>(new_node, value);
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
		m_alloc.template construct<node_t, MY_TYPE const&>(new_node, value);

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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::insert(base_iterator_t it, MY_TYPE const& value)
{
	static allocator_t m_alloc;

	if (it.parent() != this || it.current() == null)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	node_ptr_t new_node = m_alloc.allocate(1);
	m_alloc.template construct<node_t, MY_TYPE const&>(new_node, value);

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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::pop(bool last)
{
	static allocator_t m_alloc;

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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::pop(MY_TYPE& value, bool last)
{
	static allocator_t m_alloc;

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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::pop_at(windex idx)
{
	static allocator_t m_alloc;

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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::pop_at(base_iterator_t it)
{
	static allocator_t m_alloc;

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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::pop_at(windex idx, MY_TYPE& value)
{
	static allocator_t m_alloc;

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

bool ang::collections::list_object<MY_TYPE, MY_ALLOC>::pop_at(base_iterator_t it, MY_TYPE& value)
{
	static allocator_t m_alloc;

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


ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper()
	: m_ptr(null)
{
	set(new ang::collections::list_object<MY_TYPE, MY_ALLOC>());
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(ang::collections::list_object<MY_TYPE, MY_ALLOC>* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new ang::collections::list_object<MY_TYPE, MY_ALLOC>());
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(const ang::collections::ienum<data_type>* store)
	: m_ptr(null)
{
	set(new collections::list_object<MY_TYPE, MY_ALLOC>(store));
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>> && ptr)
	: object_wrapper<collections::list_object<MY_TYPE, MY_ALLOC>>()
{
	if (ptr.is_empty())
	{
		set(new ang::collections::list_object<MY_TYPE, MY_ALLOC>());
	}
	else
	{
		auto temp = ptr.m_ptr;
		ptr.m_ptr = null;
		m_ptr = temp;
	}
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>> const& ptr)
	: object_wrapper<collections::list_object<MY_TYPE, MY_ALLOC>>()
{
	set(!ptr.is_empty() ? ptr.get() : new ang::collections::list_object<MY_TYPE, MY_ALLOC>());
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::~object_wrapper()
{
	reset();
}

void ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::reset()
{
	if (m_ptr)
		m_ptr->release();
	m_ptr = null;
}

void ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::reset_unsafe()
{
	m_ptr = null;
}

bool ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::is_empty()const
{
	return m_ptr == null;
}

ang::collections::list_object<MY_TYPE, MY_ALLOC>* ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::get(void)const
{
	return m_ptr;
}

void ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::set(ang::collections::list_object<MY_TYPE, MY_ALLOC>* ptr)
{
	ang::collections::list_object<MY_TYPE, MY_ALLOC> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

ang::collections::list_object<MY_TYPE, MY_ALLOC>** ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::addres_of(void)
{
	return &m_ptr;
}

ang::collections::list_object<MY_TYPE, MY_ALLOC>** ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::addres_for_init(void)
{
	reset();
	return &m_ptr;
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (ang::collections::list_object<MY_TYPE, MY_ALLOC>* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (const std::nullptr_t&)
{
	reset();
	return*this;
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (ang::collections::ienum<data_type> const* items)
{
	if (m_ptr == null)
		set(new collections::list_object<MY_TYPE, MY_ALLOC>(items));
	else
		m_ptr->copy(items);
	return *this;
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>> const& other)
{
	set(other.m_ptr);
	return*this;
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator += (MY_TYPE item)
{
	if (is_empty())
		set(new collections::list_object<MY_TYPE, MY_ALLOC>());
	m_ptr->push(item);
	return*this;
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator += (collections::ienum<data_type> const* items)
{
	if (is_empty())
		set(new collections::list_object<MY_TYPE, MY_ALLOC>(items));
	else m_ptr->extend(items);
	return*this;
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>& ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator += (object_wrapper const& items)
{
	if (is_empty())
		set(new collections::list_object<MY_TYPE, MY_ALLOC>(items.get()));
	else m_ptr->extend((collections::ienum<MY_TYPE>*)items.get());
	return*this;
}

ang::object_wrapper_ptr<ang::collections::list_object<MY_TYPE, MY_ALLOC>> ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator & (void)
{
	return this;
}

ang::collections::list_object<MY_TYPE, MY_ALLOC> * ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator -> (void)
{
	return get();
}

ang::collections::list_object<MY_TYPE, MY_ALLOC> const* ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator ang::collections::list_object<MY_TYPE, MY_ALLOC> * (void)
{
	return get();
}

ang::object_wrapper<ang::collections::list_object<MY_TYPE, MY_ALLOC>>::operator ang::collections::list_object<MY_TYPE, MY_ALLOC> const* (void)const
{
	return get();
}


#endif//__ANG_COLLECTION_LIST_H__