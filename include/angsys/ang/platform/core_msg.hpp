/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_PLATFORM_HPP__
#error ...
#elif !defined __ANG_PLATFORM_MSG_H__
#define __ANG_PLATFORM_MSG_H__

namespace ang
{
	namespace platform
	{
		namespace events
		{
			class LINK message final
				: public object
			{
			private:
				core_msg_t _msg;
				var_args_t _args;
				mutable dword _result;

			public:
				message(const message& m);
				message(const message* m);
				message(core_msg_t m = 0, ulong64 wparam = 0, long64 lparam = 0);
				//template<typename... Ts>
				//message(core_msg_t m, Ts... args) : _msg(m) {
				//	_args = var_args{ 0_l , 0_ul, move(args ...) };
				//}


			public: //overrides
				ANG_DECLARE_INTERFACE();

			public: //properties
				core_msg_t msg()const;
				windex push_arg(objptr);
				ulong64 wparam()const;
				long64 lparam()const;
				objptr arg(windex i)const;
				array_view<objptr> args()const;
				dword result()const;
				void result(dword)const;

			private:
				virtual~message();
			};

			ANG_BEGIN_INTERFACE(LINK, imsg_event_args)
				visible vcall const message_t& msg_info()const pure;
				visible vcall void handled(bool) pure;
				visible vcall bool handled()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, icreated_event_args, public imsg_event_args)
				visible vcall icore_view_t core_view()const pure;
				visible vcall icore_app_t core_app()const pure;
				visible vcall var_args_t args_list()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ivisibility_change_event_args, public imsg_event_args)
				visible vcall icore_view_t core_view()const pure;
				visible vcall bool is_visible()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, idisplay_info_event_args, public imsg_event_args)
				visible vcall icore_view_t core_view()const pure;
				visible vcall display_invalidate_reason_t invalidate_reason()const pure;
				visible vcall display::display_info_t display_info()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, iactivate_event_args, public imsg_event_args)
				visible vcall activate_status_t status()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, idraw_event_args, imsg_event_args)
				visible vcall icore_view_t core_view()const pure;
				visible vcall icore_context_t core_context()const pure;
				visible vcall graphics::size<float> canvas_size()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ikey_event_args, public imsg_event_args)
				visible vcall uint key()const pure;
				visible vcall input::key_modifiers_t modifiers()const pure;
				visible vcall word flags()const pure;
				visible vcall bool is_pressed()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, ipointer_event_args, public imsg_event_args)
				visible vcall graphics::point<float> position()const pure;
				visible vcall input::key_modifiers_t modifiers()const pure;
				visible vcall input::poiner_info_t info()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, iapp_status_event_args, public imsg_event_args)
				visible vcall icore_app_t core_app()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, iupdate_event_args, public imsg_event_args)
				visible vcall shared_ptr<core::time::step_timer> step_timer()const pure;
			ANG_END_INTERFACE();

			ANG_BEGIN_INTERFACE_WITH_BASE(LINK, itext_change_event_args, public imsg_event_args)
				visible vcall mstring text()const pure;
				visible vcall input::ikeyboard_t keyboard()const pure;
			ANG_END_INTERFACE();
		}
	}
}


namespace ang
{
	namespace platform
	{
		namespace events
		{

			class start_app_event
				: public event
			{
			public:
				template<class obj_t>
				start_app_event(obj_t* o, void(obj_t::*f)(object_t, iapp_status_event_args_t))
					: event((core_msg_t)core_msg_enum::start_app) {
					set<iapp_status_event_args_t>(o, f);
				}
				template<class calleable_t>
				start_app_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::start_app) {
					set<iapp_status_event_args_t>(f);
				}
			};

			class exit_app_event
				: public event
			{
			public:
				template<class obj_t>
				exit_app_event(obj_t* o, void(obj_t::*f)(object_t, iapp_status_event_args_t))
					: event((core_msg_t)core_msg_enum::exit_app) {
					set<iapp_status_event_args_t>(o, f);
				}
				template<class calleable_t>
				exit_app_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::exit_app) {
					set<iapp_status_event_args_t>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class created_event
				: public event
			{
			public:
				template<class obj_t>
				created_event(obj_t* o, void(obj_t::*f)(object_t, icreated_event_args_t))
					: event((core_msg_t)core_msg_enum::created) {
					set<icreated_event_args>(o, f);
				}
				template<class calleable_t>
				created_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::created) {
					set<icreated_event_args>(f);
				}
			};

