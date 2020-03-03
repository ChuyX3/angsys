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

#ifndef __ANG_MATHS_COMPLEX_H__
#define __ANG_MATHS_COMPLEX_H__

#include <ang/maths/angle.h>

namespace ang
{
	namespace maths
	{
		class real;
		class imaginary;
		class complex;

		using number = anyone<real, imaginary, complex>;

		real operator + (real const& n1, real const& n2);
		real operator - (real const& n1, real const& n2);
		complex operator + (real const& n1, imaginary const& n2);
		complex operator - (real const& n1, imaginary const& n2);
		real operator * (real const& n1, real const& n2);
		real operator / (real const& n1, real const& n2);
		imaginary operator * (real const& n1, imaginary const& n2);
		imaginary operator / (real const& n1, imaginary const& n2);
		imaginary operator + (imaginary const& n1, imaginary const& n2);
		imaginary operator - (imaginary const& n1, imaginary const& n2);
		complex operator + (imaginary const& n1, real const& n2);
		complex operator - (imaginary const& n1, real const& n2);
		real operator * (imaginary const& n1, imaginary const& n2);
		real operator / (imaginary const& n1, imaginary const& n2);
		imaginary operator * (imaginary const& n2, real const& n1);
		imaginary operator / (imaginary const& n2, real const& n1);
		complex operator + (complex const& n1, real const& n2);
		complex operator - (complex const& n1, real const& n2);
		complex operator + (complex const& n1, imaginary const& n2);
		complex operator - (complex const& n1, imaginary const& n2);
		complex operator + (real const& n1, complex const& n2);
		complex operator - (real const& n1, complex const& n2);
		complex operator + (imaginary const& n1, complex const& n2);
		complex operator - (imaginary const& n1, complex const& n2);
		complex operator + (complex const& n1, complex const& n2);
		complex operator - (complex const& n1, complex const& n2);
		complex operator * (complex const& n1, real const& n2);
		complex operator * (real const& n1, complex const& n2);
		complex operator * (complex const& n1, imaginary const& n2);
		complex operator * (imaginary const& n1, complex const& n2);
		complex operator / (complex const& n1, real const& n2);
		complex operator / (complex const& n1, imaginary const& n2);
		complex operator * (complex const& n1, complex const& n2);
		complex operator / (complex const& n1, complex const& n2);
		complex operator / (real const& n1, complex const& n2);
		complex operator / (imaginary const& n1, complex const& n2);

		class real
		{
		private:
			double m_value;

		public:
			real() : m_value(0) {}
			real(double value) : m_value(value) {}
			real(const real& other) : m_value(other.m_value) {}

			double value()const { return m_value; }
			void value(double val) { m_value = val; }

			real operator - ()const { return -m_value; }

			real& operator = (real const& other) {
				m_value = other.m_value;
				return*this;
			}
			real& operator += (real const& other) {
				m_value += other.m_value;
				return*this;
			}
			real& operator -= (real const& other) {
				m_value -= other.m_value;
				return*this;
			}
			real& operator *= (real const& other) {
				m_value *= other.m_value;
				return*this;
			}
			real& operator /= (real const& other) {
				m_value /= other.m_value;
				return*this;
			}

			bool operator == (real const& other)const { return m_value == other.m_value; }
			bool operator != (real const& other)const { return m_value != other.m_value; }
			bool operator >= (real const& other)const { return m_value >= other.m_value; }
			bool operator <= (real const& other)const { return m_value <= other.m_value; }
			bool operator > (real const& other)const { return m_value > other.m_value; }
			bool operator < (real const& other)const { return m_value < other.m_value; }
		};

		class imaginary
		{
		private:
			double m_value;

		public:
			imaginary() : m_value(0) {}
			explicit imaginary(double value) : m_value(value) {}
			imaginary(const imaginary& other) : m_value(other.m_value) {}

			double value()const { return m_value; }
			void value(double val) { m_value = val; }

			imaginary operator - ()const { return imaginary(-m_value); }

