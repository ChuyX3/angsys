#ifndef __COFFE_COLLECTION_LIST_H__
#error Can't include binary_tree.h, please include binary_tree.h inside
#else

DECLARE_ALLOC(MY_TYPE, MY_ALLOC);

coffe::collections::list_object<MY_TYPE, MY_ALLOC>::list_object()
	: m_count(0)
	, m_head(null)
	, m_tail(null)
{

}

coffe::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(const coffe::nullptr_t&)
	: list_object()
{

}

coffe::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(coffe::collections::list_object<MY_TYPE, MY_ALLOC>&& other)
	: list_object()
{
	m_count = other.m_count;
	m_head = other.m_head;
	m_tail = other.m_tail;

	other.m_count = 0;
	other.m_head = null;
	other.m_tail = null;
}

coffe::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(coffe::collections::list_object<MY_TYPE, MY_ALLOC>const& other)
	: list_object()
{
	copy((ienum_t const*)&other);
}

coffe::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(coffe::collections::list_object<MY_TYPE, MY_ALLOC>const* other)
	: list_object()
{
	if(other != null)
		copy((ienum_t const*)other);
}

coffe::collections::list_object<MY_TYPE, MY_ALLOC>::list_object(coffe::collections::ienum<MY_TYPE>const* other)
	: list_object()
{
	if (other != null)
		copy(other);
}


coffe::collections::list_object<MY_TYPE, MY_ALLOC>::~list_object()
{
}

coffe::rtti_t const& coffe::collections::list_object<MY_TYPE, MY_ALLOC>::class_info()
{
	static const char name[] = COFFE_UTILS_TO_STRING(COFFE_EXPAND_ARGS(coffe::collections::list<MY_TYPE,MY_ALLOC>));
	static rtti_t const* parents[] = {
		&runtime::type_of<object>(),
		&runtime::type_of<ilist<MY_TYPE>>(),
		&runtime::type_of<ienum<MY_TYPE>>()
	};
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>(), align_of<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>(), parents, &default_query_interface);
	return info;
}

coffe::rtti_t const& coffe::collections::list_object<MY_TYPE, MY_ALLOC>::runtime_info()const
{
	return class_info();
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::query_interface(coffe::rtti_t const& id, coffe::unknown_ptr_t out)
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
	else if (id.type_id() == type_of<ienumerable<MY_TYPE>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienumerable<MY_TYPE>*>(this);
		return true;
	}
	else if (id.type_id() == type_of<ienum<MY_TYPE>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ienum<MY_TYPE>*>(this);
		return true;
	}
	return false;
}

coffe::comparision_result_t coffe::collections::list_object<MY_TYPE, MY_ALLOC>::compare(const coffe::object* obj)const
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
			if (logic_operation<logic_operation_type::same, MY_TYPE, MY_TYPE>::operate(*it1, *it2))
				return comparision_result::diferent;
		return comparision_result::same;
	}
	return comparision_result::diferent;
}

void coffe::collections::list_object<MY_TYPE, MY_ALLOC>::dispose()
{
	clear();
}

void coffe::collections::list_object<MY_TYPE, MY_ALLOC>::clear()
{
	while (pop());
	m_head = null;
	m_tail = null;
	m_count = 0;
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::is_empty()const
{
	return m_count == 0;
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::move(list_object<MY_TYPE, MY_ALLOC>& other)
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

wsize coffe::collections::list_object<MY_TYPE, MY_ALLOC>::counter()const
{
	return m_count;
}


MY_TYPE& coffe::collections::list_object<MY_TYPE, MY_ALLOC>::at(coffe::collections::base_iterator<MY_TYPE> const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty() || it.parent() != static_cast<ienum_t*>(this))
		throw_exception(error_code::invalid_memory);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	return node->data;
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::increase(base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(error_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	it.current(node->next);
	return it.current() != null;
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::increase(base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(error_code::invalid_param);
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

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::decrease(base_iterator_t& it)const 
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(error_code::invalid_param);
#endif
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	it.current(node->prev);
	return it.current() != null;
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::decrease(base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		throw_exception(error_code::invalid_param);
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

coffe::collections::forward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::begin()
{
	return forward_iterator_t(const_cast<list_object*>(this), pointer(m_head));
}

coffe::collections::forward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::end()
{
	return forward_iterator_t(const_cast<list_object*>(this), null);
}

coffe::collections::const_forward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::begin()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), pointer(m_head));
}

coffe::collections::const_forward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::end()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), null);
}

