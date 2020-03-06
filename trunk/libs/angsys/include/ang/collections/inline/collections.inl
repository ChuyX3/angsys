/*********************************************************************************************************************/
/*   File Name: ang/inline/collections.inl                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error Can't include collections.inl, please include angsys.h inside
#elif !defined __ANG_COLLECTIONS_INL__
#define __ANG_COLLECTIONS_INL__

namespace ang
{
	template<typename T>
	struct intf_class_info<collections::ienum<T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::ienum<T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "ang::collections::ienum<"_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::ienum<T>), align_of<wsize>(), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::ienumerable<T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::ienumerable<T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "ang::collections::ienumerable<"_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::ienumerable<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::iarray<T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::iarray<T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "ang::collections::iarray<"_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::iarray<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::ivector<T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::ivector<T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "ang::collections::ivector<"_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::ilist<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::ilist<T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::ilist<T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "ang::collections::ilist<"_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::ilist<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::isequence_list<T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::isequence_list<T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "ang::collections::isequence_list<"_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::isequence<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::iset<T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::iset<T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "ang::collections::iset<"_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::iset<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename K, typename T>
	struct intf_class_info<collections::imap<K,T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::imap<K, T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "ang::collections::imap<"_sv
					<< ang::type_of<K>().type_name()
					<< ","_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::imap<K, T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::imap<ang::string, T>>
	{
		inline static ang::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= ang::rtti_from_type<ang::collections::imap<ang::string, T>>::types();
			static ang::rtti_t const& info = rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "ang::collections::imap<ang::string,"_sv
					<< ang::type_of<T>().type_name()
					<< ">"_sv;
				return ang::move(out);
			}(), gender::class_type, sizeof(ang::collections::imap<string, T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};
}


/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
ang::collections::base_iterator<T>::base_iterator(ang::collections::base_iterator<T> const& it)
	: m_parent(it.parent())
	, m_current(it.current())
	, m_offset(it.offset())
{
}

template<typename T>
ang::collections::base_iterator<T>::base_iterator(ang::collections::ienum<T>* par, pointer cur, wsize off)
	: m_parent(par)
	, m_current(cur)
	, m_offset(off)
{
}

template<typename T>
ang::collections::base_iterator<T>::~base_iterator()
{
}

template<typename T>
inline ang::collections::ienum<T>* ang::collections::base_iterator<T>::parent()const
{
	return m_parent;
}

template<typename T>
inline void ang::collections::base_iterator<T>::parent(ang::collections::ienum<T>* p)
{
	m_parent = p;
}

template<typename T>
inline pointer ang::collections::base_iterator<T>::current()const
{
	return m_current;
}

template<typename T>
inline void ang::collections::base_iterator<T>::current(pointer c)
{
	m_current = c;
}

template<typename T>
inline wsize ang::collections::base_iterator<T>::offset()const
{
	return m_offset;
}

template<typename T>
inline void ang::collections::base_iterator<T>::offset(wsize c)
{
	m_offset = c;
}

template<typename T>
inline bool ang::collections::base_iterator<T>::is_valid()const
{
	return bool(m_current != null && m_parent != null);
}



template<typename T>
inline void ang::collections::base_iterator<T>::clear()
{
	m_current = null;
}

template<typename T>
inline ang::collections::base_iterator<T>& ang::collections::base_iterator<T>::operator = (ang::collections::base_iterator<T> const& it)
{
	m_parent = it.parent();
	m_current = it.current();
	m_offset = it.offset();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
ang::collections::iterator<T>::operator typename ang::collections::auto_type<T>::ptr_type()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	return this->m_parent->at(*this);
}

template<typename T>
typename ang::collections::auto_type<T>::ptr_type ang::collections::iterator<T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	return this->m_parent->at(*this);
}

template<typename T>
typename ang::collections::auto_type<T>::ref_type ang::collections::iterator<T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	if constexpr(is_pointer<typename ang::collections::auto_type<T>::return_type>::value)
		return *this->m_parent->at(*this);
	else
		return this->m_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
ang::collections::iterator<const T>::operator typename ang::collections::auto_type<T>::const_ptr_type()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	return this->m_parent->at(*this);
}

template<typename T>
typename ang::collections::auto_type<T>::const_ptr_type ang::collections::iterator<const T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	return this->m_parent->at(*this);
}

template<typename T>
typename ang::collections::auto_type<T>::const_ref_type ang::collections::iterator<const T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	if constexpr (is_pointer<typename ang::collections::auto_type<T>::return_type>::value)
		return *this->m_parent->at(*this);
	else
		return this->m_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	forward_iterator<T> saveState(*this);
	this->m_parent->increase(*this);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator -- (int)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	forward_iterator<T> saveState(*this);
	this->m_parent->decrease(*this);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator + (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	forward_iterator<T> saveState(*this);
	this->m_parent->increase(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator - (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	forward_iterator<T> saveState(*this);
	this->m_parent->decrease(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	backward_iterator<T> saveState(*this);
	this->m_parent->decrease(*this);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator -- (int)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	backward_iterator<T> saveState(*this);
	this->m_parent->increase(*this);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator + (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	backward_iterator<T> saveState(*this);
	this->m_parent->decrease(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator - (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	backward_iterator<T> saveState(*this);
	this->m_parent->increase(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	forward_iterator<const T> saveState(*this);
	this->m_parent->increase(*this);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator -- (int)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	forward_iterator<const T> saveState(*this);
	this->m_parent->decrease(*this);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator + (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	forward_iterator<const T> saveState(*this);
	this->m_parent->increase(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator - (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	forward_iterator<const T> saveState(*this);
	this->m_parent->decrease(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	backward_iterator<const T> saveState(*this);
	this->m_parent->decrease(*this);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator -- (int)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	backward_iterator<const T> saveState(*this);
	this->m_parent->increase(*this);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator + (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	backward_iterator<const T> saveState(*this);
	this->m_parent->decrease(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator - (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	backward_iterator<const T> saveState(*this);
	this->m_parent->increase(saveState);
	return saveState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper() 
	: m_ptr(null)
{
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper(ang::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper(ang::collections::ienum<T>* ptr) 
	: m_ptr(null)
{
	set(ptr);
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper(intf_wrapper && other)
	: m_ptr(null)
{
	ang::collections::ienum<T> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper(intf_wrapper const& other) 
	: m_ptr(null)
{
	set(other.m_ptr);
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::~intf_wrapper() {
	reset();
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::ienum<T>>::reset()
{
	iobject * _obj = ang::interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::ienum<T>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T> inline
bool ang::intf_wrapper<ang::collections::ienum<T>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T> inline
ang::collections::ienum<T>* ang::intf_wrapper<ang::collections::ienum<T>>::get(void)const
{
	return m_ptr;
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::ienum<T>>::set(ang::collections::ienum<T>* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = ang::interface_cast<iobject>(m_ptr);
	iobject * _new = ang::interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>& ang::intf_wrapper<ang::collections::ienum<T>>::operator = (ang::collections::ienum<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>& ang::intf_wrapper<ang::collections::ienum<T>>::operator = (ang::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>& ang::intf_wrapper<ang::collections::ienum<T>>::operator = (ang::intf_wrapper<ang::collections::ienum<T>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>& ang::intf_wrapper<ang::collections::ienum<T>>::operator = (ang::intf_wrapper<ang::collections::ienum<T>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T> inline
ang::collections::ienum<T> ** ang::intf_wrapper<ang::collections::ienum<T>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T> inline
ang::intf_wrapper_ptr<ang::collections::ienum<T>> ang::intf_wrapper<ang::collections::ienum<T>>::operator & (void)
{
	return this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::operator ang::intfptr()const
{
	return static_interface_cast(get());
}

template<typename T> inline
ang::collections::ienum<T>* ang::intf_wrapper<ang::collections::ienum<T>>::operator -> (void)
{
	return get();
}

template<typename T> inline
ang::collections::ienum<T> const* ang::intf_wrapper<ang::collections::ienum<T>>::operator -> (void)const
{
	return get();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper()
	: m_ptr(null)
{
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper(ang::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper(ang::collections::iarray<T>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper(intf_wrapper && other)
	: m_ptr(null)
{
	ang::collections::iarray<T> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper(intf_wrapper const& other)
	: m_ptr(null)
{
	set(other.m_ptr);
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::~intf_wrapper() {
	reset();
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::iarray<T>>::reset()
{
	iobject * _obj = ang::interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::iarray<T>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T> inline
bool ang::intf_wrapper<ang::collections::iarray<T>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T> inline
ang::collections::iarray<T>* ang::intf_wrapper<ang::collections::iarray<T>>::get(void)const
{
	return m_ptr;
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::iarray<T>>::set(ang::collections::iarray<T>* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = ang::interface_cast<iobject>(m_ptr);
	iobject * _new = ang::interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>& ang::intf_wrapper<ang::collections::iarray<T>>::operator = (ang::collections::iarray<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>& ang::intf_wrapper<ang::collections::iarray<T>>::operator = (ang::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>& ang::intf_wrapper<ang::collections::iarray<T>>::operator = (ang::intf_wrapper<ang::collections::iarray<T>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>& ang::intf_wrapper<ang::collections::iarray<T>>::operator = (ang::intf_wrapper<ang::collections::iarray<T>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T> inline
ang::collections::iarray<T> ** ang::intf_wrapper<ang::collections::iarray<T>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T> inline
ang::intf_wrapper_ptr<ang::collections::iarray<T>> ang::intf_wrapper<ang::collections::iarray<T>>::operator & (void)
{
	return this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::operator ang::intfptr()const
{
	return static_interface_cast(get());
}

template<typename T> inline
ang::collections::iarray<T>* ang::intf_wrapper<ang::collections::iarray<T>>::operator -> (void)
{
	return get();
}

template<typename T> inline
ang::collections::iarray<T> const* ang::intf_wrapper<ang::collections::iarray<T>>::operator -> (void)const
{
	return get();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper()
	: m_ptr(null)
{
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper(ang::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper(ang::collections::ilist<T>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper(intf_wrapper && other)
	: m_ptr(null)
{
	ang::collections::ilist<T> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper(intf_wrapper const& other)
	: m_ptr(null)
{
	set(other.m_ptr);
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::~intf_wrapper() {
	reset();
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::ilist<T>>::reset()
{
	iobject * _obj = ang::interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::ilist<T>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T> inline
bool ang::intf_wrapper<ang::collections::ilist<T>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T> inline
ang::collections::ilist<T>* ang::intf_wrapper<ang::collections::ilist<T>>::get(void)const
{
	return m_ptr;
}

template<typename T> inline
void ang::intf_wrapper<ang::collections::ilist<T>>::set(ang::collections::ilist<T>* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = ang::interface_cast<iobject>(m_ptr);
	iobject * _new = ang::interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>& ang::intf_wrapper<ang::collections::ilist<T>>::operator = (ang::collections::ilist<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>& ang::intf_wrapper<ang::collections::ilist<T>>::operator = (ang::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>& ang::intf_wrapper<ang::collections::ilist<T>>::operator = (ang::intf_wrapper<ang::collections::ilist<T>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>& ang::intf_wrapper<ang::collections::ilist<T>>::operator = (ang::intf_wrapper<ang::collections::ilist<T>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T> inline
ang::collections::ilist<T> ** ang::intf_wrapper<ang::collections::ilist<T>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T> inline
ang::intf_wrapper_ptr<ang::collections::ilist<T>> ang::intf_wrapper<ang::collections::ilist<T>>::operator & (void)
{
	return this;
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::operator ang::intfptr()const
{
	return static_interface_cast(get());
}

template<typename T> inline
ang::collections::ilist<T>* ang::intf_wrapper<ang::collections::ilist<T>>::operator -> (void)
{
	return get();
}

template<typename T> inline
ang::collections::ilist<T>const* ang::intf_wrapper<ang::collections::ilist<T>>::operator -> (void)const
{
	return get();
}

#endif//__ANG_COLLECTIONS_INL__
