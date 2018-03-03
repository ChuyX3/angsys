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
#else


ang::intf_wrapper<MY_TYPE>::intf_wrapper() : _ptr(null) {

}


ang::intf_wrapper<MY_TYPE>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}


ang::intf_wrapper<MY_TYPE>::intf_wrapper(MY_TYPE* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<MY_TYPE>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	MY_TYPE * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<MY_TYPE>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}


ang::intf_wrapper<MY_TYPE>::~intf_wrapper() {
	clean();
}


void ang::intf_wrapper<MY_TYPE>::clean()
{
	iobject * _obj = dyn_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}


bool ang::intf_wrapper<MY_TYPE>::is_empty()const
{
	return _ptr == null;
}


MY_TYPE* ang::intf_wrapper<MY_TYPE>::get(void)const
{
	return _ptr;
}


void ang::intf_wrapper<MY_TYPE>::set(MY_TYPE* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = dyn_cast<iobject>(_ptr);
	iobject * _new = dyn_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}


ang::intf_wrapper<MY_TYPE>& ang::intf_wrapper<MY_TYPE>::operator = (MY_TYPE* ptr)
{
	set(ptr);
	return*this;
}


ang::intf_wrapper<MY_TYPE>& ang::intf_wrapper<MY_TYPE>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}


ang::intf_wrapper<MY_TYPE>& ang::intf_wrapper<MY_TYPE>::operator = (ang::intf_wrapper<MY_TYPE> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::intf_wrapper<MY_TYPE>& ang::intf_wrapper<MY_TYPE>::operator = (ang::intf_wrapper<MY_TYPE> const& other)
{
	set(other._ptr);
	return*this;
}


MY_TYPE ** ang::intf_wrapper<MY_TYPE>::addres_of(void)
{
	return &_ptr;
}


ang::intf_wrapper_ptr<MY_TYPE> ang::intf_wrapper<MY_TYPE>::operator & (void)
{
	return this;
}

MY_TYPE* ang::intf_wrapper<MY_TYPE>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<MY_TYPE>::operator ang::intfptr()const
{
	return (interface*)get();
}


ang::intf_wrapper<MY_TYPE>::operator MY_TYPE* (void)const
{
	return get();
}

////////////////////////////////////////////////////////////////////////////////////

ang::intf_wrapper<const MY_TYPE>::intf_wrapper() : _ptr(null) {

}


ang::intf_wrapper<const MY_TYPE>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}


ang::intf_wrapper<const MY_TYPE>::intf_wrapper(const MY_TYPE* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<const MY_TYPE>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	const MY_TYPE * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<const MY_TYPE>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}


ang::intf_wrapper<const MY_TYPE>::~intf_wrapper() {
	clean();
}


void ang::intf_wrapper<const MY_TYPE>::clean()
{
	iobject const * _obj = dyn_cast<iobject>(_ptr);
	if (_obj)const_cast<iobject*>(_obj)->release();
	_ptr = null;
}


bool ang::intf_wrapper<const MY_TYPE>::is_empty()const
{
	return _ptr == null;
}


MY_TYPE const * ang::intf_wrapper<const MY_TYPE>::get(void)const
{
	return _ptr;
}


void ang::intf_wrapper<const MY_TYPE>::set(MY_TYPE const * ptr)
{
	if (ptr == _ptr) return;
	iobject const * _old = dyn_cast<iobject>(_ptr);
	iobject const * _new = dyn_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)const_cast<iobject*>(_new)->add_ref();
	if (_old)const_cast<iobject*>(_old)->release();
}


ang::intf_wrapper<const MY_TYPE>& ang::intf_wrapper<const MY_TYPE>::operator = (MY_TYPE const * ptr)
{
	set(ptr);
	return*this;
}


ang::intf_wrapper<const MY_TYPE>& ang::intf_wrapper<const MY_TYPE>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}


ang::intf_wrapper<const MY_TYPE>& ang::intf_wrapper<const MY_TYPE>::operator = (ang::intf_wrapper<const MY_TYPE> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::intf_wrapper<const MY_TYPE>& ang::intf_wrapper<const MY_TYPE>::operator = (ang::intf_wrapper<const MY_TYPE> const& other)
{
	set(other._ptr);
	return*this;
}


MY_TYPE const ** ang::intf_wrapper<const MY_TYPE>::addres_of(void)
{
	return &_ptr;
}


ang::intf_wrapper_ptr<const MY_TYPE> ang::intf_wrapper<const MY_TYPE>::operator & (void)
{
	return this;
}

MY_TYPE const * ang::intf_wrapper<const MY_TYPE>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<const MY_TYPE>::operator ang::cintfptr()const
{
	return (interface const *)get();
}


ang::intf_wrapper<const MY_TYPE>::operator MY_TYPE const * (void)const
{
	return get();
}

////////////////////////////////////////////////////////////////////////////////////

#endif//__intf_WRAPPER_HPP__