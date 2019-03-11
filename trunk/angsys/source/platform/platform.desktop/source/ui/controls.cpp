#include "pch.h"
#include <ang/platform/win32/windows.h>
#include "wnd_handle.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;
using namespace ang::platform::windows::ui;

extern "C" LRESULT WINAPI window_procedure(HWND hWnd, UINT m, WPARAM wprm, LPARAM lprm);

control::control()
{
}

control::~control()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::ui::control);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::ui::control, window);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::ui::control, window);

bool control::get_focus_property(base_property<bool>const* prop)
{
	HWND hwnd = GetFocus();
	return hwnd != null && hwnd == field_to_parent(&control::focus, prop)->core_view_handle();
}

void control::set_focus_property(base_property<bool>* prop, bool&& value)
{
	SetFocus((HWND)field_to_parent(&control::focus, prop)->core_view_handle());
}

bool control::create(wnd_create_args_t args)
{
	if (args.is_empty())
		return false;

	wnd_create_args_ex_t argsx = args->as<wnd_create_args_ex>();

	wstring class_name = "ang::windows::ui::control"_s;

	HMODULE instance = GetModuleHandleW(NULL);
	WNDCLASSW wndclass = { 0 };
	class_regist_args_t class_arg = new class_regist_args(class_name.get()
		, class_style_t(class_style::parent_dc + class_style::save_bits + class_style::hredraw + class_style::vredraw));

	if (!GetClassInfoW(instance, (cwstr_t)class_name, &wndclass))
	{
		if (window::regist_wnd_class(class_arg).is_empty())
			return false;
	}

	hwnd h = new _hwnd();

	h->m_wnd = this;

	pointer parent = args->parent;
	h->m_user_args = (array_view<const var>)args->user_args;

	auto style = args->style - wnd_style::popup + wnd_style::child;
	dword st = style;
	h->m_hwnd = CreateWindowExW(argsx.is_empty() ? 0U : (dword)argsx->style_ex, (cwstr_t)class_name
		, (cwstr_t)args->wnd_name, style
		, (int)args->rect_area->left, (int)args->rect_area->top
		, (int)args->rect_area->width, (int)args->rect_area->height
		, (HWND)parent, (HMENU)args->id
		, instance, h);

	if (h->m_hwnd == null)
	{
		HRESULT hr = GetLastError();
		printf("Error:%d\n", hr);
		UnregisterClassW((cwstr_t)class_name, instance);
		delete h;
		return false;
	}

	send_msg(events::message((events::core_msg)events::win_msg::initial_update));
	//attach(h);
	if (args->style & wnd_style::shown)
		show(showing_flag::show);

	return true;
}

void control::window_proc(events::message& msg)
{
	switch ((events::win_msg)msg.msg().get())
	{
	case events::win_msg::initial_update: {
		handle->m_dispatcher = new windows::dispatcher(
			bind(this, &control::listen_to),
			bind(this, &control::send_msg),
			bind(this, &control::post_task));
		msg.result(on_initialize(msg));
	}break;
	case events::win_msg::child_notify:
		msg.result(on_child_notify(msg));
		break;
	default:
		return def_window_proc(msg);
	}
}

///////////////////////////////////////////////////////////////////////////////////////

text_box::text_box()
	: m_has_focus(false)
{
}

text_box::~text_box()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::ui::text_box);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::ui::text_box, control);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::ui::text_box, control);

listen_token<void(objptr, wstring)> text_box::add_text_changed_event_handler(base_event* prop, function<void(objptr, wstring)> e)
{
	return field_to_parent(&text_box::text_changed_event, prop)->m_text_changed_event += e;
}

bool text_box::remove_text_changed_event_handler(base_event* prop, listen_token<void(objptr, wstring)> token)
{
	return field_to_parent(&text_box::text_changed_event, prop)->m_text_changed_event -= token;
}

wstring text_box::get_text_property(base_property<wstring>const* prop)
{
	wchar buffer[MAX_PATH];
	HWND wnd = (HWND)field_to_parent(&text_box::text, prop)->core_view_handle();
	GetWindowTextW(wnd, buffer, MAX_PATH);
	return cwstr_t(buffer, -1);
}

void text_box::set_text_property(base_property<wstring>* prop, ang::text::raw_cstr_t value)
{
	wstring str = value;
	HWND wnd = (HWND)field_to_parent(&text_box::text, prop)->core_view_handle();
	SetWindowTextW(wnd, str->cstr());
}

bool text_box::create(wnd_create_args_t args)
{
	if (args.is_empty())
		return false;

	HMODULE instance = GetModuleHandleW(NULL);
	WNDCLASSW wndclass = { 0 };

	wnd_create_args_ex_t argsx = args->as<wnd_create_args_ex>();
	pointer parent = args->parent;

	hwnd h = new _hwnd();
	h->m_wnd = this;
	
	h->m_user_args = (array_view<const var>)args->user_args;

	auto style = args->style - wnd_style::popup + wnd_style::child;
	h->m_hwnd = CreateWindowExW(argsx.is_empty() ? 0U : (dword)argsx->style_ex, L"Edit"
		, (cwstr_t)args->wnd_name, style
		, (int)args->rect_area->left, (int)args->rect_area->top
		, (int)args->rect_area->width, (int)args->rect_area->height
		, (HWND)parent, (HMENU)args->id
		, instance, h);

	if (h->m_hwnd == null)
	{
		HRESULT hr = GetLastError();
		printf("Error:%d\n", hr);
		delete h;
		return false;
	}

	attach(h);
	SetWindowLongPtrW((HWND)h->m_hwnd, GWLP_USERDATA, (LONG_PTR)h);
	SetClassLongPtrW((HWND)h->m_hwnd, GCL_WNDPROC, (LONG_PTR)&window_procedure);

	send_msg(events::message((events::core_msg)events::win_msg::initial_update));

	return true;
}

dword text_box::on_child_notify(events::message& msg)
{
	wchar buffer[MAX_PATH];

	switch ((uint)msg.wparam())
	{
	case EN_CHANGE:
		GetWindowTextW(handle->m_hwnd, buffer, MAX_PATH);
		m_text_changed_event(this, cwstr_t(buffer, -1));
		return TRUE;
	case EN_SETFOCUS:
		m_has_focus = true;
		return TRUE;
	case EN_KILLFOCUS:
		m_has_focus = false;
		return TRUE;
	default:
		break;
	}
	return FALSE;
}