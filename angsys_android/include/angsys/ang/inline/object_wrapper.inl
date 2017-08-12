/*********************************************************************************************************************/
/*   File Name: ang/inline/object_wrapper.hpp                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __OBJECT_WRAPPER_HPP__
#define __OBJECT_WRAPPER_HPP__

template<typename T>
inline ang::object_wrapper<T>::object_wrapper() : _ptr(null) {

}
template<typename T>
inline ang::object_wrapper<T>::object_wrapper(T* ptr) : _ptr(null) {
	set(ptr);
}
template<typename T>
inline ang::object_wrapper<T>::object_wrapper(object_wrapper && other) : _ptr(null) {
	T * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename T>
inline ang::object_wrapper<T>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename T>
inline ang::object_wrapper<T>::object_wrapper(std::nullptr_t const&) : _ptr(null) {
}

template<typename T>
inline ang::object_wrapper<T>::~object_wrapper() { clean(); }

template<typename T>
inline void ang::object_wrapper<T>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline void ang::object_wrapper<T>::clean_unsafe()
{
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<T>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline T* ang::object_wrapper<T>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<T>::set(T* ptr)
{
	T * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if(_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::object_wrapper<T>& ang::object_wrapper<T>::operator = (T* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<T>& ang::object_wrapper<T>::operator = (ang::object_wrapper<T> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T>
inline ang::object_wrapper<T>& ang::object_wrapper<T>::operator = (ang::object_wrapper<T> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline T ** ang::object_wrapper<T>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper_ptr<T> ang::object_wrapper<T>::operator& (void)
{
	return this;
}


template<typename T>
inline T * ang::object_wrapper<T>::operator -> (void)
{
	return get();
}

template<typename T>
inline T const* ang::object_wrapper<T>::operator -> (void)const
{
	return get();
}

template<typename T>
inline ang::object_wrapper<T>::operator T * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<T>::operator T const* (void)const
{
	return get();
}

#endif//__OBJECT_WRAPPER_HPP__