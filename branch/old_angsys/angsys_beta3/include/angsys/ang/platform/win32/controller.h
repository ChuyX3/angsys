#ifndef __ANG_WIN32_CONTROLLER_H__
#define __ANG_WIN32_CONTROLLER_H__

#include <angsys.h>
#include <ang/core/timer.h>
#include <ang/platform/platform.h>

#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY
#ifdef ANG_PLATFORM_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EX_EXPORTS
#else//#elif defined ANGsys_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace platform
	{
		namespace input
		{
			ang_object(controller);
			ang_object(controller_manager);

			class LINK controller
				: public smart<controller, icontroller>
			{
			private:
				friend controller_manager;
				struct handle; typedef handle *handle_t;
				handle_t m_handle;

				controller(uint id);
				virtual~controller();

				static uint get_device_id_property(base_property<uint>const*);
				static controller_buttons_state_t get_state_property(base_property<controller_buttons_state_t>const*);
				static analog_input_value_t get_triggers_property(base_property<analog_input_value_t>const*);
				static thumb_stick_value_t get_left_thumb_stick_property(base_property<thumb_stick_value_t>const*);
				static thumb_stick_value_t get_right_thumb_stick_property(base_property<thumb_stick_value_t>const*);

				static events::event_token_t add_digital_input_event(events::base_event*, events::event_t);
				static bool remove_digital_input_event(events::base_event*, events::event_token_t);
				static events::event_token_t add_analog_input_event(events::base_event*, events::event_t);
				static bool remove_analog_input_event(events::base_event*, events::event_token_t);

			public:
				ANG_DECLARE_INTERFACE();

				property<const uint, get_device_id_property> device_id;
				property<const controller_buttons_state_t, get_state_property> state;
				property<const analog_input_value_t, get_triggers_property> triggers;
				property<const thumb_stick_value_t, get_left_thumb_stick_property> left_thumb_stick;
				property<const thumb_stick_value_t, get_right_thumb_stick_property> right_thumb_stick;

				events::event<events::icontroller_digital_input_args, add_digital_input_event, remove_digital_input_event> digital_input_event;
				events::event<events::icontroller_analog_input_args, add_analog_input_event, remove_analog_input_event> analog_input_event;

			private:
				virtual uint get_controller_id()const override;
				virtual controller_buttons_state_t get_state()const override;
				virtual thumb_stick_value_t get_left_thumb_stick()const override;
				virtual thumb_stick_value_t get_right_thumb_stick()const override;
				virtual analog_input_value_t get_triggers()const override; //'y' = left, 'x' = right

				virtual events::event_token_t add_digital_input_event(events::event_t)override;
				virtual bool remove_digital_input_event(events::event_token_t)override;
				virtual events::event_token_t add_analog_input_event(events::event_t)override;
				virtual bool remove_analog_input_event(events::event_token_t)override;
			};

			class LINK controller_manager
				: public smart<controller_manager, icontroller_manager, singleton<controller_manager_t>>
			{
			private:
				friend singleton<controller_manager_t>;

				friend controller_manager;
				struct handle; typedef handle *handle_t;
				handle_t m_handle;

				static array_view<const controller_t> get_controller_property(base_property<array_view<const controller_t>> const*);
				static events::event_token_t add_controller_connected_event(events::base_event*, events::event_t);
				static bool remove_controller_connected_event(events::base_event*, events::event_token_t);
				static events::event_token_t add_controller_disconnected_event(events::base_event*, events::event_t);
				static bool remove_controller_disconnected_event(events::base_event*, events::event_token_t);
			
			private:
				controller_manager();
				virtual~controller_manager();

			public:
				ANG_DECLARE_INTERFACE();
			
				property<const array_view<const controller_t>, get_controller_property> contoller;
				events::event<events::icontroller_status_args, add_controller_connected_event, remove_controller_connected_event> controller_connected_event;
				events::event<events::icontroller_status_args, add_controller_disconnected_event, remove_controller_disconnected_event> controller_disconnected_event;

				void update();

			private:
				virtual icontroller_t get_controller(uint)const override;
				virtual events::event_token_t add_controller_connected_event(events::event_t)override;
				virtual bool remove_controller_connected_event(events::event_token_t)override;
				virtual events::event_token_t add_controller_disconnected_event(events::event_t)override;
				virtual bool remove_controller_disconnected_event(events::event_token_t)override;

			};

		}
	}
}


#endif//WINDOWS_PLATFORM
#endif//__ANG_WIN32_CONTROLLER_H__
