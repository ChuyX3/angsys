#pragma once

namespace ang
{
	namespace graphics
	{
		class gl_context final
			: public implement<gl_context
			, iid("ang::graphics::gl_context")>
		{
			HDC m_hdc;
			HGLRC m_gl_context;
			//HGLRC m_gl_async_context;
			platform::icore_view_t m_wnd;
			core::async::mutex_t m_mutex;
			core::async::idispatcher_t m_async_worker;

		public:
			gl_context(platform::icore_view_t wnd);

		public:
			bool lock();
			void present();
			bool unlock();

			template<typename T>
			inline core::async::iasync<T> create_task(function<T(void)> action)const {
				return m_async_worker->run_async<T>([=](core::async::iasync<T> task)->T {
					return action();
				});
			}
		public:

		private:
			virtual void dispose()override;
			void init(platform::icore_view_t wnd);

		private:
			virtual~gl_context();
		};
	}
}