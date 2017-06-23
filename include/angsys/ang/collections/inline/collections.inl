/*********************************************************************************************************************/
/*   File Name: ang/collections/inline/collections.hpp                                                               */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_COLLECTIONS_H__
#error Can't include collections.hpp, please include angsys.h inside
#elif !defined __ANG_COLLECTIONS_HPP__
#define __ANG_COLLECTIONS_HPP__

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
inline ang::type_name_t ang::collections::ienum<T>::class_name()
{
	static string _type_name = ang::move("ang::collections::ienum<"_o + type_name<T>() + ">"_s);
	return _type_name->cstr();
}

template<class T>
inline ang::type_name_t ang::collections::ienum<T>::object_name()const
{
	return ang::collections::ienum<T>::class_name();
}

template<class T>
inline bool ang::collections::ienum<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == class_name())
	{
		*out = static_cast<ang::collections::ienum<T>*>(this);
		return true;
	}
	return false;
}

template<class T>
inline bool ang::collections::icollection<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::icollection<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline bool ang::collections::icollection<T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::icollection<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline ang::type_name_t ang::collections::icollection<T>::class_name()
{
	static string _type_name = ang::move("ang::collections::icollection<"_o + type_name<T>() + ">"_s);
	return _type_name->cstr();
}

template<class T>
inline ang::type_name_t ang::collections::icollection<T>::object_name()const
{
	return ang::collections::icollection<T>::class_name();
}

template<class T>
inline bool ang::collections::icollection<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == ang::collections::icollection<T>::class_name())
	{
		*out = static_cast<ang::collections::icollection<T>*>(this);
		return true;
	}
	else if (className == ang::collections::ienum<T>::class_name())
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
		|| name == ang::collections::icollection<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline bool ang::collections::ilist<T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::ilist<T>::class_name()
		|| name == ang::collections::icollection<T>::class_name()
		|| name == ang::collections::ienum<T>::class_name())
		return true;
	return false;
}

template<class T>
inline ang::type_name_t ang::collections::ilist<T>::class_name()
{
	static string _type_name = ang::move("ang::collections::ilist<"_o + type_name<T>() + ">"_s);
	return _type_name->cstr();
}

template<class T>
inline ang::type_name_t ang::collections::ilist<T>::object_name()const
{
	return ang::collections::ilist<T>::class_name();
}

template<class T>
inline bool ang::collections::ilist<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == ang::collections::ilist<T>::class_name())
	{
		*out = static_cast<ang::collections::ilist<T>*>(this);
		return true;
	}
	else if (ang::collections::icollection<T>::query_object(className, out))
	{
		return true;
	}
	return false;
}

template<class K, class T>
inline bool ang::collections::imap<K, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::imap<K, T>::class_name())
		return true;
	return false;
}

template<class K, class T>
inline bool ang::collections::imap<K, T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::imap<K, T>::class_name())
		return true;
	return false;
}

template<class K, class T>
inline ang::type_name_t ang::collections::imap<K, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::imap<"_o + type_name<K>() + ","_s + type_name<T>() + ">"_s);
	return _type_name->cstr();
}

template<class K, class T>
inline ang::type_name_t ang::collections::imap<K, T>::object_name()const
{
	return ang::collections::imap<K, T>::class_name();
}

template<class K, class T>
inline bool ang::collections::imap<K, T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == ang::collections::imap<K, T>::class_name())
	{
		*out = static_cast<ang::collections::imap<K, T>*>(this);
		return true;
	}
	else if (ang::collections::icollection<pair<K,T>>::query_object(className, out))
	{
		return true;
	}
	return false;
}


template<class T>
inline bool ang::collections::imap<ang::string, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::imap<ang::string, T>::class_name())
		return true;
	return false;
}

template<class T>
inline bool ang::collections::imap<ang::string, T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::imap<ang::string, T>::class_name())
		return true;
	return false;
}

template<class T>
inline ang::type_name_t ang::collections::imap<ang::string, T>::class_name()
{
	static string _type_name = ang::move("ang::collections::imap<ang::string"_o + ","_s + type_name<T>() + ">"_s);
	return _type_name->cstr();
}

template<class T>
inline ang::type_name_t ang::collections::imap<ang::string, T>::object_name()const
{
	return ang::collections::imap<ang::string, T>::class_name();
}

template<class T>
inline bool ang::collections::imap<ang::string, T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == ang::collections::imap<ang::string, T>::class_name())
	{
		*out = static_cast<ang::collections::imap<ang::string, T>*>(this);
		return true;
	}
	else if (ang::collections::icollection<pair<ang::string, T>>::query_object(className, out))
	{
		return true;
	}
	return false;
}

