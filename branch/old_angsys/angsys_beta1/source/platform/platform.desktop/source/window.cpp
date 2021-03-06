#include "pch.h"
#include "ang/platform/angwin/angwin.h"
#include "async_msg_task.h"


using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;

//#if defined _DEBUG
//#define new ANG_DEBUG_NEW()
//#endif

LRESULT WINAPI window_procedure(HWND hWnd, UINT m, WPARAM wprm, LPARAM lprm);

////////////////////////////////////////////////////////////////////

class_regist_args::class_regist_args(const class_regist_args&args)
	: _class_name(args._class_name), _class_style(args._class_style)
{

}
class_regist_args::class_regist_args(wstring className, class_style_t classStyle, uint menuName, pointer icon, pointer brush, pointer cursor)
	: _class_name(className), _class_style(classStyle), _menu_name(menuName), _hicon(icon), _hbrush(brush), _hcursor(cursor)
{

}
class_regist_args::~class_regist_args()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::windows::class_regist_args, ang::object);

wstring const& class_regist_args::name()const
{
	return _class_name;
}

class_style_t class_regist_args::style()const
{
	return _class_style;
}

uint class_regist_args::menu_name()const
{
	return _menu_name;
}

pointer class_regist_args::icon()const
{
	return _hicon;
}

pointer class_regist_args::brush()const
{
	return _hbrush;
}

pointer class_regist_args::cursor()const
{
	return _hcursor;
}


////////////////////////////////////////////////////////////////////

wnd_create_args::wnd_create_args(const wnd_create_args&args)
	: _wnd_class_name(args._wnd_class_name)
	, _wnd_name(args._wnd_name)
	, _rect_area(args._rect_area)
	, _wnd_style(args._wnd_style)
	, _parent(args._parent)
	, _user_args(args._user_args)
{

}

wnd_create_args::wnd_create_args(wstring className, wstring wndName, foundation::rect<float> area, wnd_style_t wndStyle, hwnd_t wndParent, var_args_t userArgs)
	: _wnd_class_name(className.get())
	, _wnd_name(wndName.get())
	, _rect_area(area)
	, _wnd_style(wndStyle)
	, _parent(wndParent)
	, _user_args(userArgs)
{

}

wnd_create_args::~wnd_create_args()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::windows::wnd_create_args, ang::object);

wstring const& wnd_create_args::wnd_name()const
{
	return _wnd_name;
}

wstring const& wnd_create_args::wnd_class_name()const
{
	return _wnd_class_name;
}

foundation::rect<float> wnd_create_args::rect_area()const
{
	return _rect_area;
}

wnd_style_t wnd_create_args::style()const
{
	return _wnd_style;
}

pointer wnd_create_args::parent()const
{
	return _parent;
}

var_args_t wnd_create_args::user_args()const
{
	return _user_args;
}

////////////////////////////////////////////////////////////////////

wnd_create_args_ex::wnd_create_args_ex(const wnd_create_args_ex&args)
	: wnd_create_args(args)
	, _wnd_style_ex(args._wnd_style_ex)
{

}

wnd_create_args_ex::wnd_create_args_ex(string className, string wndName, foundation::rect<float> area, wnd_style_ex_t wndExStyle, wnd_style_t wndStyle, hwnd_t wndParent, var_args_t userArgs)
	: wnd_create_args(className, wndName, area, wndStyle, wndParent, userArgs)
	, _wnd_style_ex(wndExStyle)
{

}

wnd_create_args_ex::~wnd_create_args_ex()
{

}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::platform::windows::wnd_create_args_ex, ang::platform::windows::wnd_create_args);

wnd_style_ex_t wnd_create_args_ex::style_ex()const
{
	return _wnd_style_ex;
}

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			typedef struct _HWnd
			{
				HWND hWnd;
				window_t wnd;
				core::async::mutex_t mutex;
				core::async::cond_t cond;
				//display::orientation_t current_orientation;
				var_args_t user_args;
			}*HWnd;
		}
	}
}

