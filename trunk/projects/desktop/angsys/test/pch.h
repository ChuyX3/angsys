#pragma once

#define ANG_DYNAMIC_LIBRARY

#include <angsys.h>
#include <ang/dom/xml.h>
#include <ang/core/async.h>
#include <ang/platform/platform.h>
#include <ang/graphics/graphics.h>
#include <ang/framework/framework.h>
#include <ang/maths/vectors.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GLU.h>


#pragma comment(lib, "angsys.lib")
#pragma comment(lib, "platform.lib")
#pragma comment(lib, "graphics.lib")
#pragma comment(lib, "framework.lib")

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glew32s.lib")