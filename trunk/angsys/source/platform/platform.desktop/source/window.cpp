#include "pch.h"
#include "ang/platform/win32/windows.h"
#include "dispatcher.h"


using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;

LRESULT WINAPI window_procedure(HWND hWnd, UINT m, WPARAM wprm, LPARAM lprm);

////////////////////////////////////////////////////////////////////

class_regist_args::class_regist_args(const class_regist_args&args)
	: m_class_name(args.m_class_name), m_class_style(args.m_class_style)
{

}
class_regist_args::class_regist_args(wstring className, class_style_t classStyle, uint menuName, pointer icon, pointer brush, pointer cursor)
	: m_class_name(className), m_class_style(classStyle), m_menu_name(menuName), m_hicon(icon), m_hbrush(brush), m_hcursor(cursor)
{

}
class_regist_args::~class_regist_args()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::class_regist_args);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::class_regist_args, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::class_regist_args, object)

wstring const& class_regist_args::name()const
{
	return m_class_name;
}

class_style_t class_regist_args::style()const
{
	return m_class_style;
}

uint class_regist_args::menu_name()const
{
	return m_menu_name;
}

pointer class_regist_args::icon()const
{
	return m_hicon;
}

pointer class_regist_args::brush()const
{
	return m_hbrush;
}

pointer class_regist_args::cursor()const
{
	return m_hcursor;
}


////////////////////////////////////////////////////////////////////

wnd_create_args::wnd_create_args(const wnd_create_args&args)
	: m_wnd_class_name(args.m_wnd_class_name)
	, m_wnd_name(args.m_wnd_name)
	, m_rect_area(args.m_rect_area)
	, m_wnd_style(args.m_wnd_style)
	, m_parent(args.m_parent)
	, m_user_args(args.m_user_args)
{

}

wnd_create_args::wnd_create_args(wstring className, wstring wndName, graphics::rect<float> area, wnd_style_t wndStyle, pointer wndParent, var_args_t userArgs)
	: m_wnd_class_name(className.get())
	, m_wnd_name(wndName.get())
	, m_rect_area(area)
	, m_wnd_style(wndStyle)
	, m_parent(wndParent)
	, m_user_args(userArgs)
{

}

wnd_create_args::~wnd_create_args()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::wnd_create_args);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::wnd_create_args, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::wnd_create_args, object);

wstring const& wnd_create_args::wnd_name()const
{
	return m_wnd_name;
}

wstring const& wnd_create_args::wnd_class_name()const
{
	return m_wnd_class_name;
}

graphics::rect<float> wnd_create_args::rect_area()const
{
	return m_rect_area;
}

wnd_style_t wnd_create_args::style()const
{
	return m_wnd_style;
}

pointer wnd_create_args::parent()const
{
	return m_parent;
}

var_args_t wnd_create_args::user_args()const
{
	return m_user_args;
}

////////////////////////////////////////////////////////////////////

wnd_create_args_ex::wnd_create_args_ex(const wnd_create_args_ex&args)
	: wnd_create_args(args)
	, m_wnd_style_ex(args.m_wnd_style_ex)
{

}

wnd_create_args_ex::wnd_create_args_ex(string className, string wndName, graphics::rect<float> area, wnd_style_ex_t wndExStyle, wnd_style_t wndStyle, pointer wndParent, var_args_t userArgs)
	: wnd_create_args(className, wndName, area, wndStyle, wndParent, userArgs)
	, m_wnd_style_ex(wndExStyle)
{

}

wnd_create_args_ex::~wnd_create_args_ex()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::wnd_create_args_ex);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::wnd_create_args_ex, wnd_create_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::wnd_create_args_ex, wnd_create_args);


wnd_style_ex_t wnd_create_args_ex::style_ex()const
{
	return m_wnd_style_ex;
}

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			typedef struct window::_hwnd
			{
				HWND m_hwnd = null;
				window_t m_wnd = null;
				dispatcher_t m_dispatcher;
				core::async::mutex_ptr_t m_mutex = make_shared<core::async::mutex>();
				core::async::cond_ptr_t m_cond = make_shared<core::async::cond>();
				//display::orientation_t current_orientation;
				var_args_t m_user_args = null;

			}*hwnd;
		}
	}
}

