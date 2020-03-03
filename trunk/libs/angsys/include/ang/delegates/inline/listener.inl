#ifndef __COFFE_LISTENER_H__
#error ...
#elif !defined __COFFE_LISTENER_INL__
#define __COFFE_LISTENER_INL__


template<typename T, typename... Ts>
coffe::delegates::listener<T(Ts...)>::listener()
{
}

template<typename T, typename... Ts>
coffe::delegates::listener<T(Ts...)>::listener(listener&& other)
	: functions(coffe::move(other.functions))
{
}

template<typename T, typename... Ts>
coffe::delegates::listener<T(Ts...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename T, typename... Ts>
coffe::delegates::listener<T(Ts...)>::~listener()
{
	clear();
}

template<typename T, typename... Ts>
void coffe::delegates::listener<T(Ts...)>::clear()
{
	functions.clear();
}

template<typename T, typename... Ts>
bool coffe::delegates::listener<T(Ts...)>::is_empty()const
{
	return functions.is_empty();
}

template<typename T, typename... Ts>
coffe::delegates::listen_token<T(Ts...)> coffe::delegates::listener<T(Ts...)>::push(coffe::delegates::function<T(Ts...)> func)
{
	functions += coffe::move(func);
	return functions.last();
}

template<typename T, typename... Ts>
bool coffe::delegates::listener<T(Ts...)>::pop(coffe::delegates::listen_token<T(Ts...)> token)
{
	if (functions.is_empty())
		return false;
	functions.remove(token);
	return true;
}

template<typename T, typename... Ts>
coffe::delegates::listener<T(Ts...)>& coffe::delegates::listener<T(Ts...)>::operator = (listener&& other)
{
	functions = coffe::move(other.functions);
	return *this;
}

template<typename T, typename... Ts>
coffe::delegates::listener<T(Ts...)>& coffe::delegates::listener<T(Ts...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename T, typename... Ts>
coffe::delegates::listen_token<T(Ts...)> coffe::delegates::listener<T(Ts...)>::operator += (coffe::delegates::function<T(Ts...)> func)
{
	return push(coffe::move(func));
}

template<typename T, typename... Ts>
bool coffe::delegates::listener<T(Ts...)>::operator -= (coffe::delegates::listen_token<T(Ts...)> token)
{
	return pop(coffe::move(token));
}

template<typename T, typename... Ts>
coffe::array<T> coffe::delegates::listener<T(Ts...)>::operator() (Ts... args)const
{
	array<T> result;
	if (!is_empty())
	{
		windex i = 0;
		result.size(functions.size());
		for (function<T(Ts...)> const& func : functions)
			result[i++] = func(args...);
	}
	return coffe::move(result);
}

template<typename T, typename... Ts>
coffe::array<T> coffe::delegates::listener<T(Ts...)>::invoke(Ts... args)const
{
	array<T> result;
	if (!is_empty())
	{
		windex i = 0;
		result.size(functions.size());
		for (function<T(Ts...)> const& func : functions)
			result[i++] = func(args...);
	}
	return coffe::move(result);
}

template<typename... Ts>
coffe::delegates::listener<void(Ts...)>::listener()
{
}

template<typename... Ts>
coffe::delegates::listener<void(Ts...)>::listener(listener&& other)
	: functions(coffe::move(other.functions))
{
}

template<typename... Ts>
coffe::delegates::listener<void(Ts...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename... Ts>
coffe::delegates::listener<void(Ts...)>::~listener()
{
	clear();
}

template<typename... Ts>
void coffe::delegates::listener<void(Ts...)>::clear()
{
	functions.clear();
}

template<typename... Ts>
bool coffe::delegates::listener<void(Ts...)>::is_empty()const
{
	return functions.is_empty();
}

template<typename... Ts>
coffe::delegates::listen_token<void(Ts...)> coffe::delegates::listener<void(Ts...)>::push(coffe::delegates::function<void(Ts...)> func)
{
	functions += coffe::move(func);
	return functions.rbegin();
}

template<typename... Ts>
bool coffe::delegates::listener<void(Ts...)>::pop(coffe::delegates::listen_token<void(Ts...)> token)
{
	if (functions.is_empty())
		return false;
	functions.remove(token);
	return true;
}

template<typename... Ts>
coffe::delegates::listener<void(Ts...)>& coffe::delegates::listener<void(Ts...)>::operator = (listener&& other)
{
	functions = coffe::move(other.functions);
	return *this;
}

template<typename... Ts>
coffe::delegates::listener<void(Ts...)>& coffe::delegates::listener<void(Ts...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename... Ts>
coffe::delegates::listen_token<void(Ts...)> coffe::delegates::listener<void(Ts...)>::operator += (coffe::delegates::function<void(Ts...)> func)
{
	return push(coffe::move(func));
}

template<typename... Ts>
bool coffe::delegates::listener<void(Ts...)>::operator -= (coffe::delegates::listen_token<void(Ts...)> token)
{
	return pop(coffe::move(token));
}


template<typename... Ts>
void coffe::delegates::listener<void(Ts...)>::invoke(Ts... args)const
{
	if (!is_empty())
	{
		for (function<void(Ts...)> const & func : functions) {
			func(args...);
		}
	}
}

template<typename... Ts>
void coffe::delegates::listener<void(Ts...)>::operator() (Ts... args)const
{
	if (!is_empty())
	{
		for (function<void(Ts...)> const & func : functions) {
			func(args...);
		}
	}
}


#endif//__CORE_LISTENER_INL__