			imaginary& operator = (imaginary const& other) {
				m_value = other.m_value;
				return*this;
			}
			imaginary& operator += (imaginary const& other) {
				m_value += other.m_value;
				return*this;
			}
			imaginary& operator -= (imaginary const& other) {
				m_value -= other.m_value;
				return*this;
			}

			bool operator == (imaginary const& other)const { return m_value == other.m_value; }
			bool operator != (imaginary const& other)const { return m_value != other.m_value; }
			bool operator >= (imaginary const& other)const { return m_value >= other.m_value; }
			bool operator <= (imaginary const& other)const { return m_value <= other.m_value; }
			bool operator > (imaginary const& other)const { return m_value > other.m_value; }
			bool operator < (imaginary const& other)const { return m_value < other.m_value; }
		};

		class complex
		{
		public:
			static complex  pow(complex const& c, unsigned int p) { return c.pow(p); }

			static complex  pow(complex const& c, complex const& p) { return c.pow(p); }

		private:
			real m_real;
			imaginary m_imag;

		public:
			complex() : m_real(0), m_imag(0) {}
			complex(real const& r) : m_real(r), m_imag(0) {}
			complex(imaginary const& i) : m_real(0), m_imag(i) {}
			complex(real const& r, imaginary const& i) : m_real(r), m_imag(i) {}
			complex(real const& mod, angle const& arg) : m_real(0), m_imag(0) {
				m_real.value(mod.value() * cos(arg.value()));
				m_imag.value(mod.value() * sin(arg.value()));
			}
			complex(const complex& other) : m_real(other.m_real), m_imag(other.m_imag) {}

			maths::real real()const { return m_real; }
			maths::real real(double val) { m_real.value(val); }
			maths::imaginary imaginary()const { return m_imag; }
			maths::imaginary imaginary(double val) { m_imag.value(val); }

			inline maths::real module()const {
				return sqrt(m_real.value() * m_real.value() + m_imag.value() * m_imag.value());
			}

			inline angle argument()const {
				if (m_real == 0)
					return 90_deg;
				else if (m_real < 0)
					return angle(constants::pi + atan(m_imag.value() / m_real.value()));
				else
					return angle(atan(m_imag.value() / m_real.value()));
			}

			inline complex pow(unsigned int p)const {
				double mod = ::pow(m_real.value() * m_real.value() + m_imag.value() * m_imag.value(), double(p) / 2.0);
				angle ang = angle(argument().value() * p);
				return complex(mod * cos(ang.value()), maths::imaginary(mod * sin(ang.value())));
			}

			inline complex pow(complex const& p)const {
				double r = module().value();
				double th = argument().value();
				double x = p.real().value();
				double y = p.imaginary().value();

				double r2 = maths::pow(r, x) / maths::exp(th * y);
				double th2 = y * maths::log(r) + th * x;
				return complex(r2, angle(th2));
			}

			complex& operator = (complex const& other) {
				m_real = other.m_real;
				m_imag = other.m_imag;
				return*this;
			}
			complex& operator += (complex const& other) {
				m_real += other.m_real;
				m_imag = other.m_imag;
				return*this;
			}
			complex& operator -= (complex const& other) {
				m_real -= other.m_real;
				m_imag -= other.m_imag;
				return*this;
			}
		};


		inline real operator + (real const& n1, real const& n2) { return n1.value() + n2.value(); }
		inline real operator - (real const& n1, real const& n2) { return n1.value() - n2.value(); }
		inline complex operator + (real const& n1, imaginary const& n2) { return complex(n1, n2); }
		inline complex operator - (real const& n1, imaginary const& n2) { return complex(n1, -n2); }
		inline real operator * (real const& n1, real const& n2) { return n1.value() * n2.value(); }
		inline real operator / (real const& n1, real const& n2) { return n1.value() / n2.value(); }
		inline imaginary operator * (real const& n1, imaginary const& n2) { return imaginary(n1.value() * n2.value()); }
		inline imaginary operator / (real const& n1, imaginary const& n2) { return imaginary(-n1.value() / n2.value()); }

