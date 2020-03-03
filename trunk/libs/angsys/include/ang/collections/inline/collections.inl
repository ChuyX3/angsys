/*********************************************************************************************************************/
/*   File Name: coffe/collections/inline/collections.hpp                                                               */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __COFFE_H__
#error Can't include collections.inl, please include coffe.h inside
#elif !defined __COFFE_COLLECTIONS_INL__
#define __COFFE_COLLECTIONS_INL__

namespace coffe
{
	template<typename T>
	struct intf_class_info<collections::ienum<T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::ienum<T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "coffe::collections::ienum<"_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::ienum<T>), align_of<wsize>(), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::ienumerable<T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::ienumerable<T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "coffe::collections::ienumerable<"_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::ienumerable<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::iarray<T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::iarray<T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "coffe::collections::iarray<"_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::iarray<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::ivector<T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::ivector<T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "coffe::collections::ivector<"_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::ilist<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::ilist<T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::ilist<T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "coffe::collections::ilist<"_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::ilist<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::isequence_list<T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::isequence_list<T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t {
				fast_astring_t out;
				out << "coffe::collections::isequence_list<"_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::isequence<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::iset<T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::iset<T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "coffe::collections::iset<"_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::iset<T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename K, typename T>
	struct intf_class_info<collections::imap<K,T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::imap<K, T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "coffe::collections::imap<"_sv
					<< coffe::type_of<K>().type_name()
					<< ","_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::imap<K, T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<collections::imap<coffe::string, T>>
	{
		inline static coffe::rtti_t const& class_info()
		{
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<coffe::collections::imap<coffe::string, T>>::types();
			static coffe::rtti_t const& info = rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "coffe::collections::imap<coffe::string,"_sv
					<< coffe::type_of<T>().type_name()
					<< ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, sizeof(coffe::collections::imap<string, T>), alignof(wsize), parents, &iintf::default_query_interface);
			return info;
		}
	};
}


/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
coffe::collections::base_iterator<T>::base_iterator(coffe::collections::base_iterator<T> const& it)
	: m_parent(it.parent())
	, m_current(it.current())
	, m_offset(it.offset())
{
}

template<typename T>
coffe::collections::base_iterator<T>::base_iterator(coffe::collections::ienum<T>* par, pointer cur, wsize off)
	: m_parent(par)
	, m_current(cur)
	, m_offset(off)
{
}

template<typename T>
coffe::collections::base_iterator<T>::~base_iterator()
{
}

template<typename T>
inline coffe::collections::ienum<T>* coffe::collections::base_iterator<T>::parent()const
{
	return m_parent;
}

template<typename T>
inline void coffe::collections::base_iterator<T>::parent(coffe::collections::ienum<T>* p)
{
	m_parent = p;
}

template<typename T>
inline pointer coffe::collections::base_iterator<T>::current()const
{
	return m_current;
}

template<typename T>
inline void coffe::collections::base_iterator<T>::current(pointer c)
{
	m_current = c;
}

template<typename T>
inline wsize coffe::collections::base_iterator<T>::offset()const
{
	return m_offset;
}

template<typename T>
inline void coffe::collections::base_iterator<T>::offset(wsize c)
{
	m_offset = c;
}

template<typename T>
inline bool coffe::collections::base_iterator<T>::is_valid()const
{
	return bool(m_current != null && m_parent != null);
}



template<typename T>
inline void coffe::collections::base_iterator<T>::clear()
{
	m_current = null;
}

template<typename T>
inline coffe::collections::base_iterator<T>& coffe::collections::base_iterator<T>::operator = (coffe::collections::base_iterator<T> const& it)
{
	m_parent = it.parent();
	m_current = it.current();
	m_offset = it.offset();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
coffe::collections::iterator<T>::operator typename coffe::collections::auto_type<T>::ptr_type()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	return this->m_parent->at(*this);
}

template<typename T>
typename coffe::collections::auto_type<T>::ptr_type coffe::collections::iterator<T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	return this->m_parent->at(*this);
}

template<typename T>
typename coffe::collections::auto_type<T>::ref_type coffe::collections::iterator<T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	if constexpr(is_pointer<typename coffe::collections::auto_type<T>::return_type>::value)
		return *this->m_parent->at(*this);
	else
		return this->m_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
coffe::collections::iterator<const T>::operator typename coffe::collections::auto_type<T>::const_ptr_type()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	return this->m_parent->at(*this);
}

template<typename T>
typename coffe::collections::auto_type<T>::const_ptr_type coffe::collections::iterator<const T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	return this->m_parent->at(*this);
}

template<typename T>
typename coffe::collections::auto_type<T>::const_ref_type coffe::collections::iterator<const T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	if constexpr (is_pointer<typename coffe::collections::auto_type<T>::return_type>::value)
		return *this->m_parent->at(*this);
	else
		return this->m_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline coffe::collections::forward_iterator<T>& coffe::collections::forward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline coffe::collections::forward_iterator<T>& coffe::collections::forward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline coffe::collections::forward_iterator<T>& coffe::collections::forward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this);
	return*this;
}

