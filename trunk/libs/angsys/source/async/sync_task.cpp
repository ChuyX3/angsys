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

//ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::async::sync);
//ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::sync, bean, isync);
//ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::sync, bean, isync);

//ang::rtti_t const& ang::intf_class_info<ang::core::async::sync>::class_info()
//{
//	static type_name_t name = "ang::core::async::sync"_sv;
//	static inherit_pack_info_t parents
//		= rtti_from_type<ang::core::async::sync>::types();
//	static rtti_t const& info = rtti::regist(name, gender::class_type, size_of<sync>(), align_of<sync>(), parents, &iintf::default_query_interface);
//	return info;
//}

/////////////////////////////////////////////////////////////////////////////

sync_task::sync_task()
{

}

sync_task::~sync_task()
{

}

//ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::async::sync_task);
//ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::async::sync_task);
//ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::async::sync_task, sync);

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