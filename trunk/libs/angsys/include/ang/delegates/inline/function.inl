#ifndef __COFFE_FUNCTION_H__
#error ...
#elif !defined __COFFE_FUNCTION_INL__
#define __COFFE_FUNCTION_INL__

namespace coffe
{
	template<typename T, typename... Ts>
	struct intf_class_info<coffe::delegates::ifunction<T(Ts...)>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= rtti_from_type<coffe::delegates::ifunction<T(Ts...)>>::types();
			static coffe::rtti_t const& info = coffe::rtti::regist([]()->fast_astring_t {
					fast_astring_t out;
					out << "coffe::delegates::ifunction<"_sv << coffe::type_of<T(Ts...)>().type_name() << ">"_sv;
					return coffe::move(out);
				}()
				, gender::class_type
				, size_of<coffe::delegates::ifunction<T(Ts...)>>()
				, align_of<coffe::delegates::ifunction<T(Ts...)>>()
				, parents
				, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T, typename... Ts>
	struct intf_class_info<coffe::delegates::function_base<T(Ts...)>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= rtti_from_type<coffe::delegates::function_base<T(Ts...)>>::types();
			static coffe::rtti_t const& info = coffe::rtti::regist([]()->fast_astring_t {
					fast_astring_t out;
					out << "coffe::delegates::function<"_sv << coffe::type_of<T(Ts...)>().type_name() << ">"_sv;
					return coffe::move(out);
				}()
				, gender::class_type
				, size_of<coffe::delegates::function_base<T(Ts...)>>()
				, align_of<coffe::delegates::function_base<T(Ts...)>>()
				, parents
				, &iintf::default_query_interface);
			return info;
		}
	};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::object_wrapper()
	: m_ptr(null)
{

}

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::object_wrapper(coffe::delegates::function_base<T(Ts...)>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	delegates::function_base<T(Ts...)> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::~object_wrapper()
{
	reset();
}

template<typename T, typename... Ts>
void coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename T, typename... Ts>
inline void coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T, typename... Ts>
inline bool coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T, typename... Ts>
inline coffe::delegates::function_base<T(Ts...)>* coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::get(void)const
{
	return m_ptr;
}

template<typename T, typename... Ts>
inline void coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::set(coffe::delegates::function_base<T(Ts...)>* ptr)
{
	delegates::function_base<T(Ts...)> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, typename... Ts>
coffe::delegates::function_base<T(Ts...)>** coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>& coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::operator = (coffe::delegates::function_base<T(Ts...)>* func)
{
	set(func);
	return*this;
}

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>& coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::operator = (coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T, typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>& coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::operator = (coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T, typename... Ts>
coffe::object_wrapper_ptr<coffe::delegates::function_base<T(Ts...)>> coffe::object_wrapper<coffe::delegates::function_base<T(Ts...)>>::operator & (void)
{
	return this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::object_wrapper()
	: m_ptr(null)
{

}

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::object_wrapper(coffe::delegates::function_base<void(Ts...)>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	delegates::function_base<void(Ts...)> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::~object_wrapper()
{
	reset();
}

template<typename... Ts>
void coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename... Ts>
inline void coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename... Ts>
inline bool coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::is_empty()const
{
	return m_ptr == null;
}

template<typename... Ts>
inline coffe::delegates::function_base<void(Ts...)>* coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::get(void)const
{
	return m_ptr;
}

template<typename... Ts>
inline void coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::set(coffe::delegates::function_base<void(Ts...)>* ptr)
{
	delegates::function_base<void(Ts...)> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename... Ts>
coffe::delegates::function_base<void(Ts...)>** coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::addres_of(void)
{
	return &m_ptr;
}

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>& coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::operator = (coffe::delegates::function_base<void(Ts...)>* func)
{
	set(func);
	return*this;
}

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>& coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::operator = (coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename... Ts>
coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>& coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::operator = (coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename... Ts>
coffe::object_wrapper_ptr<coffe::delegates::function_base<void(Ts...)>> coffe::object_wrapper<coffe::delegates::function_base<void(Ts...)>>::operator & (void)
{
	return this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__COFFE_FUNCTION_INL__