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

			ang_begin_interface(LINK imsg_event_args)
				visible vcall const message& msg()const pure;
				visible vcall void handled(bool) pure;
				visible vcall bool handled()const pure;
			ang_end_interface();

			ang_begin_interface(LINK icreated_event_args, imsg_event_args)
				visible vcall icore_view_t core_view()const pure;
				visible vcall icore_app_t core_app()const pure;
				visible vcall var_args_t args_list()const pure;
			ang_end_interface();

			ang_begin_interface(LINK ivisibility_change_event_args, imsg_event_args)
				visible vcall icore_view_t core_view()const pure;
				visible vcall bool is_visible()const pure;
			ang_end_interface();

			ang_begin_interface(LINK idisplay_info_event_args, imsg_event_args)
				visible vcall icore_view_t core_view()const pure;
				visible vcall display_invalidate_reason_t const& invalidate_reason()const pure;
				visible vcall display::display_info_t const& display_info()const pure;
			ang_end_interface();

			ang_begin_interface(LINK iactivate_event_args, imsg_event_args)
				visible vcall activate_status_t const& status()const pure;
			ang_end_interface();

			ang_begin_interface(LINK idraw_event_args, imsg_event_args)
				visible vcall icore_view_t core_view()const pure;
				visible vcall icore_context_t core_context()const pure;
				visible vcall graphics::size<float> const& canvas_size()const pure;
			ang_end_interface();

			ang_begin_interface(LINK ikey_event_args, imsg_event_args)
				visible vcall char32_t key()const pure;
				visible vcall input::key_info_t const& info()const pure;
			ang_end_interface();

			ang_begin_interface(LINK ipointer_event_args, imsg_event_args)
				visible vcall graphics::point<float> const& position()const pure;
				visible vcall input::key_modifiers_t const& modifiers()const pure;
				visible vcall input::poiner_info_t const& info()const pure;
			ang_end_interface();

			ang_begin_interface(LINK iapp_status_event_args, imsg_event_args)
				visible vcall icore_app_t core_app()const pure;
			ang_end_interface();

			ang_begin_interface(LINK itext_change_event_args, imsg_event_args)
				visible vcall text::istring_t text()const pure;
				visible vcall input::ikeyboard_t keyboard()const pure;
			ang_end_interface();

			ang_begin_interface(LINK icontroller_status_args, imsg_event_args)
				visible vcall uint controller_id()const pure;
				visible vcall input::icontroller_t controller()const pure;
				visible vcall input::controller_status_t status()const pure;
			ang_end_interface();

			ang_begin_interface(LINK icontroller_digital_input_args, imsg_event_args)
				visible vcall input::icontroller_t controller()const pure;
				visible vcall input::controller_button_t button()const pure;
				visible vcall input::controller_button_state_t state()const pure;
			ang_end_interface();

			ang_begin_interface(LINK icontroller_analog_input_args, imsg_event_args)
				visible vcall input::icontroller_t controller()const pure;
				visible vcall input::controller_button_t button()const pure;
				visible vcall input::analog_input_state_t state()const pure;
			ang_end_interface();

			
			ang_interface(icontroller_analog_input_args);
		}
	}
}


#endif//__ANG_PLATFORM_MSG_H__