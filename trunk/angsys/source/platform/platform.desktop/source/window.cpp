#include "pch.h"
#include "ang/platform/win32/windows.h"
#include "dispatcher.h"
#include "event_args.h"


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

wstring class_regist_args::get_name_propery(base_property<wstring>const* prop) {
	return field_to_parent(&self::name, prop)->m_class_name.get();
}

class_style_t class_regist_args::get_style_propery(base_property<class_style_t>const* prop) {
	return field_to_parent(&self::style, prop)->m_class_style;
}

uint class_regist_args::get_menu_name_propery(base_property<uint>const* prop) {
	return field_to_parent(&self::menu_name, prop)->m_menu_name;
}
pointer class_regist_args::get_icon_propery(base_property<pointer>const* prop) {
	return field_to_parent(&self::icon, prop)->m_hicon;
}
pointer class_regist_args::get_brush_propery(base_property<pointer>const* prop) {
	return field_to_parent(&self::brush, prop)->m_hbrush;
}
pointer class_regist_args::get_cursor_propery(base_property<pointer>const* prop) {
	return field_to_parent(&self::cursor, prop)->m_hcursor;
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

wstring wnd_create_args::get_wnd_name_property(base_property<wstring>const*prop) {
	return field_to_parent(&self::wnd_name, prop)->m_wnd_name.get();
}
wstring wnd_create_args::get_wnd_class_name_property(base_property<wstring>const*prop) {
	return field_to_parent(&self::wnd_class_name, prop)->m_wnd_class_name.get();
}
graphics::rect<float> wnd_create_args::get_rect_area_property(base_property<graphics::rect<float>>const*prop) {
	return field_to_parent(&self::rect_area, prop)->m_rect_area;
}
wnd_style_t wnd_create_args::get_style_property(base_property<wnd_style_t>const*prop){
	return field_to_parent(&self::style, prop)->m_wnd_style;
}
pointer wnd_create_args::get_parent_property(base_property<pointer>const*prop) {
	return field_to_parent(&self::parent, prop)->m_parent;
}
array_view<const var> wnd_create_args::get_user_args_property(base_property<var_args_t>const*prop) {
	return (array_view<const var>)field_to_parent(&self::user_args, prop)->m_user_args;
}
////////////////////////////////////////////////////////////////////

wnd_create_args_ex::wnd_create_args_ex(const wnd_create_args_ex&args)
	: base(args)
	, m_wnd_style_ex(args.m_wnd_style_ex)
{

}

wnd_create_args_ex::wnd_create_args_ex(string className, string wndName, graphics::rect<float> area, wnd_style_ex_t wndExStyle, wnd_style_t wndStyle, pointer wndParent, var_args_t userArgs)
	: base(
		forward<string>(className),
		forward<string>(wndName), 
		forward<graphics::rect<float>>(area),
		forward<wnd_style_t>(wndStyle),
		forward<pointer>(wndParent),
		forward<var_args_t>(userArgs))
	,m_wnd_style_ex(forward<wnd_style_ex_t>(wndExStyle))
{

}

wnd_create_args_ex::~wnd_create_args_ex()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::wnd_create_args_ex);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::wnd_create_args_ex, wnd_create_args);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::wnd_create_args_ex, wnd_create_args);

wnd_style_ex_t wnd_create_args_ex::get_style_ex_property(base_property<wnd_style_ex_t>const*prop) {
	return field_to_parent(&self::style_ex, prop)->m_wnd_style_ex;
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

	auto h = reinterpret_cast<hwnd>(GetWindowLongPtrW((HWND)hWnd, GWLP_USERDATA));
	if (!h)
		return null;

	return h->m_wnd;
}

