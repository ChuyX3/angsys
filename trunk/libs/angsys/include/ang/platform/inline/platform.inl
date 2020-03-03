#ifndef __COFFE_PLATFORM_H__
#error ...
#elif !defined __COFFE_PLATFORM_INL__
#define __COFFE_PLATFORM_INL__

//coffe::events

template<typename F, typename A>
inline void coffe::platform::events::static_event_function<F, A>::invoke(coffe::objptr caller, coffe::platform::events::imsg_event_args_t args)const
{
	m_function(caller, static_cast<A*>(args.get()));
}

template<typename F, typename A>
inline void coffe::platform::events::static_event_function<F, coffe::intf_wrapper<A>>::invoke(coffe::objptr caller, coffe::platform::events::imsg_event_args_t args)const
{
	m_function(caller, static_cast<A*>(args.get()));
}

template<typename T, bool IS_OBJECT, typename A>
inline void coffe::platform::events::member_event_function<T, IS_OBJECT, A>::invoke(coffe::objptr caller, coffe::platform::events::imsg_event_args_t args)const
{
	(m_obj->*m_function)(caller, static_cast<A*>(args.get()));
}

template<typename T, typename A>
inline void coffe::platform::events::member_event_function<T, true, A>::invoke(coffe::objptr caller, coffe::platform::events::imsg_event_args_t args)const
{
	auto lock = m_obj.lock();
	if (lock != null)(lock->*m_function)(caller, static_cast<A*>(args.get()));
}

#endif//__COFFE_PLATFORM_INL__
