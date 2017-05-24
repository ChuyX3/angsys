/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_PLATFORM_H__
#define __ANG_PLATFORM_H__

#include <angsys.h>
#include <ang/foundation.h>
#include <ang/core/async.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY
#ifdef ANG_PLATFORM_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANG_PLATFORM_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace platform
	{
		struct icore_app;
		struct icore_view;
		struct icore_context;
		struct imessage_reciever;
		struct icore_msg_dispatcher;

		typedef intf_wrapper<icore_app> icore_app_t;
		typedef intf_wrapper<icore_view> icore_view_t;
		typedef intf_wrapper<icore_context> icore_context_t;
		typedef intf_wrapper<imessage_reciever> imessage_reciever_t;
		typedef intf_wrapper<icore_msg_dispatcher> icore_msg_dispatcher_t;

		ANG_BEGIN_ENUM(LINK, activate_status, uint)
			deactivated,
			activated
		ANG_END_ENUM(activate_status);

		ANG_BEGIN_ENUM(LINK, display_invalidate_reason, uint)
			none = 0,
			display_invalidate,
			dpi_change,
			scale_factor_changed,
			orientation_changed,
			size_changed,
		ANG_END_ENUM(display_invalidate_reason);

		namespace display
		{
			ANG_BEGIN_ENUM(LINK, orientation, uint)
				none,
				landscape = 1,
				portrait = 2,
				landscape_flipped = 4,
				portrait_flipped = 8,
			ANG_END_ENUM(orientation);

			typedef struct display_info
			{
				property<const orientation_t, display_info > current_orientation;
				property<const orientation_t, display_info > native_orientation;
				property<const graphics::size<float>, display_info > display_resolution;
				property<const graphics::size<float>, display_info > display_scale_factor;
				property<const double, display_info > dot_per_inche;
			}display_info_t;
		}//dysplay
	
		namespace events
		{
			class message;
			struct imsg_event_args;

			typedef uint core_msg_t;
			typedef object_wrapper<message> message_t;
			typedef intf_wrapper<imsg_event_args> imsg_event_args_t;

			ANG_BEGIN_ENUM(LINK, core_msg_enum, core_msg_t)
				none = 0x0000,
				created = 0x0001,
				destroyed = 0x0002,
				size = 0x0005,
				activate = 0x0006,
				got_focus = 0x0007,
				lost_focus = 0x0008,
				draw = 0x000F,
				begin_drawing = 0x0407,
				end_drawing = 0x0408,
				visibility_change = 0x0018,

				display_change = 0x007E,
				key_down = 0x0100,
				key_up = 0x0101,
				pointer_moved = 0X0245,
				pointer_pressed = 0X0246,
				pointer_released = 0X0247,
				pointer_enter = 0x0249,
				pointer_leave = 0x024A,

				start_app = 0x0403,
				exit_app = 0x0404,
				orientation = 0x0405,
				initial_update = 0x0406,
				update = 0x0407,
				text_change = 0x0408,

				user_msg = 0x0450,
			ANG_END_ENUM(core_msg_enum);

			typedef core::delegates::ifunction<void, object*, imsg_event_args*> ievent;
			typedef core::delegates::function_data <void(object*, imsg_event_args*)> event;
			typedef core::delegates::function <void(object*, imsg_event_args*)> event_t;
			typedef core::delegates::listener <void(object*, imsg_event_args*)> event_listener;

			struct icreated_event_args;
			struct ivisibility_change_event_args;
			struct idisplay_info_event_args;
			struct iactivate_event_args;
			struct idraw_event_args;
			struct ikey_event_args;
			struct ipointer_event_args;
			struct iapp_status_event_args;
			struct iupdate_event_args;
			struct itext_change_event_args;

			typedef intf_wrapper<imsg_event_args> imsg_event_args_t;
			typedef intf_wrapper<icreated_event_args> icreated_event_args_t;
			typedef intf_wrapper<ivisibility_change_event_args> ivisibility_change_event_args_t;
			typedef intf_wrapper<idisplay_info_event_args> idisplay_info_event_args_t;
			typedef intf_wrapper<iactivate_event_args> iactivate_event_args_t;
			typedef intf_wrapper<idraw_event_args> idraw_event_args_t;
			typedef intf_wrapper<ikey_event_args> ikey_event_args_t;
			typedef intf_wrapper<ipointer_event_args> ipointer_event_args_t;
			typedef intf_wrapper<iapp_status_event_args> iapp_status_event_args_t;
			typedef intf_wrapper<iupdate_event_args> iupdate_event_args_t;
			typedef intf_wrapper<itext_change_event_args> itext_change_event_args_t;
		}

	}
}

#include <ang/platform/input.h>
#include <ang/platform/events.h>
#include <ang/platform/core_msg.h>

namespace ang
{
	namespace platform
	{
		
		ANG_BEGIN_INTERFACE(LINK, imessage_reciever)
			visible vcall void send_msg(events::message_t) pure;
			visible vcall core::async::iasync_t<dword> post_msg(events::message_t) pure;
			visible vcall bool listen_to(events::event_t) pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, icore_msg_dispatcher, public imessage_reciever)
			visible vcall bool dispatch_msg() pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE(LINK, icore_context)
			visible vcall  pointer get_core_context_handle()const pure;
			visible vcall  bool bind_graphic_native_surface(pointer)pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE(LINK, icore_view)
			visible vcall pointer get_core_view_handle()const pure;
			visible vcall icore_context_t get_core_context()const pure;
			visible vcall graphics::size<float> get_core_view_size()const pure;
			visible vcall graphics::size<float> get_core_view_scale_factor()const pure;
			visible vcall imessage_reciever_t get_listener()const pure;
		ANG_END_INTERFACE();

		ANG_BEGIN_INTERFACE(LINK, icore_app)
			visible vcall pointer get_core_app_handle()const pure;
			visible vcall icore_view_t get_main_core_view() pure;
			visible vcall input::ikeyboard_t get_keyboard()pure;
			visible vcall imessage_reciever_t get_listener()const pure;
		ANG_END_INTERFACE();
	}
}


ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::events::core_msg_enum_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::activate_status_t);

ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::display::display_info_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::display::orientation_t);

ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::input::poiner_info_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::input::key_modifiers_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::input::text_selection_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::input::keyboard_type_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::input::pointer_hardware_type_t);


#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_PLATFORM_H__