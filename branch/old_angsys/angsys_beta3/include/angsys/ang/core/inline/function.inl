#ifndef __CORE_FUNCTION_H__
#error ...
#elif !defined __CORE_FUNCTION_INL__
#define __CORE_FUNCTION_INL__

	
template<typename T, typename... Ts>
inline ang::rtti_t const& ang::core::delegates::ifunction<T(Ts...)>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::core::delegates::ifunction<"_s) += ang::type_of<T(Ts...)>().type_name()) += ">"_s);
	static ang::rtti_t const* parents[] = { &ang::type_of<intf>() };
	static ang::rtti_t const& info = ang::rtti::regist(name, genre::class_type, sizeof(ang::core::delegates::ifunction<T(Ts...)>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<typename... Ts>
inline ang::rtti_t const& ang::core::delegates::ifunction<void(Ts...)>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::core::delegates::ifunction<"_s) += ang::type_of<void(Ts...)>().type_name()) += ">"_s);
	static ang::rtti_t const* parents[] = { &ang::type_of<intf>() };
	static ang::rtti_t const& info = ang::rtti::regist(name, genre::class_type, sizeof(ang::core::delegates::ifunction<void(Ts...)>), alignof(wsize), parents, &default_query_interface);
	return info;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Ts>
inline ang::rtti_t const& ang::core::delegates::function_object<T(Ts...)>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::core::delegates::function<"_s) += ang::type_of<T(Ts...)>().type_name()) += ">"_s);
	static ang::rtti_t const* parents[] = { &ang::type_of<object>(), &ang::type_of<ifunction<T(Ts...)>>() };
	static ang::rtti_t const& info = ang::rtti::regist(name, genre::class_type, sizeof(ang::core::delegates::function_object<T(Ts...)>), alignof(wsize), parents, &intf::default_query_interface);
	return info;
}

template<typename T, typename... Ts>
inline ang::rtti_t const& ang::core::delegates::function_object<T(Ts...)>::runtime_info()const
{
	return class_info();
}

template<typename T, typename... Ts>
inline bool ang::core::delegates::function_object<T(Ts...)>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<function_object<T(Ts...)>*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == ang::type_of<ifunction<T(Ts...)>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ifunction<T(Ts...)>*>(this);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename... Ts>
inline ang::rtti_t const& ang::core::delegates::function_object<void(Ts...)>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::core::delegates::function<"_s) += ang::type_of<void(Ts...)>().type_name()) += ">"_s);
	static ang::rtti_t const* parents[] = { &runtime::type_of<object>(), &runtime::type_of<ifunction<void(Ts...)>>() };
	static ang::rtti_t const& info = ang::rtti::regist(name, genre::class_type, sizeof(ang::core::delegates::function_object<void(Ts...)>), alignof(wsize), parents, &intf::default_query_interface);
	return info;
}

template<typename... Ts>
inline ang::rtti_t const& ang::core::delegates::function_object<void(Ts...)>::runtime_info()const
{
	return class_info();
}

template<typename... Ts>
inline bool ang::core::delegates::function_object<void(Ts...)>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<function_object<void(Ts...)>*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == ang::type_of<ifunction<void(Ts...)>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<ifunction<void(Ts...)>*>(this);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::object_wrapper()
	: m_ptr(null)
{

}

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::object_wrapper(ang::core::delegates::function_object<T(Ts...)>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_object<T(Ts...)> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::~object_wrapper()
{
	reset();
}

template<typename T, typename... Ts>
void ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename T, typename... Ts>
inline void ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T, typename... Ts>
inline bool ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T, typename... Ts>
inline ang::core::delegates::function_object<T(Ts...)>* ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::get(void)const
{
	return m_ptr;
}

template<typename T, typename... Ts>
inline void ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::set(ang::core::delegates::function_object<T(Ts...)>* ptr)
{
	core::delegates::function_object<T(Ts...)> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, typename... Ts>
ang::core::delegates::function_object<T(Ts...)>** ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>& ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::operator = (ang::core::delegates::function_object<T(Ts...)>* func)
{
	set(func);
	return*this;
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>& ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>& ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T, typename... Ts>
ang::object_wrapper_ptr<ang::core::delegates::function_object<T(Ts...)>> ang::object_wrapper<ang::core::delegates::function_object<T(Ts...)>>::operator & (void)
{
	return this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::object_wrapper()
	: m_ptr(null)
{

}

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::object_wrapper(ang::core::delegates::function_object<void(Ts...)>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_object<void(Ts...)> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::~object_wrapper()
{
	reset();
}

template<typename... Ts>
void ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename... Ts>
inline void ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename... Ts>
inline bool ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::is_empty()const
{
	return m_ptr == null;
}

template<typename... Ts>
inline ang::core::delegates::function_object<void(Ts...)>* ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::get(void)const
{
	return m_ptr;
}

template<typename... Ts>
inline void ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::set(ang::core::delegates::function_object<void(Ts...)>* ptr)
{
	core::delegates::function_object<void(Ts...)> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename... Ts>
ang::core::delegates::function_object<void(Ts...)>** ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::addres_of(void)
{
	return &m_ptr;
}

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>& ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::operator = (ang::core::delegates::function_object<void(Ts...)>* func)
{
	set(func);
	return*this;
}

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>& ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename... Ts>
ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>& ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename... Ts>
ang::object_wrapper_ptr<ang::core::delegates::function_object<void(Ts...)>> ang::object_wrapper<ang::core::delegates::function_object<void(Ts...)>>::operator & (void)
{
	return this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__OBJECT_WRAPPER_HPP__