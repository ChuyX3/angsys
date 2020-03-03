#ifndef __COFFE_CORE_ASYNC_H__
#error ...
#elif !defined __COFFE_CORE_ASYNC_INL__
#define __COFFE_CORE_ASYNC_INL__

namespace coffe
{
	template<typename T>
	struct intf_class_info<core::async::iaction<T>> {
		inline static rtti_t const& class_info() {
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<core::async::iaction<T>>::types();
			static coffe::rtti_t const& info = coffe::rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "coffe::core::async::iaction<"_sv << coffe::type_of<T>().type_name() << ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, size_of<wsize>(), align_of<wsize>(), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<core::async::itask<T>> {
		inline static rtti_t const& class_info() {
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<core::async::itask<T>>::types();
			static coffe::rtti_t const& info = coffe::rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "coffe::core::async::itask<"_sv << coffe::type_of<T>().type_name() << ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, size_of<wsize>(), align_of<wsize>(), parents, &iintf::default_query_interface);
			return info;
		}
	};

	template<typename T>
	struct intf_class_info<core::async::task_handler<T>> {
		inline static rtti_t const& class_info() {
			static inherit_pack_info_t parents
				= coffe::rtti_from_type<core::async::task_handler<T>>::types();
			static coffe::rtti_t const& info = coffe::rtti::regist([]()->fast_astring_t
			{
				fast_astring_t out;
				out << "coffe::core::async::task<"_sv << coffe::type_of<T>().type_name() << ">"_sv;
				return coffe::move(out);
			}(), gender::class_type, size_of<wsize>(), align_of<wsize>(), parents, &iintf::default_query_interface);
			return info;
		}
	};
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline coffe::core::async::iasync<T> coffe::core::async::task::run_async(delegates::function<T(iasync<T>)> func)
{
	object_wrapper<task_handler<T>> wrapper = new task_handler<T>();
	wrapper->attach(run_async<void>([=](iasync<void>)
	{
		wrapper->done(func(wrapper.get()));
	}));
	return wrapper.get();
}

namespace coffe
{
	namespace core
	{
		namespace async
		{
			template<typename Task, typename T>
			struct __run_async_helper {
				static iasync<T> run(Task* task, coffe::function<T(coffe::core::async::iasync<T>)> func) {
					task_handler_ptr<T> wrapper = new task_handler<T>();
					wrapper->attach(task->run_async(function<void(iasync<void>)>([=](iasync<void>)
					{
						wrapper->done(func(wrapper.get()));
					})));
					return wrapper.get();
				}
			};

			template<typename Task>
			struct __run_async_helper<Task, void> {
				static iasync<void> run(Task* task, coffe::function<void(coffe::core::async::iasync<void>)> func) {
					return task->run_async(func);
				}
			};


			template<typename Task, typename T, typename U>
			struct __then_helper {
				static iasync<U> then(Task* task, coffe::function<U(coffe::core::async::iasync<T>)> func) {
					task_handler_ptr<U> wrapper = new task_handler<U>();
					wrapper->attach(task->then([=](iasync<T> task) {
						wrapper->done(func(coffe::forward<iasync<T>>(task)));
					}));
					return wrapper.get();
				}
			};

