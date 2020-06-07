/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_PLATFORM_H__
#error ...
#elif !defined __ANG_PLATFORM_FRAMEWORK_H__
#define __ANG_PLATFORM_FRAMEWORK_H__

namespace ang
{
	namespace platform
	{
		ang_declare_object(app);

		typedef struct app_args {
			app_t app;
			string name;
			string title;
			string version;
			dword fps = 60;
		}app_args_t;

		class LINK app : public implement<app, iid("ang::platform::app")>
		{
		private: /*property handlers*/
			static icore_app* get_core_app_property(base_property const*);
			static icore_view* get_core_view_property(base_property const*);
			static imessage_listener* get_dispatcher_property(base_property const*);

		public:
			error run(function<void(app_args_t&)> setup);

		public:
			app();

		public: /*properties*/
			property<icore_app_t, get_core_app_property> core_app;
			property<icore_view_t, get_core_view_property> core_view;
			property<imessage_listener_t, get_dispatcher_property> core_dispatcher;

		public: /*events*/
			ang_platform_event(events::icreated_event_args, created_event);
			ang_platform_event(events::imsg_event_args, destroyed_event);

			ang_platform_event(events::idraw_event_args, draw_event);
			ang_platform_event(events::imsg_event_args, update_event);

			ang_platform_event(events::idisplay_info_event_args, orientation_event);
			ang_platform_event(events::iactivate_event_args, activate_event);
			ang_platform_event(events::idisplay_info_event_args, size_event);
			ang_platform_event(events::idisplay_info_event_args, display_change_event);

			ang_platform_event(events::ikey_event_args, key_pressed_event);
			ang_platform_event(events::ikey_event_args, key_released_event);
			ang_platform_event(events::itext_change_event_args, text_changed_event);

			ang_platform_event(events::ipointer_event_args, pointer_entered_event);
			ang_platform_event(events::ipointer_event_args, pointer_pressed_event);
			ang_platform_event(events::ipointer_event_args, pointer_moved_event);
			ang_platform_event(events::ipointer_event_args, pointer_released_event);
			ang_platform_event(events::ipointer_event_args, pointer_leaved_event);

			ang_platform_event(events::ipointer_event_args, mouse_moved_event);
			ang_platform_event(events::ipointer_event_args, mouse_lbutton_pressed_event);
			ang_platform_event(events::ipointer_event_args, mouse_rbutton_pressed_event);
			ang_platform_event(events::ipointer_event_args, mouse_lbutton_released_event);
			ang_platform_event(events::ipointer_event_args, mouse_rbutton_released_event);

		protected:
			virtual bool init_app();
			virtual void update_app();
			virtual bool exit_app();

		private: //event handrels
			void on_created_event(objptr, events::icreated_event_args_t);
			void on_destroyed_event(objptr, events::imsg_event_args_t);

			void on_draw_event(objptr, events::idraw_event_args_t);
			void on_update_event(objptr, events::imsg_event_args_t);

			void on_orientation_event(objptr, events::idisplay_info_event_args_t);
			void on_activate_event(objptr, events::iactivate_event_args_t);
			void on_size_event(objptr, events::idisplay_info_event_args_t);
			void on_display_change_event(objptr, events::idisplay_info_event_args_t);

			void on_key_pressed_event(objptr, events::ikey_event_args_t);
			void on_key_released_event(objptr, events::ikey_event_args_t);
			void on_text_changed_event(objptr, events::itext_change_event_args_t);

			void on_pointer_entered_event(objptr, events::ipointer_event_args_t);
			void on_pointer_pressed_event(objptr, events::ipointer_event_args_t);
			void on_pointer_moved_event(objptr, events::ipointer_event_args_t);
			void on_pointer_released_event(objptr, events::ipointer_event_args_t);
			void on_pointer_leaved_event(objptr, events::ipointer_event_args_t);

			void on_mouse_moved_event(objptr, events::ipointer_event_args_t);
			void on_mouse_lbutton_pressed_event(objptr, events::ipointer_event_args_t);
			void on_mouse_rbutton_pressed_event(objptr, events::ipointer_event_args_t);
			void on_mouse_lbutton_released_event(objptr, events::ipointer_event_args_t);
			void on_mouse_rbutton_released_event(objptr, events::ipointer_event_args_t);

		protected:
			virtual~app();
		};
	}
}

#endif//__ANG_PLATFORM_H__