#include "pch.h"
#include <ang/platform/uwp/windows.h>
#include <Unknwn.h>
#include "dispatcher.h"
#include "event_args.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::events;
using namespace ang::platform::windows;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

struct core_window::_hwnd
{
	bool m_is_visible = false;
	graphics::size<float> m_clip_size = { 1,1 };
	graphics::size<float> m_scale_factor = { 1,1 };
	CoreWindow^ m_wnd = null;
	core::async::mutex_t m_mutex;
	collections::list<events::message> m_msg_queue;
	dispatcher_t m_dispatcher = null;
};


core_window::core_window()
	: m_handle(null)
	, m_created_event(this, [](core_msg_t msg) { return msg == core_msg::created; })
	, m_initialize_event(this, [](core_msg_t msg) { return msg == core_msg::initial_update; })
	, m_finalize_event(this, [](core_msg_t msg) { return msg == core_msg::close; })
	, m_destroyed_event(this, [](core_msg_t msg) { return msg == core_msg::destroyed; })
	, m_draw_event(this, [](core_msg_t msg) { return msg == core_msg::draw; })
	, m_update_event(this, [](core_msg_t msg) { return msg == core_msg::update; })
	, m_orientation_event(this, [](core_msg_t msg) { return msg == core_msg::orientation; })
	, m_activate_event(this, [](core_msg_t msg) { return msg == core_msg::got_focus || msg == core_msg::lost_focus; })
	, m_size_event(this, [](core_msg_t msg) { return msg == core_msg::size; })
	, m_display_change_event(this, [](core_msg_t msg) { return msg == core_msg::display_change; })
	, m_put_char_event(this, [](core_msg_t msg) { return msg == core_msg::put_char; })
	, m_key_pressed_event(this, [](core_msg_t msg) { return msg == core_msg::key_down; })
	, m_key_released_event(this, [](core_msg_t msg) { return msg == core_msg::key_up; })
	, m_pointer_entered_event(this, [](core_msg_t msg) { return msg == core_msg::pointer_entered; })
	, m_pointer_leaved_event(this, [](core_msg_t msg) { return msg == core_msg::pointer_leaved; })
	, m_pointer_moved_event(this, [](core_msg_t msg) { return msg == core_msg::pointer_moved; })
	, m_pointer_pressed_event(this, [](core_msg_t msg) { return msg == core_msg::pointer_pressed; })
	, m_pointer_released_event(this, [](core_msg_t msg) { return msg == core_msg::pointer_pressed; })
{
	m_handle = new _hwnd();
	m_handle->m_dispatcher = new windows::dispatcher(
		bind(this, &core_window::listen_to),
		bind(this, &core_window::send_msg),
		bind(this, &core_window::post_task));
}

core_window::~core_window()
{
	//m_created_event.empty();
	m_initialize_event.empty();
	m_destroyed_event.empty();
	m_draw_event.empty();
	m_update_event.empty();
	m_orientation_event.empty();
	m_activate_event.empty();
	m_size_event.empty();
	m_key_pressed_event.empty();
	m_key_released_event.empty();
	m_pointer_moved_event.empty();
	m_pointer_pressed_event.empty();
	m_pointer_released_event.empty();
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::core_window);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::core_window, object, icore_view);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::core_window, object, icore_view);


ang_platform_implement_event_handler(core_window, created_event);
ang_platform_implement_event_handler(core_window, initialize_event);
ang_platform_implement_event_handler(core_window, finalize_event);
ang_platform_implement_event_handler(core_window, destroyed_event);

ang_platform_implement_event_handler(core_window, draw_event);
ang_platform_implement_event_handler(core_window, update_event);

ang_platform_implement_event_handler(core_window, orientation_event);
ang_platform_implement_event_handler(core_window, activate_event);
ang_platform_implement_event_handler(core_window, size_event);
ang_platform_implement_event_handler(core_window, display_change_event);

ang_platform_implement_event_handler(core_window, key_pressed_event);
ang_platform_implement_event_handler(core_window, key_released_event);
ang_platform_implement_event_handler(core_window, put_char_event);

ang_platform_implement_event_handler(core_window, pointer_entered_event);
ang_platform_implement_event_handler(core_window, pointer_pressed_event);
ang_platform_implement_event_handler(core_window, pointer_moved_event);
ang_platform_implement_event_handler(core_window, pointer_released_event);
ang_platform_implement_event_handler(core_window, pointer_leaved_event);


graphics::point<float> core_window::get_position_property(base_property<graphics::point<float>> const* prop)
{
	return  graphics::point<float>(0, 0);
}

graphics::size<float> core_window::get_clip_size_property(base_property<graphics::size<float>> const* prop)
{
	return  field_to_parent(&core_window::clip_size, prop)->handle->m_clip_size;
}

