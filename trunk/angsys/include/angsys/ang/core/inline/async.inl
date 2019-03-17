#ifndef __ANG_CORE_ASYNC_H__
#error ...
#elif !defined __ANG_CORE_ASYNC_INL__
#define __ANG_CORE_ASYNC_INL__

template<typename T> 
inline ang::runtime::rtti_t const& ang::core::async::iaction<T>::class_info() {
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::core::async::iaction<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<intf>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<wsize>(), align_of<wsize>(), parents, &default_query_interface);
	return info;
}

template<typename T>
inline ang::runtime::rtti_t const& ang::core::async::itask<T>::class_info() {
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::core::async::itask<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<iaction<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<wsize>(), align_of<wsize>(), parents, &default_query_interface);
	return info;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::core::async::iasync<T> ang::core::async::task::run_async(delegates::function<T(iasync<T>)> func)
{
	object_wrapper<task_handler<T>> wrapper = new task_handler<T>();
	wrapper->attach(run_async<void>([=](iasync<void>)
	{
		wrapper.get()->done(func.get()->invoke(wrapper.get()));
	}));
	return wrapper;
}

namespace ang
{
	namespace core
	{
		namespace async
		{
			template<typename Task, typename T>
			struct __run_async_helper {
				static iasync<T> run(Task* task, ang::core::delegates::function<T(ang::core::async::iasync<T>)> func) {
					task_handler_ptr<T> wrapper = new task_handler<T>();
					wrapper->attach(task->run_async(function<void(iasync<void>)>([=](iasync<void>)
					{
						wrapper.get()->done(func.get()->invoke(wrapper.get()));
					})));
					return wrapper;
				}
			};

			template<typename Task>
			struct __run_async_helper<Task, void> {
				static iasync<void> run(Task* task, ang::core::delegates::function<void(ang::core::async::iasync<void>)> func) {
					return task->run_async(func);
				}
			};


			template<typename Task, typename T, typename U>
			struct __then_helper {
				static iasync<U> then(Task* task, ang::core::delegates::function<U(ang::core::async::iasync<T>)> func) {
					task_handler_ptr<U> wrapper = new task_handler<U>();
					wrapper->attach(task->then([=](iasync<T> task) {
						wrapper.get()->done(func.get()->invoke(ang::forward<iasync<T>>(task)));
					}));
					return wrapper.get();
				}
			};

			template<typename Task, typename T>
			struct __then_helper<Task, T,void> {
				static iasync<void> then(Task* task, ang::core::delegates::function<void(ang::core::async::iasync<T>)> func) {
					return task->then(func);
				}
			};
		}
	}
}

template<typename T> template<typename U>
inline ang::core::async::iasync<U> ang::core::async::itask<T>::then(ang::core::delegates::function<U(ang::core::async::iasync<T>)> func)
{
	return __then_helper<itask<T>, T, U>::then(this, forward<decltype(func)>(func));
}


template<typename T>
inline ang::core::async::iasync<T> ang::core::async::idispatcher::run_async(ang::core::delegates::function<T(ang::core::async::iasync<T>)> func)
{
	return __run_async_helper<idispatcher, T>::run(this, func);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::core::async::task_handler<T>::task_handler()
	: m_result(T())
	, m_task(null)
{

}

template<typename T>
inline ang::core::async::task_handler<T>::task_handler(ang::core::async::iasync<void> handle)
	: m_result(T())
	, m_task(handle)
{

}

template<typename T>
inline ang::core::async::task_handler<T>::~task_handler()
{
	try { if(!m_task.is_empty())m_task->result(); }catch(...){}
	m_task = null;
}

template<typename T>
inline ang::runtime::rtti_t const& ang::core::async::task_handler<T>::class_info() {
	static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::core::async::task_handler<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<object>(), &runtime::type_of<itask<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<task_handler<T>>(), align_of<task_handler<T>>(), parents, &default_query_interface);
	return info;
}

template<typename T>
inline ang::runtime::rtti_t const& ang::core::async::task_handler<T>::runtime_info()const {
	return task_handler<T>::class_info();
}

template<typename T>
inline bool ang::core::async::task_handler<T>::query_interface(ang::runtime::rtti_t const& id, ang::unknown_ptr_t out)
{
	if (id.type_id() == class_info().type_id())
	{
		if (out == null) return false;
		*out = static_cast<task_handler<T>*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == type_of<itask<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<itask<T>*>(this);
		return true;
	}
	else if (id.type_id() == type_of<iaction<T>>().type_id()) {
		if (out == null) return false;
		*out = static_cast<iaction<T>*>(this);
		return true;
	}
}


template<typename T>
inline ang::core::async::iasync<void> ang::core::async::task_handler<T>::then(delegates::function<void(iasync<T>)> func)
{
	iasync<T> this_ = this;
	return m_task->then([=](iasync<void>)
	{
		func.get()->invoke(this_.get());
		//this_ = null;
	});
}

template<typename T>
inline bool ang::core::async::task_handler<T>::wait(async_action_status_t s)const
{
	return m_task->wait(s);
}

template<typename T>
inline bool ang::core::async::task_handler<T>::wait(async_action_status_t s, dword ms)const
{
	return m_task->wait(s, ms);
}

template<typename T>
inline ang::core::async::async_action_status_t ang::core::async::task_handler<T>::status()const
{
	return m_task->status();
}

template<typename T>
inline bool ang::core::async::task_handler<T>::cancel()
{
	return m_task->cancel();
}

template<typename T>
inline T ang::core::async::task_handler<T>::result()const
{
	m_task->result();
	return ang::move(m_result);
}

template<typename T>
inline void ang::core::async::task_handler<T>::done(T&& res)
{
	m_result = ang::forward<T>(res);
}

template<typename T>
inline void ang::core::async::task_handler<T>::attach(ang::core::async::iasync<void> async)
{
	if (!m_task.is_empty())
		throw_exception(except_code::invalid_access);
	m_task = async;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif//__ANG_CORE_ASYNC_INL__