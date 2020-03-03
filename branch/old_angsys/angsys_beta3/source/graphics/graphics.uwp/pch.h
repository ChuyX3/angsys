// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define ANGSYS_DYNAMIC_LIBRARY

#include "compile_config.h"

#include <angsys.h>
#pragma comment(lib, "angsys.lib")

#include <wrl.h>
#include <dxgi.h>
#include <..\winrt\WinRTBase.h>
#include <windows.graphics.holographic.h>
#include <..\um\HolographicSpaceInterop.h>
#include <Windows.Graphics.DirectX.Direct3D11.interop.h>

// TODO: reference additional headers your program requires here
