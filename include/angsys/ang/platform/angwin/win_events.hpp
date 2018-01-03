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
			ANG_BEGIN_ENUM(LINK, win_msg_enum, core_msg_t)
				none = 0x0000,
				created = 0x0001,
				destroyed = 0x0002,
				move = 0x0003,
				size = 0x0005,
				activate = 0x0006,
				got_focus = 0x0007,
				lost_focus = 0x0008,
				enable = 0x000A,
				set_draw = 0x000B,
				set_text = 0x000C,
				get_text = 0x000D,
				get_text_lenght = 0x000E,
				draw = 0x000F,
				close = 0x0010,
				quit = 0x0012,
				erase_bkgnd = 0x0014,
				sys_color_change = 0x0015,
				visibility_change = 0x0018,
				show_window = visibility_change,
				font_change = 0x001D,
				time_change = 0x001E,
				cancel_mode = 0x001F,
				set_cursor = 0x0020,
				mouse_activate = 0x0021,
				child_activate = 0x0022,
				paint_icon = 0x0026,
				icon_erase_bkgnd = 0x0027,
				next_dlg_ctrl = 0x0028,
				spooler_status = 0x002A,
				draw_item = 0x002B,
				measure_item = 0x002C,
				delete_item = 0x002D,
				vkey_to_item = 0x002E,
				char_to_item = 0x002F,
				set_font = 0x0030,
				get_font = 0x0031,
				set_hot_key = 0x0032,
				get_hot_key = 0x0033,
				notify = 0x004E,

				context_menu = 0x007B,
				style_changing = 0x007C,
				style_changed = 0x007D,
				display_change = 0x007E,
				get_icon = 0x007F,
				set_icon = 0x0080,

				nc_create = 0x0081,
				nc_destroy = 0x0082,
				nc_calc_size = 0x0083,
				nc_hit_test = 0x0084,
				nc_paint = 0x0085,
				nc_activate = 0x0086,
				get_dlg_code = 0x0087,
				sysnc_paint = 0x0088,
				nc_mouse_move = 0x00A0,
				nc_lbutton_down = 0x00A1,
				nc_lbutton_up = 0x00A2,
				nc_lbutton_dbl_clk = 0x00A3,
				nc_rbutton_down = 0x00A4,
				nc_rbutton_up = 0x00A5,
				nc_rbutton_dbl_clk = 0x00A6,
				nc_mbutton_down = 0x00A7,
				nc_mbutton_up = 0x00A8,
				nc_mbutton_dbl_clk = 0x00A9,

				nc_xbutton_down = 0x00AB,
				nc_xbutton_up = 0x00AC,
				nc_xbutton_dbl_clk = 0x00AD,
				input_device_change = 0x00FE,
				input = 0x00FF,

				key_first = 0x0100,
				key_down = 0x0100,
				key_up = 0x0101,
				put_char = 0x0102,
				dead_char = 0x0103,
				sys_key_down = 0x0104,
				sys_key_up = 0x0105,
				sys_char = 0x0106,
				sys_dead_char = 0x0107,
				uini_char = 0x0109,
				key_last = 0x0109,

				init_dialog = 0x0110,
				command = 0x0111,
				sys_command = 0x0112,
				timer = 0x0113,
				hscroll = 0x0114,
				vscroll = 0x0115,
				init_menu = 0x0116,
				init_menu_popup = 0x0117,

				get_true = 0x0119,
				get_true_notify = 0x011A,

				menu_select = 0x011F,
				menu_char = 0x0120,
				enter_idle = 0x0121,
				menu_rbutton_up = 0x0122,
				menu_drag = 0x0123,
				menu_get_object = 0x0124,
				un_init_menu_popup = 0x0125,
				menu_command = 0x0126,

				ctl_color_msg_box = 0x0132,
				ctl_color_edit = 0x0133,
				ctl_color_list_box = 0x0134,
				ctl_color_btn = 0x0135,
				ctl_color_dlg = 0x0136,
				ctl_color_scroll_bar = 0x0137,
				ctl_color_static = 0x0138,
				get_hmenu = 0x01E1,

				mouse_first = 0x0200,
				mouse_move = 0x0200,
				lbutton_down = 0x0201,
				lbutton_up = 0x0202,
				lbutton_dbl_clk = 0x0203,
				rbutton_down = 0x0204,
				rbutton_up = 0x0205,
				rbutton_dbl_clk = 0x0206,
				mbutton_down = 0x0207,
				mbutton_up = 0x0208,
				mbutton_dbl_clk = 0x0209,

				mouse_wheel = 0x020A,
				xbutton_down = 0x020B,
				xbutton_up = 0x020C,
				xbutton_dbl_clk = 0x020D,
				mouse_hwheel = 0x020E,
				mouse_last = 0x020E,

				parent_notify = 0x0210,
				enter_menu_loop = 0x0211,
				exit_menu_loop = 0x0212,

				next_menu = 0x0213,
				sizing = 0x0214,
				capture_changed = 0x0215,
				moving = 0x0216,

				touch_input = 0x0240,
				pointer_moved = 0x0245,
				pointer_pressed = 0x0246,
				pointer_released = 0x0247,
				pointer_canceled = 0x024C,

				system_reserved_event = 0x0401,
				interprocess_command = 0x0402,
				start_app = 0x0403,
				exit_app = 0x0404,
				orientation = 0x0405,
				initial_update = 0x0406,
				update = 0x0407,
				text_change = 0x0408,
				user_msg = 0x0450,
			ANG_END_ENUM(win_msg_enum);

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


			///////////////////////////////////////////////////////////////////////////

			class mouse_moved_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_moved_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::mouse_move) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_moved_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::mouse_move) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_lbutton_down_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_lbutton_down_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::lbuttonDown) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_lbutton_down_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::lbuttonDown) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_lbutton_up_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_lbutton_up_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::lbuttonUp) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_lbutton_up_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::lbuttonUp) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_lbutton_dblclick_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_lbutton_dblclick_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::lbuttonDblClk) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_lbutton_dblclick_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::lbuttonDblClk) {
					set<ipointer_event_args>(f);
				}
			};
		

			class mouse_rbutton_down_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_rbutton_down_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::rbuttonDown) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_rbutton_down_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::rbuttonDown) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_rbutton_up_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_rbutton_up_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::rbuttonUp) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_rbutton_up_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::rbuttonUp) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_rbutton_dblclick_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_rbutton_dblclick_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::rbuttonDblClk) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_rbutton_dblclick_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::rbuttonDblClk) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_xbutton_down_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_xbutton_down_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::xbuttonDown) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_xbutton_down_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::xbuttonDown) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_xbutton_up_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_xbutton_up_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::xbuttonUp) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_xbutton_up_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::xbuttonUp) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_xbutton_dblclick_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_xbutton_dblclick_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::xbuttonDblClk) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_xbutton_dblclick_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::xbuttonDblClk) {
					set<ipointer_event_args>(f);
				}
			};
		}
	}
}

#endif