// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <angsys.h>
#include <ang/collections/vector.h>
#include <ang/collections/map.h>

#include <ang/platform/platform.h>
#include <ang/platform/angwin/angwin.h>

#include <ang/graphics/angraph.h>

#include <ang/maths/maths.h>
#include <ang/maths/vectors.h>

#include <ang/com_wrapper.h>

#include <d3d11.h>
#include <d3d11_2.h>
#include <d3dcompiler.h>

#pragma comment(lib, "angsys.lib")
#pragma comment(lib, "angraph.lib")
#pragma comment(lib, "angsys.platform.lib")

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


ANG_DECLARE_MEMORYMANAGER()
