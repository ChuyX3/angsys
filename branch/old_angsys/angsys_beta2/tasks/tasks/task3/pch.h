// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include<winapifamily.h>

#define ANG_NO_RUNTIME_ERRORS
#define ANGSYS_DYNAMIC_LIBRARY
#include <angsys.hpp>
#include <ang/platform/angwin/angwin.hpp>
#include <ang/graphics/angraph.hpp>

#pragma comment(lib, "angsys.lib")
#pragma comment(lib, "angraph.lib")
#pragma comment(lib, "angsys.platform.lib")
// TODO: reference additional headers your program requires here

#define _USE_MATH_DEFINES
#include <math.h>

#include <Xinput.h>
#pragma comment(lib,"xinput.lib")

//#define VK_NO_PROTOTYPES
//#include <vulkan\vulkan.h>