#ifndef __ANG_WIN32_H__
#define __ANG_WIN32_H__

#include <angsys.h>
#include <ang/core/async.h>
#include <ang/dom/xml.h>
#include <ang/platform/platform.h>
#include <ang/collections/list.h>
#include <ang/collections/hash_map.h>

#if defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM

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

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			ang_object(app);
			ang_object(window);
			ang_object(process);
			ang_object(child_process);
			ang_object(windows_file_system);

			using appptr = app_t;
			using wndptr = window_t;

			safe_enum(LINK, showing_flag, dword)
			{
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
			};

			safe_flags(LINK, class_style, dword)
			{
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
			};

			safe_flags(LINK, wnd_style, dword)
			{
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
			};

			safe_flags(LINK, wnd_style_ex, dword)
			{
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
			}
;

			struct LINK system_info
			{
				static ushort screen_counter();//SM_CMONITORS
				static ushort mouse_buttons();//SM_CMOUSEBUTTONS
				static graphics::size<float> main_screen_bound();
				static graphics::size<float> current_screen_bound();
				static display::orientation_t current_screen_orientation();
				static display::orientation_t native_screen_orientation();
			};
		}
	}
}

#include <ang/platform/win32/gdi.h>
#include <ang/platform/win32/events.h>

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
					, uint menu = 0
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
				wstring m_class_name;
				class_style_t m_class_style;
				uint m_menu_name;
				pointer m_hicon;
				pointer m_hbrush;
				pointer m_hcursor;

				virtual~class_regist_args();
			};

			typedef object_wrapper<class_regist_args> class_regist_args_t;

			struct LINK wnd_create_args
				: public object
			{
				wnd_create_args(const wnd_create_args&);
				wnd_create_args(wstring className, wstring wndName
					, graphics::rect<float> area = graphics::rect<float>(100, 100, 900, 700)
					, wnd_style_t wndStyle = wnd_style::overlapped_window + wnd_style::shown
					, pointer wndParent = null
					, var_args_t userArgs = null);

				ANG_DECLARE_INTERFACE();

				wstring const& wnd_name()const;
				wstring const& wnd_class_name()const;

				graphics::rect<float> rect_area()const;
				wnd_style_t style()const;
				pointer parent()const;
				var_args_t user_args()const;

			protected:
				wstring		m_wnd_class_name;
				wstring		m_wnd_name;
				graphics::rect<float>	m_rect_area;
				wnd_style_t		m_wnd_style;
				pointer			m_parent;
				var_args_t		m_user_args;

				virtual~wnd_create_args();
			};

			typedef object_wrapper<wnd_create_args> wnd_create_args_t;

			struct LINK wnd_create_args_ex
				: public wnd_create_args
			{
				wnd_create_args_ex(const wnd_create_args_ex&);
				wnd_create_args_ex(string className, string wndName
					, graphics::rect<float> area = graphics::rect<float>(100, 100, 900, 700)
					, wnd_style_ex_t wndStyleEx = wnd_style_ex::none
					, wnd_style_t wndStyle = wnd_style::overlapped_window
					, pointer wndParent = null
					, var_args_t userArgs = null);

				ANG_DECLARE_INTERFACE();
				wnd_style_ex_t style_ex()const;

			protected:
				wnd_style_ex_t m_wnd_style_ex;

				virtual~wnd_create_args_ex();
			};
			typedef object_wrapper<wnd_create_args_ex> wnd_create_args_ex_t;

	
			class LINK window
				: public object
				, public icore_view
			//	, public imessage_listener
			//	, public core::async::idispatcher
			{
			public:
				static wstring regist_wnd_class(class_regist_args_t);
				static void unregist_wnd_class(wstring);
				static wndptr handle_to_window(pointer wnd);
				static void adjust_window_rect(graphics::rect<float>& rect, wnd_style_t style, bool hasMenu = false);

			private: //Non copiable
				window(const window&) = delete;
				window& operator = (const window&) = delete;

			protected:
				struct _hwnd;
				typedef _hwnd* hwnd_t;
				hwnd_t m_handle;
				//dispatcher_t m_dispatcher;
				//collections::list<function<void(core::async::iasync<void>)>> m_stack;

			public:
				window();
				window(wnd_create_args_t);

			protected:
				virtual~window();

			public: //Events
				events::event_listener created_event;
				events::event_listener closed_event;
				events::event_listener destroyed_event;
				events::event_listener draw_event;
				events::event_listener update_event;

				events::event_listener orientation_event;
				events::event_listener activate_event;
				events::event_listener size_event;
				events::event_listener display_change_event;

				events::event_listener char_event;
				events::event_listener key_pressed_event;
				events::event_listener key_released_event;

				events::event_listener pointer_entered_event;
				events::event_listener pointer_pressed_event;
				events::event_listener pointer_moved_event;
				events::event_listener pointer_released_event;
				events::event_listener pointer_leaved_event;

				events::event_listener mouse_moved_event;
				events::event_listener mouse_lbutton_pressed_event;
				events::event_listener mouse_rbutton_pressed_event;
				events::event_listener mouse_lbutton_released_event;
				events::event_listener mouse_rbutton_released_event;

			public: //properties
				wndptr parent();
				void parent(wndptr);
				void parent_from_handle(pointer);
				graphics::point<float> position();
				graphics::size<float> wnd_size();
				graphics::size<float> clip_size()const;

			public: //methods
				void screen_to_client(graphics::point<float>&);
				void client_to_screen(graphics::point<float>&);
				void screen_to_client(graphics::rect<float>&);
				void client_to_screen(graphics::rect<float>&);
				void position(graphics::point<float>);
				void size(graphics::size<float>);
				void redraw();
				void show(showing_flag_t flag);
				void update_wnd()const;

			public: //Overrides
				ANG_DECLARE_INTERFACE();

				virtual pointer core_view_handle()const override;
				virtual icore_context_t core_context()const override;
				virtual graphics::size<float> core_view_size()const override;
				virtual graphics::size<float> core_view_scale_factor()const override;
				virtual imessage_listener_t listener()const override;
				virtual core::async::idispatcher_t dispatcher()const override;
				
				//virtual dword send_msg(events::message) override;
				//virtual core::async::iasync<dword> post_msg(events::message) override;
				//virtual bool listen_to(events::event_t) override;

				bool is_created()const;
				virtual bool create(wnd_create_args_t);

			protected:
				virtual bool close();

				virtual void attach(pointer);
				virtual void detach();
				virtual void def_window_proc(events::message& msg);

				dword send_msg(events::message);
				core::async::iasync<void> post_task(core::async::iasync<void>);
				bool listen_to(events::event_t);

			public: //Custom overrides
				virtual void window_proc(events::message& msg);
				virtual void initial_update();

			protected: //Internal Event Handlers
				events::event_trigger<window>& owner(events::event_listener&);
				int trigger(events::event_listener const&, events::imsg_event_args_t)const;

				dword on_created(events::message& m);
				dword on_close(events::message& m);
				dword on_destroyed(events::message& m);
				dword on_draw(events::message& m);
				dword on_update(events::message& m);
				dword on_activate(events::message& m);

				dword on_dysplay_event(events::message& m);
				
				dword on_pointer_event(events::message& m);
				dword on_mouse_event(events::message& m);
				dword on_key_event(events::message& m);

				dword on_task_command(events::message& m);

				friend class dispatcher;
			};

			class LINK process
				: public object
				//, public imessage_listener
			{
			public:
				static process_t current_process();

			protected:
				struct _hprocess;
				typedef _hprocess *hprocess_t;
				hprocess_t m_process;
				
				//mutable core::async::mutex_ptr_t mutex;
				//mutable core::async::cond_ptr_t cond;
				//array<string> cmd_args;

			public:
				process();

			protected:
				virtual~process();

			public: //Methods
				virtual dword run();
				virtual dword run(array<string> args);

			protected:
				int main_loop();
				void destroy();
				dword on_message_dispatcher(events::message);
				events::event_trigger<process>const& trigger(events::event_listener const& listener)const;
			public: //Overrides
				ANG_DECLARE_INTERFACE();

				pointer handle()const;
				core::async::idispatcher_t dispatcher()const;

			protected:
				dword send_msg(events::message);
				core::async::iasync<void> post_task(core::async::iasync<void>);
				bool listen_to(events::event_t);
				virtual dword dispatch_msg(events::message);

				virtual bool close();

			public: //Properties
				bool is_worker_thread()const;
				core::async::mutex_ptr_t main_mutex()const;
				core::async::cond_ptr_t main_cond()const;
				core::async::thread_t main_worker_thread()const;
				array<string> command_line_args()const;

				var property(cstr_t)const;
				void property(cstr_t, var);

			public: //Events
				events::event_listener start_app_event;
				events::event_listener exit_app_event;

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
				static appptr current_app();

			protected:
				wndptr m_main_wnd;

			public:
				app();

			protected:
				virtual~app();

			public: //Methods
				core::async::iasync<dword> run_async(wndptr wnd, wnd_create_args_t args);

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				void set_main_wnd(wndptr mainWindow);

				virtual pointer core_app_handle()const override;
				virtual icore_view_t main_core_view() override;
				virtual input::ikeyboard_t keyboard() override;
				//virtual imessage_reciever_t get_listener()const override;
				//virtual Core::Files::IFileSystem* GetFileSystem();

			protected:
				virtual dword on_run_async(core::async::iasync<dword>, wndptr);

			public: //Properties
				wndptr main_wnd()const;
				void main_wnd(wndptr);

			public: //Custom Implementation
				virtual bool init_app(array<string> cmdl) override;
				virtual void update_app() override;
				virtual bool exit_app() override;

			public: //Events
				void on_main_wnd_closed(objptr, platform::events::imsg_event_args_t);
				//events::event_listener main_wnd_created_event;
				//events::event_listener main_wnd_destroyed_event;

			};
		}
	}
}

#endif//WINDOWS_PLATFORM
#endif//__ANGWIN_H__