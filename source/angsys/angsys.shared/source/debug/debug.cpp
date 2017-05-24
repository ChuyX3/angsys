/*********************************************************************************************************************/
/*   File Name: deubg.cpp                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "debug.h"

#if defined _DEBUG || defined _DEVELOPPER

using namespace ang;
using namespace ang::debug;

ANG_IMPLEMENT_INTERFACE(ang::debug, ilogger);
ANG_IMPLEMENT_ENUM(ang::debug, log_level, byte, log_level::none);
ANG_IMPLEMENT_FLAGS(ang::debug, console_colors, byte);


struct KernelModule
{
	HMODULE hModule;
	KernelModule() {
		MEMORY_BASIC_INFORMATION mbi = { 0 };
		VirtualQuery(VirtualQuery, &mbi, sizeof(mbi));
		hModule = reinterpret_cast<HMODULE>(mbi.AllocationBase);
	}
};

static HMODULE GetKernelModule()
{
	static KernelModule hkernelModule;
	return hkernelModule.hModule;
}

template<typename T>
struct kernel_proc;

template<typename return_t, typename... args_t>
struct kernel_proc<return_t(args_t...)>
{
	typedef return_t(WINAPI*type)(args_t...);
	static auto get(const char* name)->type {
		return type(GetProcAddress(GetKernelModule(), name));
	}
};


#ifdef WINDOWS_PLATFORM
console_logger::console_logger(cstr_t name)
	: _handle(INVALID_HANDLE_VALUE)
{
	auto AllocConsole = kernel_proc<BOOL(void)>::get("AllocConsole");

	if (AllocConsole && AllocConsole())
	{
		auto GetStdHandle = kernel_proc<HANDLE(DWORD)>::get("GetStdHandle");
		_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (_handle != INVALID_HANDLE_VALUE)
		{
			auto SetConsoleScreenBufferSize = kernel_proc<BOOL(HANDLE, COORD)>::get("SetConsoleScreenBufferSize");
			auto SetConsoleTitleA = kernel_proc<BOOL(LPCSTR)>::get("SetConsoleTitleA");
			if (SetConsoleScreenBufferSize != NULL)
			{
				COORD newSize = { 200, 500 };
				SetConsoleScreenBufferSize(_handle, newSize);
			}
			if (name.cstr() && SetConsoleTitleA != NULL)
			{
				SetConsoleTitleA(name);
			}
			auto AttachConsole = kernel_proc<BOOL(DWORD)>::get("AttachConsole");
			AttachConsole(GetCurrentProcessId());
			freopen("CON", "w", stdout);
			freopen("CON", "r", stdin);
		}
	}
}

console_logger::~console_logger()
{
	auto FreeConsole = kernel_proc<BOOL(void)>::get("FreeConsole");
	if (FreeConsole)FreeConsole();
	_handle = INVALID_HANDLE_VALUE;
}

void console_logger::print(log_level_t level, cstr_t format, ...)const
{
	if (_handle == INVALID_HANDLE_VALUE)
		return;
	va_list args;
	va_start(args, format);

	static auto SetConsoleTextAttribute = kernel_proc<BOOL(HANDLE, WORD)>::get("SetConsoleTextAttribute");
	if (SetConsoleTextAttribute)
	{
		console_property prop;

		switch (level)
		{
		case log_level::info:
			prop.background = 0;
			prop.foreground = (console_colors::intencity + console_colors::green).value();
			break;
		case log_level::warning:
			prop.background = 0;
			prop.foreground = (console_colors::intencity + console_colors::green + console_colors::red).value();
			break;
		case log_level::error:
			prop.background = 0;
			prop.foreground = (console_colors::intencity + console_colors::red).value();
			break;
		case log_level::none:
		default:
			prop.background = 0;
			prop.foreground = (console_colors::blue + console_colors::green + console_colors::red).value();
			break;
		}
		SetConsoleTextAttribute(_handle, prop.flags);
	}

	size_t size = _vscprintf(format, args);
	if (size <= 0)
		return;
	if (size >= 300)
	{
		char *buffer = new char[size + 1];
		vsprintf_s(buffer, size + 1, format, args);
		DWORD written;
		WriteFile(_handle, buffer, size, &written, NULL);
		delete[]buffer;
	}
	else
	{
		char buffer[300];
		vsprintf_s(buffer, 300, format, args);
		DWORD written;
		WriteFile(_handle, buffer, size, &written, NULL);
	}
	va_end(args);


	if (SetConsoleTextAttribute)
	{
		console_property prop;
		prop.background = 0;
		prop.foreground = (console_colors::blue + console_colors::green + console_colors::red).value();
		SetConsoleTextAttribute(_handle, prop.flags);
	}
}






#endif



#endif//_DEBUG/_DEVELOPPER
