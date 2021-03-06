// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include "compile_config.h"

#include <angsys.hpp>
#include <ang/core/files.hpp>
#include <ang/platform/platform.hpp>
#include <ang/collections/vector.hpp>
#include <ang/com_wrapper.hpp>

#include <ang/collections/vector.hpp>
#include <ang/collections/map.hpp>


#pragma comment(lib, "angsys.lib")
#pragma comment(lib, "angsys.platform.lib")


#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <mfmediaengine.h>

namespace ang
{
	inline void throw_if_failed(HRESULT hr) { if (FAILED(hr)) throw exception_t((uint)hr, "ang::exception: COM ERROR"); }
}

#if DIRECTX11_SUPPORT
#include <d3d11.h>
#include <d3d11_2.h>
#include <d3dcompiler.h>

#include <d2d1_2.h>
#include <d2d1effects_1.h>
#include <dwrite_2.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")

#endif

#if OPENGL_SUPPORT
#if OPENGLES_SUPPORT
#include <EGL\egl.h>
#include <GLES2\gl2.h>
#else
#endif
#endif


//ANG_DECLARE_MEMORYMANAGER()

extern "C" void __declspec(dllimport) ang_debug_output_info(const char* format, ...);
extern "C" void __declspec(dllimport) ang_debug_output_warning(const char* format, ...);
extern "C" void __declspec(dllimport) ang_debug_output_error(const char* format, ...);



