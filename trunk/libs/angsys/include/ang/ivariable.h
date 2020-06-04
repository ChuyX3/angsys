/*********************************************************************************************************************/
/*   File Name: ang/ivariable.h                                                                                      */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_IVARIABLE_H__
#define __ANG_IVARIABLE_H__
namespace ang
{
	struct nvt LINK ivariable
		: intf<ivariable
		, iid("ang::ivariable")>
	{
		virtual rtti_t const& value_type()const = 0;
		virtual bool set_value(rtti_t const&, unknown_t) = 0;
		virtual bool get_value(rtti_t const&, unknown_t)const = 0;
		virtual variant clone()const = 0;
		virtual string to_string()const = 0;
		virtual string to_string(text::text_format_t)const = 0;
		template<typename T> bool set_value(T const& val);
		template<typename T> bool get_value(T& val)const;
	};

	template<> class LINK intf_wrapper<ivariable>
	{
	public:
		typedef ivariable type;
		typedef ivariable* type_ptr;
		typedef ivariable& type_ref;
		typedef ivariable const* ctype_ptr;
		typedef ivariable const& ctype_ref;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);

		template<typename T, gender_t TYPE> intf_wrapper(object_wrapper<variable<T, TYPE>> ptr);
		template<wsize N> intf_wrapper(const char(&ar)[N]);
		template<wsize N> intf_wrapper(const wchar(&ar)[N]);
		template<wsize N> intf_wrapper(const mchar(&ar)[N]);
		template<wsize N> intf_wrapper(const char16(&ar)[N]);
		template<wsize N> intf_wrapper(const char32(&ar)[N]);
		template<typename T, wsize N> intf_wrapper(T(&ar)[N]);
		//template<typename T> intf_wrapper(array_view<T> const& ar);
		template<typename T, text::encoding E> intf_wrapper(str_view<T, E>);
		template<text::encoding E, template<typename>class A> intf_wrapper(text::fast_string<E, A> const&);
		template<text::encoding E> intf_wrapper(text::basic_string<E> const&);

		intf_wrapper(bool); //bool convertible
		intf_wrapper(int); //int convertible
		intf_wrapper(uint); //uint convertible
		intf_wrapper(long); //long convertible
		intf_wrapper(ulong); //ulong convertible
		intf_wrapper(long64); //long convertible
		intf_wrapper(ulong64); //ulong convertible
		intf_wrapper(float); //float convertible
		intf_wrapper(double); //double convertible

		template<typename T> intf_wrapper(T const& val);
		template<typename T> intf_wrapper(variable<T>* val);

