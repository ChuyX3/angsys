/*********************************************************************************************************************/
/*   File Name: dispatcher_thread.cpp                                                                                */
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


ang::object_wrapper<dispatcher_thread>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<dispatcher_thread>::object_wrapper(dispatcher_thread* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<dispatcher_thread>::object_wrapper(object_wrapper<dispatcher_thread> && other) : _ptr(null) {
	dispatcher_thread * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<dispatcher_thread>::object_wrapper(object_wrapper<dispatcher_thread> const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<dispatcher_thread>::object_wrapper(std::nullptr_t const&) : _ptr(null) {
}

ang::object_wrapper<dispatcher_thread>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<dispatcher_thread>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

bool ang::object_wrapper<dispatcher_thread>::is_empty()const
{
	return _ptr == null;
}


dispatcher_thread* ang::object_wrapper<dispatcher_thread>::get(void)const
{
	return _ptr;
}


void ang::object_wrapper<dispatcher_thread>::set(dispatcher_thread* ptr)
{
	dispatcher_thread * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<dispatcher_thread>& ang::object_wrapper<dispatcher_thread>::operator = (dispatcher_thread* ptr)
{
	set(ptr);
	return*this;
}


ang::object_wrapper<dispatcher_thread>& ang::object_wrapper<dispatcher_thread>::operator = (ang::object_wrapper<dispatcher_thread> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::object_wrapper<dispatcher_thread>& ang::object_wrapper<dispatcher_thread>::operator = (ang::object_wrapper<dispatcher_thread> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper<dispatcher_thread>::operator dispatcher_thread * (void)
{
	return get();
}


ang::object_wrapper<dispatcher_thread>::operator dispatcher_thread const* (void)const
{
	return get();
}

dispatcher_thread * ang::object_wrapper<dispatcher_thread>::operator -> (void)
{
	return get();
}

dispatcher_thread const* ang::object_wrapper<dispatcher_thread>::operator -> (void)const
{
	return get();
}

///////////////////////////////////////////////////////////////

dispatcher_thread::dispatcher_thread()
	: thread()
	, _cond(thread_manager::instance()->main_cond)
	, _mutex(thread_manager::instance()->main_mutex)
{

}

dispatcher_thread::~dispatcher_thread()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::async::dispatcher_thread, ang::core::async::thread);

bool dispatcher_thread::dispatch()
{
	_mutex->lock();
	ang::swap(_tasks, _tasks_backup);
	_mutex->unlock();
	while (!_tasks_backup.is_empty() && _tasks_backup->has_items())
	{
		thread_callback_t task;
		_tasks_backup->pop(task, false);
		task(nullptr);
	}
	return true;
}

bool dispatcher_thread::start(thread_callback_t callback, void_args_t args, thread_priority_t priority)
{
	return start(callback, args, priority, detach_state::joinable);
}

bool dispatcher_thread::start(thread_callback_t callback, void_args_t args
	, thread_priority_t priority, detach_state_t)
{
	thread::start([this, callback, args](pointer)->dword
	{
		auto _thread = reinterpret_cast<thread_handler_t>(_handle);
		start_event(this, args);
		while (_thread->_status != async_action_status::canceled)
		{
			{ //suspending if _status is async_action_status::suspended
				scope_locker lock = _mutex;
				_cond->waitfor(_mutex, [&]() {return _thread->_status == async_action_status::suspended; });
			}
			_mutex->lock();
			ang::swap(_tasks, _tasks_backup);
			_mutex->unlock();

			while (!_tasks_backup.is_empty() && _tasks_backup->has_items())
			{
				thread_callback_t task;
				_tasks_backup->pop(task, false);	
				task(nullptr);
			}

			if (!callback.is_empty())
			{	
				callback(args);			
			}
			else
			{
				_mutex->lock();
				_thread->_status = async_action_status::suspended;
				_cond->waitfor(_mutex, [&]()
				{
					return _thread->_status == async_action_status::suspended;
				});
				_mutex->unlock();
			}
		}
		end_event(this, args);
		return 0;
	}, args, priority, detach_state::joinable);
	return true;
}

bool dispatcher_thread::post_task(thread_callback_t callback)
{
	scope_locker lock = _mutex;
	_tasks += callback;
	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	if (_thread->_status == async_action_status::suspended) {
		_thread->_status = async_action_status::running;
		_cond->signal();
	}
	return true;
}

async_action_status_t dispatcher_thread::status()const
{
	return  (_handle) ? (async_action_status)thread_handler_t(_handle)->_status.value() : async_action_status::stopped;
}

bool dispatcher_thread::cancel()
{
	if (_handle == null)
		return false;
	scope_locker lock = _mutex;
	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	async_action_status_t status = async_action_status::finished;
	if (!status.is_active(_thread->_status))
	{
		if (_thread->_status == async_action_status::suspended)
		{
			_thread->_status = async_action_status::canceled;
			_cond->signal();
		}
		else
		{
			_thread->_status = async_action_status::canceled;
		}
		return true;
	}
	return false;
}

bool dispatcher_thread::suspend()
{
	if (_handle == null)
		return false;
	scope_locker lock = _mutex;
	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	if (_thread->_status == async_action_status::running)
	{
		_thread->_status = async_action_status::suspended;
		return true;
	}
	return false;
}

bool dispatcher_thread::resume()
{
	if (_handle == null || is_current_thread())
		return false;
	scope_locker lock = _mutex;
	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	if (_thread->_status == async_action_status::suspended)
	{
		_thread->_status = async_action_status::running;
		_cond->signal();
		return true;
	}
	return false;
}

bool dispatcher_thread::wait(async_action_status_t status, dword ms)const
{
	thread_t prevent_destruction = const_cast<dispatcher_thread*>(this);
	if (_handle == null || is_current_thread())
		return false;
	auto _thread = reinterpret_cast<thread_handler_t>(_handle);
	if (ms == dword(-1))
	{
		scope_locker locker = _mutex;
		_cond->waitfor(_mutex, [&]()
		{
			return !status.is_active(_thread->_status);
		});
		return true;
	}
	else
	{
		scope_locker locker = _mutex;
		_cond->wait(_mutex, ms);
		return status.is_active(_thread->_status);
	}
}

