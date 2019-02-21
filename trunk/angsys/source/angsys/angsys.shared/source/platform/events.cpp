#include "pch.h"
#include <ang/platform/platform.h>

using namespace ang;
using namespace ang::core;
using namespace ang::core::delegates;
using namespace ang::platform;
using namespace ang::platform::events;

safe_enum_rrti2(ang::platform::events, core_msg_enum);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::ievent, interface);

ievent_function::~ievent_function()
{
}

void* ievent_function::operator new(wsize size)
{
	memory::iraw_allocator* allocator = memory::get_raw_allocator(ang_object_memory);
	return allocator->memory_alloc(size);
}

void ievent_function::operator delete(void* ptr)
{
	memory::iraw_allocator* allocator = memory::get_raw_allocator(ang_object_memory);
	return allocator->memory_release((byte*)ptr);
}


//#if defined _DEBUG
//#define new new(__FILE__, __LINE__)
//#endif

//////////////////////////////////////////////////////////////

function_object<void(object*, platform::events::imsg_event_args*)>::function_object(platform::events::core_msg_t msg)
	: _msg(msg)
	, _function(null)
{
}

function_object <void(object*, platform::events::imsg_event_args*)>::~function_object()
{
	if (_function != null)
		delete _function;
}

//ifunction<void(object*, platform::events::imsg_event_args*)>

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::event, object, core::delegates::ifunction<void(object*, platform::events::imsg_event_args*)>);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::event);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::event, object, core::delegates::ifunction<void(object*, platform::events::imsg_event_args*)>);

core_msg_t function_object <void(object*, platform::events::imsg_event_args*)>::msg_type()const
{
	return _msg;
}

ievent* function_object <void(object*, platform::events::imsg_event_args*)>::clone()const
{
	auto e = new function_object <void(object*, platform::events::imsg_event_args*)>(_msg);
	e->_function = _function->clone();
	return e;
}

void function_object <void(object*, platform::events::imsg_event_args*)>::invoke(objptr obj, platform::events::imsg_event_args_t args)const
{
	_function->invoke(obj.get(), args.get());
}

////////////////////////////////////////////////////////////////////////////////////////

event_listener::listener(object* parent, function_type<bool(platform::events::core_msg_t)> func)
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
	_functions.reset();
}

void event_listener::empty()
{
	_functions.reset();
}

bool event_listener::is_empty()const
{
	return _functions.is_empty() || _functions->is_empty();
}

index event_listener::push(events::event_t func)
{
	_functions += func;
	return (uint)_functions.get()->size() - 1;
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
	push(func);
	return *this;
}


int event_listener::invoke(platform::events::imsg_event_args_t args)const
{
	if (!is_empty())
	{
		int c = 0;
		objptr caller = _parent.lock<object>();
		auto msg = args->msg_info().msg();
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
	function_object <void(object*, platform::events::imsg_event_args*)> * temp = other._ptr;
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
	reset();
}

void event_t::reset()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void event_t::reset_unsafe()
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
	function_object <void(object*, platform::events::imsg_event_args*)> * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

event_t& event_t::operator = (event_t && other)
{
	function_object <void(object*, platform::events::imsg_event_args*)> * temp = other._ptr;
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

