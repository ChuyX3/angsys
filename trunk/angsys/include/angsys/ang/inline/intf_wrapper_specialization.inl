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

ang::intf_wrapper<MY_TYPE>::intf_wrapper() : m_ptr(null) {

}


ang::intf_wrapper<MY_TYPE>::intf_wrapper(ang::nullptr_t const&) : m_ptr(null) {

}


ang::intf_wrapper<MY_TYPE>::intf_wrapper(MY_TYPE* ptr) : m_ptr(null) {
	set(ptr);
}

ang::intf_wrapper<MY_TYPE>::intf_wrapper(intf_wrapper && other) : m_ptr(null) {
	MY_TYPE * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

ang::intf_wrapper<MY_TYPE>::intf_wrapper(intf_wrapper const& other) : m_ptr(null) {
	set(other.m_ptr);
}


ang::intf_wrapper<MY_TYPE>::~intf_wrapper() {
	reset();
}


void ang::intf_wrapper<MY_TYPE>::reset()
{
	iobject * _obj = interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

void ang::intf_wrapper<MY_TYPE>::reset_unsafe()
{
	m_ptr = null;
}


bool ang::intf_wrapper<MY_TYPE>::is_empty()const
{
	return m_ptr == null;
}


MY_TYPE* ang::intf_wrapper<MY_TYPE>::get(void)const
{
	return m_ptr;
}


void ang::intf_wrapper<MY_TYPE>::set(MY_TYPE* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = interface_cast<iobject>(m_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	m_ptr = ptr;
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
	reset();
	return*this;
}


ang::intf_wrapper<MY_TYPE>& ang::intf_wrapper<MY_TYPE>::operator = (ang::intf_wrapper<MY_TYPE> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}


ang::intf_wrapper<MY_TYPE>& ang::intf_wrapper<MY_TYPE>::operator = (ang::intf_wrapper<MY_TYPE> const& other)
{
	set(other.m_ptr);
	return*this;
}


MY_TYPE ** ang::intf_wrapper<MY_TYPE>::addres_of(void)
{
	return &m_ptr;
}


ang::intf_wrapper_ptr<MY_TYPE> ang::intf_wrapper<MY_TYPE>::operator & (void)
{
	return this;
}

ang::intf_wrapper<MY_TYPE>::operator ang::intfptr()const
{
	return static_interface_cast(get());
}

MY_TYPE* ang::intf_wrapper<MY_TYPE>::operator -> (void)
{
	return get();
}

MY_TYPE const* ang::intf_wrapper<MY_TYPE>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<MY_TYPE>::operator MY_TYPE* (void)
{
	return get();
}

ang::intf_wrapper<MY_TYPE>::operator MY_TYPE const* (void)const
{
	return get();
}

////////////////////////////////////////////////////////////////////////////////////

#endif//__intf_WRAPPER_HPP__