#if defined _DEBUG
#define NEW new(__FILE__, __LINE__)
#else
#define NEW new
#endif

void window::adjust_window_rect(graphics::rect<float>& rect, wnd_style_t style, bool hasMenu)
{

	RECT r = { (long)rect.left, (long)rect.top, (long)rect.right, (long)rect.bottom };
	::AdjustWindowRect(&r, (dword)style, hasMenu);

	rect.left = (float)r.left;
	rect.top = (float)r.top;
	rect.right = (float)r.right;
	rect.bottom = (float)r.bottom;
}

window_t window::handle_to_window(pointer hWnd)
{
	if (hWnd == null)
		return null;

	auto handle = reinterpret_cast<hwnd>(GetWindowLongPtrW((HWND)hWnd, GWLP_USERDATA));
	if (!handle)
		return null;

	return handle->m_wnd;
}

wstring window::regist_wnd_class(class_regist_args_t args)
{
	if (args.is_empty())
		return (cstr_t)null;
	WNDCLASSEXW wcex;

	if (GetClassInfoExW(GetModuleHandle(NULL), (cwstr_t)args->name(), &wcex))
	{
		return args->name();
	}

	wcex.cbSize = sizeof(WNDCLASSEXW);

	cwstr_t className = args->name();

	wcex.style = (dword)args->style();
	wcex.lpfnWndProc = (WNDPROC)&window_procedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = (HICON)args->icon();// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINGRAPH3D));
	wcex.hCursor = args->cursor() ? (HCURSOR)args->cursor() : LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)args->brush() ? (HBRUSH)args->brush() : (HBRUSH)GetStockObject(BLACK_BRUSH);// (HBRUSH)GetStockObject(BLACK_BRUSH); //(HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName = args->menu_name() ? MAKEINTRESOURCEW(args->menu_name()) : NULL;
	wcex.lpszClassName = className;// args->name();
	wcex.hIconSm = NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (RegisterClassExW(&wcex) == 0)
		return null;
	return args->name().get();
}

void window::unregist_wnd_class(wstring className)
{
	UnregisterClassW((cwstr_t)className, GetModuleHandle(NULL));
}

window::window(wnd_create_args_t args)
	: window()
{
	create(args);
}



window::window()
	: m_handle(null)
	, created_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::created; })
	, closed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::close; })
	, destroyed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::destroyed; })
	, draw_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::draw; })
	, update_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::update; })
	, orientation_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::orientation; })
	, activate_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::got_focus || msg == (core_msg_t)win_msg::lost_focus; })
	, size_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::size; })
	, display_change_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::display_change; })
	, char_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::put_char; })
	, key_pressed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::key_down; })
	, key_released_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::key_up; })
	, pointer_entered_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_entered; })
	, pointer_leaved_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_leaved; })
	, pointer_moved_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_moved; })
	, pointer_pressed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_pressed; })
	, pointer_released_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_pressed; })
	, mouse_moved_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::mouse_move; })
	, mouse_lbutton_pressed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::lbutton_down; })
	, mouse_lbutton_released_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::lbutton_up; })
	, mouse_rbutton_pressed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::rbutton_down; })
	, mouse_rbutton_released_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::rbutton_up; })
{
}

window::~window()
{
	owner(created_event).empty();
	owner(destroyed_event).empty();
	owner(draw_event).empty();
	owner(update_event).empty();
	owner(orientation_event).empty();
	owner(activate_event).empty();
	owner(size_event).empty();
	owner(char_event).empty();
	owner(key_pressed_event).empty();
	owner(key_released_event).empty();
	owner(pointer_moved_event).empty();
	owner(pointer_pressed_event).empty();
	owner(pointer_released_event).empty();
	owner(mouse_moved_event).empty();
	owner(mouse_lbutton_pressed_event).empty();
	owner(mouse_lbutton_released_event).empty();
	owner(mouse_rbutton_pressed_event).empty();
	owner(mouse_rbutton_released_event).empty();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::window);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::window, object, icore_view);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::window, object, icore_view);

pointer window::core_view_handle()const
{
	if (is_created())
		return m_handle->m_hwnd;
	return null;
}

