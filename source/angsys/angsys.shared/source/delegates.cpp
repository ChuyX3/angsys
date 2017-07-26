#include "pch.h"
#include <ang/core/delegates.h>

using namespace ang;
using namespace ang::core;
using namespace ang::core::delegates;


ang::core::delegates::listener<dword(pointer)>::listener()
{
}


ang::core::delegates::listener<dword(pointer)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{
}


ang::core::delegates::listener<dword(pointer)>::listener(listener const& other)
	: functions(other.functions)
{
}


ang::core::delegates::listener<dword(pointer)>::~listener()
{
	empty();
}


void ang::core::delegates::listener<dword(pointer)>::empty()
{
	functions.clean();
}


bool ang::core::delegates::listener<dword(pointer)>::is_empty()const
{
	return functions.is_empty() || functions->is_empty();
}


index ang::core::delegates::listener<dword(pointer)>::append(ang::core::delegates::function<dword(pointer)> func)
{
	functions += ang::move(func);
	return functions.get()->size() - 1;
}


ang::core::delegates::listener<dword(pointer)>& ang::core::delegates::listener<dword(pointer)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}


ang::core::delegates::listener<dword(pointer)>& ang::core::delegates::listener<dword(pointer)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}


ang::core::delegates::listener<dword(pointer)>& ang::core::delegates::listener<dword(pointer)>::operator += (ang::core::delegates::function<dword(pointer)> func)
{
	append(ang::move(func));
	return *this;
}


ang::array<dword> ang::core::delegates::listener<dword(pointer)>::operator() (pointer  args)const
{
	array<dword> result;
	if (!is_empty())
	{
		result = ang::move(array<dword>(functions->size()));

		function<dword(pointer)>func;

		for (index_t i = 0U; i < result->size(); ++i)
		{
			func = functions[i];
			result[i] = func(args);
		}
	}
	return ang::move(result);
}


ang::array<dword> ang::core::delegates::listener<dword(pointer)>::invoke(pointer  args)const
{
	array<dword> result;
	if (!is_empty())
	{
		result = ang::move(array<dword>(functions->size()));

		function<dword(pointer)>func;

		for (index_t i = 0U; i < result->size(); ++i)
		{
			func = functions[i];
			result[i] = func(args);
		}
	}
	return ang::move(result);
}


////////////////////////////////////////////////////////////////////////////////////

ang::core::delegates::listener<void(objptr, pointer)>::listener()
{
}


ang::core::delegates::listener<void(objptr, pointer)>::listener(listener&& other)
	: functions(ang::move(other.functions))
{
}


ang::core::delegates::listener<void(objptr, pointer)>::listener(listener const& other)
	: functions(other.functions)
{
}


ang::core::delegates::listener<void(objptr, pointer)>::~listener()
{
	empty();
}


void ang::core::delegates::listener<void(objptr, pointer)>::empty()
{
	functions.clean();
}


bool ang::core::delegates::listener<void(objptr, pointer)>::is_empty()const
{
	return functions.is_empty() || functions->is_empty();
}


index ang::core::delegates::listener<void(objptr, pointer)>::append(ang::core::delegates::function<void(objptr, pointer)> func)
{
	functions += ang::move(func);
	return functions.get()->size() - 1;
}


ang::core::delegates::listener<void(objptr, pointer)>& ang::core::delegates::listener<void(objptr, pointer)>::operator = (listener&& other)
{
	functions = ang::move(other.functions);
	return *this;
}


ang::core::delegates::listener<void(objptr, pointer)>& ang::core::delegates::listener<void(objptr, pointer)>::operator = (listener const& other)
{
	functions = other.functions;
	return *this;
}


ang::core::delegates::listener<void(objptr, pointer)>& ang::core::delegates::listener<void(objptr, pointer)>::operator += (ang::core::delegates::function<void(objptr, pointer)> func)
{
	append(ang::move(func));
	return *this;
}


void ang::core::delegates::listener<void(objptr, pointer)>::operator() (objptr sender, pointer  args)const
{
	if (!is_empty()) for(function<void(objptr, pointer)>func : (static_array<function<void(objptr, pointer)>>) functions)
		func(sender, args);
}


void ang::core::delegates::listener<void(objptr, pointer)>::invoke(objptr sender, pointer args)const
{
	if (!is_empty()) for (function<void(objptr, pointer)>func : (static_array<function<void(objptr, pointer)>>) functions)
		func(sender, args);
}