void window::adjust_window_rect(foundation::rect<float>& rect, wnd_style_t style, bool hasMenu)
{

	RECT r = { (long)rect.left, (long)rect.top, (long)rect.right, (long)rect.bottom };
	::AdjustWindowRect(&r, style.value(), hasMenu);

	rect.left = (float)r.left;
	rect.top = (float)r.top;
	rect.right = (float)r.right;
	rect.bottom = (float)r.bottom;
}

window_t window::handle_to_window(pointer hWnd)
{
	if (hWnd == null)
		return null;

	auto handle = reinterpret_cast<HWnd>(GetWindowLongPtrW((HWND)hWnd, GWLP_USERDATA));
	if (!handle)
		return null;

	return handle->wnd;
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

	wcex.style = args->style();
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
	: _handle(null)
	, createdEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::created; })
	, destroyedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::destroyed; })
	, drawEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::draw; })
	, updateEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::update; })
	, orientationEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::orientation; })
	, activateEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::got_focus || msg == win_msg_enum::lost_focus; })
	, sizeEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::size; })
	, charEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::put_char; })
	, keyPressedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::key_down; })
	, keyReleasedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::key_up; })
	, pointerMovedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::pointer_moved; })
	, pointerPressedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::pointer_pressed; })
	, pointerReleasedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::pointer_pressed; })
	, mouseMovedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::mouse_move; })
	, mouseButtonPressedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::lbutton_down
		|| msg == win_msg_enum::rbutton_down || msg == win_msg_enum::mbutton_down; })
	, mouseButtonReleasedEvent(this, [](core_msg_t msg) { return msg == win_msg_enum::lbutton_up
		|| msg == win_msg_enum::rbutton_up || msg == win_msg_enum::mbutton_up; })
{

}

window::~window()
{
	owner(createdEvent).empty();
	owner(destroyedEvent).empty();
	owner(drawEvent).empty();
	owner(updateEvent).empty();
	owner(orientationEvent).empty();
	owner(activateEvent).empty();
	owner(sizeEvent).empty();
	owner(charEvent).empty();
	owner(keyPressedEvent).empty();
	owner(keyReleasedEvent).empty();
	owner(pointerMovedEvent).empty();
	owner(pointerPressedEvent).empty();
	owner(pointerReleasedEvent).empty();
	owner(mouseMovedEvent).empty();
	owner(mouseButtonPressedEvent).empty();
	owner(mouseButtonReleasedEvent).empty();
}

ANG_IMPLEMENT_CLASSNAME(ang::platform::windows::window);
ANG_IMPLEMENT_OBJECTNAME(ang::platform::windows::window);

bool window::is_child_of(type_name_t className)
{
	if (className == type_name<window>()
		|| object::is_child_of(className)
		|| className == type_name<icore_view>()
		|| className == type_name<imessage_reciever>())
		return true;
	return false;
}

bool window::is_kind_of(type_name_t className)const
{
	if (className == type_name<window>()
		|| object::is_kind_of(className)
		|| className == type_name<icore_view>()
		|| className == type_name<imessage_reciever>())
		return true;
	return false;
}

bool window::query_object(type_name_t className, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (className == type_name<window>())
	{
		*out = static_cast<window*>(this);
		return true;
	}
	else if (object::query_object(className, out))
	{
		return true;
	}
	if (className == type_name<icore_view>())
	{
		*out = static_cast<icore_view*>(this);
		return true;
	}
	if (className == type_name<imessage_reciever>())
	{
		*out = static_cast<imessage_reciever*>(this);
		return true;
	}
	return false;
}

pointer window::get_core_view_handle()const
{
	if (is_created())
	{
		auto handle = HWnd(_handle);
		return handle->hWnd;
	}

	return null;
}

icore_context_t window::get_core_context()const
{
	return new graphics::device_context(const_cast<window*>(this));
}

foundation::size<float> window::get_core_view_size()const
{
	return clip_size();
}

foundation::size<float> window::get_core_view_scale_factor()const
{
	return foundation::size<float>(1, 1);
}

bool window::is_created()const
{
	return (_handle != null);
}

