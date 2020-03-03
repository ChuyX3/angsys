#include "pch.h"
#include <ang/platform/win32/controller.h>
#include "event_args.h"
#include <Xinput.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::input;

struct controller::handle
{
	uint id = 0;
	events::event_listener digital_input_event;
	events::event_listener analog_input_event;
	XINPUT_STATE last_state;

	handle(controller* ptr)
		: digital_input_event(ptr, [](events::core_msg_t msg) { return msg == (events::core_msg_t)events::core_msg::contorller_button_change; })
		, analog_input_event(ptr, [](events::core_msg_t msg) { return msg == (events::core_msg_t)events::core_msg::contorller_analog_change; })
	{
	}
};

controller::controller(uint id)
	: m_handle(null)
{
	m_handle = new handle(this);
	m_handle->id = id;
}

controller::~controller()
{
	delete m_handle;
	m_handle = null;
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::input::controller);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::input::controller, object, icontroller);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::input::controller, object, icontroller);

uint controller::get_device_id_property(base_property<uint>const* prop)
{
	return field_to_parent(&controller::device_id, prop)->get_controller_id();
}

controller_buttons_state_t controller::get_state_property(base_property<controller_buttons_state_t>const* prop)
{
	return field_to_parent(&controller::state, prop)->get_state();
}

analog_input_value_t controller::get_triggers_property(base_property<analog_input_value_t>const* prop)
{
	return field_to_parent(&controller::triggers, prop)->get_triggers();
}

thumb_stick_value_t controller::get_left_thumb_stick_property(base_property<thumb_stick_value_t>const* prop)
{
	return field_to_parent(&controller::left_thumb_stick, prop)->get_left_thumb_stick();
}

thumb_stick_value_t controller::get_right_thumb_stick_property(base_property<thumb_stick_value_t>const* prop)
{
	return field_to_parent(&controller::right_thumb_stick, prop)->get_right_thumb_stick();
}

events::event_token_t controller::add_digital_input_event(events::base_event* prop, events::event_t e)
{
	return field_to_parent(&controller::digital_input_event, prop)->add_analog_input_event(e);
}

bool controller::remove_digital_input_event(events::base_event* prop, events::event_token_t token)
{
	return field_to_parent(&controller::digital_input_event, prop)->remove_digital_input_event(token);
}

events::event_token_t controller::add_analog_input_event(events::base_event* prop, events::event_t e)
{
	return field_to_parent(&controller::analog_input_event, prop)->add_analog_input_event(e);
}

bool controller::remove_analog_input_event(events::base_event* prop, events::event_token_t token)
{
	return field_to_parent(&controller::analog_input_event, prop)->remove_analog_input_event(token);
}

uint controller::get_controller_id()const
{
	return m_handle->id;
}

controller_buttons_state_t controller::get_state()const
{
	return controller_buttons_state(m_handle->last_state.Gamepad.wButtons);
}

thumb_stick_value_t controller::get_left_thumb_stick()const
{
	return { (float)max(-1, m_handle->last_state.Gamepad.sThumbLX / 32767.0), (float)max(-1, m_handle->last_state.Gamepad.sThumbLY / 32767.0) };
}

thumb_stick_value_t controller::get_right_thumb_stick()const
{
	return { (float)max(-1, m_handle->last_state.Gamepad.sThumbRX / 32767.0), (float)max(-1, m_handle->last_state.Gamepad.sThumbRY / 32767.0) };
}

analog_input_value_t controller::get_triggers()const
{
	return { (float)m_handle->last_state.Gamepad.bRightTrigger / 255.0f, (float)m_handle->last_state.Gamepad.sThumbRY / 255.0f };
}

events::event_token_t controller::add_digital_input_event(events::event_t e)
{
	return m_handle->digital_input_event += e;
}

bool controller::remove_digital_input_event(events::event_token_t token)
{
	return m_handle->digital_input_event -= token;
}

events::event_token_t controller::add_analog_input_event(events::event_t e)
{
	return m_handle->analog_input_event += e;
}