graphics::size<float> core_window::get_scale_factor_property(base_property<graphics::size<float>> const* prop)
{
	return  field_to_parent(&core_window::scale_factor, prop)->handle->m_scale_factor;
}

//void core_window::set_scale_factor_property(base_property<graphics::size<float>>* prop, graphics::size<float>&& value)
//{
//	field_to_parent(&core_window::scale_factor, prop)->handle->m_scale_factor = value;
//	//field_to_parent(&core_window::scale_factor, prop)->dispatcher()->post_msg(events::message(events::core_msg::display_change, (uint)display_invalidate_reason::scale_factor_changed));
//}

core_window::hwnd_t core_window::get_handle_property(base_property<hwnd_t> const* prop)
{
	return field_to_parent(&core_window::handle, prop)->m_handle;
}

bool core_window::get_is_created_property(base_property<bool> const* prop)
{
	return field_to_parent(&core_window::is_created, prop)->handle->m_wnd != null;
}

bool core_window::get_is_visible_property(base_property<bool> const* prop)
{
	return field_to_parent(&core_window::is_visible, prop)->handle->m_is_visible;
}

void core_window::clear()
{
	destroy();
	if (handle)
	{
		m_handle->m_dispatcher = null;
		delete m_handle;
		m_handle = null;
	}
}

pointer core_window::core_view_handle()const
{
	return reinterpret_cast<IUnknown*>(handle->m_wnd);
}

graphics::icore_context_t core_window::core_context()const
{
	return null;
}

graphics::size<float> core_window::core_view_size()const
{
	return clip_size;
}

graphics::size<float> core_window::core_view_scale_factor()const
{
	return scale_factor;
}

imessage_listener_t core_window::dispatcher()const
{
	return handle->m_dispatcher.get();
}

void core_window::create(pointer ptr)
{
	handle->m_wnd = reinterpret_cast<CoreWindow^>(ptr);

	handle->m_wnd->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(
			[=](CoreWindow^sender, WindowSizeChangedEventArgs^args)
	{
		send_msg(events::message(events::core_msg::size, (uint)display_invalidate_reason::size_changed));
	});

	handle->m_wnd->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(
			[=](CoreWindow^sender, VisibilityChangedEventArgs^args)
	{
		send_msg(events::message(events::core_msg::visibility_change, args->Visible));
	});

	handle->m_wnd->Closed +=
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(
			[=](CoreWindow^sender, CoreWindowEventArgs^args)
	{
		send_msg(events::message(events::core_msg::close));
	});

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(
			[=](DisplayInformation^, Platform::Object^)
	{
		send_msg(events::message(events::core_msg::display_change, (uint)display_invalidate_reason::dpi_change));
	});

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(
			[=](DisplayInformation^, Platform::Object^)
	{
		send_msg(events::message(events::core_msg::orientation, (uint)display_invalidate_reason::orientation_changed));
	});

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(
			[=](DisplayInformation^, Platform::Object^)
	{
		send_msg(events::message(events::core_msg::display_change, (uint)display_invalidate_reason::display_invalidate));
	});

	handle->m_wnd->KeyDown += ref new TypedEventHandler<CoreWindow ^, KeyEventArgs ^>(
		[=](CoreWindow ^sender, KeyEventArgs^args)
	{
		send_msg(events::message(events::core_msg::key_down, 0, (long64)reinterpret_cast<pointer>(args)));
	});

	handle->m_wnd->KeyUp += ref new TypedEventHandler<CoreWindow ^, KeyEventArgs ^>(
		[=](CoreWindow ^sender, KeyEventArgs^args)
	{
		send_msg(events::message(events::core_msg::key_up, 0, (long64)reinterpret_cast<pointer>(args)));
	});

	handle->m_wnd->CharacterReceived += ref new TypedEventHandler<CoreWindow ^, CharacterReceivedEventArgs ^>(
		[=](CoreWindow ^sender, CharacterReceivedEventArgs^args)
	{
		send_msg(events::message(events::core_msg::key_up, 0, (long64)reinterpret_cast<pointer>(args)));
	});

	handle->m_wnd->PointerEntered += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(
		[=](CoreWindow ^, PointerEventArgs ^args)
	{
		send_msg(events::message(events::core_msg::pointer_entered, 0, (long64)reinterpret_cast<pointer>(args)));
	});

	handle->m_wnd->PointerPressed += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(
		[=](CoreWindow ^, PointerEventArgs ^args)
	{
		send_msg(events::message(events::core_msg::pointer_pressed, 0, (long64)reinterpret_cast<pointer>(args)));
	});

	handle->m_wnd->PointerMoved += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(
		[=](CoreWindow ^, PointerEventArgs ^args)
	{
		send_msg(events::message(events::core_msg::pointer_moved, 0, (long64)reinterpret_cast<pointer>(args)));
	});

	handle->m_wnd->PointerReleased += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(
		[=](CoreWindow ^, PointerEventArgs ^args)
	{
		send_msg(events::message(events::core_msg::pointer_released, 0, (long64)reinterpret_cast<pointer>(args)));
	});

	handle->m_wnd->PointerExited += ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(
		[=](CoreWindow ^, PointerEventArgs ^args)
	{
		send_msg(events::message(events::core_msg::pointer_leaved, 0, (long64)reinterpret_cast<pointer>(args)));
	});
}

