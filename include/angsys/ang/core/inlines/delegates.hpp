#ifndef __ANG_DELEGATES_H__
#error Can't include ang_delegate.hpp, please include ang_delegate.h inside
#elif !defined __ANG_DELEGATES_HPP__
#define __ANG_DELEGATES_HPP__


template<typename return_t, typename... args_t>
inline ang::type_name_t ang::core::delegates::ifunction<return_t, args_t...>::class_name()
{
	static string className = string("ang::core::delegates::ifunction<") + args_list_type_name<return_t, args_t...>() + ">";
	return className->cstr();
}

template<typename... args_t>
inline ang::type_name_t ang::core::delegates::ifunction<void, args_t...>::class_name()
{
	static string className = string("ang::core::delegates::ifunction<void,") + args_list_type_name<args_t...>() + ">";
	return className->cstr();
}

template<typename return_t, typename... args_t>
inline ang::type_name_t ang::core::delegates::ifunction<return_t, args_t...>::object_name()const
{
	return class_name();
}

template<typename... args_t>
inline ang::type_name_t ang::core::delegates::ifunction<void, args_t...>::object_name()const
{
	return class_name();
}

template<typename return_t, typename... args_t>
inline ang::bool_t ang::core::delegates::ifunction<return_t, args_t...>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name();
}

template<typename... args_t>
inline ang::bool_t ang::core::delegates::ifunction<void, args_t...>::is_kind_of(ang::type_name_t name)const
{
	return name == class_name();
}

template<typename return_t, typename... args_t>
inline ang::bool_t ang::core::delegates::ifunction<return_t, args_t...>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (name == class_name())
	{
		*out = this;
		return true;
	}
	return false;
}

template<typename... args_t>
inline ang::bool_t ang::core::delegates::ifunction<void, args_t...>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (name == class_name())
	{
		*out = this;
		return true;
	}
	return false;
}

template<typename return_t, typename... args_t>
inline ang::type_name_t ang::core::delegates::function_data<return_t(args_t...)>::class_name()
{
	static string className = string("ang::core::delegates::function<") + type_name<return_t>() + "(" + args_list_type_name<args_t...>() + ")>";
	return className->cstr();
}

template<typename... args_t>
inline ang::type_name_t ang::core::delegates::function_data<void(args_t...)>::class_name()
{
	static string className = string("ang::core::delegates::function<void(") + args_list_type_name<args_t...>() + ")>";
	return className->cstr();
}

template<typename return_t, typename... args_t>
inline ang::type_name_t ang::core::delegates::function_data<return_t(args_t...)>::object_name()const
{
	return class_name();
}

template<typename... args_t>
inline ang::type_name_t ang::core::delegates::function_data<void(args_t...)>::object_name()const
{
	return class_name();
}

template<typename return_t, typename... args_t>
inline ang::bool_t ang::core::delegates::function_data<return_t(args_t...)>::is_kind_of(ang::type_name_t name)const
{
	return (name == class_name()
		|| ifunction<return_t, args_t...>::is_kind_of(name));
}


template<typename... args_t>
inline ang::bool_t ang::core::delegates::function_data<void(args_t...)>::is_kind_of(ang::type_name_t name)const
{
	return (name == class_name()
		|| ifunction<void, args_t...>::is_kind_of(name));
}


template<typename return_t, typename... args_t>
inline ang::bool_t ang::core::delegates::function_data<return_t(args_t...)>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (name == class_name())
	{
		*out = this;
		return true;
	}
	else if (ifunction<return_t, args_t...>::query_object(name, out))
		return true;
	
	return false;
}

template<typename... args_t>
inline ang::bool_t ang::core::delegates::function_data<void(args_t...)>::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (name == class_name())
	{
		*out = this;
		return true;
	}
	else if (ifunction<void, args_t...>::query_object(name, out))
		return true;

	return false;
}


template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::object_wrapper()
	: _ptr(null)
{

}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_data<return_t(args_t...)> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::~object_wrapper()
{
	clean();
}

template<typename return_t, typename... args_t>
void ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename return_t, typename... args_t>
inline void ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::clean_unsafe()
{
	_ptr = null;
}

template<typename return_t, typename... args_t>
inline ang::bool_t ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::is_empty()const
{
	return _ptr == null;
}

template<typename return_t, typename... args_t>
inline ang::core::delegates::function_data<return_t(args_t...)>* ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::get(void)const
{
	return _ptr;
}

