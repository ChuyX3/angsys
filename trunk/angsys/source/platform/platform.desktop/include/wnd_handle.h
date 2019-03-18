#ifndef __WND_HANDLE_H__
#define __WND_HANDLE_H__

#include <ang/platform/win32/windows.h>
#include "dispatcher.h"

namespace ang
{
	namespace platform
	{
		namespace windows
		{
			typedef struct window::_hwnd
			{
				HWND m_hwnd = null;
				window_t m_wnd = null;
				dispatcher_t m_dispatcher;
			//	input::keyboard_t m_keyboard;
				core::async::mutex_ptr_t m_mutex = make_shared<core::async::mutex>();
				core::async::cond_ptr_t m_cond = make_shared<core::async::cond>();
				//display::orientation_t current_orientation;
				var_args_t m_user_args = null;
			}*hwnd;

			struct ui_thread::_hthread
			{
				pointer hmodule = null;
				core::async::thread_t core_thread = null;
				function<void(void)> update_propc;
				dispatcher_t dispatcher;
				collections::hash_map<astring, var> properties;
			};
			thread_local extern ui_thread* s_current_ui_thread;
		}
	}
}
#endif//__WND_HANDLE_H__
