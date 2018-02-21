#ifndef __ANG_SINGLETON_HPP__
#error Can't include ang_ingleton.hpp, please include angsys.h inside
#elif !defined __ANG_SINGLETON_INL__
#define __ANG_SINGLETON_INL__

/////////////////////////////////////////////////////////////////////////////
template<class T>
T* ang::singleton<ang::object_wrapper<T>>::instance_manager(T* ptr, bool write, bool del)
{
	static ang::object_wrapper<T> _instance = null;
	if (write) {
		if (!del) _instance.clean_unsafe();
		_instance = ptr;
	}
	return _instance.get();
}

template<class T>
T* ang::singleton<ang::object_wrapper<T>>::instance()
{
	if (!is_instanced())
		new T();
	return instance_manager(null, false);
}

template<class T>
bool ang::singleton<ang::object_wrapper<T>>::release_instance()
{
	instance_manager(null, true, true);
	return true;
}

template<class T>
bool ang::singleton<ang::object_wrapper<T>>::is_instanced()
{
	return (instance_manager(null, false) != null);
}

template<class T>
ang::singleton<ang::object_wrapper<T>>::singleton()
{
	if (is_instanced())
		throw exception_t(except_code::two_singleton);
	instance_manager(static_cast<T*>(this), true);
}

template<class T>
ang::singleton<ang::object_wrapper<T>>::~singleton()
{
	if (instance_manager(null, false, false) == this)
		instance_manager(null, true, false);
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
T* ang::singleton<T>::instance_manager(T* ptr, bool write, bool del)
{
	static T* _instance = null;
	if (write) {
		if (_instance && del) delete _instance;
		_instance = ptr;
	}
	return _instance;
}

template<class T> 
T* ang::singleton<T>::instance()
{
	if (!is_instanced())
		new T();
	return instance_manager(null, false);
}

template<class T>
bool ang::singleton<T>::release_instance()
{
	instance_manager(null, true, true);
	return true;
}

template<class T>
bool ang::singleton<T>::is_instanced()
{
	return (instance_manager(null, false) != null);
}


template<class T>
ang::singleton<T>::singleton()
{
	if (is_instanced())
		throw exception_t(except_code::two_singleton);
	instance_manager(static_cast<T*>(this), true, false);
}

template<class T>
ang::singleton<T>::~singleton()
{
	if(instance_manager(null, false) == this)
		instance_manager(null, true, false);
}


#endif//__ANG_SINGLETON_INL__