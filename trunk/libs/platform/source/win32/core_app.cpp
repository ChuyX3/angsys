#include "pch.h"
#include <ang/platform/platform.h>
#include <ang/core/time.h>

#include "dispatcher.h"
#include "core_app.h"
#include <comdef.h>
#include <windowsx.h>

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;

namespace ang::platform {
	extern icore_app* s_current_app;
}

LRESULT STDCALL core_app::wndproc(HWND hwnd, UINT m, WPARAM wprm, LPARAM lprm)
{
	message msg((core_msg)m, wprm, lprm);
	core_app_t wnd = null;

	if ((core_msg::created == (core_msg)m) && (lprm != 0))
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lprm;
		if (pcs->lpCreateParams)
		{
			wnd = (core_app*)pcs->lpCreateParams;
			wnd->m_hwnd = hwnd;
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)pcs->lpCreateParams);
		}
	}
	else
	{
		wnd = reinterpret_cast<core_app*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	}

	if (!wnd.is_empty())
	{
		wnd->wndproc(msg);
		return msg.result();
	}
	return DefWindowProcW(hwnd, m, wprm, lprm);
}

core_app::core_app()
	: m_hint(null)
	, m_hwnd(null)
{
	s_current_app = this;
	m_thread = core::async::thread::this_thread();
	m_timer.reset();
	m_controllers = new input::controller_manager();
}

core_app::~core_app()
{
	m_controllers = null;
	s_current_app = null;
}

pointer core_app::core_app_handle()const
{
	return m_hint;
}

icore_view_t core_app::core_view()
{
	return this;
}

input::ikeyboard_t core_app::keyboard()
{
	return null;
}

ivar core_app::property(astring name)const
{
	return null;
}

void core_app::property(astring name, ivar var)
{

}

pointer core_app::core_view_handle()const
{
	return m_hwnd;
}

graphics::icore_context_t core_app::core_context()const
{
	return new graphics::device_context(const_cast<core_app*>(this));
}

graphics::size<float> core_app::core_view_size()const
{
	if (IsWindow(m_hwnd)) {
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		return { float(rc.right - rc.left), float(rc.bottom - rc.top) };
	}
	return{ 0,0 };
}

graphics::size<float> core_app::core_view_scale_factor()const
{
	return{ 1.0f,1.0f };
}

imessage_listener_t core_app::dispatcher()const
{
	return const_cast<core_app*>(this);
}

error core_app::run(function<error(icore_app_t)> setup, app_args_t& args)
{
	//m_frm = frm;
	m_thread = core::async::thread::this_thread();
	error err = setup(this);

	wstring name = args.name->cstr();
	wstring title = args.title->cstr();

	if (err.code() != error_code::success)
		return err;
	setup = null;//releasing scope

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_SAVEBITS;
	wcex.lpfnWndProc = &core_app::wndproc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hint;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = name.cstr();
	wcex.hIconSm = NULL;
	RegisterClassExW(&wcex);;

	HWND hwnd = CreateWindowExW(0, name.cstr(), title.cstr(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hint, this);

	if (!hwnd)
	{
		_com_error err(GetLastError());
		castr_t msg(err.ErrorMessage(), -1);
		return error(err.Error(), msg, error_code::system_error);
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	core::time::step_timer timer;
	if (args.fps > 0)
	{
		timer.frames_per_second(args.fps);
		timer.fixed_time_step(true);
	}
	
	// Main message loop:
	MSG msg;
	while (true)
	{
		core::async::async_action_status_t status = m_thread->status();

		if (status & core::async::async_action_status::canceled)
			PostQuitMessage(0);

		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);

			/*if (msg.hwnd == NULL)
			{
				events::message m{ (events::core_msg)msg.message, msg.wParam, msg.lParam };
				wndproc(m);
			}*/
		}

		if (msg.message == WM_QUIT)
			break;
		else timer.tick([&]() {
			m_controllers->update(timer.elapsed_time());
			SendMessageW(m_hwnd, (UINT)core_msg::update, timer.elapsed_time(), timer.total_time());
		});
	}

	return error_code::success;
}

