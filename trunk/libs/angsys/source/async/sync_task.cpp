#include "pch.h"
#include "thread_manager.h"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::async::isync);
//ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::sync);

/////////////////////////////////////////////////////////////////////////////

void sync::synchronize(function<void(isync_t)>&& callback) 
{
	sync_task_t task = new sync_task();
	task->run(ang::forward<function<void(isync_t)>>(callback));
}

sync::sync()
{

}

sync::~sync()
{

}

/////////////////////////////////////////////////////////////////////////////

sync_task::sync_task()
{

}

sync_task::~sync_task()
{

}

void sync_task::run(function<void(isync_t)> callback)
{
	callback(this);

	scope_locker<mutex_t> lock = m_mutex;
	m_cond.waitfor(m_mutex, [&]() 
	{
		return !m_task_set.is_empty();
	});
}

void sync_task::run_sync(function<void(isync_t)> callback)
{
	scope_locker<mutex_t> lock = m_mutex;
	auto task = async::task::run_async<void>([=](iasync<void> task)
	{
		callback(this);
		scope_locker<mutex_t> lock = m_mutex;
		m_task_set.remove(task);
		m_cond.signal();
	});

	m_task_set.insert(ang::forward<iasync<void>>(task));
}

mutex_t& sync_task::shared_mutex()const
{
	return m_shared_mutex;
}

/////////////////////////////////////////////////////////////////////////////