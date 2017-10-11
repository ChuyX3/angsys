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

/////////////////////////////////////////////////////////////////////////

template<typename T>
ang::value_wrapper<T>::value_wrapper()
	: object(), value<T>() {
}

template<typename T>
ang::value_wrapper<T>::value_wrapper(T const& val)
	: object(), value<T>(val) {
}

template<typename T>
ang::value_wrapper<T>::value_wrapper(ang::value<T> const& other)
	: object(), value<T>(other) {
}

template<typename T>
ang::value_wrapper<T>::value_wrapper(ang::value_wrapper<T> const& other)
	: object(), value<T>(other) {
}

template<typename T>
ang::value_wrapper<T>::~value_wrapper() {
}


template<typename T>
ang::type_name_t ang::value_wrapper<T>::class_name() {
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::value_wrapper<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<typename T>
ang::type_name_t ang::value_wrapper<T>::object_name()const {
	return class_name();
}

template<class T>
inline bool ang::value_wrapper<T>::is_kind_of(ang::type_name_t name)const
{
	if (name == ang::value_wrapper<T>::class_name()
		|| object::is_kind_of(name)
		|| is_type_of<value<T>>(name))
		return true;
	return false;
}

template<class T>
inline bool ang::value_wrapper<T>::is_child_of(ang::type_name_t name)
{
	if (name == ang::value_wrapper<T>::class_name()
		|| object::is_child_of(name)
		|| is_type_of<value<T>>(name))
		return true;
	return false;
}

template<class T>
inline bool ang::value_wrapper<T>::query_object(ang::type_name_t className, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == ang::value_wrapper<T>::class_name())
	{
		*out = static_cast<ang::value_wrapper<T>*>(this);
		return true;
	}
	else if (object::query_object(className, out))
	{
		return true;
	}
	else if (className == type_of<T>())
	{
		*out = &value<T>::get();
		return true;
	}
	return false;
}

template<typename T>
ang::string ang::value_wrapper<T>::to_string()const {
	return type_of<T>();
}

//////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::object_wrapper() : _ptr(null) {

}
template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::object_wrapper(ang::value_wrapper<T>* ptr) : _ptr(null) {
	set(ptr);
}
template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::object_wrapper(object_wrapper && other) : _ptr(null) {
	ang::value_wrapper<T> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::object_wrapper(std::nullptr_t const&) : _ptr(null) {
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::~object_wrapper() { clean(); }

template<typename T>
inline void ang::object_wrapper<ang::value_wrapper<T>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename T>
inline void ang::object_wrapper<ang::value_wrapper<T>>::clean_unsafe()
{
	_ptr = null;
}

template<typename T>
inline bool ang::object_wrapper<ang::value_wrapper<T>>::is_empty()const
{
	return _ptr == null;
}

template<typename T>
inline ang::value_wrapper<T>* ang::object_wrapper<ang::value_wrapper<T>>::get(void)const
{
	return _ptr;
}

template<typename T>
inline void ang::object_wrapper<ang::value_wrapper<T>>::set(ang::value_wrapper<T>* ptr)
{
	ang::value_wrapper<T> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>& ang::object_wrapper<ang::value_wrapper<T>>::operator = (ang::value_wrapper<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>& ang::object_wrapper<ang::value_wrapper<T>>::operator = (ang::object_wrapper<ang::value_wrapper<T>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>& ang::object_wrapper<ang::value_wrapper<T>>::operator = (ang::object_wrapper<ang::value_wrapper<T>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename T>
inline ang::value_wrapper<T> ** ang::object_wrapper<ang::value_wrapper<T>>::addres_of(void)
{
	return &_ptr;
}

template<typename T>
inline ang::object_wrapper_ptr<ang::value_wrapper<T>> ang::object_wrapper<ang::value_wrapper<T>>::operator& (void)
{
	return this;
}


template<typename T>
inline T * ang::object_wrapper<ang::value_wrapper<T>>::operator -> (void)
{
	return &get()->get();
}

template<typename T>
inline T const* ang::object_wrapper<ang::value_wrapper<T>>::operator -> (void)const
{
	return &get()->get();
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::operator T * (void)
{
	return &get()->get();
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::operator T const* (void)const
{
	return &get()->get();
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::operator ang::value_wrapper<T> * (void)
{
	return get();
}

template<typename T>
inline ang::object_wrapper<ang::value_wrapper<T>>::operator ang::value_wrapper<T> const* (void)const
{
	return get();
}

/////////////////////////////////////////////////////////////////////////

#endif//__OBJECT_WRAPPER_HPP__