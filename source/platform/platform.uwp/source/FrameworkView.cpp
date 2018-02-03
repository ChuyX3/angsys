#include "pch.h"
#include <ang/platform/uwp/angwin.hpp>
#include <ang/core/files.hpp>
#include "FrameworkView.hpp"

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

/////////////////////////////////////////////////

FrameworkViewSource::FrameworkViewSource(framework_t f)
	: framework_(f)
{
}

FrameworkViewSource::~FrameworkViewSource()
{
	framework_ = nullptr;
}

Windows::ApplicationModel::Core::IFrameworkView^ FrameworkViewSource::CreateView()
{
	return ref new FrameworkView(framework_);
}

/////////////////////////////////////////////////////////

FrameworkView::FrameworkView(weak_ptr<framework> f)
	: m_windowClosed(false)
	, m_windowVisible(false)
	, m_framework(f)
{
	auto fs = core::files::ifile_system::get_file_system();

	auto path = interop::string_cast<wstring>(Windows::ApplicationModel::Package::Current->InstalledLocation->Path);
	path += "\\"_s;
	fs->register_paths(path);

	path = interop::string_cast<wstring>(Windows::Storage::ApplicationData::Current->LocalFolder->Path);
	path += "\\"_s;
	fs->register_paths(path);
}

FrameworkView::~FrameworkView()
{
	m_framework = nullptr;
}

// The first method called when the IFrameworkView is being created.
void FrameworkView::Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView)
{
	// Register event handlers for app lifecycle. This example includes Activated, so that we
	// can make the CoreWindow active and start rendering on the window.
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &FrameworkView::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &FrameworkView::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &FrameworkView::OnResuming);
}

// Called when the CoreWindow object is created (or re-created).
void FrameworkView::SetWindow(Windows::UI::Core::CoreWindow^ window)
{
	dispatcher = window->Dispatcher;

	window->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &FrameworkView::OnWindowSizeChanged);

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &FrameworkView::OnVisibilityChanged);

	window->Closed +=
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &FrameworkView::OnWindowClosed);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &FrameworkView::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &FrameworkView::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &FrameworkView::OnDisplayContentsInvalidated);

	window->PointerPressed += 
		ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(this, &FrameworkView::OnPointerPressed);

	window->PointerMoved += 
		ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(this, &FrameworkView::OnPointerMoved);

	window->PointerReleased += 
		ref new TypedEventHandler<CoreWindow ^, PointerEventArgs ^>(this, &FrameworkView::OnPointerReleased);

	m_framework.lock()->create_window(window);
}

void FrameworkView::Load(Platform::String^ entryPoint)
{
	
}

void FrameworkView::Run()
{
	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

		}
		else
		{
			dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void FrameworkView::Uninitialize()
{

}

// Application lifecycle event handlers.

void FrameworkView::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

void FrameworkView::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	core::async::task::run_async<void>([this, deferral](core::async::iasync<void>* task)
	{
		//m_main->OnSuspending();
		deferral->Complete();
	});
}

void FrameworkView::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	//m_main->OnResuming();
}

// Window event handlers.

void FrameworkView::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	//GetDeviceResources()->SetLogicalSize(Size(sender->Bounds.Width, sender->Bounds.Height));
	//m_main->OnWindowSizeChanged();
}

void FrameworkView::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void FrameworkView::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

void FrameworkView::OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args)
{

}

void FrameworkView::OnPointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args)
{

}

void FrameworkView::OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args)
{

}

// DisplayInformation event handlers.

void FrameworkView::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	//GetDeviceResources()->SetDpi(sender->LogicalDpi);
	//m_main->OnWindowSizeChanged();
}

void FrameworkView::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	//GetDeviceResources()->SetCurrentOrientation(sender->CurrentOrientation);
	//m_main->OnWindowSizeChanged();
}

void FrameworkView::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	//GetDeviceResources()->ValidateDevice();
}
