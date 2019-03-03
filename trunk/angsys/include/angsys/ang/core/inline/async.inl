#ifndef __ANG_CORE_ASYNC_H__
#error ...
#elif !defined __ANG_CORE_ASYNC_INL__
#define __ANG_CORE_ASYNC_INL__

template<typename T> 
inline ang::runtime::rtti_t const& ang::core::async::iaction<T>::class_info() {
	static const cstr_view<char> name = text::string_pool::instance()->save_string((string("ang::core::async::iaction<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<interface>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<wsize>(), align_of<wsize>(), parents, &default_query_interface);
	return info;
}

template<typename T>
inline ang::runtime::rtti_t const& ang::core::async::itask<T>::class_info() {
	static const cstr_view<char> name = text::string_pool::instance()->save_string((string("ang::core::async::itask<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<iaction<T>>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<wsize>(), align_of<wsize>(), parents, &default_query_interface);
	return info;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::core::async::iasync<T> ang::core::async::task::run_async(delegates::function<T(iasync<T>)> func)
{
	object_wrapper<task_handler<T>> _task = new task_handler<T>();
	_task->attach(run_async<void>([=](iasync<void> async)
	{
		_task.get()->done(func.get()->invoke(_task.get()));
	}));
	return _task;
}

template<typename T> template<typename U>
inline ang::core::async::iasync<U> ang::core::async::itask<T>::then(ang::core::delegates::function<U(ang::core::async::iasync<T>)> func)
{
	object_wrapper<task_handler<U>> handler = new task_handler<U>();
	iasync<T> this_ = this;
	handler->attach(then([=](iasync<T>) {
		handler.get()->done(func.get()->invoke(this_.get()));
	}));
	return handler.get();
}

template<typename T, typename...Ts>
inline ang::core::async::iasync<T> ang::core::async::idispatcher::run_async(ang::core::delegates::function<T(Ts...)> func, Ts...args)
{
	task_handler_ptr<T> handler = new task_handler<T>();
	iasync<T> this_ = this->run_async(function<void(iasync<void>)>([=](iasync<T>)
	{
		handler.get()->done(func.get()->invoke(forward<Ts>(args)...));
	}));

	return handler.get();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::core::async::task_handler<T>::task_handler()
	: _result(T())
	, _task(null)
{

}

template<typename T>
inline ang::core::async::task_handler<T>::task_handler(ang::core::async::iasync<void> handle)
	: _result(T())
	, _task(handle)
{

}

template<typename T>
inline ang::core::async::task_handler<T>::~task_handler()
{
	try { if(!_task.is_empty())_task->result(); }catch(...){}
	_task = null;
}

template<typename T>
inline ang::runtime::rtti_t const& ang::core::async::task_handler<T>::class_info() {
	static const cstr_view<char> name = text::string_pool::instance()->save_string((string("ang::core::async::task_handler<"_s) += rtti::type_of<T>().type_name()) += ">"_s);
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
	return _task->then([=](iasync<void> async)
	{
		func(this_.get());
	});
}

template<typename T>
inline bool ang::core::async::task_handler<T>::wait(async_action_status_t s)const
{
	return _task->wait(s);
}

template<typename T>
inline bool ang::core::async::task_handler<T>::wait(async_action_status_t s, dword ms)const
{
	return _task->wait(s, ms);
}

template<typename T>
inline ang::core::async::async_action_status_t ang::core::async::task_handler<T>::status()const
{
	return _task->status();
}

template<typename T>
inline bool ang::core::async::task_handler<T>::cancel()
{
	return _task->cancel();
}

template<typename T>
inline T ang::core::async::task_handler<T>::result()const
{
	_task->result();
	return _result;
}

template<typename T>
inline void ang::core::async::task_handler<T>::done(T const& res)
{
	_result = res;
}

template<typename T>
inline void ang::core::async::task_handler<T>::attach(ang::core::async::iasync<void> async)
{
	if (!_task.is_empty())
		throw_exception(except_code::invalid_access);
	_task = async;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif//__ANG_CORE_ASYNC_INL__