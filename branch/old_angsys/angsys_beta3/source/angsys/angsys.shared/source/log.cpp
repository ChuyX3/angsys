#include "pch.h"
#include <angsys.h>

extern "C" void ang_debug_output_verbose(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;

	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);

#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_VERBOSE, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif
#endif
	free(buffer);
}

extern "C" void ang_debug_output_info(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;

	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);

#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_INFO, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif
#endif
	free(buffer);
}

extern "C" void ang_debug_output_warning(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;


	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);

#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_WARN, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	printf("%s %s\n", "angsys", buffer);
	//OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif
#endif
	free(buffer);
}

extern "C" void ang_debug_output_error(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	size_t size = _vscprintf(format, argList);

	char* buffer = (char*)malloc(size + 1);
	buffer[size] = 0;


	vsprintf_s(buffer, size + 1, format, argList);
	va_end(argList);


#if defined ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_ERROR, "angsys", "%s", buffer);
#elif defined WINDOWS_PLATFORM && WINDOWS_PLATFORM == WINDOWS_DESKTOP_PLATFORM
#if defined _DEBUG || defined _DEVELOPPER
	printf("%s %s\n", "angsys", buffer);
	OutputDebugStringA("angsys");
	OutputDebugStringA(buffer);
	OutputDebugStringA("\n");
#endif
#endif
	free(buffer);
}

