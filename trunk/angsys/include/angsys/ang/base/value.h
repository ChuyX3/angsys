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

	template<typename T, genre_t TYPE, bool IS_SAFE_ENUM>
	struct value {
		typedef T type;
		static constexpr genre_t genre_id = TYPE;

		value() : _value(default_value<T>::value) {}
		value(T const& v) : _value(v) {}
		value(value const& v) : _value(v._value) {}
		value(T && v) : _value(ang::move(v)) { 
			v = default_value<T>::value; 
		}
		value(value && v) : _value(ang::move(v._value)) {
			v._value = default_value<T>::value;
		}

		operator T& () { return get(); }
		operator T const& ()const { return get(); }

		value& operator = (T const& v){
			_value = v;
			return*this;
		}
		value& operator = (value const& v) {
			_value = v._value;
			return*this;
		}
		value& operator = (T && v) {
			_value = ang::move(v);
			v = default_value<T>::value;
			return*this;
		}
		value& operator = (value && v) {
			_value = ang::move(v._value);
			v._value = default_value<T>::value;
			return*this;
		}

		T& get() { return _value; }
		T const& get()const { return _value; }
		void set(T const& v) { _value = v; }
		void move(T && v) {
			_value = ang::forward<T>(v);
			v = default_value<T>::value;
		}

	private:
		T _value;
	};

	template<typename T>
	struct value<T, genre::union_type, false> {
		typedef T type;
		static constexpr genre_t genre_id = genre::union_type;

		value() : _value(default_value<T>::value) {}
		value(T const& v) : _value(v) {}
		value(value const& v) : _value(v._value) {}
		value(T && v) : _value(ang::move(v)) {
			v = default_value<T>::value;
		}
		value(value && v) : _value(ang::move(v._value)) {
			v._value = default_value<T>::value;
		}

		operator T& () { return get(); }
		operator T const& ()const { return get(); }

		value& operator = (T const& v) {
			_value = v;
			return*this;
		}
		value& operator = (value const& v) {
			_value = v._value;
			return*this;
		}
		value& operator = (T && v) {
			_value = ang::move(v);
			v = default_value<T>::value;
			return*this;
		}
		value& operator = (value && v) {
			_value = ang::move(v._value);
			v._value = default_value<T>::value;
			return*this;
		}

		T* operator ->() { return &get(); }
		T const* operator ->()const { return &get(); }

		T& get() { return _value; }
		T const& get()const { return _value; }
		void set(T const& v) { _value = v; }
		void move(T && v) {
			_value = ang::forward<T>(v);
			v = default_value<T>::value;
		}

	private:
		T _value;
	};

	template<typename T>
	struct value<T, genre::class_type, false> : public T {
		typedef T type;
		static constexpr genre_t genre_id = genre::class_type;

		value() {}
		value(T const& v) : T(ang::forward<T>(v)) { }
		value(value const& v) : T(ang::forward<value>(v)) { }
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
		void set(T const& v) { static_cast<T&>(*this) = ang::forward<T>(v); }
		void move(T && v) {
			static_cast<T&>(*this) = ang::forward<T>(v);
		}
	};

	template<typename T>
	struct value<T, genre::class_type, true> : public T {
		typedef typename T::type type;
		static constexpr genre_t genre_id = genre::enum_type;

		value() {}
		value(type const& v) { this->_value = v; }
		value(value const& v) { this->_value = v._value; }
		value(type && v) {
			this->_value = v;
			v = default_value<type>::value;
		}
		value(value && v) {
			this->_value = ang::move(v._value);
			v._value = default_value<type>::value;
		}

		operator type& () { return get(); }
		operator type const& ()const { return get(); }

		value& operator = (type const& v) {
			this->_value = v;
			return*this;
		}
		value& operator = (value const& v) {
			this->_value = v._value;
			return*this;
		}
		value& operator = (type && v) {
			this->_value = v;
			v = default_value<type>::value;
			return*this;
		}
		value& operator = (value && v) {
			this->_value = ang::move(v._value);
			v._value = default_value<type>::value;
			return*this;
		}


		T* operator ->() { return this; }
		T const* operator ->()const { return this; }

		type& get() { return *reinterpret_cast<type*>(&this->_value); }
		type const& get()const { return *reinterpret_cast<type const*>(&this->_value); }
		void set(type const& v) { this->_value = v; }
		void move(type && v) {
			this->_value = v;
			v = default_value<type>::value;
		}
	};