wstring window::regist_wnd_class(class_regist_args_t args)
{
	if (args.is_empty())
		return (cstr_t)null;
	WNDCLASSEXW wcex;

	if (GetClassInfoExW(GetModuleHandle(NULL), (cwstr_t)args->name, &wcex))
	{
		return args->name;
	}

	wcex.cbSize = sizeof(WNDCLASSEXW);

	cwstr_t className = args->name;

	wcex.style = (dword)args->style;
	wcex.lpfnWndProc = (WNDPROC)&window_procedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = (HICON)args->icon;// LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINGRAPH3D));
	wcex.hCursor = args->cursor ? (HCURSOR)args->cursor : LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = args->brush ? (HBRUSH)args->brush : (HBRUSH)GetStockObject(BLACK_BRUSH);// (HBRUSH)GetStockObject(BLACK_BRUSH); //(HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName = args->menu_name ? MAKEINTRESOURCEW(args->menu_name) : NULL;
	wcex.lpszClassName = className;// args->name();
	wcex.hIconSm = NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (RegisterClassExW(&wcex) == 0)
		return null;
	return (text::wstring_buffer*)args->name;
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
	, m_created_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::created; })
	, m_closed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::close; })
	, m_destroyed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::destroyed; })
	, m_draw_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::draw; })
	, m_update_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::update; })
	, m_orientation_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::orientation; })
	, m_activate_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::got_focus || msg == (core_msg_t)win_msg::lost_focus; })
	, m_size_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::size; })
	, m_display_change_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::display_change; })
	, m_char_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::put_char; })
	, m_key_pressed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::key_down; })
	, m_key_released_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::key_up; })
	, m_pointer_entered_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_entered; })
	, m_pointer_leaved_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_leaved; })
	, m_pointer_moved_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_moved; })
	, m_pointer_pressed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_pressed; })
	, m_pointer_released_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::pointer_pressed; })
	, m_mouse_moved_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::mouse_move; })
	, m_mouse_lbutton_pressed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::lbutton_down; })
	, m_mouse_lbutton_released_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::lbutton_up; })
	, m_mouse_rbutton_pressed_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::rbutton_down; })
	, m_mouse_rbutton_released_event(this, [](core_msg_t msg) { return msg == (core_msg_t)win_msg::rbutton_up; })
{
}

window::~window()
{
	m_created_event.empty();
	m_destroyed_event.empty();
	m_draw_event.empty();
	m_update_event.empty();
	m_orientation_event.empty();
	m_activate_event.empty();
	m_size_event.empty();
	m_char_event.empty();
	m_key_pressed_event.empty();
	m_key_released_event.empty();
	m_pointer_moved_event.empty();
	m_pointer_pressed_event.empty();
	m_pointer_released_event.empty();
	m_mouse_moved_event.empty();
	m_mouse_lbutton_pressed_event.empty();
	m_mouse_lbutton_released_event.empty();
	m_mouse_rbutton_pressed_event.empty();
	m_mouse_rbutton_released_event.empty();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::window);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::window, object, icore_view);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::window, object, icore_view);

ang_platform_implement_event_handler(window, created_event);
ang_platform_implement_event_handler(window, closed_event);
ang_platform_implement_event_handler(window, destroyed_event);

ang_platform_implement_event_handler(window, draw_event);
ang_platform_implement_event_handler(window, update_event);

ang_platform_implement_event_handler(window, orientation_event);
ang_platform_implement_event_handler(window, activate_event);
ang_platform_implement_event_handler(window, size_event);
ang_platform_implement_event_handler(window, display_change_event);

ang_platform_implement_event_handler(window, char_event);
ang_platform_implement_event_handler(window, key_pressed_event);
ang_platform_implement_event_handler(window, key_released_event);

ang_platform_implement_event_handler(window, pointer_entered_event);
ang_platform_implement_event_handler(window, pointer_pressed_event);
ang_platform_implement_event_handler(window, pointer_moved_event);
ang_platform_implement_event_handler(window, pointer_released_event);
ang_platform_implement_event_handler(window, pointer_leaved_event);

ang_platform_implement_event_handler(window, mouse_moved_event);
ang_platform_implement_event_handler(window, mouse_lbutton_pressed_event);
ang_platform_implement_event_handler(window, mouse_rbutton_pressed_event);
ang_platform_implement_event_handler(window, mouse_lbutton_released_event);
ang_platform_implement_event_handler(window, mouse_rbutton_released_event);


pointer window::core_view_handle()const
{
	if (is_created)
		return handle->m_hwnd;
	return null;
}

icore_context_t window::core_context()const
{
	return new graphics::device_context(const_cast<window*>(this));
}

graphics::size<float> window::core_view_size()const
{
	return clip_size;
}

