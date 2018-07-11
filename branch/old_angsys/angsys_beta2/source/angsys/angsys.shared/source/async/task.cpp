#include "pch.h"
#include "ang/core/async.hpp"
#include "thread.hpp"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

extern "C" ulong64 ang_get_performance_time();


task::task()
{

}

task::~task()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::core::async::task);
ANG_IMPLEMENT_OBJECTNAME(ang::core::async::task);

bool task::is_inherited_of(type_name_t name)
{
	return name == class_name() ||
		object::is_inherited_of(name) ||
		itask::is_inherited_of(name);
}

bool task::is_kind_of(type_name_t name)const
{
	return name == class_name() ||
		object::is_kind_of(name) ||
		itask::is_kind_of(name);
}

bool task::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null) return false;
	if (name == type_of(*this)) {

		*out = static_cast<task*>(this);
		return true;
	}
	else if (object::query_object(name, out)) {
		return true;
	}
	else if (itask::query_object(name, out)) {
		return true;
	}
	else if (name == type_of<cond*>()) {

		*out = static_cast<async_task*>(this)->cond_.get();
		return true;
	}
	else if (name == type_of<mutex*>()) {

		*out = static_cast<async_task*>(this)->mutex_.get();
		return true;
	}
	return false;
}


itask_t task::run_async(delegates::function <void(itask*)> func)
{
	async_task_t _task = new async_task();
	_task->run(ang::move(func));
	return _task.get();
}

itask_t task::run_async(delegates::function <void(itask*, var_args_t)> func, var_args_t args)
{
	async_task_t _task = new async_task();
	_task->run(ang::move(func), ang::move(args));
	return _task.get();
}

async_task::async_task()
	: dispatched(false)
	, handled(false)
	, cond_(new value_wrapper<cond_t>())
	, mutex_(new value_wrapper<mutex_t>())
	, status_(async_action_status::initializing)
	, worker_thread(null)
	, then_callback(null)
{

}

async_task::async_task(async_task_t parent)
	: dispatched(parent->dispatched)
	, handled(false)
	, cond_(parent->cond_.get())
	, mutex_(parent->mutex_.get())
	, status_(async_action_status::initializing)
	, worker_thread(parent->worker_thread.get())
	, then_callback(null)
{

}

async_task::async_task(dispatcher_thread_t thread)
	: dispatched(true)
	, handled(false)
	, cond_(new value_wrapper<cond_t>())
	, mutex_(new value_wrapper<mutex_t>())
	, status_(async_action_status::initializing)
	, worker_thread(thread.get())
	, then_callback(null)
{

}

async_task::~async_task()
{
	//scope_locker lock = (mutex_t&)mutex;
	join();
}

void async_task::run(delegates::function <void(itask*)> _func)
{
	auto func = _func.get(); func->add_ref(); add_ref();
	thread_routine_t callback = [func, this](icore_thread* thread_, var_args_t a) {
		async_task_t this_ = this;
		scope_locker<mutex_t> lock = (mutex_t&)mutex_;
		if (status_ == async_action_status::canceled) {
			//status_ = async_action_status::completed;
			if(!dispatched) thread_->cancel();
			cond_->signal();
			return;
		}
		status_ = async_action_status::running;
		cond_->signal();

		mutex_->unlock();
		try { func->invoke(this); }
		catch (...) {}
		mutex_->lock();

		if (status_ == async_action_status::canceled) {
			//status_ = async_action_status::completed;
			if (!dispatched) thread_->cancel();
			cond_->signal();
			return;
		}
		if (then_callback.is_empty())
		{
			status_ = async_action_status::wait_for_then;
			cond_->signal();
		}
		else
		{
			status_ = async_action_status::completed;
			worker_thread->then(then_callback, null);
			cond_->signal();
			mutex_->unlock();
			mutex_->lock();
			join();
		}

		func->release();
		release();
	};

	scope_locker<mutex_t> lock = (mutex_t&)mutex_;
	if (worker_thread.is_empty())
	{
		worker_thread = thread::create_thread_suspended(0, null, false);
		status_ = async_action_status::wait_for_start;
		cond_->signal();
		worker_thread->start(callback, null);
	}
	else
	{
		status_ = async_action_status::wait_for_start;
		cond_->signal();
		worker_thread->then(callback, null);
	}

}