			template<typename Task, typename T>
			struct __then_helper<Task, T,void> {
				static iasync<void> then(Task* task, coffe::function<void(coffe::core::async::iasync<T>)> func) {
					return task->then(func);
				}
			};
		}
	}
}

template<typename T> template<typename U>
inline coffe::core::async::iasync<U> coffe::core::async::itask<T>::then(coffe::function<U(coffe::core::async::iasync<T>)> func)
{
	return __then_helper<itask<T>, T, U>::then(this, forward<decltype(func)>(func));
}


template<typename T>
inline coffe::core::async::iasync<T> coffe::core::async::idispatcher::run_async(coffe::function<T(coffe::core::async::iasync<T>)> func)
{
	return __run_async_helper<idispatcher, T>::run(this, func);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline coffe::core::async::task_handler<T>::task_handler()
	: m_result(T())
	, m_task(null)
{

}

template<typename T>
inline coffe::core::async::task_handler<T>::task_handler(coffe::core::async::iasync<void> handle)
	: m_result(T())
	, m_task(handle)
{

}

template<typename T>
inline coffe::core::async::task_handler<T>::~task_handler()
{
	try { if(!m_task.is_empty())m_task->result(); }catch(...){}
	m_task = null;
}

template<typename T>
inline coffe::core::async::iasync<void> coffe::core::async::task_handler<T>::then(delegates::function<void(iasync<T>)> func)
{
	iasync<T> this_ = this;
	return m_task->then([=](iasync<void>)
	{
		func(this_.get());
		//this_ = null;
	});
}

template<typename T>
inline bool coffe::core::async::task_handler<T>::wait(async_action_status_t s)const
{
	return m_task->wait(s);
}

template<typename T>
inline bool coffe::core::async::task_handler<T>::wait(async_action_status_t s, dword ms)const
{
	return m_task->wait(s, ms);
}

template<typename T>
inline coffe::core::async::async_action_status_t coffe::core::async::task_handler<T>::status()const
{
	return m_task->status();
}

template<typename T>
inline bool coffe::core::async::task_handler<T>::cancel()
{
	return m_task->cancel();
}

template<typename T>
inline T coffe::core::async::task_handler<T>::result()const
{
	m_task->result();
	return coffe::move(m_result);
}

template<typename T>
inline void coffe::core::async::task_handler<T>::done(T&& res)const
{
	m_result = coffe::forward<T>(res);
}

template<typename T>
inline void coffe::core::async::task_handler<T>::attach(coffe::core::async::iasync<void> async)
{
	if (!m_task.is_empty())
		throw_exception(error_code::invalid_access);
	m_task = async;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>::intf_wrapper()
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>::intf_wrapper(coffe::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>::intf_wrapper(coffe::core::async::iaction<T>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>::intf_wrapper(intf_wrapper && other)
	: m_ptr(null)
{
	coffe::core::async::iaction<T> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>::intf_wrapper(intf_wrapper const& other)
	: m_ptr(null)
{
	set(other.m_ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>::~intf_wrapper() {
	reset();
}

template<typename T> inline
void coffe::intf_wrapper<coffe::core::async::iaction<T>>::reset()
{
	iobject * _obj = coffe::interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::core::async::iaction<T>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T> inline
bool coffe::intf_wrapper<coffe::core::async::iaction<T>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T> inline
coffe::core::async::iaction<T>* coffe::intf_wrapper<coffe::core::async::iaction<T>>::get(void)const
{
	return m_ptr;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::core::async::iaction<T>>::set(coffe::core::async::iaction<T>* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = coffe::interface_cast<iobject>(m_ptr);
	iobject * _new = coffe::interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>& coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator = (coffe::core::async::iaction<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>& coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator = (coffe::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>& coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator = (coffe::intf_wrapper<coffe::core::async::iaction<T>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>& coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator = (coffe::intf_wrapper<coffe::core::async::iaction<T>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T> inline
coffe::core::async::iaction<T> ** coffe::intf_wrapper<coffe::core::async::iaction<T>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T> inline
coffe::intf_wrapper_ptr<coffe::core::async::iaction<T>> coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator & (void)
{
	return this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator coffe::intfptr()const
{
	return static_interface_cast(get());
}

template<typename T> inline
coffe::core::async::iaction<T>* coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator -> (void)
{
	return get();
}

template<typename T> inline
coffe::core::async::iaction<T> const* coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator -> (void)const
{
	return get();
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::iaction<T>>::operator coffe::nullable<T>()const
{
	return is_empty() ? nullable<T>(null) : nullable<T>(get()->result());
}

template<> inline
coffe::intf_wrapper<coffe::core::async::iaction<void>>::operator coffe::nullable<void>()const
{
	if (is_empty())
		return null;
	else {
		get()->result();
		return new bean(); //void object
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>::intf_wrapper()
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>::intf_wrapper(coffe::nullptr_t const&)
	: m_ptr(null)
{
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>::intf_wrapper(coffe::core::async::itask<T>* ptr)
	: m_ptr(null)
{
	set(ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>::intf_wrapper(intf_wrapper && other)
	: m_ptr(null)
{
	coffe::core::async::itask<T> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>::intf_wrapper(intf_wrapper const& other)
	: m_ptr(null)
{
	set(other.m_ptr);
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>::~intf_wrapper() {
	reset();
}

template<typename T> inline
void coffe::intf_wrapper<coffe::core::async::itask<T>>::reset()
{
	iobject * _obj = coffe::interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::core::async::itask<T>>::reset_unsafe()
{
	m_ptr = null;
}

template<typename T> inline
bool coffe::intf_wrapper<coffe::core::async::itask<T>>::is_empty()const
{
	return m_ptr == null;
}

template<typename T> inline
coffe::core::async::itask<T>* coffe::intf_wrapper<coffe::core::async::itask<T>>::get(void)const
{
	return m_ptr;
}

template<typename T> inline
void coffe::intf_wrapper<coffe::core::async::itask<T>>::set(coffe::core::async::itask<T>* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = coffe::interface_cast<iobject>(m_ptr);
	iobject * _new = coffe::interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>& coffe::intf_wrapper<coffe::core::async::itask<T>>::operator = (coffe::core::async::itask<T>* ptr)
{
	set(ptr);
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>& coffe::intf_wrapper<coffe::core::async::itask<T>>::operator = (coffe::nullptr_t const&)
{
	reset();
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>& coffe::intf_wrapper<coffe::core::async::itask<T>>::operator = (coffe::intf_wrapper<coffe::core::async::itask<T>> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>& coffe::intf_wrapper<coffe::core::async::itask<T>>::operator = (coffe::intf_wrapper<coffe::core::async::itask<T>> const& other)
{
	set(other.m_ptr);
	return*this;
}

template<typename T> inline
coffe::core::async::itask<T> ** coffe::intf_wrapper<coffe::core::async::itask<T>>::addres_of(void)
{
	return &m_ptr;
}

template<typename T> inline
coffe::intf_wrapper_ptr<coffe::core::async::itask<T>> coffe::intf_wrapper<coffe::core::async::itask<T>>::operator & (void)
{
	return this;
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>::operator coffe::intfptr()const
{
	return static_interface_cast(get());
}

template<typename T> inline
coffe::core::async::itask<T>* coffe::intf_wrapper<coffe::core::async::itask<T>>::operator -> (void)
{
	return get();
}

template<typename T> inline
coffe::core::async::itask<T> const* coffe::intf_wrapper<coffe::core::async::itask<T>>::operator -> (void)const
{
	return get();
}

template<typename T> inline
coffe::intf_wrapper<coffe::core::async::itask<T>>::operator coffe::nullable<T>()const
{
	return is_empty() ? nullable<T>(null) : nullable<T>(get()->result());
}

template<> inline
coffe::intf_wrapper<coffe::core::async::itask<void>>::operator coffe::nullable<void>()const
{
	if (is_empty())
		return null;
	else {
		get()->result();
		return new bean(); //void object
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif//__COFFE_CORE_ASYNC_INL__
