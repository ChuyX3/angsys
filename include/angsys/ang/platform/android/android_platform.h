#ifndef __ANG_ANDROID_PLATFORM_H__
#define __ANG_ANDROID_PLATFORM_H__

#include <angsys.h>
#include <ang/platform/platform.h>

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
			class msg_Task;
			class dispatcher;
			class assets_manager;

			typedef object_wrapper<activity> activity_t;
			typedef object_wrapper<msg_Task> msg_Task_t;
			typedef object_wrapper<dispatcher> dispatcher_t;
			typedef object_wrapper<assets_manager> assets_manager_t;
		}
	}
}

#include <ang/platform/android/android_events.h>

namespace ang
{
	namespace platform
	{
		namespace android
		{

			class LINK dispatcher final
				: public object
				, public icore_msg_dispatcher
			{
			private:
				activity_t _activity;
				core::async::dispatcher_thread_t _dispatcher;
				ALooper* _looper;
				AInputQueue* _input_queue;
				events::message_t _non_queue_msg;
				core::delegates::function<dword(pointer)> commandHandler = nullptr;
				core::delegates::function<dword(pointer)> inputHandler = nullptr;

			protected:
				dispatcher(activity*);
				virtual~dispatcher();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

			public: //ICoreMsgDispatcher overrides
				virtual void send_msg(events::message_t)override;
				virtual core::async::iasync_t<dword> post_msg(events::message_t)override;
				virtual bool listen_to(events::event_t);
				virtual bool dispatch_msg();

			public: // Properties
				bool is_worker_thread()const;
				bool is_dispatcher_thread()const;

			public: //Events
				dword on_dispatch_command_event(pointer);
				dword on_dispatch_input_event(pointer);

			protected:
				bool OnPreDispatchMessage(events::message_t msg);
				bool OnPostDispatchMessage(events::message_t msg);

			protected:
				friend activity;
			};





			class activity
				: public object
				, public icore_app
				, public icore_view
				, public singleton<activity_t>
			{
			public:
				friend dispatcher;
				friend singleton<activity_t>;

			private:
				mutable core::async::cond_t _cond;
				mutable core::async::mutex_t _mutex;
		
				core::async::thread_t _main_thread;
				core::async::thread_t _input_thread;
				core::async::thread_t _render_thread;

				core::async::iasync<void> * _input_async_action;
				core::async::iasync<void> * _render_async_action;
				core::async::async_action_status_t _status;

				dispatcher_t _dispatcher;
				ANativeActivity * _native_activity;
				ANativeWindow* _native_window;
				AConfiguration* _config;

				foundation::rect<float> _client_area;
				foundation::size<float> _scale_factor;

			public:
				activity();

			protected:
				virtual~activity();

			public: //Methods
				virtual core::async::iasync<dword>* run_async(ANativeActivity* activity);
				virtual core::async::iasync<dword>* update_activity(ANativeActivity* activity);
				virtual core::async::iasync<dword>* stop_async();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

			public: //Overrides
				virtual pointer get_core_app_handle()const override;
				virtual icore_view_t get_main_core_view() override;
				virtual input::ikeyboard_t get_keyboard()override;
				virtual imessage_reciever_t get_listener()const override;
				virtual pointer get_core_view_handle()const override;
				virtual icore_context_t get_core_context()const override;
				virtual graph::size<float> get_core_view_size()const override;
				virtual graph::size<float> get_core_view_scale_factor()const override;

			public: //Properties
				core::async::cond_t main_cond()const;
				core::async::mutex_t main_mutex()const;
				

				bool IsMainThread()const;
				bool IsInputThread()const;
				bool IsRenderThread()const;
				bool IsAllowedThread()const;
				core::async::thread_t main_thread()const;
				core::async::thread_t input_thread()const;
				core::async::thread_t render_thread()const;

				bool begin_rendering();
				bool stop_rendering();

				core::async::async_action_status_t activity_status()const;
				ANativeActivity* native_activity()const;

			private:
				void destroy();

			public: //Custom Implementation
				virtual bool init_app();
				virtual void update_app();
				virtual bool exit_app();

			public: //Events
				events::event_listener start_app_event;
				events::event_listener exitAppEvent;

				events::event_listener created_event;
				events::event_listener destroyed_event;

				events::event_listener update_event;
				events::event_listener draw_event;
				events::event_listener pointer_pressed_event;
				events::event_listener pointer_released_event;
				events::event_listener pointer_moved_event;

				events::event_listener begin_rendering_event;
				events::event_listener end_rendering_event;

			protected: //Events
				dword on_input_dispatcher_event_handler(ang::core::async::iasync<dword>* task, var_args_t args);
				dword on_render_event_handler(ang::core::async::iasync<dword>* task, var_args_t args);

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