template<typename T>
inline coffe::collections::forward_iterator<T>& coffe::collections::forward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this);
	return*this;
}

template<typename T>
inline coffe::collections::forward_iterator<T> coffe::collections::forward_iterator<T>::operator ++ (int)
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
inline coffe::collections::forward_iterator<T> coffe::collections::forward_iterator<T>::operator -- (int)
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
inline coffe::collections::forward_iterator<T> coffe::collections::forward_iterator<T>::operator + (int)const
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
inline coffe::collections::forward_iterator<T> coffe::collections::forward_iterator<T>::operator - (int)const
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
inline coffe::collections::backward_iterator<T>& coffe::collections::backward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline coffe::collections::backward_iterator<T>& coffe::collections::backward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline coffe::collections::backward_iterator<T>& coffe::collections::backward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this);
	return*this;
}

template<typename T>
inline coffe::collections::backward_iterator<T>& coffe::collections::backward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this);
	return*this;
}

template<typename T>
inline coffe::collections::backward_iterator<T> coffe::collections::backward_iterator<T>::operator ++ (int)
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
inline coffe::collections::backward_iterator<T> coffe::collections::backward_iterator<T>::operator -- (int)
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
inline coffe::collections::backward_iterator<T> coffe::collections::backward_iterator<T>::operator + (int)const
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
inline coffe::collections::backward_iterator<T> coffe::collections::backward_iterator<T>::operator - (int)const
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
inline coffe::collections::forward_iterator<const T>& coffe::collections::forward_iterator<const T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline coffe::collections::forward_iterator<const T>& coffe::collections::forward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline coffe::collections::forward_iterator<const T>& coffe::collections::forward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this);
	return*this;
}

template<typename T>
inline coffe::collections::forward_iterator<const T>& coffe::collections::forward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this);
	return*this;
}

template<typename T>
inline coffe::collections::forward_iterator<const T> coffe::collections::forward_iterator<const T>::operator ++ (int)
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
inline coffe::collections::forward_iterator<const T> coffe::collections::forward_iterator<const T>::operator -- (int)
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
inline coffe::collections::forward_iterator<const T> coffe::collections::forward_iterator<const T>::operator + (int)const
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
inline coffe::collections::forward_iterator<const T> coffe::collections::forward_iterator<const T>::operator - (int)const
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
inline coffe::collections::backward_iterator<const T>& coffe::collections::backward_iterator<const T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline coffe::collections::backward_iterator<const T>& coffe::collections::backward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline coffe::collections::backward_iterator<const T>& coffe::collections::backward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->decrease(*this);
	return*this;
}

template<typename T>
inline coffe::collections::backward_iterator<const T>& coffe::collections::backward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (this->m_parent == null)
		throw_exception(error_code::invalid_memory);
#endif
	this->m_parent->increase(*this);
	return*this;
}

