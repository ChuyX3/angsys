#include "pch.h"
#include <ang/core/delegates.hpp>

#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

using namespace ang;
using namespace ang::core;
using namespace ang::core::delegates;


ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::object_wrapper()
	: _ptr(null)
{
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::object_wrapper(object_wrapper&& other)
	: object_wrapper()
{
	core::delegates::function_data<void(void)> * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::object_wrapper(object_wrapper const& other)
	: object_wrapper()
{
	set(other.get());
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::object_wrapper(ang::nullptr_t const&)
	: object_wrapper()
{
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::~object_wrapper()
{
}

void ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::is_empty()const
{
	return _ptr == null;
}

ang::core::delegates::function_data<void(void)>* ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::set(ang::core::delegates::function_data<void(void)>* ptr)
{
	core::delegates::function_data<void(void)> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::core::delegates::function_data<void(void)>** ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>& ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(void)>> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<ang::core::delegates::function_data<void(void)>>& ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::operator = (ang::object_wrapper<ang::core::delegates::function_data<void(void)>> const& other)
{
	set(other._ptr);
	return*this;
}

ang::object_wrapper_ptr<ang::core::delegates::function_data<void(void)>> ang::object_wrapper<ang::core::delegates::function_data<void(void)>>::operator & (void)
{
	return this;
}

////////////////////////////////////////////////////////////////////////////////////

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
	return (index)functions.get()->size() - 1;
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
		result = new collections::array_buffer<dword>();//ang::move(array<dword>(functions->size()));
		result->size(functions->size());
		function<dword(pointer)>func;

		for (index i = 0U; i < result->size(); ++i)
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
		result = new collections::array_buffer<dword>();//ang::move(array<dword>(functions->size()));
		result->size(functions->size());

		function<dword(pointer)>func;

		for (index i = 0U; i < result->size(); ++i)
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
	return (index)functions.get()->size() - 1;
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
	if (!is_empty()) for(function<void(objptr, pointer)>func : (array_view<function<void(objptr, pointer)>>) functions)
		func(sender, args);
}


void ang::core::delegates::listener<void(objptr, pointer)>::invoke(objptr sender, pointer args)const
{
	if (!is_empty()) for (function<void(objptr, pointer)>func : (array_view<function<void(objptr, pointer)>>) functions)
		func(sender, args);
}
