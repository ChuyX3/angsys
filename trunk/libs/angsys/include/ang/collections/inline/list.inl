#ifndef __COFFE_COLLECTIONS_LIST_H__
#error Can't include binary_tree.h, please include binary_tree.h inside
#elif !defined __COFFE_COLLECTIONS_LIST_INL__
#define __COFFE_COLLECTIONS_LIST_INL__


namespace coffe
{
	template<typename T, template<typename>class A>
	struct intf_class_info<coffe::collections::list_object<T, A>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::list_object<T, A>>::types();
			static coffe::rtti_t const& info = rtti::regist(
				[]()->fast_astring_t {
				fast_astring_t out;
				out << "coffe::collections::list<"_s << coffe::type_of<T>().type_name() << ">"_s;
				return coffe::move(out); }()
					, genre::class_type
					, size_of<coffe::collections::list_object<T, A>>()
					, align_of<coffe::collections::list_object<T, A>>()
					, parents
					, &iintf::default_query_interface);
			return info;
		}
	};
}


template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::list_object()
	: fast_list_t()
{
}

template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::list_object(const coffe::nullptr_t&)
	: fast_list_t()
{
}

template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::list_object(wsize sz)
	: fast_list_t(sz)
{
}

template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::list_object(coffe::list<typename coffe::collections::auto_type<T>::type, A> && ar)
	: fast_list_t(coffe::forward<list<element_type, A>>(ar))
{
}

template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::list_object(coffe::list<typename coffe::collections::auto_type<T>::type, A> const& ar)
	: fast_list_t(ar)
{
}

template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::list_object(coffe::collections::list_object<T, A>&& ar)
	: fast_list_t(coffe::forward<fast_list_t>(ar))
{
}

template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::list_object(const coffe::collections::list_object<T, A>& ar)
	: fast_list_t(ar)
{
}

template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::list_object(const coffe::collections::ienum_ptr<T>& store)
	: fast_list_t()
{
	copy(store);
}

template<typename T, template<typename>class A> template<typename IT>
inline coffe::collections::list_object<T, A>::list_object(IT first, IT last)
	: fast_list_t()
{
	copy(first, last);
}

template<typename T, template<typename>class A> template<typename T2>
inline coffe::collections::list_object<T, A>::list_object(coffe::initializer_list<T2> list)
	: fast_list_t(coffe::forward<coffe::initializer_list<T2>>(list))
{
}

template<typename T, template<typename>class A> template<typename T2>
inline coffe::collections::list_object<T, A>::list_object(coffe::array_view<T2> const& ar)
	: fast_list_t(ar)
{
}

template<typename T, template<typename>class A>
inline coffe::collections::list_object<T, A>::~list_object()
{
}

template<typename T, template<typename>class A> template<typename T2>
inline void coffe::collections::list_object<T, A>::copy(coffe::collections::ienum_ptr<T2> const& items)
{
	fast_list_t::clear();
	fast_list_t::capacity(items->counter(), false);
	wsize i = 0;

	for (T2 const& item : items) {
		push_back(item);
	}
}

template<typename T, template<typename>class A>
inline void coffe::collections::list_object<T, A>::dispose()
{
	clear();
}

template<typename T, template<typename>class A>
inline coffe::string coffe::collections::list_object<T, A>::to_string()const
{
	mstring buffer = new text::mstring_buffer();
	if (m_first == null) {
		(*buffer) << "{ }"_s;
	}
	else {
		coffe::to_string((*buffer) << "{ "_s, m_first->data);
		for (node_type_ptr it = m_first->next; it != null; it = it->next) {
			coffe::to_string((*buffer) << ", "_s, it->data);
		}
		(*buffer) << " }"_s;
	}
	return buffer.get();
}

template<typename T, template<typename>class A>
inline coffe::string coffe::collections::list_object<T, A>::to_string(coffe::text::text_format_t format)const
{
	mstring buffer = new text::mstring_buffer();
	if (m_first == null) {
		(*buffer) << "{ }"_s;
	}
	else {
		coffe::to_string((*buffer) << "{ "_s, m_first->data, format);
		for (node_type_ptr it = m_first->next; it != null; it = it->next) {
			coffe::to_string((*buffer) << ", "_s, it->data, format);
		}
		(*buffer) << " }"_s;
	}
	return buffer.get();
}

