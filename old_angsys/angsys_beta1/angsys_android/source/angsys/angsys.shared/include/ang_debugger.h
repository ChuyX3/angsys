#ifndef __DEBUG_H__
#define __DEBUG_H__
#if defined _DEBUG || defined _DEVELOPPER

#include <angsys.h>

namespace ang
{
	namespace debug
	{
#ifdef WINDOWS_PLATFORM


		ANG_BEGIN_FLAGS(, console_colors, byte)
			black = 0,
			blue = 0X1,
			green = 0X2,
			red = 0X4,
			intencity = 0X8,
		ANG_END_FLAGS(console_colors);

		union console_property
		{
			word flags;
			struct
			{
				byte foreground;
				byte background;
			};

		};


		


		class console_logger
			//	: public object
			: public ilogger
		{
		private:
			HANDLE _handle;
			HANDLE _error_handle;
		public:
			console_logger(cstr_t name);

		public:
			virtual ~console_logger();

		public:
			virtual void print(log_level_t, cstr_t, ...)const override;
		};

		class file_logger
			: public object
			, public ilogger
		{
		private:
			log_level_t _log_level;
			HANDLE _handle;
		public:
			file_logger(cstr_t file);

		private:
			virtual ~file_logger();

		public:
			virtual void print(log_level_t, cstr_t, ...)const override;
		};

#elif defined ANDROID_PLATFORM

		class android_logger
			: public ilogger
		{
		public:
			android_logger();
			virtual ~android_logger();

			ANG_DECLARE_INTERFACE();

		public:
			virtual void print(log_level_t, cstr_t, ...)const override;
		};

		typedef android_logger platform_logger;
#endif

	}
}


#endif//_DEBUG/_DEVELOPPER
#endif//__DEBUG_H__