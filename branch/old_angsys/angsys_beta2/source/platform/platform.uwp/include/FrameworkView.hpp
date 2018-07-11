#pragma once

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			ref class FrameworkView sealed
				: public Windows::ApplicationModel::Core::IFrameworkView
			{
			private:
				bool m_windowClosed;
				bool m_windowVisible;
				weak_ptr<framework> m_framework;
				Windows::UI::Core::CoreDispatcher^ dispatcher;

			public:
				FrameworkView(weak_ptr<framework>);
				virtual~FrameworkView();

			public:
				// IFrameworkView methods.
				virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
				virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
				virtual void Load(Platform::String^ entryPoint);
				virtual void Run();
				virtual void Uninitialize();

			protected:
				// Application lifecycle event handlers.
				void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
				void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
				void OnResuming(Platform::Object^ sender, Platform::Object^ args);

				// Window event handlers.
				void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
				void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
				void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);
				void OnPointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
				void OnPointerMoved(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);
				void OnPointerReleased(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args);

				// DisplayInformation event handlers.
				void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
				void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
				void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

				
			};

			ref class FrameworkViewSource sealed
				: public Windows::ApplicationModel::Core::IFrameworkViewSource
			{
			private:
				framework_t framework_;

			public:
				FrameworkViewSource(framework_t);
				virtual~FrameworkViewSource();
				virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
			};



		}
	}
}