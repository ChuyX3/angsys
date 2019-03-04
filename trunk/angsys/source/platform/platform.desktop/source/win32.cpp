#include "pch.h"
#include "ang/platform/win32/windows.h"

using namespace ang;
using namespace ang::platform;
using namespace ang::platform::windows;

ANG_EXTERN ulong64 get_performance_time_us(void)
{
	{
#if defined ANDROID_PLATFORM
		struct timespec ts;
		ulong64 theTick = 0;
		clock_gettime(CLOCK_REALTIME, &ts);
		theTick = (ulong64)ts.tv_nsec / 1000.0;
		theTick += (ulong64)ts.tv_sec * 1000000.0;
		return theTick;
#else
		static struct PerformanceFrequency {
			ulong64 QuadPart;
			PerformanceFrequency() {
				LARGE_INTEGER _frec;
				QueryPerformanceFrequency(&_frec);
				QuadPart = (ulong64)_frec.QuadPart;
			}
		}frec;

		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return  (1000000 * count.QuadPart) / frec.QuadPart; //uS
#endif
	}
}

safe_enum_rrti2(ang::platform::windows, showing_flag);
safe_enum_rrti2(ang::platform::windows, class_style);
safe_flags_implement(ang::platform::windows, class_style);

static collections::pair<cstr_t, class_style> _parse_class_style_map[] =
{
	{ "byte_align_client"_s , class_style::byte_align_client },
	{ "byte_align_window"_s , class_style::byte_align_window },
	{ "class_dc"_s , class_style::class_dc },
	{ "dbl_clks"_s , class_style::dbl_clks },
	{ "drop_shaow"_s , class_style::drop_shaow },
	{ "global_class"_s , class_style::global_class },
	{ "hredraw"_s , class_style::hredraw },
	{ "no_close"_s , class_style::no_close },
	{ "none"_s , class_style::none },
	{ "own_dc"_s , class_style::own_dc },
	{ "parent_dc"_s , class_style::parent_dc },
	{ "save_bits"_s , class_style::save_bits },
	{ "vredraw"_s , class_style::vredraw }
};

static collections::pair<class_style, cstr_t> to_string_class_style_map[] =
{
	{ class_style::none , "none"_s },
	{ class_style::vredraw , "vredraw"_s },
	{ class_style::hredraw , "hredraw"_s },
	{ class_style::dbl_clks , "dbl_clks"_s },
	{ class_style::own_dc , "own_dc"_s },
	{ class_style::class_dc , "class_dc"_s },
	{ class_style::parent_dc , "parent_dc"_s },
	{ class_style::no_close , "no_close"_s },
	{ class_style::save_bits , "save_bits"_s },
	{ class_style::byte_align_client , "byte_align_client"_s },
	{ class_style::byte_align_window , "byte_align_window"_s },
	{ class_style::global_class , "global_class"_s },
	{ class_style::drop_shaow , "drop_shaow" }
};

wstring class_style_t::to_string()const
{
	wstring out = "";
	class_style_t val = get();
	bool first = true;
	for (int i = algorithms::array_size(to_string_class_style_map) - 1; i >= 0; --i)
	{
		if (val.m_value == 0)
			break;
		if (val & to_string_class_style_map[i].key)
		{
			if (first) {
				first = false;
				out << to_string_class_style_map[i].value;
			}
			else
				out << " + "_s << to_string_class_style_map[i].value;
			val -= to_string_class_style_map[i].key;
		}
	}
	return ang::move(out);
}

//template<> class_style_t ang::xml::xml_value::as<class_style_t>()const
//{
//	cwstr_t key = as<cwstr_t>();
//	index idx = ang_binary_search<cwstr_t>(key, _parse_class_style_map);
//	if (idx < array_size(_parse_class_style_map))
//		return _parse_class_style_map[idx].value;
//	return class_style::none;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

safe_enum_rrti2(ang::platform::windows, wnd_style);
safe_flags_implement(ang::platform::windows, wnd_style);