void core_window::initialize()
{
	auto bounds = handle->m_wnd->Bounds;
	handle->m_clip_size = { bounds.Width,bounds.Height };
	handle->m_is_visible = handle->m_wnd->Visible;

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();
	float scale = (float)currentDisplayInformation->RawPixelsPerViewPixel;
	handle->m_scale_factor = { scale ,scale };
}

void core_window::update()
{

}

void core_window::finalize()
{
	handle->m_wnd = null;
}

void core_window::destroy()
{

}

dword core_window::send_msg(events::message m)
{
	if (!core_app::instance()->is_main_thread())
		return dispatcher()->post_msg(m)->result();

	m.result(-1);
	switch (m.msg())
	{
	case events::core_msg::created:
		m.result(on_created(m));
		break;
	case events::core_msg::initial_update:
		m.result(on_initialize(m));
		break;
	case events::core_msg::activate:
		m.result(on_activate(m));
		break;
	case events::core_msg::update:
		m.result(on_update(m));
		break;
	case events::core_msg::size:
		m.result(on_display_event(m));
		break;
	case events::core_msg::display_change:
		m.result(on_display_event(m));
		break;
	case events::core_msg::orientation:
		m.result(on_display_event(m));
		break;
	case events::core_msg::close:
		m.result(on_finalize(m));
		break;
	case events::core_msg::destroyed:
		m.result(on_destroy(m));
		break;
	case events::core_msg::system_reserved_event:
		m.result(on_command(m));
		break;
	case events::core_msg::pointer_entered:
	case events::core_msg::pointer_pressed:
	case events::core_msg::pointer_moved:
	case events::core_msg::pointer_released:
	case events::core_msg::pointer_leaved:
		m.result(on_pointer_event(m));
		break;
	case events::core_msg::key_down:
	case events::core_msg::key_up:
		m.result(on_key_event(m));
		break;
	}
	return m.result();
}

core::async::iasync<void> core_window::post_task(core::async::iasync<void> async)
{
	auto task = interface_cast<async_task>(async.get());
	if (task == null)
		return null;
	task->add_ref();
	{//scope
		core::async::scope_locker<core::async::mutex_t> lock = handle->m_mutex;
		handle->m_msg_queue += events::message(events::core_msg::system_reserved_event, 0, reinterpret_cast<LPARAM>(task));
	}
	return async;
}

events::event_token_t core_window::listen_to(events::event_t e)
{
	switch (e.get()->msg_type())
	{
	case events::core_msg::size:
		return m_size_event += e;
	}

	return events::event_token_t();
}

////////////////////////////////////////////////////////////////////////////

void core_window::dispatch_msgs()
{
	if (!core_app::instance()->is_main_thread())
		throw_exception(except_code::invalid_access);
	core::async::scope_locker<core::async::mutex_t> lock = handle->m_mutex;
	events::message msg;
	while (handle->m_msg_queue->pop(msg, false))
	{
		handle->m_mutex.unlock();
		send_msg(msg);
		handle->m_mutex.lock();
	}
}

dword core_window::on_created(events::message& m)
{
	create((pointer)m.lparam());
	m_created_event(new events::created_event_args(m, this, core_app::instance(), null));

	return 0;
}

dword core_window::on_initialize(events::message& m)
{
	initialize();
	m_initialize_event(new events::created_event_args(m, this, core_app::instance(), null));
	return 0;
}

dword core_window::on_finalize(events::message& m)
{
	finalize();
	m_finalize_event(new events::msg_event_args(m));
	return 0;
}

dword core_window::on_destroy(events::message& m)
{
	m_destroyed_event(new events::msg_event_args(m));
	destroy();
	return 0;
}

dword core_window::on_draw(events::message& m)
{
	m_draw_event(new events::draw_event_args(m, this, null, clip_size));
	return 0;
}

dword core_window::on_update(events::message& m)
{
	if (is_visible)
	{
		handle->m_wnd->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

		update();
	}
	else
	{
		handle->m_wnd->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
	}
	m_update_event(new events::msg_event_args(m));
	return 0;
}

