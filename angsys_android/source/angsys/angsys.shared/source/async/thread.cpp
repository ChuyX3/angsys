/*********************************************************************************************************************/
/*   File Name: thread.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/async.h"
#include "ang/core/time.h"
#include "ang_inlines.h"
#include "ang_core_async.h"

//#if defined _DEBUG && defined MEMORY_DEBUGGING
//#define NEW ANG_DEBUG_NEW()
//#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
//#else
//#define NEW new
//#define NEW_ARGS(...) new(__VA_ARGS__)
//#endif


#if defined WINDOWS_PLATFORM
typedef dword thread_result_t;
#else
typedef pointer thread_result_t;
#endif

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

typedef struct _thread_data
{
	thread_t _thread = null;
	async_action_status_t _status = async_action_status::none;
	thread_callback_t _callback = null;
	pointer _args = null;
	dword _result = 0;
}thread_data_t, *thread_data_ptr_t;

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

inline thread_data_ptr_t get_thread_data(ang_core_thread_ptr_t thread) {
	if (thread == null )
		return null;
	return thread->tle_data<thread_data_t>();
}

thread_t thread::main_thread()
{
	auto _thread = get_thread_data(ang_core_thread_main_thread());
	return _thread ? _thread->_thread : null;
}

thread_t thread::current_thread()
{
	auto _this_thread = ang_core_thread_this_thread();
	auto _thread_data = get_thread_data(_this_thread);
	if (_thread_data == null)
	{
	}
	return _thread_data->_thread;
}

dword thread::current_thread_id()
{
	return ang_core_thread_this_thread_id();
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
	if (name == type_of<thread>()
		|| object::is_child_of(name)
		|| name, type_of<iasync_task>())
		return true;
	return false;
}

bool thread::is_kind_of(type_name_t name)const
{
	if (name == type_of<thread>()
		|| object::is_kind_of(name)
		|| name, type_of<iasync_task>())
		return true;
	return false;
}

bool thread::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == type_of<thread>())
	{
		*out = static_cast<thread*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (name == type_of<iasync_task>())
	{
		*out = static_cast<iasync_task*>(this);
		return true;
	}
	return false;
}

bool thread::start(thread_callback_t callback, pointer args
	, thread_priority_t priority, detach_state_t ds)
{
	ang_scope_locker<ang_core_mutex_ptr_t> lock = ang_core_thread_main_mutex();

	if (_handle)
	{
		auto state = _handle->thread_state();
		if (state > ang_thread_state_wait_for_finish && state < ang_thread_state_attached)
			dettach();
		else if (state != ang_thread_state_wait_for_finish)
			return false;
	}

	thread_data_ptr_t data = _handle && _handle->tle_data<thread_data_t>() ?
		_handle->tle_data<thread_data_t>() :
		ang_allocator<thread_data_t>::construct(ang_allocator<thread_data_t>::alloc(1));

	dettach();

#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	uint flags = (ds == detach_state::detached) ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE;
#elif defined WINDOWS_PLATFORM
	uint flags = priority == thread_priority::low ? THREAD_PRIORITY_BELOW_NORMAL
		: priority == thread_priority::high ? THREAD_PRIORITY_NORMAL
		: THREAD_PRIORITY_ABOVE_NORMAL);
#endif

	data->_args = args;
	data->_callback = callback;
	data->_thread = this;
	data->_status = async_action_status::starting;
	_handle = ang_core_thread::create(flags, sizeof(thread_data_t), data, false);

	if (_handle == null)
	{
		ang_allocator<thread_data_t>::destruct_and_free(data);
		return false;
	}

	_handle->set_tle_notify([](ang_core_thread_ptr_t _thread, wsize sz, pointer data)
	{
		if (data && sizeof(thread_data_t) == sz)
		{
			auto thread_data = reinterpret_cast<thread_data_ptr_t>(data);
			thread_data->_thread->dettach();
			thread_data->_status = async_action_status::completed;
			ang_singal_main_cond();
			ang_allocator<thread_data_t>::destruct_and_free(thread_data);
		}
	});

	_handle->start([](pointer args)->thread_result_t
	{
		thread_result_t result;
		auto data = reinterpret_cast<thread_data_ptr_t>(args);

		ang_lock_main_mutex();
		data->_status = async_action_status::running;
		ang_singal_main_cond();
		ang_unlock_main_mutex();
		try {
			result = (thread_result_t)data->_callback(data->_args);
		}
		catch (...) {
			result = (thread_result_t)-1;
		}
		ang_lock_main_mutex();
		data->_result = (dword)result;
		data->_status = async_action_status::completed;
		ang_singal_main_cond();
		ang_unlock_main_mutex();

		return result;
		
	}, data);

	return true;
}

void thread::dettach()
{
	auto _thread = _handle;
	_handle = null;
	if (_thread != null)
		_thread->join();
}

dword thread::result()const
{
	if (!_handle)
		return -1;
	ang_scope_locker<ang_core_mutex_ptr_t> lock = ang_core_thread_main_mutex();

	async_action_status_t status = async_action_status::finished;
	auto data = _handle->tle_data<thread_data_t>();
	if (is_current_thread())
		return data->_result;
	else while (!status.is_active(data->_status))
		ang_wait_main_cond();
	return data->_result;
}

pointer thread::handle()const
{
	return _handle;
}

bool thread::is_main_thread()const
{
	return  (_handle) ? _handle->is_main_thread() : false;
}

bool thread::is_current_thread()const
{
	return current_thread_id() == id();
}

void thread::then(delegates::function<void(iasync<dword>*)> callback)
{
	if (!_handle && _handle->thread_state() > ang_thread_state_wait_for_finish)
		return;

	struct then_context
	{
		thread_t thread;
		delegates::function<void(iasync<dword>*)> callback;
	};

	then_context* context = ang_allocator<then_context>::construct(ang_allocator<then_context>::alloc(1));
	context->thread = this;
	context->callback = callback;
	callback.get()->add_ref();
	_handle->then([](pointer args, pointer result)->thread_result_t
	{
		auto context = (then_context*)args;
		context->callback(context->thread.get());
		return (thread_result_t)context->thread->result();
	}, context);
}

async_action_status_t thread::status()const
{
	thread_data_ptr_t data;
	if (_handle && (data = _handle->tle_data<thread_data_t>()))
		return data->_status;
	return async_action_status::stopped;
}

bool thread::cancel()
{
	if (!_handle)
		return false;
	auto data = _handle->tle_data<thread_data_t>();
	data->_status = async_action_status::canceled;
	return _handle->cancel();
}

bool thread::suspend()
{
	if (_handle == null)
		return false;

	auto data = _handle->tle_data<thread_data_t>();
	ang_scope_locker<ang_core_mutex_ptr_t> lock = ang_core_thread_main_mutex();
	if (data->_status != async_action_status::running)
		return false;
	if (data->_status == async_action_status::suspended)
		return true;
	if(is_current_thread()) while (data->_status == async_action_status::suspended)
		ang_wait_main_cond();
	return true;
}

bool thread::resume()
{
	if (_handle == null || is_current_thread())
		return false;
	auto data = _handle->tle_data<thread_data_t>();
	ang_scope_locker<ang_core_mutex_ptr_t> lock = ang_core_thread_main_mutex();
	if (data->_status != async_action_status::suspended)
		return false;

	data->_status = async_action_status::running;
	ang_singal_main_cond();
	return true;
}

bool thread::wait(async_action_status_t status, dword ms)const
{
	thread_t prevent_destruction = const_cast<thread*>(this);
	if (_handle == null || is_current_thread())
		return false;
	auto data = _handle->tle_data<thread_data_t>();
	ang_scope_locker<ang_core_mutex_ptr_t> lock = ang_core_thread_main_mutex();
	if (ms == dword(-1))
	{
		if (status.is_active(data->_status.value()))
			return true;
		auto last = time::get_performance_time();
		auto _ms = double(ms);
		while (_ms > 0.0 && !status.is_active(data->_status.value()))
		{
			ang_timed_wait_main_cond(dword(_ms));
			auto current = time::get_performance_time();
			_ms -= (current - last);
			last = current;
		}
		return status.is_active(data->_status.value());
	}
	else
	{
		while(!status.is_active(data->_status.value()))
			ang_wait_main_cond();
		return true;
	}
}

void thread::join()
{
	if (!_handle) return;
	auto data = _handle->tle_data<thread_data_t>();
	data->_status = async_action_status::canceled;
	 _handle->join();
}

dword thread::id()const
{
	return _handle != null ? _handle->thread_id() : -1;
}
