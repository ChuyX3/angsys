// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <angsys.h>
#include <ang/collections/vector.h>
#include <ang/collections/map.h>
#include <ang/core/time.h>

#include <ang/platform/platform.h>
#include <ang/platform/angwin/angwin.h>

#include <ang/graphics/angraph.h>
#include <ang/graphics/drawing.h>

#include <ang/maths/maths.h>
#include <ang/maths/vectors.h>

#include <ang/com_wrapper.h>

#pragma comment(lib, "angsys.lib")
#pragma comment(lib, "angraph.lib")
#pragma comment(lib, "angsys.platform.lib")

#include <Xinput.h>
#pragma comment(lib,"xinput.lib")

ANG_DECLARE_MEMORYMANAGER()


extern "C" void __declspec(dllimport) ang_debug_output_info(const char* format, ...);
extern "C" void __declspec(dllimport) ang_debug_output_warning(const char* format, ...);
extern "C" void __declspec(dllimport) ang_debug_output_error(const char* format, ...);