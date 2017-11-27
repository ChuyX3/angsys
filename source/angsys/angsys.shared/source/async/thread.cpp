#include "pch.h"
#include "ang/core/async.hpp"
#include "thread.hpp"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

extern "C" ulong64 ang_get_performance_time();

ANG_IMPLEMENT_INTERFACE(ang::core::async, icore_thread);


thread_t thread::create_thread_suspended(uint flags, ibuffer_view_t buffer, bool alloc)
{
	return new core_thread(flags, buffer, alloc);;
}

thread_t thread::create_thread(thread_routine_t routine, var_args_t args, uint flags, ibuffer_view_t buffer, bool alloc)
{
	core_thread_t thread = new core_thread(flags, buffer, alloc);
	thread->start(routine, args);
	return thread.get();
}


void thread::sleep(dword ms)
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	usleep(ms * 1000);
#elif defined WINDOWS_PLATFORM
	Sleep(ms);
#endif
}
thread_t thread::main_thread()
{
	return core_thread_manager::instance()->main_thread().get();
}

thread_t thread::this_thread()
{
	return core_thread_manager::instance()->this_thread().get();
}

dword thread::this_thread_id()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return pthread_self();
#elif defined WINDOWS_PLATFORM
	return GetCurrentThreadId();
#endif
}

thread::thread() {}
thread::~thread() {}


ANG_IMPLEMENT_CLASSNAME(ang::core::async::thread);
ANG_IMPLEMENT_OBJECTNAME(ang::core::async::thread);

bool thread::is_inherited_of(type_name_t name)
{
	return name == class_name() ||
		object::is_inherited_of(name) ||
		icore_thread::is_inherited_of(name);
}

bool thread::is_kind_of(type_name_t name)const
{
	return name == class_name() ||
		object::is_kind_of(name) ||
		icore_thread::is_kind_of(name);
}

bool thread::query_object(type_name_t name, unknown_ptr_t out)
{
	if (out == null) return false;
	if (name == type_of(*this)) {

		*out = static_cast<thread*>(this);
		return true;
	}
	else if (object::query_object(name, out)) {
		return true;
	}
	else if (icore_thread::query_object(name, out)) {
		return true;
	}
	return false;
}


ulong64 core_thread_manager::hash_table_context_create_hash_number(dword key, ulong64 size)
{
	return (ulong64)((2475825 + key + 1) % size);
}

void core_thread_manager::hash_table_context_delete_data(ang_hash_table_pair_t pair)
{
	core_thread* thread = (core_thread*)(void*)pair.value;
	if (thread != NULL)
	{
		thread->release();
	}
}

core_thread_manager::core_thread_manager()
	: _thread_map()
	, _main_cond()
	, _main_mutex()
	, _main_thread(null)
{
	_main_thread = attach_this_thread(new core_thread(), true, null, false);
	_main_thread->add_ref();
	_thread_map.insert(core_thread::this_thread_id(), _main_thread.get());
}

core_thread_manager::~core_thread_manager()
{
	_main_mutex.lock();
	_thread_map.clean();
	_main_thread = null;
	_main_mutex.unlock();
}

core_thread_t core_thread_manager::attach_this_thread(core_thread_t thread, bool is_main, ibuffer_view_t data, bool alloc)
{
	if (thread.is_empty()) thread = new core_thread();
	thread->_state = async_action_status::attached; //initializing
	thread->_is_main = is_main;
	if (alloc && !data.is_empty()) {
		thread->_tle_data = new(data->buffer_size()) buffer();
		memcpy(thread->_tle_data->buffer_ptr(), data->buffer_ptr(), data->buffer_size());
	}
	else {
		thread->_tle_data = data;
	}
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	thread->_thread = pthread_self();
#elif defined WINDOWS_PLATFORM
	DuplicateHandle(GetCurrentProcess()
		, GetCurrentThread()
		, GetCurrentProcess()
		, &thread->_hthread
		, 0, FALSE, DUPLICATE_SAME_ACCESS);
	thread->_id = GetCurrentThreadId();
#endif
	return thread;
}

dword core_thread_manager::this_thread_id()
{
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	return (dword)pthread_self();
#elif defined WINDOWS_PLATFORM
	return GetCurrentThreadId();
#endif
}

