/*********************************************************************************************************************/
/*   File Name: ang/delegates/inline/listener.inl                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_LISTENER_H__
#error ...
#elif !defined __ANG_LISTENER_INL__
#define __ANG_LISTENER_INL__


template<typename T, typename... Ts>
ang::delegates::listener<T(Ts...)>::listener()
{
}

template<typename T, typename... Ts>
ang::delegates::listener<T(Ts...)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{
}

template<typename T, typename... Ts>
ang::delegates::listener<T(Ts...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename T, typename... Ts>
ang::delegates::listener<T(Ts...)>::~listener()
{
	clear();
}

template<typename T, typename... Ts>
void ang::delegates::listener<T(Ts...)>::clear()
{
	functions.clear();
}

template<typename T, typename... Ts>
bool ang::delegates::listener<T(Ts...)>::is_empty()const
{
	return functions.is_empty();
}

template<typename T, typename... Ts>
ang::delegates::listen_token<T(Ts...)> ang::delegates::listener<T(Ts...)>::push(ang::delegates::function<T(Ts...)> func)
{
	functions += ang::move(func);
	return functions.last();
}

template<typename T, typename... Ts>
bool ang::delegates::listener<T(Ts...)>::pop(ang::delegates::listen_token<T(Ts...)> token)
{
	if (functions.is_empty())
		return false;
	functions.remove(token);
	return true;
}

template<typename T, typename... Ts>
ang::delegates::listener<T(Ts...)>& ang::delegates::listener<T(Ts...)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}

template<typename T, typename... Ts>
ang::delegates::listener<T(Ts...)>& ang::delegates::listener<T(Ts...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename T, typename... Ts>
ang::delegates::listen_token<T(Ts...)> ang::delegates::listener<T(Ts...)>::operator += (ang::delegates::function<T(Ts...)> func)
{
	return push(ang::move(func));
}

template<typename T, typename... Ts>
bool ang::delegates::listener<T(Ts...)>::operator -= (ang::delegates::listen_token<T(Ts...)> token)
{
	return pop(ang::move(token));
}

template<typename T, typename... Ts>
ang::array<T> ang::delegates::listener<T(Ts...)>::operator() (Ts... args)const
{
	array<T> result;
	if (!is_empty())
	{
		windex i = 0;
		result.size(functions.size());
		for (function<T(Ts...)> const& func : functions)
			result[i++] = func(args...);
	}
	return ang::move(result);
}

template<typename T, typename... Ts>
ang::array<T> ang::delegates::listener<T(Ts...)>::invoke(Ts... args)const
{
	array<T> result;
	if (!is_empty())
	{
		windex i = 0;
		result.size(functions.size());
		for (function<T(Ts...)> const& func : functions)
			result[i++] = func(args...);
	}
	return ang::move(result);
}

template<typename... Ts>
ang::delegates::listener<void(Ts...)>::listener()
{
}

template<typename... Ts>
ang::delegates::listener<void(Ts...)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{
}

template<typename... Ts>
ang::delegates::listener<void(Ts...)>::listener(listener const& other)
	: functions(other.functions)
{
}

template<typename... Ts>
ang::delegates::listener<void(Ts...)>::~listener()
{
	clear();
}

template<typename... Ts>
void ang::delegates::listener<void(Ts...)>::clear()
{
	functions.clear();
}

template<typename... Ts>
bool ang::delegates::listener<void(Ts...)>::is_empty()const
{
	return functions.is_empty();
}

template<typename... Ts>
ang::delegates::listen_token<void(Ts...)> ang::delegates::listener<void(Ts...)>::push(ang::delegates::function<void(Ts...)> func)
{
	functions += ang::move(func);
	return functions.rbegin();
}

template<typename... Ts>
bool ang::delegates::listener<void(Ts...)>::pop(ang::delegates::listen_token<void(Ts...)> token)
{
	if (functions.is_empty())
		return false;
	functions.remove(token);
	return true;
}

template<typename... Ts>
ang::delegates::listener<void(Ts...)>& ang::delegates::listener<void(Ts...)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}

template<typename... Ts>
ang::delegates::listener<void(Ts...)>& ang::delegates::listener<void(Ts...)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}

template<typename... Ts>
ang::delegates::listen_token<void(Ts...)> ang::delegates::listener<void(Ts...)>::operator += (ang::delegates::function<void(Ts...)> func)
{
	return push(ang::move(func));
}

template<typename... Ts>
bool ang::delegates::listener<void(Ts...)>::operator -= (ang::delegates::listen_token<void(Ts...)> token)
{
	return pop(ang::move(token));
}


template<typename... Ts>
void ang::delegates::listener<void(Ts...)>::invoke(Ts... args)const
{
	if (!is_empty())
	{
		for (function<void(Ts...)> const & func : functions) {
			func(args...);
		}
	}
}

template<typename... Ts>
void ang::delegates::listener<void(Ts...)>::operator() (Ts... args)const
{
	if (!is_empty())
	{
		for (function<void(Ts...)> const & func : functions) {
			func(args...);
		}
	}
}


#endif//__ANG_LISTENER_INL__