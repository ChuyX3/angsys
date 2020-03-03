#ifndef __EVENT_ARGS_H__
#define __EVENT_ARGS_H__

#include <ang/platform/uwp/windows.h>


namespace ang
{
	namespace platform
	{
		namespace events
		{

			//IMsgEventArgs empty implementation, Used for default
			class msg_event_args
				: public smart<msg_event_args, imsg_event_args>
			{
			private:
				message m_msg;

			public:
				msg_event_args(message);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

			private:
				virtual~msg_event_args();
			};

			class app_status_event_args
				: public smart<app_status_event_args, iapp_status_event_args>
			{
			private:
				message m_msg;
				windows::appptr m_app;

			public:
				app_status_event_args(message, windows::appptr app);

			public: //overrides
				ANG_DECLARE_INTERFACE();

				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

				virtual icore_app_t core_app()const override;

			private:
				virtual~app_status_event_args();
			};

			class created_event_args
				: public smart<created_event_args, icreated_event_args>
			{
			private:
				message m_msg;
				windows::wndptr m_view;
				windows::appptr m_app;
				var_args_t m_args;

			public:
				created_event_args(message, windows::wndptr, windows::appptr, var_args_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

				virtual icore_view_t core_view()const override;
				virtual icore_app_t core_app()const override;
				virtual var_args_t args_list()const override;

			private:
				virtual~created_event_args();
			};

			class display_info_event_args
				: public smart<display_info_event_args, idisplay_info_event_args>
			{
			private:
				message m_msg;
				windows::wndptr m_view;
				display_invalidate_reason_t m_reason;
				display::display_info_t m_info;

			public:
				display_info_event_args(message, windows::wndptr, display_invalidate_reason_t, display::display_info_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
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
				: public smart<visibility_change_event_args, ivisibility_change_event_args>
			{
			private:
				message m_msg;
				windows::wndptr m_view;
				bool m_visible;
			public:
				visibility_change_event_args(message, windows::wndptr, bool);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual icore_view_t core_view()const override;
				virtual bool is_visible()const override;

			private:
				virtual~visibility_change_event_args();
			};

			class activate_event_args
				: public smart<activate_event_args, iactivate_event_args>
			{
			private:
				message m_msg;
				activate_status_t m_status;

			public:
				activate_event_args(message, activate_status_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual activate_status_t const& status()const override;

			private:
				virtual~activate_event_args();
			};

			class draw_event_args
				: public smart<draw_event_args, idraw_event_args>
			{
			private:
				message m_msg;
				windows::wndptr m_view;
				graphics::icore_context_t m_dc;
				graphics::size<float> m_size;

			public:
				draw_event_args(message, windows::wndptr, graphics::icore_context_t, graphics::size<float>);

			public: //overrides
				ANG_DECLARE_INTERFACE();
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
				: public smart<pointer_event_args, ipointer_event_args>
			{
			private:
				message m_msg;
				input::poiner_info_t m_info;

			public:
				pointer_event_args(message, input::poiner_info_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
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
				: public smart<key_event_args, ikey_event_args>
			{
			private:
				message m_msg;
				input::key_info_t m_info;

			public:
				key_event_args(message, input::key_info_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual char32_t key()const override;
				virtual input::key_info_t const& info()const override;


			private:
				virtual~key_event_args();
			};

			class text_change_event_args
				: public smart<text_change_event_args, itext_change_event_args>
			{
			private:
				message m_msg;
				wstring m_text;
				input::ikeyboard_t m_keyboard;

			public:
				text_change_event_args(message, wstring, input::ikeyboard_t = null);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual text::istring_t text()const override;
				virtual input::ikeyboard_t keyboard()const override;

			private:
				virtual~text_change_event_args();
			};

			/////////////////////////////////////////////////////////////////////////////////

			class controller_status_args
				: public smart<controller_status_args, icontroller_status_args>
			{
			private:
				message m_msg;
				input::icontroller_t m_controller;
				input::controller_status_t m_status;

			public:
				controller_status_args(message, input::icontroller_t, input::controller_status_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
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
				: public smart<controller_digital_input_args, icontroller_digital_input_args>
			{
			private:
				message m_msg;

			public:
				controller_digital_input_args();

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual input::icontroller_t controller()const override;
				virtual input::controller_button_t button()const override;
				virtual input::controller_button_state_t state()const override;

			private:
				virtual~controller_digital_input_args();
			};
			
			class controller_analog_input_args
				: public smart<controller_analog_input_args, icontroller_analog_input_args>
			{
			private:
				message m_msg;

			public:
				controller_analog_input_args();

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message& msg()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual input::icontroller_t controller()const override;
				virtual input::controller_button_t button()const override;
				virtual input::analog_input_state_t state()const override;

			private:
				virtual~controller_analog_input_args();
			};

		}
	}
}

#endif//__EVENT_ARGS_H__