#define safe_enum(_LINK, _name, _type) struct _LINK _name##_proxy { enum type : _type; protected: type _value; }; \
	struct _LINK _name##_t : public ang::value<_name##_proxy> { \
			static rtti_t const& class_info(); \
			_name##_t() : value(default_value<type>::value) {} \
			_name##_t(type const& v) : value(v) {} \
			_name##_t(_name##_t const& v) : value(v) {} \
			_name##_t(type && v) : value(ang::forward<type>(v)) {	} \
			_name##_t(_name##_t && v) : value(ang::forward<value>(v)) { } \
			ang::cstr_t to_string()const; \
			_name##_t& operator = (type const& v){ _value = v; return*this; } \
			_name##_t& operator = (_name##_t const& v) { _value = v.get(); return*this; } \
			_name##_t& operator = (type && v) { _value = ang::move(v); v = default_value<type>::value; return*this; } \
			_name##_t& operator = (_name##_t && v) { _value = ang::move(v._value); v.set(default_value<type>::value); return*this; } \
	}; typedef _name##_proxy::type _name; enum _name##_proxy::type : _type


#define safe_flags(_LINK, _name, _type) struct _name##_proxy { enum type : _type; protected: _type _value; }; \
	struct _LINK _name##_t : public ang::value<_name##_proxy> { \
			static rtti_t const& class_info(); \
			_name##_t() : value(default_value<type>::value) {} \
			_name##_t(type const& v) : value(v) {} \
			_name##_t(_name##_t const& v) : value(v) {} \
			_name##_t(type && v) : value(ang::forward<type>(v)) {	} \
			_name##_t(_name##_t && v) : value(ang::forward<value>(v)) { } \
			_name##_t& operator = (type const& v){ _value = v; return*this; } \
			_name##_t& operator = (_name##_t const& v) { _value = v.get(); return*this; } \
			_name##_t& operator = (type && v) { _value = ang::move(v); v = default_value<type>::value; return*this; } \
			_name##_t& operator = (_name##_t && v) { _value = ang::move(v.get()); v.set(default_value<type>::value); return*this; } \
			_name##_t& operator ^= (const _name##_t& v) { _value ^= v.get(); return*this; } \
			_name##_t& operator ^= (type v) { _value ^= v;	return*this; } \
			_name##_t& operator *= (const _name##_t& v) { _value &= v.get(); return*this; } \
			_name##_t& operator *= (type v) { _value &= v;	return*this; } \
			_name##_t& operator += (const _name##_t& v) { _value |= v.get(); return*this; } \
			_name##_t& operator += (type v) { _value |= v;	return*this; } \
			_name##_t& operator -= (const _name##_t& v) { _value &= ~v.get(); return*this; } \
			_name##_t& operator -= (type v) { _value &= ~v;	return*this; } \
	}; typedef _name##_proxy::type _name; enum _name##_proxy::type : _type


#define FUNCX_TYPE_OF_DIR(A0) &ang::rtti::type_of<A0>()

#define safe_enum_rrti(_NAMESPACE, _NAME, ...) \
	ang::rtti_t const& _NAMESPACE::_NAME::class_info() { \
		ang::rtti_t const* parent[] = { ANG_EXPAND(APPLY_FUNCX_N(FUNCX_TYPE_OF_DIR, COMA_SEPARATOR, __VA_ARGS__)) }; \
		return rtti::regist(#_NAMESPACE"::"#_NAME, genre::enum_type, sizeof(_NAME), alignof(_NAME), null, null); \
	}

	namespace text
	{
		struct LINK encoding_t : public ang::value<encoding_proxy>
		{
			static encoding_t parse(cstr_t);
			static encoding_t parse(cwstr_t);
			static encoding_t parse(cmstr_t);
			static rtti_t const& class_info();
			encoding_t() : value(default_value<type>::value) {}
			encoding_t(type const& v) : value(v) {}
			encoding_t(encoding_t const& v) : value(v) {}
			encoding_t(type && v) : value(ang::forward<type>(v)) {	}
			encoding_t(encoding_t && v) : value(ang::forward<value>(v)) { }
			cstr_t to_string()const;
			encoding_t& operator = (type const& v) { _value = v; return*this; }
			encoding_t& operator = (encoding_t const& v) { _value = v.get(); return*this; }
			encoding_t& operator = (type && v) { _value = ang::move(v); v = default_value<type>::value; return*this; }
			encoding_t& operator = (encoding_t && v) { _value = ang::move(v._value); v.set(default_value<type>::value); return*this; }
		};
	}
}

#endif//__ANG_BASE_VALUE_H__
