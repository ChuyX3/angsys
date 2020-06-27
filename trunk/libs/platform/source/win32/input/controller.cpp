#include "pch.h"
#include "core_app.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::input;

ang_intf_implement_event_handler(icontroller_manager, controller_connected_event);
ang_intf_implement_event_handler(icontroller_manager, controller_disconnected_event);

ang_intf_implement_event_handler(icontroller, digital_button_change_event);
ang_intf_implement_event_handler(icontroller, analog_input_change_event);

extern input::icontroller_manager_t s_controller_manager;

controller::controller(uint id)
	: m_id(id)
	, m_is_enable(false)
	, m_analog_input_change_event(this)
	, m_digital_button_change_event(this)
	, m_analog_input_args(new events::controller_analog_input_args(events::message(events::core_msg::controller_analog_change)))
	, m_digital_input_args(new events::controller_digital_input_args(events::message(events::core_msg::controller_button_change)))
{
	memset(&m_state, 0, size_of<XINPUT_STATE>());
}

controller::~controller()
{
}

void controller::dispose()
{
	m_analog_input_change_event.empty();
	m_digital_button_change_event.empty();
	m_analog_input_args.reset();
	m_digital_input_args.reset();
}

uint controller::controller_id()const
{
	return m_id;
}

controller_buttons_state_t controller::state()const
{
	return m_buttons_state;
}

thumb_stick_value_t controller::left_thumb_stick()const
{
	return m_left_thumb_stick;
}

thumb_stick_value_t controller::right_thumb_stick()const
{
	return m_right_thumb_stick;
}

analog_input_value_t controller::triggers()const
{
	return m_triggers;
}

bool controller::is_enable()const
{
	return m_is_enable;
}

void controller::init(XINPUT_STATE const& state)
{
	if (!m_is_enable) {
		m_is_enable = true;
		m_state = state;

		m_triggers.x = state.Gamepad.bRightTrigger / 255.0f;
		m_triggers.y = state.Gamepad.bLeftTrigger / 255.0f;
		m_left_thumb_stick.x = state.Gamepad.sThumbLX / 32767.0f;
		m_left_thumb_stick.y = state.Gamepad.sThumbLY / 32767.0f;
		m_right_thumb_stick.x = state.Gamepad.sThumbRX / 32767.0f;
		m_right_thumb_stick.y = state.Gamepad.sThumbRY / 32767.0f;
		m_buttons_state = controller_button(state.Gamepad.wButtons);
	}
}

void controller::update(float delta, XINPUT_STATE const& state)
{
	m_triggers.x = state.Gamepad.bRightTrigger / 255.0f;
	m_triggers.y = state.Gamepad.bLeftTrigger / 255.0f;
	m_left_thumb_stick.x = state.Gamepad.sThumbLX / 32767.0f;
	m_left_thumb_stick.y = state.Gamepad.sThumbLY / 32767.0f;
	m_right_thumb_stick.x = state.Gamepad.sThumbRX / 32767.0f;
	m_right_thumb_stick.y = state.Gamepad.sThumbRY / 32767.0f;
	m_buttons_state = controller_button(state.Gamepad.wButtons);
	
	word test = 1;
	word change = m_state.Gamepad.wButtons ^ state.Gamepad.wButtons;
	if (!m_digital_button_change_event.is_empty()) {
		while (change != 0) {
			if ((change & test) == test) {
				change &= ~test;
				controller_button_state_t s;
				s.button = controller_button(test);
				s.is_presed = (m_state.Gamepad.wButtons & test) != 0 ? true : false;

				m_digital_input_args->msg(events::message{
						events::core_msg::controller_button_change,
						ulong64(this),
						long64(&s)
					});

				try {
					m_digital_button_change_event.invoke(m_digital_input_args.get());
				}
				catch (...) {

				}
			}
			test <<= 1;
		}
	}
	
	if (!m_analog_input_change_event.is_empty()) {
		if (state.Gamepad.bLeftTrigger > 50 || state.Gamepad.bRightTrigger > 50) {
			analog_input_state_t s;
			if (state.Gamepad.bLeftTrigger > 50)
				s.button += controller_button::left_trigger;
			if (state.Gamepad.bRightTrigger > 50)
				s.button += controller_button::right_trigger;
			s.value = m_triggers;
			m_analog_input_args->msg(events::message{
						events::core_msg::controller_analog_change,
						ulong64(this),
						long64(&s)
				});
		
			try {
				m_analog_input_change_event.invoke(m_analog_input_args.get());
			}
			catch (...) {
			}
		}
#define INPUT_DEADZONE 10000

		if (state.Gamepad.sThumbRX > INPUT_DEADZONE
			|| state.Gamepad.sThumbRX < -INPUT_DEADZONE
			|| state.Gamepad.sThumbRY > INPUT_DEADZONE
			|| state.Gamepad.sThumbRY < -INPUT_DEADZONE) {
			analog_input_state_t s;
			s.button = controller_button::right_thumb;
			s.value = m_right_thumb_stick;
			m_analog_input_args->msg(events::message{
						events::core_msg::controller_analog_change,
						ulong64(this),
						long64(&s)
				});	
			try {
				m_analog_input_change_event.invoke(m_analog_input_args.get());
			}
			catch (...) {
			}
		}

		if (state.Gamepad.sThumbLX > INPUT_DEADZONE
			|| state.Gamepad.sThumbLX < -INPUT_DEADZONE
			|| state.Gamepad.sThumbLY > INPUT_DEADZONE
			|| state.Gamepad.sThumbLY < -INPUT_DEADZONE) {
			analog_input_state_t s;
			s.button = controller_button::left_thumb;
			s.value = m_left_thumb_stick;
			m_analog_input_args->msg(events::message{
						events::core_msg::controller_analog_change,
						ulong64(this),
						long64(&s)
				});
			try {
				m_analog_input_change_event.invoke(m_analog_input_args.get());
			}
			catch (...) {
			}
		}
	}
	m_state = state;
}