dword core_window::on_activate(events::message& m)
{
	m_activate_event(new events::activate_event_args(m, m.wparam() ?
		activate_status::activated :
		activate_status::deactivated));
	return 0;
}

dword core_window::on_display_event(events::message& m)
{
	auto bounds = handle->m_wnd->Bounds;
	handle->m_clip_size = { bounds.Width,bounds.Height };

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->ResolutionScale;
	Windows::Graphics::Display::ResolutionScale;
	float scale = (float)currentDisplayInformation->RawPixelsPerViewPixel;
	handle->m_scale_factor = { scale ,scale };

	display::display_info_t info = {
		(display::orientation)currentDisplayInformation->CurrentOrientation,	//orientation_t current_orientation;
		(display::orientation)currentDisplayInformation->NativeOrientation,		//orientation_t native_orientation;
		clip_size,	//graphics::size<float> display_resolution;
		scale_factor, //graphics::size<float> display_scale_factor;
		currentDisplayInformation->LogicalDpi//double dot_per_inche;
	};
	
	switch (m.msg())
	{
	case events::core_msg::size:
		m_size_event(new events::display_info_event_args(m, this, display_invalidate_reason::size_changed, info));
		break;
	case events::core_msg::display_change:
		m_display_change_event(new events::display_info_event_args(m, this, (display_invalidate_reason)m.wparam(), info));
		break;
	case events::core_msg::orientation:
		m_orientation_event(new events::display_info_event_args(m, this, display_invalidate_reason::orientation_changed, info));
		break;
	}
	return 0;
}

dword core_window::on_pointer_event(events::message& m)
{
	PointerEventArgs^ args = reinterpret_cast<PointerEventArgs^>((pointer)m.lparam());
	
	input::poiner_info_t info = {
		{ args->CurrentPoint->Position.X,args->CurrentPoint->Position.Y }, //graphics::point<float> point;
		args->CurrentPoint->PointerId, //short id;
		args->CurrentPoint->Properties->IsLeftButtonPressed, //bool is_primary_action;
		args->CurrentPoint->Properties->IsRightButtonPressed,		//bool is_secondary_action;
		(input::pointer_hardware_type)args->CurrentPoint->PointerDevice->PointerDeviceType, //pointer_hardware_type_t type;
		input::key_modifiers::none //TODO://key_modifiers_t modifiers;
	};

	switch (m.msg())
	{
	case events::core_msg::pointer_entered:
		m_pointer_entered_event(new events::pointer_event_args(m, info));
		break;
	case events::core_msg::pointer_pressed:
		m_pointer_pressed_event(new events::pointer_event_args(m, info));
		break;
	case events::core_msg::pointer_moved:
		m_pointer_moved_event(new events::pointer_event_args(m, info));
		break;
	case events::core_msg::pointer_released:
		m_pointer_released_event(new events::pointer_event_args(m, info));
		break;
	case events::core_msg::pointer_leaved:
		m_pointer_leaved_event(new events::pointer_event_args(m, info));
		break;
	}
	
	return 0;
}

dword core_window::on_key_event(events::message& m)
{
	KeyEventArgs^args = reinterpret_cast<KeyEventArgs^>((pointer)m.lparam());

	//input::key_state::
	input::key_info_t info = {
		(input::virtual_key)args->VirtualKey, //virtual_key key_code;
		0, //char32_t char_code;
		args->KeyStatus.RepeatCount, //word flags;
		args->KeyStatus.IsKeyReleased ? input::key_state::released: input::key_state::pressed, //key_state_t state;
		input::key_modifiers::none,	//key_modifiers_t modifiers;
	};

	switch (m.msg())
	{
	case events::core_msg::key_down:
		m_key_pressed_event(new events::key_event_args(m, info));
		break;
	case events::core_msg::key_up:
		m_key_released_event(new events::key_event_args(m, info));
		break;
	}

	return 0;
}

dword core_window::on_put_char_event(events::message& m)
{
	CharacterReceivedEventArgs^args = reinterpret_cast<CharacterReceivedEventArgs^>((pointer)m.lparam());
	
	input::key_info_t info = {
		(input::virtual_key)0, //virtual_key key_code;
		args->KeyCode, //char32_t char_code;
		args->KeyStatus.RepeatCount, //word flags;
		args->KeyStatus.IsKeyReleased ? input::key_state::released : input::key_state::pressed, //key_state_t state;
		input::key_modifiers::none,	//key_modifiers_t modifiers;
	};

	m_put_char_event(new events::key_event_args(m, info));
	
	return 0;
}


dword core_window::on_command(events::message& m)
{
	auto task = reinterpret_cast<async_task*>(m.lparam());
	task->execute();
	task->release();
	return 0;
}