icore_context_t window::core_context()const
{
	return new graphics::device_context(const_cast<window*>(this));
}

graphics::size<float> window::core_view_size()const
{
	return clip_size();
}

graphics::size<float> window::core_view_scale_factor()const
{
	return graphics::size<float>(1, 1);
}

bool window::is_created()const
{
	return (m_handle != null);
}

bool window::create(wnd_create_args_t args)
{
	if (args.is_empty())
		return false;

	wnd_create_args_ex_t argsx = args->as<wnd_create_args_ex>();

	HMODULE instance = GetModuleHandleW(NULL);
	WNDCLASSW wndclass = { 0 };
	class_regist_args_t class_arg = new class_regist_args("ang::windows::window"_s
		, class_style_t(class_style::own_dc + class_style::save_bits + class_style::hredraw + class_style::vredraw));

	wstring class_name = args->wnd_class_name().get();

	if (!class_name.is_empty() && class_name->length() > 0)
	{
		if (!GetClassInfoW(instance, (cwstr_t)class_name, &wndclass))
		{
			if (window::regist_wnd_class(class_arg).is_empty())
				return false;
			class_name = "ang::windows::window"_s;
		}
	}
	else
	{
		if (window::regist_wnd_class(class_arg).is_empty())
			return false;
		class_name = "ang::windows::window"_s;
	}

	hwnd handle = new _hwnd();

	handle->m_wnd = this;

	pointer parent = args->parent();
	handle->m_user_args = args->user_args();

	auto style = args->style() - wnd_style::shown;
	dword st = style;
	handle->m_hwnd = CreateWindowExW(argsx.is_empty() ? 0U : (dword)argsx->style_ex(), (cwstr_t)class_name
		, (cwstr_t)args->wnd_name(), style
		, (int)args->rect_area().left, (int)args->rect_area().top
		, (int)args->rect_area().width(), (int)args->rect_area().height()
		, (HWND)parent, null
		, instance, handle);

	if (handle->m_hwnd == null)
	{
		HRESULT hr = GetLastError();
		printf("Error:%d\n", hr);
		UnregisterClassW((cwstr_t)class_name, instance);
		delete handle;
		return false;
	}

	send_msg(message((core_msg_t)win_msg::initial_update));
	//attach(handle);
	if (args->style() & wnd_style::shown)
		show(showing_flag::show);

	return true;
}

//dword_t window::release()
//{
//	return object::release();
//}

bool window::close()
{
	if (m_handle)
	{
		auto handle = m_handle;
		detach();
		SetWindowLongPtrW(handle->m_hwnd, GWLP_USERDATA, NULL);
		handle->m_wnd = nullptr;
		delete handle;
	}
	return true;
}

void window::attach(pointer h)
{
	m_handle = reinterpret_cast<hwnd_t>(h);
}

void window::detach()
{
	m_handle = null;
}

core::async::iasync<void> window::post_task(core::async::iasync<void> async)
{
	if (!is_created())
		return null;

	auto task = interface_cast<async_task>(async.get());
	if (task == null)
		return null;
	task->add_ref();
	if (PostMessageW(m_handle->m_hwnd,(core_msg_t)win_msg::system_reserved_event, 0, reinterpret_cast<LPARAM>(task)) == 0)
	{
		task->release();
		return null;
	}
	return async;
}

imessage_listener_t window::listener()const
{
	return is_created() ? m_handle->m_dispatcher.get() : null;
}

core::async::idispatcher_t window::dispatcher()const
{
	return is_created() ? m_handle->m_dispatcher.get() : null;
}

bool window::listen_to(event_t event)
{
	switch ((win_msg)event.get()->msg_type())
	{
	case win_msg::created:
		created_event += event;
		return true;
	case win_msg::destroyed:
		destroyed_event += event;
		return true;
	case win_msg::draw:
		draw_event += event;
		return true;
	case win_msg::update:
		update_event += event;
		return true;
	case win_msg::size:
		size_event += event;
		return true;
	case win_msg::pointer_moved:
		pointer_moved_event += event;
		return true;
	case win_msg::pointer_pressed:
		pointer_pressed_event += event;
		return true;
	case win_msg::pointer_released:
		pointer_released_event += event;
		return true;
	default:
		return false;
	}
}