		inline imaginary operator + (imaginary const& n1, imaginary const& n2) { return imaginary(n1.value() + n2.value()); }
		inline imaginary operator - (imaginary const& n1, imaginary const& n2) { return imaginary(n1.value() - n2.value()); }
		inline complex operator + (imaginary const& n1, real const& n2) { return complex(n2, n1); }
		inline complex operator - (imaginary const& n1, real const& n2) { return complex(-n2, n1); }
		inline real operator * (imaginary const& n1, imaginary const& n2) { return -n1.value() * n2.value(); }
		inline real operator / (imaginary const& n1, imaginary const& n2) { return n1.value() / n2.value(); }
		inline imaginary operator * (imaginary const& n2, real const& n1) { return imaginary(n1.value() * n2.value()); }
		inline imaginary operator / (imaginary const& n2, real const& n1) { return imaginary(n1.value() / n2.value()); }

		inline complex operator + (complex const& n1, real const& n2) { return{ n1.real() + n2, n1.imaginary() }; }
		inline complex operator - (complex const& n1, real const& n2) { return{ n1.real() - n2, n1.imaginary() }; }
		inline complex operator + (complex const& n1, imaginary const& n2) { return{ n1.real() , n1.imaginary() + n2 }; }
		inline complex operator - (complex const& n1, imaginary const& n2) { return{ n1.real() , n1.imaginary() - n2 }; }
		inline complex operator + (real const& n1, complex const& n2) { return{ n1 + n2.real(),  n2.imaginary() }; }
		inline complex operator - (real const& n1, complex const& n2) { return{ n1 - n2.real(), -n2.imaginary() }; }
		inline complex operator + (imaginary const& n1, complex const& n2) { return{ n2.real(), n1 + n2.imaginary() }; }
		inline complex operator - (imaginary const& n1, complex const& n2) { return{ -n2.real(), n1 - n2.imaginary() }; }
		inline complex operator + (complex const& n1, complex const& n2) { return{ n1.real() + n2.real(), n1.imaginary() + n2.imaginary() }; }
		inline complex operator - (complex const& n1, complex const& n2) { return{ n1.real() - n2.real(), n1.imaginary() - n2.imaginary() }; }


		inline complex operator * (complex const& n1, real const& n2) { return{ n1.real() * n2, n1.imaginary() * n2 }; }
		inline complex operator * (real const& n1, complex const& n2) { return{ n2.real() * n1, n2.imaginary() * n1 }; }
		inline complex operator * (complex const& n1, imaginary const& n2) { return{ n1.imaginary() * n2, n1.real() * n2 }; }
		inline complex operator * (imaginary const& n1, complex const& n2) { return{ n1 * n2.imaginary(), n1 * n2.real() }; }

		inline complex operator / (complex const& n1, real const& n2) { return{ n1.real() / n2, n1.imaginary() / n2 }; }
		inline complex operator / (complex const& n1, imaginary const& n2) { return{ n1.imaginary() / n2, n1.real() / n2 }; }

		inline complex operator * (complex const& n1, complex const& n2) { return n1.real() * n2.real() + n1.imaginary() * n2.imaginary() + n1.real() * n2.imaginary() + n1.imaginary() * n2.real(); }
		inline complex operator / (complex const& n1, complex const& n2) { return (n1 * complex{ n2.real(), -n2.imaginary() }) / (n2.real() * n2.real() - n2.imaginary() * n2.imaginary()); }

		inline complex operator / (real const& n1, complex const& n2) { return (n1 * complex{ n2.real(), -n2.imaginary() }) / (n2.real() * n2.real() - n2.imaginary() * n2.imaginary()); }
		inline complex operator / (imaginary const& n1, complex const& n2) { return (n1 * complex{ n2.real(), -n2.imaginary() }) / (n2.real() * n2.real() - n2.imaginary() * n2.imaginary()); }
	}

	inline maths::real operator "" _r(long double value) { return maths::real(value); }
	maths::real operator "" _r(unsigned long long value) { return maths::real((double)value); }

	inline maths::imaginary operator "" _i(long double value) { return maths::imaginary(value); }
	inline maths::imaginary operator "" _i(unsigned long long value) { return maths::imaginary((double)value); }
}

#endif /*__ANG_MATHS_COMPLEX_H__*/