static collections::pair<cstr_t, wnd_style> _parse_wnd_style_map[] =
{
	{ "border"_s , wnd_style::border },
	{ "caption"_s , wnd_style::caption },
	{ "child"_s , wnd_style::child },
	{ "clip_children"_s , wnd_style::clip_children },
	{ "clip_siblings"_s , wnd_style::clip_siblings },
	{ "dialog_frame"_s , wnd_style::dialog_frame },
	{ "disabled"_s , wnd_style::disabled },
	{ "group"_s , wnd_style::group },
	{ "hscroll"_s , wnd_style::hscroll },
	{ "maximize"_s , wnd_style::maximize },
	{ "maximize_box"_s , wnd_style::maximize_box },
	{ "minimize"_s , wnd_style::minimize },
	{ "minimize_box"_s , wnd_style::minimize_box },
	{ "none"_s , wnd_style::none },
	{ "overlapped"_s , wnd_style::overlapped },
	{ "overlapped_window"_s , wnd_style::overlapped_window },
	{ "popup"_s , wnd_style::popup },
	{ "resizable_frame"_s , wnd_style::resizable_frame },
	{ "shown"_s , wnd_style::shown },
	{ "system_menu"_s , wnd_style::system_menu },
	{ "tab_stop"_s , wnd_style::tab_stop },
	{ "vscroll"_s , wnd_style::vscroll },
};

static collections::pair<wnd_style, cstr_t> to_string_wnd_style_map[] =
{
	{ wnd_style::popup , "popup"_s },
	{ wnd_style::none , "none"_s },
	{ wnd_style::maximize_box , "maximize_box"_s },
	{ wnd_style::minimize_box , "minimize_box"_s },
	{ wnd_style::resizable_frame , "resizable_frame"_s },
	{ wnd_style::system_menu , "system_menu"_s },
	{ wnd_style::hscroll , "hscroll"_s },
	{ wnd_style::vscroll , "vscroll"_s },
	{ wnd_style::dialog_frame , "dialog_frame"_s },
	{ wnd_style::border , "border"_s },
	{ wnd_style::caption , "caption"_s },
	{ wnd_style::overlapped_window , "overlapped_window"_s },
	{ wnd_style::maximize , "maximize"_s },
	{ wnd_style::clip_children , "clip_children"_s },
	{ wnd_style::clip_siblings , "clip_siblings"_s },
	{ wnd_style::disabled , "disabled"_s },
	{ wnd_style::shown , "shown"_s },
	{ wnd_style::minimize , "minimize"_s },
	{ wnd_style::child , "child"_s },
};

wstring wnd_style_t::to_string()const
{
	wstring out = "";
	wnd_style_t val = get();
	bool first = true;
	for (int i = algorithms::array_size(to_string_wnd_style_map) - 1; i >= 0; --i)
	{
		if (val == wnd_style::none)
			break;
		if (val & to_string_wnd_style_map[i].key)
		{
			if (first) {
				first = false;
				out << to_string_wnd_style_map[i].value;
			}
			else
				out << " + "_s << to_string_wnd_style_map[i].value;
			val -= to_string_wnd_style_map[i].key;
		}
	}
	return ang::move(out);
}

//wnd_style_t wnd_style_t::parse(text::raw_cstr_t str)
//{
//	cwstr_t key = as<cwstr_t>();
//	index idx = ang_binary_search<cwstr_t>(key, _parse_wnd_style_map);
//	if (idx < array_size(_parse_wnd_style_map))
//		return _parse_wnd_style_map[idx].value;
//	return wnd_style::none;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

safe_enum_rrti2(ang::platform::windows, wnd_style_ex);
safe_flags_implement(ang::platform::windows, wnd_style_ex);

