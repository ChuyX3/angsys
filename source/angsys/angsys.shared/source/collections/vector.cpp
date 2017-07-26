/*********************************************************************************************************************/
/*   File Name: vector.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <angsys.h>
#include <ang/collections/vector.h>

#include <ang/core/async.h>

using namespace ang;


ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper()
	: _ptr(null)
{

}

ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(ang::collections::vector_buffer<string>* ptr)
	: object_wrapper<ang::collections::vector_buffer<string>>()
{
	set(ptr);
}


ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(ang::initializer_list_t<string> list)
	: object_wrapper<ang::collections::vector_buffer<string>>()
{
	set(new collections::vector_buffer<string>(ang::move(list)));
}


ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(ang::static_array<string> list)
	: object_wrapper<ang::collections::vector_buffer<string>>()
{
	set(new collections::vector_buffer<string>(list.size(), list.get()));
}

ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(ang::initializer_list_t<cstr_t> list)
	: object_wrapper<ang::collections::vector_buffer<string>>()
{
	set(new collections::vector_buffer<string>((uint)list.size()));
	auto data = get();

	for (auto it = list.begin(); it != list.end(); ++it)
		data->append(*it);
}

ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(ang::initializer_list_t<cwstr_t> list)
	: object_wrapper<ang::collections::vector_buffer<string>>()
{
	set(new collections::vector_buffer<string>((uint)list.size()));
	auto data = get();
	for (auto it = list.begin(); it != list.end(); ++it)
		data->append(*it);
}

ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(const ang::collections::ienum<string>* store)
	: object_wrapper<ang::collections::vector_buffer<string>>()
{
	set(new collections::vector_buffer<string>(store));
}


ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<string>> && other)
	: object_wrapper<collections::vector_buffer<string>>()
{
	collections::vector_buffer<string> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}


ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<string>> const& other)
	: object_wrapper<collections::vector_buffer<string>>()
{
	set(other.get());
}

ang::object_wrapper<ang::collections::vector_buffer<string>>::object_wrapper(ang::nullptr_t const& other)
	: object_wrapper<collections::vector_buffer<string>>() { }

ang::object_wrapper<ang::collections::vector_buffer<string>>::~object_wrapper()
{
	clean();
}


void ang::object_wrapper<ang::collections::vector_buffer<string>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}


void ang::object_wrapper<ang::collections::vector_buffer<string>>::clean_unsafe()
{
	_ptr = null;
}


bool ang::object_wrapper<ang::collections::vector_buffer<string>>::is_empty()const
{
	return _ptr == null;
}


ang::collections::vector_buffer<string>* ang::object_wrapper<ang::collections::vector_buffer<string>>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<ang::collections::vector_buffer<string>>::set(ang::collections::vector_buffer<string>* ptr)
{
	ang::collections::vector_buffer<string> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


ang::collections::vector_buffer<string>** ang::object_wrapper<ang::collections::vector_buffer<string>>::addres_of(void)
{
	return &_ptr;
}


ang::object_wrapper<ang::collections::vector_buffer<string>>& ang::object_wrapper<ang::collections::vector_buffer<string>>::operator = (ang::collections::vector_buffer<string>* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<string>>& ang::object_wrapper<ang::collections::vector_buffer<string>>::operator = (const ang::nullptr_t&)
{
	clean();
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<string>>& ang::object_wrapper<ang::collections::vector_buffer<string>>::operator = (ang::collections::ienum<string> const* items)
{
	if (_ptr == null)
		set(new collections::vector_buffer<string>(items));
	else
		_ptr->copy(items);
	return *this;
}


ang::object_wrapper<ang::collections::vector_buffer<string>>& ang::object_wrapper<ang::collections::vector_buffer<string>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<string>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<string>>& ang::object_wrapper<ang::collections::vector_buffer<string>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<string>> const& other)
{
	set(other._ptr);
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<string>>& ang::object_wrapper<ang::collections::vector_buffer<string>>::operator += (string value)
{
	if (is_empty())
		set(new ang::collections::vector_buffer<string>());
	get()->append(ang::move(value));
	return*this;
}


ang::object_wrapper_ptr<ang::collections::vector_buffer<string>> ang::object_wrapper<ang::collections::vector_buffer<string>>::operator & (void)
{
	return this;
}


ang::collections::vector_buffer<string> * ang::object_wrapper<ang::collections::vector_buffer<string>>::operator -> (void)
{
	return get();
}


ang::collections::vector_buffer<string> const* ang::object_wrapper<ang::collections::vector_buffer<string>>::operator -> (void)const
{
	return get();
}


ang::object_wrapper<ang::collections::vector_buffer<string>>::operator ang::collections::vector_buffer<string> * (void)
{
	return get();
}


ang::object_wrapper<ang::collections::vector_buffer<string>>::operator ang::collections::vector_buffer<string> const* (void)const
{
	return get();
}


string const& ang::object_wrapper<ang::collections::vector_buffer<string>>::operator[](int idx)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


string & ang::object_wrapper<ang::collections::vector_buffer<string>>::operator[](int idx)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper()
	: _ptr(null)
{

}

ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(ang::collections::vector_buffer<wstring>* ptr)
	: object_wrapper<ang::collections::vector_buffer<wstring>>()
{
	set(ptr);
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(ang::initializer_list_t<wstring> list)
	: object_wrapper<ang::collections::vector_buffer<wstring>>()
{
	set(new collections::vector_buffer<wstring>(ang::move(list)));
}

ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(ang::initializer_list_t<cwstr_t> list)
	: object_wrapper<ang::collections::vector_buffer<wstring>>()
{
	set(new collections::vector_buffer<wstring>((uint)list.size()));
	auto data = get();

	for (auto it = list.begin(); it != list.end(); ++it)
		data->append(*it);
}

ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(ang::static_array<wstring> list)
	: object_wrapper<ang::collections::vector_buffer<wstring>>()
{
	set(new collections::vector_buffer<wstring>(list.size(), list.get()));
}

ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(ang::initializer_list_t<cstr_t> list)
	: object_wrapper<ang::collections::vector_buffer<wstring>>()
{
	set(new collections::vector_buffer<wstring>((uint)list.size()));
	auto data = get();
	for (auto it = list.begin(); it != list.end(); ++it)
		data->append(*it);
}

ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(const ang::collections::ienum<wstring>* store)
	: object_wrapper<ang::collections::vector_buffer<wstring>>()
{
	set(new collections::vector_buffer<wstring>(store));
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<wstring>> && other)
	: object_wrapper<collections::vector_buffer<wstring>>()
{
	collections::vector_buffer<wstring> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<wstring>> const& other)
	: object_wrapper<collections::vector_buffer<wstring>>()
{
	set(other.get());
}

ang::object_wrapper<ang::collections::vector_buffer<wstring>>::object_wrapper(ang::nullptr_t const& other)
	: object_wrapper<collections::vector_buffer<wstring>>() { }

ang::object_wrapper<ang::collections::vector_buffer<wstring>>::~object_wrapper()
{
	clean();
}


void ang::object_wrapper<ang::collections::vector_buffer<wstring>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}


void ang::object_wrapper<ang::collections::vector_buffer<wstring>>::clean_unsafe()
{
	_ptr = null;
}


bool ang::object_wrapper<ang::collections::vector_buffer<wstring>>::is_empty()const
{
	return _ptr == null;
}


ang::collections::vector_buffer<wstring>* ang::object_wrapper<ang::collections::vector_buffer<wstring>>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<ang::collections::vector_buffer<wstring>>::set(ang::collections::vector_buffer<wstring>* ptr)
{
	ang::collections::vector_buffer<wstring> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


ang::collections::vector_buffer<wstring>** ang::object_wrapper<ang::collections::vector_buffer<wstring>>::addres_of(void)
{
	return &_ptr;
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>& ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator = (ang::collections::vector_buffer<wstring>* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>& ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator = (const ang::nullptr_t&)
{
	clean();
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>& ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator = (ang::collections::ienum<wstring> const* items)
{
	if (_ptr == null)
		set(new collections::vector_buffer<wstring>(items));
	else
		_ptr->copy(items);
	return *this;
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>& ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<wstring>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>& ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<wstring>> const& other)
{
	set(other._ptr);
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>& ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator += (wstring value)
{
	if (is_empty())
		set(new ang::collections::vector_buffer<wstring>());
	get()->append(ang::move(value));
	return*this;
}


ang::object_wrapper_ptr<ang::collections::vector_buffer<wstring>> ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator & (void)
{
	return this;
}


ang::collections::vector_buffer<wstring> * ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator -> (void)
{
	return get();
}


ang::collections::vector_buffer<wstring> const* ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator -> (void)const
{
	return get();
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator ang::collections::vector_buffer<wstring> * (void)
{
	return get();
}


ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator ang::collections::vector_buffer<wstring> const* (void)const
{
	return get();
}


wstring const& ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator[](int idx)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


wstring & ang::object_wrapper<ang::collections::vector_buffer<wstring>>::operator[](int idx)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<ang::collections::vector_buffer<objptr>>::object_wrapper()
	: _ptr(null)
{

}

ang::object_wrapper<ang::collections::vector_buffer<objptr>>::object_wrapper(ang::collections::vector_buffer<objptr>* ptr)
	: object_wrapper<ang::collections::vector_buffer<objptr>>()
{
	set(ptr);
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>::object_wrapper(ang::initializer_list_t<objptr> list)
	: object_wrapper<ang::collections::vector_buffer<objptr>>()
{
	set(new collections::vector_buffer<objptr>(ang::move(list)));
}

ang::object_wrapper<ang::collections::vector_buffer<objptr>>::object_wrapper(ang::static_array<objptr> list)
	: object_wrapper<ang::collections::vector_buffer<objptr>>()
{
	set(new collections::vector_buffer<objptr>(list.size(), list.get()));
}

ang::object_wrapper<ang::collections::vector_buffer<objptr>>::object_wrapper(const ang::collections::ienum<objptr>* store)
	: object_wrapper<ang::collections::vector_buffer<objptr>>()
{
	set(new collections::vector_buffer<objptr>(store));
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<objptr>> && other)
	: object_wrapper<collections::vector_buffer<objptr>>()
{
	collections::vector_buffer<objptr> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>::object_wrapper(ang::object_wrapper<ang::collections::vector_buffer<objptr>> const& other)
	: object_wrapper<collections::vector_buffer<objptr>>()
{
	set(other.get());
}

ang::object_wrapper<ang::collections::vector_buffer<objptr>>::object_wrapper(ang::nullptr_t const& other)
	: object_wrapper<collections::vector_buffer<objptr>>() { }

ang::object_wrapper<ang::collections::vector_buffer<objptr>>::~object_wrapper()
{
	clean();
}


void ang::object_wrapper<ang::collections::vector_buffer<objptr>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}


void ang::object_wrapper<ang::collections::vector_buffer<objptr>>::clean_unsafe()
{
	_ptr = null;
}


bool ang::object_wrapper<ang::collections::vector_buffer<objptr>>::is_empty()const
{
	return _ptr == null;
}


ang::collections::vector_buffer<objptr>* ang::object_wrapper<ang::collections::vector_buffer<objptr>>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<ang::collections::vector_buffer<objptr>>::set(ang::collections::vector_buffer<objptr>* ptr)
{
	ang::collections::vector_buffer<objptr> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}


ang::collections::vector_buffer<objptr>** ang::object_wrapper<ang::collections::vector_buffer<objptr>>::addres_of(void)
{
	return &_ptr;
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>& ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator = (ang::collections::vector_buffer<objptr>* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>& ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator = (const ang::nullptr_t&)
{
	clean();
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>& ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator = (ang::collections::ienum<objptr> const* items)
{
	if (_ptr == null)
		set(new collections::vector_buffer<objptr>(items));
	else
		_ptr->copy(items);
	return *this;
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>& ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<objptr>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>& ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator = (ang::object_wrapper<ang::collections::vector_buffer<objptr>> const& other)
{
	set(other._ptr);
	return*this;
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>& ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator += (objptr value)
{
	if (is_empty())
		set(new ang::collections::vector_buffer<objptr>());
	get()->append(ang::move(value));
	return*this;
}


ang::object_wrapper_ptr<ang::collections::vector_buffer<objptr>> ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator & (void)
{
	return this;
}


ang::collections::vector_buffer<objptr> * ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator -> (void)
{
	return get();
}


ang::collections::vector_buffer<objptr> const* ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator -> (void)const
{
	return get();
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator ang::collections::vector_buffer<objptr> * (void)
{
	return get();
}


ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator ang::collections::vector_buffer<objptr> const* (void)const
{
	return get();
}


objptr const& ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator[](int idx)const
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}


objptr & ang::object_wrapper<ang::collections::vector_buffer<objptr>>::operator[](int idx)
{
#ifdef DEBUG_SAFE_CODE
	if (is_empty()) throw(exception_t(except_code::invalid_memory));
	if (((uint)idx >= _ptr->size()) || (idx < 0)) throw(exception_t(except_code::array_overflow));
#endif
	return _ptr->data()[idx];
}

#include "vector_specialization.inl"

ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(short)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(ushort)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(int)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(uint)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(long)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(ulong)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(long64)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(ulong64)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(float)
ANG_IMPLEMENT_VECTOR_DATA_SPECIALIZATION(double)

ANG_IMPLEMENT_INTERFACE_VECTOR_SPECIALIZATION(ang::core::async::iasync_task)
ANG_IMPLEMENT_OBJECT_VECTOR_SPECIALIZATION(ang::core::delegates::function_data<dword(pointer)>)
ANG_IMPLEMENT_OBJECT_VECTOR_SPECIALIZATION(ang::core::delegates::function_data<void(objptr, pointer)>)







