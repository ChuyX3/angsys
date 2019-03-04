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
				message(core_msg_t m = 0, ulong64 wparam = 0, long64 lparam = 0);
				~message();
				
			public: //properties
				core_msg_t msg()const;
				ulong64 wparam()const;
				long64 lparam()const;
				dword result()const;
				void result(dword)const;
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

			ang_begin_interface(LINK iupdate_event_args, imsg_event_args)
				visible vcall double delta()const pure;
				visible vcall double total()const pure;
			ang_end_interface();

			ang_begin_interface(LINK itext_change_event_args, imsg_event_args)
				visible vcall text::istring_t text()const pure;
				visible vcall input::ikeyboard_t keyboard()const pure;
			ang_end_interface();
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
				template<class T>
				start_app_event(T* o, void(T::*f)(objptr, iapp_status_event_args_t))
					: event((core_msg_t)core_msg_enum::start_app) {
					set<iapp_status_event_args_t>(o, f);
				}
				template<class T>
				start_app_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, iapp_status_event_args_t))
					: event((core_msg_t)core_msg_enum::start_app) {
					set<iapp_status_event_args_t>(o, f);
				}
				template<class F>
				start_app_event(F f)
					: event((core_msg_t)core_msg_enum::start_app) {
					set<iapp_status_event_args_t>(f);
				}
			};

			class exit_app_event
				: public event
			{
			public:
				template<class T>
				exit_app_event(T* o, void(T::*f)(objptr, iapp_status_event_args_t))
					: event((core_msg_t)core_msg_enum::exit_app) {
					set<iapp_status_event_args_t>(o, f);
				}
				template<class T>
				exit_app_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, iapp_status_event_args_t))
					: event((core_msg_t)core_msg_enum::exit_app) {
					set<iapp_status_event_args_t>(o, f);
				}
				template<class F>
				exit_app_event(F f)
					: event((core_msg_t)core_msg_enum::exit_app) {
					set<iapp_status_event_args_t>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class created_event
				: public event
			{
			public:
				template<class T>
				created_event(T* o, void(T::*f)(objptr, icreated_event_args_t))
					: event((core_msg_t)core_msg_enum::created) {
					set<icreated_event_args>(o, f);
				}
				template<class T>
				created_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, icreated_event_args_t))
					: event((core_msg_t)core_msg_enum::created) {
					set<icreated_event_args>(o, f);
				}
				template<class F>
				created_event(F f)
					: event((core_msg_t)core_msg_enum::created) {
					set<icreated_event_args>(f);
				}
			};

			class closed_event
				: public event
			{
			public:
				template<class T>
				closed_event(T* o, void(T::*f)(objptr, imsg_event_args_t))
					: event((core_msg_t)core_msg_enum::close) {
					set<imsg_event_args>(o, f);
				}
				template<class T>
				closed_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, imsg_event_args_t))
					: event((core_msg_t)core_msg_enum::close) {
					set<imsg_event_args>(o, f);
				}
				template<class F>
				closed_event(F f)
					: event((core_msg_t)core_msg_enum::close) {
					set<imsg_event_args>(f);
				}
			};

			class destroyed_event
				: public event
			{
			public:
				template<class T>
				destroyed_event(T* o, void(T::*f)(objptr, imsg_event_args_t))
					: event((core_msg_t)core_msg_enum::destroyed) {
					set<imsg_event_args>(o, f);
				}
				template<class T>
				destroyed_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, imsg_event_args_t))
					: event((core_msg_t)core_msg_enum::destroyed) {
					set<imsg_event_args>(o, f);
				}
				template<class F>
				destroyed_event(F f)
					: event((core_msg_t)core_msg_enum::destroyed) {
					set<imsg_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class update_event
				: public event
			{
			public:
				template<class T>
				update_event(T* o, void(T::*f)(objptr, iupdate_event_args_t))
					: event((core_msg_t)core_msg_enum::update) {
					set<iupdate_event_args>(o, f);
				}
				template<class T>
				update_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, iupdate_event_args_t))
					: event((core_msg_t)core_msg_enum::update) {
					set<iupdate_event_args>(o, f);
				}
				template<class F>
				update_event(F f)
					: event((core_msg_t)core_msg_enum::update) {
					set<iupdate_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class draw_event
				: public event
			{
			public:
				template<class T>
				draw_event(T* o, void(T::*f)(objptr, idraw_event_args_t))
					: event((core_msg_t)core_msg_enum::draw) {
					set<idraw_event_args>(o, f);
				}
				template<class T>
				draw_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, idraw_event_args_t))
					: event((core_msg_t)core_msg_enum::draw) {
					set<idraw_event_args>(o, f);
				}
				template<class F>
				draw_event(F f)
					: event((core_msg_t)core_msg_enum::draw) {
					set<idraw_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class activate_event
				: public event
			{
			public:
				template<class T>
				activate_event(T* o, void(T::*f)(objptr, iactivate_event_args_t))
					: event((core_msg_t)core_msg_enum::activate) {
					set<iactivate_event_args>(o, f);
				}
				template<class T>
				activate_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, iactivate_event_args_t))
					: event((core_msg_t)core_msg_enum::activate) {
					set<iactivate_event_args>(o, f);
				}
				template<class F>
				activate_event(F f)
					: event((core_msg_t)core_msg_enum::activate) {
					set<iactivate_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class display_size_change_event
				: public event
			{
			public:
				template<class T>
				display_size_change_event(T* o, void(T::*f)(objptr, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::size) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class T>
				display_size_change_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::size) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class F>
				display_size_change_event(F f)
					: event((core_msg_t)core_msg_enum::size) {
					set<idisplay_info_event_args>(f);
				}
			};

			class display_orientation_change_event
				: public event
			{
			public:
				template<class T>
				display_orientation_change_event(T* o, void(T::*f)(objptr, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::orientation) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class T>
				display_orientation_change_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::orientation) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class F>
				display_orientation_change_event(F f)
					: event((core_msg_t)core_msg_enum::orientation) {
					set<idisplay_info_event_args>(f);
				}
			};

			class display_invalidate_event
				: public event
			{
			public:
				template<class T>
				display_invalidate_event(T* o, void(T::*f)(objptr, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::display_change) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class T>
				display_invalidate_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, idisplay_info_event_args_t))
					: event((core_msg_t)core_msg_enum::display_change) {
					set<idisplay_info_event_args>(o, f);
				}
				template<class F>
				display_invalidate_event(F f)
					: event((core_msg_t)core_msg_enum::display_change) {
					set<idisplay_info_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////

			class key_pressed_event
				: public event
			{
			public:
				template<class T>
				key_pressed_event(T* o, void(T::*f)(objptr, ikey_event_args_t))
					: event((core_msg_t)core_msg_enum::key_down) {
					set<ikey_event_args>(o, f);
				}
				template<class T>
				key_pressed_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, ikey_event_args_t))
					: event((core_msg_t)core_msg_enum::key_down) {
					set<ikey_event_args>(o, f);
				}
				template<class F>
				key_pressed_event(F f)
					: event((core_msg_t)core_msg_enum::key_down) {
					set<ikey_event_args>(f);
				}
			};

			class key_released_event
				: public event
			{
			public:
				template<class T>
				key_released_event(T* o, void(T::*f)(objptr, ikey_event_args_t))
					: event((core_msg_t)core_msg_enum::key_up) {
					set<ikey_event_args>(o, f);
				}
				template<class T>
				key_released_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, ikey_event_args_t))
					: event((core_msg_t)core_msg_enum::key_up) {
					set<ikey_event_args>(o, f);
				}
				template<class F>
				key_released_event(F f)
					: event((core_msg_t)core_msg_enum::key_up) {
					set<ikey_event_args>(f);
				}
			};

			/////////////////////////////////////////////////////////////////////////////////////////////


			class pointer_moved_event
				: public event
			{
			public:
				template<class T>
				pointer_moved_event(T* o, void(T::*f)(objptr, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_moved) {
					set<ipointer_event_args>(o, f);
				}
				template<class T>
				pointer_moved_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_moved) {
					set<ipointer_event_args>(o, f);
				}
				template<class F>
				pointer_moved_event(F f)
					: event((core_msg_t)core_msg_enum::pointer_moved) {
					set<ipointer_event_args>(f);
				}
			};

			class pointer_pressed_event
				: public event
			{
			public:
				template<class T>
				pointer_pressed_event(T* o, void(T::*f)(objptr, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_pressed) {
					set<ipointer_event_args>(o, f);
				}
				template<class T>
				pointer_pressed_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_pressed) {
					set<ipointer_event_args>(o, f);
				}
				template<class F>
				pointer_pressed_event(F f)
					: event((core_msg_t)core_msg_enum::pointer_pressed) {
					set<ipointer_event_args>(f);
				}
			};

			class pointer_released_event
				: public event
			{
			public:
				template<class T>
				pointer_released_event(T* o, void(T::*f)(objptr, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_released) {
					set<ipointer_event_args>(o, f);
				}
				template<class T>
				pointer_released_event(typename smart_ptr_type<T>::smart_ptr_t o, void(T::*f)(objptr, ipointer_event_args_t))
					: event((core_msg_t)core_msg_enum::pointer_released) {
					set<ipointer_event_args>(o, f);
				}
				template<class F>
				pointer_released_event(F f)
					: event((core_msg_t)core_msg_enum::pointer_released) {
					set<iupdate_event_args>(f);
				}
			};
		}
	}
}

#endif//__ANG_PLATFORM_MSG_H__