dword window::send_msg(message msg)
{
	return ::SendMessageW(
			(HWND)core_view_handle(),
			(UINT)msg.msg(),
			(WPARAM)msg.wparam(),
			(LPARAM)msg.lparam()
		);
}

void window::show(showing_flag_t sf)
{
	ShowWindow((HWND)core_view_handle(), (dword)sf.get());
}

void window::update_wnd()const
{
	UpdateWindow((HWND)core_view_handle());
}

window_t window::parent()
{
	return handle_to_window(GetParent((HWND)core_view_handle()));
}

void window::parent(window_t wnd)
{
	if ((wnd == null) || (wnd->core_view_handle() == null))
		SetParent((HWND)core_view_handle(), NULL);

	SetParent((HWND)core_view_handle(), (HWND)wnd->core_view_handle());
}

void window::parent_from_handle(pointer wnd)
{
	if ((wnd == null))
		SetParent((HWND)core_view_handle(), NULL);
	SetParent((HWND)core_view_handle(), (HWND)wnd);
}

void window::screen_to_client(graphics::point<float>& p)
{
	POINT ptn = { LONG(p.x), LONG(p.x) };
	::ScreenToClient((HWND)core_view_handle(), &ptn);
	p.x = (float)ptn.x;
	p.y = (float)ptn.y;
}

void window::client_to_screen(graphics::point<float>&p)
{
	POINT ptn = { LONG(p.x), LONG(p.x) };
	::ClientToScreen((HWND)core_view_handle(), &ptn);
	p.x = (float)ptn.x;
	p.y = (float)ptn.y;
}

void window::screen_to_client(graphics::rect<float>&r)
{
	POINT ptn = { LONG(r.left), LONG(r.top) };
	::ScreenToClient((HWND)core_view_handle(), &ptn);

	r.move_to((float)ptn.x, (float)ptn.y);
}

void window::client_to_screen(graphics::rect<float>&r)
{
	POINT ptn = { LONG(r.left), LONG(r.top) };
	::ClientToScreen((HWND)core_view_handle(), &ptn);
	r.move_to((float)ptn.x, (float)ptn.y);
}

graphics::point<float> window::position()
{
	RECT rw;
	GetWindowRect((HWND)core_view_handle(), &rw);
	graphics::point<float> p((float)rw.left, (float)rw.top);
	window* wnd = parent();
	if (wnd != null)
		wnd->screen_to_client(p);
	return p;
}

graphics::size<float> window::wnd_size()
{
	RECT rw;
	GetWindowRect((HWND)core_view_handle(), &rw);
	graphics::size<float> s((float)rw.right - (float)rw.left, (float)rw.bottom - (float)rw.top);
	return s;
}

graphics::size<float> window::clip_size()const
{
	RECT rw;
	GetClientRect((HWND)core_view_handle(), &rw);
	graphics::size<float> s((float)rw.right - (float)rw.left, (float)rw.bottom - (float)rw.top);
	return s;
}

void window::position(graphics::point<float> p)
{
	//graphics::size<float> s = wnd_size();
	//MoveWindow((HWND)core_view_handle(), (int)p.x, (int)p.y, (int)s.width, (int)s.height, true);
	auto handle = (HWND)core_view_handle();
	if (::IsWindow(handle))
		::SetWindowPos(handle, NULL, (int)p.x, (int)p.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
}

void window::size(graphics::size<float> s)
{
	//graphics::point<float> p = position();
	//MoveWindow((HWND)core_view_handle(), (int)p.x, (int)p.y, (int)s.width, (int)s.height, true);
	auto handle = (HWND)core_view_handle();
	if(::IsWindow(handle))
		::SetWindowPos(handle, NULL, 0, 0, (int)s.width, (int)s.height, SWP_NOACTIVATE | SWP_NOMOVE);
}

void window::redraw()
{
	InvalidateRect((HWND)core_view_handle(), NULL, FALSE);
}

//Message handler

LRESULT WINAPI window_procedure(HWND hWnd, UINT m, WPARAM wprm, LPARAM lprm)
{
	message msg(m, wprm, lprm);
	window_t wnd = null;

	if (((core_msg_t)win_msg::created == m) && (lprm != 0))
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lprm;
		auto handle = (hwnd)pcs->lpCreateParams;
		if (handle)
		{
			handle->m_hwnd = (HWND)hWnd;
			wnd = handle->m_wnd;
			//msg = new message(m, handle, (pointer)lprm);
			SetWindowLongPtrW((HWND)hWnd, GWLP_USERDATA, (LONG_PTR)handle);
		}
	}
	else
	{
		wnd = window::handle_to_window(hWnd);
	}

	if (!wnd.is_empty())
	{
		wnd->window_proc(msg);
		return msg.result();
	}
	return DefWindowProcW((HWND)hWnd, m, wprm, lprm);
}

