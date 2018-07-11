/*********************************************************************************************************************/
/*   File Name: angsys.hpp                                                                                             */
/*   Autor: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                    */
/*   File description: this file declare all functions and clases for dynamic type naming and types conversions.     */
/*   Also it defines useful macros for the use of this functions and clases.                                         */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/


#ifndef __ANGSYS_HPP__
#error ...
#elif !defined __ANG_VALUE_HPP__
#define __ANG_VALUE_HPP__

namespace ang
{
	template<typename T> class value_wrapper;
	typedef value_wrapper<bool> boolean;
	typedef value_wrapper<int> integer;
	typedef value_wrapper<uint> uinteger;
	typedef value_wrapper<long64> integer64;
	typedef value_wrapper<ulong64> uinteger64;
	typedef value_wrapper<float> floating;
	typedef value_wrapper<double> floating64;

	typedef object_wrapper<boolean> boolean_t;
	typedef object_wrapper<integer> integer_t;
	typedef object_wrapper<uinteger> uinteger_t;
	typedef object_wrapper<integer64> integer64_t;
	typedef object_wrapper<uinteger64> uinteger64_t;
	typedef object_wrapper<floating> floating_t;
	typedef object_wrapper<floating64> floating64_t;

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
		~value() {}
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
		value<type const*> operator & ()const { return value<type const*>(&get(), 1); }
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

	public: /*constructors*/
		value() : _value(null) {}
		value(type val, uint size) : _value(val) {}
		value(ang::nullptr_t) : _value(null) {}
		value(value<T>* val, uint size) : _value(&val->get()) {}
		value(value const& other) : _value(other._value) {}
		value(value && other) : _value(ang::move(other._value)) {}
		~value() {}

	public: /*getters and setters*/
		type & get() { return _value; }
		type const& get()const { return _value; }
		void set(type val) { _value = ang::move(val); }
		type data()const { return _value; }

	public: /*operators*/
		value& operator = (type val) { set(ang::move(val)); return*this; }
		value& operator = (value const& val) { set(val._value); return*this; }
		value& operator = (value && val) { set(ang::move(val._value)); return*this; }

		operator type& () { return get(); }
		operator type ()const { return get(); }

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

	public: /*constructors*/
		value() : _value(null) {}
		value(type val, uint size) : _value(val) {}
		value(ang::nullptr_t) : _value(null) {}
		value(value<T> const* val, uint size) : _value(&val->get()) {}
		value(value const& other) : _value(other._value) {}
		value(value && other) : _value(ang::move(other._value)) {}
		~value() {}

	public: /*getters and setters*/
		type & get() { return _value; }
		type const& get()const { return _value; }
		void set(type val) { _value = ang::move(val); }
		type data()const { return _value; }

	public: /*operators*/
		value& operator = (type val) { set(ang::move(val), 1); return*this; }
		value& operator = (value const& val) { set(val._value); return*this; }
		value& operator = (value && val) { set(ang::move(val._value)); return*this; }

		operator type& () { return get(); }
		operator type ()const { return get(); }

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

	template<typename T1, typename T2> inline typename operation_result<T1, T2>::type value_operation_add(T1 const& arg1, T2 const& arg2) { return arg1 + arg2; }
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
	template<typename T1, typename T2> inline bool operator != (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_dif(arg1.get(), arg2.get()); }
	template<typename T1, typename T2> inline bool operator >= (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_same_ma(arg1.get(), arg2.get()); }
	template<typename T1, typename T2> inline bool operator <= (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_same_mi(arg1.get(), arg2.get()); }
	template<typename T1, typename T2> inline bool operator > (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_ma(arg1.get(), arg2.get()); }
	template<typename T1, typename T2> inline bool operator < (value<T1> const& arg1, value<T2> const& arg2) { return value_compare_mi(arg1.get(), arg2.get()); }

