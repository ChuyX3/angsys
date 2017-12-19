#ifndef __ANG_ASYNC_HPP__
#error Can't include async.inl, please include ang_async.hpp inside
#elif !defined __ANG_ASYNC_INL__
#define __ANG_ASYNC_INL__

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#define NEW_ARGS(...) new(__VA_ARGS__, __FILE__, __LINE__)
#else
#define NEW new
#define NEW_ARGS(...) new(__VA_ARGS__)
#endif

template<typename T>
inline ang::type_name_t ang::core::async::ioperation<T>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::core::async::ioperation<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<typename T>
inline bool ang::core::async::ioperation<T>::is_inherited_of(ang::type_name_t name)
{
	return name == class_name();
}

template<typename T>
inline ang::type_name_t ang::core::async::ioperation<T>::object_name()const
{
	return class_name();
}

template<typename T>
inline bool ang::core::async::ioperation<T>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name();
}

template<typename T>
inline bool ang::core::async::ioperation<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::core::async::ioperation<T>*>(this);
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

template<typename T>
inline ang::type_name_t ang::core::async::iasync<T>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::core::async::iasync<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<typename T>
inline bool ang::core::async::iasync<T>::is_inherited_of(ang::type_name_t name)
{
	return name == class_name()
		|| itask::is_inherited_of(name);
}

template<typename T>
inline ang::type_name_t ang::core::async::iasync<T>::object_name()const
{
	return class_name();
}

template<typename T>
inline bool ang::core::async::iasync<T>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name()
		|| itask::is_kind_of(name);
}

template<typename T>
inline bool ang::core::async::iasync<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::core::async::iasync<T>*>(this);
		return true;
	}
	else if (ang::core::async::itask::query_object(name, out))
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////


template<typename T>
inline ang::type_name_t ang::core::async::task_handler<T>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::core::async::task_handler<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<typename T>
inline bool ang::core::async::task_handler<T>::is_inherited_of(ang::type_name_t name)
{
	return name == class_name()
		|| iasync<T>::is_inherited_of(name);
}

template<typename T>
inline ang::type_name_t ang::core::async::task_handler<T>::object_name()const
{
	return class_name();
}

template<typename T>
inline bool ang::core::async::task_handler<T>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name()
		|| iasync<T>::is_kind_of(name);
}

template<typename T>
inline bool ang::core::async::task_handler<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::core::async::task_handler<T>*>(this);
		return true;
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (iasync<T>::query_object(name, out))
	{
		return true;
	}
	return false;
}


template<typename T>
inline ang::core::async::task_handler<T>::task_handler()
	: _result()
	, _task(null)
{

}

template<typename T>
inline ang::core::async::task_handler<T>::task_handler(ang::core::async::itask* task)
	: _result()
	, _task(task)
{

}

template<typename T>
inline ang::core::async::task_handler<T>::~task_handler()
{
	if(!_task.is_empty())_task->join();
}

template<typename T>
inline bool ang::core::async::task_handler<T>::wait(ang::core::async::async_action_status_t status)const
{
	return _task.is_empty() ? false : _task->wait(status);
}

template<typename T>
inline bool ang::core::async::task_handler<T>::wait(ang::core::async::async_action_status_t status, dword ms)const
{
	return _task.is_empty() ? false : _task->wait(status, ms);
}

template<typename T>
inline ang::core::async::async_action_status_t ang::core::async::task_handler<T>::status()const
{
	return _task.is_empty() ? async_action_status::error : _task->status();
}

template<typename T>
inline bool ang::core::async::task_handler<T>::cancel()
{
	return _task.is_empty() ? false : _task->cancel();
}

template<typename T>
inline bool ang::core::async::task_handler<T>::join()const
{
	return _task.is_empty() ? false : _task->join();
}


template<typename T>
inline ang::core::async::itask_t ang::core::async::task_handler<T>::then(delegates::function<void(ang::core::async::iasync<T>*)> func)
{
	return _task.is_empty() ? null : _task->then([=](ang::core::async::itask*t){ func(this); });
}

template<typename T>
inline ang::core::async::itask_t ang::core::async::task_handler<T>::then(delegates::function<void(ang::core::async::itask*)> func)
{
	return _task.is_empty() ? null : _task->then(func);
}

template<typename T>
inline T ang::core::async::task_handler<T>::result()const
{
	if(_task.is_empty())
		throw_exception(except_code::invalid_access);
	_task->join();
	return _result;
}

template<typename T> inline ang::core::async::iasync_t<T> ang::core::async::task::run_async(ang::core::delegates::function<T(ang::core::async::iasync<T>*)> func) {
	return task_handler<T>::create_task(func);
}

template<typename T> inline ang::core::async::iasync_t<T> ang::core::async::task::run_async(ang::core::delegates::function<T(ang::core::async::iasync<T>*, ang::var_args_t)> func, ang::var_args_t args) {
	return task_handler<T>::create_task(func, args);
}