void window::window_proc(message& msg)
{
	switch ((win_msg)msg.msg())
	{
	case win_msg::created: {
		attach(LPCREATESTRUCT((LPARAM)msg.lparam())->lpCreateParams);
		m_handle->m_dispatcher = new windows::dispatcher(
			bind(this, &window::listen_to),
			bind(this, &window::send_msg),
			bind(this, &window::post_task));

		msg.result(on_created(msg));
	}break;

	case win_msg::initial_update: {
		initial_update();
	} break;

	case win_msg::close: {
		on_close(msg);
	} break;

	case win_msg::destroyed: {
		on_destroyed(msg);
	} break;

	case win_msg::draw: {
		on_draw(msg);
	} break;

	case win_msg::update: {
		on_update(msg);
	} break;

	case win_msg::display_change:
	case win_msg::orientation:
	case win_msg::size: {
		on_dysplay_event(msg);
	} break;

	case win_msg::system_reserved_event: {
		on_task_command(msg);
	}

	case win_msg::got_focus:
	case win_msg::lost_focus: {
		on_activate(msg);
	} break;

	case win_msg::pointer_moved:
	case win_msg::pointer_pressed:
	case win_msg::pointer_released: {
		on_pointer_event(msg);
	} break;

	
	case win_msg::mouse_move:
	case win_msg::lbutton_down:
	case win_msg::rbutton_down:
	case win_msg::xbutton_down:
	case win_msg::lbutton_up:
	case win_msg::rbutton_up:
	case win_msg::xbutton_up: {
		on_mouse_event(msg);
	} break;

	case win_msg::key_down:
	case win_msg::put_char: 
	case win_msg::key_up: {
		on_key_event(msg);
		break;
	}
	default: {
		def_window_proc(msg);
	} break;
	}
}

void window::def_window_proc(events::message& msg)
{
	msg.result(DefWindowProcW((HWND)core_view_handle(), msg.msg(), (WPARAM)msg.wparam(), (LPARAM)msg.lparam()));
}

void window::initial_update()
{
	return;
}

events::event_trigger<window>& window::owner(events::event_listener& listener)
{
	return static_cast<events::event_trigger<window>&>(listener);
}

int window::trigger(events::event_listener const& listener, events::imsg_event_args_t args)const
{
	return static_cast<events::event_trigger<window>const&>(listener).invoke(args);
}

dword window::on_created(events::message& m)
{
	icreated_event_args_t args = new created_event_args(m, this, app::current_app(), m_handle->m_user_args);
	try { 
		trigger(created_event, args.get());
	}
	catch (const exception_t& e) { 
	}
	def_window_proc(m);
	return m.result();
}

dword window::on_close(events::message& m)
{
	imsg_event_args_t args = new msg_event_args(m);

	try { trigger(closed_event, args.get()); }
	catch (const exception_t& e) {
	}
	def_window_proc(m);
	return m.result();
}

dword window::on_destroyed(events::message& m)
{
	imsg_event_args_t args = new msg_event_args(m);
	try { trigger(destroyed_event, args.get()); }
	catch (const exception_t& e) { 
	}
	def_window_proc(m);

	close();
	return m.result();
}

dword window::on_draw(events::message& m)
{
	graphics::device_context_t dc = new graphics::paint_dc(this);
	if (dc->get_HDC() == null)
		dc = new graphics::device_context(this);

	idraw_event_args_t args = new draw_event_args(m, this, dc, clip_size());
	try { 
		trigger(draw_event, args.get()); 
	}
	catch (const exception_t& e) {	
	}
	m.result(0);
	return m.result();
}

