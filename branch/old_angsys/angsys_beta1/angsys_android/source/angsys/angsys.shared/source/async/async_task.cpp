/*********************************************************************************************************************/
/*   File Name: async_task.cpp                                                                                       */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/core/async.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

#if defined _DEBUG
#define new ANG_DEBUG_NEW()
#endif

ang::type_name_t ang::core::async::async_task_result<void>::class_name()
{
	return "ang::core::async::async_task_result<void>"_s;
}


bool ang::core::async::async_task_result<void>::is_child_of(ang::type_name_t name)
{
	return name == class_name()
		|| iasync<void>::is_child_of(name);
}


ang::type_name_t ang::core::async::async_task_result<void>::object_name()const
{
	return class_name();
}


bool ang::core::async::async_task_result<void>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name()
		|| iasync<void>::is_kind_of(name);
}


bool ang::core::async::async_task_result<void>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::core::async::async_task_result<void>*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (iasync<void>::query_object(name, out))
	{
		return true;
	}
	else if (iasync<void>::query_object(name, out))
	{
		return true;
	}
	return false;
}



ang::core::async::async_task_result<void>::async_task_result()
	: _cond(new ang::core::async::cond())
	, _mutex(new ang::core::async::mutex())
	, _was_canceled(false)
	, _handled(false)
	, _thread(new ang::core::async::thread())
	, _status(ang::core::async::async_action_status::starting)
{

}


ang::core::async::async_task_result<void>::~async_task_result()
{
}


void ang::core::async::async_task_result<void>::complete()
{
	if (_thread.is_empty() || !_thread->is_current_thread())
		throw(ang::exception_t(ang::except_code::invalid_access));

	scope_locker locker = _mutex;
	_was_canceled = _status == async_action_status::canceled;
	_status = async_action_status::completed;
	_cond->signal();
	if (!_then.is_empty())
	{
		_then(this);
	}
}

bool ang::core::async::async_task_result<void>::wait(ang::core::async::async_action_status_t status, dword ms)const
{
	async_task_result_t<void> prevent_destruction = const_cast<async_task_result<void>*>(this);
	if (_thread->is_current_thread())
		return false;
	if (ms == dword(-1))
	{
		scope_locker locker = _mutex;
		_cond->waitfor(_mutex, [&]()
		{
			return !status.is_active(_status);
		});
		return true;
	}
	else
	{
		scope_locker locker = _mutex;
		if(!status.is_active(_status))
			_cond->wait(_mutex, ms);
		return status.is_active(_status);
	}
}


ang::core::async::async_action_status_t ang::core::async::async_task_result<void>::status()const
{
	return _status;
}


bool ang::core::async::async_task_result<void>::cancel()
{
	scope_locker locker = _mutex;
	async_action_status_t status = async_action_status::starting
		+ async_action_status::running
		+ async_action_status::suspended;

	if (status.is_active(_status))
	{
		_status = async_action_status::canceled;
		return true;
	}
	return false;
}


bool ang::core::async::async_task_result<void>::suspend()
{
	scope_locker locker = _mutex;
	if (_status == async_action_status::running)
	{
		_status = async_action_status::suspended;
		return true;
	}
	return false;
}


bool ang::core::async::async_task_result<void>::resume()
{
	scope_locker locker = _mutex;
	if (_status == async_action_status::suspended)
	{
		_status = async_action_status::running;
		return true;
	}
	return false;
}


void ang::core::async::async_task_result<void>::result()const
{
	async_task_result_t<void> prevent_destruction = const_cast<async_task_result<void>*>(this);
	if (_was_canceled)
		throw exception_t(except_code::operation_canceled);
	if (_thread->is_current_thread())
		return;
	wait(async_action_status::completed, -1);
	return;
}


void ang::core::async::async_task_result<void>::then(delegates::function<void(iasync<void>*)> func)
{
	scope_locker lock = _mutex;
	if (_handled) {
		throw(ang::exception_t(ang::except_code::expired_object));
	}
	_then = func;
	if (_status == async_action_status::completed)
	{
		_handled = true;
		this->add_ref();
		if (!_thread->start([=](pointer)->dword
		{
			this->_then(this);
			this->release();
			return 0;
		}, null))
		{
			dispatcher_thread_t dispatcher = interface_cast<dispatcher_thread>(_thread.get());
			if (dispatcher)
			{
				dispatcher->post_task([=](pointer)->dword {
					this->_then(this);
					this->release();
					return 0;
				});
			}
			else
			{
				_thread = thread::create_worker_thread([=](void)
				{
					this->_then(this);
					this->release();
				});
			}
		}
	}
}


ang::core::async::iasync_t<void> ang::core::async::async_task<void>::run_async(ang::core::delegates::function<void(iasync<void>*, var_args_t)> _func, var_args_t _args, ang::core::async::thread_priority_t priority)
{
	if (_func.is_empty())
		return null;

	auto args = _args.get();
	if (args)args->add_ref();

	auto func = _func.get();
	func->add_ref();

	async_task_result<void>* _async = new async_task_result<void>();
	_async->add_ref();
	auto terminate = [=]()->dword {
		func->release();
		if (args)args->release();
		_async->release();
		return 0U;
	};

	if (!_async->_thread->start([=](pointer)->dword {
		_async->_status = async_action_status::running;
		_async->_cond->signal();
		func->invoke(_async, args);
		_async->complete();
		return terminate();

	}, null, priority))
		return (iasync<void>*)(wsize)terminate();
	return _async;
}


ang::core::async::iasync_t<void> ang::core::async::async_task<void>::run_async(ang::core::async::async_task_result<void>* _async, ang::core::delegates::function<void(iasync<void>*, var_args_t)> _func, ang::var_args_t _args, ang::core::async::thread_priority_t priority)
{
	if (_func.is_empty())
		return null;

	auto args = _args.get();
	if (args)args->add_ref();

	auto func = _func.get();
	func->add_ref();

	_async->add_ref();

	auto terminate = [=]()->dword {
		func->release();
		if (args)args->release();
		_async->release();
		return 0U;
	};

	if (!_async->_thread->start([=](pointer)->dword {
		_async->_status = async_action_status::running;
		_async->_cond->signal();
		func->invoke(_async, args);
		_async->complete();
		return terminate();

	}, null, priority))
		return (iasync<void>*)(wsize)terminate();
	return _async;
}
