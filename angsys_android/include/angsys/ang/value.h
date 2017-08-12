/*********************************************************************************************************************/
/*   File Name: ang/value.h                                                                                          */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file defines a wrapper for native types                                                  */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_VALUE_H__
#define __ANG_VALUE_H__

#include <ang/utils.h>

namespace ang
{
	template<typename T> struct value
	{
	public: /*type definitions*/
		typedef T type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value() : _value() {}
		value(type val) : _value(val) {}
		value(value const& other) : _value(other._value) {}
		value(value && other) : _value(ang::move(other._value)) {}		
		~value(){}
	public: /*getters and setters*/
		type & get() { return _value; }
		type const& get()const { return _value; }
		void set(type val) { _value = ang::move(val); }

	public: /*operators*/
		value& operator = (type val) { set(ang::move(val)); return*this; }
		value& operator = (value const& val) { set(val._value); return*this; }
		value& operator = (value && val) { set(ang::move(val._value)); return*this; }

		operator type& () { return get(); }
		operator type ()const { return get(); }

		type* operator -> () { return &get(); }
		type const* operator -> ()const { return &get(); }

		value<type*> operator & () { return value<type*>(&get(), 1); }
		value<type const*> operator & ()const { return value<type const*>(&get(),1); }
	};

	template<typename T>
	struct remove_value
	{
		typedef T type;
		typedef value<T> value_type;
	};

	template<typename T>
	struct remove_value<value<T>>
	{
		typedef T type;
		typedef value<T> value_type;
	};

	template<typename T> struct value<const T>
	{
	public: /*type definitions*/
		typedef T type;

	private: /*members*/
		const type _value;

	public: /*constructors*/
		value(type val) : _value(val) {}
		value(value const& other) : _value(other._value) {}
		~value() {}
	public: /*getters and setters*/
		type const& get()const { return _value; }

	public: /*operators*/
		operator type ()const { return get(); }
		type const* operator -> ()const { return &get(); }
		value<type const*> operator & ()const { return &get(); }
	};

	template<typename T> struct value<T&>
	{
	public: /*type definitions*/
		typedef T type;

	private: /*members*/
		type& _value;

	public: /*constructors*/
		value(type& val) : _value(val) {}
		value(value& other) : _value(other._value) {}
		value(value<T>& other) : _value(other.get()) {}
		~value() {}

	public: /*getters and setters*/
		type & get() { return _value; }
		type const& get()const { return _value; }
		void set(type val) { _value = ang::move(val); }

	public: /*operators*/
		value& operator = (type val) { set(ang::move(val)); return*this; }
		value& operator = (value const& val) { set(val._value); return*this; }
		value& operator = (value<T> val) { set(ang::move(val.get())); }

		operator type& () { return get(); }
		operator type ()const { return get(); }

		type* operator -> () { return &get(); }
		type const* operator -> ()const { return &get(); }

		value<type*> operator & () { return &get(); }
		value<type const*> operator & ()const { return &get(); }
	};

	template<typename T> struct value<const T&>
	{
	public: /*type definitions*/
		typedef T type;

	private: /*members*/
		const type& _value;

	public: /*constructors*/
		value(type const& val) : _value(val) {}
		value(value const& other) : _value(other._value) {}
		~value() {}
	public: /*getters and setters*/
		type const& get()const { return _value; }

	public: /*operators*/
		operator type ()const { return get(); }
		type const* operator -> ()const { return &get(); }
		value<type const*> operator & ()const { return &get(); }
	};

