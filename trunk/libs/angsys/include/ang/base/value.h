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
	
	template<typename T, gender_t TYPE>
	struct value {
		typedef T type;
		static constexpr gender_t gender_id = TYPE;

		static runtime::rtti_t const& class_info() {
			return ang::type_of<T>();
		}

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

		T& get() { return m_value; }
		T const& get()const { return m_value; }
		void set(T const& v) { m_value = v; }
		void move(T && v) {
			m_value = ang::forward<T>(v);
			v = default_value<T>::value;
		}

		T& operator ->() {
			return get();
		}

		T const& operator ->()const {
			return get();
		}

		T* operator & () {
			return &get();
		}
		T const* operator & ()const {
			return &get();
		}

	protected:
		T m_value;
	};

	template<typename T>
	struct value<T, gender::union_type> {
		typedef T type;
		static constexpr gender_t gender_id = gender::union_type;

		static runtime::rtti_t const& class_info() {
			return ang::type_of<T>();
		}

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
	struct value<T, gender::class_type> : public T {
		typedef T type;
		static constexpr gender_t gender_id = gender::class_type;

		static runtime::rtti_t const& class_info() {
			return ang::type_of<T>();
		}

		value() {
		
		}
		template<typename...Ts, enable_if<or_expression<is_constructible<type, Ts&&...>::value, is_trivially_constructible<type>::value>::value, bool> = true>
		value(Ts&&... args)
			: type({ang::forward<Ts>(args)...}) {
		}
		

		value(T const& v) : T(v) { }
		value(value const& v) : T(v.get()) { }
		value(T && v) : T(ang::forward<T>(v)) { }
		value(value && v) : T(ang::forward<value>(v)) { }

		operator T& () { return static_cast<T&>(*this); }
		operator T const& ()const { return static_cast<T const&>(*this); }

		value& operator = (T const& v) {
			static_cast<T&>(*this) = v;
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

		template<typename U> value& operator = (U && v) {
			static_cast<T&>(*this) = ang::forward<U>(v);
			return*this;
		}

		template<typename U> value& operator = (U const& v) {
			static_cast<T&>(*this) = v;
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
	struct value<T, gender::enum_type> {
		typedef T type;
		typedef enum_type<T> base;
		static constexpr gender_t gender_id = gender::enum_type;

		static runtime::rtti_t const& class_info() {
			return ang::type_of<T>();
		}

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

	template<typename T>
	struct value<T[], gender::value_type> : array<T> {
		typedef T type;
		static constexpr gender_t gender_id = gender::value_type;

		static runtime::rtti_t const& class_info() {
			return ang::type_of<T>();
		}

		value() : array<T>() {}
		value(ang::nullptr_t const&) : array<T>(null) {}
		value(wsize sz) : array<T>(sz) {}
		value(value const& other) : array<T>(other) {}
		value(value && other) : array<T>(ang::forward<value>(other)) {}
		value(array<T> const& other) : array<T>(other) {}
		value(array<T> && other) : array<T>(ang::forward<value>(other)) {}
		template<typename T2>
		value(array_view<T2> const& ar) : array<T>(ar) {}
		template<typename T2>
		value(initializer_list<T2> const& list) : array<T>(list) {}
		template<typename T2, wsize N>
		value(T2(&ar)[N]) : array<T>(ar) {}
		template<typename IT>
		value(IT first, IT last) : array<T>(first, last) {}
		~value() {}

		value& operator = (type first) {
			array<T>::operator = (first);
			return*this;
		}
		value& operator = (array<T> const& val) {
			array<T>::operator = (val);
			return*this;
		}
		value& operator = (array<T> && val) {
			array<T>::operator = (ang::forward<array<T>>(val));
			return*this;
		}
		value& operator = (value const& val) {
			array<T>::operator = (val);
			return*this;
		}
		value& operator = (value && val) {
			array<T>::operator = (ang::forward<value>(val));
			return*this;
		}
		template<typename T2>
		value& operator = (array_view<T2> const& ar) {
			array<T>::operator = (ar);
			return*this;
		}
		template<typename T2>
		value& operator = (initializer_list<T2> const& list) {
			array<T>::operator = (list);
			return*this;
		}
		template<typename T2, wsize N>
		value& operator = (T2(&ar)[N]) {
			array<T>::operator = (ar);
			return*this;
		}

	protected:
		T m_value;
	};
}

#define MY_LINKAGE LINK

#define MY_TYPE char
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE uchar
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE short
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE ushort
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE int
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE uint
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE long
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE ulong
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE long64
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE ulong64
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE float
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#define MY_TYPE double
#include <ang/base/inline/value_specialization.hpp>
#undef MY_TYPE

#undef MY_LINKAGE

#ifdef WINDOWS_PLATFORM
template<typename T> struct enum_class;
template<typename T> struct flags_class;
#elif defined ANDROID_PLATFORM

template<typename T> struct enum_class
	: public ang::value<T>
{
	using type = typename ang::value<T>::type;
	using base = ang::value<T>;

	static_assert(ang::gender_of<T>() == ang::gender::enum_type, "T is not a enum type");
	static ang::rtti_t const& class_info() {
		return ang::type_of<T>();
	}
	static enum_class<T> parse(ang::cstr_t);

	enum_class() : base(ang::default_value<type>::value) {}
	enum_class(type const& v) : base(v) {}
	enum_class(enum_class<T> const& v) : base(v) {}
	enum_class(type && v) : base(ang::forward<type>(v)) {	}
	enum_class(enum_class<T> && v) : base(ang::forward<base>(v)) { }
	ang::castr_t to_string()const;
	enum_class<T>& operator = (type const& v){ this->get() = v; return*this; }
	enum_class<T>& operator = (enum_class<T> const& v) { this->get() = v.get(); return*this; }
	enum_class<T>& operator = (type && v) { this->get() = ang::move(v); v = ang::default_value<type>::value; return*this; }
	enum_class<T>& operator = (enum_class<T> && v) { this->get() = ang::move(v.get()); v.set(ang::default_value<type>::value); return*this; }
	friend inline bool operator == (enum_class<T> const& a1, enum_class<T> const& a2) { return a1.get() == a2.get(); }
	friend inline bool operator != (enum_class<T> const& a1, enum_class<T> const& a2) { return a1.get() != a2.get(); }
	friend inline bool operator >= (enum_class<T> const& a1, enum_class<T> const& a2) { return a1.get() >= a2.get(); }
	friend inline bool operator <= (enum_class<T> const& a1, enum_class<T> const& a2) { return a1.get() <= a2.get(); }
	friend inline bool operator > (enum_class<T> const& a1, enum_class<T> const& a2) { return a1.get() > a2.get(); }
	friend inline bool operator < (enum_class<T> const& a1, enum_class<T> const& a2) { return a1.get() < a2.get(); }
	friend inline bool operator == (enum_class<T> const& a1, T a2) { return a1.get() == a2; }
	friend inline bool operator == (T a1, enum_class<T> const& a2) { return a1 == a2.get(); }
	friend inline bool operator != (enum_class<T> const& a1, T a2) { return a1.get() != a2; }
	friend inline bool operator != (T a1, enum_class<T> const& a2) { return a1 != a2.get(); }
	friend inline bool operator >= (enum_class<T> const& a1, T a2) { return a1.get() >= a2; }
	friend inline bool operator >= (T a1, enum_class<T> const& a2) { return a1 >= a2.get(); }
	friend inline bool operator <= (enum_class<T> const& a1, T a2) { return a1.get() <= a2; }
	friend inline bool operator <= (T a1, enum_class<T> const& a2) { return a1 <= a2.get(); }
	friend inline bool operator > (enum_class<T> const& a1, T a2) { return a1.get() > a2; }
	friend inline bool operator > (T a1, enum_class<T> const& a2) { return a1 > a2.get(); }
	friend inline bool operator < (enum_class<T> const& a1, T a2) { return a1.get() < a2; }
	friend inline bool operator < (T a1, enum_class<T> const& a2) { return a1 < a2.get(); }
};

template<typename T> struct flags_class
	: public ang::value<T> 
{
	using type = typename ang::value<T>::type;
	using base_type = typename ang::value<T>::base;
	using base = typename ang::value<T>;

	static ang::rtti_t const& class_info() {
		return ang::type_of<T>();
	}
	flags_class() : base(ang::default_value<type>::value) {}
	flags_class(type const& v) : base(v) {}
	flags_class(flags_class<T> const& v) : base(v) {}
	flags_class(type && v) : base(ang::forward<type>(v)) {	}
	flags_class(flags_class<T> && v) : base(ang::forward<base>(v)) { }
	ang::fast_astring_t to_string()const;
	bool is_active(flags_class<T> v)const { return ((base_type)v.get_value() & this->get_value()) == (base_type)v.get_value(); }
	flags_class<T>& operator = (type const& v){ this->get_value() = v; return*this; }
	flags_class<T>& operator = (flags_class<T> const& v) { this->get_value() = v.get_value(); return*this; }
	flags_class<T>& operator = (type && v) { this->get() = ang::move(v); v = ang::default_value<type>::value; return*this; }
	flags_class<T>& operator = (flags_class<T> && v) { this->get_value() = ang::move(v.get_value()); v.set(ang::default_value<type>::value); return*this; }
	flags_class<T>& operator ^= (const flags_class<T>& v) { this->get_value() ^= (base_type)v.get_value(); return*this; }
	flags_class<T>& operator ^= (type v) { this->get_value() ^= (base_type)v;	return*this; }
	flags_class<T>& operator *= (const flags_class<T>& v) { this->get_value() &= (base_type)v.get_value(); return*this; }
	flags_class<T>& operator *= (type v) { this->get_value() &= (base_type)v;	return*this; }
	flags_class<T>& operator += (const flags_class<T>& v) { this->get_value() |= (base_type)v.get_value(); return*this; }
	flags_class<T>& operator += (type v) { this->get_value() |= (base_type)v;	return*this; }
	flags_class<T>& operator -= (const flags_class<T>& v) { this->get_value() &= ~(base_type)v.get_value(); return*this; }
	flags_class<T>& operator -= (type v) { this->get_value() &= ~(base_type)v;	return*this; }
	bool operator !(void)const { return this->get_value() == 0; }
	operator bool(void)const { return this->get_value() != 0; }
	operator base_type(void)const { return this->get_value(); }

private:
	base_type& get_value() { return reinterpret_cast<base_type&>(this->get()); }
	base_type const& get_value()const { return reinterpret_cast<base_type const&>(this->get()); }

	friend inline flags_class<T> operator * (T e1, T e2) { return T(e1 & e2); }
	friend inline flags_class<T> operator * (const flags_class<T>& e1, T e2) { return T(e1.get_value() & (base_type)e2); }
	friend inline flags_class<T> operator * (T e1, const flags_class<T>& e2) { return T((base_type)e1 & e2.get_value()); }
	friend inline flags_class<T> operator * (const flags_class<T>& e1, const flags_class<T>& e2) { return T(e1.get_value() & e2.get_value()); }
	friend inline flags_class<T> operator + (T e1, T e2) { return T(e1 | e2); }
	friend inline flags_class<T> operator + (const flags_class<T>& e1, T e2) { return T(e1.get_value() | (base_type)e2); }
	friend inline flags_class<T> operator + (T e1, const flags_class<T>& e2) { return T((base_type)e1 | e2.get_value()); }
	friend inline flags_class<T> operator + (const flags_class<T>& e1, const flags_class<T>& e2) { return T(e1.get_value() | e2.get_value()); }
	friend inline flags_class<T> operator - (T e1, T e2) { return T(e1 & ~e2); }
	friend inline flags_class<T> operator - (const flags_class<T>& e1, T e2) { return T(e1.get_value() & ~(base_type)e2); }
	friend inline flags_class<T> operator - (T e1, const flags_class<T>& e2) { return T((base_type)e1 & ~e2.get_value()); }
	friend inline flags_class<T> operator - (const flags_class<T>& e1, const flags_class<T>& e2) { return T(e1.get_value() & ~e2.get_value()); }
	friend inline flags_class<T> operator & (T e1, T e2) { return T(e1 & e2); }
	friend inline flags_class<T> operator & (const flags_class<T>& e1, T e2) { return T(e1.get_value() & (base_type)e2); }
	friend inline flags_class<T> operator & (T e1, const flags_class<T>& e2) { return T((base_type)e1 & e2.get_value()); }
	friend inline flags_class<T> operator & (const flags_class<T>& e1, const flags_class<T>& e2) { return T(e1.get_value() & e2.get_value()); }
	friend inline flags_class<T> operator | (T e1, T e2) { return T(e1 | e2); }
	friend inline flags_class<T> operator | (const flags_class<T>& e1, T e2) { return T(e1.get_value() | (base_type)e2); }
	friend inline flags_class<T> operator | (T e1, const flags_class<T>& e2) { return T((base_type)e1 | e2.get_value()); }
	friend inline flags_class<T> operator | (const flags_class<T>& e1, const flags_class<T>& e2) { return T(e1.get_value() | e2.get_value()); }
	friend inline flags_class<T> operator ^ (T e1, T e2) { return T(e1 ^ e2); }
	friend inline flags_class<T> operator ^ (const flags_class<T>& e1, T e2) { return T(e1.get_value() ^ (base_type)e2); }
	friend inline flags_class<T> operator ^ (T e1, const flags_class<T>& e2) { return T((base_type)e1 ^ e2.get_value()); }
	friend inline flags_class<T> operator ^ (const flags_class<T>& e1, const flags_class<T>& e2) { return T(e1.get_value() ^ e2.get_value()); }
	friend inline flags_class<T> operator ~ (T e2) { return T(~(base_type)e2); }
	friend inline flags_class<T> operator ~ (const flags_class<T>& e2) { return T(~e2.get_value()); }
	friend inline bool operator == (flags_class<T> const& a1, flags_class<T> const& a2) { return a1.get_value() == a2.get_value(); }
	friend inline bool operator != (flags_class<T> const& a1, flags_class<T> const& a2) { return a1.get_value() != a2.get_value(); }
	friend inline bool operator >= (flags_class<T> const& a1, flags_class<T> const& a2) { return a1.get_value() >= a2.get_value(); }
	friend inline bool operator <= (flags_class<T> const& a1, flags_class<T> const& a2) { return a1.get_value() <= a2.get_value(); }
	friend inline bool operator > (flags_class<T> const& a1, flags_class<T> const& a2) { return a1.get_value() > a2.get_value(); }
	friend inline bool operator < (flags_class<T> const& a1, flags_class<T> const& a2) { return a1.get_value() < a2.get_value(); }
	friend inline bool operator == (flags_class<T> const& a1, T a2) { return a1.get() == a2; }
	friend inline bool operator == (T a1, flags_class<T> const& a2) { return a1 == a2.get(); }
	friend inline bool operator != (flags_class<T> const& a1, T a2) { return a1.get() != a2; }
	friend inline bool operator != (T a1, flags_class<T> const& a2) { return a1 != a2.get(); }
	friend inline bool operator >= (flags_class<T> const& a1, T a2) { return a1.get() >= a2; }
	friend inline bool operator >= (T a1, flags_class<T> const& a2) { return a1 >= a2.get(); }
	friend inline bool operator <= (flags_class<T> const& a1, T a2) { return a1.get() <= a2; }
	friend inline bool operator <= (T a1, flags_class<T> const& a2) { return a1 <= a2.get(); }
	friend inline bool operator > (flags_class<T> const& a1, T a2) { return a1.get() > a2; }
	friend inline bool operator > (T a1, flags_class<T> const& a2) { return a1 > a2.get(); }
	friend inline bool operator < (flags_class<T> const& a1, T a2) { return a1.get() < a2; }
	friend inline bool operator < (T a1, flags_class<T> const& a2) { return a1 < a2.get(); }
};

#endif

namespace ang {
	//hack
	template<typename T> struct __gender_of<enum_class<T>, false, false, true, false> {
		static constexpr gender_t value = gender::enum_type;
	};

	template<typename T> struct __gender_of<flags_class<T>, false, false, true, false> {
		static constexpr gender_t value = gender::enum_type;
	};

	template<typename T>
	struct default_value<enum_class<T>, gender::enum_type> {
		static constexpr T value = T(0);
		inline operator T const& ()const { return value; }
		inline T const& get()const { return value; }
	};

	template<typename T>
	struct default_value<flags_class<T>, gender::enum_type> {
		static constexpr T value = T(0);
		inline operator T const& ()const { return value; }
		inline T const& get()const { return value; }
	};

	namespace runtime
	{
		template<typename T>
		struct type_info_builder<enum_class<T>, gender::enum_type> {
			static constexpr gender gender = gender::enum_type;
			static rtti_t const& type_of() {
				return enum_class<T>::class_info();
			}
		};

		template<typename T>
		struct type_info_builder<flags_class<T>, gender::enum_type> {
			static constexpr gender gender = gender::enum_type;
			static rtti_t const& type_of() {
				return flags_class<T>::class_info();
			}
		};
	}

	namespace algorithms
	{
		template<typename T>
		struct hash<enum_class<T>> {
			using type = typename remove_const<typename remove_reference<T>::type>::type;
			static ulong64 make(type const& value) {
				union { type k; ulong64 val; } key;
				key.val = 0;
				key.k = value;
				return (ulong64)((2475825 + key.val + 1));
			}
			ulong64 operator()(type const& value)const {
				return make(value);
			}
		};

	}
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WINDOWS_PLATFORM

#define declare_enum(_LINK, _name, _type)  enum class _name : _type; using _name##_t = enum_class<_name>; \
	template<> struct _LINK ::enum_class<_name> : public ang::value<_name> { \
		using type = typename ang::value<_name>::type; \
		using base = typename ang::value<_name>; \
		static rtti_t const& class_info(); \
		static _name##_t parse(cstr_t);\
		enum_class() : base(default_value<type>::value) {} \
		enum_class(type const& v) : base(v) {} \
		enum_class(_name##_t const& v) : base(v) {} \
		enum_class(type && v) : base(ang::forward<type>(v)) {	} \
		enum_class(_name##_t && v) : base(ang::forward<base>(v)) { } \
		ang::castr_t to_string()const; \
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


#define declare_flags(_LINK, _name, _type) enum class _name : _type; using _name##_t = flags_class<_name>; \
	template<> struct _LINK ::flags_class<_name> : public ang::value<_name> { \
		using type = typename ang::value<_name>::type; \
		using base_type = typename ang::value<_name>::base; \
		using base = typename ang::value<_name>; \
		static rtti_t const& class_info(); \
		flags_class() : base(default_value<type>::value) {} \
		flags_class(type const& v) : base(v) {} \
		flags_class(_name##_t const& v) : base(v) {} \
		flags_class(type && v) : base(ang::forward<type>(v)) {	} \
		flags_class(_name##_t && v) : base(ang::forward<base>(v)) { } \
		ang::fast_astring_t to_string()const; \
		bool is_active(_name##_t v)const; \
		_name##_t& operator = (type const& v){ get() = v; return*this; } \
		_name##_t& operator = (_name##_t const& v) { get() = v.get(); return*this; } \
		_name##_t& operator = (type && v) { get() = ang::move(v); v = default_value<type>::value; return*this; } \
		_name##_t& operator = (_name##_t && v) { get() = ang::move(v.get()); v.set(default_value<type>::value); return*this; } \
		_name##_t& operator ^= (const _name##_t& v) { m_value ^= (base_type)v.get(); return*this; } \
		_name##_t& operator ^= (type v) { m_value ^= (base_type)v;	return*this; } \
		_name##_t& operator *= (const _name##_t& v) { m_value &= (base_type)v.get(); return*this; } \
		_name##_t& operator *= (type v) { m_value &= (base_type)v;	return*this; } \
		_name##_t& operator += (const _name##_t& v) { m_value |= (base_type)v.get(); return*this; } \
		_name##_t& operator += (type v) { m_value |= (base_type)v;	return*this; } \
		_name##_t& operator -= (const _name##_t& v) { m_value &= ~(base_type)v.get(); return*this; } \
		_name##_t& operator -= (type v) { m_value &= ~(base_type)v;	return*this; } \
		bool operator !(void)const { return m_value == 0; } \
		operator bool(void)const { return m_value != 0; } \
		operator base_type(void)const { return m_value; } \
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

#define declare_enum_flags(_LINK, _name, _flag_name, _type) enum class _name : _type; using _flag_name = _name;\
	 using _name##_t = enum_class<_name>;  using _flag_name##_t = flags_class<_flag_name>;\
	template<> struct _LINK ::enum_class<_name> : public ang::value<_name>{ \
		using type = typename ang::value<_name>::type; \
		using base = typename ang::value<_name>; \
		static rtti_t const& class_info(); \
		static _name##_t parse(cstr_t);\
		enum_class() : base(default_value<type>::value) {} \
		enum_class(type const& v) : base(v) {} \
		enum_class(_name##_t const& v) : base(v) {} \
		enum_class(type && v) : base(ang::forward<type>(v)) {	} \
		enum_class(_name##_t && v) : base(ang::forward<base>(v)) { } \
		ang::castr_t to_string()const; \
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
	}; \
	template<> struct _LINK ::flags_class<_flag_name> : public ang::value<_flag_name> { \
		using type = typename ang::value<_name>::type; \
		using base_type = typename ang::value<_name>::base; \
		using base = typename ang::value<_name>; \
		static rtti_t const& class_info(); \
		flags_class() : base(default_value<type>::value) {} \
		flags_class(type const& v) : base(v) {} \
		flags_class(_flag_name##_t const& v) : base(v) {} \
		flags_class(type && v) : base(ang::forward<type>(v)) {	} \
		flags_class(_flag_name##_t && v) : base(ang::forward<base>(v)) { } \
		ang::fast_astring_t to_string()const; \
		bool is_active(_flag_name##_t v)const; \
		_flag_name##_t& operator = (type const& v){ get() = v; return*this; } \
		_flag_name##_t& operator = (_flag_name##_t const& v) { get() = v.get(); return*this; } \
		_flag_name##_t& operator = (type && v) { get() = ang::move(v); v = default_value<type>::value; return*this; } \
		_flag_name##_t& operator = (_flag_name##_t && v) { get() = ang::move(v.get()); v.set(default_value<type>::value); return*this; } \
		_flag_name##_t& operator ^= (const _flag_name##_t& v) { m_value ^= (base_type)v.get(); return*this; } \
		_flag_name##_t& operator ^= (type v) { m_value ^= (base_type)v;	return*this; } \
		_flag_name##_t& operator *= (const _flag_name##_t& v) { m_value &= (base_type)v.get(); return*this; } \
		_flag_name##_t& operator *= (type v) { m_value &= (base_type)v;	return*this; } \
		_flag_name##_t& operator += (const _flag_name##_t& v) { m_value |= (base_type)v.get(); return*this; } \
		_flag_name##_t& operator += (type v) { m_value |= (base_type)v;	return*this; } \
		_flag_name##_t& operator -= (const _flag_name##_t& v) { m_value &= ~(base_type)v.get(); return*this; } \
		_flag_name##_t& operator -= (type v) { m_value &= ~(base_type)v;	return*this; } \
		bool operator !(void)const { return m_value == 0; } \
		operator bool(void)const { return m_value != 0; } \
		operator base_type(void)const { return m_value; } \
		friend _LINK _flag_name##_t operator * (type, type); \
		friend _LINK _flag_name##_t operator * (const _flag_name##_t&, type); \
		friend _LINK _flag_name##_t operator * (type, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator * (const _flag_name##_t&, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator + (type, type); \
		friend _LINK _flag_name##_t operator + (const _flag_name##_t&, type); \
		friend _LINK _flag_name##_t operator + (type, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator + (const _flag_name##_t&, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator - (type, type); \
		friend _LINK _flag_name##_t operator - (const _flag_name##_t&, type); \
		friend _LINK _flag_name##_t operator - (type, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator - (const _flag_name##_t&, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator & (type, type); \
		friend _LINK _flag_name##_t operator & (const _flag_name##_t&, type); \
		friend _LINK _flag_name##_t operator & (type, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator & (const _flag_name##_t&, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator | (type, type); \
		friend _LINK _flag_name##_t operator | (const _flag_name##_t&, type); \
		friend _LINK _flag_name##_t operator | (type, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator | (const _flag_name##_t&, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator ^ (type, type); \
		friend _LINK _flag_name##_t operator ^ (const _flag_name##_t&, type); \
		friend _LINK _flag_name##_t operator ^ (type, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator ^ (const _flag_name##_t&, const _flag_name##_t&); \
		friend _LINK _flag_name##_t operator ~ (type); \
		friend _LINK _flag_name##_t operator ~ (_flag_name##_t const&); \
		friend _LINK bool operator == (_flag_name##_t const&, _flag_name##_t const&); \
		friend _LINK bool operator != (_flag_name##_t const&, _flag_name##_t const&); \
		friend _LINK bool operator >= (_flag_name##_t const&, _flag_name##_t const&); \
		friend _LINK bool operator <= (_flag_name##_t const&, _flag_name##_t const&); \
		friend _LINK bool operator > (_flag_name##_t const&, _flag_name##_t const&); \
		friend _LINK bool operator < (_flag_name##_t const&, _flag_name##_t const&); \
		friend _LINK bool operator == (_flag_name##_t const&, type); \
		friend _LINK bool operator != (_flag_name##_t const&, type); \
		friend _LINK bool operator >= (_flag_name##_t const&, type); \
		friend _LINK bool operator <= (_flag_name##_t const&, type); \
		friend _LINK bool operator > (_flag_name##_t const&, type); \
		friend _LINK bool operator < (_flag_name##_t const&, type); \
		friend _LINK bool operator == (type, _flag_name##_t const&); \
		friend _LINK bool operator != (type, _flag_name##_t const&); \
		friend _LINK bool operator >= (type, _flag_name##_t const&); \
		friend _LINK bool operator <= (type, _flag_name##_t const&); \
		friend _LINK bool operator > (type, _flag_name##_t const&); \
		friend _LINK bool operator < (type, _flag_name##_t const&); \
	}; enum class _name : _type


#define ANG_FLAGS_DECLARATION(_LINK, _FULLNAME) \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _FULLNAME)

#define ANG_ENUM_FLAGS_DECLARATION(_LINK, _FULLNAME) \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _FULLNAME)

#define ANG_ENUM_DECLARATION(_LINK, _FULLNAME) \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _FULLNAME)


#define ANG_ENUM_TO_STRING_IMPLEMENT(_FULLNAME, MAP, DEF_VAL) \
ang::castr_t _FULLNAME##_t::to_string()const { \
	wsize idx = ang::algorithms::binary_search<_FULLNAME>(this->get(), ang::to_array(MAP)); \
	if (idx > ang::algorithms::array_size(MAP)) return DEF_VAL; \
	else return MAP[idx].value; \
}

#define ANG_ENUM_PARSE_IMPLEMENT(_FULLNAME, MAP, DEF_VAL) \
_FULLNAME##_t _FULLNAME##_t::parse(ang::cstr_t str) { \
	auto idx = ang::algorithms::binary_search(str, ang::to_array(MAP)); \
	if (idx >= ang::algorithms::array_size(MAP)) return DEF_VAL; \
	else return MAP[idx].value; \
}

#define ANG_FLAGS_TO_STRING_IMPLEMENT(_FULLNAME, MAP) \
ang::fast_astring_t _FULLNAME##_t::to_string()const { \
	ang::fast_astring_t out; \
	_FULLNAME##_t val = get(); \
	bool first = true; \
	for (int i = ang::algorithms::array_size(MAP) - 1; i >= 0; --i) { \
		if (val.get() == default_value<type>::value) break; \
		if (val.is_active(MAP[i].key)) { \
			if (first) { \
				first = false;  \
				out << MAP[i].value;  \
			} else out << " + "_sv << MAP[i].value; \
			val -= MAP[i].key; \
		} \
	} \
	return ang::move(out); \
}


#define ANG_FLAGS_IMPLEMENT(_NAMESPACE, _name) \
	bool flags_class<_NAMESPACE::_name>::is_active(_NAMESPACE::_name##_t v)const{ return ((base_type)v.get() & m_value) == (base_type)v.get(); } \
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
	_NAMESPACE::_name##_t _NAMESPACE::operator * (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 & (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator * (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value & (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator * (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 & e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator * (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value & e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator + (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 | (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator + (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value | (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator + (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 | e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator + (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value | e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator - (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 & ~(typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator - (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value & ~(typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator - (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 & ~e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator - (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value & ~e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator & (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 & (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator & (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value & (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator & (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 & e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator & (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value & e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator | (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 | (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator | (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value | (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator | (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 | e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator | (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value | e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ^ (_NAMESPACE::_name e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 ^ (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ^ (const _NAMESPACE::_name##_t& e1, _NAMESPACE::_name e2) { return _NAMESPACE::_name(e1.m_value ^ (typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ^ (_NAMESPACE::_name e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name((typename _name##_t::base_type)e1 ^ e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ^ (const _NAMESPACE::_name##_t& e1, const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(e1.m_value ^ e2.m_value); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ~ (_NAMESPACE::_name e2) { return _NAMESPACE::_name(~(typename _name##_t::base_type)e2); } \
	_NAMESPACE::_name##_t _NAMESPACE::operator ~ (const _NAMESPACE::_name##_t& e2) { return _NAMESPACE::_name(~e2.m_value); } \
	ANG_ENUM_IMPLEMENT(_NAMESPACE, _name)


#define ANG_ENUM_FLAGS_IMPLEMENT(_NAMESPACE, _name, _flag_name) \
	bool flags_class<_NAMESPACE::_flag_name>::is_active(_NAMESPACE::_flag_name##_t v)const{ return ((base_type)v.get() & m_value) == (base_type)v.get(); } \
	bool _NAMESPACE::operator == (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name##_t const& second) { return first.get() == second.get(); } \
	bool _NAMESPACE::operator != (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name##_t const& second) { return first.get() != second.get(); } \
	bool _NAMESPACE::operator >= (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name##_t const& second) { return first.get() >= second.get(); } \
	bool _NAMESPACE::operator <= (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name##_t const& second) { return first.get() <= second.get(); } \
	bool _NAMESPACE::operator > (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name##_t const& second) { return first.get() > second.get(); } \
	bool _NAMESPACE::operator < (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name##_t const& second) { return first.get() < second.get(); } \
	bool _NAMESPACE::operator == (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name second) { return first.get() == second; } \
	bool _NAMESPACE::operator != (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name second) { return first.get() != second; } \
	bool _NAMESPACE::operator >= (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name second) { return first.get() >= second; } \
	bool _NAMESPACE::operator <= (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name second) { return first.get() <= second; } \
	bool _NAMESPACE::operator > (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name second) { return first.get() > second; } \
	bool _NAMESPACE::operator < (_NAMESPACE::_flag_name##_t const& first, _NAMESPACE::_flag_name second) { return first.get() < second; } \
	bool _NAMESPACE::operator == (_NAMESPACE::_flag_name first, _NAMESPACE::_flag_name##_t const& second) { return first == second.get(); } \
	bool _NAMESPACE::operator != (_NAMESPACE::_flag_name first, _NAMESPACE::_flag_name##_t const& second) { return first != second.get(); } \
	bool _NAMESPACE::operator >= (_NAMESPACE::_flag_name first, _NAMESPACE::_flag_name##_t const& second) { return first >= second.get(); } \
	bool _NAMESPACE::operator <= (_NAMESPACE::_flag_name first, _NAMESPACE::_flag_name##_t const& second) { return first <= second.get(); } \
	bool _NAMESPACE::operator > (_NAMESPACE::_flag_name first, _NAMESPACE::_flag_name##_t const& second) { return first > second.get(); } \
	bool _NAMESPACE::operator < (_NAMESPACE::_flag_name first, _NAMESPACE::_flag_name##_t const& second) { return first < second.get(); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator * (_NAMESPACE::_flag_name e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 & (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator * (const _NAMESPACE::_flag_name##_t& e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name(e1.m_value & (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator * (_NAMESPACE::_flag_name e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 & e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator * (const _NAMESPACE::_flag_name##_t& e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name(e1.m_value & e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator + (_NAMESPACE::_flag_name e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 | (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator + (const _NAMESPACE::_flag_name##_t& e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name(e1.m_value | (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator + (_NAMESPACE::_flag_name e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 | e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator + (const _NAMESPACE::_flag_name##_t& e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name(e1.m_value | e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator - (_NAMESPACE::_flag_name e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 & ~(typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator - (const _NAMESPACE::_flag_name##_t& e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name(e1.m_value & ~(typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator - (_NAMESPACE::_flag_name e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 & ~e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator - (const _NAMESPACE::_flag_name##_t& e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name(e1.m_value & ~e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator & (_NAMESPACE::_flag_name e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 & (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator & (const _NAMESPACE::_flag_name##_t& e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name(e1.m_value & (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator & (_NAMESPACE::_flag_name e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 & e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator & (const _NAMESPACE::_flag_name##_t& e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name(e1.m_value & e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator | (_NAMESPACE::_flag_name e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 | (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator | (const _NAMESPACE::_flag_name##_t& e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name(e1.m_value | (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator | (_NAMESPACE::_flag_name e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 | e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator | (const _NAMESPACE::_flag_name##_t& e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name(e1.m_value | e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator ^ (_NAMESPACE::_flag_name e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 ^ (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator ^ (const _NAMESPACE::_flag_name##_t& e1, _NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name(e1.m_value ^ (typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator ^ (_NAMESPACE::_flag_name e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name((typename _flag_name##_t::base_type)e1 ^ e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator ^ (const _NAMESPACE::_flag_name##_t& e1, const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name(e1.m_value ^ e2.m_value); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator ~ (_NAMESPACE::_flag_name e2) { return _NAMESPACE::_flag_name(~(typename _flag_name##_t::base_type)e2); } \
	_NAMESPACE::_flag_name##_t _NAMESPACE::operator ~ (const _NAMESPACE::_flag_name##_t& e2) { return _NAMESPACE::_flag_name(~e2.m_value); } \
	ANG_ENUM_RTTI(_NAMESPACE, _flag_name) ANG_ENUM_RTTI(_NAMESPACE, _name) \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(_NAMESPACE::_name) 

#define ANG_ENUM_IMPLEMENT(_NAMESPACE, _name) \
	ANG_ENUM_RTTI(_NAMESPACE, _name) \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(_NAMESPACE::_name)

#define FUNCX_TYPE_OF_DIR(A0) &ang::rtti::type_of<A0>()

#define ANG_ENUM_RTTI(_NAMESPACE, _name) \
	ang::rtti_t const& _NAMESPACE::_name##_t::class_info() { \
		static ang::inherit_info_t parent[] = { {&ang::type_of<ang::value<_NAMESPACE::_name>>(), 0} }; \
		return ang::rtti::regist(ang::type_name_t(#_NAMESPACE"::"#_name), ang::gender::enum_type, ang::size_of<_NAMESPACE::_name##_t>(), ang::align_of<_NAMESPACE::_name##_t>(), parent, nullptr); \
	}

#else

#define declare_enum(_LINK, _name, _type)  \
	enum class _name : _type; \
	using _name##_t = ::enum_class<_name>; \
	enum class _name : _type


#define declare_flags(_LINK, _name, _type) \
	enum class _name : _type; \
	using _name##_t = ::flags_class<_name>; \
	enum class _name : _type

#define declare_enum_flags(_LINK, _name, _flag_name, _type) \
	enum class _name : _type; \
	using _flag_name = _name; \
	using _name##_t = ::enum_class<_name>; \
	using _flag_name##_t = ::flags_class<_flag_name>; \
	enum class _name : _type


#define ANG_FLAGS_DECLARATION(_LINK, _FULLNAME) \
	template<> _LINK ang::fast_astring_t flags_class<_FULLNAME>::to_string()const; \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _FULLNAME)

#define ANG_ENUM_FLAGS_DECLARATION(_LINK, _FULLNAME) \
	template<> _LINK enum_class<_FULLNAME> enum_class<_FULLNAME>::parse(ang::cstr_t); \
	template<> _LINK ang::fast_astring_t flags_class<_FULLNAME>::to_string()const; \
	template<> _LINK ang::castr_t enum_class<_FULLNAME>::to_string()const; \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _FULLNAME)

#define ANG_ENUM_DECLARATION(_LINK, _FULLNAME) \
	template<> _LINK enum_class<_FULLNAME> enum_class<_FULLNAME>::parse(ang::cstr_t); \
	template<> _LINK ang::castr_t enum_class<_FULLNAME>::to_string()const; \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _FULLNAME)


#define ANG_ENUM_TO_STRING_IMPLEMENT(_FULLNAME, MAP, DEF_VAL) \
template<> ang::castr_t _FULLNAME##_t::to_string()const { \
	wsize idx = ang::algorithms::binary_search<_FULLNAME>(this->get(), ang::to_array(MAP)); \
	if (idx > ang::algorithms::array_size(MAP)) return DEF_VAL; \
	else return MAP[idx].value; \
}

#define ANG_ENUM_PARSE_IMPLEMENT(_FULLNAME, MAP, DEF_VAL) \
template<> _FULLNAME##_t _FULLNAME##_t::parse(ang::cstr_t str) { \
	auto idx = ang::algorithms::binary_search(str, ang::to_array(MAP)); \
	if (idx >= ang::algorithms::array_size(MAP)) return DEF_VAL; \
	else return MAP[idx].value; \
}

#define ANG_FLAGS_IMPLEMENT(_NAMESPACE, _name) \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(_NAMESPACE::_name)


#define ANG_ENUM_FLAGS_IMPLEMENT(_NAMESPACE, _name, _flag_name) \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(_NAMESPACE::_name)

#define ANG_ENUM_IMPLEMENT(_NAMESPACE, _name) \
	ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(_NAMESPACE::_name)

#define ANG_ENUM_RTTI(_NAMESPACE, _name) 

#endif

#ifdef WINDOWS_PLATFORM
namespace ang
{
	namespace text
	{
		using encoding_t = enum_class<encoding>;
		template<> struct LINK enum_class<encoding> : public ang::value<encoding>
		{
			static encoding_t parse(cstr_t);
			static rtti_t const& class_info();
			enum_class() : value(default_value<type>::value) {}
			enum_class(type const& v) : value(v) {}
			enum_class(encoding_t const& v) : value(v) {}
			enum_class(type && v) : value(ang::forward<type>(v)) { }
			enum_class(encoding_t && v) : value(ang::forward<value>(v)) { }
			castr_t to_string()const;
			encoding_t& operator = (type const& v) { get() = v; return*this; }
			encoding_t& operator = (encoding_t const& v) { get() = v.get(); return*this; }
			encoding_t& operator = (type && v) { get() = ang::move(v); v = default_value<type>::value; return*this; }
			encoding_t& operator = (encoding_t && v) { get() = ang::move(v.get()); v.set(default_value<type>::value); return*this; }
			friend inline bool operator == (encoding_t const& a1, encoding_t const& a2) { return a1.get() == a2.get(); }
			friend inline bool operator != (encoding_t const& a1, encoding_t const& a2) { return a1.get() != a2.get(); }
			friend inline bool operator >= (encoding_t const& a1, encoding_t const& a2) { return a1.get() >= a2.get(); }
			friend inline bool operator <= (encoding_t const& a1, encoding_t const& a2) { return a1.get() <= a2.get(); }
			friend inline bool operator > (encoding_t const& a1, encoding_t const& a2) { return a1.get() > a2.get(); }
			friend inline bool operator < (encoding_t const& a1, encoding_t const& a2) { return a1.get() < a2.get(); }
			friend inline bool operator == (encoding_t const& a1, encoding a2) { return a1.get() == a2; }
			friend inline bool operator == (encoding a1, encoding_t const& a2) { return a1 == a2.get(); }
			friend inline bool operator != (encoding_t const& a1, encoding a2) { return a1.get() != a2; }
			friend inline bool operator != (encoding a1, encoding_t const& a2) { return a1 != a2.get(); }
			friend inline bool operator >= (encoding_t const& a1, encoding a2) { return a1.get() >= a2; }
			friend inline bool operator >= (encoding a1, encoding_t const& a2) { return a1 >= a2.get(); }
			friend inline bool operator <= (encoding_t const& a1, encoding a2) { return a1.get() <= a2; }
			friend inline bool operator <= (encoding a1, encoding_t const& a2) { return a1 <= a2.get(); }
			friend inline bool operator > (encoding_t const& a1, encoding a2) { return a1.get() > a2; }
			friend inline bool operator > (encoding a1, encoding_t const& a2) { return a1 > a2.get(); }
			friend inline bool operator < (encoding_t const& a1, encoding a2) { return a1.get() < a2; }
			friend inline bool operator < (encoding a1, encoding_t const& a2) { return a1 < a2.get(); }
		};
	}
}

#else
namespace ang
{
	namespace text
	{
		using encoding_t = enum_class<encoding>;
	}
}

#endif

ANG_ENUM_DECLARATION(LINK, ang::text::encoding);

#endif//__ANG_BASE_VALUE_H__
