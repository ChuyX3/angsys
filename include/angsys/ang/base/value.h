#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_VALUE_H__
#define __ANG_BASE_VALUE_H__

namespace ang //constants
{

	template<typename T, typename = void> struct value {
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
		T const& get() { return _value; }
		void set(T const& v) { _value = v; }
		void move(T && v) {
			_value = ang::forward<T>(v);
			v = default_value<T>::value;
		}

	private:
		T _value;
	};

	template<typename T>
	struct value<T, void_t<typename T::enum_t>> : public T {
		typedef typename T::enum_t type;
		value() {}
		value(type const& v) { _value = v; }
		value(value const& v) { _value = v._value; }
		value(type && v) {
			_value = v;
			v = default_value<type>::value;
		}
		value(value && v) {
			_value = ang::move(v._value);
			v._value = default_value<type>::value;
		}

		operator type& () { return get(); }
		operator type const& ()const { return get(); }

		value& operator = (type const& v) {
			_value = v;
			return*this;
		}
		value& operator = (value const& v) {
			_value = v._value;
			return*this;
		}
		value& operator = (type && v) {
			_value = v;
			v = default_value<type>::value;
			return*this;
		}
		value& operator = (value && v) {
			_value = ang::move(v._value);
			v._value = default_value<type>::value;
			return*this;
		}

		type& get() { return *reinterpret_cast<type*>(&_value); }
		type const& get()const { return *reinterpret_cast<type const*>(&_value); }
		void set(type const& v) { _value = v; }
		void move(type && v) {
			_value = v;
			v = default_value<type>::value;
		}
	};


	template<typename E>
	struct safe_flag : public E, public value<typename E::type> {
		typedef typename E::type type;
		typedef typename E::base enum_base;
		safe_flag() { }
		safe_flag(safe_flag const& e) { E::value_ = e.get(); }
		safe_flag(type e) { E::value_ = e; }
		enum_base get()const { return E::value_; }
		void set(enum_base value) { E::value_ = value; }
		safe_flag& operator = (const safe_flag& value) { set(value.get()); return*this; }
		safe_flag& operator = (type value) { set(value);	return*this; }
		safe_flag& operator |= (const safe_flag& value) { E::value_ |= value.get(); return*this; }
		safe_flag& operator |= (type value) { E::value_ |= value;	return*this; }
		safe_flag& operator &= (const safe_flag& value) { E::value_ &= value.get(); return*this; }
		safe_flag& operator &= (type value) { E::value_ &= value;	return*this; }
		safe_flag& operator ^= (const safe_flag& value) { E::value_ ^= value.get(); return*this; }
		safe_flag& operator ^= (type value) { E::value_ ^= value;	return*this; }
		safe_flag& operator *= (const safe_flag& value) { E::value_ &= value.get(); return*this; }
		safe_flag& operator *= (type value) { E::value_ &= value;	return*this; }
		safe_flag& operator += (const safe_flag& value) { E::value_ |= value.get(); return*this; }
		safe_flag& operator += (type value) { E::value_ |= value;	return*this; }
		safe_flag& operator -= (const safe_flag& value) { E::value_ &= ~value.get(); return*this; }
		safe_flag& operator -= (type value) { E::value_ &= ~value;	return*this; }
		operator type()const { return (type)E::value_; }
		operator enum_base()const { return E::value_; }
		inline bool is_active(type flag)const {
			if ((E::value_ & (enum_base)flag) == (enum_base)flag) return true;
			return false;
		}
		inline bool is_active(safe_flag const& flag)const {
			if ((E::value_ & flag.get()) == flag.get()) return true;
			return false;
		}

	protected:
		safe_flag(enum_base e) { E::value_ = e; }

