#ifndef __ANG_SINGLETON_H__
#error Can't include ang_ingleton.hpp, please include angsys.h inside
#elif !defined __ANG_SINGLETON_HPP__
#define __ANG_SINGLETON_HPP__

/////////////////////////////////////////////////////////////////////////////
template<class T>
T* ang::singleton<ang::object_wrapper<T>>::instance(T* ptr, bool write, bool del)
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
	return instance(null, false);
}

template<class T>
bool ang::singleton<ang::object_wrapper<T>>::release_instance()
{
	instance(null, true, true);
	return true;
}

template<class T>
bool ang::singleton<ang::object_wrapper<T>>::is_instanced()
{
	return (instance(null, false) != null);
}

template<class T>
ang::singleton<ang::object_wrapper<T>>::singleton()
{
	if (is_instanced())
		throw exception(except_code::two_singleton);
	instance(static_cast<T*>(this), true);
}

template<class T>
ang::singleton<ang::object_wrapper<T>>::~singleton()
{
	if (_instance != null)
		instance(null, true, false);
}

/////////////////////////////////////////////////////////////////////////////

template<class T>
T* ang::singleton<ang::object_wrapper<T>>::instance(T* ptr, bool write, bool del)
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
	return instance(null, false);
}

template<class T>
bool ang::singleton<T>::release_instance()
{
	instance(null, true, true);
	return true;
}

template<class T>
bool ang::singleton<T>::is_instanced()
{
	return (instance(null, false) != null);
}


template<class T>
ang::singleton<T>::singleton()
{
	if (is_instanced())
		throw exception(except_code::two_singleton);
	instance(static_cast<T*>(this), true, false);
}

template<class T>
ang::singleton<T>::~singleton()
{
	if(instance(null, false) == this)
		instance(null, true, false);
}


#endif//__ANG_SINGLETON_HPP__