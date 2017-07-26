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






			//IMsgEventArgs empty implementation, Used for default
			class LINK msg_event_args
				: public object
				, public imsg_event_args
			{
			private:
				message_t msg;

			public:
				msg_event_args(message_t);
				virtual~msg_event_args();

			public: //overrides
				ANG_DECLARE_INTERFACE();
				virtual const message_t& msg_info()const override;
				virtual void handled(bool_t value) override;
				virtual bool_t handled()const override;
			};
		}
	}
}


#endif // ANDROID_PLATFORM
#endif //__ANG_ANDROID_PLATFORM_H__