	template<typename T> struct value<T*>
	{
	public: /*type definitions*/
		typedef T* type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value() : _value(null) {}
		value(type val, uint size) : _value(val), _size(size){}
		value(ang::nullptr_t) : _value(null), _size(0) {}
		value(value<T>* val, uint size) : _value(&val->get()), _size(size) {}
		value(value const& other) : _value(other._value), _size(other._size) {}
		value(value && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
		template<wsize N>
		value(T(&ar)[N])
			: _value(ar), _size(N - 1U) { }
		~value() {}

	public: /*getters and setters*/
		type & get() { return _value; }
		type const& get()const { return _value; }
		void set(type val, uint size) { _value = ang::move(val); _size = ang::move(size);}
		type data()const { return _value; }
		uint size()const { return _size; }

	public: /*operators*/
		value& operator = (type val) { set(ang::move(val), 1); return*this; }
		value& operator = (value const& val) { set(val._value, val._size); return*this; }
		value& operator = (value && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
		template<wsize N> value& operator = (T(&ar)[N]) { set(ar, N - 1U); return*this; }

		operator type& () { return get(); }
		operator type ()const { return get(); }

		T& operator [](int idx) { return get()[idx]; }
		T const& operator [](int idx)const { return get()[idx]; }

		value<T>& operator * () { return *reinterpret_cast<value<T>*>(get()); }
		value<T> const& operator * ()const { return *reinterpret_cast<value<T> const*>(get()); }

		value<T**> operator & () { return &get(); }
		//value<T *const*> operator & ()const { return &get(); }
	};

	template<typename T> struct value<T const*>
	{
	public: /*type definitions*/
		typedef T const* type;

	private: /*members*/
		type _value;
		uint _size;

	public: /*constructors*/
		value() : _value(null), _size(0) {}
		value(type val, uint size) : _value(val), _size(size) {}
		value(ang::nullptr_t) : _value(null), _size(0) {}
		value(value<T> const* val, uint size) : _value(&val->get()), _size(size) {}
		value(value const& other) : _value(other._value), _size(other._size) {}
		value(value && other) : _value(ang::move(other._value)), _size(ang::move(other._size)) {}
		template<wsize N>
		value(const T(&ar)[N])
			: _value(ar), _size(N - 1U) { }
		~value() {}

	public: /*getters and setters*/
		type & get() { return _value; }
		type const& get()const { return _value; }
		void set(type val, uint size) { _value = ang::move(val); _size = ang::move(size); }
		type data()const { return _value; }
		uint size()const { return _size; }

	public: /*operators*/
		value& operator = (type val) { set(ang::move(val), 1); return*this; }
		value& operator = (value const& val) { set(val._value, val._size); return*this; }
		value& operator = (value && val) { set(ang::move(val._value), ang::move(val._size)); return*this; }
		template<wsize N> value& operator = (const T(&ar)[N]) { set(ar, N - 1U); return*this; }

		operator type& () { return get(); }
		operator type ()const { return get(); }

		T const& operator [](int idx)const { return get()[idx]; }

		value<T const> & operator * () { return *reinterpret_cast<value<T const>*>((void*)get()); }
		value<T const> const& operator * ()const { return *reinterpret_cast<value<T const> const*>((void*)get()); }

		//value<T const**> operator & () { return &get(); }
		//value<T const*const*> operator & ()const { return &get(); }
	};

	template<typename T> struct value<T**>
	{
	public: /*type definitions*/
		typedef T** type;

	private: /*members*/
		type _value;

	public: /*constructors*/
		value() : _value() {}
		value(type val) : _value(val) {}
		value(ang::nullptr_t) : _value(null) {}
		value(value const& other) : _value(other._value) {}
		value(value && other) : _value(ang::move(other._value)) {}
		~value() {}

	public: /*getters and setters*/
		type get()const { return _value; }
		void set(type val) { _value = ang::move(val); }

	public: /*operators*/
		value& operator = (type val) { set(ang::move(val)); return*this; }
		value& operator = (value const& val) { set(val._value); return*this; }
		value& operator = (value && val) { set(ang::move(val._value)); return*this; }
		operator type ()const { return get(); }
		T*& operator * () { return *get(); }
	};

	template<typename T1, typename T2> inline bool value_compare_same(T1 const& arg1, T2 const& arg2) { return arg1 == arg2; }
	template<typename T1, typename T2> inline bool value_compare_dif(T1 const& arg1, T2 const& arg2) { return arg1 != arg2; }
	template<typename T1, typename T2> inline bool value_compare_same_ma(T1 const& arg1, T2 const& arg2) { return arg1 >= arg2; }
	template<typename T1, typename T2> inline bool value_compare_same_mi(T1 const& arg1, T2 const& arg2) { return arg1 <= arg2; }
	template<typename T1, typename T2> inline bool value_compare_ma(T1 const& arg1, T2 const& arg2) { return arg1 > arg2; }
	template<typename T1, typename T2> inline bool value_compare_mi(T1 const& arg1, T2 const& arg2) { return arg1 < arg2; }

	template<typename T1, typename T2> inline typename operation_result<T1,T2>::type value_operation_add(T1 const& arg1, T2 const& arg2) { return arg1 + arg2; }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_sub(T1 const& arg1, T2 const& arg2) { return arg1 - arg2; }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_mul(T1 const& arg1, T2 const& arg2) { return arg1 * arg2; }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_div(T1 const& arg1, T2 const& arg2) { return arg1 / arg2; }

	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_add(value<T1> const& arg1, T2 const& arg2) { return arg1.get() + arg2; }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_sub(value<T1> const& arg1, T2 const& arg2) { return arg1.get() - arg2; }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_mul(value<T1> const& arg1, T2 const& arg2) { return arg1.get() * arg2; }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_div(value<T1> const& arg1, T2 const& arg2) { return arg1.get() / arg2; }

	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_add(T1 const& arg1, value<T2> const& arg2) { return arg1 + arg2.get(); }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_sub(T1 const& arg1, value<T2> const& arg2) { return arg1 - arg2.get(); }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_mul(T1 const& arg1, value<T2> const& arg2) { return arg1 * arg2.get(); }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_div(T1 const& arg1, value<T2> const& arg2) { return arg1 / arg2.get(); }

	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_add(value<T1> const& arg1, value<T2> const& arg2) { return arg1.get() + arg2.get(); }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_sub(value<T1> const& arg1, value<T2> const& arg2) { return arg1.get() - arg2.get(); }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_mul(value<T1> const& arg1, value<T2> const& arg2) { return arg1.get() * arg2.get(); }
	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_div(value<T1> const& arg1, value<T2> const& arg2) { return arg1.get() / arg2.get(); }

	template<typename T1, typename T2> inline bool operator == (value<T1> const& arg1, T2 arg2) { return value_compare_same(arg1.get(), arg2); }
	template<typename T1, typename T2> inline bool operator != (value<T1> const& arg1, T2 arg2) { return value_compare_dif(arg1.get(), arg2); }
	template<typename T1, typename T2> inline bool operator >= (value<T1> const& arg1, T2 arg2) { return value_compare_same_ma(arg1.get(), arg2); }
	template<typename T1, typename T2> inline bool operator <= (value<T1> const& arg1, T2 arg2) { return value_compare_same_mi(arg1.get(), arg2); }
	template<typename T1, typename T2> inline bool operator > (value<T1> const& arg1, T2 arg2) { return value_compare_ma(arg1.get(), arg2); }
	template<typename T1, typename T2> inline bool operator < (value<T1> const& arg1, T2 arg2) { return value_compare_mi(arg1.get(), arg2); }

	template<typename T1, typename T2> inline bool operator == (T1 arg1, value<T2> const& arg2) { return value_compare_same(arg1, arg2.get()); }
	template<typename T1, typename T2> inline bool operator != (T1 arg1, value<T2> const& arg2) { return value_compare_dif(arg1, arg2.get()); }
	template<typename T1, typename T2> inline bool operator >= (T1 arg1, value<T2> const& arg2) { return value_compare_same_ma(arg1, arg2.get()); }
	template<typename T1, typename T2> inline bool operator <= (T1 arg1, value<T2> const& arg2) { return value_compare_same_mi(arg1, arg2.get()); }
	template<typename T1, typename T2> inline bool operator > (T1 arg1, value<T2> const& arg2) { return value_compare_ma(arg1, arg2.get()); }
	template<typename T1, typename T2> inline bool operator < (T1 arg1, value<T2> const& arg2) { return value_compare_mi(arg1, arg2.get()); }

	template<typename T1, typename T2> inline bool operator == (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_same(arg1.get(), arg2.get()); }
	template<typename T1, typename T2> inline bool operator != (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_dif(arg1.get() , arg2.get()); }
	template<typename T1, typename T2> inline bool operator >= (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_same_ma(arg1.get() , arg2.get()); }
	template<typename T1, typename T2> inline bool operator <= (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_same_mi(arg1.get() , arg2.get()); }
	template<typename T1, typename T2> inline bool operator > (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_ma(arg1.get() , arg2.get()); }
	template<typename T1, typename T2> inline bool operator < (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_mi(arg1.get() , arg2.get()); }

	template<typename T1, typename T2> inline auto operator + (value<T1> const& arg1, T2 arg2)->value<decltype(T1() + T2())> { return value_operation_add(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator - (value<T1> const& arg1, T2 arg2)->value<decltype(T1() - T2())> { return value_operation_sub(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator * (value<T1> const& arg1, T2 arg2)->value<decltype(T1() * T2())> { return value_operation_mul(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator / (value<T1> const& arg1, T2 arg2)->value<decltype(T1() / T2())> { return value_operation_div(arg1 , arg2); }

	template<typename T1, typename T2> inline auto operator + (T1 arg1, value<T2> const& arg2)->value<decltype(T1() + T2())> { return value_operation_add(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator - (T1 arg1, value<T2> const& arg2)->value<decltype(T1() - T2())> { return value_operation_sub(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator * (T1 arg1, value<T2> const& arg2)->value<decltype(T1() * T2())> { return value_operation_mul(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator / (T1 arg1, value<T2> const& arg2)->value<decltype(T1() / T2())> { return value_operation_div(arg1 , arg2); }

	template<typename T1, typename T2> inline auto operator + (value<T1> const& arg1, value<T2> const& arg2)->value<decltype(T1() + T2())> { return value_operation_add(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator - (value<T1> const& arg1, value<T2> const& arg2)->value<decltype(T1() - T2())> { return value_operation_sub(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator * (value<T1> const& arg1, value<T2> const& arg2)->value<decltype(T1() * T2())> { return value_operation_mul(arg1 , arg2); }
	template<typename T1, typename T2> inline auto operator / (value<T1> const& arg1, value<T2> const& arg2)->value<decltype(T1() / T2())> { return value_operation_div(arg1 , arg2); }

}//ang

#endif//__ANG_VALUE_H__
