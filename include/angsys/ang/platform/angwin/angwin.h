#ifndef __ANGWIN_H__
#define __ANGWIN_H__

#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM

#pragma warning( disable : 4251 4506)

#include <angsys.h>
#include <ang/xml.h>
#include <ang/core/async.h>
#include <ang/platform/platform.h>

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

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			class process;
			class child_process;
			class app;
			class windows_file_system;
			class window;

			typedef pointer hwnd_t;
			typedef pointer hprocces_t;

			typedef object_wrapper<process> process_t;
			typedef object_wrapper<child_process> child_process_t;
			typedef object_wrapper<app> app_t;
			typedef object_wrapper<window> window_t;

			ANG_BEGIN_ENUM(LINK, showing_flag, dword)
				hide = 0,
				normal = 1,
				minimized = 2,
				maximized = 3,
				maximize = 3,
				mo_active = 4,
				show = 5,
				minimize = 6,
				show_no_active = 7,
				show_na = 8,
				restore = 9,
				def = 10,
				force_minimize = 11,
			ANG_END_ENUM(showing_flag);

			ANG_BEGIN_FLAGS(LINK, class_style, dword)
				none = 0X0000,
				vredraw = 0x0001,
				hredraw = 0x0002,
				dbl_clks = 0x0008,
				own_dc = 0x0020,
				class_dc = 0x0040,
				parent_dc = 0x0080,
				no_close = 0x0200,
				save_bits = 0x0800,
				byte_align_client = 0x1000,
				byte_align_window = 0x2000,
				global_class = 0x4000,
				drop_shaow = 0x00020000,
			ANG_END_FLAGS(class_style);

			ANG_BEGIN_FLAGS(LINK, wnd_style, dword)
				none = 0X00000000L,
				overlapped = 0x00000000L,
				popup = 0x80000000L,
				child = 0x40000000L,
				minimize = 0x20000000L,
				shown = 0x10000000L,
				disabled = 0x08000000L,
				clip_siblings = 0x04000000L,
				clip_children = 0x02000000L,
				maximize = 0x01000000L,
				caption = 0x00C00000L,
				border = 0x00800000L,
				dialog_frame = 0x00400000L,
				vscroll = 0x00200000L,
				hscroll = 0x00100000L,
				system_menu = 0x00080000L,
				resizable_frame = 0x00040000L,
				group = 0x00020000L,
				tab_stop = 0x00010000L,
				maximize_box = 0x00010000L,
				minimize_box = 0x00020000L,
				overlapped_window = 0x00CF0000L,
			ANG_END_FLAGS(wnd_style);

			ANG_BEGIN_FLAGS(LINK, wnd_style_ex, dword)
				none = 0X00000000L,
				dlg_modal_frame = 0x00000001L,
				no_parent_notify = 0x00000004L,
				top_most = 0x00000008L,
				accept_files = 0x00000010L,
				transparent = 0x00000020L,
				mdi_child = 0x00000040L,
				tool_window = 0x00000080L,
				window_edge = 0x00000100L,
				client_edge = 0x00000200L,
				context_help = 0x00000400L,
				right = 0x00001000L,
				left = 0x00000000L,
				rtl_reading = 0x00002000L,
				ltr_reading = 0x00000000L,
				left_scrollbar = 0x00004000L,
				right_scrollbar = 0x00000000L,
				control_parent = 0x00010000L,
				static_edge = 0x00020000L,
				app_window = 0x00040000L,
				overlapped_window = 0x00000300L,
				palette_window = 0x00000188L
			ANG_END_FLAGS(wnd_style_ex);
			
			class LINK system_info
			{
			public:
				static ushort screen_counter();//SM_CMONITORS
				static ushort mouse_buttons();//SM_CMOUSEBUTTONS
				static foundation::size<float> main_screen_bound();
				static foundation::size<float> current_screen_bound();
				static display::orientation_t current_screen_orientation();
				static display::orientation_t native_screen_orientation();
			};

		}
	}
}

ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::windows::class_style_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::windows::wnd_style_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::windows::wnd_style_ex_t);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::windows::class_style_t*);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::windows::wnd_style_t*);
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::windows::wnd_style_ex_t*);

template<> LINK ang::platform::windows::class_style_t ang::xml::xml_value::as<ang::platform::windows::class_style_t>()const;
template<> LINK ang::platform::windows::wnd_style_t ang::xml::xml_value::as<ang::platform::windows::wnd_style_t>()const;
template<> LINK ang::platform::windows::wnd_style_ex_t ang::xml::xml_value::as<ang::platform::windows::wnd_style_ex_t>()const;

