#include "pch.h"
#include <ang/platform/uwp/windows.h>
#include "dispatcher.h"
#include "event_args.h"


using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

namespace ang
{

	namespace platform
	{	
	
		ref class WinRTApp sealed 
			: public IFrameworkView
		{
		public:
			WinRTApp();
			virtual void Initialize(CoreApplicationView^ applicationView);
			virtual void SetWindow(CoreWindow^ window);
			virtual void Load(Platform::String^ entryPoint);
			virtual void Run();
			virtual void Uninitialize();
		};
		
		ref class WinRTAppSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
		{
		public:
			virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
		};

	}
}



core_app::core_app()
	: m_is_running(false)
	, m_start_event(this, [](events::core_msg_t code) { return code == events::core_msg::start_app; })
	, m_exit_event(this, [](events::core_msg_t code) { return code == events::core_msg::exit_app; })
	, m_dispatcher(null)
{

}

core_app::~core_app()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::platform::windows::core_app, object, icore_app);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::platform::windows::core_app);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::platform::windows::core_app, object, icore_app);

dword core_app::run()
{
	auto source = ref new WinRTAppSource();
	CoreApplication::Run(source);
	release_instance();
	return 0;
}

void core_app::clear()
{
	//destroy();
}

imessage_listener_t core_app::dispatcher()const
{
	return m_dispatcher;
}

pointer core_app::core_app_handle()const
{
	return null;
}

icore_view_t core_app::main_core_view()
{
	return (core_window*)main_wnd;
}

input::ikeyboard_t core_app::core_app::keyboard()
{
	return null;
}

bool core_app::is_main_thread()const 
{
	return m_ui_thread.is_empty() ? false : m_ui_thread->has_thread_access();
}

core::async::thread_t core_app::main_thread()const
{
	return m_ui_thread;
}

var core_app::settings(cstr_t key)const
{
	try {
		return m_settings[key];
	}
	catch (exception_t const& e) {
		e->what();
	}
}

void core_app::settings(cstr_t key, var val)
{
	m_settings[key] = val;
}

bool core_app::initialize(pointer args)
{
	m_ui_thread = core::async::thread::this_thread();
	m_dispatcher = new windows::dispatcher(
		bind(this, &core_app::listen_to),
		bind(this, &core_app::send_msg),
		bind(this, &core_app::post_task));
	m_main_wnd = new core_window();

	auto applicationView = reinterpret_cast<CoreApplicationView^>(args);

	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(
			[](CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
	{
		// Run() won't start until the CoreWindow is activated.
		CoreWindow::GetForCurrentThread()->Activate();
	});

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(
			[this](Platform::Object^ sender, SuspendingEventArgs^ args)
	{
		dispatcher()->send_msg(
			events::message(events::core_msg::activate, false, (long64)reinterpret_cast<pointer>(args))
		);
	});

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(
			[this](Platform::Object^ sender, Platform::Object^ args)
	{
		dispatcher()->send_msg(
			events::message(events::core_msg::activate, true, (long64)reinterpret_cast<pointer>(args))
		);
	});

	send_msg(events::message(events::core_msg::start_app));

	return true;
}

void core_app::running()
{
	while (main_wnd->is_created)
	{
		dispatch_msgs();
		main_wnd->dispatcher()->send_msg(events::message(events::core_msg::update));
	}
	main_wnd->dispatcher()->send_msg(events::message(events::core_msg::destroyed));
	destroy();
}

bool core_app::destroy()
{
	send_msg(events::message(events::core_msg::exit_app));
	m_start_event.empty();
	m_exit_event.empty();
	return true;
}

void core_app::dispatch_msgs()
{
	if (!m_ui_thread->has_thread_access())
		throw_exception(except_code::invalid_access);
	core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
	events::message msg;
	while (m_msg_queue->pop(msg, false))
	{
		m_mutex.unlock();
		send_msg(msg);
		m_mutex.lock();
	}
}

dword core_app::send_msg(events::message m)
{
	if (!m_ui_thread->has_thread_access())
		return dispatcher()->post_msg(m)->result();
	
	m.result(-1);
	switch (m.msg())
	{
		case events::core_msg::start_app:
			m.result(on_start(m));
			break;
		case events::core_msg::exit_app:
			m.result(on_exit(m));
			break;
		case events::core_msg::activate:
			m.result(on_activated(m));
			break;
		case events::core_msg::system_reserved_event:
			m.result(on_command(m));
			break;
	}
	return m.result();
}

core::async::iasync<void> core_app::post_task(core::async::iasync<void> async)
{
	auto task = interface_cast<async_task>(async.get());
	if (task == null)
		return null;
	task->add_ref();
	{//scope
		core::async::scope_locker<core::async::mutex_t> lock = m_mutex;
		m_msg_queue += events::message(events::core_msg::system_reserved_event, 0, reinterpret_cast<LPARAM>(task));
	}
	return async;
}

events::event_token_t core_app::listen_to(events::event_t e)
{
	return events::event_token_t();
}

ang_platform_implement_event_handler(core_app, start_event);
ang_platform_implement_event_handler(core_app, exit_event);

dword core_app::on_start(events::message& m)
{
	events::iapp_status_event_args_t args = new events::app_status_event_args(m, this);
	m_start_event(args.get());
	return 0;
}

dword core_app::on_activated(events::message& m)
{
	return main_wnd->dispatcher()->send_msg(m);
}

dword core_app::on_exit(events::message& m)
{
	events::iapp_status_event_args_t args = new events::app_status_event_args(m, this);
	m_exit_event(args.get());
	return 0;
}

dword core_app::on_command(events::message& m)
{
	auto task = reinterpret_cast<async_task*>(m.lparam());
	task->execute();
	task->release();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

IFrameworkView^ WinRTAppSource::CreateView()
{
	return ref new WinRTApp();
}

WinRTApp::WinRTApp() 
{
}

void WinRTApp::Initialize(CoreApplicationView^ applicationView)
{
	core_app::instance()->initialize(reinterpret_cast<pointer>(applicationView));
}

void WinRTApp::SetWindow(CoreWindow^ window)
{
	core_app::instance()->main_wnd->dispatcher()->send_msg(events::message(events::core_msg::created, 0, (long64)reinterpret_cast<pointer>(window)));
}

void WinRTApp::Load(Platform::String^ entryPoint)
{
	core_app::instance()->main_wnd->dispatcher()->send_msg(events::message(events::core_msg::initial_update));
}

void WinRTApp::Run()
{
	core_app::instance()->running();
}

void WinRTApp::Uninitialize()
{
}

