/*********************************************************************************************************************/
/*   File Name: ang/delegates/inline/function.inl                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_FUNCTION_H__
#error ...
#elif !defined __ANG_FUNCTION_INL__
#define __ANG_FUNCTION_INL__

namespace ang
{
	template<typename T, typename... Ts>
	struct intf_class_info<ang::delegates::ifunction<T(Ts...)>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= rtti_from_type<ang::delegates::ifunction<T(Ts...)>>::types();
			static ang::rtti_t const& info = ang::rtti::regist([]()->fast_astring_t {
					fast_astring_t out;
					out << "ang::delegates::ifunction<"_sv << ang::type_of<T(Ts...)>().type_name() << ">"_sv;
					return ang::move(out);
				}()
				, gender::class_type
				, size_of<ang::delegates::ifunction<T(Ts...)>>()
				, align_of<ang::delegates::ifunction<T(Ts...)>>()
				, parents
				, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T, typename... Ts>
	struct intf_class_info<ang::delegates::function_base<T(Ts...)>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= rtti_from_type<ang::delegates::function_base<T(Ts...)>>::types();
			static ang::rtti_t const& info = ang::rtti::regist([]()->fast_astring_t {
					fast_astring_t out;
					out << "ang::delegates::function<"_sv << ang::type_of<T(Ts...)>().type_name() << ">"_sv;
					return ang::move(out);
				}()
				, gender::class_type
				, size_of<ang::delegates::function_base<T(Ts...)>>()
				, align_of<ang::delegates::function_base<T(Ts...)>>()
				, parents
				, &iintf::default_query_interface);
			return info;
		}
	};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::object_wrapper()
	: m_ptr(null)
{

}

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::object_wrapper(ang::delegates::function_base<T(Ts...)>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	delegates::function_base<T(Ts...)> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::~object_wrapper()
{
	reset();
}

template<typename T, typename... Ts>
void ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename T, typename... Ts>
inline void ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T, typename... Ts>
inline bool ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T, typename... Ts>
inline ang::delegates::function_base<T(Ts...)>* ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::get(void)const
{
	return m_ptr;
}

template<typename T, typename... Ts>
inline void ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::set(ang::delegates::function_base<T(Ts...)>* ptr)
{
	delegates::function_base<T(Ts...)> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T, typename... Ts>
ang::delegates::function_base<T(Ts...)>** ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>& ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::operator = (ang::delegates::function_base<T(Ts...)>* func)
{
	set(func);
	return*this;
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>& ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::operator = (ang::object_wrapper<ang::delegates::function_base<T(Ts...)>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T, typename... Ts>
ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>& ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::operator = (ang::object_wrapper<ang::delegates::function_base<T(Ts...)>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T, typename... Ts>
ang::object_wrapper_ptr<ang::delegates::function_base<T(Ts...)>> ang::object_wrapper<ang::delegates::function_base<T(Ts...)>>::operator & (void)
{
	return this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::object_wrapper()
	: m_ptr(null)
{

}

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::object_wrapper(ang::delegates::function_base<void(Ts...)>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	delegates::function_base<void(Ts...)> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::~object_wrapper()
{
	reset();
}

template<typename... Ts>
void ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

template<typename... Ts>
inline void ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename... Ts>
inline bool ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::is_empty()const
{
	return m_ptr == null;
}

template<typename... Ts>
inline ang::delegates::function_base<void(Ts...)>* ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::get(void)const
{
	return m_ptr;
}

template<typename... Ts>
inline void ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::set(ang::delegates::function_base<void(Ts...)>* ptr)
{
	delegates::function_base<void(Ts...)> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

template<typename... Ts>
ang::delegates::function_base<void(Ts...)>** ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::addres_of(void)
{
	return &m_ptr;
}

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>& ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::operator = (ang::delegates::function_base<void(Ts...)>* func)
{
	set(func);
	return*this;
}

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>& ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::operator = (ang::object_wrapper<ang::delegates::function_base<void(Ts...)>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename... Ts>
ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>& ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::operator = (ang::object_wrapper<ang ::delegates::function_base<void(Ts...)>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename... Ts>
ang::object_wrapper_ptr<ang::delegates::function_base<void(Ts...)>> ang::object_wrapper<ang::delegates::function_base<void(Ts...)>>::operator & (void)
{
	return this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__ANG_FUNCTION_INL__