#include <ang/platform/angwin/gdi.h>
#include <ang/platform/angwin/win_events.h>

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			struct LINK class_regist_args
				: public object
			{
				class_regist_args(const class_regist_args&);
				class_regist_args(wstring class_name, class_style_t class_style
					, uint menuName = 0
					, pointer icon = null
					, pointer brush = null
					, pointer cursor = null);

				ANG_DECLARE_INTERFACE();

				wstring const& name()const;
				class_style_t style()const;
				uint menu_name()const;
				pointer icon()const;
				pointer brush()const;
				pointer cursor()const;

			protected:
				wstring _class_name;
				class_style_t _class_style;
				uint _menu_name;
				pointer _hicon;
				pointer _hbrush;
				pointer _hcursor;

				virtual~class_regist_args();
			};

			typedef object_wrapper<class_regist_args> class_regist_args_t;

			struct LINK wnd_create_args
				: public object
			{
				wnd_create_args(const wnd_create_args&);
				wnd_create_args(wstring className, wstring wndName
					, foundation::rect<float> area = foundation::rect<float>(100, 100, 900, 700)
					, wnd_style_t wndStyle = wnd_style::overlapped_window
					, hwnd_t wndParent = null
					, var_args_t userArgs = null);

				ANG_DECLARE_INTERFACE();

				wstring const& wnd_name()const;
				wstring const& wnd_class_name()const;

				foundation::rect<float> rect_area()const;
				wnd_style_t style()const;
				pointer parent()const;
				var_args_t user_args()const;

			protected:
				wstring		_wnd_class_name;
				wstring		_wnd_name;
				foundation::rect<float>	_rect_area;
				wnd_style_t		_wnd_style;
				hwnd_t			_parent;
				var_args_t		_user_args;

				virtual~wnd_create_args();
			};

			typedef object_wrapper<wnd_create_args> wnd_create_args_t;

			struct LINK wnd_create_args_ex
				: public wnd_create_args
			{
				wnd_create_args_ex(const wnd_create_args_ex&);
				wnd_create_args_ex(string className, string wndName
					, foundation::rect<float> area = foundation::rect<float>(100, 100, 900, 700)
					, wnd_style_ex_t wndStyleEx = wnd_style_ex::none
					, wnd_style_t wndStyle = wnd_style::overlapped_window
					, hwnd_t wndParent = null
					, var_args_t userArgs = null);

				ANG_DECLARE_INTERFACE();
				wnd_style_ex_t style_ex()const;

			protected:
				wnd_style_ex_t _wnd_style_ex;

				virtual~wnd_create_args_ex();
			};
			typedef object_wrapper<wnd_create_args_ex> wnd_create_args_ex_t;


			class LINK window
				: public object
				, public icore_view
				, public imessage_reciever
			{
			public:
				static wstring regist_wnd_class(class_regist_args_t);
				static void unregist_wnd_class(wstring);
				static window_t handle_to_window(hwnd_t wnd);
				static void adjust_window_rect(foundation::rect<float>& rect, wnd_style_t style, bool hasMenu = false);

			private: //Non copiable
				window(const window&) = delete;
				window& operator = (const window&) = delete;

			protected:
				pointer _handle;

			public:
				window();
				window(wnd_create_args_t);

			protected:
				virtual~window();

			public: //Events
				events::event_listener createdEvent;
				events::event_listener destroyedEvent;
				events::event_listener drawEvent;
				events::event_listener updateEvent;

				events::event_listener orientationEvent;
				events::event_listener activateEvent;
				events::event_listener sizeEvent;

				events::event_listener charEvent;
				events::event_listener keyPressedEvent;
				events::event_listener keyReleasedEvent;

				events::event_listener pointerMovedEvent;
				events::event_listener pointerPressedEvent;
				events::event_listener pointerReleasedEvent;

				events::event_listener mouseMovedEvent;
				events::event_listener mouseButtonPressedEvent;
				events::event_listener mouseButtonReleasedEvent;

			public: //properties
				window_t parent();
				void parent(window_t);
				void parent_from_handle(hwnd_t);
				foundation::point<float> position();
				foundation::size<float> wnd_size();
				foundation::size<float> clip_size()const;

			public: //methods
				void screen_to_client(foundation::point<float>&);
				void client_to_screen(foundation::point<float>&);
				void screen_to_client(foundation::rect<float>&);
				void client_to_screen(foundation::rect<float>&);
				void position(foundation::point<float>);
				void size(foundation::size<float>);

				void show(showing_flag_t flag);
				void update_wnd()const;

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				virtual hwnd_t get_core_view_handle()const override;
				virtual icore_context_t get_core_context()const override;
				virtual foundation::size<float> get_core_view_size()const override;
				virtual foundation::size<float> get_core_view_scale_factor()const override;
				virtual imessage_reciever_t get_listener()const override;

				virtual void send_msg(events::message_t) override;
				virtual core::async::iasync_t<dword> post_msg(events::message_t) override;
				virtual bool listen_to(events::event_t) override;

				bool is_created()const;
				virtual bool create(wnd_create_args_t);

			protected:
				virtual bool close();

				virtual void attach(pointer);
				virtual void detach();
				virtual void def_window_proc(events::message_t msg);

			public: //Custom overrides
				virtual void pre_create(wnd_create_args_t args);
				virtual void window_proc(events::message_t msg);
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


			class LINK process
				: public object
				, public icore_msg_dispatcher
			{
			public:
				static process_t current_process();

			protected:
				mutable core::async::mutex_t mutex;
				mutable core::async::cond_t cond;
				array<string> cmd_args;
				hprocces_t _process;

			public:
				process();

			protected:
				virtual~process();

			public: //Methods
				virtual dword run();
				virtual dword run(array<string> args);

			protected:
				bool update();
				void destroy();
				dword on_message_dispatcher(events::message_t);

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				virtual void send_msg(events::message_t msg) override;
				virtual core::async::iasync_t<dword> post_msg(events::message_t msg) override;
				virtual bool listen_to(events::event_t) override;
				virtual bool dispatch_msg()override;

				hprocces_t handle()const;
				bool is_created()const;

			protected:
				virtual bool create(pointer);
				virtual bool close();
				virtual void attach(hprocces_t);
				virtual void detach();

			public: //Properties
				bool is_worker_thread()const;
				core::async::mutex_t main_mutex()const;
				core::async::cond_t main_cond()const;
				core::async::thread_t main_worker_thread()const;
				array<string> command_line_args()const;

			public: //Events
				events::event_listener startAppEvent;
				events::event_listener exitAppEvent;

			public: //Custom Implementation
				virtual bool init_app(array<string> cmdl);
				virtual void update_app();
				virtual bool exit_app();
			};


			class LINK app
				: public process
				, public icore_app
			{
			public:
				static app_t current_app();

			protected:
				window_t _main_wnd;

			public:
				app();

			protected:
				virtual~app();

			public: //Methods
				core::async::iasync_t<dword> run_async(window_t wnd, wnd_create_args_t args);

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				void set_main_wnd(window_t mainWindow);

				virtual pointer get_core_app_handle()const override;
				virtual icore_view_t get_main_core_view() override;
				virtual input::ikeyboard_t get_keyboard() override;
				virtual imessage_reciever_t get_listener()const override;
				//virtual Core::Files::IFileSystem* GetFileSystem();

			protected:
				virtual dword on_run_async(core::async::iasync_t<dword>, window_t);

			public: //Properties
				window_t main_wnd()const;

			public: //Events
				events::event_listener mainWndCreatedEvent;
				events::event_listener mainWndDestroyedEvent;

			};


			class child_process
				: public object
				, public icore_msg_dispatcher
			{
			public:
				child_process();
				virtual~child_process();

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				virtual void send_msg(events::message_t msg) override;
				virtual core::async::iasync_t<dword> post_msg(events::message_t msg) override;

				hprocces_t handle()const;
				bool is_created()const;

			protected:
				//virtual Bool IsPointerEmpty()const;
				virtual bool listen_to(events::event_t) override;
				virtual bool dispatch_msg()override;

				virtual void attach(pointer);
				virtual void detach();

			public: //Properties
				core::async::thread_t main_worker_thread()const;
				array<string> const& command_line_args()const;

			public: //Events
				events::event_listener startAppEvent;
				events::event_listener exitAppEvent;

			public: //Custom Implementation
				virtual bool init_app();
				virtual void update_app();
				virtual bool exit_app();
			};
		}
	}
}

ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::events::win_msg_enum_t)
ANG_REGISTER_RUNTIME_TYPE_INFORMATION(ang::platform::windows::showing_flag_t)

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//PLATFORM
#endif//__ANGWIN_H__