coffe::collections::forward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::last()
{
	return forward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

coffe::collections::const_forward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::last()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

coffe::collections::backward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::rbegin()
{
	return backward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

coffe::collections::backward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::rend()
{
	return backward_iterator_t(const_cast<list_object*>(this), null);
}

coffe::collections::const_backward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::rbegin()const
{
	return const_backward_iterator_t(const_cast<list_object*>(this), pointer(m_tail));
}

coffe::collections::const_backward_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::rend()const
{
	return const_backward_iterator_t(const_cast<list_object*>(this), null);
}

coffe::collections::ienum_ptr<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::enumerate(iteration_method_t)const
{
	return const_cast<list_object<MY_TYPE, MY_ALLOC>*>(this);
}

coffe::collections::iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::at(windex idx)
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

coffe::collections::const_iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::at(windex idx)const
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

void coffe::collections::list_object<MY_TYPE, MY_ALLOC>::copy(const coffe::collections::ienum<MY_TYPE>* items)
{
	if (items == static_cast<ienum_t*>(this))
		return;
	clear();
	if(items != null) for (MY_TYPE const& item : *items) {
		push(item);
	}
}

coffe::collections::iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::find(core::delegates::function<bool(MY_TYPE const&)> func, bool invert)const
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

coffe::collections::iterator<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::find(core::delegates::function<bool(MY_TYPE const&)> func, base_iterator_t next_to, bool invert)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty() || next_to.parent() != static_cast<ienum_t const*>(this))
		throw_exception(error_code::invalid_memory);
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

coffe::collections::ienum_ptr<MY_TYPE> coffe::collections::list_object<MY_TYPE, MY_ALLOC>::find_all(core::delegates::function<bool(MY_TYPE const&)> func)const
{
	list<MY_TYPE,MY_ALLOC> list;
	for (node_ptr_t node = m_head; node != null; node = node->next) {
		if (func(node->data))
			list->push(node->data);
	}
	return list.get();
}

void coffe::collections::list_object<MY_TYPE, MY_ALLOC>::extend(const ienum<MY_TYPE>* items)
{
	if (items == static_cast<ienum_t*>(this))
		return;
	if (items != null) for (MY_TYPE const& item : *items) {
		push(item);
	}
}

void coffe::collections::list_object<MY_TYPE, MY_ALLOC>::push(MY_TYPE const& value, bool last) 
{
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

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::insert(windex idx, MY_TYPE const& value) 
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
		m_alloc.template construct<node_t, MY_TYPE const&>(new_node, value);
		idx--;
		node_ptr_t node = m_head;
		for (windex i = 0; i < idx; ++i)
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
		for (windex i = 0; i < idx; ++i)
			node = node->prev;
		new_node->next = node->next;
		new_node->prev = node;
		node->next->prev = new_node;
		node->next = new_node;
	}
	m_count++;
	return true;
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::insert(base_iterator_t it, MY_TYPE const& value)
{
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

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::pop(bool last)
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

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::pop(MY_TYPE& value, bool last)
{
	if (m_count == 0)
		return false;
	if (last) {
		node_ptr_t node = m_tail;
		m_tail = node->prev;
		if (m_tail) m_tail->next = null;
		else m_head = null;
		value = coffe::move(node->data);
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	else
	{
		node_ptr_t node = m_head;
		m_head = node->next;
		if (m_head) m_head->prev = null;
		else m_tail = null;
		value = coffe::move(node->data);
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	m_count--;
	return true;
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::pop_at(windex idx)
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

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::pop_at(base_iterator_t it)
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

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::pop_at(windex idx, MY_TYPE& value)
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
		value = coffe::move(node->data);
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
		value = coffe::move(node->data);
		m_alloc.template destroy<node_t>(node);
		m_alloc.deallocate(node);
	}
	m_count--;
	return true;
}

bool coffe::collections::list_object<MY_TYPE, MY_ALLOC>::pop_at(base_iterator_t it, MY_TYPE& value)
{
	if (it.parent() != this || it.current() == null)
		return false;
	node_ptr_t node = reinterpret_cast<node_ptr_t>(it.current());
	if (node->prev) node->prev->next = node->next;
	else m_head = node->next;
	if (node->next) node->next->prev = node->prev;
	else m_tail = node->prev;
	value = coffe::move(node->data);
	m_alloc.template destroy<node_t>(node);
	m_alloc.deallocate(node);
	m_count--;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper()
	: m_ptr(null)
{
	set(new coffe::collections::list_object<MY_TYPE, MY_ALLOC>());
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(coffe::collections::list_object<MY_TYPE, MY_ALLOC>* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new coffe::collections::list_object<MY_TYPE, MY_ALLOC>());
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(const coffe::collections::ienum<data_type>* store)
	: m_ptr(null)
{
	set(new collections::list_object<MY_TYPE, MY_ALLOC>(store));
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>> && ptr)
	: object_wrapper<collections::list_object<MY_TYPE, MY_ALLOC>>()
{
	if (ptr.is_empty())
	{
		set(new coffe::collections::list_object<MY_TYPE, MY_ALLOC>());
	}
	else
	{
		auto temp = ptr.m_ptr;
		ptr.m_ptr = null;
		m_ptr = temp;
	}
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::object_wrapper(coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>> const& ptr)
	: object_wrapper<collections::list_object<MY_TYPE, MY_ALLOC>>()
{
	set(!ptr.is_empty() ? ptr.get() : new coffe::collections::list_object<MY_TYPE, MY_ALLOC>());
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::~object_wrapper()
{
	reset();
}

void coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::reset()
{
	if (m_ptr)
		m_ptr->release();
	m_ptr = null;
}

void coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::reset_unsafe()
{
	m_ptr = null;
}

bool coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::is_empty()const
{
	return m_ptr == null;
}

coffe::collections::list_object<MY_TYPE, MY_ALLOC>* coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::get(void)const
{
	return m_ptr;
}

void coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::set(coffe::collections::list_object<MY_TYPE, MY_ALLOC>* ptr)
{
	coffe::collections::list_object<MY_TYPE, MY_ALLOC> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

coffe::collections::list_object<MY_TYPE, MY_ALLOC>** coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::addres_of(void)
{
	return &m_ptr;
}

coffe::collections::list_object<MY_TYPE, MY_ALLOC>** coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::addres_for_init(void)
{
	reset();
	return &m_ptr;
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>& coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (coffe::collections::list_object<MY_TYPE, MY_ALLOC>* ptr)
{
	set(ptr);
	return*this;
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>& coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (const std::nullptr_t&)
{
	reset();
	return*this;
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>& coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (coffe::collections::ienum<data_type> const* items)
{
	if (m_ptr == null)
		set(new collections::list_object<MY_TYPE, MY_ALLOC>(items));
	else
		m_ptr->copy(items);
	return *this;
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>& coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>& coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator = (coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>> const& other)
{
	set(other.m_ptr);
	return*this;
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>& coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator += (MY_TYPE item)
{
	if (is_empty())
		set(new collections::list_object<MY_TYPE, MY_ALLOC>());
	m_ptr->push(item);
	return*this;
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>& coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator += (collections::ienum<data_type> const* items)
{
	if (is_empty())
		set(new collections::list_object<MY_TYPE, MY_ALLOC>(items));
	else m_ptr->extend(items);
	return*this;
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>& coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator += (object_wrapper const& items)
{
	if (is_empty())
		set(new collections::list_object<MY_TYPE, MY_ALLOC>(items.get()));
	else m_ptr->extend((collections::ienum<MY_TYPE>*)items.get());
	return*this;
}

coffe::object_wrapper_ptr<coffe::collections::list_object<MY_TYPE, MY_ALLOC>> coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator & (void)
{
	return this;
}

coffe::collections::list_object<MY_TYPE, MY_ALLOC> * coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator -> (void)
{
	return get();
}

coffe::collections::list_object<MY_TYPE, MY_ALLOC> const* coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator -> (void)const
{
	return get();
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator coffe::collections::list_object<MY_TYPE, MY_ALLOC> * (void)
{
	return get();
}

coffe::object_wrapper<coffe::collections::list_object<MY_TYPE, MY_ALLOC>>::operator coffe::collections::list_object<MY_TYPE, MY_ALLOC> const* (void)const
{
	return get();
}


#endif//__COFFE_COLLECTION_LIST_H__
