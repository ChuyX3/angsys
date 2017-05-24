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


		struct win32_mutex
		{
			HANDLE _mutex;
			win32_mutex()
			{
				_mutex = CreateMutexExW(NULL, NULL, 0, SYNCHRONIZE);
			}
			~win32_mutex()
			{
				CloseHandle(_mutex);
			}

			bool lock(dword time = -1)const {
				return 0 == WaitForSingleObject(_mutex, time);
			}
			bool try_lock()const {
				return 0 == WaitForSingleObject(_mutex, 0);
			}
			bool unlock()const {
				return ReleaseMutex(_mutex) ? true : false;
			}
		};

		struct locker
		{
			win32_mutex & _mutex;
			locker(win32_mutex& m): _mutex(m){
				_mutex.lock();
			}
			~locker() {
				_mutex.unlock();
			}
		};

		class console_logger
		//	: public object
			: public ilogger
		{
		private:
			HANDLE _handle;
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
#endif

	}
}


#endif//_DEBUG/_DEVELOPPER
#endif//__DEBUG_H__