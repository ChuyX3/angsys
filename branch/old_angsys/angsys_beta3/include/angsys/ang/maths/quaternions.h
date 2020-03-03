#ifndef __ANG_QUATERNIONS_MATHS_H__
#define __ANG_QUATERNIONS_MATHS_H__

#include <ang/maths/maths.h>
#include <ang/maths/vectors.h>

namespace ang
{
	namespace maths
	{
		namespace quaternions
		{
			typedef double3 space_vector;

			/********************************************************/
			/*                  quaternion definition               */
			/********************************************************/
			struct quaternion
			{
				union {
					double _vector[4];
					struct {
						double r;
						double i;
						double j;
						double k;
					};
				};

				quaternion(quaternion&&) = default;
				quaternion& operator = (quaternion&&) = default;

				inline quaternion();
				inline quaternion(space_vector const& xyz, double real = 0);
				inline quaternion(double real, double i, double j, double k);
				inline quaternion(const quaternion&);

				inline quaternion& operator = (const quaternion&);
				inline quaternion& operator += (const quaternion&);
				inline quaternion& operator -= (const quaternion&);
				inline quaternion& operator *= (double);
				inline quaternion& operator /= (double);

				template<uint> double get()const { throw(exception(except_code::array_overflow)); }
				template<uint> void set(double value) { throw(exception(except_code::array_overflow)); }

				explicit inline operator space_vector()const {
					return space_vector(i, j, k);
				}

				inline double operator [](uint)const;
				explicit inline operator double()const;//Length

				inline bool operator == (const quaternion&)const;//Comparing values
				inline bool operator != (const quaternion&)const;//Comparing values
				inline bool operator > (const quaternion&)const;//Comparing length
				inline bool operator < (const quaternion&)const;//Comparing length

				friend inline quaternion operator + (const quaternion&, const quaternion&);
				friend inline quaternion operator - (const quaternion&, const quaternion&);
				friend inline quaternion operator - (const quaternion&);

				friend inline quaternion operator ~ (const quaternion&);
				friend inline quaternion operator * (const quaternion&, double);
				friend inline quaternion operator * (double, const quaternion&);
				friend inline quaternion operator * (const quaternion&, const quaternion&);
				friend inline quaternion operator / (const quaternion&, double);
			};

			const quaternion e0 = { 0,0,0,0 };
			const quaternion e1 = { 0,1,0,0 };
			const quaternion e2 = { 0,0,1,0 };
			const quaternion e3 = { 0,0,0,1 };

			quaternion px(double theta) { return{ cos(theta / 2.0),sin(theta / 2.0),0,0 }; }
			quaternion py(double theta) { return{ cos(theta / 2.0),0,sin(theta / 2.0),0 }; }
			quaternion pz(double theta) { return{ cos(theta / 2.0),0,0,sin(theta / 2.0) }; }

		}

	}
}

#endif//__ANG_VECTOR_MATHS_H__
