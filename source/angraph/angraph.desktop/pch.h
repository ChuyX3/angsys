// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include "compile_config.h"

#include <angsys.h>
#include <ang/core/files.h>
#include <ang/platform/platform.h>
#include <ang/collections/vector.h>
#include <ang/com_wrapper.h>

#include <ang/collections/vector.h>
#include <ang/collections/map.h>


#pragma comment(lib, "angsys.lib")
#pragma comment(lib, "angsys.platform.lib")



#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <mfmediaengine.h>

namespace ang
{
	inline void throw_if_failed(HRESULT hr) { if (FAILED(hr)) throw new exception((uint)hr, "ang::exception: COM ERROR"); }
}


#include <d3d11.h>
#include <d3d11_2.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


ANG_DECLARE_MEMORYMANAGER()

// TODO: reference additional headers your program requires here
