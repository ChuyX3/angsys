#ifndef __ANG_WIN32_H__
#define __ANG_WIN32_H__

#include <angsys.h>
#include <ang/core/async.h>
#include <ang/dom/xml.h>
#include <ang/platform/platform.h>
#include <ang/collections/list.h>
#include <ang/collections/hash_map.h>

#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_STORE_PLATFORM

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
		namespace windows
		{
			ang_object(core_app);
			ang_object(core_window);
			ang_object(windows_file_system);

			using appptr = core_app_t;
			using wndptr = core_window_t;

		}
	}
}

//#include <ang/platform/win32/gdi.h>
//#include <ang/platform/win32/events.h>

namespace ang
{
	namespace platform
	{
		namespace windows
		{

			class LINK core_window
				: public smart<core_window, icore_view>
			{
			
			private: //Non copiable
				core_window(const core_window&) = delete;
				core_window& operator = (const core_window&) = delete;

			protected:
				struct _hwnd;
				typedef _hwnd* hwnd_t;
				hwnd_t m_handle;

			private: //property handlers
				static graphics::point<float> get_position_property(base_property<graphics::point<float>> const* prop);
				static graphics::size<float> get_clip_size_property(base_property<graphics::size<float>> const* prop);
				static graphics::size<float> get_scale_factor_property(base_property<graphics::size<float>> const* prop);
				static void set_scale_factor_property(base_property<graphics::size<float>>* prop, graphics::size<float>&& value);
				static hwnd_t get_handle_property(base_property<hwnd_t> const* prop);
				static bool get_is_created_property(base_property<bool> const* prop);
				static bool get_is_visible_property(base_property<bool> const* prop);

			public:
				core_window();

			protected:
				virtual~core_window();

			public: //Events
				//ang_platform_event(events::icreated_event_args, created_event);
				ang_platform_event(events::icreated_event_args, initialize_event);
				ang_platform_event(events::imsg_event_args, closed_event);
				ang_platform_event(events::imsg_event_args, destroyed_event);

				ang_platform_event(events::idraw_event_args, draw_event);
				ang_platform_event(events::imsg_event_args, update_event);

				ang_platform_event(events::idisplay_info_event_args, orientation_event);
				ang_platform_event(events::idisplay_info_event_args, activate_event);
				ang_platform_event(events::idisplay_info_event_args, size_event);
				ang_platform_event(events::idisplay_info_event_args, display_change_event);

				ang_platform_event(events::ikey_event_args, key_pressed_event);
				ang_platform_event(events::ikey_event_args, key_released_event);
				//ang_platform_event(events::itext_change_event_args, text_change_event);

				ang_platform_event(events::ipointer_event_args, pointer_entered_event);
				ang_platform_event(events::ipointer_event_args, pointer_pressed_event);
				ang_platform_event(events::ipointer_event_args, pointer_moved_event);
				ang_platform_event(events::ipointer_event_args, pointer_released_event);
				ang_platform_event(events::ipointer_event_args, pointer_leaved_event);

			public: //properties
				property<const graphics::point<float>, get_position_property> position;
				property<const graphics::size<float>, get_clip_size_property> clip_size;
				property<const graphics::size<float>, get_scale_factor_property, set_scale_factor_property> scale_factor;
				property<const hwnd_t, get_handle_property> handle;
				property<const bool, get_is_created_property> is_created;
				property<const bool, get_is_visible_property> is_visible;

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				virtual void clear()override;
				virtual pointer core_view_handle()const override;
				virtual icore_context_t core_context()const override;
				virtual graphics::size<float> core_view_size()const override;
				virtual graphics::size<float> core_view_scale_factor()const override;
				virtual imessage_listener_t dispatcher()const override;


				virtual bool initialize(pointer);
				virtual bool update();
				virtual bool terminate();

			protected:
				dword send_msg(events::message);
				core::async::iasync<void> post_task(core::async::iasync<void>);
				events::event_token_t listen_to(events::event_t);

			public: //Custom overrides
				virtual void dispatch_messages();

			protected: //Internal Event Handlers
				dword on_initialize(events::message& m);
				dword on_finalize(events::message& m);
				dword on_draw(events::message& m);
				dword on_update(events::message& m);
				dword on_activate(events::message& m);
				dword on_display_event(events::message& m);
				dword on_pointer_event(events::message& m);
				dword on_mouse_event(events::message& m);
				dword on_key_event(events::message& m);
				dword on_command(events::message& m);
				dword on_task_command(events::message& m);

				friend class dispatcher;
			};

			class LINK core_app
				: public smart<core_app, icore_app, singleton<core_app_t>>
			{
			public:
				friend singleton<core_app_t>;
	
			protected:
				bool m_is_running;
				core::async::mutex_t m_mutex;
				core::async::thread_t m_ui_thread;
				collections::list<events::message> m_msg_queue;
				collections::hash_map<string, var> m_settings;
				imessage_listener_t m_dispatcher;
				wndptr m_main_wnd;

				static core_window* get_main_wnd_property(base_property<wndptr>const* prop) {
					return field_to_parent(&core_app::main_wnd, prop)->m_main_wnd.get();
				}
				static void set_main_wnd_property(base_property<wndptr>* prop, const wndptr& val) {
					auto proxy = field_to_parent(&core_app::main_wnd, prop);
					if (!proxy->m_main_wnd.is_empty())
						return;
					proxy->m_main_wnd = val;
				}

			protected:
				core_app();
				virtual~core_app();

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				void clear()override;
				imessage_listener_t dispatcher()const;
				virtual pointer core_app_handle()const override;
				virtual icore_view_t main_core_view() override;
				virtual input::ikeyboard_t keyboard() override;

			public:
				virtual dword run();
				virtual bool initialize(pointer);
				virtual void running();
				virtual bool terminate();

			protected:
				void dispatch_msgs();
				dword send_msg(events::message);
				core::async::iasync<void> post_task(core::async::iasync<void>);
				events::event_token_t listen_to(events::event_t);		

			public: //Properties
				property<core_window, get_main_wnd_property, set_main_wnd_property> main_wnd;

				bool is_current_thread()const;
				core::async::thread_t main_worker_thread()const;

				var settings(cstr_t)const;
				void settings(cstr_t, var);

			public: //Events
				ang_platform_event(events::iapp_status_event_args, initialize_event);
				ang_platform_event(events::iapp_status_event_args, finalize_event);

			public: //Custom Implementation
				dword on_initialize(events::message&);
				dword on_activated(events::message&);
				dword on_finalize(events::message&);
				dword on_command(events::message&);
				
			};

		}

	}
}

#endif//WINDOWS_PLATFORM
#endif//__ANGWIN_H__