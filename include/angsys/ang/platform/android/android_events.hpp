#ifndef __ANG_ANDROID_PLATFORM_H__
#error...
#elif !defined __ANG_ANDROID_EVENTS_H__
#define __ANG_ANDROID_EVENTS_H__
#ifdef ANDROID_PLATFORM

namespace ang
{
	namespace platform
	{
		namespace events
		{

			ANG_BEGIN_ENUM(LINK, android_msg_enum, core_msg_t)
				none = 0x0000,
				created = 0x0001,
				destroyed = 0x0002,
				size = 0x0005,
				activate = 0x0006,
				got_focus = 0x0007,
				lost_focus = 0x0008,
				draw = 0x000F,
				begin_drawing = 0x0407,
				end_drawing = 0x0408,
				visibility_change = 0x0018,

				display_change = 0x007E,
				key_down = 0x0100,
				key_up = 0x0101,
				touch_input = 0x0240,
				pointer_moved = 0X0245,
				pointer_pressed = 0X0246,
				pointer_released = 0X0247,
				pointer_enter = 0x0249,
				pointer_leave = 0x024A,
				pointer_canceled = 0x024C,

				start_app = 0x0403,
				exit_app = 0x0404,
				orientation = 0x0405,
				initial_update = 0x0406,
				update = 0x0407,
				text_change = 0x0408,

				config_change = 0x0409,
				low_memory = 0x040A,
				input_device_crerated = 0x0410,
				input_device_destroyed = 0x0411,

				resume = 0x0415,
				pause = 0x0416,
				stop = 0x0417,

				user_msg = 0x0450,
			ANG_END_ENUM(android_msg_enum);

			//imsg_event_args empty implementation, Used for default
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
				virtual foundation::size<float> canvas_size()const override;

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
				virtual double delta_time()const override;
				virtual double total_time()const override;

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
				virtual foundation::point<float> position()const override;
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
				virtual wstring text()const override;
				virtual input::ikeyboard_t keyboard()const override;

			private:
				virtual~text_change_event_args();
			};

		}
	}
}


#endif // ANDROID_PLATFORM
#endif //__ANG_ANDROID_PLATFORM_H__