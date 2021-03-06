/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_PLATFORM_H__
#define __ANG_PLATFORM_H__

#include <angsys.h>
#include <ang/core/files.h>
#include <ang/core/async.h>
#include <ang/graphics/foundations.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANG_DYNAMIC_LIBRARY
#ifdef ANG_PLATFORM_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_PLATFORM_EXPORTS
#else//#elif defined ANG_STATIC_LIBRARY
#define LINK
#endif//ANG_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace platform
	{
		ang_declare_interface(icore_app);
		ang_declare_interface(icore_view);
		ang_declare_interface(imessage_listener);
		ang_declare_interface(imessage_dispatcher);
		//ang_declare_interface(icore_msg_dispatcher);

		declare_enum(LINK, activate_status, uint)
		{
			deactivated,
			activated
		};

		declare_enum(LINK, display_invalidate_reason, uint)
		{
			none = 0,
			display_invalidate,
			dpi_change,
			scale_factor_changed,
			orientation_changed,
			size_changed,
		};

		namespace display
		{
			declare_enum(LINK, orientation, uint)
			{
				none,
				landscape = 1,
				portrait = 2,
				landscape_flipped = 4,
				portrait_flipped = 8,
			};

			typedef struct display_info
			{
				orientation_t current_orientation;
				orientation_t native_orientation;
				graphics::size<float> display_resolution;
				graphics::size<float> display_scale_factor;
				double dot_per_inche;
			}display_info_t;
		}//dysplay
	
		namespace events
		{	
			ang_declare_interface(imsg_event_args);
			ang_declare_interface(icreated_event_args);
			ang_declare_interface(ivisibility_change_event_args);
			ang_declare_interface(idisplay_info_event_args);
			ang_declare_interface(iactivate_event_args);
			ang_declare_interface(idraw_event_args);
			ang_declare_interface(ikey_event_args);
			ang_declare_interface(iapp_status_event_args);
			ang_declare_interface(itext_change_event_args);

			class message;
			//typedef uint core_msg_t;
			//typedef object_wrapper<message> message_t;

			declare_enum(LINK, core_msg, uint)
			{	
				none = 0x0000,
				created = 0x0001,
				destroyed = 0x0002,
				size = 0x0005,
				activate = 0x0006,
				got_focus = 0x0007,
				lost_focus = 0x0008,
				draw = 0x000F,
				close = 0x0010,
				begin_drawing = 0x0407,
				end_drawing = 0x0408,
				visibility_change = 0x0018,

				display_change = 0x007E,
				key_down = 0x0100,
				key_up = 0x0101,
				put_char = 0x0102,
				pointer_moved = 0X0245,
				pointer_pressed = 0X0246,
				pointer_released = 0X0247,
				pointer_entered = 0x0249,
				pointer_leaved = 0x024A,
				pointer_canceled = 0x024C,

				mouse_move = 0x0200,
				lbutton_down = 0x0201,
				lbutton_up = 0x0202,
				lbutton_dbl_clk = 0x0203,
				rbutton_down = 0x0204,
				rbutton_up = 0x0205,
				rbutton_dbl_clk = 0x0206,
				mbutton_down = 0x0207,
				mbutton_up = 0x0208,
				mbutton_dbl_clk = 0x0209,

				system_reserved_event = 0x0401,
				start_app = 0x0403,
				exit_app = 0x0404,
				orientation = 0x0405,
				initial_update = 0x0406,
				update = 0x0407,
				text_change = 0x0408,
				user_msg = 0x0450,

				controller_status_change = 0x0460,
				controller_button_change = 0x0461,
				controller_analog_change = 0x0463,
			};
			
			typedef delegates::ifunction<void(object*, imsg_event_args*)> ievent;
			typedef delegates::function_base<void(object*, imsg_event_args*)> base_event_handler;
			typedef delegates::function <void(object*, imsg_event_args*)> event_t;
			typedef delegates::listener <void(object*, imsg_event_args*)> event_listener;
			typedef delegates::listen_token<void(object*, imsg_event_args*)> event_token, event_token_t;

			template<typename T, core_msg MSG> class event_handler;
		
			ang_declare_interface(icreated_event_args);
			ang_declare_interface(ivisibility_change_event_args);
			ang_declare_interface(idisplay_info_event_args);
			ang_declare_interface(idisplay_info_event_args);
			ang_declare_interface(iactivate_event_args);
			ang_declare_interface(idraw_event_args);
			ang_declare_interface(ikey_event_args);
			ang_declare_interface(ipointer_event_args);
			ang_declare_interface(iapp_status_event_args);
			ang_declare_interface(itext_change_event_args);

			ang_declare_interface(icontroller_status_args);
			ang_declare_interface(icontroller_digital_input_args);
			ang_declare_interface(icontroller_analog_input_args);

			using start_app_event = event_handler<iapp_status_event_args, core_msg::start_app>;
			using exit_app_event = event_handler<iapp_status_event_args, core_msg::exit_app>;
			using created_event = event_handler<icreated_event_args, core_msg::created>;
			using initialize_event = event_handler<icreated_event_args, core_msg::initial_update>;
			using closed_event = event_handler<imsg_event_args, core_msg::close>;
			using destroyed_event = event_handler<imsg_event_args, core_msg::destroyed>;
			using update_event = event_handler<imsg_event_args, core_msg::update>;
			using draw_event = event_handler<idraw_event_args, core_msg::draw>;
			using activate_event = event_handler<iactivate_event_args, core_msg::activate>;
			using visibility_change_event = event_handler<ivisibility_change_event_args, core_msg::visibility_change>;
			using display_size_change_event = event_handler<idisplay_info_event_args, core_msg::size>;
			using display_orientation_change_event = event_handler<idisplay_info_event_args, core_msg::orientation>;
			using display_invalidate_event = event_handler<idisplay_info_event_args, core_msg::display_change>;
			using key_pressed_event = event_handler<ikey_event_args, core_msg::key_down>;
			using key_released_event = event_handler<ikey_event_args, core_msg::key_up>;
			using text_changed_event = event_handler<itext_change_event_args, core_msg::text_change>;
			using pointer_moved_event = event_handler<ipointer_event_args, core_msg::pointer_moved>;
			using pointer_pressed_event = event_handler<ipointer_event_args, core_msg::pointer_pressed>;
			using pointer_released_event = event_handler<ipointer_event_args, core_msg::pointer_released>;
			using pointer_entered_event = event_handler<ipointer_event_args, core_msg::pointer_entered>;
			using pointer_leaved_event = event_handler<ipointer_event_args, core_msg::pointer_leaved>;
			using controller_status_change_event = event_handler<icontroller_status_args, core_msg::controller_status_change>;
			using controller_button_change_event = event_handler<icontroller_digital_input_args, core_msg::controller_button_change>;
			using controller_analog_change_event = event_handler<icontroller_analog_input_args, core_msg::controller_analog_change>;

			using mouse_moved_event = event_handler<ipointer_event_args, core_msg::mouse_move>;
			using mouse_lbutton_down_event = event_handler<ipointer_event_args, core_msg::lbutton_down>;
			using mouse_lbutton_up_event = event_handler<ipointer_event_args, core_msg::lbutton_up>;
			using mouse_lbutton_dblclick_event = event_handler<ipointer_event_args, core_msg::lbutton_dbl_clk>;
			using mouse_rbutton_down_event = event_handler<ipointer_event_args, core_msg::rbutton_down>;
			using mouse_rbutton_up_event = event_handler<ipointer_event_args, core_msg::rbutton_up>;
			using mouse_rbutton_dblclick_event = event_handler<ipointer_event_args, core_msg::rbutton_dbl_clk>;
		}
	}
}

