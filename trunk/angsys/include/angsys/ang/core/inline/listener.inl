#ifndef __CORE_LISTENER_H__
#error ...
#elif !defined __CORE_LISTENER_INL__
#define __CORE_LISTENER_INL__


template<typename T, typename... Ts>
ang::core::delegates::listener<T(Ts...)>::listener()
{
}

template<typename T, typename... Ts>
ang::core::delegates::listener<T(Ts...)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{
}

template<typename T, typename... Ts>
ang::core::delegates::listener<T(Ts...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename T, typename... Ts>
ang::core::delegates::listener<T(Ts...)>::~listener()
{
	empty();
}

template<typename T, typename... Ts>
void ang::core::delegates::listener<T(Ts...)>::empty()
{
	functions.clear();
}

template<typename T, typename... Ts>
bool ang::core::delegates::listener<T(Ts...)>::is_empty()const
{
	return functions.is_empty() || functions->is_empty();
}

template<typename T, typename... Ts>
index ang::core::delegates::listener<T(Ts...)>::push(ang::core::delegates::function<T(Ts...)> func)
{
	functions += ang::move(func);
	return functions.get()->size() - 1;
}

template<typename T, typename... Ts>
ang::core::delegates::listener<T(Ts...)>& ang::core::delegates::listener<T(Ts...)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}

template<typename T, typename... Ts>
ang::core::delegates::listener<T(Ts...)>& ang::core::delegates::listener<T(Ts...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename T, typename... Ts>
ang::core::delegates::listener<T(Ts...)>& ang::core::delegates::listener<T(Ts...)>::operator += (ang::core::delegates::function<T(Ts...)> func)
{
	push(ang::move(func));
	return *this;
}

template<typename T, typename... Ts>
ang::collections::ienum_t<T> ang::core::delegates::listener<T(Ts...)>::operator() (Ts... args)const
{
	collections::vector<T> result;
	if (!is_empty())
	{
		for (function<T(Ts...)>func : functions)
			result += func(args...);
	}
	return result.get();
}

template<typename T, typename... Ts>
ang::collections::ienum_t<T> ang::core::delegates::listener<T(Ts...)>::invoke(Ts... args)const
{
	collections::vector<T> result;
	if (!is_empty())
	{
		for (function<T(Ts...)>func : functions)
			result += func(args...);
	}
	return result.get();
}





template<typename... Ts>
ang::core::delegates::listener<void(Ts...)>::listener()
{
}

template<typename... Ts>
ang::core::delegates::listener<void(Ts...)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{
}

template<typename... Ts>
ang::core::delegates::listener<void(Ts...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename... Ts>
ang::core::delegates::listener<void(Ts...)>::~listener()
{
	empty();
}

template<typename... Ts>
void ang::core::delegates::listener<void(Ts...)>::empty()
{
	functions.clear();
}

template<typename... Ts>
bool ang::core::delegates::listener<void(Ts...)>::is_empty()const
{
	return functions.is_empty() || functions->is_empty();
}

template<typename... Ts>
index ang::core::delegates::listener<void(Ts...)>::push(ang::core::delegates::function<void(Ts...)> func)
{
	functions += ang::move(func);
	return functions.get()->size() - 1;
}

template<typename... Ts>
ang::core::delegates::listener<void(Ts...)>& ang::core::delegates::listener<void(Ts...)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}

template<typename... Ts>
ang::core::delegates::listener<void(Ts...)>& ang::core::delegates::listener<void(Ts...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename... Ts>
ang::core::delegates::listener<void(Ts...)>& ang::core::delegates::listener<void(Ts...)>::operator += (ang::core::delegates::function<void(Ts...)> func)
{
	push(ang::move(func));
	return *this;
}


template<typename... Ts>
void ang::core::delegates::listener<void(Ts...)>::invoke(Ts... args)const
{
	if (!is_empty())
	{
		for (function<void(Ts...)> const & func : functions) {
			func(args...);
		}
	}
}

template<typename... Ts>
void ang::core::delegates::listener<void(Ts...)>::operator() (Ts... args)const
{
	if (!is_empty())
	{
		for (function<void(Ts...)> const & func : functions) {
			func(args...);
		}
	}
}


#endif//__CORE_LISTENER_INL__