#ifndef __ANG_ANDROID_PLATFORM_H__
#define __ANG_ANDROID_PLATFORM_H__

#include <angsys.hpp>
#include <ang/platform/platform.hpp>

#ifdef ANDROID_PLATFORM

#ifdef LINK
#undef LINK
#endif//LINK
#define LINK

namespace ang
{
	namespace platform
	{
		namespace android
		{
			class activity;
			class core_view;
			class assets_manager;

			typedef object_wrapper<activity> activity_t;
			typedef object_wrapper<core_view> core_view_t;
			typedef object_wrapper<assets_manager> assets_manager_t;
		}
	}
}

#include <ang/platform/android/android_events.hpp>

namespace ang
{
	namespace platform
	{
		namespace android
		{

			class core_view
				: public object
				, public icore_view
			{
			private:
				mutable weak_ptr<activity> _activity;
				ANativeWindow* _native_window;
				graphics::size<float> _client_area;
				graphics::size<float> _scale_factor;

			public:
				core_view(activity*);

			public: //overrides
				ANG_DECLARE_INTERFACE();

				virtual pointer get_core_view_handle()const override;
				virtual icore_context_t get_core_context()const override;
				virtual graphics::size<float> get_core_view_size()const override;
				virtual graphics::size<float> get_core_view_scale_factor()const override;
				virtual imessage_reciever_t get_listener()const override;

			private:
				void native_window(ANativeWindow*);
				void client_area(graphics::size<float>);
				void scale_factor(graphics::size<float>);

			private:
				friend activity;
				virtual~core_view();
			};

			class activity
				: public object
				, public icore_app
				, public imessage_reciever
				//, public singleton<activity_t>
			{
			public:
				friend singleton<activity_t>;

			private:
				mutable core::async::cond_t _cond;
				mutable core::async::mutex_t _mutex;
		
				core::async::thread_t _main_thread;
				core::async::idispatcher_t _worker_thread;

				core::delegates::function<dword(pointer)> command_handler;
				core::delegates::function<dword(pointer)> input_handler;

				core_view_t _core_view;

				ANativeActivity * _native_activity;		
				AConfiguration* _config;
				ALooper* _looper;
				AInputQueue* _input_queue;
				int _msg_pipe[2];

			public:
				activity();

			protected:
				virtual~activity();

			public: //Methods
				virtual core::async::itask_t run_async(ANativeActivity* activity);
				virtual core::async::itask_t update_activity(ANativeActivity* activity);
				virtual core::async::itask_t stop_async();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

			public: //Overrides
				virtual pointer get_core_app_handle()const override;
				virtual icore_view_t get_main_core_view() override;
				virtual input::ikeyboard_t get_keyboard()override;
				virtual imessage_reciever_t get_listener()const override;

				virtual void send_msg(events::message_t)override;
				virtual core::async::iasync_t<dword> post_msg(events::message_t)override;
				virtual bool listen_to(events::event_t)override;
				virtual bool dispatch_msg()override;

			public: //Properties
				core::async::cond_t main_cond()const;
				core::async::mutex_t main_mutex()const;
				ANativeActivity* native_activity()const;

				bool is_main_thread()const;
				bool is_worker_thread()const;
				bool is_allowed_thread()const;
				core::async::thread_t main_thread()const;
				core::async::thread_t worker_thread()const;

			private:
				void destroy();

			public: //Custom Implementation
				virtual bool init_app();
				virtual void update_app();
				virtual bool exit_app();

			public: //Events
				events::event_listener start_app_event;
				events::event_listener exit_app_event;
				events::event_listener created_event;
				events::event_listener destroyed_event;
				events::event_listener update_event;
				events::event_listener draw_event;
				events::event_listener pointer_pressed_event;
				events::event_listener pointer_released_event;
				events::event_listener pointer_moved_event;
				events::event_listener pointer_canceled_event;

			protected: //Events
				events::event_trigger<activity>& owner(events::event_listener& listener);
				events::event_trigger<activity>const& trigger(events::event_listener const& listener)const;

				dword on_dispatch_command_event(pointer);
				dword on_dispatch_input_event(pointer);

				virtual bool on_pre_dispatch_message(events::message_t msg);
				virtual bool on_post_dispatch_message(events::message_t msg);
				virtual void on_command_event_handler(events::message_t);
				virtual void on_init_window(events::message_t);
				virtual void on_term_window(events::message_t);
				virtual void on_window_resized(events::message_t);
				virtual void on_window_redraw_needed(events::message_t);
				virtual void on_content_rect_changed(events::message_t);
				virtual void on_get_focus(events::message_t);
				virtual void on_lost_focus(events::message_t);
				virtual void on_config_changed(events::message_t);
				virtual void on_low_memory(events::message_t);
				virtual void on_start(events::message_t);
				virtual void on_resume(events::message_t);
				virtual void on_save_state(events::message_t);
				virtual void on_pause(events::message_t);
				virtual void on_stop(events::message_t);
				virtual void on_destroy(events::message_t);
				virtual void on_input_event(events::message_t);
				virtual void on_update(events::message_t);
				virtual void on_draw(events::message_t);
			};

		}
	}
}

#endif // ANDROID_PLATFORM
#endif //__ANG_ANDROID_PLATFORM_H__