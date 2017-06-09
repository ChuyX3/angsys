// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include "compile_config.h"

extern void* operator new (size_t);
extern void operator delete (void*);
extern void* operator new[](size_t);
extern void operator delete[](void*);
extern void* operator new (size_t, const char*, int);
extern void* operator new[](size_t, const char*, int);
extern void operator delete (void*, const char*, int);
extern void operator delete[](void*, const char*, int);


extern "C" void __declspec(dllexport) ang_debug_output_info(const char* format, ...);
extern "C" void __declspec(dllexport) ang_debug_output_warning(const char* format, ...);
extern "C" void __declspec(dllexport) ang_debug_output_error(const char* format, ...);