static collections::pair<cstr_t, wnd_style_ex> _parse_wnd_style_ex_map[] =
{
	{ "accept_files"_s , wnd_style_ex::accept_files },
	{ "app_window"_s , wnd_style_ex::app_window },
	{ "client_edge"_s , wnd_style_ex::client_edge },
	{ "context_help"_s , wnd_style_ex::context_help },
	{ "control_parent"_s , wnd_style_ex::control_parent },
	{ "dlg_modal_frame"_s , wnd_style_ex::dlg_modal_frame },
	{ "left"_s , wnd_style_ex::left },
	{ "left_scrollbar"_s , wnd_style_ex::left_scrollbar },
	{ "ltr_reading"_s , wnd_style_ex::ltr_reading },
	{ "mdi_child"_s , wnd_style_ex::mdi_child },
	{ "no_parent_notify"_s , wnd_style_ex::no_parent_notify },
	{ "none"_s , wnd_style_ex::none },
	{ "overlapped_window"_s , wnd_style_ex::overlapped_window },
	{ "palette_window"_s , wnd_style_ex::palette_window },
	{ "right"_s , wnd_style_ex::right },
	{ "right_scrollbar"_s , wnd_style_ex::right_scrollbar },
	{ "rtl_reading"_s , wnd_style_ex::rtl_reading },
	{ "static_edge"_s , wnd_style_ex::static_edge },
	{ "tool_window"_s , wnd_style_ex::tool_window },
	{ "top_most"_s , wnd_style_ex::top_most },
	{ "transparent"_s , wnd_style_ex::transparent },
	{ "window_edge"_s , wnd_style_ex::window_edge }
};

static collections::pair<wnd_style_ex, cstr_t> to_string_wnd_style_ex_map[] =
{
	{ wnd_style_ex::none , "none"_s },
	{ wnd_style_ex::dlg_modal_frame , "dlg_modal_frame"_s },
	{ wnd_style_ex::no_parent_notify , "no_parent_notify"_s },
	{ wnd_style_ex::top_most , "top_most"_s },
	{ wnd_style_ex::accept_files , "accept_files"_s },
	{ wnd_style_ex::transparent , "transparent"_s },
	{ wnd_style_ex::mdi_child , "mdi_child"_s },
	{ wnd_style_ex::tool_window , "tool_window"_s },
	{ wnd_style_ex::window_edge , "window_edge"_s },
	{ wnd_style_ex::palette_window , "palette_window"_s },
	{ wnd_style_ex::client_edge , "client_edge"_s },
	{ wnd_style_ex::overlapped_window , "overlapped_window"_s },
	{ wnd_style_ex::context_help , "context_help"_s },
	{ wnd_style_ex::right , "right"_s },
	{ wnd_style_ex::rtl_reading , "rtl_reading"_s },
	{ wnd_style_ex::left_scrollbar , "left_scrollbar"_s },
	{ wnd_style_ex::control_parent , "control_parent"_s },
	{ wnd_style_ex::static_edge , "static_edge"_s },
	{ wnd_style_ex::app_window , "app_window" }
};


wstring wnd_style_ex_t::to_string()const
{
	wstring out = "";
	wnd_style_ex_t val = get();
	bool first = true;
	for (int i = algorithms::array_size(to_string_wnd_style_ex_map) - 1; i >= 0; --i)
	{
		if (val.m_value == 0)
			break;
		if (val & to_string_wnd_style_ex_map[i].key)
		{
			if (first) {
				first = false;
				out << to_string_wnd_style_ex_map[i].value;
			}
			else
				out << " + "_s << to_string_wnd_style_ex_map[i].value;
			val -= to_string_wnd_style_ex_map[i].key;
		}
	}
	return ang::move(out);
}

//template<> wnd_style_ex_t ang::xml::xml_value::as<wnd_style_ex_t>()const
//{
//	cwstr_t key = as<cwstr_t>();
//	index idx = ang_binary_search<cwstr_t>(key, _parse_wnd_style_ex_map);
//	if (idx < array_size(_parse_wnd_style_ex_map))
//		return _parse_wnd_style_ex_map[idx].value;
//	return wnd_style_ex::none;
//}