template<typename T>
inline coffe::collections::backward_iterator<const T> coffe::collections::backward_iterator<const T>::operator ++ (int)
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
inline coffe::collections::backward_iterator<const T> coffe::collections::backward_iterator<const T>::operator -- (int)
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
inline coffe::collections::backward_iterator<const T> coffe::collections::backward_iterator<const T>::operator + (int)const
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
inline coffe::collections::backward_iterator<const T> coffe::collections::backward_iterator<const T>::operator - (int)const
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
coffe::intf_wrapper<coffe::collections::ienum<T>>::intf_wrapper() 
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>::intf_wrapper(coffe::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>::intf_wrapper(coffe::collections::ienum<T>* ptr) 
	: m_ptr(null)
{
	set(ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>::intf_wrapper(intf_wrapper && other)
	: m_ptr(null)
{
	coffe::collections::ienum<T> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>::intf_wrapper(intf_wrapper const& other) 
	: m_ptr(null)
{
	set(other.m_ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>::~intf_wrapper() {
	reset();
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::ienum<T>>::reset()
{
	iobject * _obj = coffe::interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::ienum<T>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T> inline
bool coffe::intf_wrapper<coffe::collections::ienum<T>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T> inline
coffe::collections::ienum<T>* coffe::intf_wrapper<coffe::collections::ienum<T>>::get(void)const
{
	return m_ptr;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::ienum<T>>::set(coffe::collections::ienum<T>* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = coffe::interface_cast<iobject>(m_ptr);
	iobject * _new = coffe::interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>& coffe::intf_wrapper<coffe::collections::ienum<T>>::operator = (coffe::collections::ienum<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>& coffe::intf_wrapper<coffe::collections::ienum<T>>::operator = (coffe::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>& coffe::intf_wrapper<coffe::collections::ienum<T>>::operator = (coffe::intf_wrapper<coffe::collections::ienum<T>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>& coffe::intf_wrapper<coffe::collections::ienum<T>>::operator = (coffe::intf_wrapper<coffe::collections::ienum<T>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T> inline
coffe::collections::ienum<T> ** coffe::intf_wrapper<coffe::collections::ienum<T>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T> inline
coffe::intf_wrapper_ptr<coffe::collections::ienum<T>> coffe::intf_wrapper<coffe::collections::ienum<T>>::operator & (void)
{
	return this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ienum<T>>::operator coffe::intfptr()const
{
	return static_interface_cast(get());
}

template<typename T> inline
coffe::collections::ienum<T>* coffe::intf_wrapper<coffe::collections::ienum<T>>::operator -> (void)
{
	return get();
}

template<typename T> inline
coffe::collections::ienum<T> const* coffe::intf_wrapper<coffe::collections::ienum<T>>::operator -> (void)const
{
	return get();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>::intf_wrapper()
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>::intf_wrapper(coffe::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>::intf_wrapper(coffe::collections::iarray<T>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>::intf_wrapper(intf_wrapper && other)
	: m_ptr(null)
{
	coffe::collections::iarray<T> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>::intf_wrapper(intf_wrapper const& other)
	: m_ptr(null)
{
	set(other.m_ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>::~intf_wrapper() {
	reset();
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::iarray<T>>::reset()
{
	iobject * _obj = coffe::interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::iarray<T>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T> inline
bool coffe::intf_wrapper<coffe::collections::iarray<T>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T> inline
coffe::collections::iarray<T>* coffe::intf_wrapper<coffe::collections::iarray<T>>::get(void)const
{
	return m_ptr;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::iarray<T>>::set(coffe::collections::iarray<T>* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = coffe::interface_cast<iobject>(m_ptr);
	iobject * _new = coffe::interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>& coffe::intf_wrapper<coffe::collections::iarray<T>>::operator = (coffe::collections::iarray<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>& coffe::intf_wrapper<coffe::collections::iarray<T>>::operator = (coffe::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>& coffe::intf_wrapper<coffe::collections::iarray<T>>::operator = (coffe::intf_wrapper<coffe::collections::iarray<T>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>& coffe::intf_wrapper<coffe::collections::iarray<T>>::operator = (coffe::intf_wrapper<coffe::collections::iarray<T>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T> inline
coffe::collections::iarray<T> ** coffe::intf_wrapper<coffe::collections::iarray<T>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T> inline
coffe::intf_wrapper_ptr<coffe::collections::iarray<T>> coffe::intf_wrapper<coffe::collections::iarray<T>>::operator & (void)
{
	return this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::iarray<T>>::operator coffe::intfptr()const
{
	return static_interface_cast(get());
}

template<typename T> inline
coffe::collections::iarray<T>* coffe::intf_wrapper<coffe::collections::iarray<T>>::operator -> (void)
{
	return get();
}

template<typename T> inline
coffe::collections::iarray<T> const* coffe::intf_wrapper<coffe::collections::iarray<T>>::operator -> (void)const
{
	return get();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>::intf_wrapper()
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>::intf_wrapper(coffe::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>::intf_wrapper(coffe::collections::ilist<T>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>::intf_wrapper(intf_wrapper && other)
	: m_ptr(null)
{
	coffe::collections::ilist<T> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>::intf_wrapper(intf_wrapper const& other)
	: m_ptr(null)
{
	set(other.m_ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>::~intf_wrapper() {
	reset();
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::ilist<T>>::reset()
{
	iobject * _obj = coffe::interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::ilist<T>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T> inline
bool coffe::intf_wrapper<coffe::collections::ilist<T>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T> inline
coffe::collections::ilist<T>* coffe::intf_wrapper<coffe::collections::ilist<T>>::get(void)const
{
	return m_ptr;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::collections::ilist<T>>::set(coffe::collections::ilist<T>* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = coffe::interface_cast<iobject>(m_ptr);
	iobject * _new = coffe::interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>& coffe::intf_wrapper<coffe::collections::ilist<T>>::operator = (coffe::collections::ilist<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>& coffe::intf_wrapper<coffe::collections::ilist<T>>::operator = (coffe::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>& coffe::intf_wrapper<coffe::collections::ilist<T>>::operator = (coffe::intf_wrapper<coffe::collections::ilist<T>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>& coffe::intf_wrapper<coffe::collections::ilist<T>>::operator = (coffe::intf_wrapper<coffe::collections::ilist<T>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T> inline
coffe::collections::ilist<T> ** coffe::intf_wrapper<coffe::collections::ilist<T>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T> inline
coffe::intf_wrapper_ptr<coffe::collections::ilist<T>> coffe::intf_wrapper<coffe::collections::ilist<T>>::operator & (void)
{
	return this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::collections::ilist<T>>::operator coffe::intfptr()const
{
	return static_interface_cast(get());
}

template<typename T> inline
coffe::collections::ilist<T>* coffe::intf_wrapper<coffe::collections::ilist<T>>::operator -> (void)
{
	return get();
}

template<typename T> inline
coffe::collections::ilist<T>const* coffe::intf_wrapper<coffe::collections::ilist<T>>::operator -> (void)const
{
	return get();
}

#endif//__COFFE_COLLECTIONS_INL__
