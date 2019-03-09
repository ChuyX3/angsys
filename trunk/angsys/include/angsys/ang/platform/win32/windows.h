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
				: public smart<class_regist_args>
			{
			private:
				wstring m_class_name;
				class_style_t m_class_style;
				uint m_menu_name;
				pointer m_hicon;
				pointer m_hbrush;
				pointer m_hcursor;

				static wstring get_name_propery(base_property<wstring>const*);
				static class_style_t get_style_propery(base_property<class_style_t>const*);
				static uint get_menu_name_propery(base_property<uint>const*);
				static pointer get_icon_propery(base_property<pointer>const*);
				static pointer get_brush_propery(base_property<pointer>const*);
				static pointer get_cursor_propery(base_property<pointer>const*);

			public:
				class_regist_args(const class_regist_args&);
				class_regist_args(wstring class_name, class_style_t class_style
					, uint menu = 0
					, pointer icon = null
					, pointer brush = null
					, pointer cursor = null);

				ANG_DECLARE_INTERFACE();

				property<const wstring, get_name_propery> name;
				property<const class_style_t, get_style_propery> style;
				property<const uint, get_menu_name_propery> menu_name;
				property<const pointer, get_icon_propery> icon;
				property<const pointer, get_brush_propery> brush;
				property<const pointer, get_cursor_propery> cursor;

			protected:
				virtual~class_regist_args();
			};

			typedef object_wrapper<class_regist_args> class_regist_args_t;

			struct LINK wnd_create_args
				: public smart<wnd_create_args>
			{
			private:
				wstring m_wnd_class_name;
				wstring m_wnd_name;
				graphics::rect<float> m_rect_area;
				wnd_style_t m_wnd_style;
				pointer m_parent;
				var_args_t m_user_args;

				static wstring get_wnd_name_property(base_property<wstring>const*);
				static wstring get_wnd_class_name_property(base_property<wstring>const*);
				static graphics::rect<float> get_rect_area_property(base_property<graphics::rect<float>>const*);
				static wnd_style_t get_style_property(base_property<wnd_style_t>const*);
				static pointer get_parent_property(base_property<pointer>const*);
				static array_view<const var> get_user_args_property(base_property<var_args_t>const*);

			public:
				wnd_create_args(const wnd_create_args&);
				wnd_create_args(wstring className, wstring wndName
					, graphics::rect<float> area = graphics::rect<float>(100, 100, 900, 700)
					, wnd_style_t wndStyle = wnd_style::overlapped_window + wnd_style::shown
					, pointer wndParent = null
					, var_args_t userArgs = null);

				ANG_DECLARE_INTERFACE();

				property<const wstring, get_wnd_name_property> wnd_name;
				property<const wstring, get_wnd_class_name_property> wnd_class_name;
				property<const graphics::rect<float>, get_rect_area_property> rect_area;
				property<const wnd_style_t, get_style_property> style;
				property<const pointer, get_parent_property> parent;
				property<const var_args_t, get_user_args_property> user_args;

			protected:
				virtual~wnd_create_args();
			};

			typedef object_wrapper<wnd_create_args> wnd_create_args_t;

			struct LINK wnd_create_args_ex
				: public smart<wnd_create_args_ex, wnd_create_args>
			{
			private:
				wnd_style_ex_t m_wnd_style_ex;
				static wnd_style_ex_t get_style_ex_property(base_property<wnd_style_ex_t>const*);

			public:
				wnd_create_args_ex(const wnd_create_args_ex&);
				wnd_create_args_ex(string className, string wndName
					, graphics::rect<float> area = graphics::rect<float>(100, 100, 900, 700)
					, wnd_style_ex_t wndStyleEx = wnd_style_ex::none
					, wnd_style_t wndStyle = wnd_style::overlapped_window
					, pointer wndParent = null
					, var_args_t userArgs = null);

				ANG_DECLARE_INTERFACE();
				property<const wnd_style_ex_t, get_style_ex_property> style_ex;

			protected:
				virtual~wnd_create_args_ex();
			};
			typedef object_wrapper<wnd_create_args_ex> wnd_create_args_ex_t;

	
			class LINK window
				: public smart<window, icore_view>
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
	
			private: //property handlers
				static window* get_parent_property(base_property<wndptr> const* prop);
				static void set_parent_property(base_property<wndptr>* prop, const wndptr&);
				static graphics::point<float> get_position_property(base_property<graphics::point<float>> const* prop);
				static void set_position_property(base_property<graphics::point<float>>* prop, graphics::point<float>&&);
				static graphics::size<float> get_clip_size_property(base_property<graphics::size<float>> const* prop);
				static void set_clip_size_property(base_property<graphics::size<float>>* prop, graphics::size<float>&&);
				static graphics::size<float> get_wnd_size_property(base_property<graphics::size<float>> const* prop);
				static hwnd_t get_handle_property(base_property<hwnd_t> const* prop);
				static bool get_is_created_property(base_property<bool> const* prop);
			public:
				window();
				window(wnd_create_args_t);

			protected:
				virtual~window();

			public: //Events
				ang_platform_event(events::icreated_event_args, created_event);
				ang_platform_event(events::imsg_event_args, closed_event);
				ang_platform_event(events::imsg_event_args, destroyed_event);

				ang_platform_event(events::idraw_event_args, draw_event);
				ang_platform_event(events::imsg_event_args, update_event);

				ang_platform_event(events::idisplay_info_event_args, orientation_event);
				ang_platform_event(events::idisplay_info_event_args, activate_event);
				ang_platform_event(events::idisplay_info_event_args, size_event);
				ang_platform_event(events::idisplay_info_event_args, display_change_event);

				ang_platform_event(events::ikey_event_args, char_event);
				ang_platform_event(events::ikey_event_args, key_pressed_event);
				ang_platform_event(events::ikey_event_args, key_released_event);

				ang_platform_event(events::ipointer_event_args, pointer_entered_event);
				ang_platform_event(events::ipointer_event_args, pointer_pressed_event);
				ang_platform_event(events::ipointer_event_args, pointer_moved_event);
				ang_platform_event(events::ipointer_event_args, pointer_released_event);
				ang_platform_event(events::ipointer_event_args, pointer_leaved_event);

				ang_platform_event(events::ipointer_event_args, mouse_moved_event);
				ang_platform_event(events::ipointer_event_args, mouse_lbutton_pressed_event);
				ang_platform_event(events::ipointer_event_args, mouse_rbutton_pressed_event);
				ang_platform_event(events::ipointer_event_args, mouse_lbutton_released_event);
				ang_platform_event(events::ipointer_event_args, mouse_rbutton_released_event);


			public: //properties
				property<wndptr , get_parent_property, set_parent_property> parent;
				property<graphics::point<float>, get_position_property, set_position_property> position;
				property<graphics::size<float>, get_clip_size_property, set_clip_size_property> clip_size;
				property<const graphics::size<float>, get_wnd_size_property> wnd_size;
				property<const hwnd_t, get_handle_property> handle;
				property<const bool, get_is_created_property> is_created;

			public: //methods
				void parent_from_handle(pointer);
				void screen_to_client(graphics::point<float>&);
				void client_to_screen(graphics::point<float>&);
				void screen_to_client(graphics::rect<float>&);
				void client_to_screen(graphics::rect<float>&);		
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
				
				virtual bool create(wnd_create_args_t);

			protected:
				virtual bool close();

				virtual void attach(pointer);
				virtual void detach();
				virtual void def_window_proc(events::message& msg);

				dword send_msg(events::message);
				core::async::iasync<void> post_task(core::async::iasync<void>);
				events::event_token_t listen_to(events::event_t);

			public: //Custom overrides
				virtual void window_proc(events::message& msg);
				virtual void initial_update();

			protected: //Internal Event Handlers
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
				: public smart<process>
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
				
			public: //Overrides
				ANG_DECLARE_INTERFACE();

				pointer handle()const;
				core::async::idispatcher_t dispatcher()const;

			protected:
				dword send_msg(events::message);
				core::async::iasync<void> post_task(core::async::iasync<void>);
				events::event_token_t listen_to(events::event_t);
				virtual dword dispatch_msg(events::message);

				virtual bool close();

			public: //Properties
				bool is_worker_thread()const;
				core::async::mutex_ptr_t main_mutex()const;
				core::async::cond_ptr_t main_cond()const;
				core::async::thread_t main_worker_thread()const;
				array<string> command_line_args()const;

				var settings(cstr_t)const;
				void settings(cstr_t, var);

			public: //Events
				ang_platform_event(events::iapp_status_event_args, start_app_event);
				ang_platform_event(events::iapp_status_event_args, exit_app_event);

			public: //Custom Implementation
				virtual bool init_app(array<string> cmdl);
				virtual void update_app();
				virtual bool exit_app();
			};

			class LINK app
				: public smart<app, process, icore_app>
			{
			public:
				static appptr current_app();

			protected:
				wndptr m_main_wnd;
				static window* get_main_wnd_property(base_property<wndptr>const* prop) {
					return field_to_parent(&app::main_wnd, prop)->m_main_wnd.get();
				}
				static void set_main_wnd_property(base_property<wndptr>* prop, const wndptr& val) {
					auto proxy = field_to_parent(&app::main_wnd, prop);
					if (!proxy->m_main_wnd.is_empty())
						return;
					proxy->m_main_wnd = val;
				}


			public:
				app();

			protected:
				virtual~app();

			public: //Methods
				core::async::iasync<dword> run_async(wndptr wnd, wnd_create_args_t args);

			public: //Overrides
				ANG_DECLARE_INTERFACE();
				//void set_main_wnd(wndptr mainWindow);

				virtual pointer core_app_handle()const override;
				virtual icore_view_t main_core_view() override;
				virtual input::ikeyboard_t keyboard() override;
				//virtual imessage_reciever_t get_listener()const override;
				//virtual Core::Files::IFileSystem* GetFileSystem();

			protected:
				virtual dword on_run_async(core::async::iasync<dword>, wndptr);

			public: //Properties
				property<window, get_main_wnd_property, set_main_wnd_property> main_wnd;
				
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