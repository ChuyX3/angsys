/*********************************************************************************************************************/
/*   File Name: ang/collections/inline/collections.hpp                                                               */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error Can't include collections.inl, please include angsys.h inside
#elif !defined __ANG_COLLECTIONS_INL__
#define __ANG_COLLECTIONS_INL__

namespace ang
{
	namespace runtime
	{
		template<typename K, typename T>
		struct __type_info_builder_genre_class_helper<collections::pair<K, T>, false> : true_type {
			static rtti_t const& type_of() {
				static const cstr_view<char> name = text::string_pool::instance()->save_string((((astring("ang::collections::pair<"_s) += rtti::type_of<K>().type_name()) += ","_s) += rtti::type_of<T>().type_name()) += ">"_s);
				static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<collections::pair<K, T>>(), align_of<collections::pair<K, T>>());
				return info;
			}
		};
	}
}


template<typename T>
inline ang::rtti_t const& ang::collections::ienum<T>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::collections::ienum<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<intf>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::ienum<T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<typename T>
inline ang::rtti_t const& ang::collections::iarray<T>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::collections::iarray<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::ienum<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::iarray<T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<typename T>
inline ang::rtti_t const& ang::collections::ilist<T>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::collections::ilist<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::iarray<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::ilist<T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<typename T>
inline ang::rtti_t const& ang::collections::isequence<T>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::collections::isequence<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::ienum<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::isequence<T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<typename K, typename T>
inline ang::rtti_t const& ang::collections::imap<K, T>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((((astring("ang::collections::imap<"_s) += rtti::type_of<K>().type_name()) += ","_s)+= rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::ienum<pair<K,T>>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::imap<K, T>), alignof(wsize), parents, &default_query_interface);
	return info;
}


template<ang::text::encoding E, template<typename>class A, typename T>
inline ang::rtti_t const& ang::collections::imap<ang::text::basic_string<E,A>, T>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((((astring("ang::collections::imap<"_s) += rtti::type_of<ang::text::basic_string<E, A>>().type_name()) += ","_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::ienum<pair<text::basic_string<E,A>,T>>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::imap<text::basic_string<E, A>, T>), alignof(wsize), parents, &default_query_interface);
	return info;
}


template<typename T>
inline ang::rtti_t const& ang::collections::imap<ang::string, T>::class_info()
{
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::collections::imap<ang::string,"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::ienum<pair<string,T>>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::imap<string, T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
ang::collections::base_iterator<T>::base_iterator(ang::collections::base_iterator<T> const& it)
	: _parent(it.parent())
	, _current(it.current())
	, _offset(it.offset())
{
}

template<typename T>
ang::collections::base_iterator<T>::base_iterator(ang::collections::ienum<T>* par, pointer cur, wsize off)
	: _parent(par)
	, _current(cur)
	, _offset(off)
{
}

template<typename T>
ang::collections::base_iterator<T>::~base_iterator()
{
}

template<typename T>
inline ang::collections::ienum<T>* ang::collections::base_iterator<T>::parent()const
{
	return _parent;
}

template<typename T>
inline void ang::collections::base_iterator<T>::parent(ang::collections::ienum<T>* p)
{
	_parent = p;
}

template<typename T>
inline pointer ang::collections::base_iterator<T>::current()const
{
	return _current;
}

template<typename T>
inline void ang::collections::base_iterator<T>::current(pointer c)
{
	_current = c;
}

template<typename T>
inline wsize ang::collections::base_iterator<T>::offset()const
{
	return _offset;
}

template<typename T>
inline void ang::collections::base_iterator<T>::offset(wsize c)
{
	_offset = c;
}

template<typename T>
inline bool ang::collections::base_iterator<T>::is_valid()const
{
	return bool(_current != null && _parent != null);
}



template<typename T>
inline void ang::collections::base_iterator<T>::clear()
{
	_current = null;
}

template<typename T>
inline ang::collections::base_iterator<T>& ang::collections::base_iterator<T>::operator = (ang::collections::base_iterator<T> const& it)
{
	_parent = it.parent();
	_current = it.current();
	_offset = it.offset();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
ang::collections::iterator<T>::operator T*()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return &this->_parent->at(*this);
}

template<typename T>
T* ang::collections::iterator<T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return &this->_parent->at(*this);
}

template<typename T>
T& ang::collections::iterator<T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return this->_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
ang::collections::iterator<const T>::operator T const*()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return &this->_parent->at(*this);
}

template<typename T>
T const* ang::collections::iterator<const T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return &this->_parent->at(*this);
}

template<typename T>
T const& ang::collections::iterator<const T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return this->_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	forward_iterator<T> saveState(*this);
	this->_parent->increase(*this);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator -- (int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	forward_iterator<T> saveState(*this);
	this->_parent->decrease(*this);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator + (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	forward_iterator<T> saveState(*this);
	this->_parent->increase(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator - (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	forward_iterator<T> saveState(*this);
	this->_parent->decrease(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	backward_iterator<T> saveState(*this);
	this->_parent->decrease(*this);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator -- (int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	backward_iterator<T> saveState(*this);
	this->_parent->increase(*this);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator + (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	backward_iterator<T> saveState(*this);
	this->_parent->decrease(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator - (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	backward_iterator<T> saveState(*this);
	this->_parent->increase(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	forward_iterator<const T> saveState(*this);
	this->_parent->increase(*this);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator -- (int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	forward_iterator<const T> saveState(*this);
	this->_parent->decrease(*this);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator + (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	forward_iterator<const T> saveState(*this);
	this->_parent->increase(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator - (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	forward_iterator<const T> saveState(*this);
	this->_parent->decrease(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this);
	return*this;
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	backward_iterator<const T> saveState(*this);
	this->_parent->decrease(*this);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator -- (int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	backward_iterator<const T> saveState(*this);
	this->_parent->increase(*this);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator + (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	backward_iterator<const T> saveState(*this);
	this->_parent->decrease(saveState);
	return saveState;
}

template<typename T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator - (int)const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	backward_iterator<const T> saveState(*this);
	this->_parent->increase(saveState);
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
	iobject * _obj = interface_cast<iobject>(m_ptr);
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
	iobject * _old = interface_cast<iobject>(m_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
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

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::operator ang::collections::ienum<T>* (void)
{
	return get();
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ienum<T>>::operator ang::collections::ienum<T> const* (void)const
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
	iobject * _obj = interface_cast<iobject>(m_ptr);
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
	iobject * _old = interface_cast<iobject>(m_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
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

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::operator ang::collections::iarray<T>* (void)
{
	return get();
}

template<typename T> inline
ang::intf_wrapper<ang::collections::iarray<T>>::operator ang::collections::iarray<T> const* (void)const
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
	iobject * _obj = interface_cast<iobject>(m_ptr);
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
	iobject * _old = interface_cast<iobject>(m_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
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
ang::collections::ilist<T> const* ang::intf_wrapper<ang::collections::ilist<T>>::operator -> (void)const
{
	return get();
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::operator ang::collections::ilist<T>* (void)
{
	return get();
}

template<typename T> inline
ang::intf_wrapper<ang::collections::ilist<T>>::operator ang::collections::ilist<T> const* (void)const
{
	return get();
}


#endif//__ANG_COLLECTIONS_INL__