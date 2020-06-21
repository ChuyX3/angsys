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
#else

ang::object_wrapper<MY_TYPE>::object_wrapper() : m_ptr(null) {

}

ang::object_wrapper<MY_TYPE>::object_wrapper(MY_TYPE* ptr) : m_ptr(null) {
	set(ptr);
}

ang::object_wrapper<MY_TYPE>::object_wrapper(object_wrapper && other) : m_ptr(null) {
	MY_TYPE * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

ang::object_wrapper<MY_TYPE>::object_wrapper(object_wrapper const& other) : m_ptr(null) {
	set(other.m_ptr);
}


ang::object_wrapper<MY_TYPE>::object_wrapper(std::nullptr_t const&) : m_ptr(null) {
}


ang::object_wrapper<MY_TYPE>::~object_wrapper() { reset(); }


void ang::object_wrapper<MY_TYPE>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}


void ang::object_wrapper<MY_TYPE>::reset_unsafe()
{
	m_ptr = null;
}


bool ang::object_wrapper<MY_TYPE>::is_empty()const
{
	return m_ptr == null;
}


MY_TYPE* ang::object_wrapper<MY_TYPE>::get(void)const
{
	return m_ptr;
}


void ang::object_wrapper<MY_TYPE>::set(MY_TYPE* ptr)
{
	MY_TYPE * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if(m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}


ang::object_wrapper<MY_TYPE>& ang::object_wrapper<MY_TYPE>::operator = (MY_TYPE* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<MY_TYPE>& ang::object_wrapper<MY_TYPE>::operator = (ang::nullptr_t const&)
{
	reset();
	return*this;
}

ang::object_wrapper<MY_TYPE>& ang::object_wrapper<MY_TYPE>::operator = (ang::object_wrapper<MY_TYPE> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}


ang::object_wrapper<MY_TYPE>& ang::object_wrapper<MY_TYPE>::operator = (ang::object_wrapper<MY_TYPE> const& other)
{
	set(other.m_ptr);
	return*this;
}


MY_TYPE ** ang::object_wrapper<MY_TYPE>::addres_of(void)
{
	return &m_ptr;
}


ang::object_wrapper_ptr<MY_TYPE> ang::object_wrapper<MY_TYPE>::operator& (void)
{
	return this;
}

MY_TYPE* ang::object_wrapper<MY_TYPE>::operator -> (void) const
{
	return get();
}

ang::object_wrapper<MY_TYPE>::operator MY_TYPE* (void)
{
	return get();
}

ang::object_wrapper<MY_TYPE>::operator MY_TYPE const* (void)const
{
	return get();
}

/////////////////////////////////////////////////////////////////////////

#endif//__ANGSYS_H__