core_thread_t core_thread_manager::main_thread()const
{
	return _main_thread;
}

core_thread_t core_thread_manager::this_thread()const
{
	core_thread_t thread = null;
	main_mutex().lock();
	if (!_thread_map.find(this_thread_id(), &thread))
	{
		thread = const_cast<core_thread_manager*>(this)->attach_this_thread(null, false, null, false).get();
		const_cast<core_thread_manager*>(this)->_thread_map.insert(this_thread_id(), thread.get());
	}
	thread->add_ref();
	main_mutex().unlock();
	return thread;
}

core_thread_t core_thread_manager::regist_thread(core_thread_t thread)
{
	core_thread_t result = null;
	main_mutex().lock();
	if (_thread_map.insert(thread->thread_id(), thread))
		result = thread;
	if (!result.is_empty())result->add_ref();
	main_mutex().unlock();
	return result;
}

core_thread_t core_thread_manager::unregist_thread(core_thread_t thread)
{
	core_thread_t result = null;
	main_mutex().lock();
	if (!_thread_map.remove(thread->thread_id(), &result))
		result = null;
	main_mutex().unlock();
	return result;
}


dword core_thread::core_thread_start_routine(pointer args)
{
	auto manager = core_thread_manager::instance();
	core_thread_t thread = (core_thread*)(args);

	manager->main_mutex().lock();
	thread->_state = async_action_status::wait_for_start; //wait for start routine
	manager->main_cond().signal();
	manager->main_mutex().unlock();
	thread->sleep(1);
	dword result = -1;
	do
	{
		manager->main_mutex().lock();
		while (thread->_start_routine.is_empty() && (thread->_state & async_action_status::finished))
			manager->main_cond().wait(manager->main_mutex());

		if (thread->_state & async_action_status::finished)//cancel
		{
			thread->_state = async_action_status::completed;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			return result;
		}

		auto routine = thread->_start_routine;
		auto user_args = thread->_user_args;
		thread->_start_routine = null;
		thread->_user_args = null;
		thread->_state = async_action_status::running; //running worker thread
		manager->main_cond().signal();
		manager->main_mutex().unlock();

		 routine(thread.get(), user_args);
		 result = 0;
		manager->main_mutex().lock();
		if (thread->_state & async_action_status::finished)//cancel
		{
			thread->_state = async_action_status::completed;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			return result;
		}
		else
		{
			thread->_state = async_action_status::wait_for_then;
			manager->main_cond().signal();
			manager->main_mutex().unlock();
			thread->sleep(1);
		}
	} while (true);
}


core_thread::core_thread()
	: _is_main(false)
	, _join_request(false)
{
	this->_state = async_action_status::initializing; //initializing
	this->_is_main = false;
	this->_tle_data = null;
}

core_thread::core_thread(wsize flags, ibuffer_view_t data, bool alloc)
	: _is_main(false)
	, _join_request(false)
{
	scope_locker lock = core_thread_manager::instance()->main_mutex();

	this->_state = async_action_status::initializing; //initializing
	this->_is_main = false;
	if (alloc && !data.is_empty()) {
		this->_tle_data = new(data->buffer_size()) buffer();
		memcpy(this->_tle_data->buffer_ptr(), data->buffer_ptr(), data->buffer_size());
	}
	else {
		this->_tle_data = data;
	}
#if defined ANDROID_PLATFORM || defined LINUX_PLATFORM
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, flags);
	pthread_create(&this->_thread, &attr, [](pointer args)->pointer { return (pointer)core_thread::core_thread_start_routine(args); }, this);
	pthread_attr_destroy(&attr);
	if (this->_thread == 0)
		throw_exception(except_code::invalid_memory);
#elif defined WINDOWS_PLATFORM
	this->_hthread = CreateThread(NULL, 0
		, [](pointer args)->dword { return core_thread::core_thread_start_routine(args); }
		, this, CREATE_SUSPENDED, &this->_id);
	SetThreadPriority(this->_hthread, flags);
	if (this->_hthread == null)
		throw_exception(except_code::invalid_memory);
	ResumeThread(this->_hthread);
#endif
	core_thread_manager::instance()->regist_thread(this);
}


core_thread::~core_thread()
{

}

//ANG_IMPLEMENT_BASIC_INTERFACE(ang::core::async::core_thread, ang::core::async::thread);

