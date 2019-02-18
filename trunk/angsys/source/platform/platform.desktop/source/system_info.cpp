#include "pch.h"
#include "ang/platform/angwin/angwin.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;


ushort system_info::screen_counter()
{
	return GetSystemMetrics(SM_CMONITORS);
}

ushort system_info::mouse_buttons()
{
	return GetSystemMetrics(SM_CMOUSEBUTTONS);
}

graphics::size<float> system_info::main_screen_bound()
{
	graphics::size<float> size((float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN));
	return size;
}

graphics::size<float> system_info::current_screen_bound()
{
	auto app = app::current_app();
	if (app != null && app->main_wnd() != null)
	{
		auto monitor = MonitorFromWindow((HWND)app->main_wnd()->core_view_handle(), MONITOR_DEFAULTTONEAREST);
		if (monitor != null)
		{
			MONITORINFO info;
			if (GetMonitorInfo(monitor, &info) != FALSE)
			{
				return graphics::size<float>(float(info.rcMonitor.right - info.rcMonitor.left), float(info.rcMonitor.bottom - info.rcMonitor.top));
			}
		}
	}
	return  main_screen_bound();
}

display::orientation_t system_info::current_screen_orientation()
{
	//Compare the height and width of the screen and act accordingly.
	auto size = system_info::current_screen_bound();
	if (size.width > size.height)
		return display::orientation::landscape;
	else
		return display::orientation::portrait;
}

display::orientation g_SystemInfo_NativeScreenOrientation = display::orientation::landscape; //= system_info::CurrentScreenOrientation();

display::orientation_t system_info::native_screen_orientation()
{
	return g_SystemInfo_NativeScreenOrientation;
}