void async_task::run(delegates::function <void(itask*, var_args_t)> _func, var_args_t _args)
{
	auto args = _args.get(); args->add_ref();
	auto func = _func.get(); func->add_ref(); add_ref();
	thread_routine_t callback = [args, func,this](icore_thread* thread_, var_args_t a) {
		scope_locker<mutex_t> lock = (mutex_t&)mutex_;
		if (status_ == async_action_status::canceled) {
			//status_ = async_action_status::completed;
			if (!dispatched)thread_->cancel();
			cond_->signal();
			return;
		}
		status_ = async_action_status::running;
		cond_->signal();

		mutex_->unlock();
		try { func->invoke(this, args); }
		catch (...) {}
		mutex_->lock();

		if (status_ == async_action_status::canceled) {
			//status_ = async_action_status::completed;
			if (!dispatched) thread_->cancel();
			cond_->signal();
			return;
		}
		if (then_callback.is_empty())
		{
			status_ = async_action_status::wait_for_then;
			cond_->signal();
		}
		else
		{
			status_ = async_action_status::completed;
			worker_thread->then(then_callback, null);
			cond_->signal();
			mutex_->unlock();
			mutex_->lock();
			join();
			//then_callback = null;
		}
		
		func->release();
		args->release();
		release();
	};

	scope_locker<mutex_t> lock = (mutex_t&)mutex_;
	if (worker_thread.is_empty())
	{
		worker_thread = thread::create_thread_suspended(0, null, false);
		status_ = async_action_status::wait_for_start;
		cond_->signal();
		worker_thread->start(callback, null);
	}
	else
	{
		status_ = async_action_status::wait_for_start;
		cond_->signal();
		worker_thread->then(callback, null);
	}

}

itask_t async_task::then(delegates::function<void(itask*)> _func)
{
	scope_locker<mutex_t> lock = (mutex_t&)mutex_;

	if (worker_thread.is_empty() || !then_callback.is_empty())
		return null;

	if (handled)
		throw_exception(except_code::expired_object);
	else if (status_ & async_action_status::canceled) {
		handled = true;
		status_ = async_action_status::completed;
		throw_exception(except_code::operation_canceled);
	}
	else if (status_ == async_action_status::completed)
		throw_exception(except_code::expired_object);

	async_task* task_ = new async_task(this);
	task_->add_ref();
	auto func = _func.get(); func->add_ref();
	then_callback = [task_, func](icore_thread* this_thread, var_args_t user_args) {
		task_->run(delegates::function<void(itask*)>(func));
		task_->release();
		func->release();
	};

	if (status_ == async_action_status::wait_for_then) {
		status_ = async_action_status::completed;
		worker_thread->then(then_callback, null);
		cond_->signal();
	}

	return task_;
}

bool async_task::join()const
{
	scope_locker<mutex_t> lock = (mutex_t&)mutex_;
	if (worker_thread.is_empty())
		return false;

	if (handled) return true;
	
	handled = true;
	if(!worker_thread->is_current_thread()) cond_->waitfor((mutex_t&)mutex_, [&]()->bool
	{
		return status_ <= async_action_status::running;//  !(async_action_status::finished | async_action_status::wait_for_then).is_active(status_);
	});

	if (status_ == async_action_status::canceled)
	{
		status_ = async_action_status::completed;
		throw_exception(except_code::operation_canceled);
	}
	else
	{	
		status_ = async_action_status::completed;
		if (then_callback.is_empty() && !dispatched) {
			const_cast<async_task*>(this)->worker_thread->cancel();
		}
	}

	return true;
}

bool async_task::wait(async_action_status_t state)const
{
	if (worker_thread.is_empty() || worker_thread->is_current_thread())
		return false;
	scope_locker<mutex_t> lock = (mutex_t&)mutex_;
	if (status_ > state)return false;
	cond_->waitfor((mutex_t&)mutex_,
		[&]() { return !state.is_active(status_); });
	return true;
}

bool async_task::wait(async_action_status_t state, dword ms)const
{
	if (worker_thread.is_empty() || worker_thread->is_current_thread())
		return false;
	dword last_time = (dword)(ang_get_performance_time() / 1000.0);
	dword current = 0;

	scope_locker<mutex_t> lock = (mutex_t&)mutex_;
	if (status_ > state)return false;
	while (!state.is_active(status_))
	{
		cond_->wait((mutex_t&)mutex_, ms);
		thread::sleep(1);
		current = (dword)(ang_get_performance_time() / 1000);
		if (ms <= (current - last_time))
			break;
		else ms -= (current - last_time);
		last_time = current;
	}
	return state.is_active(status_);
}

