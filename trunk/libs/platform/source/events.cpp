#include "pch.h"
#include <ang/platform/platform.h>

using namespace ang;
using namespace ang::core;
using namespace ang::delegates;
using namespace ang::platform;
using namespace ang::platform::events;

ANG_ENUM_IMPLEMENT(ang::platform::events, core_msg);

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::start_app_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::exit_app_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::created_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::initialize_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::closed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::destroyed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::update_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::draw_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::activate_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::visibility_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::display_size_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::display_orientation_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::display_invalidate_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::key_pressed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::key_released_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::text_changed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::pointer_moved_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::pointer_pressed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::pointer_released_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::pointer_entered_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::pointer_leaved_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::controller_status_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::controller_button_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(ang::platform::events::controller_analog_change_event);

//ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::platform::events::ievent);

//#if defined _DEBUG
//#define new new(__FILE__, __LINE__)
//#endif

//////////////////////////////////////////////////////////////
function_base<void(object*, platform::events::imsg_event_args*)>::function_base(platform::events::core_msg_t msg)
	: m_msg(msg)
	, m_function(null)
{
}

function_base <void(object*, platform::events::imsg_event_args*)>::~function_base()
{
	m_function = null;
}

//ifunction<void(object*, platform::events::imsg_event_args*)>

//ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::events::base_event_handler);

core_msg_t function_base <void(object*, platform::events::imsg_event_args*)>::msg_type()const
{
	return m_msg;
}

ievent* function_base <void(object*, platform::events::imsg_event_args*)>::clone()const
{
	auto e = new function_base <void(object*, platform::events::imsg_event_args*)>(m_msg);
	e->m_function = m_function->clone();
	return e;
}

void function_base <void(object*, platform::events::imsg_event_args*)>::invoke(objptr obj, platform::events::imsg_event_args_t args)const
{
	m_function->invoke(obj.get(), args.get());
}

////////////////////////////////////////////////////////////////////////////////////////

event_listener::listener(object* parent)
	: m_parent(parent)
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
	m_functions.clear();
}

void event_listener::empty()
{
	m_functions.clear();
}

bool event_listener::is_empty()const
{
	return m_functions.is_empty();
}

event_token_t event_listener::push(events::event_t func)
{
	m_functions += func;
	return m_functions.rbegin();
}

bool event_listener::pop(event_token_t token)
{
	if (m_functions.is_empty())
		return false;
	return m_functions.remove(token);
}

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
		for(event_t e : m_functions) {
			if (msg == e->msg_type())
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
	function_base <void(object*, platform::events::imsg_event_args*)> * temp = other.m_ptr;
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
	function_base <void(object*, platform::events::imsg_event_args*)> * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
	if (temp)temp->release();
}

event_t& event_t::operator = (event_t && other)
{
	function_base <void(object*, platform::events::imsg_event_args*)> * temp = other.m_ptr;
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