		~intf_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);
		type ** addres_for_init(void);

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as() {
			return interface_cast<typename smart_ptr_type<T>::type>(m_ptr);
		}

		template<typename T> explicit operator T& () {
			return (T&)as<T>();
		}

		template<typename T> explicit operator T const& ()const {
			return (T const&)const_cast<var*>(this)->as<T>();
		}

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);
		template<typename T> intf_wrapper& operator = (T*);
		template<typename T, gender_t TYPE> intf_wrapper& operator = (object_wrapper<variable<T, TYPE>> ptr);
		template<typename T> intf_wrapper& operator = (intf_wrapper<T> const&);

		template<typename T, text::encoding E> intf_wrapper& operator = (str_view<T, E> const&); //string convertible
		template<typename T, wsize N> intf_wrapper& operator = (T(&ar)[N]); //array convertible

		intf_wrapper& operator = (bool); //bool convertible
		intf_wrapper& operator = (int); //int convertible
		intf_wrapper& operator = (uint); //uint convertible
		intf_wrapper& operator = (long); //long convertible
		intf_wrapper& operator = (ulong); //ulong convertible
		intf_wrapper& operator = (long64); //long convertible
		intf_wrapper& operator = (ulong64); //ulong convertible
		intf_wrapper& operator = (float); //float convertible
		intf_wrapper& operator = (double); //double convertible

		intf_wrapper_ptr<type> operator & (void);
		operator ang::intfptr()const;
		type* operator -> (void);
		type const* operator -> (void)const;
		operator type* (void);
		operator type const* (void)const;

	private:
		ivariable* m_ptr;

	};

	template<typename T>
	struct ivariant_setter {
		static bool set(ivariable* var, T const& value) {
			return var->set_value(type_of<T>(), (void*)&value);
		}
	};

	template<typename T>
	struct ivariant_setter<object_wrapper<T>> {
		static bool set(ivariable* var, object_wrapper<T> const& value) {
			return var->set_value(type_of<T>(), (void*)value.get());
		}
	};

	template<typename T>
	struct ivariant_setter<intf_wrapper<T>> {
		static bool set(ivariable* var, intf_wrapper<T> const& value) {
			return var->set_value(type_of<T>(), (void*)value.get());
		}
	};

	template<typename T>
	struct ivariant_setter<array_view<T>> {
		static bool set(ivariable* var, array_view<T> const& ar) {
			return var->set_value(type_of<array_view<T>>(), (void*)(&ar));
		}
	};

	template<typename T, template<typename>class A>
	struct ivariant_setter<array<T,A>> {
		static bool set(ivariable* var, array_view<T> const& ar) {
			return var->set_value(type_of<array_view<T>>(), (void*)(&ar));
		}
	};

	template<typename T, wsize N>
	struct ivariant_setter<stack_array<T,N>> {
		static bool set(ivariable* var, array_view<T> const& ar) {
			return var->set_value(type_of<array_view<T>>(), (void*)(&ar));
		}
	};

	template<typename T, template<typename>class A>
	struct ivariant_setter<vector<T, A>> {
		static bool set(ivariable* var, array_view<T> const& ar) {
			return var->set_value(type_of<array_view<T>>(), (void*)(&ar));
		}
	};

	template<typename T, wsize N>
	struct ivariant_setter<T const(&)[N]> {
		static bool set(ivariable* var, T const(&ar)[N]) {
			array_view<T> value = collections::to_array((T*)ar, N);	
			return var->set_value(type_of<array_view<T>>(), (void*)&value);
		}
	};

	template<typename T, text::encoding E>
	struct ivariant_setter<str_view<T,E>> {
		static bool set(ivariable* var, str_view<T, E> const& cstr) {
			return var->set_value(type_of<str_view<T, E>>(), (void*)(&ar));
		}
	};

	template<text::encoding E, template<typename>class A>
	struct ivariant_setter<text::fast_string<E, A>> {
		static bool set(ivariable* var, cstr_view<typename text::char_type_by_encoding<E>::char_type,E> const& cstr) {
			return var->set_value(type_of<array_view<T>>(), (void*)(&ar));
		}
	};

	template<typename T>
	inline bool ivariable::set_value(T const& value) {
		return ivariant_setter<T>::set(this, value);
	}


	template<typename T>
	struct ivariant_getter {
		static bool get(ivariable const* var, T& value) {
			return var->get_value(type_of<T>(), (void*)&value);
		}
	};

	template<typename T>
	struct ivariant_getter<intf_wrapper<T>> {
		static bool get(ivariable const* var, intf_wrapper<T>& value) {
			if (value.is_empty())return false;
			return var->get_value(type_of<T>(), (void*)value.get());
		}
	};

	template<typename T>
	struct ivariant_getter<object_wrapper<T>> {
		static bool get(ivariable const* var, object_wrapper<T>& value) {
			if (value.is_empty())return false;
			return var->get_value(type_of<T>(), (void*)value.get());
		}
	};

	template<typename T>
	struct ivariant_getter<array_view<T>> {
		static bool get(ivariable const* var, array_view<T>& value) {
			return var->get_value(type_of<array_view<T>>(), (void*)&value);
		}
	};

	template<typename T>
	inline bool ivariable::get_value(T& value)const {
		return ivariant_getter<T>::get(this, value);
	}

}

#endif/*__ANG_VARIABLE_H__*/