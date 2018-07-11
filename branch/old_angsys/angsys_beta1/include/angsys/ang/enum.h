/*********************************************************************************************************************/
/*   File Name: ang/enum.h                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is declare useful templates clases for scoped enums and flags management.           */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ...
#elif !defined __ANG_ENUM_H__
#define __ANG_ENUM_H__

namespace ang
{
	template<class _type, class _child>
	class flag
	{
		typedef _type		type;
		typedef _child		child_t;
		typedef flag<_type, _child>	base_t;

	protected:
		type _value;

	public:
		flag(_type value) :_value(ang::move(value)) {
		}
		child_t& operator |=(child_t f) {
			_value |= f._value;
			return (child_t&)(*this);
		}
		child_t& operator &=(child_t f) {
			_value &= f._value;
			return (child_t&)(*this);
		}
		child_t operator |(child_t f) {
			return (child_t)(_value | f._value);
		}
		child_t operator &(child_t f) {
			return (child_t)(_value & f._value);
		}
		child_t operator +(child_t f) {
			return (child_t)(_value | f._value);
		}
		child_t operator -(child_t f) {
			return (child_t)(_value&~f._value);
		}
		child_t operator ~() {
			return child_t(~_value);
		}
		child_t& operator +=(child_t f) {
			_value |= f._value;
			return (child_t&)(*this);
		}
		child_t& operator -=(child_t f) {
			_value &= ~f._value;
			return (child_t&)(*this);
		}
		bool operator == (child_t b)const {
			return (this->_value == b._value) ? true : false;
		}
		bool operator != (child_t b)const {
			return (this->_value != b._value) ? true : false;
		}
		bool operator >= (child_t b)const {
			return (this->_value >= b._value) ? true : false;
		}
		bool operator <= (child_t b)const {
			return (this->_value <= b._value) ? true : false;
		}
		bool operator < (child_t b)const {
			return (this->_value < b._value) ? true : false;
		}
		bool operator >(child_t b)const {
			return (this->_value > b._value) ? true : false;
		}
		bool is_active(type flag)const {
			if ((_value & flag) == flag)
				return true;
			return false;
		}
		bool is_active(child_t const& flag)const {
			if ((_value & flag._value) == flag._value)
				return true;
			return false;
		}
		type value()const {
			return _value;
		}
		type& value() {
			return _value;
		}
		void value(type value) {
			_value = ang::move(value);
		}
	};

}