dword window::on_update(events::message& m)
{
	
	iupdate_event_args_t args = new update_event_args(m, (double)m.wparam() / 1000000.0, (double)m.lparam() / 1000000.0);
	try { 
		trigger(update_event,args.get());
	}
	catch (const exception_t& e) { 
	}
	m.result(0);
	return m.result();
}

dword window::on_activate(events::message& m)
{
	int handled = 0;
	activate_status_t status = m.msg() == (core_msg_t)win_msg::got_focus ? activate_status::activated : activate_status::deactivated;

	iactivate_event_args_t args = new activate_event_args(m, status);
	try {
		handled = trigger(activate_event, args.get());
	}
	catch (const exception_t& e) { 
	}
	if (!handled) def_window_proc(m);
	else m.result(0);
	return m.result();
}

struct display_info_event_args2 {
	display_invalidate_reason_t state;
	display::display_info info;
};


dword window::on_dysplay_event(events::message& m)
{
	dword value = (dword)m.lparam();

	display_invalidate_reason_t reason
		= (win_msg)m.msg() == win_msg::size ? display_invalidate_reason::size_changed
		: (win_msg)m.msg() == win_msg::display_change ? display_invalidate_reason::display_invalidate
		: (win_msg)m.msg() == win_msg::orientation ? display_invalidate_reason::orientation_changed
		: display_invalidate_reason::none;

	display::display_info info = {
			system_info::current_screen_orientation(),
			system_info::current_screen_orientation(),
			graphics::size<float>((float)LOWORD(value), (float)HIWORD(value)),
			core_view_scale_factor(),
			96
	};
	
	m.result(-1);
	idisplay_info_event_args_t args = new display_info_event_args(m, this, reason, info);
	try {
		switch ((win_msg)m.msg())
		{
		case win_msg::size: trigger(size_event, args.get()); break;
		case win_msg::display_change: trigger(display_change_event, args.get()); break;
		case win_msg::orientation: trigger(orientation_event, args.get()); break;
		}	
	}
	catch (const exception_t& e) { 
	}
	def_window_proc(m);
	return m.result();
}

dword window::on_pointer_event(events::message& m)
{
	WPARAM wprm = (WPARAM)m.wparam();
	LPARAM lprm = (LPARAM)m.lparam();

	short id;
	bool is_pa;
	bool is_sa;
	input::pointer_hardware_type_t type;
	input::key_modifiers_t modifiers = input::key_modifiers::none;

	POINTER_INFO pi;
	id = (short)GET_POINTERID_WPARAM(wprm);
	GetPointerInfo((uint)id, &pi);
	type = (input::pointer_hardware_type)(pi.pointerType - 2);
	is_pa = IS_POINTER_FIRSTBUTTON_WPARAM(wprm);
	is_sa = IS_POINTER_SECONDBUTTON_WPARAM(wprm);
	//POINTER_MOD_SHIFT

	modifiers += ((POINTER_MOD_CTRL & pi.dwKeyStates) == POINTER_MOD_CTRL) ? input::key_modifiers::control : input::key_modifiers::none;
	modifiers += ((POINTER_MOD_SHIFT & pi.dwKeyStates) == POINTER_MOD_SHIFT) ? input::key_modifiers::shift : input::key_modifiers::none;

	ipointer_event_args_t args = new pointer_event_args(m, {
		graphics::point<float>((float)GET_X_LPARAM(lprm), (float)GET_Y_LPARAM(lprm)),
		id,
		is_pa,
		is_sa,
		type,
		modifiers,
		});

	int count = 0;
	try { 
		switch ((win_msg)m.msg())
		{
		case win_msg::pointer_entered: count = trigger(pointer_entered_event, args.get()); m.result(0); break;
		case win_msg::pointer_pressed: count = trigger(pointer_pressed_event, args.get()); m.result(0); break;
		case win_msg::pointer_moved: count = trigger(pointer_moved_event, args.get()); m.result(0); break;
		case win_msg::pointer_released: count = trigger(pointer_released_event, args.get()); m.result(0); break;
		case win_msg::pointer_leaved: count = trigger(pointer_leaved_event, args.get()); m.result(0); break;
		}
	}
	catch (const exception_t& e) {
	}
	if (count == 0)
		def_window_proc(m);
	return m.result();
}


