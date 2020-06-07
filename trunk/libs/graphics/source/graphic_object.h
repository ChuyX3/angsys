#pragma once
#include <ang/graphics/graphics.h>

namespace ang
{
	namespace graphics
	{
		class graphic_object /*abstract*/
			: public implement<graphic_object
			, iid("ang::graphics::graphic_object")>
		{
		public:
			pointer operator new(wsize sz) {
				return object::operator new(sz, 16U);
			}

#ifdef _DEBUG
			pointer operator new(wsize sz, const char*, int) {
				return object::operator new(sz, 16U);
			}
#endif//_DEBUG

		protected:
			graphic_object();
			virtual void dispose() = 0;
		};

		template<typename T, typename IID, typename...Ts>
		using graphic = implement<T, IID, graphic_object, Ts...>;
	}

	//COFFE_DECLARE_CLASS_INFO_OVERRIDE(, graphics::graphic_object);
}
