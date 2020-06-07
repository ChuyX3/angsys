#ifndef __ANG_FRAMEWORK_H__
#elif !defined __ANG_FRAMEWORK_ALIGN_H__
#define __ANG_FRAMEWORK_ALIGN_H__

namespace ang
{
	namespace framework
	{
		class LINK aligned 
			: public implement<aligned
			, iid("ang::framework::aligned")>
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
			aligned();
			virtual~aligned();
		};
	}
}

#endif/*__ANG_FRAMEWORK_ALIGN_H__*/
