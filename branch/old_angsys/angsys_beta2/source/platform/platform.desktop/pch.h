// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define ANGYS_DYNAMIC_LIBRARY
#include "compile_config.h"
#include <windowsx.h>
#include <angsys.hpp>

#pragma comment(lib, "angsys.lib")

//ANG_DECLARE_MEMORYMANAGER()

extern "C" void __declspec(dllimport) ang_debug_output_info(const char* format, ...);
extern "C" void __declspec(dllimport) ang_debug_output_warning(const char* format, ...);
extern "C" void __declspec(dllimport) ang_debug_output_error(const char* format, ...);