template<class T>
inline bool ang::collections::imap<ang::wstring, T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::collections::imap<ang::wstring, T>::class_name())
		return true;
	return false;
}

template<class T>
inline bool ang::collections::imap<ang::wstring, T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::imap<ang::wstring, T>::class_name())
		return true;
	return false;
}

template<class T>
inline ang::type_name_t ang::collections::imap<ang::wstring, T>::class_name()
{
	static wstring _type_name = ang::move("ang::collections::imap<ang::wstring"_o + ","_s + type_name<T>() + ">"_s);
	return _type_name->cstr();
}

template<class T>
inline ang::type_name_t ang::collections::imap<ang::wstring, T>::object_name()const
{
	return ang::collections::imap<ang::wstring, T>::class_name();
}

template<class T>
inline bool ang::collections::imap<ang::wstring, T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == ang::collections::imap<ang::wstring, T>::class_name())
	{
		*out = static_cast<ang::collections::imap<ang::wstring, T>*>(this);
		return true;
	}
	else if (ang::collections::icollection<pair<ang::wstring, T>>::query_object(className, out))
	{
		return true;
	}
	return false;
}
//
//template<class K, class T>
//inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::imap<K, T>::find_index(index)const
//{
//	return ang::collections::iterator<ang::collections::pair<K, T>>();
//}
//
//template<class K, class T>
//inline index ang::collections::imap<K, T>::index_of(ang::collections::iterator<ang::collections::pair<K, T>>)const
//{
//	return -1;
//}
//
//template<class K, class T>
//inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::imap<K, T>::find(ang::collections::pair<K, T> const &, bool)const
//{
//	return ang::collections::iterator<ang::collections::pair<K, T>>();
//}
//
//template<class K, class T>
//inline ang::collections::iterator<ang::collections::pair<K, T>> ang::collections::imap<K, T>::find(ang::collections::pair<K, T> const &, ang::collections::iterator<ang::collections::pair<K, T>>, bool)const
//{
//	return ang::collections::iterator<ang::collections::pair<K, T>>();
//}


template<class K, class T>
inline bool ang::collections::pair<K, T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::pair<K, T>::class_name())
		return true;
	return false;
}

template<class K, class T>
inline ang::type_name_t ang::collections::pair<K, T>::class_name()
{
	static ang::string name = ang::string("ang::collections::pair<") + type_name<K>() + "," + type_name<T>() + ">";
	return name->cstr();
}


/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
ang::collections::iterator<T>::iterator(ang::collections::iterator<T>&& it)
	: _parent(it._parent)
	, _current(it._current)
	, _offset(it._offset)
	, _unused(0)
{
	it._parent = null;
	it._current = null;
	it._offset = 0;
}

template<class T>
ang::collections::iterator<T>::iterator(const ang::collections::iterator<T>& it)
	: _parent(it._parent)
	, _current(it._current)
	, _offset(it._offset)
	, _unused(0)
{
}

template<class T>
ang::collections::iterator<T>::iterator(uint off)
	: _parent(ang::null)
	, _current(ang::null)
	, _offset(off)
	, _unused(0)
{
}

template<class T>
ang::collections::iterator<T>::iterator(ang::collections::ienum<T>* par, ang::collections::position_t cur, uint off)
	: _parent(par)
	, _current(cur)
	, _offset(off)
	, _unused(0)
{
}

template<class T>
ang::collections::iterator<T>::~iterator()
{
}

template<class T>
inline bool ang::collections::iterator<T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::iterator<T>::class_name())
		return true;
	return false;
}

template<class T>
ang::type_name_t ang::collections::iterator<T>::class_name()
{
	static ang::string className = ang::string("ang::collections::iterator<"_s) + type_name<T>() + ">"_s;
	return className->cstr();
}

template<> inline ang::type_name_t ang::collections::iterator<wchar>::class_name() {
	return "ang::collections::iterator<wchar>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<char>::class_name() {
	return "ang::collections::iterator<char>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<short>::class_name() {
	return "ang::collections::iterator<short>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<ushort>::class_name() {
	return "ang::collections::iterator<ushort>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<int>::class_name() {
	return "ang::collections::iterator<int>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<uint>::class_name() {
	return "ang::collections::iterator<uint>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<long>::class_name() {
	return "ang::collections::iterator<long>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<ulong>::class_name() {
	return "ang::collections::iterator<ulong>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<long64>::class_name() {
	return "ang::collections::iterator<long>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<ulong64>::class_name() {
	return "ang::collections::iterator<ulong>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<float>::class_name() {
	return "ang::collections::iterator<float>"_s;
}

