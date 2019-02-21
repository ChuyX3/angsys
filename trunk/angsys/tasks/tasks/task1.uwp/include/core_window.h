#pragma once

namespace ang
{
	namespace platform
	{
		namespace windows
		{

			ang_object(core_window);
			ang_object(core_listener);

			class core_listener
				: public object
				, public imessage_listener
			{
			private:
				Windows::UI::Core::CoreDispatcher^ m_dispatcher;
				core::delegates::function<dword(platform::events::message)> m_wnd_proc;
				core::async::cond_ptr_t m_cond;
				core::async::mutex_ptr_t m_mutex;

			public:
				core_listener(Windows::UI::Core::CoreDispatcher^ dispatcher, core::delegates::function<dword(platform::events::message)> wnd_proc);

				ANG_DECLARE_INTERFACE();

				virtual dword send_msg(events::message msg) override;
				virtual core::async::iasync<dword> post_msg(events::message) override;
				virtual bool listen_to(events::event_t) override;

			private:
				virtual~core_listener();
			};

			class core_window
				: public object
				, public icore_view
			{
			private:
				Windows::UI::Core::CoreWindow^ m_wnd;
				core_listener_t listener;

			public:
				core_window(Windows::UI::Core::CoreWindow^ wnd
				);

				ANG_DECLARE_INTERFACE();

				virtual pointer core_view_handle()const override;
				virtual icore_context_t core_context()const override;
				virtual graphics::size<float> core_view_size()const override;
				virtual graphics::size<float> core_view_scale_factor()const override;
				virtual imessage_listener_t listener()const override;
			};

		}
	}
}