	template<typename T1, typename T2> inline auto operator + (value<T1> const& arg1, T2 arg2)->value<decltype(T1() + T2())> { return value_operation_add(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator - (value<T1> const& arg1, T2 arg2)->value<decltype(T1() - T2())> { return value_operation_sub(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator * (value<T1> const& arg1, T2 arg2)->value<decltype(T1() * T2())> { return value_operation_mul(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator / (value<T1> const& arg1, T2 arg2)->value<decltype(T1() / T2())> { return value_operation_div(arg1, arg2); }

	template<typename T1, typename T2> inline auto operator + (T1 arg1, value<T2> const& arg2)->value<decltype(T1() + T2())> { return value_operation_add(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator - (T1 arg1, value<T2> const& arg2)->value<decltype(T1() - T2())> { return value_operation_sub(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator * (T1 arg1, value<T2> const& arg2)->value<decltype(T1() * T2())> { return value_operation_mul(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator / (T1 arg1, value<T2> const& arg2)->value<decltype(T1() / T2())> { return value_operation_div(arg1, arg2); }

	template<typename T1, typename T2> inline auto operator + (value<T1> const& arg1, value<T2> const& arg2)->value<decltype(T1() + T2())> { return value_operation_add(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator - (value<T1> const& arg1, value<T2> const& arg2)->value<decltype(T1() - T2())> { return value_operation_sub(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator * (value<T1> const& arg1, value<T2> const& arg2)->value<decltype(T1() * T2())> { return value_operation_mul(arg1, arg2); }
	template<typename T1, typename T2> inline auto operator / (value<T1> const& arg1, value<T2> const& arg2)->value<decltype(T1() / T2())> { return value_operation_div(arg1, arg2); }


	template<typename T>
	class value_wrapper
		: public object
		, public value<T>
	{
	public:
		value_wrapper();
		value_wrapper(T const& val);
		value_wrapper(value<T> const& other);
		value_wrapper(value_wrapper const& other);

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;

	private:
		virtual~value_wrapper();
	};

	template<typename T>
	class object_wrapper<value_wrapper<T>>
	{
	public:
		typedef value_wrapper<T> type;

	private:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(type*);
		object_wrapper(value<T> const&);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (value<T> const&);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<type> operator & (void);
		T * operator -> (void);
		T const* operator -> (void)const;
		explicit operator T& (void);
		explicit operator T const& (void)const;
		inline operator objptr (void)const { return (object*)get(); }
		operator T* (void);
		operator T const* (void)const;
		operator type * (void);
		operator type const* (void)const;

		friend safe_pointer;
	};


	template<typename T>
	void swap(object_wrapper<T>& a, object_wrapper<T>&b) {
		swap<wsize>(*reinterpret_cast<wsize*>(a.addres_of()), *reinterpret_cast<wsize*>(b.addres_of()));
	}

	template<typename T>
	void swap(intf_wrapper<T>& a, intf_wrapper<T>&b) {
		swap<wsize>(*reinterpret_cast<wsize*>(a.addres_of()), *reinterpret_cast<wsize*>(b.addres_of()));
	}

	template<> class LINK value_wrapper<bool> final
		: public object
		, public value<bool>
	{
	public:
		static value<bool> parse(cstr_t);
		static value<bool> parse(cwstr_t);
		static string to_string(value<bool>);
		static wstring to_wstring(value<bool>);

	public:
		value_wrapper();
		value_wrapper(bool val);
		value_wrapper(boolean const& other);
		value_wrapper(value<bool> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<int> final
		: public object
		, public value<int>
	{
	public:
		static value<int> parse(cstr_t, int base = 10);
		static value<int> parse(cwstr_t, int base = 10);
		static string to_string(value<int>);
		static wstring to_wstring(value<int>);
		static string to_string(value<int>, text::text_format_t format);
		static wstring to_wstring(value<int>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(int val);
		value_wrapper(integer const& other);
		value_wrapper(value<int> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<uint> final
		: public object
		, public value<uint>
	{
	public:
		static value<uint> parse(cstr_t, int base = 10);
		static value<uint> parse(cwstr_t, int base = 10);
		static string to_string(value<uint>);
		static wstring to_wstring(value<uint>);
		static string to_string(value<uint>, text::text_format_t format);
		static wstring to_wstring(value<uint>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(uint val);
		value_wrapper(uinteger const& other);
		value_wrapper(value<uint> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<long64> final
		: public object
		, public value<long64>
	{
	public:
		static value<long64> parse(cstr_t, int base = 10);
		static value<long64> parse(cwstr_t, int base = 10);
		static string to_string(value<long64>);
		static wstring to_wstring(value<long64>);
		static string to_string(value<long64>, text::text_format_t format);
		static wstring to_wstring(value<long64>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(long64 val);
		value_wrapper(integer64 const& other);
		value_wrapper(value<long64> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<ulong64> final
		: public object
		, public value<ulong64>
	{
	public:
		static value<ulong64> parse(cstr_t, int base = 10);
		static value<ulong64> parse(cwstr_t, int base = 10);
		static string to_string(value<ulong64>);
		static wstring to_wstring(value<ulong64>);
		static string to_string(value<ulong64>, text::text_format_t format);
		static wstring to_wstring(value<ulong64>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(ulong64 val);
		value_wrapper(uinteger64 const& other);
		value_wrapper(value<ulong64> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<float> final
		: public object
		, public value<float>
	{
	public:
		static value<float> parse(cstr_t);
		static value<float> parse(cwstr_t);
		static string to_string(value<float>);
		static wstring to_wstring(value<float>);
		static string to_string(value<float>, text::text_format_t format);
		static wstring to_wstring(value<float>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(float val);
		value_wrapper(floating const& other);
		value_wrapper(value<float> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};

	template<> class LINK value_wrapper<double> final
		: public object
		, public value<double>
	{
	public:
		static value<double> parse(cstr_t);
		static value<double> parse(cwstr_t);
		static string to_string(value<double>);
		static wstring to_wstring(value<double>);
		static string to_string(value<double>, text::text_format_t format);
		static wstring to_wstring(value<double>, text::text_format_t format);

	public:
		value_wrapper();
		value_wrapper(double val);
		value_wrapper(floating64 const& other);
		value_wrapper(value<double> const& other);
		template<typename T>
		value_wrapper(value<T> const& other) : value(other.get()) {}

	public:
		ANG_DECLARE_INTERFACE();

		string to_string()const override;
		string to_string(text::text_format_t)const;

	private:
		virtual~value_wrapper();
	};



	inline integer_t operator "" _i(ulong64 value) { return new integer((int)value); }
	inline uinteger_t operator "" _ui(ulong64 value) { return new uinteger((uint)value); }
	inline integer64_t operator "" _l(ulong64 value) { return new integer64((long64)value); }
	inline integer64_t operator "" _li(ulong64 value) { return new integer64((long64)value); }
	inline uinteger64_t operator "" _ul(ulong64 value) { return new uinteger64((ulong64)value); }
	inline uinteger64_t operator "" _uli(ulong64 value) { return new uinteger64((ulong64)value); }

	inline floating_t operator "" _f(ulong64 value) { return new floating((float)(long64)value); }
	inline floating_t operator "" _f(long double value) { return new floating((float)value); }

	inline floating64_t operator "" _lf(ulong64 value) { return new floating64((double)(long64)value); }
	inline floating64_t operator "" _lf(long double value) { return new floating64((double)value); }

	namespace runtime
	{
		template<class T>
		struct runtime_type_builder<value<T>>
		{
			typedef T type;
			static inline type_name_t type_of() {
				return runtime_type_builder<type>::type_of();
			}

			static inline bool is_type_of(type_name_t name) {
				return runtime_type_builder<type>::is_type_of(name);
			}

			template<typename new_t>
			static inline auto interface_cast(void* _old) {
				return runtime_type_builder<type>::template interface_cast<new_t>(_old);
			}
		};
	}
}


#endif//__ANG_BUFFER_HPP__