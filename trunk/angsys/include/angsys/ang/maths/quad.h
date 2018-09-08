#ifndef __ANG_QUAD_MATHS_H__
#define __ANG_QUAD_MATHS_H__

#include <ang/maths/maths.h>

namespace ang
{
	namespace maths
	{
		class alignas(16) quad
		{
		private:
			union
			{
				struct
				{
					ulong64 low;
					long64 hi;
				};
				struct
				{
					uint w0;
					uint w1;
					uint w2;
					word w3;
					word w4;
				};
			};
		};
	}
}

#endif//__ANG_QUAD_MATHS_H__
