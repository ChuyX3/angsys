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


template<class T>
inline ang::type_name_t ang::core::async::iasync<T>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::core::async::iasync<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<class T>
inline bool ang::core::async::iasync<T>::is_child_of(ang::type_name_t name)
{
	return name == class_name()
		|| iasync_task::is_child_of(name);
}

template<class T>
inline ang::type_name_t ang::core::async::iasync<T>::object_name()const
{
	return class_name();
}

template<class T>
inline bool ang::core::async::iasync<T>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name()
		|| iasync_task::is_kind_of(name);
}

template<class T>
inline bool ang::core::async::iasync<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::core::async::iasync<T>*>(this);
		return true;
	}
	else if (ang::core::async::iasync_task::query_object(name, out))
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////


template<class T>
inline ang::type_name_t ang::core::async::async_task_result<T>::class_name()
{
	static type_name_t name = runtime_data_base::regist_typename(ang::move(("ang::core::async::async_task_result<"_o += type_of<T>()) += ">"_s));
	return name;
}

template<class T>
inline bool ang::core::async::async_task_result<T>::is_child_of(ang::type_name_t name)
{
	return name == class_name()
		|| iasync<T>::is_child_of(name);
}

template<class T>
inline ang::type_name_t ang::core::async::async_task_result<T>::object_name()const
{
	return class_name();
}

template<class T>
inline bool ang::core::async::async_task_result<T>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name()
		|| iasync<T>::is_kind_of(name);
}