bool window::create(wnd_create_args_t args)
{
	if (args.is_empty())
		return false;

	wnd_create_args_ex_t argsx = args->as<wnd_create_args_ex>();

	pre_create(args);

	HMODULE instance = GetModuleHandleW(NULL);
	WNDCLASSW wndclass = { 0 };
	class_regist_args_t class_arg = new class_regist_args("ang::windows::window"_s
		, class_style_t(class_style::own_dc + class_style::save_bits + class_style::hredraw + class_style::vredraw));

	wstring className = args->wnd_class_name().get();

	if (!className.is_empty() && className->length() > 0)
	{
		if (!GetClassInfoW(instance, (cwstr_t)className, &wndclass))
		{
			if (window::regist_wnd_class(class_arg).is_empty())
				return false;
			className = "ang::windows::window"_s;
		}
	}
	else
	{
		if (window::regist_wnd_class(class_arg).is_empty())
			return false;
		className = "ang::windows::window"_s;
	}

	HWnd handle = new _HWnd();

	handle->wnd = this;

	pointer parent = args->parent();
	handle->user_args = args->user_args();

	handle->hWnd = CreateWindowExW(argsx.is_empty() ? 0U : (uint)argsx->style_ex(), (cwstr_t)className
		, (cwstr_t)args->wnd_name(), (uint)(args->style() - wnd_style::shown)
		, (int)args->rect_area().left, (int)args->rect_area().top
		, (int)args->rect_area().width(), (int)args->rect_area().height()
		, (HWND)parent, null
		, instance, handle);

	if (handle->hWnd == null)
	{
		HRESULT hr = GetLastError();
		printf("Error:%d\n", hr);
		UnregisterClassW((cwstr_t)className, instance);
		delete handle;
		return false;
	}

	message_t msg = new message((core_msg_t)win_msg_enum::initial_update, 0, 0);
	send_msg(msg);

	attach(handle);

	if (args->style().is_active(wnd_style::shown))
		show(showing_flag::show);

	return true;
}

//dword_t window::release()
//{
//	return object::release();
//}

bool window::close()
{
	if (_handle)
	{
		auto handle = reinterpret_cast<HWnd>(_handle);
		detach();
		//HWND hwnd = handle->hWnd;
		//::SendMessageW(hwnd, WM_DESTROY, 0, 0);
		SetWindowLongPtrW(handle->hWnd, GWLP_USERDATA, NULL);
		handle->wnd = nullptr;
		delete handle;
	}
	return true;
}

void window::attach(pointer h)
{
	_handle = h;
}

void window::detach()
{
	_handle = null;
}

void window::pre_create(wnd_create_args_t args)
{

}

core::async::iasync_t<dword> window::post_msg(message_t msg)
{
	if (!is_created())
		return null;

	ANG_LOGI("%s: Posting Message", class_name());
	auto task = new async_msg_task(this, HWnd(_handle)->cond, HWnd(_handle)->mutex, msg);
	task->add_ref();
	if (PostMessageW((HWND)get_core_view_handle()
		, (core_msg_t)win_msg_enum::system_reserved_event, (WPARAM)task
		, (LPARAM)static_cast<imessage_reciever*>(this)) == 0)
	{
		task->release();
		return null;
	}
	return task;
}

imessage_reciever_t window::get_listener()const
{
	return const_cast<window*>(this);
}

bool window::listen_to(event_t event)
{
	switch ((core_msg_t)event.get()->msg_type())
	{
	case win_msg_enum::created:
		createdEvent += event;
		return true;
	case win_msg_enum::destroyed:
		destroyedEvent += event;
		return true;
	case win_msg_enum::draw:
		drawEvent += event;
		return true;
	case win_msg_enum::update:
		updateEvent += event;
		return true;
	case win_msg_enum::size:
		sizeEvent += event;
		return true;
	case win_msg_enum::pointer_moved:
		pointerMovedEvent += event;
		return true;
	case win_msg_enum::pointer_pressed:
		pointerPressedEvent += event;
		return true;
	case win_msg_enum::pointer_released:
		pointerReleasedEvent += event;
		return true;
	default:
		return false;
	}
}


void window::send_msg(message_t msg)
{
	if (!msg.is_empty())
		msg->result(::SendMessageW((HWND)get_core_view_handle(), (UINT)msg->msg(), (WPARAM)msg->arg1(), (LPARAM)msg->arg2()));
}

