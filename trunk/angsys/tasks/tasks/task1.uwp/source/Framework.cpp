#include "pch.h"
#include "Framework.h"


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

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto source = ref new FrameworkSource();
	CoreApplication::Run(source);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IFrameworkView^ FrameworkSource::CreateView()
{
	return ref new Framework();
}

Framework::Framework()
	: m_windowClosed(false)
	, m_windowVisible(true)
{
	graphics::engine::instance();
	timer = ang::make_shared<core::time::step_timer>();
}

Framework::~Framework()
{
	graphics::engine::release_instance();
}


void Framework::Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView)
{
	applicationView->Activated += 
		ref new TypedEventHandler<CoreApplicationView ^, IActivatedEventArgs ^>(this, &Framework::OnActivated);

	CoreApplication::Suspending += 
		ref new EventHandler<SuspendingEventArgs^>(this, &Framework::OnSuspending);

	CoreApplication::Resuming += 
		ref new EventHandler<Platform::Object^>(this, &Framework::OnResuming);
}

void Framework::SetWindow(Windows::UI::Core::CoreWindow^ window)
{
	window->KeyDown += ref new TypedEventHandler<CoreWindow ^, KeyEventArgs ^>(this, &Framework::OnKeyPressed);
	window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &Framework::OnWindowClosed);
	window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &Framework::OnVisibilityChanged);

	Engine->init(new graphics::holographic_space(Windows::Graphics::Holographic::HolographicSpace::CreateForCoreWindow(window)));
	timer->reset();
}

void Framework::Load(Platform::String^ entryPoint)
{

}

void Framework::Run(void)
{
	while (!m_windowClosed)
	{
		if (m_windowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			timer->tick([this]()
			{
				Engine->update(timer);
				Engine->render();
			});		

		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void Framework::Uninitialize(void)
{

}


void Framework::OnActivated(CoreApplicationView ^sender, IActivatedEventArgs ^args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

void Framework::OnSuspending(Platform::Object ^sender, SuspendingEventArgs ^args)
{
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();
	Concurrency::create_task([this, deferral]()
	{
		//m_deviceResources->Trim();

		// Insert your code here.

		deferral->Complete();
	});
}

void Framework::OnResuming(Platform::Object ^sender, Platform::Object ^args)
{
	
}

void Framework::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
}

void Framework::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
	m_windowClosed = true;
}

// DisplayInformation event handlers.
void Framework::OnKeyPressed(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args)
{

}