#include "pch.h"
#include "angbase.h"
#include "ang_debugger.h"

#if defined _DEBUG || defined _DEVELOPPER
using namespace ang;
using namespace ang::debug;

ANG_IMPLEMENT_INTERFACE(ang::debug, ilogger);
ANG_IMPLEMENT_ENUM(ang::debug, log_level, byte, log_level::verbose);

#ifdef ANDROID_PLATFORM

android_logger::android_logger()
{
}

android_logger::~android_logger()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::debug::android_logger, ang::debug::ilogger);

void android_logger::print(log_level_t level, cstr_t format, ...)const
{
	va_list args;
	va_start(args, format);
	size_t size = _vscprintf(format, args);
	if (size >= 255)
	{
		char* buffer = (char*)malloc(size + 1);
		buffer[size] = 0;

		vsprintf_s(buffer, size + 1, format, args);
		va_end(args);

		switch (level.get())
		{
		case log_level::verbose:
			__android_log_print(ANDROID_LOG_VERBOSE, "angsys", "%s", buffer);
			break;
		case log_level::info:
			__android_log_print(ANDROID_LOG_INFO, "angsys", "%s", buffer);
			break;
		case log_level::warning:
			__android_log_print(ANDROID_LOG_WARN, "angsys", "%s", buffer);
			break;
		case log_level::error:
			__android_log_print(ANDROID_LOG_ERROR, "angsys", "%s", buffer);
			break;
		}
		free(buffer);
	}
	else
	{
		char buffer[255];
		buffer[size] = 0;

		vsprintf_s(buffer, size + 1, format, args);
		va_end(args);

		switch (level.get())
		{
		case log_level::verbose:
			__android_log_print(ANDROID_LOG_VERBOSE, "angsys", "%s", buffer);
			break;
		case log_level::info:
			__android_log_print(ANDROID_LOG_INFO, "angsys", "%s", buffer);
			break;
		case log_level::warning:
			__android_log_print(ANDROID_LOG_WARN, "angsys", "%s", buffer);
			break;
		case log_level::error:
			__android_log_print(ANDROID_LOG_ERROR, "angsys", "%s", buffer);
			break;
		}
	}
}

#endif//ANG_PLATFORM


#endif