template<typename T, template<typename>class A>
inline wsize coffe::collections::list_object<T, A>::counter()const
{
	return size();
}

template<typename T, template<typename>class A>
inline typename coffe::collections::auto_type<T>::return_type coffe::collections::list_object<T, A>::at(typename coffe::collections::list_object<T, A>::base_iterator_t const& it)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty())
		throw_exception(error_code::invalid_memory);
	if (it.current() == null)
		throw_exception(error_code::invalid_param);
#endif
	node_type_ptr node = reinterpret_cast<node_type_ptr>(it.current());
	if constexpr (is_pointer<typename coffe::collections::auto_type<T>::return_type>::value)
		return &node->data;
	else
		return node->data;
}

template<typename T, template<typename>class A>
inline void coffe::collections::list_object<T, A>::copy(const coffe::collections::ienum_ptr<T>& items)
{
	fast_list_t::clear();
	for (typename coffe::collections::auto_type<T>::const_ref_type item : items) {
		push_back(item);
	}
}

template<typename T, template<typename>class A>
void coffe::collections::list_object<T, A>::extend(const coffe::collections::ienum_ptr<T>& items)
{
	for (typename coffe::collections::auto_type<T>::const_ref_type item : items) {
		push_back(item);
	}
}

template<typename T, template<typename>class A>
void coffe::collections::list_object<T, A>::push_front(typename coffe::collections::auto_type<T>::arg_type && value)
{
	fast_list_t::push_front(coffe::forward<element_type>(value));
}

template<typename T, template<typename>class A>
void coffe::collections::list_object<T, A>::push_front(typename coffe::collections::auto_type<T>::arg_type const& value)
{
	fast_list_t::push_front(value);
}

template<typename T, template<typename>class A>
void coffe::collections::list_object<T, A>::push_back(typename coffe::collections::auto_type<T>::arg_type && value)
{
	fast_list_t::push_back(coffe::forward<element_type>(value));
}

template<typename T, template<typename>class A>
void coffe::collections::list_object<T, A>::push_back(typename coffe::collections::auto_type<T>::arg_type const& value)
{
	fast_list_t::push_back(value);
}

template<typename T, template<typename>class A>
typename coffe::collections::list_object<T, A>::iterator_t coffe::collections::list_object<T, A>::insert(coffe::collections::base_iterator<T> it, typename coffe::collections::auto_type<T>::arg_type && value)
{
	if (it.parent() != this)
		return end();
	typename fast_list_t::base_iterator_t it_ = reinterpret_cast<node_type_ptr>(it.current());
	fast_list_t::insert(it_, coffe::forward<element_type>(value));
	return iterator_t(this, fast_list_t::get_node(it_), 0);
}

template<typename T, template<typename>class A>
typename coffe::collections::list_object<T, A>::iterator_t coffe::collections::list_object<T, A>::insert(coffe::collections::base_iterator<T> it, typename coffe::collections::auto_type<T>::arg_type const& value)
{
	if (it.parent() != this)
		return end();
	typename fast_list_t::base_iterator_t it_ = reinterpret_cast<node_type_ptr>(it.current());
	fast_list_t::insert(it_, value);
	return iterator_t(this, fast_list_t::get_node(it_), 0);
}

template<typename T, template<typename>class A>
bool coffe::collections::list_object<T, A>::pop_front(typename coffe::collections::auto_type<T>::arg_ptr_type value)
{
	return value != null ? fast_list_t::pop_front(*value) : fast_list_t::pop_front();
}

template<typename T, template<typename>class A>
bool coffe::collections::list_object<T, A>::pop_back(typename coffe::collections::auto_type<T>::arg_ptr_type value)
{
	return value != null ? fast_list_t::pop_back(*value) : fast_list_t::pop_back();
}

