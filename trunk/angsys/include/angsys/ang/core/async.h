#ifndef __ANG_CORE_ASYNC_H__
#define __ANG_CORE_ASYNC_H__

#include <angsys.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#else
#define LINK
#endif


namespace ang
{
	namespace core
	{
		namespace async
		{
			template<typename T> struct iasync;
			template<typename T> struct ioperation;
			template<typename T> class task_handler;

			typedef shared_ptr<cond> cond_ptr_t;
			typedef shared_ptr<mutex> mutex_ptr_t;

			ang_object(thread);
			ang_interface(itask);
			ang_interface(icore_thread);
			ang_interface(idispatcher);
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#endif//__ANG_CORE_ASYNC_H__