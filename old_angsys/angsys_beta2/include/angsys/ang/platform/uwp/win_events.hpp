#ifndef __ANGWIN_H__
#error ...
#elif !defined __WINMSG_H__
#define __WINMSG_H__

namespace ang
{
	namespace platform
	{
		namespace events
		{

			//IMsgEventArgs empty implementation, Used for default
			class LINK msg_event_args
				: public object
				, public imsg_event_args
			{
			private:
				message_t msg;

			public:
				msg_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

			private:
				virtual~msg_event_args();
			};

			//typedef object_wrapper<msg_event_args> msg_event_args_t;

			class LINK app_status_event_args
				: public object
				, public iapp_status_event_args
			{
			private:
				message_t msg;

			public:
				app_status_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();

				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

				virtual icore_app_t core_app()const override;

			private:
				virtual~app_status_event_args();
			};


			class LINK created_event_args
				: public object
				, public icreated_event_args
			{
			private:
				message_t msg;

			public:
				created_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

				virtual icore_view_t core_view()const override;
				virtual icore_app_t core_app()const override;
				virtual var_args_t args_list()const override;

			private:
				virtual~created_event_args();
			};


			class LINK display_info_event_args
				: public object
				, public idisplay_info_event_args
			{
			private:
				message_t msg;

			public:
				display_info_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;

				virtual icore_view_t core_view()const override;
				virtual display_invalidate_reason_t invalidate_reason()const override;
				virtual display::display_info_t display_info()const override;

			private:
				virtual~display_info_event_args();
			};

			class LINK visibility_change_event_args
				: public object
				, public ivisibility_change_event_args
			{
			private:
				message_t msg;

			public:
				visibility_change_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual icore_view_t core_view()const override;
				virtual bool is_visible()const override;

			private:
				virtual~visibility_change_event_args();
			};


			class LINK activate_event_args
				: public object
				, public iactivate_event_args
			{
			private:
				message_t msg;

			public:
				activate_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual activate_status_t status()const override;

			private:
				virtual~activate_event_args();
			};


			class LINK draw_event_args
				: public object
				, public idraw_event_args
			{
			private:
				message_t msg;

			public:
				draw_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual icore_view_t core_view()const override;
				virtual icore_context_t core_context()const override;
				virtual graphics::size<float> canvas_size()const override;

			private:
				virtual~draw_event_args();
			};


			class LINK update_event_args
				: public object
				, public iupdate_event_args
			{
			private:
				message_t msg;

			public:
				update_event_args(message_t);	

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual shared_ptr<core::time::step_timer> step_timer()const override;

			private:
				virtual~update_event_args();
			};


			///////////////////////////////////////////////////
			class LINK pointer_event_args
				: public object
				, public ipointer_event_args
			{
			private:
				message_t msg;

			public:
				pointer_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual graphics::point<float> position()const override;
				virtual input::key_modifiers_t modifiers()const override;
				virtual input::poiner_info_t info()const override;

			private:
				virtual~pointer_event_args();
			};

			class LINK key_event_args
				: public object
				, public ikey_event_args
			{
			private:
				message_t msg;

			public:
				key_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual uint key()const override;
				virtual input::key_modifiers_t modifiers()const override;
				virtual word flags()const override;
				virtual bool is_pressed()const override;

			private:
				virtual~key_event_args();
			};

			class LINK text_change_event_args
				: public object
				, public itext_change_event_args
			{
			private:
				message_t msg;

			public:
				text_change_event_args(message_t);

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool value) override;
				virtual bool handled()const override;
				virtual mstring text()const override;
				virtual input::ikeyboard_t keyboard()const override;

			private:
				virtual~text_change_event_args();
			};

		}
	}
}

#endif