bool controller::remove_analog_input_event(events::event_token_t token)
{
	return m_handle->analog_input_event -= token;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

struct controller_manager::handle
{
	stack_array<collections::pair<controller_status_t, XINPUT_STATE>, 4> m_states;
	stack_array<controller_t, 4> m_controllers;
	events::event_listener m_controller_connected_event;
	events::event_listener m_controller_disconnected_event;

	handle(controller_manager* ptr)
		: m_controller_connected_event(ptr, [](events::core_msg_t msg) { return msg == (events::core_msg_t)events::core_msg::controller_status_change; })
		, m_controller_disconnected_event(ptr, [](events::core_msg_t msg) { return msg == (events::core_msg_t)events::core_msg::controller_status_change; })
	{
		for (collections::pair<controller_status_t, XINPUT_STATE>& item : m_states)
			ZeroMemory(&item.value, sizeof(XINPUT_STATE));
	}
};


controller_manager::controller_manager()
	: m_handle(null)
{
	m_handle = new handle(this);

	uint i = 0;
	for (controller_t& item : m_handle->m_controllers)
		item = new controller(i++);
}

controller_manager::~controller_manager()
{
	delete m_handle;
	m_handle = null;
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::input::controller_manager);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::input::controller_manager, object, icontroller_manager);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::input::controller_manager, object, icontroller_manager);

icontroller_t controller_manager::get_controller(uint id)const
{
	return m_handle->m_controllers[id];
}

events::event_token_t controller_manager::add_controller_connected_event(events::event_t e)
{
	return m_handle->m_controller_connected_event += e;
}

bool controller_manager::remove_controller_connected_event(events::event_token_t token)
{
	return m_handle->m_controller_connected_event -= token;
}

events::event_token_t controller_manager::add_controller_disconnected_event(events::event_t e)
{
	return m_handle->m_controller_disconnected_event += e;
}

bool controller_manager::remove_controller_disconnected_event(events::event_token_t token)
{
	return m_handle->m_controller_disconnected_event -= token;
}

array_view<const controller_t> controller_manager::get_controller_property(base_property<array_view<const controller_t>> const* prop)
{
	return to_array((controller_t const*)field_to_parent(&controller_manager::contoller, prop)->m_handle->m_controllers.data(), 4);
}

events::event_token_t controller_manager::add_controller_connected_event(events::base_event* prop, events::event_t e)
{
	return field_to_parent(&controller_manager::controller_connected_event, prop)->add_controller_connected_event(e);
}

bool controller_manager::remove_controller_connected_event(events::base_event* prop, events::event_token_t token)
{
	return field_to_parent(&controller_manager::controller_connected_event, prop)->remove_controller_connected_event(token);
}

events::event_token_t controller_manager::add_controller_disconnected_event(events::base_event* prop, events::event_t e)
{
	return field_to_parent(&controller_manager::controller_disconnected_event, prop)->add_controller_disconnected_event(e);
}

bool controller_manager::remove_controller_disconnected_event(events::base_event* prop, events::event_token_t token)
{
	return field_to_parent(&controller_manager::controller_disconnected_event, prop)->remove_controller_disconnected_event(token);
}

void controller_manager::update()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	for (uint i = 0; i < 4; i++)
	{
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			if (m_handle->m_states[i].key == controller_status::disconnected) {
				m_handle->m_states[i].key = controller_status::connected;
				m_handle->m_controllers[i]->m_handle->last_state = state;
				events::icontroller_status_args_t args = new events::controller_status_args(
					events::message(events::core_msg::controller_status_change),
					m_handle->m_controllers[i],
					m_handle->m_states[i].key);

				try { m_handle->m_controller_connected_event(args.get()); }
				catch (...) { }
			}
			else
			{

			}
		}
		else if (m_handle->m_states[i].key != controller_status::disconnected)
		{
			m_handle->m_states[i].key = controller_status::disconnected;
			events::icontroller_status_args_t args = new events::controller_status_args(
				events::message(events::core_msg::controller_status_change),
				m_handle->m_controllers[i],
				m_handle->m_states[i].key
			);
			try { m_handle->m_controller_disconnected_event(args.get()); }
			catch (...) { }
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////