			class destroyed_event
				: public event
			{
			public:
				template<class obj_t>
				destroyed_event(obj_t* o, void(obj_t::*f)(object_t, imsg_event_args_t))
					: event((core_msg_t)core_msg_enum::destroyed) {
					set<imsg_event_args>(o, f);
				}
				template<class calleable_t>
				destroyed_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::destroyed) {
					set<imsg_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class update_event
				: public event
			{
			public:
				template<class obj_t>
				update_event(obj_t* o, void(obj_t::*f)(object_t, iupdate_event_args_t))
					: event((core_msg_t)core_msg_enum::update) {
					set<iupdate_event_args>(o, f);
				}
				template<class calleable_t>
				update_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::update) {
					set<iupdate_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class draw_event
				: public event
			{
			public:
				template<class obj_t>
				draw_event(obj_t* o, void(obj_t::*f)(object_t, idraw_event_args_t))
					: event((core_msg_t)core_msg_enum::draw) {
					set<idraw_event_args>(o, f);
				}
				template<class calleable_t>
				draw_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::draw) {
					set<idraw_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class activate_event
				: public event
			{
			public:
				template<class obj_t>
				activate_event(obj_t* o, void(obj_t::*f)(object_t, iactivate_event_args_t))
					: event((core_msg_t)core_msg_enum::activate) {
					set<iactivate_event_args>(o, f);
				}
				template<class calleable_t>
				activate_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::activate) {
					set<iactivate_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class display_size_change_event
				: public event
			{
			public:
				template<class obj_t>
				display_size_change_event(obj_t* o, void(obj_t::*f)(object_t, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::size) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class calleable_t>
				display_size_change_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::size) {
					set<idisplay_info_event_args>(f);
				}
			};

			class display_orientation_change_event
				: public event
			{
			public:
				template<class obj_t>
				display_orientation_change_event(obj_t* o, void(obj_t::*f)(object_t, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::orientation) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class calleable_t>
				display_orientation_change_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::orientation) {
					set<idisplay_info_event_args>(f);
				}
			};

			class display_invalidate_event
				: public event
			{
			public:
				template<class obj_t>
				display_invalidate_event(obj_t* o, void(obj_t::*f)(object_t, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::display_change) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class calleable_t>
				display_invalidate_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::display_change) {
					set<idisplay_info_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class key_pressed_event
				: public event
			{
			public:
				template<class obj_t>
				key_pressed_event(obj_t* o, void(obj_t::*f)(object_t, ikey_event_args_t))
					: event((core_msg_t)core_msg_enum::key_down) {
					set<ikey_event_args>(o, f);
				}
				template<class calleable_t>
				key_pressed_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::key_down) {
					set<ikey_event_args>(f);
				}
			};

			class key_released_event
				: public event
			{
			public:
				template<class obj_t>
				key_released_event(obj_t* o, void(obj_t::*f)(object_t, ikey_event_args_t))
					: event((core_msg_t)core_msg_enum::key_up) {
					set<ikey_event_args>(o, f);
				}
				template<class calleable_t>
				key_released_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::key_up) {
					set<ikey_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////


			class pointer_moved_event
				: public event
			{
			public:
				template<class obj_t>
				pointer_moved_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_moved) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				pointer_moved_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::pointer_moved) {
					set<ipointer_event_args>(f);
				}
			};

			class pointer_pressed_event
				: public event
			{
			public:
				template<class obj_t>
				pointer_pressed_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_pressed) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				pointer_pressed_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::pointer_pressed) {
					set<ipointer_event_args>(f);
				}
			};

			class pointer_released_event
				: public event
			{
			public:
				template<class obj_t>
				pointer_released_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_released) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				pointer_released_event(calleable_t f)
					: event((core_msg_t)core_msg_enum::pointer_released) {
					set<iupdate_event_args>(f);
				}
			};
		}
	}
}

#endif//__ANG_PLATFORM_MSG_H__