template<> inline ang::type_name_t ang::collections::iterator<double>::class_name()
{
	return "ang::collections::iterator<double>"_s;
}


template<class T>
inline ang::collections::ienum<T>* ang::collections::iterator<T>::parent()const
{
	return _parent;
}

template<class T>
inline void ang::collections::iterator<T>::parent(ang::collections::ienum<T>* p)
{
	_parent = p;
}

template<class T>
inline ang::collections::position_t ang::collections::iterator<T>::current()const
{
	return _current;
}

template<class T>
inline void ang::collections::iterator<T>::current(ang::collections::position_t c)
{
	_current = c;
}

template<class T>
inline uint ang::collections::iterator<T>::offset()const
{
	return _offset;
}

template<class T>
inline void ang::collections::iterator<T>::offset(uint c)
{
	_offset = c;
}

template<class T>
inline bool ang::collections::iterator<T>::is_valid()const
{
	return bool_t(_current != null && _parent != null);
}



template<class T>
inline void ang::collections::iterator<T>::clean()
{
	_current = null;
}

template<class T>
inline const T* ang::collections::iterator<T>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return &_parent->at(*this);
}

template<class T>
inline T* ang::collections::iterator<T>::operator -> ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return &(_parent->at(*this));
}

template<class T>
inline const T& ang::collections::iterator<T>::operator * ()const {
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return _parent->at(*this);
}

template<class T>
inline T& ang::collections::iterator<T>::operator * () {
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return _parent->at(*this);
}

template<class T>
inline ang::collections::iterator<T>&
ang::collections::iterator<T>::operator = (ang::collections::iterator<T>&& it)
{
	_parent = it._parent;
	_current = it._current;
	_offset = it._offset;
	it._parent = null;
	it._current = null;
	it._offset = 0;
	return *this;
}

template<class T>
inline ang::collections::iterator<T>&
ang::collections::iterator<T>::operator = (const ang::collections::iterator<T>& it)
{
	_parent = it._parent;
	_current = it._current;
	_offset = it._offset;
	return *this;
}

template<class T>
inline ang::collections::iterator<T>::operator T*()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return &_parent->at(*this);
}


template<class T>
inline ang::collections::iterator<T>& ang::collections::iterator<T>::operator ++()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	_parent->next(*this);
	return*this;
}

template<class T>
inline ang::collections::iterator<T>& ang::collections::iterator<T>::operator --()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	_parent->prev(*this);
	return*this;
}

template<class T>
inline ang::collections::iterator<T> ang::collections::iterator<T>::operator ++(int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	iterator<T> saveState(*this);
	_parent->next(*this);
	return saveState;
}

template<class T>
inline ang::collections::iterator<T> ang::collections::iterator<T>::operator --(int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	iterator<T> saveState(*this);
	_parent->prev(*this);
	return saveState;
}

template<class T>
inline ang::collections::iterator<T>& ang::collections::iterator<T>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	_parent->next(*this, val);
	return*this;
}

template<class T>
inline ang::collections::iterator<T>& ang::collections::iterator<T>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	_parent->prev(*this, val);
	return*this;
}

template<class T>
inline ang::collections::iterator<T> ang::collections::iterator<T>::operator + (int value)const
{
	ang::collections::iterator<T> it = *this;
	return (it += value);
}

template<class T>
inline ang::collections::iterator<T> ang::collections::iterator<T>::operator - (int value)const
{
	ang::collections::iterator<T> it = *this;
	return (it -= value);
}



/////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
ang::collections::iterator<ang::object_wrapper<T>>::iterator(ang::collections::iterator<ang::object_wrapper<T>>&& it)
	: _parent(it._parent)
	, _current(it._current)
	, _offset(it._offset)
	, _unused(0)
{
	it._parent = null;
	it._current = null;
	it._offset = 0;
}

template<class T>
ang::collections::iterator<ang::object_wrapper<T>>::iterator(const ang::collections::iterator<ang::object_wrapper<T>>& it)
	: _parent(it._parent)
	, _current(it._current)
	, _offset(it._offset)
	, _unused(0)
{
}

template<class T>
ang::collections::iterator<ang::object_wrapper<T>>::iterator(ang::collections::ienum<ang::object_wrapper<T>>* par, ang::collections::position_t cur, uint off)
	: _parent(par)
	, _current(cur)
	, _offset(off)
	, _unused(0)
{
}

template<class T>
ang::collections::iterator<ang::object_wrapper<T>>::~iterator()
{
}

