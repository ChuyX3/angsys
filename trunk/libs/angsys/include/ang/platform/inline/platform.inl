#ifndef __ANG_PLATFORM_H__
#error ...
#elif !defined __ANG_PLATFORM_INL__
#define __ANG_PLATFORM_INL__

//ang::events

template<typename F, typename A>
inline void ang::platform::events::static_event_function<F, A>::invoke(ang::objptr caller, ang::platform::events::imsg_event_args_t args)const
{
	m_function(caller, static_cast<A*>(args.get()));
}

template<typename F, typename A>
inline void ang::platform::events::static_event_function<F, ang::intf_wrapper<A>>::invoke(ang::objptr caller, ang::platform::events::imsg_event_args_t args)const
{
	m_function(caller, static_cast<A*>(args.get()));
}

template<typename T, bool IS_OBJECT, typename A>
inline void ang::platform::events::member_event_function<T, IS_OBJECT, A>::invoke(ang::objptr caller, ang::platform::events::imsg_event_args_t args)const
{
	(m_obj->*m_function)(caller, static_cast<A*>(args.get()));
}

template<typename T, typename A>
inline void ang::platform::events::member_event_function<T, true, A>::invoke(ang::objptr caller, ang::platform::events::imsg_event_args_t args)const
{
	auto lock = m_obj.lock();
	if (lock != null)(lock->*m_function)(caller, static_cast<A*>(args.get()));
}

#endif//__ANG_PLATFORM_INL__