dword window::on_mouse_event(events::message& m)
{
	WPARAM wprm = (WPARAM)m.wparam();
	LPARAM lprm = m.lparam();

	short id;
	bool is_pa;
	bool is_sa;
	input::pointer_hardware_type_t type;
	input::key_modifiers_t modifiers = input::key_modifiers::none;

	id = 1U;
	is_pa = (MK_LBUTTON & wprm) == MK_LBUTTON;
	is_sa = (MK_RBUTTON & wprm) == MK_RBUTTON;
	type = input::pointer_hardware_type::mouse;
	modifiers += ((MK_CONTROL & wprm) == MK_CONTROL) ? input::key_modifiers::control : input::key_modifiers::none;
	modifiers += ((MK_SHIFT & wprm) == MK_SHIFT) ? input::key_modifiers::shift : input::key_modifiers::none;

	ipointer_event_args_t args = new pointer_event_args(m, {
		graphics::point<float>((float)GET_X_LPARAM(lprm), (float)GET_Y_LPARAM(lprm)),
		id,
		is_pa,
		is_sa,
		type,
		modifiers,
		});
	int count = 0;
	try {
		switch ((win_msg)m.msg())
		{
		case win_msg::mouse_move: count = trigger(mouse_moved_event, args.get()); m.result(0); break;
		case win_msg::lbutton_down: count = trigger(mouse_lbutton_pressed_event, args.get()); m.result(0); break;
		case win_msg::rbutton_down: count = trigger(mouse_rbutton_pressed_event, args.get()); m.result(0); break;
		case win_msg::lbutton_up: count = trigger(mouse_lbutton_released_event, args.get()); m.result(0); break;
		case win_msg::rbutton_up: count = trigger(mouse_rbutton_released_event, args.get()); m.result(0); break;

		}
	}
	catch (const exception_t& e) { 
	}
	if (count == 0)
		def_window_proc(m);
	return m.result();
}


dword window::on_key_event(events::message& m)
{
	
	uint modifiers = 0;
	if (GetKeyState(VK_CONTROL) && 0x8000)
		modifiers |= (uint)input::key_modifiers::control;
	if (GetKeyState(VK_SHIFT) && 0x8000)
		modifiers |= (uint)input::key_modifiers::shift;
	if (GetKeyState(VK_MENU) && 0x8000)
		modifiers |= (uint)input::key_modifiers::alt;
	if (GetKeyState(VK_CAPITAL) && 0x0001)
		modifiers |= (uint)input::key_modifiers::caps_lock;
	if (GetKeyState(VK_NUMLOCK) && 0x0001)
		modifiers |= (uint)input::key_modifiers::num_lock;

	ikey_event_args_t args = new key_event_args(m, {
		(input::virtual_key)m.wparam(), //property<const virtual_key> key;
		(char32_t)m.wparam(), //property<const virtual_key> key;
		(word)(uint)m.lparam(), //property<const word> flags;
		(win_msg)m.msg() == win_msg::key_down ? input::key_state::pressed
		: (win_msg)m.msg() == win_msg::put_char ? input::key_state::pressed
		: input::key_state::released,
		(input::key_modifiers)modifiers //property<const key_modifiers> modifiers;
		});
	m.result(-1);
	try {
		switch ((win_msg)m.msg())
		{
		case win_msg::key_down: trigger(key_pressed_event, args.get()); m.result(0); break;
		case win_msg::put_char: trigger(char_event, args.get()); m.result(0); break;
		case win_msg::key_up: trigger(key_released_event, args.get()); m.result(0); break;
		}
	}
	catch (const exception_t& e) {
	}
	def_window_proc(m);
	return m.result();
}

dword window::on_task_command(events::message& m)
{
	auto task = reinterpret_cast<async_task*>(m.lparam());
	task->execute();
	task->release();
	m.result(0);
	return 0;
}