#include <ang/platform/defines.h>
#include <ang/platform/events.h>
#include <ang/platform/input.h>
#include <ang/platform/core_msg.h>

namespace ang
{
	namespace platform
	{
		struct nvt LINK imessage_listener
			: intf<imessage_listener
			, iid("ang::platform::imessage_listener")
			, core::async::idispatcher>
		{
			virtual dword send_msg(events::message) = 0;
			virtual core::async::iasync<dword> post_msg(events::message) = 0;
			virtual events::event_token_t notify(events::event_t) = 0;
			virtual bool notify(events::core_msg_t, events::event_token_t) = 0;
		};

		//struct nvt LINK imessage_dispatcher
		//	: intf<imessage_dispatcher
		//	, iid("ang::platform::imessage_dispatcher")
		//	, imessage_listener>
		//{
		//	virtual dword dispatch_msg(events::message) = 0;
		//	virtual core::async::iasync<void> post_task(function<void(void)>) = 0;
		//};

		struct nvt LINK icore_view
			: intf<icore_view
			, iid("ang::platform::icore_view")>
		{
			virtual pointer core_view_handle()const = 0;
			virtual graphics::icore_context_t core_context()const = 0;
			virtual graphics::size<float> core_view_size()const = 0;
			virtual graphics::size<float> core_view_scale_factor()const = 0;
			virtual imessage_listener_t dispatcher()const = 0;
		};

		struct nvt LINK icore_app
			: intf<icore_app
			, iid("ang::platform::icore_app")>
		{
			static icore_app_t current_app();
			virtual pointer core_app_handle()const = 0;
			virtual icore_view_t core_view() = 0;
			virtual input::ikeyboard_t keyboard() = 0;
			virtual ivar property(astring name)const = 0;
			virtual void property(astring name, ivar) = 0;
		};
	}
}

#include <ang/platform/app.h>
#include <ang/platform/controller.h>

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::start_app_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::exit_app_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::created_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::initialize_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::closed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::destroyed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::update_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::draw_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::activate_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::visibility_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::display_size_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::display_orientation_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::display_invalidate_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::key_pressed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::key_released_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::text_changed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::pointer_moved_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::pointer_pressed_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::pointer_released_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::pointer_entered_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::pointer_leaved_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::controller_status_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::controller_button_change_event);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ang::platform::events::controller_analog_change_event);

ANG_ENUM_DECLARATION(LINK, ang::platform::activate_status)
ANG_ENUM_DECLARATION(LINK, ang::platform::display_invalidate_reason)
ANG_ENUM_DECLARATION(LINK, ang::platform::display::orientation)
ANG_ENUM_DECLARATION(LINK, ang::platform::events::core_msg)
ANG_FLAGS_DECLARATION(LINK, ang::platform::input::key_modifiers)
ANG_ENUM_DECLARATION(LINK, ang::platform::input::pointer_hardware_type)
ANG_ENUM_DECLARATION(LINK, ang::platform::input::keyboard_type)
ANG_ENUM_DECLARATION(LINK, ang::platform::input::key_state)
ANG_ENUM_DECLARATION(LINK, ang::platform::input::virtual_key)
ANG_ENUM_FLAGS_DECLARATION(LINK, ang::platform::input::controller_button)
ANG_ENUM_DECLARATION(LINK, ang::platform::input::controller_status)

#ifdef  LINK
#undef  LINK
#endif//LINK

#include <ang/platform/inline/platform.inl>

#endif//__ANG_PLATFORM_HPP__