template<> inline ang::core::async::iasync_t<void> ang::core::async::task::run_async(ang::core::delegates::function<void(iasync<void>*)> func) {
	return static_cast<iasync<void>*>(task::run_async([func](itask *t) { func(static_cast<iasync<void>*>(t)); }).get());
}

template<> inline ang::core::async::iasync_t<void> ang::core::async::task::run_async(ang::core::delegates::function<void(iasync<void>*, var_args_t)> func, var_args_t args) {
	return static_cast<iasync<void>*>(task::run_async([func](itask *t, var_args_t a) { func(static_cast<iasync<void>*>(t), a); }, args).get());
}

template<typename T>
ang::core::async::iasync_t<T> ang::core::async::task_handler<T>::create_task(ang::core::delegates::function<T(iasync<T>*)> func)
{
	if (func.is_empty()) return null;
	task_handler_t<T> _async = NEW task_handler<T>();
	_async->_task = task::run_async([=](itask* t, var_args_t a) {
		_async.get()->_result = func(_async.get());
	}, { _async.get() });
	return _async;
}

template<typename T>
ang::core::async::iasync_t<T> ang::core::async::task_handler<T>::create_task(ang::core::delegates::function<T(iasync<T>*,var_args_t)> func, ang::var_args_t args)
{
	if (func.is_empty()) return null;
	task_handler_t<T> _async = NEW task_handler<T>();
	_async->_task = task::run_async([=](itask* t, var_args_t a) {
		_async.get()->_result = func(_async.get(), args);
	}, { _async.get(), args });
	return _async;
}

template<typename T> template<typename U>
ang::core::async::iasync_t<T> ang::core::async::task_handler<T>::handle_then(ang::core::async::iasync_t<U> task, ang::core::delegates::function<T(iasync<U>*)> func)
{
	if (func.is_empty()) return null;
	task_handler_t<T> _async = NEW task_handler<T>();
	_async->_task = task->then([=](iasync<U>* t) {
		_async.get()->_result = func(task.get());
	});
	return _async;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>::intf_wrapper() : _ptr(null) {

}

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {

}

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>::intf_wrapper(ang::core::async::iasync<result_t>* ptr) : _ptr(null) {
	set(ptr);
}
template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ang::core::async::iasync<result_t> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}
template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>::~intf_wrapper() {
	clean();
}

template<typename result_t>
inline void ang::intf_wrapper<ang::core::async::iasync<result_t>>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

template<typename result_t>
inline bool ang::intf_wrapper<ang::core::async::iasync<result_t>>::is_empty()const
{
	return _ptr == null;
}

template<typename result_t>
inline ang::core::async::iasync<result_t>* ang::intf_wrapper<ang::core::async::iasync<result_t>>::get(void)const
{
	return _ptr;
}

template<typename result_t>
inline void ang::intf_wrapper<ang::core::async::iasync<result_t>>::set(ang::core::async::iasync<result_t>* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>& ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator = (ang::core::async::iasync<result_t>* ptr)
{
	set(ptr);
	return*this;
}

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>& ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}


template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>& ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator = (ang::intf_wrapper<ang::core::async::iasync<result_t>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>& ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator = (ang::intf_wrapper<ang::core::async::iasync<result_t>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename result_t>
inline ang::core::async::iasync<result_t> ** ang::intf_wrapper<ang::core::async::iasync<result_t>>::addres_of(void)
{
	return &_ptr;
}

template<typename result_t>
inline ang::intf_wrapper_ptr<ang::core::async::iasync<result_t>> ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator & (void)
{
	return this;
}

template<typename result_t>
inline ang::core::async::iasync<result_t> * ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator -> (void)
{
	return get();
}

template<typename result_t>
inline ang::core::async::iasync<result_t> const* ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator -> (void)const
{
	return get();
}

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator ang::core::async::iasync<result_t> * (void)
{
	return get();
}

template<typename result_t>
inline ang::intf_wrapper<ang::core::async::iasync<result_t>>::operator ang::core::async::iasync<result_t> const* (void)const
{
	return get();
}

template<typename result_t>
template<typename then_result_t>
ang::core::async::iasync_t<then_result_t> ang::intf_wrapper<ang::core::async::iasync<result_t>>::then(
	ang::core::delegates::function<then_result_t(ang::core::async::iasync<result_t>*)> func)
{
	if (_ptr == null)
		return null;

	return core::async::task_handler<then_result_t>::template handle_then<result_t>(_ptr, [=](core::async::iasync<result_t>* task)->then_result_t 
	{
		return ang::move(func(task));
	});

}


#undef NEW
#undef NEW_ARGS

#endif//__ANG_ASYNC_INL__