#define ANG_BEGIN_FLAGS(_LINK, _NAME, _BASE) \
	class _LINK _NAME##_t : public ang::flag<_BASE, _NAME##_t> { \
	public: \
		typedef _BASE enum_base;\
		enum _enum : enum_base; typedef _enum enum_t; \
		friend _LINK _NAME##_t operator + (enum_t, enum_t); \
		friend _LINK _NAME##_t operator + (const _NAME##_t&, enum_t); \
		friend _LINK _NAME##_t operator + (enum_t, const _NAME##_t&); \
		friend _LINK _NAME##_t operator - (enum_t, enum_t); \
		friend _LINK _NAME##_t operator - (const _NAME##_t&, enum_t); \
		friend _LINK _NAME##_t operator - (enum_t, const _NAME##_t&); \
		friend _LINK bool operator == (_NAME##_t const&, _NAME##_t const&); \
		friend _LINK bool operator != (_NAME##_t const&, _NAME##_t const&); \
		friend _LINK bool operator == (_NAME##_t const&, enum_t); \
		friend _LINK bool operator != (_NAME##_t const&, enum_t); \
		friend _LINK bool operator == (enum_t, _NAME##_t const&); \
		friend _LINK bool operator != (enum_t, _NAME##_t const&); \
		enum _enum : enum_base {

#define ANG_END_FLAGS(_NAME) }; \
		_NAME##_t(); \
		_NAME##_t(enum_t); \
		_NAME##_t(const _NAME##_t&); \
		_NAME##_t&operator = (enum_t); \
		_NAME##_t&operator = (const _NAME##_t&); \
		operator enum_t()const; \
		operator enum_base()const; \
		string_t to_string()const;\
	protected: \
		_NAME##_t(enum_base); \
		friend ang::flag <enum_base, _NAME##_t >; \
	}; typedef _NAME##_t::enum_t _NAME;

#define ANG_IMPLEMENT_FLAGS(_NAMESPACE, _NAME, _BASE) \
	bool _NAMESPACE::operator == (_NAMESPACE::_NAME##_t const& first, _NAMESPACE::_NAME##_t const& second) { return first.value() == second.value(); } \
	bool _NAMESPACE::operator != (_NAMESPACE::_NAME##_t const& first, _NAMESPACE::_NAME##_t const& second) { return first.value() != second.value(); } \
	bool _NAMESPACE::operator == (_NAMESPACE::_NAME##_t const& first, _NAMESPACE::_NAME second) { return first.value() == (_BASE)second; } \
	bool _NAMESPACE::operator != (_NAMESPACE::_NAME##_t const& first, _NAMESPACE::_NAME second) { return first.value() != (_BASE)second; } \
	bool _NAMESPACE::operator == (_NAMESPACE::_NAME first, _NAMESPACE::_NAME##_t const& second) { return (_BASE)first == second.value(); } \
	bool _NAMESPACE::operator != (_NAMESPACE::_NAME first, _NAMESPACE::_NAME##_t const& second) { return (_BASE)first != second.value(); } \
	_NAMESPACE::_NAME##_t _NAMESPACE::operator + (_NAMESPACE::_NAME e1, _NAMESPACE::_NAME e2) { return _NAMESPACE::_NAME##_t(e1 | e2); } \
	_NAMESPACE::_NAME##_t _NAMESPACE::operator + (const _NAMESPACE::_NAME##_t& e1, _NAMESPACE::_NAME e2) { return _NAMESPACE::_NAME##_t(e1._value | e2); } \
	_NAMESPACE::_NAME##_t _NAMESPACE::operator + (_NAMESPACE::_NAME e1, const _NAMESPACE::_NAME##_t& e2) { return _NAMESPACE::_NAME##_t(e1 | e2._value); } \
	_NAMESPACE::_NAME##_t _NAMESPACE::operator - (_NAMESPACE::_NAME e1, _NAMESPACE::_NAME e2) { return _NAMESPACE::_NAME##_t(e1 & ~e2); } \
	_NAMESPACE::_NAME##_t _NAMESPACE::operator - (const _NAMESPACE::_NAME##_t& e1, _NAMESPACE::_NAME e2) { return _NAMESPACE::_NAME##_t(e1._value & ~e2); } \
	_NAMESPACE::_NAME##_t _NAMESPACE::operator - (_NAMESPACE::_NAME e1, const _NAMESPACE::_NAME##_t& e2) { return _NAMESPACE::_NAME##_t(e1 & ~e2._value); } \
	_NAMESPACE::_NAME##_t::_NAME##_t() : flag<_BASE, _NAMESPACE::_NAME##_t>(0) { } \
	_NAMESPACE::_NAME##_t::_NAME##_t(_NAMESPACE::_NAME e) : flag<_BASE, _NAMESPACE::_NAME##_t>(e) { } \
	_NAMESPACE::_NAME##_t::_NAME##_t(_BASE e) : flag<_BASE, _NAMESPACE::_NAME##_t>(e) { } \
	_NAMESPACE::_NAME##_t::_NAME##_t(const _NAMESPACE::_NAME##_t&e) : flag(e._value) { } \
	_NAMESPACE::_NAME##_t& _NAME##_t::operator = (_NAMESPACE::_NAME e) { _value = e; return*this; } \
	_NAMESPACE::_NAME##_t& _NAME##_t::operator = (const _NAMESPACE::_NAME##_t& e) { _value = e._value; return*this; } \
	_NAMESPACE::_NAME##_t::operator _NAMESPACE::_NAME()const { return _NAMESPACE::_NAME(_value); } \
	_NAMESPACE::_NAME##_t::operator _BASE()const { return _value; }

#define ANG_BEGIN_ENUM(_LINK, _NAME, _BASE) \
	class _LINK _NAME##_t { \
	public: typedef _BASE type; \
	public: enum enum_t : _BASE {

#define ANG_END_ENUM(_NAME) }; \
		static _NAME##_t parse(cstr_t);\
		static _NAME##_t parse(cwstr_t);\
		_NAME##_t(); \
		_NAME##_t(enum_t); \
		_NAME##_t(const _NAME##_t&); \
		enum_t get()const; \
		void set(enum_t); \
		cstr_t to_string()const;\
		_NAME##_t&operator = (enum_t); \
		_NAME##_t&operator = (const _NAME##_t&); \
		operator enum_t()const; \
		explicit operator type()const; \
	protected: \
		enum_t _value; \
	}; typedef _NAME##_t::enum_t _NAME;

#define ANG_IMPLEMENT_ENUM(_NAMESPACE, _NAME, _BASE, _DEFVAL) \
		_NAMESPACE::_NAME##_t::_NAME##_t() : _value(_DEFVAL) {} \
		_NAMESPACE::_NAME##_t::_NAME##_t(_NAMESPACE::_NAME val) : _value(val) {} \
		_NAMESPACE::_NAME##_t::_NAME##_t(const _NAMESPACE::_NAME##_t& val) : _value(val._value) {} \
		_NAMESPACE::_NAME _NAMESPACE::_NAME##_t::get()const { return _value; } \
		void _NAMESPACE::_NAME##_t::set(_NAMESPACE::_NAME val) { _value = val; } \
		_NAMESPACE::_NAME##_t& _NAMESPACE::_NAME##_t::operator = (_NAMESPACE::_NAME val) { set(val); return*this; } \
		_NAMESPACE::_NAME##_t& _NAMESPACE::_NAME##_t::operator = (const _NAMESPACE::_NAME##_t& val) { set(val._value); return*this; } \
		_NAMESPACE::_NAME##_t::operator _NAMESPACE::_NAME()const{ return _value; } \
		//_NAMESPACE::_NAME##_t::operator _BASE()const { return _value; }


#endif//__ANG_ENUM_H__
