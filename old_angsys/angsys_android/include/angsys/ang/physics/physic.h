#ifndef __ANG_PHYSIC_H__
#define __ANG_PHYSIC_H__

#include <angsys.h>
#include <ang/maths.h>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY
#ifdef ANG_PHYSIC_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EX_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

#define ANG_PHYSIC_UNITS_DECLARE_BASE_UNIT(UNIT_TYPE, _SYMBOL) \
	ang::physic::units::unit<UNIT_TYPE> operator "" _SYMBOL(long double value) { return value; } \
	ang::physic::units::unit<UNIT_TYPE> operator "" _SYMBOL(unsigned long long int value) { return (double)value; }

#define ANG_PHYSIC_UNITS_DECLARE_UNIT_CONVERSION(UNIT_TYPE, _SYMBOL, _FACTOR) \
	ang::physic::units::unit<UNIT_TYPE> operator "" _SYMBOL(long double value) { return value * (double)(_FACTOR); } \
	ang::physic::units::unit<UNIT_TYPE> operator "" _SYMBOL(unsigned long long int value) { return (double)value * (double)(_FACTOR); }

namespace ang
{
	namespace physic
	{
		namespace units
		{
			ANG_BEGIN_ENUM(LINK, unit_type, uint)
				none,
				time,

				distance,
				area,
				volume,

				speed,
				acceleration,

				ANG_END_ENUM(unit_type);

			class algorithms;
			template<uint UNIT_TYPE> struct unit
			{
				double _value;
				unit(double value) :_value(value) {}
				unit(const unit& value) : _value(value._value) { }
				inline double get()const { return _value; }
				inline void set(double value) { _value = value; }

				inline friend unit operator + (unit const& a0, unit const& a1) { return a0._value + a1._value; }
				inline friend unit operator - (unit const& a0, unit const& a1) { return a0._value - a1._value; }
				inline friend unit operator * (unit const& a0, double a1) { return a0._value * a1; }
				inline friend unit operator * (double a0, unit const& a1) { return a0 * a1._value; }
				inline friend unit operator / (unit const& a0, double a1) { return a0._value / a1; }
			};

			typedef unit<unit_type::time> time_t;
			typedef unit<unit_type::distance> distance_t;
			typedef unit<unit_type::area> area_t;
			typedef unit<unit_type::volume> volume_t;

			typedef unit<unit_type::speed> speed_t;
			typedef unit<unit_type::acceleration> acceleration_t;

			area_t operator * (distance_t const& a0, distance_t const& a1) { return a0._value * a1._value; }
			double operator / (distance_t const& a0, distance_t const& a1) { return a0._value * a1._value; }

			volume_t operator * (area_t const& a0, distance_t const& a1) { return a0._value * a1._value; }
			volume_t operator * (distance_t const& a0, area_t const& a1) { return a0._value * a1._value; }
			volume_t operator * (area_t const& a0, distance_t const& a1) { return a0._value * a1._value; }

			area_t operator * (distance_t const& a0, distance_t const& a1) { return a0._value * a1._value; }
			volume_t operator * (area_t const& a0, distance_t const& a1) { return a0._value * a1._value; }
			volume_t operator * (distance_t const& a0, area_t const& a1) { return a0._value * a1._value; }
		
		}
	}
}

ANG_PHYSIC_UNITS_DECLARE_BASE_UNIT(ang::physic::units::unit_type::time, _S);
ANG_PHYSIC_UNITS_DECLARE_UNIT_CONVERSION(ang::physic::units::unit_type::time, _mS, 0.001);
ANG_PHYSIC_UNITS_DECLARE_UNIT_CONVERSION(ang::physic::units::unit_type::time, _uS, 0.000001);
ANG_PHYSIC_UNITS_DECLARE_UNIT_CONVERSION(ang::physic::units::unit_type::time, _nS, 0.000000001);

namespace ang
{
	namespace physic
	{



	}
}

#endif///__ANG_PHYSIC_H__