template<class T>
inline bool ang::core::async::async_task_result<T>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (name == class_name())
	{
		*out = static_cast<ang::core::async::async_task_result<T>*>(this);
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


template<class T>
inline ang::core::async::async_task_result<T>::async_task_result(ang::core::async::async_task* task)
	: _result()
	, _task(task)
{

}

//template<class T>
//inline ang::core::async::async_task_result<T>::async_task_result(ang::core::async::thread_t _t, ang::core::async::mutex_t _m, ang::core::async::cond_t _c)
//	: _result()
//	, _cond(_c)
//	, _mutex(_m)
//	, _was_canceled(false)
//	, _handled(false)
//	, _thread(_t)
//	, _status(ang::core::async::async_action_status::starting)
//{
//
//}

template<class T>
inline ang::core::async::async_task_result<T>::~async_task_result()
{
	scope_locker lock = _mutex;
	if (_status == async_action_status::wait_for_then && !_thread.is_empty())
	{
		_status = async_action_status::completed;
		_thread->cancel();
	}
}

//template<class T>
//inline void ang::core::async::async_task_result<T>::complete()
//{
//	if (_thread.is_empty() || !_thread->is_current_thread())
//		throw(ang::exception_t(ang::except_code::invalid_access));
//
//	scope_locker locker = _mutex;
//	_was_canceled = true;
//	_status = async_action_status::completed;
//	_cond->signal();
//	if (!_then.is_empty())
//	{
//		_then(this);
//	}
//}

template<class T>
inline void ang::core::async::async_task_result<T>::complete(T value)
{
	if (_thread.is_empty() || !_thread->is_current_thread())
		throw(ang::exception_t(ang::except_code::invalid_access));

	scope_locker locker = _mutex;
	//_status == async_action_status::canceled;
	_result = value;
	_status = async_action_status::wait_for_then;
	_cond->signal();
	//if (!_then.is_empty())
	//{
	//	_then(this);
	//}
}

template<class T>
inline bool ang::core::async::async_task_result<T>::wait(ang::core::async::async_action_status_t status, dword ms)const
{
	async_task_result_t<T> prevent_destruction = const_cast<async_task_result<T>*>(this);
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

template<class T>
inline ang::core::async::async_action_status_t ang::core::async::async_task_result<T>::status()const
{
	return _status;
}

template<class T>
inline bool ang::core::async::async_task_result<T>::cancel()
{
	scope_locker locker = _mutex;
	_status = async_action_status::canceled;
	_cond.signal();
	/*async_action_status_t status = async_action_status::initializing
		+ async_action_status::running;

	if (status.is_active(_status))
	{
		_status = async_action_status::canceled;
		return true;
	}*/
	return true;
}


template<class T>
inline T ang::core::async::async_task_result<T>::result()const
{
	scope_locker lock = _mutex;
	if (_thread.is_empty()) { throw(ang::exception_t(ang::except_code::invalid_access)); }
	async_task_result_t<T> _this = const_cast<async_task_result<T>*>(this);

	if(!_thread->is_current_thread()){
		//_cond.waitfor(_mutex, [this]() { return _status < async_action_status::wait_for_then; });
		if (_status == async_action_status::canceled) { throw(ang::exception_t(ang::except_code::operation_canceled)); }
		//if (_status >= async_action_status::completed) { throw(ang::exception_t(ang::except_code::expired_object)); }
		_status = async_action_status::completed;
		_thread->join();
		//else if (_status == async_action_status::wait_for_then) { 
		//	_status = async_action_status::completed;
		//}
	}
	//else if (_status == async_action_status::canceled) { throw(ang::exception_t(ang::except_code::operation_canceled)); }
	return _result;
}

template<class T>
inline ang::core::async::iasync_task_t ang::core::async::async_task_result<T>::then(delegates::function<void(ang::core::async::iasync<T>*)> func, ang::core::async::iasync_task_t then_task)
{
	scope_locker lock = _mutex;
	if (_thread.is_empty()) { throw(ang::exception_t(ang::except_code::invalid_access)); }
	else if (_status < async_action_status::running) { throw(ang::exception_t(ang::except_code::invalid_access)); }
	else if (_status > async_action_status::wait_for_then) { throw(ang::exception_t(ang::except_code::expired_object)); }
	else
	{
		async_task_result_t<T> _this = this;
		_status = async_action_status::completed;
		_thread->then([=](thread_t, var_args_t)->dword
		{
			func(_this.get());
		
			return 0;
		}, null);
	}
	return then_task.get();
}

template<typename T>
ang::core::async::iasync_t<T> ang::core::async::async_task<T>::run_async(ang::core::delegates::function<T(iasync<T>*,var_args_t)> func, ang::var_args_t args)
{
	if (_func.is_empty())
		return null;

	async_task_result_t<T> _async = NEW async_task_result<T>();

	if (_async->_thread.is_empty())	
		_async->_thread = thread::create_thread_suspended(0, null, false);
	
	delegates::function<dword(thread_t, var_args_t)> callback = [=](thread_t t, var_args_t a)->dword {
		_async->_mutex.lock();
		_async->_status = async_action_status::running;
		_async->_cond->signal();
		_async->_mutex.unlock();
		_async->complete(func(_async, args));
		return 0;
	};

	if ((_async->_thread->thread_state() > async_action_status::wait_for_start) ?
		!_async->_thread->then(callback, { _async.get(), func.get(), args.get() }) :
		!_async->_thread->start(callback, { _async.get(), func.get(), args.get() }))
		return null;
	return _async;
}


template<typename T>
ang::core::async::iasync_t<T> ang::core::async::async_task<T>::run_async(ang::core::async::async_task_result<T>* task, ang::core::delegates::function<T(iasync<T>*, var_args_t)> func, ang::var_args_t args)
{
	async_task_result_t<T> _async = task;
	if (_func.is_empty() || _async.is_empty())
		return null;

	if (_async->_thread.is_empty())
		_async->_thread = thread::create_thread_suspended(0, null, false);

	delegates::function<dword(thread_t, var_args_t)> callback = [=](thread_t t, var_args_t a)->dword {
		_async->_mutex.lock();
		_async->_status = async_action_status::running;
		_async->_cond->signal();
		_async->_mutex.unlock();
		_async->complete(func(_async, args));
		return 0;
	};

	if ((_async->_thread->thread_state() > async_action_status::wait_for_start) ?
		!_async->_thread->then(callback, { _async.get(), func.get(), args.get() }) :
		!_async->_thread->start(callback, { _async.get(), func.get(), args.get() }))
		return null;
	return _async;
}


template<typename T>
ang::core::async::iasync_t<T> ang::core::async::async_task<T>::run_sync(ang::core::async::async_task_result<T>* task, ang::core::delegates::function<T(iasync<T>*, var_args_t)> func, ang::var_args_t args)
{
	async_task_result_t<T> _task = task;
	if (func.is_empty())
		return null;

	task->_mutex->lock();
	task->_thread = thread::this_thread();
	task->_status = async_action_status::running;
	task->_cond->signal();
	task->_mutex->unlock();
	task->complete(func(task, args));

	return _task.get();
}


//template<typename T>
//ang::core::async::iasync_t<T> ang::core::async::dispatcher_thread::run_async(ang::core::delegates::function<T(ang::core::async::iasync<T>*, ang::var_args_t)> _func, ang::var_args_t _args)
//{
//	if (_func.is_empty())
//		return null;
//
//	auto args = _args.get();
//	if (args)args->add_ref();
//
//	auto func = _func.get();
//	func->add_ref();
//
//	async_task_result_t<T> _task = NEW async_task_result<T>();
//	async_task_result<T>* task = _task.get();
//	task->add_ref();
//	task->_thread = this;
//	thread_callback_t callback = [=](pointer)->dword {
//		if (task->_status == async_action_status::canceled)
//		{
//			task->complete();
//			func->release();
//			if (args)args->release();
//			task->release();
//			return -1;
//		}
//		else
//		{
//			task->_status = async_action_status::running;
//			task->_cond->signal();
//
//			task->complete(func->invoke(task, args));
//
//			func->release();
//			if (args)args->release();
//			task->release();
//			return 0U;
//		}
//	};
//
//	post_task(callback);
//	return _task.get();
//}
//
//
//template<typename T, class... Ts>
//ang::core::async::iasync_t<T> ang::core::async::dispatcher_thread::run_async(ang::core::delegates::function<T(ang::core::async::iasync<T>*, ang::var_args_t)> _func, Ts const&... args) {
//	return run_async(ang::move(_func), var_args_t( args... ));
//}
//
//


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
	ang::core::async::async_task_result_t<then_result_t> _then_task = NEW ang::core::async::async_task_result<then_result_t>();
	ang::core::async::async_task_result<then_result_t> *then_task = _then_task.get();
	then_task->add_ref();
	try {
		_ptr->then([=](ang::core::async::iasync<result_t>* async)
		{
			ang::core::async::async_task<then_result_t>::run_sync(then_task, [&](ang::core::async::iasync<then_result_t>* _task, var_args_t)
			{
				return func(async);
			});
			then_task->release();
		});
	}
	catch (exception_t)
	{
		return null;
	}
	
	return ang::move(_then_task);
}


#undef NEW
#undef NEW_ARGS

#endif//__ANG_ASYNC_INL__