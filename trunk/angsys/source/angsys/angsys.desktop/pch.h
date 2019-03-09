// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include<winapifamily.h>

#include "compile_config.h"



extern "C" __declspec(dllexport) void ang_debug_output_verbose(const char* format, ...);
extern "C" __declspec(dllexport) void ang_debug_output_info(const char* format, ...);
extern "C" __declspec(dllexport) void ang_debug_output_warning(const char* format, ...);
extern "C" __declspec(dllexport) void ang_debug_output_error(const char* format, ...);

// TODO: reference additional headers your program requires here