graphics::size<float> window::core_view_scale_factor()const
{
	return graphics::size<float>(1, 1);
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

	wstring class_name = (text::wstring_buffer*)args->wnd_class_name;

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

	hwnd h = new _hwnd();

	h->m_wnd = this;

	pointer parent = args->parent;
	h->m_user_args = (array_view<const var>)args->user_args;

	auto style = args->style - wnd_style::shown;
	dword st = style;
	h->m_hwnd = CreateWindowExW(argsx.is_empty() ? 0U : (dword)argsx->style_ex, (cwstr_t)class_name
		, (cwstr_t)args->wnd_name, style
		, (int)args->rect_area->left, (int)args->rect_area->top
		, (int)args->rect_area->width, (int)args->rect_area->height
		, (HWND)parent, null
		, instance, h);

	if (h->m_hwnd == null)
	{
		HRESULT hr = GetLastError();
		printf("Error:%d\n", hr);
		UnregisterClassW((cwstr_t)class_name, instance);
		delete h;
		return false;
	}

	send_msg(message((core_msg_t)win_msg::initial_update));
	//attach(h);
	if (args->style & wnd_style::shown)
		show(showing_flag::show);

	return true;
}

//dword_t window::release()
//{
//	return object::release();
//}

bool window::close()
{
	if (handle != null)
	{
		hwnd_t h = handle;
		detach();
		SetWindowLongPtrW(h->m_hwnd, GWLP_USERDATA, NULL);
		h->m_wnd = nullptr;
		delete h;
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
	if (!is_created)
		return null;

	auto task = interface_cast<async_task>(async.get());
	if (task == null)
		return null;
	task->add_ref();
	if (PostMessageW(handle->m_hwnd,(core_msg_t)win_msg::system_reserved_event, 0, reinterpret_cast<LPARAM>(task)) == 0)
	{
		task->release();
		return null;
	}
	return async;
}

imessage_listener_t window::listener()const
{
	return is_created ? handle->m_dispatcher.get() : null;
}

core::async::idispatcher_t window::dispatcher()const
{
	return is_created ? handle->m_dispatcher.get() : null;
}

events::event_token_t window::listen_to(event_t event)
{
	switch ((win_msg)event.get()->msg_type())
	{
	case win_msg::created:
		return m_created_event += event;
	case win_msg::destroyed:
		return m_destroyed_event += event;
	case win_msg::draw:
		return m_draw_event += event;
	case win_msg::update:
		return m_update_event += event;
	case win_msg::size:
		return m_size_event += event;
	case win_msg::pointer_moved:
		return m_pointer_moved_event += event;
	case win_msg::pointer_pressed:
		return m_pointer_pressed_event += event;
	case win_msg::pointer_released:
		return m_pointer_released_event += event;
	default:
		return events::event_token();
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

window* window::get_parent_property(base_property<wndptr> const* prop)
{
	return handle_to_window(GetParent((HWND)field_to_parent(&window::parent, prop)->core_view_handle())).get();
}

void set_parent_property(base_property<wndptr>* prop, const wndptr& wnd)
{
	if ((wnd == null) || (wnd->core_view_handle() == null))
		SetParent((HWND)field_to_parent(&window::parent, prop)->core_view_handle(), NULL);
	else
		SetParent((HWND)field_to_parent(&window::parent, prop)->core_view_handle(), (HWND)wnd->core_view_handle());
}

graphics::point<float> window::get_position_property(base_property<graphics::point<float>> const* prop)
{
	RECT rw;
	window* proxy = field_to_parent(&window::position, prop);
	GetWindowRect((HWND)proxy->core_view_handle(), &rw);
	graphics::point<float> p((float)rw.left, (float)rw.top);
	window* wnd = (window*)proxy->parent;
	if (wnd != null)
		wnd->screen_to_client(p);
	return p;
}

void window::set_position_property(base_property<graphics::point<float>>* prop, graphics::point<float>&& point)
{
	window* proxy = field_to_parent(&window::position, prop);
	auto h = (HWND)proxy->core_view_handle();
	if (::IsWindow(h))
		::SetWindowPos(h, NULL, (int)point.x, (int)point.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
}

graphics::size<float> window::get_clip_size_property(base_property<graphics::size<float>> const* prop)
{
	RECT rw;
	GetClientRect((HWND)field_to_parent(&window::clip_size, prop)->core_view_handle(), &rw);
	graphics::size<float> s((float)rw.right - (float)rw.left, (float)rw.bottom - (float)rw.top);
	return s;
}

void window::set_clip_size_property(base_property<graphics::size<float>>* prop, graphics::size<float>&&size)
{
	auto h = (HWND)field_to_parent(&window::clip_size, prop)->core_view_handle();
	if (::IsWindow(h))
		::SetWindowPos(h, NULL, 0, 0, (int)size.width, (int)size.height, SWP_NOACTIVATE | SWP_NOMOVE);
}

graphics::size<float> window::get_wnd_size_property(base_property<graphics::size<float>> const* prop)
{
	RECT rw;
	GetWindowRect((HWND)field_to_parent(&window::wnd_size, prop)->core_view_handle(), &rw);
	graphics::size<float> s((float)rw.right - (float)rw.left, (float)rw.bottom - (float)rw.top);
	return s;
}

window::hwnd_t window::get_handle_property(base_property<window::hwnd_t> const* prop)
{
	return field_to_parent(&window::handle, prop)->m_handle;
}

bool window::get_is_created_property(base_property<bool> const* prop)
{
	return (field_to_parent(&window::is_created, prop)->m_handle != null);
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
		auto h = (hwnd)pcs->lpCreateParams;
		if (h)
		{
			h->m_hwnd = (HWND)hWnd;
			wnd = h->m_wnd;
			//msg = new message(m, h, (pointer)lprm);
			SetWindowLongPtrW((HWND)hWnd, GWLP_USERDATA, (LONG_PTR)h);
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
		handle->m_dispatcher = new windows::dispatcher(
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

	case win_msg::erase_bkgnd:
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

dword window::on_created(events::message& m)
{
	icreated_event_args_t args = new created_event_args(m, this, app::current_app(), handle->m_user_args);
	try { 
		m_created_event(args.get());
	}
	catch (const exception_t& e) { 
	}
	def_window_proc(m);
	return m.result();
}

dword window::on_close(events::message& m)
{
	imsg_event_args_t args = new msg_event_args(m);

	try { m_closed_event(args.get()); }
	catch (const exception_t& e) {
	}
	def_window_proc(m);
	return m.result();
}

dword window::on_destroyed(events::message& m)
{
	imsg_event_args_t args = new msg_event_args(m);
	try { m_destroyed_event(args.get()); }
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

	idraw_event_args_t args = new draw_event_args(m, this, dc, clip_size);
	try { 
		m_draw_event(args.get()); 
	}
	catch (const exception_t& e) {	
	}
	m.result(0);
	return m.result();
}

dword window::on_update(events::message& m)
{
	
	imsg_event_args_t args = new msg_event_args(m);
	try { 
		m_update_event(args.get());
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
		handled = m_activate_event(args.get());
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
		case win_msg::size: m_size_event(args.get()); break;
		case win_msg::display_change: m_display_change_event(args.get()); break;
		case win_msg::orientation: m_orientation_event(args.get()); break;
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
		case win_msg::pointer_entered: count = m_pointer_entered_event(args.get()); m.result(0); break;
		case win_msg::pointer_pressed: count = m_pointer_pressed_event(args.get()); m.result(0); break;
		case win_msg::pointer_moved: count = m_pointer_moved_event(args.get()); m.result(0); break;
		case win_msg::pointer_released: count = m_pointer_released_event(args.get()); m.result(0); break;
		case win_msg::pointer_leaved: count = m_pointer_leaved_event(args.get()); m.result(0); break;
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
		case win_msg::mouse_move: count = m_mouse_moved_event(args.get()); m.result(0); break;
		case win_msg::lbutton_down: count = m_mouse_lbutton_pressed_event(args.get()); m.result(0); break;
		case win_msg::rbutton_down: count = m_mouse_rbutton_pressed_event(args.get()); m.result(0); break;
		case win_msg::lbutton_up: count = m_mouse_lbutton_released_event(args.get()); m.result(0); break;
		case win_msg::rbutton_up: count = m_mouse_rbutton_released_event(args.get()); m.result(0); break;

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
		case win_msg::key_down: m_key_pressed_event(args.get()); m.result(0); break;
		case win_msg::put_char: m_char_event(args.get()); m.result(0); break;
		case win_msg::key_up: m_key_released_event(args.get()); m.result(0); break;
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