template<typename T, template<typename>class A>
typename coffe::collections::list_object<T, A>::iterator_t coffe::collections::list_object<T, A>::remove(coffe::collections::base_iterator<T> it, typename coffe::collections::auto_type<T>::arg_ptr_type value)
{
	if (it.parent() != this || it.current() == null)
		return end();
	typename fast_list_t::base_iterator_t it_ = reinterpret_cast<node_type_ptr>(it.current());
	if (!(value != null ? fast_list_t::remove(it_, *value) : fast_list_t::remove(it_)))
		return end();
	return iterator_t(this, fast_list_t::get_node(it_), 0);
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::iterator_t coffe::collections::list_object<T, A>::find(function<bool(typename coffe::collections::auto_type<T>::arg_type const&)> cond, bool invert)const
{
	if (!is_empty())
	{
		if (invert) for (auto it = fast_list_t::rbegin(); it != fast_list_t::rend(); it++)
		{
			if (cond(*it))
				return iterator_t(const_cast<list_object<T, A>*>(this), fast_list_t::get_node(it));
		}
		else for (auto it = fast_list_t::begin(); it != fast_list_t::end(); it++)
		{
			if (cond(*it))
				return iterator_t(const_cast<list_object<T, A>*>(this), fast_list_t::get_node(it));
		}
	}
	return end();
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::iterator_t coffe::collections::list_object<T, A>::find(function<bool(typename coffe::collections::auto_type<T>::arg_type const&)> cond, coffe::collections::list_object<T, A>::base_iterator_t next_to, bool invert)const
{
	if (!is_empty() && next_to.parent() == this)
	{
		if (invert) for (auto it = next_to.current() == null ? fast_list_t::rbegin() : typename fast_list_t::reverse_const_iterator_t(reinterpret_cast<node_type_ptr>(next_to.current())); it != fast_list_t::rend(); it++)
		{
			if (cond(*it))
				return iterator_t(const_cast<list_object<T, A>*>(this), fast_list_t::get_node(it));
		}
		else for(auto it = next_to.current() == null ? fast_list_t::begin() : typename fast_list_t::const_iterator_t(reinterpret_cast<node_type_ptr>(next_to.current())); it != fast_list_t::end(); it++)
		{
			if (cond(*it))
				return iterator_t(const_cast<list_object<T, A>*>(this), fast_list_t::get_node(it));
		}
	}
	return end();
}

template<typename T, template<typename>class A>
inline coffe::collections::ienum_ptr<T> coffe::collections::list_object<T, A>::find_all(function<bool(typename coffe::collections::auto_type<T>::arg_type const&)> cond)const
{
	if (is_empty())
		return null;
	
	list<T, A> out = null;
	for (auto it = fast_list_t::begin(); it != fast_list_t::end(); it++)
	{
		if (cond(*it))
			out += *it;
	}
	return out.get();
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::forward_iterator_t coffe::collections::list_object<T, A>::begin()
{
	return forward_iterator_t(const_cast<list_object*>(this), pointer(&m_first));
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::forward_iterator_t coffe::collections::list_object<T, A>::end()
{
	return forward_iterator_t(const_cast<list_object*>(this), null);
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::const_forward_iterator_t coffe::collections::list_object<T, A>::begin()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), pointer(&m_first));
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::const_forward_iterator_t coffe::collections::list_object<T, A>::end()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), null);
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::forward_iterator_t coffe::collections::list_object<T, A>::last()
{
	return forward_iterator_t(const_cast<list_object*>(this), pointer(&m_last));
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::const_forward_iterator_t coffe::collections::list_object<T, A>::last()const
{
	return const_forward_iterator_t(const_cast<list_object*>(this), pointer(&m_last));
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::backward_iterator_t coffe::collections::list_object<T, A>::rbegin()
{
	return backward_iterator_t(const_cast<list_object*>(this), pointer(&m_last));
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::backward_iterator_t coffe::collections::list_object<T, A>::rend()
{
	return backward_iterator_t(const_cast<list_object*>(this), null);
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::const_backward_iterator_t coffe::collections::list_object<T, A>::rbegin()const
{
	return const_backward_iterator_t(const_cast<list_object*>(this), pointer(&m_last));
}

template<typename T, template<typename>class A>
inline typename coffe::collections::list_object<T, A>::const_backward_iterator_t coffe::collections::list_object<T, A>::rend()const
{
	return const_backward_iterator_t(const_cast<list_object*>(this), null);
}


template<typename T, template<typename>class A>
inline bool coffe::collections::list_object<T, A>::increase(typename coffe::collections::list_object<T, A>::base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		return false;
#endif
	node_type_ptr node = reinterpret_cast<node_type_ptr>(it.current());
	it.current(node->next);
	return true;
}

template<typename T, template<typename>class A>
inline bool coffe::collections::list_object<T, A>::increase(typename coffe::collections::list_object<T, A>::base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		return false;
#endif
	node_type_ptr node = reinterpret_cast<node_type_ptr>(it.current());
	while (node != null && offset > 0) {
		node = node->next;
		offset--;
	}
	it.current(node);
	return true;
}

template<typename T, template<typename>class A>
inline bool coffe::collections::list_object<T, A>::decrease(typename coffe::collections::list_object<T, A>::base_iterator_t& it)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		return false;
#endif
	node_type_ptr node = reinterpret_cast<node_type_ptr>(it.current());
	it.current(node->prev);
	return true;
}

template<typename T, template<typename>class A>
inline bool coffe::collections::list_object<T, A>::decrease(typename coffe::collections::list_object<T, A>::base_iterator_t& it, int offset)const
{
#ifdef DEBUG_SAFE_CODE
	if (it.parent() != this || it.current() == null)
		return false;
#endif
	node_type_ptr node = reinterpret_cast<node_type_ptr>(it.current());
	while (node != null && offset > 0) {
		node = node->prev;
		offset--;
	}
	it.current(node);
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::object_wrapper()
	: m_ptr(null)
{
	set(new coffe::collections::list_object<T, A>());
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::object_wrapper(coffe::collections::list_object<T, A>* ptr)
	: m_ptr(null)
{
	set(ptr ? ptr : new coffe::collections::list_object<T, A>());
}


template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::object_wrapper(coffe::object_wrapper<coffe::collections::list_object<T, A>> && other)
	: m_ptr(null)
{
	collections::list_object<T, A> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::object_wrapper(coffe::object_wrapper<coffe::collections::list_object<T, A>> const& other)
	: m_ptr(null)
{
	set(other.get());
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::object_wrapper(std::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::object_wrapper(wsize reserve)
	: m_ptr(null)
{
	set(new coffe::collections::list_object<T, A>(reserve));
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::object_wrapper(const coffe::collections::ienum_ptr<T>& store)
	: m_ptr(null)
{
	set(new collections::list_object<T, A>(store));
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::~object_wrapper()
{
	reset();
}

template<typename T, template <typename> class A>
inline void coffe::object_wrapper<coffe::collections::list_object<T, A>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename T, template <typename> class A>
inline void coffe::object_wrapper<coffe::collections::list_object<T, A>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T, template <typename> class A>
inline bool coffe::object_wrapper<coffe::collections::list_object<T, A>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T, template <typename> class A>
inline coffe::collections::list_object<T, A>* coffe::object_wrapper<coffe::collections::list_object<T, A>>::get(void)const
{
	return m_ptr;
}

template<typename T, template <typename> class A>
inline void coffe::object_wrapper<coffe::collections::list_object<T, A>>::set(coffe::collections::list_object<T, A>* ptr)
{
	coffe::collections::list_object<T, A> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, template <typename> class A>
inline coffe::collections::list_object<T, A>** coffe::object_wrapper<coffe::collections::list_object<T, A>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>& coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator = (coffe::collections::list_object<T, A>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>& coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator = (const std::nullptr_t&)
{
	reset();
	return*this;
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>& coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator = (coffe::object_wrapper<coffe::collections::list_object<T, A>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>& coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator = (coffe::object_wrapper<coffe::collections::list_object<T, A>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper_ptr<coffe::collections::list_object<T, A>> coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator & (void)
{
	return this;
}

template<typename T, template <typename> class A>
inline coffe::collections::list_object<T, A> * coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator -> (void)
{
	return get();
}

template<typename T, template <typename> class A>
inline coffe::collections::list_object<T, A> const* coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator -> (void)const
{
	return get();
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator coffe::collections::list_object<T, A> * (void)
{
	return get();
}

template<typename T, template <typename> class A>
inline coffe::object_wrapper<coffe::collections::list_object<T, A>>::operator coffe::collections::list_object<T, A> const* (void)const
{
	return get();
}


#endif//__COFFE_COLLECTIONS_LIST_INL__
