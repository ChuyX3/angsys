/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_PLATFORM_H__
#error ...
#elif !defined __ANG_PLATFORM_MSG_H__
#define __ANG_PLATFORM_MSG_H__

namespace ang
{
	namespace platform
	{
		namespace events
		{
			class LINK message
			{
			private:
				core_msg_t m_msg;
				ulong64 m_wparam;
				long64 m_lparam;
				mutable dword m_result;

			public:
				message(const message& m);
				message(const message* m);
				message(core_msg_t m = core_msg::none, ulong64 wparam = 0, long64 lparam = 0);
				~message();
				
			public: //properties
				core_msg_t msg()const;
				ulong64 wparam()const;
				long64 lparam()const;
				dword result()const;
				void result(dword)const;

				friend inline bool operator == (message const& m1, message const& m2) {
					return m1.m_msg == m2.m_msg;
				}
				friend inline bool operator != (message const& m1, message const& m2) {
					return m1.m_msg != m2.m_msg;
				}
			};

			struct nvt LINK imsg_event_args
				: intf<imsg_event_args
				, iid("ang::platform::events::imsg_event_args")>
			{
				virtual const message& msg()const = 0;
				virtual void handled(bool) = 0;
				virtual bool handled()const = 0;
			};

			struct nvt LINK icreated_event_args 
				: intf<icreated_event_args
				, iid("ang::platform::events::icreated_event_args")
				, imsg_event_args>
			{
				virtual icore_view_t core_view()const = 0;
				virtual icore_app_t core_app()const = 0;
				virtual smart<var[]> args_list()const = 0;
			};

			struct nvt LINK ivisibility_change_event_args
				: intf<ivisibility_change_event_args
				, iid("ang::platform::events::ivisibility_change_event_args")
				, imsg_event_args>
			{
				virtual icore_view_t core_view()const = 0;
				virtual bool is_visible()const = 0;
			};

			struct nvt LINK idisplay_info_event_args
				: intf<idisplay_info_event_args
				, iid("ang::platform::events::idisplay_info_event_args")
				, imsg_event_args>
			{
				virtual icore_view_t core_view()const = 0;
				virtual display_invalidate_reason_t const& invalidate_reason()const = 0;
				virtual display::display_info_t const& display_info()const = 0;
			};

			struct nvt LINK iactivate_event_args
				: intf<iactivate_event_args
				, iid("ang::platform::events::iactivate_event_args")
				, imsg_event_args>
			{
				virtual activate_status_t const& status()const = 0;
			};

			struct nvt LINK idraw_event_args
				: intf<idraw_event_args
				, iid("ang::platform::events::idraw_event_args")
				, imsg_event_args>
			{
				virtual icore_view_t core_view()const = 0;
				virtual graphics::icore_context_t core_context()const = 0;
				virtual graphics::size<float> const& canvas_size()const = 0;
			};

			struct nvt LINK ikey_event_args
				: intf<ikey_event_args
				, iid("ang::platform::events::ikey_event_args")
				, imsg_event_args>
			{
				virtual char32_t key()const = 0;
				virtual input::key_info_t const& info()const = 0;
			};

			struct nvt LINK ipointer_event_args
				: intf<ipointer_event_args
				, iid("ang::platform::events::ipointer_event_args")
				, imsg_event_args>
			{
				virtual graphics::point<float> const& position()const = 0;
				virtual input::key_modifiers_t const& modifiers()const = 0;
				virtual input::poiner_info_t const& info()const = 0;
			};

			struct nvt LINK iapp_status_event_args
				: intf<iapp_status_event_args
				, iid("ang::platform::events::iapp_status_event_args")
				, imsg_event_args>
			{
				virtual icore_app_t core_app()const = 0;
			};

			struct nvt LINK itext_change_event_args
				: intf<itext_change_event_args
				, iid("ang::platform::events::itext_change_event_args")
				, imsg_event_args>
			{
				virtual text::istring_t text()const = 0;
				virtual input::ikeyboard_t keyboard()const = 0;
			};

			struct nvt LINK icontroller_status_args
				: intf<icontroller_status_args
				, iid("ang::platform::events::icontroller_status_args")
				, imsg_event_args>
			{
				virtual uint controller_id()const = 0;
				virtual input::icontroller_t controller()const = 0;
				virtual input::controller_status_t status()const = 0;
			};

			struct nvt LINK icontroller_digital_input_args
				: intf<icontroller_digital_input_args
				, iid("ang::platform::events::icontroller_digital_input_args")
				, imsg_event_args>
			{
				virtual input::icontroller_t controller()const = 0;
				virtual input::controller_button_t button()const = 0;
				virtual input::controller_button_state_t state()const = 0;
			};

			struct nvt LINK icontroller_analog_input_args
				: intf<icontroller_analog_input_args
				, iid("ang::platform::events::icontroller_analog_input_args")
				, imsg_event_args>
			{
				virtual input::icontroller_t controller()const = 0;
				virtual input::controller_button_t button()const = 0;
				virtual input::analog_input_state_t state()const = 0;
			};

			ang_declare_interface(icontroller_analog_input_args);
		}
	}
}


#endif//__ANG_PLATFORM_MSG_H__
