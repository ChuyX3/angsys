#include "pch.h"
#include <ang/platform/uwp/windows.h>
#include <Unknwn.h>
#include "dispatcher.h"

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
	Windows::UI::Core::CoreWindow^ m_wnd = null;
	dispatcher_t m_dispatcher = null;
};


core_window::core_window()
	: m_handle(null)
	, m_initialize_event(this, [](core_msg_t msg) { return msg == core_msg::initial_update; })
	, m_closed_event(this, [](core_msg_t msg) { return msg == core_msg::close; })
	, m_destroyed_event(this, [](core_msg_t msg) { return msg == core_msg::destroyed; })
	, m_draw_event(this, [](core_msg_t msg) { return msg == core_msg::draw; })
	, m_update_event(this, [](core_msg_t msg) { return msg == core_msg::update; })
	, m_orientation_event(this, [](core_msg_t msg) { return msg == core_msg::orientation; })
	, m_activate_event(this, [](core_msg_t msg) { return msg == core_msg::got_focus || msg == core_msg::lost_focus; })
	, m_size_event(this, [](core_msg_t msg) { return msg == core_msg::size; })
	, m_display_change_event(this, [](core_msg_t msg) { return msg == core_msg::display_change; })
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


ang_platform_implement_event_handler(core_window, initialize_event);
ang_platform_implement_event_handler(core_window, closed_event);
ang_platform_implement_event_handler(core_window, destroyed_event);

ang_platform_implement_event_handler(core_window, draw_event);
ang_platform_implement_event_handler(core_window, update_event);

ang_platform_implement_event_handler(core_window, orientation_event);
ang_platform_implement_event_handler(core_window, activate_event);
ang_platform_implement_event_handler(core_window, size_event);
ang_platform_implement_event_handler(core_window, display_change_event);

ang_platform_implement_event_handler(core_window, key_pressed_event);
ang_platform_implement_event_handler(core_window, key_released_event);

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

void core_window::set_scale_factor_property(base_property<graphics::size<float>>* prop, graphics::size<float>&& value)
{
	field_to_parent(&core_window::scale_factor, prop)->handle->m_scale_factor = value;
	field_to_parent(&core_window::scale_factor, prop)->send_msg(events::message(events::core_msg::display_change));
}

core_window::hwnd_t core_window::get_handle_property(base_property<hwnd_t> const* prop)
{
	return field_to_parent(&core_window::handle, prop)->m_handle;
}

bool core_window::get_is_created_property(base_property<bool> const* prop)
{
	return field_to_parent(&core_window::handle, prop)->handle->m_wnd != null;
}

bool core_window::get_is_visible_property(base_property<bool> const* prop)
{
	return field_to_parent(&core_window::handle, prop)->handle->m_is_visible;
}

void core_window::clear()
{
	terminate();
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

icore_context_t core_window::core_context()const
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

bool core_window::initialize(pointer ptr)
{
	handle->m_wnd = reinterpret_cast<Windows::UI::Core::CoreWindow^>(ptr);
	handle->m_wnd->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(
			[=](CoreWindow^sender, WindowSizeChangedEventArgs^args)
	{

	});

	handle->m_wnd->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(
			[=](CoreWindow^sender, VisibilityChangedEventArgs^args)
	{
	});

	handle->m_wnd->Closed +=
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(
			[=](CoreWindow^sender, CoreWindowEventArgs^args)
	{
	});

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(
			[](DisplayInformation^, Platform::Object^)
	{

	});

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(
			[](DisplayInformation^, Platform::Object^)
	{

	});

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Platform::Object^>(
			[](DisplayInformation^, Platform::Object^)
	{

	});

}

bool core_window::update()
{

}

bool core_window::terminate()
{

}










