#include "pch.h"
#include <ang/platform/platform.hpp>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;

ANG_IMPLEMENT_ENUM(ang::platform::events, core_msg_enum, core_msg_t, core_msg_enum::none);
ANG_IMPLEMENT_INTERFACE(ang::platform::events, ievent)

ievent_function::~ievent_function()
{
}

void* ievent_function::operator new(wsize size)
{
	return ang::memory::object_allocator<byte>::alloc(size);
}

void ievent_function::operator delete(void* ptr)
{
	return ang::memory::object_allocator<byte>::free((byte*)ptr);
}


#if defined _DEBUG
#define new new(__FILE__, __LINE__)
#endif

//////////////////////////////////////////////////////////////

event::function_data(platform::events::core_msg_t msg)
	: _msg(msg)
	, _function(null)
{
}

event::~function_data()
{
	if (_function != null)
		delete _function;
}

ANG_IMPLEMENT_CLASSNAME(ang::platform::events::event);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::events::event);

bool event::is_inherited_of(type_name_t name)
{
	return (name == ang::type_of<event>()
		|| object::is_inherited_of(name)
		|| ievent::is_inherited_of(name));
}

bool event::is_kind_of(type_name_t name)const
{
	return (name == ang::type_of<event>()
		|| object::is_kind_of(name)
		|| ievent::is_kind_of(name));
}

bool event::query_object(ang::type_name_t name, ang::unknown_ptr_t out)
{
	if (out == null)
		return false;

	if (name == ang::type_of<event>())
	{
		*out = static_cast<event*>(this);
		return true; 
	}
	else if (object::query_object(name, out))
	{
		return true;
	}
	else if (ievent::query_object(name, out))
	{
		return true;
	}
	return false;
}

core_msg_t event::msg_type()const
{
	return _msg;
}

ievent* event::clone()const
{
	auto e = new event(_msg);
	e->_function = _function->clone();
	return e;
}

void event::invoke(objptr obj, platform::events::imsg_event_args_t args)const
{
	_function->invoke(obj.get(), args.get());
}

////////////////////////////////////////////////////////////////////////////////////////

event_listener::listener(object* parent, function_type<bool, platform::events::core_msg_t> func)
	: _parent(parent)
	, _comp(func)
	, _functions(new collections::vector_buffer<function>())
{

}

//event_listener::listener(listener&& other)
//	: _parent(ang::move(other._parent))
//	, _comp(ang::move(other._comp))
//	, _functions(ang::move(other._functions))
//{
//	other._parent = null;
//	other._comp = null;
//	other._functions = null;
//}
//
//event_listener::listener(listener const& other)
//	: _parent(other._parent)
//	, _comp(other._comp)
//	, _functions(new collections::vector_data<function>())
//{
//	_functions->copy(other._functions.get());
//}

event_listener::~listener()
{
	_functions.clean();
}

void event_listener::empty()
{
	_functions.clean();
}

bool event_listener::is_empty()const
{
	return _functions.is_empty() || _functions->is_empty();
}

index event_listener::append(events::event_t func)
{
	_functions += func;
	return _functions.get()->size() - 1;
}

bool event_listener::remove(events::event_t func)
{
	//_functions += ang::move(func);
	//return _functions.get()->size() - 1;
	return false;
}

//event_listener& event_listener::operator = (listener&& other)
//{
//	_functions = ang::move(other._functions);
//	return *this;
//}
//
//event_listener& event_listener::operator = (listener const& other)
//{
//	if (_functions.is_empty())
//		_functions = new collections::vector_data<function>();
//	_functions->copy(other._functions.get());
//	return *this;
//}

event_listener& event_listener::operator += (platform::events::event_t func)
{
	append(func);
	return *this;
}


int event_listener::invoke(platform::events::imsg_event_args_t args)const
{
	if (!is_empty())
	{
		int c = 0;
		objptr caller = _parent.lock<object>();
		auto msg = args->msg_info()->msg();
		for(event_t const & e : _functions) {
			if (msg == e.get()->msg_type())
			{
				c++;
				e(caller, args);
			}
		}
		return c;
	}
	return 0;
}

int event_listener::operator() (platform::events::imsg_event_args_t args)const
{
	if (!is_empty())
	{
		int c = 0;
		objptr caller = _parent.lock<object>();
		for (event_t const & e : _functions) {
			c++;
			e(caller, args);
		}
		return c;
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////

event_t::object_wrapper()
	: _ptr(null)
{

}

event_t::object_wrapper(event_t && other)
	: _ptr(null)
{
	event * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

event_t::object_wrapper(event_t const& other)
	: _ptr(null)
{
	set(other.get());
}

event_t::object_wrapper(platform::events::event* ev)
	: _ptr(null)
{
	set(ev);
}

event_t::~object_wrapper()
{
	clean();
}

void event_t::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void event_t::clean_unsafe()
{
	_ptr = null;
}

bool event_t::is_empty()const
{
	return _ptr == null;
}

platform::events::event* event_t::get(void)const
{
	return _ptr;
}

void event_t::set(platform::events::event* ptr)
{
	event * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

event_t& event_t::operator = (event_t && other)
{
	event * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
	return*this;
}

event_t& event_t::operator = (event_t const& other)
{
	set(other._ptr);
	return*this;
}

event_t& event_t::operator = (platform::events::event* ev)
{
	set(ev);
	return*this;
}

void event_t::operator () (objptr caller, platform::events::imsg_event_args_t args)const
{
	if (_ptr)
		_ptr->invoke(ang::move(caller), ang::move(args));
}

