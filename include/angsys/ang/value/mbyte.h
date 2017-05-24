#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_MBYTE_H__
#define __ANG_MBYTE_H__

namespace ang
{
	struct mbyte {
		enum special : uint
		{
			repl = 0XFFFDU,//Replcement Character
			ubom = 0XFEFFU,//Unicode Byte Order Mark
			mbom = 0XEFBBBFU,//MultiByte Order Mark
			null = 0XC080U,//modified utf-8, null character
			inv_repl = 0XFDFFU,//Inverted Replcement Character
			inv_ubom = 0XFFFEU,//Inverted Unicode Byte Order Mark
			inv_mbom = 0XBFBBEFU,//Inverted MultiByte Order Mark
			inv_null = 0X80C0U,//Inverted modified utf-8, null character
		};

		union {
			uint value;
			byte bytes[4];
		};

		inline mbyte();
		inline mbyte(wchar n);
		inline mbyte(char n);
		inline mbyte(mbyte &&);
		inline mbyte(mbyte const&);
		inline void convert(char const& _c);
		inline void convert(wchar const& c);
		inline wchar convert()const;
		inline int size()const;
		inline mbyte& operator = (mbyte&& byte);
		inline mbyte& operator = (const mbyte& byte);
		inline mbyte& operator = (char val);
		inline mbyte& operator = (wchar val);
		byte& operator [] (int idx) { return bytes[idx]; }
		byte const& operator [] (int idx)const { return bytes[idx]; }
	};

	inline bool operator == (mbyte const& arg1, mbyte const& arg2) { return arg1.value == arg2.value; }
	inline bool operator != (mbyte const& arg1, mbyte const& arg2) { return arg1.value != arg2.value; }
	inline bool operator >= (mbyte const& arg1, mbyte const& arg2) { return arg1.value >= arg2.value; }
	inline bool operator <= (mbyte const& arg1, mbyte const& arg2) { return arg1.value <= arg2.value; }
	inline bool operator > (mbyte const& arg1, mbyte const& arg2) { return arg1.value > arg2.value; }
	inline bool operator < (mbyte const& arg1, mbyte const& arg2) { return arg1.value < arg2.value; }

	inline bool operator == (mbyte const& arg1, unsigned int arg2) { return arg1.value == arg2; }
	inline bool operator != (mbyte const& arg1, unsigned int arg2) { return arg1.value != arg2; }
	inline bool operator >= (mbyte const& arg1, unsigned int arg2) { return arg1.value >= arg2; }
	inline bool operator <= (mbyte const& arg1, unsigned int arg2) { return arg1.value <= arg2; }
	inline bool operator > (mbyte const& arg1, unsigned int arg2) { return arg1.value > arg2; }
	inline bool operator < (mbyte const& arg1, unsigned int arg2) { return arg1.value < arg2; }

	inline bool operator == (unsigned int arg1, mbyte const& arg2) { return arg1 == arg2.value; }
	inline bool operator != (unsigned int arg1, mbyte const& arg2) { return arg1 != arg2.value; }
	inline bool operator >= (unsigned int arg1, mbyte const& arg2) { return arg1 >= arg2.value; }
	inline bool operator <= (unsigned int arg1, mbyte const& arg2) { return arg1 <= arg2.value; }
	inline bool operator > (unsigned int arg1, mbyte const& arg2) { return arg1 > arg2.value; }
	inline bool operator < (unsigned int arg1, mbyte const& arg2) { return arg1 < arg2.value; }


	template<> struct value<mbyte>
	{
	public: /*type definitions*/
		typedef mbyte type;

	private: /*members*/
		mbyte _value;

	public: /*constructors*/
		value() : _value() {}
		value(mbyte val) : _value(val) {}
		value(value<mbyte> const& other) : _value(other._value) {}
		value(value<mbyte> && other) : _value(ang::move(other._value)) {}
		template<typename T2> value(value<T2> const& other) : _value(other.get()) {}

	public: /*getters and setters*/
		mbyte & get() { return _value; }
		mbyte const& get()const { return _value; }
		void set(mbyte val) { _value = ang::move(val); }

	public: /*operators*/
		value<mbyte>& operator = (mbyte val) { set(ang::move(val)); return*this; }
		value<mbyte>& operator = (value<mbyte> const& val) { set(val._value); return*this; }
		value<mbyte>& operator = (value<mbyte> && val) { set(ang::move(val._value)); return*this; }

		operator mbyte& () { return get(); }
		operator mbyte ()const { return get(); }

		mbyte* operator -> () { return &get(); }
		mbyte const* operator -> ()const { return &get(); }

		value<mbyte*> operator & () { return{ &get(),1 }; }
		value<mbyte const*> operator & ()const { return{ &get(),1 }; }

		byte& operator [] (int idx) { return _value.bytes[idx]; }
		byte const& operator [] (int idx)const { return _value.bytes[idx]; }
	};


}

#include <ang/value/inline/mbyte.hpp>

#endif//__ANG_MBYTE_H__