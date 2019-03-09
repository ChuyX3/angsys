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
	: m_msg(msg)
	, m_function(null)
{
}

function_object <void(object*, platform::events::imsg_event_args*)>::~function_object()
{
	if (m_function != null)
		delete m_function;
}

//ifunction<void(object*, platform::events::imsg_event_args*)>

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::base_event_handler, object, core::delegates::ifunction<void(object*, platform::events::imsg_event_args*)>);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::events::base_event_handler);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::events::base_event_handler, object, core::delegates::ifunction<void(object*, platform::events::imsg_event_args*)>);

core_msg_t function_object <void(object*, platform::events::imsg_event_args*)>::msg_type()const
{
	return m_msg;
}

ievent* function_object <void(object*, platform::events::imsg_event_args*)>::clone()const
{
	auto e = new function_object <void(object*, platform::events::imsg_event_args*)>(m_msg);
	e->m_function = m_function->clone();
	return e;
}

void function_object <void(object*, platform::events::imsg_event_args*)>::invoke(objptr obj, platform::events::imsg_event_args_t args)const
{
	m_function->invoke(obj.get(), args.get());
}

////////////////////////////////////////////////////////////////////////////////////////

event_listener::listener(object* parent, function_type<bool(platform::events::core_msg_t)> func)
	: m_parent(parent)
	, m_comp(func)
	, m_functions()
{

}

//event_listener::listener(listener&& other)
//	: _parent(ang::move(other._parent))
//	, _comp(ang::move(other._comp))
//	, m_functions(ang::move(other.m_functions))
//{
//	other._parent = null;
//	other._comp = null;
//	other.m_functions = null;
//}
//
//event_listener::listener(listener const& other)
//	: _parent(other._parent)
//	, _comp(other._comp)
//	, m_functions(new collections::vector_data<function>())
//{
//	m_functions->copy(other.m_functions.get());
//}

event_listener::~listener()
{
	m_functions.reset();
}

void event_listener::empty()
{
	m_functions.reset();
}

bool event_listener::is_empty()const
{
	return m_functions.is_empty() || m_functions->is_empty();
}

event_token_t event_listener::push(events::event_t func)
{
	m_functions += func;
	return m_functions->last();
}

bool event_listener::pop(event_token_t token)
{
	if (m_functions.is_empty())
		return false;
	return m_functions->pop_at(token);
}

bool event_listener::remove(events::event_t func)
{
	//m_functions += ang::move(func);
	//return m_functions.get()->size() - 1;
	return false;
}

//event_listener& event_listener::operator = (listener&& other)
//{
//	m_functions = ang::move(other.m_functions);
//	return *this;
//}
//
//event_listener& event_listener::operator = (listener const& other)
//{
//	if (m_functions.is_empty())
//		m_functions = new collections::vector_data<function>();
//	m_functions->copy(other.m_functions.get());
//	return *this;
//}

event_token_t event_listener::operator += (platform::events::event_t func)
{
	return push(func);
}

bool event_listener::operator -= (event_token_t token)
{
	return pop(token);
}


int event_listener::invoke(platform::events::imsg_event_args_t args)const
{
	if (!is_empty())
	{
		int c = 0;
		objptr caller = m_parent.lock<object>();
		auto msg = args->msg().msg();
		for(event_t const & e : m_functions) {
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
		objptr caller = m_parent.lock<object>();
		for (event_t const & e : m_functions) {
			c++;
			e(caller, args);
		}
		return c;
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////

event_t::object_wrapper()
	: m_ptr(null)
{

}

event_t::object_wrapper(event_t && other)
	: m_ptr(null)
{
	function_object <void(object*, platform::events::imsg_event_args*)> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

event_t::object_wrapper(event_t const& other)
	: m_ptr(null)
{
	set(other.get());
}

event_t::object_wrapper(platform::events::base_event_handler* ev)
	: m_ptr(null)
{
	set(ev);
}

event_t::~object_wrapper()
{
	reset();
}

void event_t::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

void event_t::reset_unsafe()
{
	m_ptr = null;
}

bool event_t::is_empty()const
{
	return m_ptr == null;
}

platform::events::base_event_handler* event_t::get(void)const
{
	return m_ptr;
}

void event_t::set(platform::events::base_event_handler* ptr)
{
	function_object <void(object*, platform::events::imsg_event_args*)> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

event_t& event_t::operator = (event_t && other)
{
	function_object <void(object*, platform::events::imsg_event_args*)> * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
	return*this;
}

event_t& event_t::operator = (event_t const& other)
{
	set(other.m_ptr);
	return*this;
}

event_t& event_t::operator = (platform::events::base_event_handler* ev)
{
	set(ev);
	return*this;
}

void event_t::operator () (objptr caller, platform::events::imsg_event_args_t args)const
{
	if (m_ptr)
		m_ptr->invoke(ang::move(caller), ang::move(args));
}

