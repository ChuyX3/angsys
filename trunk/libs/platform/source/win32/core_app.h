#pragma once

#include <angsys.h>
#include <ang/core/async.h>
#include <ang/platform/platform.h>

#include <Xinput.h>

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			ang_declare_object(core_app);
			ang_declare_object(async_task);
		}
		namespace input
		{
			ang_declare_object(controller);
			ang_declare_object(controller_manager);
		}
	}

	namespace graphics
	{
		ang_declare_object(device_context);
		ang_declare_object(paint_dc);

		class device_context : public implement<
			device_context,
			iid("ang::graphics::device_context"),
			graphics::icore_context>
		{
		protected:
			pointer handle;

		public:
			device_context(platform::windows::core_app_t wnd);

			bool is_valid()const;
			virtual pointer core_context_handle()const override;
			virtual bool bind_graphic_native_surface(pointer) override;

		protected:
			virtual bool create(platform::windows::core_app_t app);
			virtual bool close();

		public: //properties
			virtual pointer get_HDC()const;

			graphics::point<float> move_to(graphics::point<float>);
			graphics::point<float> current_position()const;
			graphics::point<float> draw_line_to(graphics::point<float>);
			void draw_primitive(primitive_t, const collections::ienum<graphics::point<float>>* ptns, bool solid = true);

		protected:
			virtual~device_context();
		};

		class paint_dc
			: public implement<paint_dc, iid("ang::graphics::paint_dc"), device_context>
		{
		public:
			paint_dc(platform::windows::core_app_t);

		protected:
			virtual bool create(platform::windows::core_app_t)override;
			virtual bool close();

		public: //Properties
			graphics::rect<float> clip_area()const;

		protected:
			virtual~paint_dc();
		};
	}

	namespace platform
	{

		namespace events
		{
			class core_listener : public implement<
				core_listener, 
				iid("ang::platform::events::core_listener")>,
				public listener<void(object*, platform::events::imsg_event_args*)>
			{
			public:
				core_listener(windows::core_app*);

			private:
				virtual~core_listener();
			};

			class msg_event_args
				: public implement<msg_event_args
				, iid("ang::platform::events::msg_event_args")
				, imsg_event_args>
			{
			private:
				message m_msg;

			public:
				msg_event_args(message);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

			private:
				virtual~msg_event_args();
			};

			class app_status_event_args
				: public implement<app_status_event_args
				, iid("ang::platform::events::app_status_event_args")
				, iapp_status_event_args>
			{
			private:
				message m_msg;
				windows::core_app_t m_app;

			public:
				app_status_event_args(message, windows::core_app_t app);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

				virtual icore_app_t core_app()const override;

			private:
				virtual~app_status_event_args();
			};

			class created_event_args
				: public implement<created_event_args
				, iid("ang::platform::events::created_event_args")
				, icreated_event_args>
			{
			private:
				message m_msg;
			
				windows::core_app_t m_app;
				smart<var[]> m_args;

			public:
				created_event_args(message, windows::core_app_t, smart<var[]>);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

				virtual icore_view_t core_view()const override;
				virtual icore_app_t core_app()const override;
				virtual smart<var[]> args_list()const override;

			private:
				virtual~created_event_args();
			};

			class display_info_event_args
				: public implement<display_info_event_args
				, iid("ang::platform::events::display_info_event_args")
				, idisplay_info_event_args>
			{
			private:
				message m_msg;
				windows::core_app_t m_app;
				display_invalidate_reason_t m_reason;
				display::display_info_t m_info;

			public:
				display_info_event_args(message, windows::core_app_t, display_invalidate_reason_t, display::display_info_t);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

				virtual icore_view_t core_view()const override;
				virtual display_invalidate_reason_t const& invalidate_reason()const override;
				virtual display::display_info_t const& display_info()const override;

			private:
				virtual~display_info_event_args();
			};

			class visibility_change_event_args
				: public implement<visibility_change_event_args
				, iid("ang::platform::events::visibility_change_event_args")
				, ivisibility_change_event_args>
			{
			private:
				message m_msg;
				windows::core_app_t m_app;
				bool m_visible;
			public:
				visibility_change_event_args(message, windows::core_app_t, bool);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual icore_view_t core_view()const override;
				virtual bool is_visible()const override;

			private:
				virtual~visibility_change_event_args();
			};

			class activate_event_args
				: public implement<activate_event_args
				, iid("ang::platform::events::activate_event_args")
				, iactivate_event_args>
			{
			private:
				message m_msg;
				activate_status_t m_status;

			public:
				activate_event_args(message, activate_status_t);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual activate_status_t const& status()const override;

			private:
				virtual~activate_event_args();
			};

			class draw_event_args
				: public implement<draw_event_args
				, iid("ang::platform::events::draw_event_args")
				, idraw_event_args>
			{
			private:
				message m_msg;
				windows::core_app_t m_app;
				graphics::device_context_t m_dc;
				graphics::size<float> m_size;

			public:
				draw_event_args(message, windows::core_app_t, graphics::device_context_t, graphics::size<float>);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual icore_view_t core_view()const override;
				virtual graphics::icore_context_t core_context()const override;
				virtual graphics::size<float> const& canvas_size()const override;

			private:
				virtual~draw_event_args();
			};

			///////////////////////////////////////////////////
			class pointer_event_args
				: public implement<pointer_event_args
				, iid("ang::platform::events::pointer_event_args")
				, ipointer_event_args>
			{
			private:
				message m_msg;
				input::poiner_info_t m_info;

			public:
				pointer_event_args(message, input::poiner_info_t);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual graphics::point<float> const& position()const override;
				virtual input::key_modifiers_t const& modifiers()const override;
				virtual input::poiner_info_t const& info()const override;

			private:
				virtual~pointer_event_args();
			};

			class key_event_args
				: public implement<key_event_args
				, iid("ang::platform::events::key_event_args")
				, ikey_event_args>
			{
			private:
				message m_msg;
				input::key_info_t m_info;

			public:
				key_event_args(message, input::key_info_t);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual char32_t key()const override;
				virtual input::key_info_t const& info()const override;

			private:
				virtual~key_event_args();
			};

			class text_change_event_args
				: public implement<text_change_event_args
				, iid("ang::platform::events::text_change_event_args")
				, itext_change_event_args>
			{
			private:
				message m_msg;
				string m_text;
				input::ikeyboard_t m_keyboard;

			public:
				text_change_event_args(message, string, input::ikeyboard_t = null);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual string text()const override;
				virtual input::ikeyboard_t keyboard()const override;

			private:
				virtual~text_change_event_args();
			};

			/////////////////////////////////////////////////////////////////////////////////

			ang_declare_object(controller_digital_input_args);
			ang_declare_object(controller_analog_input_args);

			class controller_status_args
				: public implement<controller_status_args
				, iid("ang::platform::events::controller_status_args")
				, icontroller_status_args>
			{
			private:
				message m_msg;

			public:
				controller_status_args(message);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual uint controller_id()const override;
				virtual input::icontroller_t controller()const override;
				virtual input::controller_status_t status()const override;

			private:
				virtual~controller_status_args();
			};

			class controller_digital_input_args
				: public implement<controller_digital_input_args
				, iid("ang::platform::events::controller_digital_input_args")
				, icontroller_digital_input_args>
			{
			private:
				message m_msg;

			public:
				controller_digital_input_args(message);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual input::icontroller_t controller()const override;
				virtual input::controller_button_t button()const override;
				virtual input::controller_button_state_t state()const override;

				void msg(const message&);
			private:
				virtual~controller_digital_input_args();
			};

			class controller_analog_input_args
				: public implement<controller_analog_input_args
				, iid("ang::platform::events::controller_analog_input_args")
				, icontroller_analog_input_args>
			{
			private:
				message m_msg;

			public:
				controller_analog_input_args(message);

			public: /*overrides*/
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual input::icontroller_t controller()const override;
				virtual input::controller_button_t button()const override;
				virtual input::analog_input_state_t state()const override;

				void msg(const message&);
			private:
				virtual~controller_analog_input_args();
			};

		}

		namespace windows
		{
			struct system_info
			{
				static ushort screen_counter();//SM_CMONITORS
				static ushort mouse_buttons();//SM_CMOUSEBUTTONS
				static graphics::size<float> main_screen_bound();
				static graphics::size<float> current_screen_bound();
				static display::orientation_t current_screen_orientation();
				static display::orientation_t native_screen_orientation();
			};

			class core_app : public implement<
				core_app,
				iid("ang::platform::windows::core_app"),
				icore_app,
				icore_view,
				imessage_listener,
				core::async::idispatcher>
			{
			private:
				static LRESULT STDCALL wndproc(HWND hwnd, UINT msg, WPARAM wprm, LPARAM lprm);

			private:
				HINSTANCE m_hint;
				HWND m_hwnd;
				//weak_ptr<framework> m_frm;
				core::async::thread_t m_thread;
				core::time::step_timer m_timer;
				input::controller_manager_t m_controllers;

				listener<void(core::time::step_timer const&)> m_idle_event;
				collections::hash_map<events::core_msg_t,smart<events::core_listener>> m_event_listeners;

			public:
				core_app();

			public:
				virtual pointer core_app_handle()const override;
				virtual icore_view_t core_view() override;
				virtual input::ikeyboard_t keyboard() override;
				virtual ivar property(astring name)const override;
				virtual void property(astring name, ivar) override;

				virtual pointer core_view_handle()const override;
				virtual graphics::icore_context_t core_context()const override;
				virtual graphics::size<float> core_view_size()const override;
				virtual graphics::size<float> core_view_scale_factor()const override;
				virtual imessage_listener_t dispatcher()const override;

				virtual dword send_msg(events::message) override;
				virtual core::async::iasync<dword> post_msg(events::message) override;
				virtual events::event_token_t notify(events::event_t) override;
				virtual bool notify(events::core_msg_t, events::event_token_t) override;

				virtual bool has_thread_access()const override;
				virtual core::async::async_action_status_t status()const override;
				virtual void exit() override;
				virtual void join() override;
				virtual listen_token<void(core::time::step_timer const&)> add_idle_event(function<void(core::time::step_timer const&)>) override;
				virtual bool remove_idle_event(listen_token<void(core::time::step_timer const&)>) override;
				virtual core::async::iasync<void> run_async(function<void(core::async::iasync<void>)>) override;

				core::async::iasync<void> post_task(async_task_t);
				error run(function<error(icore_app_t)> setup, app_args_t& args);
				void wndproc(events::message& msg);
				void def_wndproc(events::message& msg);
			protected:
				dword on_created(events::message& m);
				dword on_destroyed(events::message& m);
				dword on_draw(events::message& m);
				dword on_update(events::message& m);
				dword on_activate(events::message& m);

				dword on_display_event(events::message& m);

				dword on_pointer_event(events::message& m);
				dword on_mouse_event(events::message& m);
				dword on_key_event(events::message& m);
				dword on_text_change(events::message& m);
				dword on_task_command(events::message& m);
			

			private:
				virtual~core_app();
			};


			class async_task final
				: public implement<async_task
				, iid("ang::platform::windows::async_task")
				, core::async::itask<void>>
			{
				friend core_app;
			public:
				bool m_was_canceled;
				mutable core::async::cond_t m_cond;
				mutable core::async::mutex_t m_mutex;
				mutable core::async::async_action_status_t m_status;
				weak_ptr<core_app> m_parent;
				function<void(core::async::iasync<void>)> m_action;
				async_task_t m_parent_task;
				async_task_t m_child_task;

			public:
				async_task(core_app*, function<void(core::async::iasync<void>)>);

			public: /*overrides*/
				void execute(void);
				virtual core::async::iasync<void> then(function<void(core::async::iasync<void>)> func);
				virtual bool wait(core::async::async_action_status_t state)const;
				virtual bool wait(core::async::async_action_status_t state, dword ms)const;
				virtual core::async::async_action_status_t status()const;
				virtual bool cancel();
				virtual void result()const;

				core::async::cond& cond()const { return m_cond; }
				core::async::mutex& mutex()const { return m_mutex; }

			private:
				virtual~async_task();
			};

		}
	}

	namespace platform
	{
		namespace input
		{
		
			class controller 
				: public implement<controller
				, iid("ang::platform::input::controller")
				, icontroller>
			{
			private:
				uint m_id;
				bool m_is_enable;
				XINPUT_STATE m_state;

				analog_input_value_t m_triggers;
				analog_input_value_t m_left_thumb_stick;
				analog_input_value_t m_right_thumb_stick;
				controller_buttons_state_t m_buttons_state;

				events::controller_analog_input_args_t m_analog_input_args;
				events::controller_digital_input_args_t m_digital_input_args;
				
				ang::platform::events::event_listener m_digital_button_change_event;
				ang::platform::events::event_listener m_analog_input_change_event;

			public:
				controller(uint id);

			public:
				uint controller_id()const override;
				controller_buttons_state_t state()const override;
				thumb_stick_value_t left_thumb_stick()const override;
				thumb_stick_value_t right_thumb_stick()const override;
				analog_input_value_t triggers()const override;

			public:
				bool is_enable()const;
				void init(XINPUT_STATE const&);
				void update(float, XINPUT_STATE const&);
				void exit();

			protected:
				void dispose()override;

				ang::platform::events::event_token_t add_digital_button_change_event(ang::platform::events::event_t)override;
				bool remove_digital_button_change_event(ang::platform::events::event_token_t) override;
				ang::platform::events::event_token_t add_analog_input_change_event(ang::platform::events::event_t)override;
				bool remove_analog_input_change_event(ang::platform::events::event_token_t) override;
			
			private:
				virtual~controller();

			};

			class controller_manager
				: public implement<controller_manager
				, iid("ang::platform::input::icontroller_manager")
				, icontroller_manager>
			{
			private:
				stack_array<controller_t, 4> m_controllers;
				events::event_listener m_controller_connected_event;
				events::event_listener m_controller_disconnected_event;
			public:
				controller_manager();

			public:
				icontroller_t controller(uint)const override;

			public:
				void update(float);

			protected:
				void dispose()override;

				ang::platform::events::event_token_t add_controller_connected_event(ang::platform::events::event_t)override;
				bool remove_controller_connected_event(ang::platform::events::event_token_t) override;
				ang::platform::events::event_token_t add_controller_disconnected_event(ang::platform::events::event_t)override;
				bool remove_controller_disconnected_event(ang::platform::events::event_token_t) override;

			private:
				virtual~controller_manager();
			};
		}
	}
}