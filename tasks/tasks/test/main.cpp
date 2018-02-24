// test.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <type_traits>

#include <emmintrin.h>  

#include <mutex>

using namespace ang;

inline __m128i shift_left_128(__m128i const& x, int n)
{
	__m128i x1, x2;
	if ((n) >= 64) {
		x1 = _mm_slli_si128(x, 8);
		x1 = _mm_slli_epi64(x1, (n)-64);
	}
	else {
		x1 = _mm_slli_epi64(x, n);
		x2 = _mm_slli_si128(x, 8);
		x2 = _mm_srli_epi64(x2, 64 - (n));
		x1 = _mm_or_si128(x1, x2);
	}
	return x1;
}
inline __m128i shift_right_128(__m128i const& x, int n)
{
	__m128i x1, x2;
	if ((n) >= 64) {
		x1 = _mm_srli_si128(x, 8);
		x1 = _mm_srli_epi64(x1, (n)-64);
	}
	else {
		x1 = _mm_srli_epi64(x, n);
		x2 = _mm_srli_si128(x, 8);
		x2 = _mm_slli_epi64(x2, 64 - (n));
		x1 = _mm_or_si128(x1, x2);
	}
	return x1;
}


typedef struct _int128 {
	_int128() {
		_value = _mm_set1_epi32(0);
	}
	_int128(int32_t value) {
		if (value < 0)
			_value = _mm_setr_epi64x(value, -1);
		else
			_value = _mm_setr_epi64x(value, 0);
	}
	_int128(int64_t value) {
		if (value < 0)		
			_value = _mm_setr_epi64x(value, -1);	
		else
			_value = _mm_setr_epi64x(value, 0);
	}
	_int128(uint32_t value) {
		_value = _mm_setr_epi64x(value, 0);
	}
	_int128(uint64_t value) {
		_value = _mm_setr_epi64x(value, 0);
	}
	_int128(_int128 const& value) {
		_value = value._value;
	}
	_int128(__m128i const& value) {
		_value = value;
	}

	int64_t& loword() { return _value.m128i_i64[0]; }
	int64_t& hiword() { return _value.m128i_i64[1]; }

	int64_t const& loword()const { return _value.m128i_i64[0]; }
	int64_t const& hiword()const { return _value.m128i_i64[1]; }

	inline _int128& operator = (int32_t value) {
		if (value < 0)
			_value = _mm_setr_epi64x(value, -1);
		else
			_value = _mm_setr_epi64x(value, 0);
		return*this;
	}
	inline _int128& operator = (int64_t value) {
		if (value < 0)
			_value = _mm_setr_epi64x(value, -1);
		else
			_value = _mm_setr_epi64x(value, 0);
		return*this;
	}
	inline _int128& operator = (uint32_t value) {
		_value = _mm_setr_epi64x(value, 0);
		return*this;
	}
	inline _int128& operator = (uint64_t value) {
		_value = _mm_setr_epi64x(value, 0);
		return*this;
	}
	inline _int128& operator = (_int128 const& value) {
		_value = value._value;
		return*this;
	}
	inline _int128& operator = (__m128i const& value) {
		_value = value;
		return*this;
	}
	inline _int128& operator &= (_int128 const& value) {
		_value = _mm_and_si128(_value, value._value);
		return*this;
	}
	inline _int128& operator |= (_int128 const& value) {
		_value = _mm_or_si128(_value, value._value);
		return*this;
	}
	inline _int128& operator ^= (_int128 const& value) {
		_value = _mm_xor_si128(_value, value._value);
		return*this;
	}
	inline _int128& operator += (_int128 const& value) {
		__m128i carry, y = value._value;
		do {
			carry = _mm_and_si128(_value, y);
			_value = _mm_xor_si128(_value, y);
			y = shift_left_128(carry, 1);
		} while (!_mm_testz_si128(y, y));
		return*this;
	}
	inline _int128& operator -= (_int128 const& value) {
		__m128i carry, y = value._value;
		do {
			carry = _mm_andnot_si128(_value, y);
			_value = _mm_xor_si128(_value, y);
			y = shift_left_128(carry, 1);
		} while (!_mm_testz_si128(y, y));
		return*this;
	}
	inline _int128& operator <<= (int n) {
		__m128i v2;
		if ((n) >= 64)
		{
			_value = _mm_slli_si128(_value, 8);
			_value = _mm_slli_epi64(_value, (n)-64);
		}
		else
		{
			v2 = _mm_slli_si128(_value, 8);
			v2 = _mm_srli_epi64(v2, 64 - (n));
			_value = _mm_or_si128(_mm_slli_epi64(_value, n), v2);
		}
		return*this;
	}
	inline _int128& operator >>= (int n) {
		__m128i v2;
		if ((n) >= 64)
		{
			_value = _mm_srli_si128(_value, 8);
			_value = _mm_srli_epi64(_value, (n)-64);
		}
		else
		{
			v2 = _mm_srli_si128(_value, 8);
			v2 = _mm_slli_epi64(v2, 64 - (n));
			_value = _mm_or_si128(_mm_srli_epi64(_value, n), v2);
		}
		return*this;
	}

	operator __m128i&() { return _value; }
	operator __m128i const&()const { return _value; }

	friend bool operator == (_int128 const& x, _int128 const& y) {
		auto res = _mm_sub_epi64(x._value, y._value);
		return _mm_testz_si128(res, res);
	}
	friend bool operator != (_int128 const& x, _int128 const& y) {
		auto res = _mm_sub_epi64(x._value, y._value);
		return !_mm_testz_si128(res, res);
	}
	friend bool operator >= (_int128 const& x, _int128 const& y) {
		auto res = _mm_sub_epi64(x._value, y._value);
		return res.m128i_i64[1] > 0 ? true : res.m128i_i64[1] == 0 ? res.m128i_i64[0] >= 0 : false;
	}
	friend bool operator <= (_int128 const& x, _int128 const& y) {
		auto res = _mm_sub_epi64(x._value, y._value);
		return res.m128i_i64[1] < 0 ? true : res.m128i_i64[1] == 0 ? res.m128i_i64[0] <= 0 : false;
	}
	friend bool operator > (_int128 const& x, _int128 const& y) {
		auto res = _mm_sub_epi64(x._value, y._value);
		return res.m128i_i64[1] > 0 ? true : res.m128i_i64[1] == 0 ? res.m128i_i64[0] > 0 : false;
	}
	friend bool operator < (_int128 const& x, _int128 const& y) {
		auto res = _mm_sub_epi64(x._value, y._value);
		return res.m128i_i64[1] < 0 ? true : res.m128i_i64[1] == 0 ? res.m128i_i64[0] < 0 : false;
	}

	friend inline _int128 operator & (_int128 const x, _int128 const y) {
		return _mm_and_si128(x._value, y._value);
	}
	friend inline _int128 operator | (_int128 const x, _int128 const y) {
		return _mm_or_si128(x._value, y._value);
	}
	friend inline _int128 operator ^ (_int128 const x, _int128 const y) {
		return _mm_xor_si128(x._value, y._value);
	}
	friend inline _int128 operator ~ (_int128 const x) {
		return _mm_andnot_si128(x._value, _mm_setr_epi64x(-1, -1));
	}
	friend inline _int128 operator << (_int128 const x, int n) {
		__m128i v1, v2;
		if ((n) >= 64) {
			v1 = _mm_slli_si128(x, 8);
			v1 = _mm_slli_epi64(v1, (n)-64);
		}
		else {
			v1 = _mm_slli_epi64(x, n);
			v2 = _mm_slli_si128(x, 8);
			v2 = _mm_srli_epi64(v2, 64 - (n));
			v1 = _mm_or_si128(v1, v2);
		}
		return v1;
	}
	friend inline _int128 operator >> (_int128 const x, int n) {
		__m128i v1, v2;
		if ((n) >= 64)
		{
			v1 = _mm_srli_si128(x, 8);
			v1 = _mm_srli_epi64(v1, (n)-64);
		}
		else
		{
			v1 = _mm_srli_epi64(x, n);
			v2 = _mm_srli_si128(x, 8);
			v2 = _mm_slli_epi64(v2, 64 - (n));
			v1 = _mm_or_si128(v1, v2);
		}
		return v1;
	}

	friend _int128 operator + (_int128 const& _x, _int128 const& _y) {	
		__m128i x = _x._value;
		__m128i y = _y._value;
		__m128i carry;
		do {
			carry = _mm_and_si128(x, y);
			x = _mm_xor_si128(x, y);
			y = shift_left_128(carry, 1);
		} while (!_mm_testz_si128(y, y));
		return x;
	}
	friend _int128 operator - (_int128 const& _x, _int128 const& _y) {
		__m128i x = _x._value;
		__m128i y = _y._value;
		__m128i borrow;
		do {
			borrow = _mm_andnot_si128(x, y);
			x = _mm_xor_si128(x, y);
			y = shift_left_128(borrow, 1);
		} while (!_mm_testz_si128(y, y));
		return x;
	}
	friend _int128 operator - (_int128 const& _x) {
		__m128i x = _mm_setzero_si128();
		__m128i y = _x._value;
		__m128i borrow;
		do {
			borrow = _mm_andnot_si128(x, y);
			x = _mm_xor_si128(x, y);
			y = shift_left_128(borrow, 1);
		} while (!_mm_testz_si128(y, y));
		return x;
	}
	friend _int128 operator * (_int128 const& _x, _int128 const& _y) {
		_int128 res = 0, y = _y, x = _x;
		do {
			if (y._value.m128i_i8[0] & 0X01) 
				res += x;
			y >>= 1;
			x <<= 1;
		} while (!_mm_testz_si128(y._value, y._value));
		
		return res;
	}
	friend _int128 operator / (_int128 const& _x, _int128 const& _y) {
		_int128 res = 0, mod = 0, y = _y, x = _x;
		do {
			mod <<= 1;
			if (x._value.m128i_i8[15] & 0X80)
				mod |= 1;
			x <<= 1;
			res <<= 1;
			if (y <= mod){
				mod -= y;
				res |= 1;
			}	
		} while (!_mm_testz_si128(x._value, x._value));
		return res;
	}
	friend _int128 operator % (_int128 const& _x, _int128 const& _y) {
		_int128 mod = 0, y = _y, x = _x;
		do {
			mod <<= 1;
			if (x._value.m128i_i8[15] & 0X80)
				mod |= 1;
			x <<= 1;
			if (y <= mod) 
				mod -= y;	
		} while (!_mm_testz_si128(x._value, x._value));
		return mod;
	}

private:
	__m128i _value;
}int128_t;


struct MyStruct
{
	char a;
	short b;
	int c;
	long64 d;
};


int main()
{	
	int a = 123;
	int* b;

	auto const& id = type_of<int>();
	
	id.dyn_cast(&a, &b);

	return 0;
}