void window::show(showing_flag_t sf)
{
	ShowWindow((HWND)get_core_view_handle(), (int_t)sf);
}

void window::update_wnd()const
{
	UpdateWindow((HWND)get_core_view_handle());
}

window_t window::parent()
{
	return handle_to_window(GetParent((HWND)get_core_view_handle()));
}

void window::parent(window_t wnd)
{
	if ((wnd == null) || (wnd->get_core_view_handle() == null))
		SetParent((HWND)get_core_view_handle(), NULL);

	SetParent((HWND)get_core_view_handle(), (HWND)wnd->get_core_view_handle());
}

void window::parent_from_handle(pointer wnd)
{
	if ((wnd == null))
		SetParent((HWND)get_core_view_handle(), NULL);
	SetParent((HWND)get_core_view_handle(), (HWND)wnd);
}

void window::screen_to_client(foundation::point<float>& p)
{
	POINT ptn = { LONG(p.x), LONG(p.x) };
	::ScreenToClient((HWND)get_core_view_handle(), &ptn);
	p.x = (float)ptn.x;
	p.y = (float)ptn.y;
}

void window::client_to_screen(foundation::point<float>&p)
{
	POINT ptn = { LONG(p.x), LONG(p.x) };
	::ClientToScreen((HWND)get_core_view_handle(), &ptn);
	p.x = (float)ptn.x;
	p.y = (float)ptn.y;
}

void window::screen_to_client(foundation::rect<float>&r)
{
	POINT ptn = { LONG(r.left), LONG(r.top) };
	::ScreenToClient((HWND)get_core_view_handle(), &ptn);

	r.move_to((float)ptn.x, (float)ptn.y);
}

void window::client_to_screen(foundation::rect<float>&r)
{
	POINT ptn = { LONG(r.left), LONG(r.top) };
	::ClientToScreen((HWND)get_core_view_handle(), &ptn);
	r.move_to((float)ptn.x, (float)ptn.y);
}

foundation::point<float> window::position()
{
	RECT rw;
	GetWindowRect((HWND)get_core_view_handle(), &rw);
	foundation::point<float> p((float)rw.left, (float)rw.top);
	window* wnd = parent();
	if (wnd != null)
		wnd->screen_to_client(p);
	return p;
}

foundation::size<float> window::wnd_size()
{
	RECT rw;
	GetWindowRect((HWND)get_core_view_handle(), &rw);
	foundation::size<float> s((float)rw.right - (float)rw.left, (float)rw.bottom - (float)rw.top);
	return s;
}

foundation::size<float> window::clip_size()const
{
	RECT rw;
	GetClientRect((HWND)get_core_view_handle(), &rw);
	foundation::size<float> s((float)rw.right - (float)rw.left, (float)rw.bottom - (float)rw.top);
	return s;
}

