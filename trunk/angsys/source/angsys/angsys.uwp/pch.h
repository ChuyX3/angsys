#pragma once

#include "targetver.h"

#include<winapifamily.h>

#include "compile_config.h"

#pragma comment(lib, "Ws2_32.lib")

extern "C" __declspec(dllexport) void ang_debug_output_verbose(const char* format, ...);
extern "C" __declspec(dllexport) void ang_debug_output_info(const char* format, ...);
extern "C" __declspec(dllexport) void ang_debug_output_warning(const char* format, ...);
extern "C" __declspec(dllexport) void ang_debug_output_error(const char* format, ...);