async_action_status_t async_task::status()const
{
	return status_;
}

bool async_task::cancel()
{
	scope_locker<mutex_t> lock = (mutex_t&)mutex_;
	if (worker_thread.is_empty() || status_ & async_action_status::finished)
		return false;
	status_ = async_action_status::canceled;
	if (!worker_thread->is_current_thread())
		cond_->signal();
	return true;
}


///////////////////////////////////////////////////////////////////////////////


ang::type_name_t ang::core::async::task_handler<void>::class_name()
{
	return "ang::core::async::task_handler<void>"_s;
}

bool ang::core::async::task_handler<void>::is_inherited_of(ang::type_name_t name)
{
	return name == class_name()
		|| iasync<void>::is_inherited_of(name);
}

ang::type_name_t ang::core::async::task_handler<void>::object_name()const
{
	return class_name();
}

bool ang::core::async::task_handler<void>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name()
		|| iasync<void>::is_kind_of(name);
}

bool ang::core::async::task_handler<void>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::core::async::task_handler<void>*>(this);
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
	return false;
}

ang::core::async::task_handler<void>::task_handler()
	: _task(null)
{

}

ang::core::async::task_handler<void>::task_handler(ang::core::async::itask* task)
	: _task(task)
{

}

ang::core::async::task_handler<void>::~task_handler()
{
	if (!_task.is_empty())_task->join();
}


bool ang::core::async::task_handler<void>::wait(ang::core::async::async_action_status_t status)const
{
	return _task.is_empty() ? false : _task->wait(status);
}

bool ang::core::async::task_handler<void>::wait(ang::core::async::async_action_status_t status, dword ms)const
{
	return _task.is_empty() ? false : _task->wait(status, ms);
}

ang::core::async::async_action_status_t ang::core::async::task_handler<void>::status()const
{
	return _task.is_empty() ? async_action_status::error : _task->status();
}

bool ang::core::async::task_handler<void>::cancel()
{
	return _task.is_empty() ? false : _task->cancel();
}

bool ang::core::async::task_handler<void>::join()const
{
	return _task.is_empty() ? false : _task->join();
}

ang::core::async::itask_t ang::core::async::task_handler<void>::then(delegates::function<void(ang::core::async::itask*)> func)
{
	return _task.is_empty() ? null : _task->then(func);
}

ang::core::async::iasync_t<void> ang::core::async::task_handler<void>::create_task(ang::core::delegates::function<void(iasync<void>*)> func)
{
	if (func.is_empty()) return null;
	task_handler_t<void> _async = new ang::core::async::task_handler<void>();
	_async->_task = task::run_async([=](itask* t, var_args_t a) {
		func(_async.get());
	}, { _async.get() });
	return _async.get();
}

ang::core::async::iasync_t<void> ang::core::async::task_handler<void>::create_task(ang::core::async::idispatcher_t dispatcher, ang::core::delegates::function<void(iasync<void>*)> func)
{
	if (func.is_empty()) return null;
	task_handler_t<void> _async = new ang::core::async::task_handler<void>();
	_async->_task = dispatcher->post_task([=](itask* t, var_args_t a) {
		func(_async.get());
	}, { _async.get() });
	return _async.get();
}

ang::core::async::iasync_t<void> ang::core::async::task_handler<void>::create_task(ang::core::delegates::function<void(iasync<void>*, var_args_t)> func, ang::var_args_t args)
{
	if (func.is_empty()) return null;
	task_handler_t<void> _async = new ang::core::async::task_handler<void>();
	_async->_task = task::run_async([=](itask* t, var_args_t a) {
		func(_async.get(), args);
	}, { _async.get(), args });
	return _async.get();
}

ang::core::async::iasync_t<void> ang::core::async::task_handler<void>::create_task(ang::core::async::idispatcher_t dispatcher, ang::core::delegates::function<void(iasync<void>*, var_args_t)> func, ang::var_args_t args)
{
	if (func.is_empty()) return null;
	task_handler_t<void> _async = new task_handler<void>();
	_async->_task = dispatcher->post_task([=](itask* t, var_args_t a) {
		func(_async.get(), args);
	}, { _async.get(), args });
	return _async;
}

