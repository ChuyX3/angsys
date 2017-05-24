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
				None = 0x0000,
				Created = 0x0001,
				Destroyed = 0x0002,
				Move = 0x0003,
				Size = 0x0005,
				Activate = 0x0006,
				GotFocus = 0x0007,
				LostFocus = 0x0008,
				Enable = 0x000A,
				SetDraw = 0x000B,
				SetText = 0x000C,
				GetText = 0x000D,
				GetTextLenght = 0x000E,
				Draw = 0x000F,
				Close = 0x0010,
				Quit = 0x0012,
				EraseBkgnd = 0x0014,
				SysColorChange = 0x0015,
				VisibilityChange = 0x0018,
				ShowWindow = VisibilityChange,
				FontChange = 0x001D,
				TimeChange = 0x001E,
				CancelMode = 0x001F,
				SetCursor = 0x0020,
				MouseActivate = 0x0021,
				ChildActivate = 0x0022,
				PaintIcon = 0x0026,
				IconEraseBkgnd = 0x0027,
				NextDlgCtrl = 0x0028,
				SpoolerStatus = 0x002A,
				DrawItem = 0x002B,
				MeasureItem = 0x002C,
				DeleteItem = 0x002D,
				VKeyToItem = 0x002E,
				CharToItem = 0x002F,
				SetFont = 0x0030,
				GetFont = 0x0031,
				SetHotKey = 0x0032,
				GetHotKey = 0x0033,
				Notify = 0x004E,

				ContextMenu = 0x007B,
				StyleChanging = 0x007C,
				StyleChanged = 0x007D,
				DisplayChange = 0x007E,
				GetIcon = 0x007F,
				SetIcon = 0x0080,

				NCCreate = 0x0081,
				NCDestroy = 0x0082,
				NCCalcSize = 0x0083,
				NCHitTest = 0x0084,
				NCPaint = 0x0085,
				NCActivate = 0x0086,
				GetDlgCode = 0x0087,
				SysNCPaint = 0x0088,
				NCMouseMove = 0x00A0,
				NCLButtonDown = 0x00A1,
				NCLButtonUp = 0x00A2,
				NCLButtonDblClk = 0x00A3,
				NCRButtonDown = 0x00A4,
				NCRButtonUp = 0x00A5,
				NCRButtonDblClk = 0x00A6,
				NCMButtonDown = 0x00A7,
				NCMButtonUp = 0x00A8,
				NCMButtonDblClk = 0x00A9,

				NCXButtonDown = 0x00AB,
				NCXButtonUp = 0x00AC,
				NCXButtonDblClk = 0x00AD,
				InputDeviceChange = 0x00FE,
				Input = 0x00FF,

				KeyFirst = 0x0100,
				KeyDown = 0x0100,
				KeyUp = 0x0101,
				Char = 0x0102,
				DeadChar = 0x0103,
				SysKeyDown = 0x0104,
				SysKeyUp = 0x0105,
				SysChar = 0x0106,
				SysDeadChar = 0x0107,
				UiniChar = 0x0109,
				KeyLast = 0x0109,

				InitDialog = 0x0110,
				Command = 0x0111,
				SysCommand = 0x0112,
				Timer = 0x0113,
				HScroll = 0x0114,
				VScroll = 0x0115,
				InitMenu = 0x0116,
				InitMenuPopup = 0x0117,

				GetRtue = 0x0119,
				GetTrueNotify = 0x011A,

				MenuSelect = 0x011F,
				MenuChar = 0x0120,
				EnterIdle = 0x0121,
				MenuRButtonUp = 0x0122,
				MenuDrag = 0x0123,
				MenuGetObject = 0x0124,
				UnInitMenuPopup = 0x0125,
				MenuCommand = 0x0126,

				CtlColorMsgBox = 0x0132,
				CtlColorEdit = 0x0133,
				CtlColorListBox = 0x0134,
				CtlColorBtn = 0x0135,
				CtlColorDlg = 0x0136,
				CtlColorScrollBar = 0x0137,
				CtlColorStatic = 0x0138,
				GetHMenu = 0x01E1,

				MouseFirst = 0x0200,
				MouseMove = 0x0200,
				LButtonDown = 0x0201,
				LButtonUp = 0x0202,
				LButtonDblClk = 0x0203,
				RButtonDown = 0x0204,
				RButtonUp = 0x0205,
				RButtonDblClk = 0x0206,
				MButtonDown = 0x0207,
				MButtonUp = 0x0208,
				MButtonDblClk = 0x0209,

				MouseWheel = 0x020A,
				XButtonDown = 0x020B,
				XButtonUp = 0x020C,
				XButtonDblClk = 0x020D,
				MouseHWheel = 0x020E,
				MouseLast = 0x020E,

				ParentNotify = 0x0210,
				EnterMenuLoop = 0x0211,
				ExitMenuLoop = 0x0212,

				NextMenu = 0x0213,
				Sizing = 0x0214,
				CaptureChanged = 0x0215,
				Moving = 0x0216,

				TouchInput = 0x0240,
				PointerMoved = 0X0245,
				PointerPressed = 0X0246,
				PointerReleased = 0X0247,

				SystemReservedEvent = 0x0401,
				InterprocessCommand = 0x0402,
				StartApp = 0x0403,
				ExitApp = 0x0404,
				Orientation = 0x0405,
				InitialUpdate = 0x0406,
				Update = 0x0407,
				TextChange = 0x0408,
				UserMsg = 0x0450,
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


			///////////////////////////////////////////////////////////////////////////

			class mouse_moved_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_moved_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::MouseMove) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_moved_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::MouseMove) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_lbutton_down_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_lbutton_down_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::LButtonDown) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_lbutton_down_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::LButtonDown) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_lbutton_up_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_lbutton_up_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::LButtonUp) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_lbutton_up_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::LButtonUp) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_lbutton_dblclick_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_lbutton_dblclick_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::LButtonDblClk) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_lbutton_dblclick_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::LButtonDblClk) {
					set<ipointer_event_args>(f);
				}
			};
		

			class mouse_rbutton_down_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_rbutton_down_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::RButtonDown) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_rbutton_down_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::RButtonDown) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_rbutton_up_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_rbutton_up_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::RButtonUp) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_rbutton_up_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::RButtonUp) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_rbutton_dblclick_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_rbutton_dblclick_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::RButtonDblClk) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_rbutton_dblclick_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::RButtonDblClk) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_xbutton_down_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_xbutton_down_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::XButtonDown) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_xbutton_down_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::XButtonDown) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_xbutton_up_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_xbutton_up_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::XButtonUp) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_xbutton_up_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::XButtonUp) {
					set<ipointer_event_args>(f);
				}
			};

			class mouse_xbutton_dblclick_event
				: public event
			{
			public:
				template<class obj_t>
				mouse_xbutton_dblclick_event(obj_t* o, void(obj_t::*f)(object_t, ipointer_event_args_t))
					: event((core_msg_t)win_msg_enum::XButtonDblClk) {
					set<ipointer_event_args>(o, f);
				}
				template<class calleable_t>
				mouse_xbutton_dblclick_event(calleable_t f)
					: event((core_msg_t)win_msg_enum::XButtonDblClk) {
					set<ipointer_event_args>(f);
				}
			};
		}
	}
}

#endif