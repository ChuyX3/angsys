#include "pch.h"
#include "ang/core/async.hpp"
#include "task.hpp"

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

bool task::is_child_of(type_name_t name)
{
	return name == class_name() ||
		object::is_child_of(name) ||
		itask::is_child_of(name);
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
	: handled(false)
	, cond_(new value_wrapper<cond_t>())
	, mutex_(new value_wrapper<mutex_t>())
	, worker_thread(null)
	, status_(async_action_status::initializing)
	, then_callback(null)
{

}

async_task::async_task(async_task_t parent)
	: handled(false)
	, cond_(parent->cond_.get())
	, mutex_(parent->mutex_.get())
	, worker_thread(parent->worker_thread.get())
	, status_(async_action_status::initializing)
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
		scope_locker lock = (mutex_t&)mutex_;
		if (status_ == async_action_status::canceled) {
			//status_ = async_action_status::completed;
			thread_->cancel();
			cond_.get();
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
			thread_->cancel();
			cond_->signal();
			return;
		}
		if (worker_thread.is_empty())
		{
			status_ = async_action_status::wait_for_then;
		}
		else
		{
			status_ = async_action_status::completed;
			worker_thread->then(then_callback, null);
			cond_->signal();
		}
		cond_->signal();
		func->release();
		release();
	};

	scope_locker lock = (mutex_t&)mutex_;
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
		scope_locker lock = (mutex_t&)mutex_;
		if (status_ == async_action_status::canceled) {
			//status_ = async_action_status::completed;
			thread_->cancel();
			cond_.get();
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
			thread_->cancel();
			cond_->signal();
			return;
		}
		if (worker_thread.is_empty())
		{
			status_ = async_action_status::wait_for_then;
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
		cond_->signal();
		func->release();
		args->release();
		release();
	};

	scope_locker lock = (mutex_t&)mutex_;
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
	scope_locker lock = (mutex_t&)mutex_;

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
	scope_locker lock = (mutex_t&)mutex_;
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
		if (then_callback.is_empty()) {
			const_cast<async_task*>(this)->worker_thread->cancel();
		}
	}

	return true;
}

bool async_task::wait(async_action_status_t state)const
{
	if (worker_thread.is_empty() || worker_thread->is_current_thread())
		return false;
	scope_locker lock = (mutex_t&)mutex_;
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

	scope_locker lock = (mutex_t&)mutex_;
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
	scope_locker lock = (mutex_t&)mutex_;
	if (worker_thread.is_empty() || status_ & async_action_status::finished)
		return false;
	status_ = async_action_status::canceled;
	if (!worker_thread->is_current_thread())
		cond_->signal();
	return true;
}


