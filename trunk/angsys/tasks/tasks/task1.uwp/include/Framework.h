#pragma once

#include <angsys.h>
#include "engine.h"

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			ref class Framework sealed
				: public Windows::ApplicationModel::Core::IFrameworkView
			{
			private:
				static Framework^ s_instance;

			public:
				static property Framework^ Instance { Framework^ get() { return s_instance; } }

			public:
				Framework();
				virtual~Framework();

			public: // IFrameworkView Methods.
				virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView) override;
				virtual void SetWindow(Windows::UI::Core::CoreWindow^ window) override;
				virtual void Load(Platform::String^ entryPoint) override;
				virtual void Run(void) override;
				virtual void Uninitialize(void) override;

			public: //Event Handlers
				// Application lifecycle event handlers.
				void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
				void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
				void OnResuming(Platform::Object^ sender, Platform::Object^ args);

				// Window event handlers.
				void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
				void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);
				void OnKeyPressed(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::KeyEventArgs ^args);

			internal: //Properties
				property graphics::engine* Engine {
					graphics::engine* get() {
						return graphics::engine::instance();
					}
				}

			private:
				core::time::step_timer_t timer;
				bool m_windowClosed;
				bool m_windowVisible;

			};
		}

		ref class FrameworkSource sealed
			: Windows::ApplicationModel::Core::IFrameworkViewSource
		{
		public:
			virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
		};

	}
}
	