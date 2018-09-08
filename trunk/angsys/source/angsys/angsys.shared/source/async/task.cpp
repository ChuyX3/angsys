#include "pch.h"
#include <ang/core/async.h>
#include "thread_manager.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined _DEBUG || defined _DEVELOPPER
#define new new(__FILE__, __LINE__)
#define new_args(...) new(__VAR_ARGS__, __FILE__, __LINE__)
#else
#define new_args(...) new(__VAR_ARGS__)
#endif // MEMORY_

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

extern "C" ulong64 get_performance_time_us();

////////////////////////////////////////////////////////////////////////////////////////


template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<void>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<mchar>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<wchar>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char16>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<char32>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<short>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ushort>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<int>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<uint>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<long>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ulong>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<long64>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<ulong64>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<float>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<double>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<object>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<variant>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<string>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<mstring>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<wstring>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<string16>, interface);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::iaction<string32>, interface);


template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<void>, iaction<void>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char>, iaction<char>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<mchar>, iaction<mchar>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<wchar>, iaction<wchar>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char16>, iaction<char16>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<char32>, iaction<char32>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<short>, iaction<short>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ushort>, iaction<ushort>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<int>, iaction<int>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<uint>, iaction<uint>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<long>, iaction<long>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ulong>, iaction<ulong>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<long64>, iaction<long64>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<ulong64>, iaction<ulong64>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<float>, iaction<float>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<double>, iaction<double>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<object>, iaction<object>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<variant>, iaction<variant>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<string>, iaction<string>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<mstring>, iaction<mstring>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<wstring>, iaction<wstring>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<string16>, iaction<string16>);
template<> ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::itask<string32>, iaction<string32>);

////////////////////////////////////////////////////////////////////////////////////////

template<> iasync<void> task::run_async(core::delegates::function<void(iasync<void>)> callback)
{
	auto thread = thread::create_thread();
	return thread->run(callback);
}

////////////////////////////////////////////////////////////////////////////////////////

task::task()
{

}

task::~task()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::task, object, itask<void>);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::async::task);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::task, object, itask<void>, iaction<void>);

////////////////////////////////////////////////////////////////////////////////////////

thread_task::thread_task(worker_thread_t th)
	: _was_canceled(false)
	, _state(async_action_status::initializing)
	, _parent_task(null)
	, _child_task(null)
	, _thread(th)
{

}

thread_task::~thread_task()
{
	action.empty();
}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::thread_task, task);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::async::thread_task);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::thread_task, task);

void thread_task::run()
{
	_mutex.lock();
	if (_state == async_action_status::canceled)
	{
		_state = async_action_status::completed;
		action.empty();
		_cond.signal();
		_mutex.unlock();
		return;
	}
	_state = async_action_status::running;

	_mutex.unlock();

	action(this);
	
	_mutex.lock();

	if (_state == async_action_status::canceled)
	{
		_state = async_action_status::completed;
		action.empty();
	}
	else if (_child_task.is_empty())
	{
		_state = async_action_status::wait_for_then;
	}
	else
	{
		_child_task->mutex().lock();
		if (_child_task->_state == async_action_status::canceled)
		{
			_child_task->mutex().unlock();
			_child_task = null;
			_state = async_action_status::wait_for_then;
		}
		else
		{
			_thread->post_task(_child_task);
			_child_task->mutex().unlock();
			_child_task = null;
			_thread = null; //unreference thread
			_state = async_action_status::completed;
			action.empty();
		}
	}

	_cond.signal();
	_mutex.unlock();
}

iasync<void> thread_task::then(core::delegates::function<void(iasync<void>)> func)
{
	scope_locker<async::mutex_t> lock = _mutex;
	if (async_action_status::finished & _state)
		return null;

	if (_child_task.is_empty() || _child_task->_state == async_action_status::canceled)
		_child_task = new thread_task(_thread);

	thread_task_t task = _child_task;
	_child_task->action += func;

	if (_state == async_action_status::wait_for_then)
	{
		_thread->post_task(_child_task);
		_child_task = null;
		_thread = null; //unreference thread
		action.empty();
		_state = async_action_status::completed;
		_cond.signal();
	}

	return task.get();
}

bool thread_task::wait(async_action_status_t st)const
{
	if (!_thread.is_empty() && _thread->is_current_thread())
		return false;
	scope_locker<async::mutex_t> lock = _mutex;
	if (_state > st)return false;
	_cond.waitfor(_mutex, [&]() { return !(st & _state); });
	return true;
}

bool thread_task::wait(async_action_status_t st, dword ms)const
{
	if (_thread.is_empty() || _thread->is_current_thread())
		return false;
	dword last_time = (dword)(get_performance_time_us() / 1000.0);
	dword current = 0;

	scope_locker<async::mutex_t> lock = _mutex;
	if (_state > st)return false;
	while (!(st & _state))
	{
		_cond.wait(_mutex, ms);
		thread::sleep(1);
		current = (dword)(get_performance_time_us() / 1000);
		if (ms <= (current - last_time))
			break;
		else ms -= (current - last_time);
		last_time = current;
	}
	return st&_state;
}

async_action_status_t thread_task::status()const
{
	return _state;
}

bool thread_task::cancel()
{
	scope_locker<async::mutex_t> lock = _mutex;
	if (_thread.is_empty() || _state & (async_action_status::finished | async_action_status::attached))
		return false;

	_was_canceled = true;
	_state = async_action_status::canceled;
	action.empty();
	if (!_thread->is_current_thread())
		_cond.signal();
	_thread = null;
	return true;
}

void thread_task::result()const
{
	scope_locker<async::mutex_t> lock = _mutex;
	//	if (_thread.is_empty())
	//		return;

	if (!_thread.is_empty() && !_thread->is_current_thread()) _cond.waitfor(_mutex, [&]() ->bool
	{
		return _state <= async_action_status::running;//  !(async_action_status::finished | async_action_status::wait_for_then).is_active(state);
	});

	if (_was_canceled)
		throw_exception(except_code::operation_canceled);

	if (_state == async_action_status::completed)
		return;

	_state = async_action_status::completed;
	if (!_thread->is_current_thread())
		_cond.signal();
	const_cast<thread_task*>(this)->_thread = null;
	const_cast<thread_task*>(this)->action.empty();
}