	public:
		friend safe_flag<E> operator * (type first, type second) { return first & second; }
		friend safe_flag<E> operator * (const safe_flag<E>& first, type second) { return first.get() & second; }
		friend safe_flag<E> operator * (type first, const safe_flag<E>& second) { return first & second.get(); }
		friend safe_flag<E> operator * (const safe_flag<E>& first, const safe_flag<E>& second) { return first.get() & second.get(); }
		friend safe_flag<E> operator + (type first, type second) { return first | second; }
		friend safe_flag<E> operator + (const safe_flag<E>& first, type second) { return first.get() | second; }
		friend safe_flag<E> operator + (type first, const safe_flag<E>& second) { return first | second.get(); }
		friend safe_flag<E> operator + (const safe_flag<E>& first, const safe_flag<E>& second) { return first.get() | second.get(); }
		friend safe_flag<E> operator - (type first, type second) { return first & ~second; }
		friend safe_flag<E> operator - (const safe_flag<E>& first, type second) { return first.get() & ~second; }
		friend safe_flag<E> operator - (type first, const safe_flag<E>& second) { return first & ~second.get(); }
		friend safe_flag<E> operator - (const safe_flag<E>& first, const safe_flag<E>& second) { return first.get() & ~second.get(); }
		friend safe_flag<E> operator & (type first, type second) { return first & ~second; }
		friend safe_flag<E> operator & (const safe_flag<E>& first, type second) { return first.get() & second; }
		friend safe_flag<E> operator & (type first, const safe_flag<E>& second) { return first & second.get(); }
		friend safe_flag<E> operator & (const safe_flag<E>& first, const safe_flag<E>& second) { return first.get() & second.get(); }
		friend safe_flag<E> operator | (type first, type second) { return first | second; }
		friend safe_flag<E> operator | (const safe_flag<E>& first, type second) { return first.get() | second; }
		friend safe_flag<E> operator | (type first, const safe_flag<E>& second) { return first | second.get(); }
		friend safe_flag<E> operator | (const safe_flag<E>& first, const safe_flag<E>& second) { return first.get() | second.get(); }
		friend safe_flag<E> operator ^ (type first, type second) { return first ^ second; }
		friend safe_flag<E> operator ^ (const safe_flag<E>& first, type second) { return first.get() ^ second; }
		friend safe_flag<E> operator ^ (type first, const safe_flag<E>& second) { return first ^ second.get(); }
		friend safe_flag<E> operator ^ (const safe_flag<E>& first, const safe_flag<E>& second) { return first.get() ^ second.get(); }
		friend safe_flag<E> operator ~ (type first) { return ~first; }
		friend safe_flag<E> operator ~ (safe_flag<E> const& first) { return ~first.get(); }
		friend bool operator == (safe_flag<E> const& first, safe_flag<E> const& second) { return first.get() == second.get(); }
		friend bool operator != (safe_flag<E> const& first, safe_flag<E> const& second) { return first.get() != second.get(); }
		friend bool operator >= (safe_flag<E> const& first, safe_flag<E> const& second) { return first.get() >= second.get(); }
		friend bool operator <= (safe_flag<E> const& first, safe_flag<E> const& second) { return first.get() <= second.get(); }
		friend bool operator > (safe_flag<E> const& first, safe_flag<E> const& second) { return first.get() > second.get(); }
		friend bool operator < (safe_flag<E> const& first, safe_flag<E> const& second) { return first.get() < second.get(); }
		friend bool operator == (safe_flag<E> const& first, type second) { return first.get() == second; }
		friend bool operator != (safe_flag<E> const& first, type second) { return first.get() != second; }
		friend bool operator >= (safe_flag<E> const& first, type second) { return first.get() >= second; }
		friend bool operator <= (safe_flag<E> const& first, type second) { return first.get() <= second; }
		friend bool operator > (safe_flag<E> const& first, type second) { return first.get() > second; }
		friend bool operator < (safe_flag<E> const& first, type second) { return first.get() < second; }
		friend bool operator == (type first, safe_flag<E> const& second) { return first == second.get(); }
		friend bool operator != (type first, safe_flag<E> const& second) { return first != second.get(); }
		friend bool operator >= (type first, safe_flag<E> const& second) { return first >= second.get(); }
		friend bool operator <= (type first, safe_flag<E> const& second) { return first <= second.get(); }
		friend bool operator > (type first, safe_flag<E> const& second) { return first > second.get(); }
		friend bool operator < (type first, safe_flag<E> const& second) { return first < second.get(); }
	};


#define safe_enum(_LINK, _name, _type) struct _name##_base { enum enum_t : _type; protected: enum_t _value; }; \
	struct _LINK _name##_t : public ang::value<_name##_base> { \
			_name##_t() : value(default_value<type>::value) {} \
			_name##_t(type const& v) : value(v) {} \
			_name##_t(value const& v) : value(v) {} \
			_name##_t(type && v) : value(ang::forward<type>(v)) {	} \
			_name##_t(value && v) : value(ang::forward<value>(v)) { } \
			_name##_t& operator = (type const& v){ _value = v; return*this; } \
			_name##_t& operator = (value const& v) { _value = v.get(); return*this; } \
			_name##_t& operator = (type && v) { _value = ang::move(v); v = default_value<type>::value; return*this; } \
			_name##_t& operator = (value && v) { _value = ang::move(v.get()); v.set(default_value<type>::value); return*this; } \
	}; typedef _name##_base::enum_t _name; enum _name##_base::enum_t : _type


#define safe_flags(_LINK, _name, _type) struct _name##_base { enum enum_t : _type; protected: _type _value; }; \
	struct _LINK _name##_t : public ang::value<_name##_base> { \
			_name##_t() : value(default_value<type>::value) {} \
			_name##_t(type const& v) : value(v) {} \
			_name##_t(value const& v) : value(v) {} \
			_name##_t(type && v) : value(ang::forward<type>(v)) {	} \
			_name##_t(value && v) : value(ang::forward<value>(v)) { } \
			_name##_t& operator = (type const& v){ _value = v; return*this; } \
			_name##_t& operator = (value const& v) { _value = v.get(); return*this; } \
			_name##_t& operator = (type && v) { _value = ang::move(v); v = default_value<type>::value; return*this; } \
			_name##_t& operator = (value && v) { _value = ang::move(v.get()); v.set(default_value<type>::value); return*this; } \
	}; typedef _name##_base::enum_t _name; enum _name##_base::enum_t : _type

}

#endif//__ANG_BASE_VALUE_H__