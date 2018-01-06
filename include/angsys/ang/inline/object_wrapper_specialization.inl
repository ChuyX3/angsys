/*********************************************************************************************************************/
/*   File Name: ang/inline/object_wrapper.hpp                                                                        */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_HPP__
#error ...
#else


ang::object_wrapper<MY_TYPE>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<MY_TYPE>::object_wrapper(MY_TYPE* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<MY_TYPE>::object_wrapper(object_wrapper && other) : _ptr(null) {
	MY_TYPE * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<MY_TYPE>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}


ang::object_wrapper<MY_TYPE>::object_wrapper(std::nullptr_t const&) : _ptr(null) {
}


ang::object_wrapper<MY_TYPE>::~object_wrapper() { clean(); }


void ang::object_wrapper<MY_TYPE>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}


void ang::object_wrapper<MY_TYPE>::clean_unsafe()
{
	_ptr = null;
}


bool ang::object_wrapper<MY_TYPE>::is_empty()const
{
	return _ptr == null;
}


MY_TYPE* ang::object_wrapper<MY_TYPE>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<MY_TYPE>::set(MY_TYPE* ptr)
{
	MY_TYPE * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if(_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


ang::object_wrapper<MY_TYPE>& ang::object_wrapper<MY_TYPE>::operator = (MY_TYPE* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<MY_TYPE>& ang::object_wrapper<MY_TYPE>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}

ang::object_wrapper<MY_TYPE>& ang::object_wrapper<MY_TYPE>::operator = (ang::object_wrapper<MY_TYPE> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<MY_TYPE>& ang::object_wrapper<MY_TYPE>::operator = (ang::object_wrapper<MY_TYPE> const& other)
{
	set(other._ptr);
	return*this;
}


MY_TYPE ** ang::object_wrapper<MY_TYPE>::addres_of(void)
{
	return &_ptr;
}


ang::object_wrapper_ptr<MY_TYPE> ang::object_wrapper<MY_TYPE>::operator& (void)
{
	return this;
}



MY_TYPE * ang::object_wrapper<MY_TYPE>::operator -> (void)
{
	return get();
}


MY_TYPE const* ang::object_wrapper<MY_TYPE>::operator -> (void)const
{
	return get();
}


ang::object_wrapper<MY_TYPE>::operator MY_TYPE * (void)
{
	return get();
}


ang::object_wrapper<MY_TYPE>::operator MY_TYPE const* (void)const
{
	return get();
}


/////////////////////////////////////////////////////////////////////////

#endif//__OBJECT_WRAPPER_HPP__