void core_app::wndproc(events::message& msg)
{
	switch (msg.msg())
	{
	case core_msg::created: {
		//auto cs = LPCREATESTRUCT((LPARAM)msg.lparam());
		on_created(msg);
	}break;
	case core_msg::destroyed: {
		on_destroyed(msg);
	} break;

	case (core_msg)WM_ERASEBKGND:
	case core_msg::draw: {
		on_draw(msg);
	} break;

	case core_msg::update: {
		on_update(msg);
	} break;

	case core_msg::display_change:
	case core_msg::orientation:
	case core_msg::size: {
		on_display_event(msg);
	} break;

	case core_msg::system_reserved_event: {
		on_task_command(msg);
	}

	case core_msg::got_focus:
	case core_msg::lost_focus: {
		on_activate(msg);
	} break;

	case core_msg::pointer_entered:
	case core_msg::pointer_pressed:
	case core_msg::pointer_moved:
	case core_msg::pointer_released:
	case core_msg::pointer_leaved: {
		on_pointer_event(msg);
	} break;

	case core_msg::mouse_move:
	case core_msg::lbutton_down:
	case core_msg::rbutton_down:
	case (core_msg)WM_XBUTTONDOWN:
	case core_msg::lbutton_up:
	case core_msg::rbutton_up:
	case (core_msg)WM_XBUTTONUP: {
		on_mouse_event(msg);
	} break;

	case core_msg::key_down:
	case core_msg::put_char:
	case core_msg::key_up: {
		on_key_event(msg);
		break;
	}
	default: {
		def_wndproc(msg);
	} break;
	}
	
}

void core_app::def_wndproc(events::message& msg)
{
	msg.result(DefWindowProcW(m_hwnd, (uint)(core_msg)msg.msg(), (WPARAM)msg.wparam(), (LPARAM)msg.lparam()));
}

dword core_app::on_created(events::message& m)
{
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {
		icreated_event_args_t args = new created_event_args(m, this, null);
		try {
			it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	
	def_wndproc(m);
	return m.result();
}

dword core_app::on_destroyed(events::message& m)
{
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {
		imsg_event_args_t args = new msg_event_args(m);
		try {
			it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	m_event_listeners.clear();
	def_wndproc(m);
	PostQuitMessage(m.lparam());
	return m.result();
}

dword core_app::on_draw(events::message& m)
{
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {
		graphics::device_context_t dc = new graphics::paint_dc(this);
		if (dc->get_HDC() == null)
			dc = new graphics::device_context(this);
		idraw_event_args_t args = new draw_event_args(m, this, dc, core_view_size());
		try {
			it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	m.result(0);
	return m.result();
}

dword core_app::on_update(events::message& m)
{
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {
		imsg_event_args_t args = new msg_event_args(m);
		try {
			it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	m.result(0);
	return m.result();
}

dword core_app::on_activate(events::message& m)
{
	int handled = 0;
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {		
		activate_status_t status = m.msg() == core_msg::got_focus ? activate_status::activated : activate_status::deactivated;
		iactivate_event_args_t args = new activate_event_args(m, status);
		try {
			handled = it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	if (!handled) def_wndproc(m);
	else m.result(0);
	return m.result();
}

dword core_app::on_display_event(events::message& m)
{
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {
		dword value = (dword)m.lparam();

		display_invalidate_reason_t reason
			= m.msg() == core_msg::size ? display_invalidate_reason::size_changed
			: m.msg() == core_msg::display_change ? display_invalidate_reason::display_invalidate
			: m.msg() == core_msg::orientation ? display_invalidate_reason::orientation_changed
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
			it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	def_wndproc(m);
	return m.result();
}

dword core_app::on_pointer_event(events::message& m)
{
	int handled = 0;
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {
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
			handled = it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	if (!handled) def_wndproc(m);
	else m.result(0);
	return m.result();

}

dword core_app::on_mouse_event(events::message& m)
{
	int handled = 0;
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {
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
			handled = it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	if (!handled) def_wndproc(m);
	else m.result(0);
	return m.result();
}

dword core_app::on_key_event(events::message& m)
{
	auto it = m_event_listeners.find(m.msg());
	if (it.is_valid()) {
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
			m.msg() == core_msg::key_down ? input::key_state::pressed
			: m.msg() == core_msg::put_char ? input::key_state::pressed
			: input::key_state::released,
			(input::key_modifiers)modifiers //property<const key_modifiers> modifiers;
			});
		m.result(-1);
		try {
			it->value->invoke(args.get());
		}
		catch (const exception & e) {
		}
	}
	def_wndproc(m);
	return m.result();
}

dword core_app::on_task_command(events::message& m)
{
	auto task = reinterpret_cast<async_task*>(m.lparam());
	task->execute();
	task->release();
	m.result(0);
	return 0;
}


