/*********************************************************************************************************************/
/*   File Name: async.cpp                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "ang/core/delegates.h"
#include "ang/core/async.h"
#include "ang_inlines.h"
#include "ang_define_enum.h"

#if defined _DEBUG && defined MEMORY_DEBUGGING
#define NEW ANG_DEBUG_NEW()
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

#define GET_DEFAULT_ALLOC() memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator)

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

////////////////////////////////////////////////////////////////////////////////////

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_data<void(void)> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::~object_wrapper()
{
}

void ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::is_empty()const
{
	return _ptr == null;
}

ang::core::delegates::function_data<void(void)>* ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::set(ang::core::delegates::function_data<void(void)>* ptr)
{
	core::delegates::function_data<void(void)> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::core::delegates::function_data<void(void)>** ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>& ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(void)>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>& ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(void)>> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper_ptr<ang::core::delegates::function_data<void(void)>> ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::operator & (void)
{
	return this;
}

////////////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_data<void(iasync<void>*)> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{
}

ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::~object_wrapper()
{
}

void ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::is_empty()const
{
	return _ptr == null;
}

ang::core::delegates::function_data<void(iasync<void>*)>* ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::get()const
{
	return _ptr;
}

void ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::set(ang::core::delegates::function_data<void(iasync<void>*)>* ptr)
{
	core::delegates::function_data<void(iasync<void>*)> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::core::delegates::function_data<void(iasync<void>*)>** ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::addres_of()
{
	return &_ptr;
}

ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>& ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>& ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper_ptr<ang::core::delegates::function_data<void(iasync<void>*)>> ang::object_wrapper<ang::core::delegates::function_data<void(iasync<void>*)>>::operator & ()
{
	return this;
}

////////////////////////////////////////////////////////////////////////////////////

ANG_IMPLEMENT_INTERFACE(ang::core::async, iasync_task);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::async::iasync<void>, iasync_task)

ANG_IMPLEMENT_ENUM(async, thread_priority, word, thread_priority::normal);

ANG_IMPLEMENT_ENUM_PARSE(thread_priority, thread_priority::normal, //alphabetical order
	high,
	low,
	normal
);

ANG_IMPLEMENT_ENUM_TOSTRING(thread_priority, "normal", //value order
	low,
	normal,
	high
);

ANG_IMPLEMENT_ENUM(async, detach_state, word, detach_state::joinable);
ANG_IMPLEMENT_ENUM_PARSE(detach_state, detach_state::joinable, //alphabetical order
	detached,
	joinable
);
ANG_IMPLEMENT_ENUM_TOSTRING(detach_state, "joinable", //value order
	detached,
	joinable
);

////////////////////////////////////////////////////////////////
ANG_IMPLEMENT_FLAGS(async, async_action_status, uint)

////////////////////////////////////////////////////////////////

mutex::mutex()
	: _handle(null)
{
	_handle = ang_core_mutex::create();
}

mutex::mutex(bool _lock)
	: _handle(null)
{
	_handle = ang_core_mutex::create();
	if (_lock) _handle->lock();
}

mutex::~mutex()
{
	if (_handle != null)
		ang_core_mutex::destroy(_handle);
	_handle = null;
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::async::mutex, ang::object);

bool mutex::lock()const
{
	if (_handle != null)
		return _handle->lock();
	return false;
}

bool mutex::trylock()const
{
	if (_handle != null)
		return _handle->trylock();
	return false;
}

bool mutex::unlock()const
{
	if (_handle != null)
		return _handle->unlock();
	return false;
}

////////////////////////////////////////////////////////////////////////////////

ang::object_wrapper<mutex>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<mutex>::object_wrapper(mutex* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<mutex>::object_wrapper(object_wrapper<mutex> && other) : _ptr(null) {
	mutex * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<mutex>::object_wrapper(object_wrapper<mutex> const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<mutex>::object_wrapper(ang::nullptr_t const&) : _ptr(null) {
}


ang::object_wrapper<mutex>::~object_wrapper()
{
	clean();
}


void ang::object_wrapper<mutex>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

bool ang::object_wrapper<mutex>::is_empty()const
{
	return _ptr == null;
}


mutex* ang::object_wrapper<mutex>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<mutex>::set(mutex* ptr)
{
	mutex * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<mutex>& ang::object_wrapper<mutex>::operator = (mutex* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<mutex>& ang::object_wrapper<mutex>::operator = (ang::object_wrapper<mutex> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<mutex>& ang::object_wrapper<mutex>::operator = (ang::object_wrapper<mutex> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper<mutex>::operator mutex * (void)
{
	return get();
}


ang::object_wrapper<mutex>::operator mutex const* (void)const
{
	return get();
}

mutex * ang::object_wrapper<mutex>::operator -> (void)const
{
	return get();
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

cond::cond()
{
	_handle = ang_core_cond::create();
}

cond::~cond()
{
	if (_handle != null)
		ang_core_cond::destroy(_handle);
	_handle = null;
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::async::cond, ang::object);


bool cond::wait(mutex_t mutex)const
{
	if (!mutex.is_empty() && _handle != null)
		return _handle->wait(mutex->_handle);
	return false;
}

bool cond::wait(mutex_t mutex, dword ms)const
{
	if (!mutex.is_empty() && _handle != null)
		return _handle->wait(mutex->_handle, ms);
	return false;
}

bool cond::signal()const
{
	if (_handle != null)
		return _handle->signal();
	return false;
}

////////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<cond>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<cond>::object_wrapper(cond* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<cond>::object_wrapper(object_wrapper<cond> && other) : _ptr(null) {
	cond * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<cond>::object_wrapper(object_wrapper<cond> const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<cond>::object_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

ang::object_wrapper<cond>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<cond>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

bool ang::object_wrapper<cond>::is_empty()const
{
	return _ptr == null;
}


cond* ang::object_wrapper<cond>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<cond>::set(cond* ptr)
{
	cond * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<cond>& ang::object_wrapper<cond>::operator = (cond* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<cond>& ang::object_wrapper<cond>::operator = (ang::object_wrapper<cond> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<cond>& ang::object_wrapper<cond>::operator = (ang::object_wrapper<cond> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper<cond>::operator cond * (void)
{
	return get();
}


ang::object_wrapper<cond>::operator cond const* (void)const
{
	return get();
}

cond* ang::object_wrapper<cond>::operator -> (void)const
{
	return get();
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

waiter::waiter()
{
}

waiter::~waiter()
{
	clean();
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::async::waiter, ang::object);

void waiter::clean()
{
	_handle = null;
}


bool waiter::add(iasync_task_t waitable)
{
	_handle += ang::move(waitable);
	return true;
}

bool waiter::wait_all(async_action_status_t status, dword ms)const
{
	if (_handle.is_empty())
		return false;
	bool res = true;
	for(iasync_task_t const& wait : _handle)
		res = res && wait->wait(status, ms);
	return res;
}


////////////////////////////////////////////////////////////////////////////////


ang::object_wrapper<waiter>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<waiter>::object_wrapper(waiter* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<waiter>::object_wrapper(object_wrapper<waiter> && other) : _ptr(null) {
	waiter * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<waiter>::object_wrapper(object_wrapper<waiter> const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<waiter>::object_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

ang::object_wrapper<waiter>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<waiter>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

bool ang::object_wrapper<waiter>::is_empty()const
{
	return _ptr == null;
}


waiter* ang::object_wrapper<waiter>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<waiter>::set(waiter* ptr)
{
	waiter * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<waiter>& ang::object_wrapper<waiter>::operator = (waiter* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<waiter>& ang::object_wrapper<waiter>::operator = (ang::object_wrapper<waiter> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<waiter>& ang::object_wrapper<waiter>::operator = (ang::object_wrapper<waiter> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper<waiter>& ang::object_wrapper<waiter>::operator += (ang::core::async::iasync_task_t w)
{
	if (_ptr == null)
		_ptr = new waiter();
	_ptr->add(ang::move(w));
	return*this;
}

ang::object_wrapper<waiter>::operator waiter * (void)
{
	return get();
}


ang::object_wrapper<waiter>::operator waiter const* (void)const
{
	return get();
}

waiter* ang::object_wrapper<waiter>::operator -> (void)const
{
	return get();
}
