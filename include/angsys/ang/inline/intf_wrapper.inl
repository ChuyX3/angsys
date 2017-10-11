/*********************************************************************************************************************/
/*   File Name: ang/inline/intf_wrapper.hpp                                                                          */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __INTF_WRAPPER_HPP__
#define __INTF_WRAPPER_HPP__

template<typename T>
inline ang::intf_wrapper<T>::intf_wrapper() : _ptr(null) {

}

template<typename T>
inline ang::intf_wrapper<T>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}

template<typename T>
inline ang::intf_wrapper<T>::intf_wrapper(T* ptr) : _ptr(null) {
	set(ptr);
}
template<typename T>
inline ang::intf_wrapper<T>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	T * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename T>
inline ang::intf_wrapper<T>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename T>
inline ang::intf_wrapper<T>::~intf_wrapper() {
	clean();
}

template<typename T>
inline void ang::intf_wrapper<T>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

template<typename T>
inline bool ang::intf_wrapper<T>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline T* ang::intf_wrapper<T>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::intf_wrapper<T>::set(T* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T>
inline ang::intf_wrapper<T>& ang::intf_wrapper<T>::operator = (T* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::intf_wrapper<T>& ang::intf_wrapper<T>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::intf_wrapper<T>& ang::intf_wrapper<T>::operator = (ang::intf_wrapper<T> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T>
inline ang::intf_wrapper<T>& ang::intf_wrapper<T>::operator = (ang::intf_wrapper<T> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline T ** ang::intf_wrapper<T>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::intf_wrapper_ptr<T> ang::intf_wrapper<T>::operator & (void)
{
	return this;
}

template<typename T>
inline T * ang::intf_wrapper<T>::operator -> (void)
{
	return get();
}

template<typename T>
inline T const* ang::intf_wrapper<T>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::intf_wrapper<T>::operator T * (void)
{
	return get();
}

template<typename T>
inline ang::intf_wrapper<T>::operator T const* (void)const
{
	return get();
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper() : _ptr(null) {

}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper(ang::collections::ienum<T>* ptr) : _ptr(null) {
	set(ptr);
}
template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ang::collections::ienum<T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>::~intf_wrapper() {
	clean();
}

template<typename T>
inline void ang::intf_wrapper<ang::collections::ienum<T>>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

template<typename T>
inline bool ang::intf_wrapper<ang::collections::ienum<T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::ienum<T>* ang::intf_wrapper<ang::collections::ienum<T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::intf_wrapper<ang::collections::ienum<T>>::set(ang::collections::ienum<T>* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>& ang::intf_wrapper<ang::collections::ienum<T>>::operator = (ang::collections::ienum<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>& ang::intf_wrapper<ang::collections::ienum<T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>& ang::intf_wrapper<ang::collections::ienum<T>>::operator = (ang::intf_wrapper<ang::collections::ienum<T>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>& ang::intf_wrapper<ang::collections::ienum<T>>::operator = (ang::intf_wrapper<ang::collections::ienum<T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::collections::ienum<T> ** ang::intf_wrapper<ang::collections::ienum<T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::intf_wrapper_ptr<ang::collections::ienum<T>> ang::intf_wrapper<ang::collections::ienum<T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::ienum<T> * ang::intf_wrapper<ang::collections::ienum<T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::ienum<T> const* ang::intf_wrapper<ang::collections::ienum<T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>::operator ang::collections::ienum<T> * (void)
{
	return get();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ienum<T>>::operator ang::collections::ienum<T> const* (void)const
{
	return get();
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper() : _ptr(null) {

}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper(ang::collections::iarray<T>* ptr) : _ptr(null) {
	set(ptr);
}
template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	T * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>::~intf_wrapper() {
	clean();
}

template<typename T>
inline void ang::intf_wrapper<ang::collections::iarray<T>>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

template<typename T>
inline bool ang::intf_wrapper<ang::collections::iarray<T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::iarray<T>* ang::intf_wrapper<ang::collections::iarray<T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::intf_wrapper<ang::collections::iarray<T>>::set(ang::collections::iarray<T>* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>& ang::intf_wrapper<ang::collections::iarray<T>>::operator = (ang::collections::iarray<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>& ang::intf_wrapper<ang::collections::iarray<T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>& ang::intf_wrapper<ang::collections::iarray<T>>::operator = (ang::intf_wrapper<ang::collections::iarray<T>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>& ang::intf_wrapper<ang::collections::iarray<T>>::operator = (ang::intf_wrapper<ang::collections::iarray<T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::collections::iarray<T> ** ang::intf_wrapper<ang::collections::iarray<T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::intf_wrapper_ptr<ang::collections::iarray<T>> ang::intf_wrapper<ang::collections::iarray<T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::iarray<T> * ang::intf_wrapper<ang::collections::iarray<T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::iarray<T> const* ang::intf_wrapper<ang::collections::iarray<T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>::operator ang::collections::iarray<T> * (void)
{
	return get();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::iarray<T>>::operator ang::collections::iarray<T> const* (void)const
{
	return get();
}



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper() : _ptr(null) {

}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper(ang::collections::ilist<T>* ptr) : _ptr(null) {
	set(ptr);
}
template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	T * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>::~intf_wrapper() {
	clean();
}

template<typename T>
inline void ang::intf_wrapper<ang::collections::ilist<T>>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

template<typename T>
inline bool ang::intf_wrapper<ang::collections::ilist<T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::collections::ilist<T>* ang::intf_wrapper<ang::collections::ilist<T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::intf_wrapper<ang::collections::ilist<T>>::set(ang::collections::ilist<T>* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>& ang::intf_wrapper<ang::collections::ilist<T>>::operator = (ang::collections::ilist<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>& ang::intf_wrapper<ang::collections::ilist<T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>& ang::intf_wrapper<ang::collections::ilist<T>>::operator = (ang::intf_wrapper<ang::collections::ilist<T>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>& ang::intf_wrapper<ang::collections::ilist<T>>::operator = (ang::intf_wrapper<ang::collections::ilist<T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::collections::ilist<T> ** ang::intf_wrapper<ang::collections::ilist<T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::intf_wrapper_ptr<ang::collections::ilist<T>> ang::intf_wrapper<ang::collections::ilist<T>>::operator & (void)
{
	return this;
}

template<typename T>
inline ang::collections::ilist<T> * ang::intf_wrapper<ang::collections::ilist<T>>::operator -> (void)
{
	return get();
}

template<typename T>
inline ang::collections::ilist<T> const* ang::intf_wrapper<ang::collections::ilist<T>>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>::operator ang::collections::ilist<T> * (void)
{
	return get();
}

template<typename T>
inline ang::intf_wrapper<ang::collections::ilist<T>>::operator ang::collections::ilist<T> const* (void)const
{
	return get();
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>::intf_wrapper() : _ptr(null) {

}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>::intf_wrapper(ang::collections::imap<K, T>* ptr) : _ptr(null) {
	set(ptr);
}
template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	T * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>::~intf_wrapper() {
	clean();
}

template<typename K, typename T>
inline void ang::intf_wrapper<ang::collections::imap<K, T>>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

template<typename K, typename T>
inline bool ang::intf_wrapper<ang::collections::imap<K, T>>::is_empty()const
{
	return _ptr == null;
}

template<typename K, typename T>
inline ang::collections::imap<K, T>* ang::intf_wrapper<ang::collections::imap<K, T>>::get(void)const
{
	return _ptr;
}

template<typename K, typename T>
inline void ang::intf_wrapper<ang::collections::imap<K, T>>::set(ang::collections::imap<K, T>* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>& ang::intf_wrapper<ang::collections::imap<K, T>>::operator = (ang::collections::imap<K, T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>& ang::intf_wrapper<ang::collections::imap<K, T>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>& ang::intf_wrapper<ang::collections::imap<K, T>>::operator = (ang::intf_wrapper<ang::collections::imap<K, T>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>& ang::intf_wrapper<ang::collections::imap<K, T>>::operator = (ang::intf_wrapper<ang::collections::imap<K, T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename K, typename T>
inline ang::collections::imap<K, T> ** ang::intf_wrapper<ang::collections::imap<K, T>>::addres_of(void)
{
	return &_ptr;
}

template<typename K, typename T>
inline ang::intf_wrapper_ptr<ang::collections::imap<K, T>> ang::intf_wrapper<ang::collections::imap<K, T>>::operator & (void)
{
	return this;
}

template<typename K, typename T>
inline ang::collections::imap<K, T> * ang::intf_wrapper<ang::collections::imap<K, T>>::operator -> (void)
{
	return get();
}

template<typename K, typename T>
inline ang::collections::imap<K, T> const* ang::intf_wrapper<ang::collections::imap<K, T>>::operator -> (void)const
{
	return get();
}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>::operator ang::collections::imap<K, T> * (void)
{
	return get();
}

template<typename K, typename T>
inline ang::intf_wrapper<ang::collections::imap<K, T>>::operator ang::collections::imap<K, T> const* (void)const
{
	return get();
}

#endif//__intf_WRAPPER_HPP__