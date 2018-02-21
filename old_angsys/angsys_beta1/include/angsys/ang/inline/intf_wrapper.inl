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

template<typename intf_t>
inline ang::intf_wrapper<intf_t>::intf_wrapper() : _ptr(null) {

}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>::intf_wrapper(intf_t* ptr) : _ptr(null) {
	set(ptr);
}
template<typename intf_t>
inline ang::intf_wrapper<intf_t>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	intf_t * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename intf_t>
inline ang::intf_wrapper<intf_t>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>::~intf_wrapper() {
	clean();
}

template<typename intf_t>
inline void ang::intf_wrapper<intf_t>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

template<typename intf_t>
inline bool ang::intf_wrapper<intf_t>::is_empty()const
{
	return _ptr == null;
}

template<typename intf_t>
inline intf_t* ang::intf_wrapper<intf_t>::get(void)const
{
	return _ptr;
}

template<typename intf_t>
inline void ang::intf_wrapper<intf_t>::set(intf_t* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>& ang::intf_wrapper<intf_t>::operator = (intf_t* ptr)
{
	set(ptr);
	return*this;
}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>& ang::intf_wrapper<intf_t>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>& ang::intf_wrapper<intf_t>::operator = (ang::intf_wrapper<intf_t> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>& ang::intf_wrapper<intf_t>::operator = (ang::intf_wrapper<intf_t> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename intf_t>
inline intf_t ** ang::intf_wrapper<intf_t>::addres_of(void)
{
	return &_ptr;
}

template<typename intf_t>
inline ang::intf_wrapper_ptr<intf_t> ang::intf_wrapper<intf_t>::operator & (void)
{
	return this;
}

template<typename intf_t>
inline intf_t * ang::intf_wrapper<intf_t>::operator -> (void)
{
	return get();
}

template<typename intf_t>
inline intf_t const* ang::intf_wrapper<intf_t>::operator -> (void)const
{
	return get();
}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>::operator intf_t * (void)
{
	return get();
}

template<typename intf_t>
inline ang::intf_wrapper<intf_t>::operator intf_t const* (void)const
{
	return get();
}

#endif//__intf_WRAPPER_HPP__