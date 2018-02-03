#ifndef __ANGWIN_H__
#define __ANGWIN_H__

#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_STORE_PLATFORM

#pragma warning( disable : 4251 4506)

#include <angsys.hpp>
//#include <ang/xml.hpp>
#include <ang/core/async.hpp>
#include <ang/platform/platform.hpp>
#include <ang/collections/map.hpp>

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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif//WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <ang/platform/uwp/win_events.hpp>

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			ANG_OBJECT(framework);
			ANG_OBJECT(window);

			ANG_OBJECT(xaml_framework);
			ANG_OBJECT(xaml_window);

			class LINK window
				: public object
				, public icore_view
				, public imessage_reciever
			{
			private: //Non copiable
				window(const window&) = delete;
				window& operator = (const window&) = delete;

			private:
				Windows::UI::Core::CoreWindow^ coreWindow;

			public:
				window();

			protected:
				virtual~window();

			public: //Events
				events::event_listener created_event;
				events::event_listener destroyed_event;
				events::event_listener draw_event;
				events::event_listener update_event;

				events::event_listener orientation_event;
				events::event_listener activate_event;
				events::event_listener size_event;

				events::event_listener char_event;
				events::event_listener key_pressed_event;
				events::event_listener key_released_event;

				events::event_listener pointer_moved_event;
				events::event_listener pointer_pressed_event;
				events::event_listener pointer_released_event;

				events::event_listener mouse_moved_event;
				events::event_listener mouse_button_pressed_event;
				events::event_listener mouse_button_released_event;

			public: //properties
				graphics::size<float> wnd_size();
				graphics::size<float> clip_size()const;

			public: //methods
				void screen_to_client(graphics::point<float>&);
				void client_to_screen(graphics::point<float>&);
				void screen_to_client(graphics::rect<float>&);
				void client_to_screen(graphics::rect<float>&);
				void position(graphics::point<float>);
				void size(graphics::size<float>);

			public: //overrides
				ANG_DECLARE_INTERFACE();

				virtual pointer get_core_view_handle()const override;
				virtual icore_context_t get_core_context()const override;
				virtual graphics::size<float> get_core_view_size()const override;
				virtual graphics::size<float> get_core_view_scale_factor()const override;
				virtual imessage_reciever_t get_listener()const override;

				virtual void send_msg(events::message_t) override;
				virtual core::async::ioperation_t<dword> post_msg(events::message_t) override;
				virtual bool listen_to(events::event_t) override;

			protected:
				virtual bool create(pointer args);
				virtual bool close();
				virtual void attach(pointer);
				virtual void detach();

			public: //Custom overrides
				virtual void initial_update();

			protected: //Internal Event Handlers
				events::event_trigger<window>& owner(events::event_listener&);
				events::event_trigger<window>const& trigger(events::event_listener const&)const;

				dword on_created(events::message_t m);
				dword on_destroyed(events::message_t m);
				dword on_paint(events::message_t m);
				dword on_update(events::message_t m);

				dword on_orientation_change(events::message_t m);
				dword on_activate(events::message_t m);
				dword on_size_change(events::message_t m);

				dword on_pointer_moved(events::message_t m);
				dword on_pointer_pressed(events::message_t m);
				dword on_pointer_released(events::message_t m);

				dword on_mouse_moved(events::message_t m);
				dword on_mouse_button_pressed(events::message_t m);
				dword on_mouse_button_released(events::message_t m);

				dword on_key_pressed(events::message_t m);
				dword on_char(events::message_t m);
				dword on_key_released(events::message_t m);
			};

			class LINK xaml_window
				: public window
			{
			private:
				Windows::UI::Xaml::Controls::Page^ mainPage;
				Windows::UI::Xaml::Controls::Panel^ mainPanel;

			public:
				xaml_window();

			protected:
				virtual~xaml_window();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				virtual icore_context_t get_core_context()const override;
				virtual graphics::size<float> get_core_view_size()const override;
				virtual graphics::size<float> get_core_view_scale_factor()const override;

			protected:
				virtual bool create(pointer args)override;
				virtual bool close();

			public: //Custom overrides
				virtual void initial_update()override;

			};

			class LINK framework
				: public object
				, public imessage_reciever
				, public icore_app
				, public singleton<framework_t>
			{
			private:
				friend ref class FrameworkView;
				friend ref class XamlFrameworkView;
				FrameworkView^ framework_;
				XamlFrameworkView^ xaml_framework_;

				window_t window_;

			public:
				framework();

			public: //overrides
				ANG_DECLARE_INTERFACE();

				virtual void send_msg(events::message_t) override;
				virtual core::async::ioperation_t<dword> post_msg(events::message_t) override;
				virtual bool listen_to(events::event_t) override;

				virtual pointer get_core_app_handle()const override;
				virtual icore_view_t get_main_core_view() override;
				virtual input::ikeyboard_t get_keyboard() override;

			public: //methods
				virtual dword run();
				virtual dword run(array<string> args);

			public: //properties
				bool is_worker_thread()const;
				core::async::mutex_ptr_t main_mutex()const;
				core::async::cond_ptr_t main_cond()const;
				core::async::thread_t main_worker_thread()const;
				array<string> command_line_args()const;

				objptr property(cstr_t)const;
				void property(cstr_t, objptr);

			public: //custom implementation
				virtual bool init_app(array<string> cmdl);
				virtual void update_app();
				virtual bool exit_app();

			protected:
				virtual bool create_window(Windows::UI::Core::CoreWindow^);

			protected:
				virtual~framework();
			};

		}
	}
}

#endif//WINDOWS_PLATFORM
#endif//__ANGWIN_H__