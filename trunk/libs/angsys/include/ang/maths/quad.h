#ifndef __COFFE_QUAD_MATHS_H__
#define __COFFE_QUAD_MATHS_H__

#include <coffe/maths/maths.h>

namespace coffe
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

#endif//__COFFE_QUAD_MATHS_H__