template<typename return_t, typename... args_t>
inline void ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::set(ang::core::delegates::function_data<return_t(args_t...)>* ptr)
{
	core::delegates::function_data<return_t(args_t...)> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename return_t, typename... args_t>
ang::core::delegates::function_data<return_t(args_t...)>** ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::addres_of(void)
{
	return &_ptr;
}


template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>& ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename return_t, typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>& ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename return_t, typename... args_t>
ang::object_wrapper_ptr<ang::core::delegates::function_data<return_t(args_t...)>> ang::object_wrapper<ang::core::delegates::function_data<return_t(args_t...)>>::operator & (void)
{
	return this;
}


///////////////////////////////////////////////////////////////////////////


template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::object_wrapper()
	: _ptr(null)
{

}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_data<void(args_t...)> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::object_wrapper(std::nullptr_t const&)
	: object_wrapper()
{

}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::~object_wrapper()
{
	clean();
}

template<typename... args_t>
void ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

template<typename... args_t>
inline void ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::clean_unsafe()
{
	_ptr = null;
}

template<typename... args_t>
inline ang::bool_t ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::is_empty()const
{
	return _ptr == null;
}

template<typename... args_t>
inline ang::core::delegates::function_data<void(args_t...)>* ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::get(void)const
{
	return _ptr;
}

template<typename... args_t>
inline void ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::set(ang::core::delegates::function_data<void(args_t...)>* ptr)
{
	core::delegates::function_data<void(args_t...)> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

template<typename... args_t>
ang::core::delegates::function_data<void(args_t...)>** ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::addres_of(void)
{
	return &_ptr;
}


template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>& ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

template<typename... args_t>
ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>& ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>> const& other)
{
	set(other._ptr);
	return*this;
}

template<typename... args_t>
ang::object_wrapper_ptr<ang::core::delegates::function_data<void(args_t...)>> ang::object_wrapper<ang::core::delegates::function_data<void(args_t...)>>::operator & (void)
{
	return this;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>::listener()
{
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{	
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>::~listener()
{
	empty();
}

template<typename return_t, typename... args_t>
void ang::core::delegates::listener<return_t(args_t...)>::empty()
{
	functions.clean();
}

template<typename return_t, typename... args_t>
ang::bool_t ang::core::delegates::listener<return_t(args_t...)>::is_empty()const
{
	return functions.is_empty() || functions->is_empty();
}

template<typename return_t, typename... args_t>
ang::index_t ang::core::delegates::listener<return_t(args_t...)>::append(ang::core::delegates::function<return_t(args_t...)> func)
{
	functions += ang::move(func);
	return functions.get()->size() - 1;
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>& ang::core::delegates::listener<return_t(args_t...)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>& ang::core::delegates::listener<return_t(args_t...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename return_t, typename... args_t>
ang::core::delegates::listener<return_t(args_t...)>& ang::core::delegates::listener<return_t(args_t...)>::operator += (ang::core::delegates::function<return_t(args_t...)> func)
{
	append(ang::move(func));
	return *this;
}

template<typename return_t, typename... args_t>
ang::array<return_t> ang::core::delegates::listener<return_t(args_t...)>::operator() (args_t... args)const
{
	array<return_t> result;
	if (!is_empty())
	{
		result = ang::move(array<return_t>(functions->size()));

		function<return_t(args_t...)>func;

		for (index_t i = 0U; i < result->size(); ++i)
		{
			func = functions[i];
			result[i] = func(args...);
		}
			
		
	}
	return ang::move(result);
}

template<typename return_t, typename... args_t>
ang::array<return_t> ang::core::delegates::listener<return_t(args_t...)>::invoke(args_t... args)const
{
	array<return_t> result;
	if (!is_empty())
	{
		result = ang::move(array<return_t>(functions->size()));

		function<return_t(args_t...)>func;

		for (index_t i = 0U; i < result->size(); ++i)
		{
			func = functions[i];
			result[i] = func(args...);
		}


	}
	return ang::move(result);
}





template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>::listener()
{
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>::~listener()
{
	empty();
}

template<typename... args_t>
void ang::core::delegates::listener<void(args_t...)>::empty()
{
	functions.clean();
}

template<typename... args_t>
ang::bool_t ang::core::delegates::listener<void(args_t...)>::is_empty()const
{
	return functions.is_empty() || functions->is_empty();
}

template<typename... args_t>
ang::index_t ang::core::delegates::listener<void(args_t...)>::append(ang::core::delegates::function<void(args_t...)> func)
{
	functions += ang::move(func);
	return functions.get()->size() - 1;
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>& ang::core::delegates::listener<void(args_t...)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>& ang::core::delegates::listener<void(args_t...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename... args_t>
ang::core::delegates::listener<void(args_t...)>& ang::core::delegates::listener<void(args_t...)>::operator += (ang::core::delegates::function<void(args_t...)> func)
{
	append(ang::move(func));
	return *this;
}


template<typename... args_t>
void ang::core::delegates::listener<void(args_t...)>::invoke(args_t... args)const
{
	if (!is_empty())
	{
		foreach(functions, [&](function<void(args_t...)> const & func) {
			func(args...);
		});
	}
}

template<typename... args_t>
void ang::core::delegates::listener<void(args_t...)>::operator() (args_t... args)const
{
	if (!is_empty())
	{
		foreach(functions, [&](function<void(args_t...)> const & func) {
			func(args...);
		});
	}
}



#endif//__ANG_DELEGATES_HPP__
