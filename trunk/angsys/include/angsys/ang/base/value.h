/*********************************************************************************************************************/
/*   File Name: ang/base/value.h                                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_VALUE_H__
#define __ANG_BASE_VALUE_H__

namespace ang //constants
{

	template<typename T, genre_t TYPE>
	struct value {
		typedef T type;
		static constexpr genre_t genre_id = TYPE;

		value() : m_value(default_value<T>::value) {}
		value(T const& v) : m_value(v) {}
		value(value const& v) : m_value(v.m_value) {}
		value(T && v) : m_value(ang::move(v)) { 
			v = default_value<T>::value; 
		}
		value(value && v) : m_value(ang::move(v.m_value)) {
			v.m_value = default_value<T>::value;
		}

		operator T& () { return get(); }
		operator T const& ()const { return get(); }

		value& operator = (T const& v){
			m_value = v;
			return*this;
		}
		value& operator = (value const& v) {
			m_value = v.m_value;
			return*this;
		}
		value& operator = (T && v) {
			m_value = ang::move(v);
			v = default_value<T>::value;
			return*this;
		}
		value& operator = (value && v) {
			m_value = ang::move(v.m_value);
			v.m_value = default_value<T>::value;
			return*this;
		}

		T& get() { return m_value; }
		T const& get()const { return m_value; }
		void set(T const& v) { m_value = v; }
		void move(T && v) {
			m_value = ang::forward<T>(v);
			v = default_value<T>::value;
		}

	private:
		T m_value;
	};

	template<typename T>
	struct value<T, genre::union_type> {
		typedef T type;
		static constexpr genre_t genre_id = genre::union_type;

		value() : m_value(default_value<T>::value) {}
		value(T const& v) : m_value(v) {}
		value(value const& v) : m_value(v.m_value) {}
		value(T && v) : m_value(ang::move(v)) {
			v = default_value<T>::value;
		}
		value(value && v) : m_value(ang::move(v.m_value)) {
			v.m_value = default_value<T>::value;
		}

		operator T& () { return get(); }
		operator T const& ()const { return get(); }

		value& operator = (T const& v) {
			m_value = v;
			return*this;
		}
		value& operator = (value const& v) {
			m_value = v.m_value;
			return*this;
		}
		value& operator = (T && v) {
			m_value = ang::move(v);
			v = default_value<T>::value;
			return*this;
		}
		value& operator = (value && v) {
			m_value = ang::move(v.m_value);
			v.m_value = default_value<T>::value;
			return*this;
		}

		T* operator ->() { return &get(); }
		T const* operator ->()const { return &get(); }

		T& get() { return m_value; }
		T const& get()const { return m_value; }
		void set(T const& v) { m_value = v; }
		void move(T && v) {
			m_value = ang::forward<T>(v);
			v = default_value<T>::value;
		}

	private:
		T m_value;
	};

	template<typename T>
	struct value<T, genre::class_type> : public T {
		typedef T type;
		static constexpr genre_t genre_id = genre::class_type;

		value() {}
		value(T const& v) : T(v) { }
		value(value const& v) : T(v.get()) { }
		value(T && v) : T(ang::forward<T>(v)) { }
		value(value && v) : T(ang::forward<value>(v)) { }

		operator T& () { return static_cast<T&>(*this); }
		operator T const& ()const { return static_cast<T const&>(*this); }

		value& operator = (T const& v) {
			static_cast<T&>(*this) = ang::forward<T>(v);
			return*this;
		}
		value& operator = (value const& v) {
			static_cast<T&>(*this) = ang::forward<value>(v);
			return*this;
		}
		value& operator = (T && v) {
			static_cast<T&>(*this) = ang::forward<T>(v);
			return*this;
		}
		value& operator = (value && v) {
			static_cast<T&>(*this) = ang::forward<value>(v);
			return*this;
		}

		T* operator ->() { return this; }
		T const* operator ->()const { return this; }

		T& get() { return static_cast<T&>(*this); }
		T const& get()const { return static_cast<T const&>(*this); }
		void set(T const& v) { static_cast<T&>(*this) = v; }
		void move(T && v) {
			static_cast<T&>(*this) = ang::forward<T>(v);
		}
	};

	template<typename T>
	struct value<T, genre::enum_type> {
		typedef T type;
		typedef enum_type<T> base;
		static constexpr genre_t genre_id = genre::enum_type;

		value() {}
		value(type const& v) { get() = v; }
		value(value const& v) { get() = v.get(); }
		value(type && v) {
			get() = v;
			v = default_value<type>::value;
		}
		value(value && v) {
			get() = ang::move(v.get());
			v.get() = default_value<type>::value;
		}

		operator type& () { return get(); }
		operator type const& ()const { return get(); }

		value& operator = (type const& v) {
			get() = v;
			return*this;
		}
		value& operator = (value const& v) {
			get() = v.get();
			return*this;
		}
		value& operator = (type && v) {
			get() = v;
			v = default_value<type>::value;
			return*this;
		}
		value& operator = (value && v) {
			get() = ang::move(v.get());
			v.get() = default_value<type>::value;
			return*this;
		}

		type& get() { return *reinterpret_cast<type*>(&m_value); }
		type const& get()const { return *reinterpret_cast<type const*>(&m_value); }
		void set(type const& v) { get() = v; }
		void move(type && v) {
			get() = v;
			v = default_value<type>::value;
		}

	protected:
		enum_type<type> m_value;
	};


#define safe_enum(_LINK, _name, _type)  enum class _name : _type; \
	struct _LINK _name##_t : public ang::value<_name> { \
		static rtti_t const& class_info(); \
		_name##_t() : value(default_value<type>::value) {} \
		_name##_t(type const& v) : value(v) {} \
		_name##_t(_name##_t const& v) : value(v) {} \
		_name##_t(type && v) : value(ang::forward<type>(v)) {	} \
		_name##_t(_name##_t && v) : value(ang::forward<value>(v)) { } \
		ang::wstring to_string()const; \
		_name##_t& operator = (type const& v){ get() = v; return*this; } \
		_name##_t& operator = (_name##_t const& v) { get() = v.get(); return*this; } \
		_name##_t& operator = (type && v) { get() = ang::move(v); v = default_value<type>::value; return*this; } \
		_name##_t& operator = (_name##_t && v) { get() = ang::move(v.get()); v.set(default_value<type>::value); return*this; } \
		friend inline bool operator == (_name##_t const& a1, _name##_t const& a2){return a1.get() == a2.get();} \
		friend inline bool operator != (_name##_t const& a1, _name##_t const& a2){return a1.get() != a2.get();} \
		friend inline bool operator >= (_name##_t const& a1, _name##_t const& a2){return a1.get() >= a2.get();} \
		friend inline bool operator <= (_name##_t const& a1, _name##_t const& a2){return a1.get() <= a2.get();} \
		friend inline bool operator > (_name##_t const& a1, _name##_t const& a2){return a1.get() > a2.get();} \
		friend inline bool operator < (_name##_t const& a1, _name##_t const& a2){return a1.get() < a2.get();} \
		friend inline bool operator == (_name##_t const& a1, _name a2){return a1.get() == a2;} \
		friend inline bool operator == (_name a1, _name##_t const& a2){return a1 == a2.get();} \
		friend inline bool operator != (_name##_t const& a1, _name a2){return a1.get() != a2;} \
		friend inline bool operator != (_name a1, _name##_t const& a2){return a1 != a2.get();} \
		friend inline bool operator >= (_name##_t const& a1, _name a2){return a1.get() >= a2;} \
		friend inline bool operator >= (_name a1, _name##_t const& a2){return a1 >= a2.get();} \
		friend inline bool operator <= (_name##_t const& a1, _name a2){return a1.get() <= a2;} \
		friend inline bool operator <= (_name a1, _name##_t const& a2){return a1 <= a2.get();} \
		friend inline bool operator > (_name##_t const& a1, _name a2){return a1.get() > a2;} \
		friend inline bool operator > (_name a1, _name##_t const& a2){return a1 > a2.get();} \
		friend inline bool operator < (_name##_t const& a1, _name a2){return a1.get() < a2;} \
		friend inline bool operator < (_name a1, _name##_t const& a2){return a1 < a2.get();} \
	}; enum class _name : _type


#define safe_flags(_LINK, _name, _type) enum class _name : _type; \
	struct _LINK _name##_t : public ang::value<_name> { \
		static rtti_t const& class_info(); \
		_name##_t() : value(default_value<type>::value) {} \
		_name##_t(type const& v) : value(v) {} \
		_name##_t(_name##_t const& v) : value(v) {} \
		_name##_t(type && v) : value(ang::forward<type>(v)) {	} \
		_name##_t(_name##_t && v) : value(ang::forward<value>(v)) { } \
		ang::wstring to_string()const; \
		_name##_t& operator = (type const& v){ get() = v; return*this; } \
		_name##_t& operator = (_name##_t const& v) { get() = v.get(); return*this; } \
		_name##_t& operator = (type && v) { get() = ang::move(v); v = default_value<type>::value; return*this; } \
		_name##_t& operator = (_name##_t && v) { get() = ang::move(v.get()); v.set(default_value<type>::value); return*this; } \
		_name##_t& operator ^= (const _name##_t& v) { m_value ^= (base)v.get(); return*this; } \
		_name##_t& operator ^= (type v) { m_value ^= (base)v;	return*this; } \
		_name##_t& operator *= (const _name##_t& v) { m_value &= (base)v.get(); return*this; } \
		_name##_t& operator *= (type v) { m_value &= (base)v;	return*this; } \
		_name##_t& operator += (const _name##_t& v) { m_value |= (base)v.get(); return*this; } \
		_name##_t& operator += (type v) { m_value |= (base)v;	return*this; } \
		_name##_t& operator -= (const _name##_t& v) { m_value &= ~(base)v.get(); return*this; } \
		_name##_t& operator -= (type v) { m_value &= ~(base)v;	return*this; } \
		bool operator !(void)const { return m_value == 0; } \
		operator bool(void)const { return m_value != 0; } \
		friend _LINK _name##_t operator * (type, type); \
		friend _LINK _name##_t operator * (const _name##_t&, type); \
		friend _LINK _name##_t operator * (type, const _name##_t&); \
		friend _LINK _name##_t operator * (const _name##_t&, const _name##_t&); \
		friend _LINK _name##_t operator + (type, type); \
		friend _LINK _name##_t operator + (const _name##_t&, type); \
		friend _LINK _name##_t operator + (type, const _name##_t&); \
		friend _LINK _name##_t operator + (const _name##_t&, const _name##_t&); \
		friend _LINK _name##_t operator - (type, type); \
		friend _LINK _name##_t operator - (const _name##_t&, type); \
		friend _LINK _name##_t operator - (type, const _name##_t&); \
		friend _LINK _name##_t operator - (const _name##_t&, const _name##_t&); \
		friend _LINK _name##_t operator & (type, type); \
		friend _LINK _name##_t operator & (const _name##_t&, type); \
		friend _LINK _name##_t operator & (type, const _name##_t&); \
		friend _LINK _name##_t operator & (const _name##_t&, const _name##_t&); \
		friend _LINK _name##_t operator | (type, type); \
		friend _LINK _name##_t operator | (const _name##_t&, type); \
		friend _LINK _name##_t operator | (type, const _name##_t&); \
		friend _LINK _name##_t operator | (const _name##_t&, const _name##_t&); \
		friend _LINK _name##_t operator ^ (type, type); \
		friend _LINK _name##_t operator ^ (const _name##_t&, type); \
		friend _LINK _name##_t operator ^ (type, const _name##_t&); \
		friend _LINK _name##_t operator ^ (const _name##_t&, const _name##_t&); \
		friend _LINK _name##_t operator ~ (type); \
		friend _LINK _name##_t operator ~ (_name##_t const&); \
		friend _LINK bool operator == (_name##_t const&, _name##_t const&); \
		friend _LINK bool operator != (_name##_t const&, _name##_t const&); \
		friend _LINK bool operator >= (_name##_t const&, _name##_t const&); \
		friend _LINK bool operator <= (_name##_t const&, _name##_t const&); \
		friend _LINK bool operator > (_name##_t const&, _name##_t const&); \
		friend _LINK bool operator < (_name##_t const&, _name##_t const&); \
		friend _LINK bool operator == (_name##_t const&, type); \
		friend _LINK bool operator != (_name##_t const&, type); \
		friend _LINK bool operator >= (_name##_t const&, type); \
		friend _LINK bool operator <= (_name##_t const&, type); \
		friend _LINK bool operator > (_name##_t const&, type); \
		friend _LINK bool operator < (_name##_t const&, type); \
		friend _LINK bool operator == (type, _name##_t const&); \
		friend _LINK bool operator != (type, _name##_t const&); \
		friend _LINK bool operator >= (type, _name##_t const&); \
		friend _LINK bool operator <= (type, _name##_t const&); \
		friend _LINK bool operator > (type, _name##_t const&); \
		friend _LINK bool operator < (type, _name##_t const&); \
	}; enum class _name : _type



#define safe_flags_implement(_NAMESPACE, _name) \
	bool _NAMESPACE::operator == (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name##_t const& second) { return first.get() == second.get(); } \
	bool _NAMESPACE::operator != (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name##_t const& second) { return first.get() != second.get(); } \
	bool _NAMESPACE::operator >= (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name##_t const& second) { return first.get() >= second.get(); } \
	bool _NAMESPACE::operator <= (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name##_t const& second) { return first.get() <= second.get(); } \
	bool _NAMESPACE::operator > (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name##_t const& second) { return first.get() > second.get(); } \
	bool _NAMESPACE::operator < (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name##_t const& second) { return first.get() < second.get(); } \
	bool _NAMESPACE::operator == (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name second) { return first.get() == second; } \
	bool _NAMESPACE::operator != (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name second) { return first.get() != second; } \
	bool _NAMESPACE::operator >= (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name second) { return first.get() >= second; } \
	bool _NAMESPACE::operator <= (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name second) { return first.get() <= second; } \
	bool _NAMESPACE::operator > (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name second) { return first.get() > second; } \
	bool _NAMESPACE::operator < (_NAMESPACE::_name##_t const& first, _NAMESPACE::_name second) { return first.get() < second; } \
	bool _NAMESPACE::operator == (_NAMESPACE::_name first, _NAMESPACE::_name##_t const& second) { return first == second.get(); } \
	bool _NAMESPACE::operator != (_NAMESPACE::_name first, _NAMESPACE::_name##_t const& second) { return first != second.get(); } \
	bool _NAMESPACE::operator >= (_NAMESPACE::_name first, _NAMESPACE::_name##_t const& second) { return first >= second.get(); } \
	bool _NAMESPACE::operator <= (_NAMESPACE::_name first, _NAMESPACE::_name##_t const& second) { return first <= second.get(); } \
	bool _NAMESPACE::operator > (_NAMESPACE::_name first, _NAMESPACE::_name##_t const& second) { return first > second.get(); } \
	bool _NAMESPACE::operator < (_NAMESPACE::_name first, _NAMESPACE::_name##_t const& second) { return first < second.get(); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator * (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 & (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator * (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value & (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator * (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 & e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator * (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value & e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator + (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 | (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator + (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value | (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator + (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 | e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator + (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value | e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator - (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 & ~(typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator - (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value & ~(typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator - (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 & ~e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator - (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value & ~e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator & (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 & (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator & (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value & (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator & (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 & e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator & (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value & e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator | (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 | (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator | (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value | (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator | (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 | e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator | (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value | e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ^ (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 ^ (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ^ (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value ^ (typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ^ (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base)e1 ^ e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ^ (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value ^ e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ~ (_NAMESPACE::_name e2) { return _NAMESPACE::_name(~(typename _name##_t::base)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ~ (const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(~e2.m_value); }

#define FUNCX_TYPE_OF_DIR(A0) &ang::rtti::type_of<A0>()

#define safe_enum_rrti(_NAMESPACE, _name, ...) \
	ang::rtti_t const& _NAMESPACE::_name##_t::class_info() { \
		ang::rtti_t const* parent[] ={ ANG_EXPAND(APPLY_FUNCX_N(FUNCX_TYPE_OF_DIR, COMA_SEPARATOR, __VA_ARGS__)) }; \
		return rtti::regist(#_NAMESPACE"::"#_name, genre::enum_type, sizeof(_name##_t), alignof(_name##_t), parent, null); \
	}

#define safe_enum_rrti2(_NAMESPACE, _name) \
	ang::rtti_t const& _NAMESPACE::_name##_t::class_info() { \
		ang::rtti_t const* parent[] = { &ang::type_of<value<_name>>() }; \
		return rtti::regist(#_NAMESPACE"::"#_name, genre::enum_type, sizeof(_name##_t), alignof(_name##_t), parent, null); \
	}

}

#endif//__ANG_BASE_VALUE_H__
