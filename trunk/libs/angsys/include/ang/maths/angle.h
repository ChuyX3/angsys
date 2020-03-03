/*********************************************************************************************************************/
/*   File Name: ang/maths/angle.h                                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/


#ifndef __ANG_MATHS_ANGLE_H__
#define __ANG_MATHS_ANGLE_H__

#include <angsys.h>

namespace ang
{
	namespace maths
	{
		class angle;

		class angle
		{
		public:
			static angle from_radians(double value) { return angle(value); }
			static angle from_degrees(double value) { return angle(value * constants::pi / 180.0); }

		private:
			double m_value;

		public:
			angle() : m_value(0) {}
			explicit angle(double value) : m_value(value) {
				while (m_value > constants::pi) m_value -= (constants::pi * 2);
				while (m_value <= -constants::pi) m_value += (constants::pi * 2);
			}
			angle(const angle& other) : m_value(other.m_value) {
				while (m_value > constants::pi) m_value -= (constants::pi * 2);
				while (m_value <= -constants::pi) m_value += (constants::pi * 2);
			}

			double value()const { return m_value; }
			void value(double val) {
				m_value = val;
				while (m_value > constants::pi) m_value -= (constants::pi * 2);
				while (m_value <= -constants::pi) m_value += (constants::pi * 2);
			}

			angle operator - ()const { return -angle(m_value); }

			angle& operator = (angle const& other) {
				m_value = other.m_value;
				while (m_value > constants::pi) m_value -= (constants::pi * 2);
				while (m_value <= -constants::pi) m_value += (constants::pi * 2);
				return*this;
			}
			angle& operator += (angle const& other) {
				m_value += other.m_value;
				while (m_value > constants::pi) m_value -= (constants::pi * 2);
				while (m_value <= -constants::pi) m_value += (constants::pi * 2);
				return*this;
			}
			angle& operator -= (angle const& other) {
				m_value -= other.m_value;
				while (m_value > constants::pi) m_value -= (constants::pi * 2);
				while (m_value <= -constants::pi) m_value += (constants::pi * 2);
				return*this;
			}
			angle& operator *= (double const& other) {
				m_value *= other;
				while (m_value > constants::pi) m_value -= (constants::pi * 2);
				while (m_value <= -constants::pi) m_value += (constants::pi * 2);
				return*this;
			}
			angle& operator /= (double const& other) {
				m_value /= other;
				while (m_value > constants::pi) m_value -= (constants::pi * 2);
				while (m_value <= -constants::pi) m_value += (constants::pi * 2);
				return*this;
			}

			bool operator == (angle const& other)const { return m_value == other.m_value; }
			bool operator != (angle const& other)const { return m_value != other.m_value; }
			bool operator >= (angle const& other)const { return m_value >= other.m_value; }
			bool operator <= (angle const& other)const { return m_value <= other.m_value; }
			bool operator > (angle const& other)const { return m_value > other.m_value; }
			bool operator < (angle const& other)const { return m_value < other.m_value; }
		};

		inline angle operator + (angle const& a1, angle const& a2) { return angle(a1.value() + a2.value()); }
		inline angle operator - (angle const& a1, angle const& a2) { return angle(a1.value() - a2.value()); }
		inline angle operator * (angle const& a, double const& r) { return angle(a.value() * r); }
		inline angle operator * (double const& r, angle const& a) { return angle(a.value() * r); }
		inline angle operator / (angle const& a, double const& r) { return angle(a.value() / r); }

	}

	inline maths::angle operator "" _rad(long double value) { return maths::angle(value); }
	inline maths::angle operator "" _rad(unsigned long long value) { return maths::angle((double)value); }
	inline maths::angle operator "" _radian(long double value) { return maths::angle(value); }
	inline maths::angle operator "" _radian(unsigned long long value) { return maths::angle((double)value); }
	inline maths::angle operator "" _deg(long double value) { return maths::angle(value * maths::constants::pi / 180.0); }
	inline maths::angle operator "" _deg(unsigned long long value) { return maths::angle((double)value * maths::constants::pi / 180.0); }
	inline maths::angle operator "" _degree(long double value) { return maths::angle(value * maths::constants::pi / 180.0); }
	inline maths::angle operator "" _degree(unsigned long long value) { return maths::angle((double)value * maths::constants::pi / 180.0); }
}

#endif /*__ANG_MATHS_ANGLE_H__*/
