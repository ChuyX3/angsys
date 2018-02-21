#ifndef __ANG_SINGLETON_H__
#error Can't include ang_ingleton.hpp, please include angsys.h inside
#elif !defined __ANG_SINGLETON_HPP__
#define __ANG_SINGLETON_HPP__

/////////////////////////////////////////////////////////////////////////////
template<class T> T*  ang::singleton<ang::object_wrapper<T>>::_instance = null;

template<class T>
T* ang::singleton<ang::object_wrapper<T>>::instance()
{
	if (_instance == null)
		new T();
	return static_cast<T*>(_instance);
}

template<class T>
bool ang::singleton<ang::object_wrapper<T>>::release_instance()
{
	if (_instance)
	{
		_instance->release();
	}
	_instance = null;
	return true;
}

template<class T>
bool ang::singleton<ang::object_wrapper<T>>::is_instanced()
{
	return (_instance != null);
}


template<class T>
ang::singleton<ang::object_wrapper<T>>::singleton()
{
	if (_instance != null)
		throw (exception_t(except_code::two_singleton));
	_instance = static_cast<T*>(this);
	_instance->add_ref();
}

template<class T>
ang::singleton<ang::object_wrapper<T>>::~singleton()
{
	if (_instance != null)//????
		_instance = null;
}


/////////////////////////////////////////////////////////////////////////////
template<class T> T*  ang::singleton<T>::_instance = null;

template<class T> 
T* ang::singleton<T>::instance()
{
	if(_instance == null)
		new T();
	return static_cast<T*>(_instance);
}

template<class T>
bool ang::singleton<T>::release_instance()
{
	if(_instance) delete _instance;
	return true;
}

template<class T>
bool ang::singleton<T>::is_instanced()
{
	return (_instance != null);
}


template<class T>
ang::singleton<T>::singleton()
{
	if(_instance != null)
		throw (exception_t(except_code::two_singleton) );
	_instance = static_cast<T*>(this);
}

template<class T>
ang::singleton<T>::~singleton()
{
	if(_instance == this)
		_instance = null;
}


#endif//__ANG_SINGLETON_HPP__