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
		struct runtime_type_builder<collections::pair<K,T>>
		{
			typedef collections::pair<K, T> type;
			static inline type_name_t type_of() {
				static type_name_t out = runtime_data_base::regist_typename(ang::move(((("ang::collections::pair<"_o += runtime::type_of<K>()) += ","_s) += runtime::type_of<T>()) += ">"_s));
				return out;
			}

			static inline bool is_type_of(type_name_t name) {
				return name == type_of();
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				if (is_type_of(runtime::type_of<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};
	}
}


template<class T>
inline bool ang::collections::ienum<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline bool ang::collections::ienum<T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline ang::type_name_t ang::collections::ienum<T>::class_name() {
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::collections::ienum<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<class T>
inline ang::type_name_t ang::collections::ienum<T>::object_name()const
{
	return ang::collections::ienum<T>::class_name();
}

template<class T>
inline bool ang::collections::ienum<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::collections::ienum<T>*>(this);
		return true;
	}
	return false;
}

template<class T>
inline bool ang::collections::iarray<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::iarray<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline bool ang::collections::iarray<T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::iarray<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline ang::type_name_t ang::collections::iarray<T>::class_name() {
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::collections::iarray<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<class T>
inline ang::type_name_t ang::collections::iarray<T>::object_name()const
{
	return ang::collections::iarray<T>::class_name();
}

template<class T>
inline bool ang::collections::iarray<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == ang::collections::iarray<T>::class_name())
	{
		*out = static_cast<ang::collections::iarray<T>*>(this);
		return true;
	}
	else if (name == ang::collections::ienum<T>::class_name())
	{
		*out = static_cast<ang::collections::ienum<T>*>(this);
		return true;
	}
	return false;
}

template<class T>
inline bool ang::collections::isequence<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::isequence<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline bool ang::collections::isequence<T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::isequence<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline ang::type_name_t ang::collections::isequence<T>::class_name() {
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::collections::isequence<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<class T>
inline ang::type_name_t ang::collections::isequence<T>::object_name()const
{
	return ang::collections::isequence<T>::class_name();
}

template<class T>
inline bool ang::collections::isequence<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == ang::collections::isequence<T>::class_name())
	{
		*out = static_cast<ang::collections::isequence<T>*>(this);
		return true;
	}
	else if (name == ang::collections::ienum<T>::class_name())
	{
		*out = static_cast<ang::collections::ienum<T>*>(this);
		return true;
	}
	return false;
}

template<class T>
inline bool ang::collections::ilist<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::ilist<T>::class_name()
		|| name == ang::collections::iarray<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline bool ang::collections::ilist<T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::ilist<T>::class_name()
		|| name == ang::collections::iarray<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline ang::type_name_t ang::collections::ilist<T>::class_name()
{
	static string _type_name = ang::move("ang::collections::ilist<"_o + type_of<T>() + ">"_s);
	return _type_name->cstr();
}

template<class T>
inline ang::type_name_t ang::collections::ilist<T>::object_name()const
{
	return ang::collections::ilist<T>::class_name();
}

template<class T>
inline bool ang::collections::ilist<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == ang::collections::ilist<T>::class_name())
	{
		*out = static_cast<ang::collections::ilist<T>*>(this);
		return true;
	}
	else if (ang::collections::iarray<T>::query_object(name, out))
	{
		return true;
	}
	return false;
}

template<class K, class T>
inline bool ang::collections::imap<K, T>::is_kind_of(ang::type_name_t name)const
{
	return (name == ang::collections::imap<K, T>::class_name())
		|| (name == ang::collections::ienum<pair<K, T>>::class_name());
}

template<class K, class T>
inline bool ang::collections::imap<K, T>::is_child_of(ang::type_name_t name)
{
	return (name == ang::collections::imap<K, T>::class_name())
		|| (name == ang::collections::ienum<pair<K, T>>::class_name());
}

template<class K, class T>
inline ang::type_name_t ang::collections::imap<K, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::imap<"_o + type_of<K>() + ","_s + type_of<T>() + ">"_s);
	return _type_name->cstr();
}

template<class K, class T>
inline ang::type_name_t ang::collections::imap<K, T>::object_name()const
{
	return ang::collections::imap<K, T>::class_name();
}

template<class K, class T>
inline bool ang::collections::imap<K, T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == ang::collections::imap<K, T>::class_name())
	{
		*out = static_cast<ang::collections::imap<K, T>*>(this);
		return true;
	}
	else if (ang::collections::iarray<pair<K,T>>::query_object(name, out))
	{
		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
ang::collections::base_iterator<T>::base_iterator(ang::collections::base_iterator<T> const& it)
	: _parent(it.parent())
	, _current(it.current())
	, _offset(it.offset())
{
}

template<class T>
ang::collections::base_iterator<T>::base_iterator(ang::collections::ienum<T>* par, ang::collections::position_t cur, wsize off)
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
inline ang::collections::position_t ang::collections::base_iterator<T>::current()const
{
	return _current;
}

template<class T>
inline void ang::collections::base_iterator<T>::current(ang::collections::position_t c)
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
inline void ang::collections::base_iterator<T>::clean()
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
		throw(exception_t(except_code::invalid_memory));
#endif
	return &this->_parent->at(*this);
}

template<class T>
T* ang::collections::iterator<T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return &this->_parent->at(*this);
}

template<class T>
T& ang::collections::iterator<T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return this->_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
ang::collections::iterator<const T>::operator T const*()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return &this->_parent->at(*this);
}

template<class T>
T const* ang::collections::iterator<const T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return &this->_parent->at(*this);
}

template<class T>
T const& ang::collections::iterator<const T>::operator * ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return this->_parent->at(*this);
}

/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->increase(*this);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<T>& ang::collections::forward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<T> ang::collections::forward_iterator<T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<T>& ang::collections::backward_iterator<T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->increase(*this);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<T> ang::collections::backward_iterator<T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->increase(*this);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<const T>& ang::collections::forward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<class T>
inline ang::collections::forward_iterator<const T> ang::collections::forward_iterator<const T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->decrease(*this, val);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->increase(*this, val);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator ++ ()
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->decrease(*this);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<const T>& ang::collections::backward_iterator<const T>::operator -- ()
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
#endif
	this->_parent->increase(*this);
	return*this;
}

template<class T>
inline ang::collections::backward_iterator<const T> ang::collections::backward_iterator<const T>::operator ++ (int)
{
#ifdef DEBUG_SAFE_CODE
		if (_parent == null)
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
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
			throw(exception_t(except_code::invalid_memory));
#endif
	backward_iterator<const T> saveState(*this);
	this->_parent->increase(saveState);
	return saveState;
}


#endif//__ANG_COLLECTIONS_INL__