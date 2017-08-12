//
// pch.h
// Header for standard system include files.
//
// Used by the build system to generate the precompiled header. Note that no
// pch.cpp is needed and the pch.h is automatically included in all cpp files
// that are part of the project
//

#include <angsys.h>

//#include <string.h>
//#include <unistd.h>
//#include <sys/resource.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include "android_native_app_glue.h"

#include <ang/collections/vector.h>

#define ANG_LOGV(...) ang_debug_output_verbose(__VAR_ARGS__)
#define ANG_LOGI(...) ang_debug_output_info(__VAR_ARGS__)
#define ANG_LOGW(...) ang_debug_output_warning(__VAR_ARGS__)
#define ANG_LOGE(...) ang_debug_output_error(__VAR_ARGS__)

extern "C" void ang_debug_output_verbose(const char* format, ...);
extern "C" void ang_debug_output_info(const char* format, ...);
extern "C" void ang_debug_output_warning(const char* format, ...);
extern "C" void ang_debug_output_error(const char* format, ...);

using namespace ang;