void window::position(foundation::point<float> p)
{
	//foundation::size<float> s = wnd_size();
	//MoveWindow((HWND)get_core_view_handle(), (int)p.x, (int)p.y, (int)s.width, (int)s.height, true);
	auto handle = (HWND)get_core_view_handle();
	if (::IsWindow(handle))
		::SetWindowPos(handle, NULL, (int)p.x, (int)p.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
}

void window::size(foundation::size<float> s)
{
	//foundation::point<float> p = position();
	//MoveWindow((HWND)get_core_view_handle(), (int)p.x, (int)p.y, (int)s.width, (int)s.height, true);
	auto handle = (HWND)get_core_view_handle();
	if(::IsWindow(handle))
		::SetWindowPos(handle, NULL, 0, 0, (int)s.width, (int)s.height, SWP_NOACTIVATE | SWP_NOMOVE);
}

void window::redraw()
{
	InvalidateRect((HWND)get_core_view_handle(), NULL, FALSE);
}

//Message handler

LRESULT WINAPI window_procedure(HWND hWnd, UINT m, WPARAM wprm, LPARAM lprm)
{
	message_t msg = new message(m, (pointer)wprm, (pointer)lprm);
	window_t wnd = null;

	if (((core_msg_t)win_msg_enum::created == m) && (lprm != 0))
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lprm;
		auto handle = (HWnd)pcs->lpCreateParams;
		if (handle)
		{
			handle->hWnd = (HWND)hWnd;
			wnd = handle->wnd;
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
		return msg->result();
	}
	return DefWindowProcW((HWND)hWnd, m, wprm, lprm);
}

void window::window_proc(message_t msg)
{
	switch ((win_msg_enum)msg->msg())
	{
	case win_msg_enum::created: {
		//Autoreference, Prevent Autodrestroy

		attach(LPCREATESTRUCT(msg->arg2())->lpCreateParams);
		msg->result(on_created(msg));
	}break;

	case win_msg_enum::initial_update: {
		initial_update();
	} break;

	case win_msg_enum::destroyed: {
		on_destroyed(msg);
	} break;

	case win_msg_enum::draw: {
		on_paint(msg);
	} break;

	case win_msg_enum::update: {
		on_update(msg);
	} break;

	case win_msg_enum::size: {
		on_size_change(msg);
	} break;

		//case win_msg_enum::DisplayChange: {
		//	if (reinterpret_cast<HWnd>(GetHandle())->currentOrientation != SystemInfo::CurrentScreenOrientation()){
		//		reinterpret_cast<HWnd>(GetHandle())->currentOrientation = SystemInfo::CurrentScreenOrientation();
		//		OnOrientationChange(msg);	
		//	}	
		//} break;

	case win_msg_enum::got_focus:
	case win_msg_enum::lost_focus: {
		on_activate(msg);
	} break;

	case win_msg_enum::pointer_moved: {
		on_pointer_moved(msg);
	} break;

	case win_msg_enum::pointer_pressed: {
		on_pointer_pressed(msg);
	} break;

	case win_msg_enum::pointer_released: {
		on_pointer_released(msg);
	} break;

	case win_msg_enum::mouse_move: {
		on_mouse_moved(msg);
	} break;

	case win_msg_enum::lbutton_down:
	case win_msg_enum::rbutton_down:
	case win_msg_enum::xbutton_down: {
		on_mouse_button_pressed(msg);
	} break;

	case win_msg_enum::lbutton_up:
	case win_msg_enum::rbutton_up:
	case win_msg_enum::xbutton_up: {
		on_mouse_button_released(msg);
	} break;

	case win_msg_enum::key_down: {
		on_key_pressed(msg);
		break;
	}
	case win_msg_enum::put_char: {
		on_char(msg);
		break;
	}
	case win_msg_enum::key_up: {
		on_key_released(msg);
		break;
	}
	default: {
		def_window_proc(msg);
	} break;
	}
}

void window::def_window_proc(message_t msg)
{
	msg->result(DefWindowProcW((HWND)get_core_view_handle(), msg->msg(), (WPARAM)msg->arg1(), (LPARAM)msg->arg2()));
}

void window::initial_update()
{
	return;
}

events::event_trigger<window>& window::owner(events::event_listener& listener)
{
	return static_cast<events::event_trigger<window>&>(listener);
}

events::event_trigger<window>const& window::trigger(events::event_listener const& listener)const
{
	return static_cast<events::event_trigger<window>const&>(listener);
}

dword window::on_created(message_t m)
{
	LPCREATESTRUCT pcs = (LPCREATESTRUCT)m->arg2();
	auto hwnd = (HWnd)pcs->lpCreateParams;
	message_t msg = new message(m->msg(), this, hwnd->user_args.get());
	icreated_event_args_t args = new created_event_args(msg);
	if (app::current_app()->main_wnd().get() == this) {
		try { trigger(app::current_app()->mainWndCreatedEvent)(args.get()); }
		catch (const exception_t& e) { ANG_LOGE("%s::on_created: %s", class_name(), (cstr_t)e->what()); }
	}
	try { trigger(createdEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_created: %s", class_name(), (cstr_t)e->what()); }
	if (msg->result() == 0)
		def_window_proc(m);
	return msg->result();
}

dword window::on_destroyed(message_t m)
{
	imsg_event_args_t args = new msg_event_args(m);
	app_t app = app::current_app();

	if (!app.is_empty() && app->main_wnd().get() == this) {
		try { trigger(app->mainWndDestroyedEvent)(args.get()); }
		catch (const exception_t& e) { ANG_LOGE("%s::on_destroyed: %s", class_name(), (cstr_t)e->what()); }
	}
	try { trigger(destroyedEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_destroyed: %s", class_name(), (cstr_t)e->what()); }
	def_window_proc(m);
	close();

	if (!app.is_empty() && app->main_wnd().get() == this)
		PostQuitMessage(-1); //app->post_msg(new message((core_msg_t)win_msg_enum::Quit));

	return m->result();
}

dword window::on_paint(message_t m)
{
	graphics::device_context_t dc = new graphics::paint_dc(this);
	if (dc->get_HDC() == null)
		dc = new graphics::device_context(this);
	message_t msg = new message(m->msg(), this, dc.get());
	idraw_event_args_t args = new draw_event_args(msg);
	try { trigger(drawEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_paint: %s", class_name(), (cstr_t)e->what()); }
	m->result(msg->result());
	return msg->result();
}

dword window::on_update(message_t m)
{
	iupdate_event_args_t args = new update_event_args(m);
	try { trigger(updateEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_update: %s", class_name(), (cstr_t)e->what()); }
	return m->result();
}

dword window::on_activate(message_t m)
{
	activate_status_t status = m->msg() == (core_msg_t)win_msg_enum::got_focus ? activate_status::activated : activate_status::deactivated;
	message_t msg = new message(m->msg(), &status);
	msg->result(-1);
	iactivate_event_args_t args = new activate_event_args(msg);
	try { trigger(activateEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_activate: %s", class_name(), (cstr_t)e->what()); }
	if (msg->result() != 0)
		def_window_proc(m);
	return m->result();
}


dword window::on_orientation_change(message_t m)
{
	/*struct {
	DisplayInfoEventArgs::InvalidateReasonState state;
	Display::DisplayInfo* info;
	}args2;

	Display::DisplayInfo info = {
	SystemInfo::CurrentScreenOrientation(),
	SystemInfo::NativeScreenOrientation(),
	GetCoreViewSize(),GetCoreViewScaleFactor(), 0 };
	args2.info = &info;
	args2.state = DisplayInfoEventArgs::InvalidateReasonState::OrientationChanged;
	Message msg(m.Msg(), this, &args2);
	msg.Result() = -1;
	DisplayInfoEventArgs args(msg);
	try { trigger(oritntationEvent)(&args); }
	catch (const Exception& e) { ANG_LOGE("%s::OnOrientationChange: %s", ClassName(), e.What().Data()); }
	if (msg.Result() != 0)
	DefWndProc(m);
	return m.Result();*/
	return 0;
}

dword window::on_size_change(message_t m)
{
	struct display_info_event_args2 {
		display_invalidate_reason_t state;
		display::display_info info;
	};

	display_info_event_args2 args2 = {
		display_invalidate_reason::size_changed,
		{
			system_info::current_screen_orientation(),
			system_info::current_screen_orientation(),
			foundation::size<float>((float)LOWORD(m->arg2()), (float)HIWORD(m->arg2())),
			get_core_view_scale_factor(),
			96
		}
	};

	message_t msg = new message(m->msg(), this, &args2);
	msg->result(-1);
	idisplay_info_event_args_t args = new display_info_event_args(msg);
	try { trigger(sizeEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_size_change: %s", class_name(), (cstr_t)e->what()); }
	if (msg->result() != 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_pointer_moved(message_t m)
{
	WPARAM wprm = (WPARAM)m->arg1();

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

	input::poiner_info_t info = {
		foundation::point<float>((float)GET_X_LPARAM(m->arg2()), (float)GET_Y_LPARAM(m->arg2())),
		id,
		is_pa,
		is_sa,
		type,
		modifiers,
	};

	ipointer_event_args_t args = new pointer_event_args(new message(m->msg(), &info));
	uint count = 0;
	try { count = trigger(pointerMovedEvent)(args.get()); m->result(0); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_pointer_moved: %s", class_name(), (cstr_t)e->what()); }
	if (count == 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_pointer_pressed(message_t m)
{
	WPARAM wprm = (WPARAM)m->arg1();

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

	input::poiner_info_t info = {
		foundation::point<float>((float)GET_X_LPARAM(m->arg2()), (float)GET_Y_LPARAM(m->arg2())),
		id,
		is_pa,
		is_sa,
		type,
		modifiers,
	};

	ipointer_event_args_t args = new pointer_event_args(new message(m->msg(), &info));
	uint count = 0;
	try { count = trigger(pointerPressedEvent)(args.get()); m->result(0); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_pointer_pressed: %s", class_name(), (cstr_t)e->what()); }
	if (count == 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_pointer_released(message_t m)
{
	WPARAM wprm = (WPARAM)m->arg1();

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

	input::poiner_info_t info = {
		foundation::point<float>((float)GET_X_LPARAM(m->arg2()), (float)GET_Y_LPARAM(m->arg2())),
		id,
		is_pa,
		is_sa,
		type,
		modifiers,
	};

	ipointer_event_args_t args = new pointer_event_args(new message(m->msg(), &info));
	uint count = 0;
	try { count = trigger(pointerReleasedEvent)(args.get()); m->result(0); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_pointer_released: %s", class_name(), (cstr_t)e->what()); }
	if (count == 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_mouse_moved(message_t m)
{
	WPARAM wprm = (WPARAM)m->arg1();

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

	input::poiner_info_t info = {
		foundation::point<float>((float)GET_X_LPARAM(m->arg2()), (float)GET_Y_LPARAM(m->arg2())),
		id,
		is_pa,
		is_sa,
		type,
		modifiers,
	};

	ipointer_event_args_t args = new pointer_event_args(new message(m->msg(), &info));
	uint count = 0;
	try { count = trigger(mouseMovedEvent)(args.get()); m->result(0); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_mouse_moved: %s", class_name(), (cstr_t)e->what()); }
	if (count == 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_mouse_button_pressed(message_t m)
{
	WPARAM wprm = (WPARAM)m->arg1();

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

	input::poiner_info_t info = {
		foundation::point<float>((float)GET_X_LPARAM(m->arg2()), (float)GET_Y_LPARAM(m->arg2())),
		id,
		is_pa,
		is_sa,
		type,
		modifiers,
	};

	ipointer_event_args_t args = new pointer_event_args(new message(m->msg(), &info));
	uint count = 0;
	try { count = trigger(mouseButtonPressedEvent)(args.get()); m->result(0); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_mouse_button_pressed: %s", class_name(), (cstr_t)e->what()); }
	if (count == 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_mouse_button_released(message_t m)
{
	WPARAM wprm = (WPARAM)m->arg1();

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

	input::poiner_info_t info = {
		foundation::point<float>((float)GET_X_LPARAM(m->arg2()), (float)GET_Y_LPARAM(m->arg2())),
		id,
		is_pa,
		is_sa,
		type,
		modifiers,
	};

	ipointer_event_args_t args = new pointer_event_args(new message(m->msg(), &info));
	uint count = 0;
	try { count = trigger(mouseButtonReleasedEvent)(args.get()); m->result(0); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_mouse_button_released: %s", class_name(), (cstr_t)e->what()); }
	if (count == 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_key_pressed(message_t m)
{
	ikey_event_args_t args = new key_event_args(m);
	m->result(-1);
	try { trigger(keyPressedEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_key_pressed: %s", class_name(), (cstr_t)e->what()); }
	if (m->result() != 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_char(message_t m)
{
	ikey_event_args_t args = new key_event_args(m);
	m->result(-1);
	try { trigger(charEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_char: %s", class_name(), (cstr_t)e->what()); }
	if (m->result() != 0)
		def_window_proc(m);
	return m->result();
}

dword window::on_key_released(message_t m)
{
	ikey_event_args_t args = new key_event_args(m);
	m->result(-1);
	try { trigger(keyReleasedEvent)(args.get()); }
	catch (const exception_t& e) { ANG_LOGE("%s::on_key_released: %s", class_name(), (cstr_t)e->what()); }
	if (m->result() != 0)
		def_window_proc(m);
	return m->result();
}