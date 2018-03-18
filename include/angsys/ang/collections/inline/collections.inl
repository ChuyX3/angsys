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
		template<class K, class T>
		struct __type_info_builder_genre_class_helper<collections::pair<K, T>, false> : true_type {
			static rtti_t const& type_of() {
				static const cstr_view<char> name = strings::string_pool::instance()->save_string((((string("ang::collections::pair<"_s) += rtti::type_of<K>().type_name()) += ","_s) += rtti::type_of<T>().type_name()) += ">"_s);
				static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(collections::pair<K, T>), alignof(collections::pair<K, T>));
				return info;
			}
		};
	}
}


template<class T>
inline ang::rtti_t const& ang::collections::ienum<T>::class_info()
{
	static const cstr_view<char> name = strings::string_pool::instance()->save_string((string("ang::collections::ienum<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<interface>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::ienum<T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<class T>
inline ang::rtti_t const& ang::collections::iarray<T>::class_info()
{
	static const cstr_view<char> name = strings::string_pool::instance()->save_string((string("ang::collections::iarray<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::ienum<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::iarray<T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<class T>
inline ang::rtti_t const& ang::collections::ilist<T>::class_info()
{
	static const cstr_view<char> name = strings::string_pool::instance()->save_string((string("ang::collections::ilist<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::iarray<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::ilist<T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

template<class T>
inline ang::rtti_t const& ang::collections::isequence<T>::class_info()
{
	static const cstr_view<char> name = strings::string_pool::instance()->save_string((string("ang::collections::isequence<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<collections::ienum<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(ang::collections::isequence<T>), alignof(wsize), parents, &default_query_interface);
	return info;
}

/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
ang::collections::base_iterator<T>::base_iterator(ang::collections::base_iterator<T> const& it)
	: _parent(it.parent())
	, _current(it.current())
	, _offset(it.offset())
{
}

template<class T>
ang::collections::base_iterator<T>::base_iterator(ang::collections::ienum<T>* par, pointer cur, wsize off)
	: _parent(par)
	, _current(cur)
	, _offset(off)
{
}

template<class T>
ang::collections::base_iterator<T>::~base_iterator()
{
}

template<class T>
inline ang::collections::ienum<T>* ang::collections::base_iterator<T>::parent()const
{
	return _parent;
}

template<class T>
inline void ang::collections::base_iterator<T>::parent(ang::collections::ienum<T>* p)
{
	_parent = p;
}

template<class T>
inline pointer ang::collections::base_iterator<T>::current()const
{
	return _current;
}

template<class T>
inline void ang::collections::base_iterator<T>::current(pointer c)
{
	_current = c;
}

template<class T>
inline wsize ang::collections::base_iterator<T>::offset()const
{
	return _offset;
}

template<class T>
inline void ang::collections::base_iterator<T>::offset(wsize c)
{
	_offset = c;
}

template<class T>
inline bool ang::collections::base_iterator<T>::is_valid()const
{
	return bool(_current != null && _parent != null);
}



template<class T>
inline void ang::collections::base_iterator<T>::clear()
{
	_current = null;
}

template<class T>
inline ang::collections::base_iterator<T>& ang::collections::base_iterator<T>::operator = (ang::collections::base_iterator<T> const& it)
{
	_parent = it.parent();
	_current = it.current();
	_offset = it.offset();
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
ang::collections::iterator<T>::operator T*()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return &this->_parent->at(*this);
}

template<class T>
T* ang::collections::iterator<T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return &this->_parent->at(*this);
}

template<class T>
T& ang::collections::iterator<T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return this->_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
ang::collections::iterator<const T>::operator T const*()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return &this->_parent->at(*this);
}

template<class T>
T const* ang::collections::iterator<const T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return &this->_parent->at(*this);
}

template<class T>
T const& ang::collections::iterator<const T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	return this->_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<class T>
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

template<class T>
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

template<class T>
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

template<class T>
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

template<class T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this);
	return*this;
}

template<class T>
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

template<class T>
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

template<class T>
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

template<class T>
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

template<class T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<class T>
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

template<class T>
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

template<class T>
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

template<class T>
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

template<class T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw_exception(except_code::invalid_memory);
#endif
	this->_parent->increase(*this);
	return*this;
}

template<class T>
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

template<class T>
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

template<class T>
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

template<class T>
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


#endif//__ANG_COLLECTIONS_INL__