void controller::exit()
{
	m_is_enable = false;
}

ang::platform::events::event_token_t controller::add_digital_button_change_event(ang::platform::events::event_t e)
{
	if (!e.is_empty() && e->msg_type() == events::core_msg::controller_button_change)
		return m_digital_button_change_event += e;
	return ang::platform::events::event_token_t();
}

bool controller::remove_digital_button_change_event(ang::platform::events::event_token_t token)
{
	return m_digital_button_change_event -= token;
}

ang::platform::events::event_token_t controller::add_analog_input_change_event(ang::platform::events::event_t e)
{
	if (!e.is_empty() && e->msg_type() == events::core_msg::controller_analog_change)
		return m_analog_input_change_event += e;
	return ang::platform::events::event_token_t();
}

bool controller::remove_analog_input_change_event(ang::platform::events::event_token_t token)
{
	return m_analog_input_change_event -= token;
}

////////////////////////////////////////////////////////////////////////////////////

controller_manager::controller_manager()
	: m_controller_connected_event(this)
	, m_controller_disconnected_event(this)
{
	s_controller_manager = this;
	m_controllers[0] = new input::controller(0);
	m_controllers[1] = new input::controller(1);
	m_controllers[2] = new input::controller(2);
	m_controllers[3] = new input::controller(3);
}

controller_manager::~controller_manager()
{
	s_controller_manager = null;
}

void controller_manager::dispose()
{
	m_controllers[0] = null;
	m_controllers[1] = null;
	m_controllers[2] = null;
	m_controllers[3] = null;
}

icontroller_t controller_manager::controller(uint id)const
{
	return m_controllers[id]->is_enable() ? m_controllers[id].get() : null;
}

void controller_manager::update(float delta)
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));
	for (int i = 0; i < 3; i++) {
		if (XInputGetState(i, &state) == ERROR_SUCCESS)
		{
			if (!m_controllers[i]->is_enable()) {
				m_controllers[i]->init(state);

				events::icontroller_status_args_t args = new events::controller_status_args({
					events::core_msg::controller_status_change,
					(ulong64)m_controllers[i].get(),
					(long64)input::controller_status::connected });
				try {
					m_controller_connected_event.invoke(args.get());
				}
				catch (...) {

				}
			}
			else
			{
				m_controllers[i]->update(delta, state);
			}
			
		}
		else if(m_controllers[i]->is_enable())
		{
			m_controllers[i]->exit();
			events::icontroller_status_args_t args = new events::controller_status_args({
						events::core_msg::controller_status_change,
						(ulong64)m_controllers[i].get(),
						(long64)input::controller_status::disconnected });
			try {
				m_controller_disconnected_event.invoke(args.get());
			}
			catch (...) {

			}
		}
	}
}

ang::platform::events::event_token_t controller_manager::add_controller_connected_event(ang::platform::events::event_t e)
{
	if (!e.is_empty() && e->msg_type() == events::core_msg::controller_status_change)
		return m_controller_connected_event += e;
	return ang::platform::events::event_token_t();
}

bool controller_manager::remove_controller_connected_event(ang::platform::events::event_token_t token)
{
	return m_controller_connected_event -= token;
}

ang::platform::events::event_token_t controller_manager::add_controller_disconnected_event(ang::platform::events::event_t e)
{
	if (!e.is_empty() && e->msg_type() == events::core_msg::controller_status_change)
		return m_controller_disconnected_event += e;
	return ang::platform::events::event_token_t();
}

bool controller_manager::remove_controller_disconnected_event(ang::platform::events::event_token_t token)
{
	return m_controller_disconnected_event -= token;
}