template<class T>
inline bool ang::collections::iterator<ang::object_wrapper<T>>::is_child_of(ang::type_name_t name)
{
	if (name == ang::collections::iterator<ang::object_wrapper<T>>::class_name())
		return true;
	return false;
}

template<class T>
ang::type_name_t ang::collections::iterator<ang::object_wrapper<T>>::class_name()
{
	static ang::string className = ang::string("ang::collections::iterator<") + type_name<T>() + ">";
	return className->cstr();
}


template<class T>
inline ang::collections::ienum<ang::object_wrapper<T>>* ang::collections::iterator<ang::object_wrapper<T>>::parent()const
{
	return _parent;
}

template<class T>
inline void ang::collections::iterator<ang::object_wrapper<T>>::parent(ang::collections::ienum<ang::object_wrapper<T>>* p)
{
	_parent = p;
}

template<class T>
inline ang::collections::position_t ang::collections::iterator<ang::object_wrapper<T>>::current()const
{
	return _current;
}

template<class T>
inline void ang::collections::iterator<ang::object_wrapper<T>>::current(ang::collections::position_t c)
{
	_current = c;
}

template<class T>
inline uint ang::collections::iterator<ang::object_wrapper<T>>::offset()const
{
	return _offset;
}

template<class T>
inline void ang::collections::iterator<ang::object_wrapper<T>>::offset(uint c)
{
	_offset = c;
}

template<class T>
inline bool ang::collections::iterator<ang::object_wrapper<T>>::is_valid()const
{
	return bool_t(_current != null && _parent != null);
}


template<class T>
inline void ang::collections::iterator<ang::object_wrapper<T>>::clean()
{
	_current = null;
}

template<class T>
inline const T* ang::collections::iterator<ang::object_wrapper<T>>::operator -> ()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return _parent->at(*this).get();
}

template<class T>
inline T* ang::collections::iterator<ang::object_wrapper<T>>::operator -> ()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return _parent->at(*this).get();
}

template<class T>
inline ang::object_wrapper<T> ang::collections::iterator<ang::object_wrapper<T>>::operator * ()const {
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return _parent->at(*this);
}

template<class T>
inline ang::object_wrapper<T>& ang::collections::iterator<ang::object_wrapper<T>>::operator * () {
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return _parent->at(*this);
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>>&
ang::collections::iterator<ang::object_wrapper<T>>::operator = (ang::collections::iterator<ang::object_wrapper<T>>&& it)
{
	_parent = it._parent;
	_current = it._current;
	_offset = it._offset;
	it._parent = null;
	it._current = null;
	it._offset = 0;
	return *this;
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>>&
ang::collections::iterator<ang::object_wrapper<T>>::operator = (const ang::collections::iterator<ang::object_wrapper<T>>& it)
{
	_parent = it._parent;
	_current = it._current;
	_offset = it._offset;
	return *this;
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>>::operator ang::object_wrapper<T>*()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return &_parent->at(*this);
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>>::operator ang::object_wrapper_ptr<T>()const
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	return &_parent->at(*this);
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>>& ang::collections::iterator<ang::object_wrapper<T>>::operator ++()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	_parent->next(*this);
	return*this;
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>>& ang::collections::iterator<ang::object_wrapper<T>>::operator --()
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	_parent->prev(*this);
	return*this;
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::iterator<ang::object_wrapper<T>>::operator ++(int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	iterator<ang::object_wrapper<T>> saveState(*this);
	_parent->next(*this);
	return saveState;
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::iterator<ang::object_wrapper<T>>::operator --(int)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	iterator<ang::object_wrapper<T>> saveState(*this);
	_parent->prev(*this);
	return saveState;
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>>& ang::collections::iterator<ang::object_wrapper<T>>::operator += (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	_parent->next(*this, val);
	return*this;
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>>& ang::collections::iterator<ang::object_wrapper<T>>::operator -= (int val)
{
#ifdef DEBUG_SAFE_CODE
	if (_parent == null)
		throw(exception_t(except_code::invalid_memory));
#endif
	_parent->prev(*this, val);
	return*this;
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::iterator<ang::object_wrapper<T>>::operator + (int value)const
{
	ang::collections::iterator<ang::object_wrapper<T>> it = *this;
	return (it += value);
}

template<class T>
inline ang::collections::iterator<ang::object_wrapper<T>> ang::collections::iterator<ang::object_wrapper<T>>::operator - (int value)const
{
	ang::collections::iterator<ang::object_wrapper<T>> it = *this;
	return (it -= value);
}



#endif//__ANG_COLLECTIONS_HPP__