bool core_thread::is_main_thread()const { return _is_main; }

bool core_thread::is_current_thread()const {
	return thread_id() == core_thread_manager::this_thread_id();
}

ibuffer_view_t core_thread::tle_buffer()const { return _tle_data; }

void core_thread::set_tle_data(ibuffer_view_t data, bool alloc) {
	if (alloc && !data.is_empty()) {
		this->_tle_data = new(data->buffer_size()) buffer();
		memcpy(this->_tle_data->buffer_ptr(), data->buffer_ptr(), data->buffer_size());
	}
	else {
		this->_tle_data = data;
	}
}

void core_thread::set_tle_notify(tle_deleting_event_t callback) { tle_notify_callback = callback; }


dword core_thread::thread_id()const { 
#if defined WINDOWS_PLATFORM
	return _id;
#elif defined  ANDROID_PLATFORM || defined LINUX_PLATFORM
	return dword(_thread);
#endif
}

async_action_status_t core_thread::status()const { return _state; }

bool core_thread::start(delegates::function<void(icore_thread*, var_args_t)> callback, var_args_t args)
{
	mutex_t& mutex = core_thread_manager::instance()->main_mutex();
	cond_t& cond = core_thread_manager::instance()->main_cond();
	scope_locker lock = mutex;
	if (is_current_thread())
		return false;
	while (_state < async_action_status::wait_for_start)
		cond.wait(mutex);
	if (_state > async_action_status::wait_for_start)
		return false;
	_start_routine = callback;
	_user_args = args;
	cond.signal();
	return !_start_routine.is_empty();
}

bool core_thread::then(delegates::function<void(icore_thread*, var_args_t)> callback, var_args_t args)
{
	if (callback.is_empty() || _join_request)return false;
	cond_t& cond = core_thread_manager::instance()->main_cond();
	mutex_t& mutex = core_thread_manager::instance()->main_mutex();
	scope_locker lock = mutex;
	if ((_state > async_action_status::wait_for_then)
		|| (_state < async_action_status::running && _start_routine.is_empty()))
		return false;

	cond.waitfor(mutex, [this]()
	{
		return !_start_routine.is_empty();
	});

	if (_state > async_action_status::wait_for_then)
		return false;

	_start_routine = callback;
	_user_args = args;
	if (!is_current_thread()) {
		cond.signal();
	}
	return true;
}

bool core_thread::wait(async_action_status_t state)const
{
	if (is_current_thread())
		return false;
	scope_locker lock = core_thread_manager::instance()->main_mutex();
	if (_state > state)return false;
	core_thread_manager::instance()->main_cond().waitfor(core_thread_manager::instance()->main_mutex(), 
		[&]() { return !state.is_active(_state); });
	return true;
}

bool core_thread::wait(async_action_status_t state, dword ms)const
{
	if (is_current_thread())
		return false;

	dword last_time = (dword)(ang_get_performance_time() / 1000);
	dword current = 0;

	cond_t& cond = core_thread_manager::instance()->main_cond();
	mutex_t& mutex = core_thread_manager::instance()->main_mutex();
	scope_locker lock = mutex;
	if (_state > state)return false;
	while (!state.is_active(_state))
	{
		cond.wait(mutex, ms);
		sleep(1);
		current = (dword)(ang_get_performance_time() / 1000);
		if (ms <= (current - last_time))
			break;
		else ms -= (current - last_time);
		last_time = current;
	}
	return state.is_active(_state);
}

bool core_thread::cancel() 
{
	scope_locker lock = core_thread_manager::instance()->main_mutex();
	if (_state & async_action_status::finished)
		return false;
	_state = async_action_status::canceled;
	if (!is_current_thread())
		core_thread_manager::instance()->main_cond().signal();
	return true;
}

bool core_thread::join()const
{
	if (is_current_thread())
		return false;

	cond_t& cond = core_thread_manager::instance()->main_cond();
	mutex_t& mutex = core_thread_manager::instance()->main_mutex();
	scope_locker lock = mutex;
	_join_request = true;
	cond.waitfor(core_thread_manager::instance()->main_mutex(),
		[this]() { return _state == async_action_status::running || !_start_routine.is_empty(); });
	_state = async_action_status::completed;
	cond.signal();
	return true;
}