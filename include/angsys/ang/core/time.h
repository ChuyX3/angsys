/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_TIME_H__
#define __ANG_TIME_H__

#include <angsys.h>

#ifdef LINK
#undef LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY
#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

namespace ang
{
	namespace core
	{
		namespace time
		{
			class timer;
			class delta_time;

			LINK double get_performance_time();
		}
	}
}

namespace ang
{

	namespace core
	{
		namespace time
		{
			class LINK timer
			{
			private:
				double _total;
				double _last_time;
				double _curr_time;

			public:
				timer();
				~timer();

			public: //Properties
				double total()const;
				double delta()const;

			public: //Methods
				void update();
				void reset();
			};

		}
	}
}

ANG_REGISTER_RUNTIME_TYPENAME(ang::core::time::timer)

#ifdef LINK
#undef LINK
#endif//LINK
#endif//__ANG_TIME_H__