/*********************************************************************************************************************/
/*   File Name: thread.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "thread_manager.h"
#include "ang/core/time.h"

//#if defined _DEBUG && defined MEMORY_DEBUGGING
//#define NEW ANG_DEBUG_NEW()
//#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
//#else
//#define NEW new
//#define NEW_ARGS(...) new(__VA_ARGS__)
//#endif


#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;


///////////////////////////////////////////////////////////////


ang::object_wrapper<thread>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<thread>::object_wrapper(thread* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<thread>::object_wrapper(object_wrapper<thread> && other) : _ptr(null) {
	thread * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<thread>::object_wrapper(object_wrapper<thread> const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<thread>::object_wrapper(std::nullptr_t const&) : _ptr(null) {
}

ang::object_wrapper<thread>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<thread>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

bool ang::object_wrapper<thread>::is_empty()const
{
	return _ptr == null;
}


thread* ang::object_wrapper<thread>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<thread>::set(thread* ptr)
{
	thread * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<thread>& ang::object_wrapper<thread>::operator = (thread* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<thread>& ang::object_wrapper<thread>::operator = (ang::object_wrapper<thread> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<thread>& ang::object_wrapper<thread>::operator = (ang::object_wrapper<thread> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper<thread>::operator thread * (void)
{
	return get();
}


ang::object_wrapper<thread>::operator thread const* (void)const
{
	return get();
}

thread * ang::object_wrapper<thread>::operator -> (void)
{
	return get();
}

thread const* ang::object_wrapper<thread>::operator -> (void)const
{
	return get();
}

///////////////////////////////////////////////////////////////

void thread::sleep(dword dwMilliseconds)
{
#if defined WINDOWS_PLATFORM
	::Sleep(dwMilliseconds);
#elif defined __ANDROID__ || defined LINUX
	::usleep(dwMilliseconds * 1000);
#endif
}

thread_t thread::main_thread()
{
	return thread_manager::get_main_thread();
}

dword thread::current_thread_id()
{
	return thread_manager::get_current_thread_id();
}

thread::thread()
	: _handle(null)
{

}

thread::~thread()
{
	dettach();
}

ANG_IMPLEMENT_CLASSNAME(ang::core::async::thread);
ANG_IMPLEMENT_OBJECTNAME(ang::core::async::thread);

bool thread::is_child_of(type_name_t name)
{
	if (name == type_name<thread>()
		|| object::is_child_of(name)
		|| name, type_name<iasync_task>())
		return true;
	return false;
}

bool thread::is_kind_of(type_name_t name)const
{
	if (name == type_name<thread>()
		|| object::is_kind_of(name)
		|| name, type_name<iasync_task>())
		return true;
	return false;
}

bool thread::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_name<thread>())
	{
		*out = static_cast<thread*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_name<iasync_task>())
	{
		*out = static_cast<iasync_task*>(this);
		return true;
	}
	return false;
}

bool thread::start(thread_callback_t callback, void_args_t args
	, thread_priority_t priority, detach_state_t ds)
{
	scope_locker lock = thread_manager::instance()->main_mutex;
	auto handle = thread_handler_t(_handle);
	if (handle != null && handle->_thread.get() == this)
		return false;

	dettach();

	_handle = thread_manager::create_thread(this, callback, args, priority, ds);
	if (_handle == null)
		return false;
	
	return true;
}

void thread::dettach()
{
	auto handle = thread_handler_t(_handle);
	_handle = null;
	if (handle != null)
	{
#if defined WINDOWS_PLATFORM
		::CloseHandle(handle->_handle);
#endif
		delete handle;
	}
}

pointer thread::handle()const
{
	return _handle;
}

bool thread::is_main_thread()const
{
	return  (_handle) ? thread_handler_t(_handle)->_is_main_thread : false;
}

bool thread::is_current_thread()const
{
	return current_thread() == this;
}

async_action_status_t thread::status()const
{
	return  (_handle) ? (async_action_status)thread_handler_t(_handle)->_status.value() : async_action_status::stopped;
}

bool thread::cancel()
{
	return false;
}

bool thread::suspend()
{
	if (_handle == null)
		return false;

	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	if(is_current_thread())
	{
		scope_locker lock = thread_manager::instance()->main_mutex;
		_thread->_status = async_action_status::suspended;
		thread_manager::instance()->main_cond->waitfor(thread_manager::instance()->main_mutex
			, [&]() {return _thread->_status == async_action_status::suspended; });
	}
	else
	{
#ifdef WINDOWS_PLATFORM
		scope_locker lock = thread_manager::instance()->main_mutex;
		if (_thread->_status == async_action_status::suspended)
			return true; //already suspended
		else if (SuspendThread(_thread->_handle) != dword(-1))
			_thread->_status = async_action_status::suspended;
		else
			return false;
#elif defined __ANDROID__ || defined LINUX
		return false;
#endif
	}
	return true;

}

bool thread::resume()
{
	if (_handle == null || is_current_thread())
		return false;
#if defined WINDOWS_PLATFORM
	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	scope_locker lock = thread_manager::instance()->main_mutex;

	if (_thread->_status != async_action_status::suspended)
		return false;

	auto res = ResumeThread(_thread->_handle);
	if (res != dword(-1))
	{
		_thread->_status = async_action_status::running;
		if (res == 0) thread_manager::instance()->main_cond->signal();
	}
	return true;
#elif defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	scope_locker lock = thread_manager::instance()->main_mutex;

	if (_thread->_status != async_action_status::suspended)
		return false;
	_thread->_status = async_action_status::running;
	thread_manager::instance()->main_cond->signal();
	return true;
#endif
}

bool thread::wait(async_action_status_t status, dword ms)const
{
	thread_t prevent_destruction = const_cast<thread*>(this);
	if (_handle == null || is_current_thread())
		return false;
	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	if (ms == dword(-1))
	{
		scope_locker lock = thread_manager::instance()->main_mutex;
		if (status.is_active(_thread->_status.value()))
			return true;
		auto last = time::get_performance_time();
		auto _ms = double_t(ms);
		while (_ms > 0.0 && !status.is_active(_thread->_status.value()))
		{
			thread_manager::instance()->main_cond->wait(thread_manager::instance()->main_mutex, dword((uint)_ms));
			auto current = time::get_performance_time();
			_ms -= (current - last);
			last = current;
		}
		return status.is_active(_thread->_status.value());
	}
	else
	{
		scope_locker lock = thread_manager::instance()->main_mutex;
		thread_manager::instance()->main_cond->waitfor(thread_manager::instance()->main_mutex,
			[&]() { return !status.is_active(_thread->_status.value()); });
		return true;
	}
}

void thread::join()
{
	if (_handle == null || is_current_thread() || is_main_thread())
		return;

	wait(async_action_status::stopped, -1);
}

dword thread::id()const
{
	if (_handle == null)
		return -1;
	return reinterpret_cast<thread_handler_t>(_handle)->_id;
}
