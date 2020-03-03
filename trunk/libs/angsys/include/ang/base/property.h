/*********************************************************************************************************************/
/*   File Name: coffe/base/property.h                                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) coffe sys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __COFFE_BASE_H__
#error coffe/base/base.h is not included
#elif !defined __COFFE_BASE_PROPERTY_H__
#define __COFFE_BASE_PROPERTY_H__

namespace coffe //constants
{
	template<typename T> struct base_property { };
	template<typename T, smart_type TYPE = smart_ptr_type<typename remove_reference<typename remove_const<T>::type>::type>::smart_type> struct property_helper;

	template<typename T>
	struct property_helper<T, smart_type::none> {
		using type = typename remove_reference<typename remove_const<T>::type>::type;
		using ret_type = type;
		using ptr_type = typename __to_pointer_helper<type>::ptr_type;
		using arg_type = type &&;
		using property_class = base_property<type>;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class*, arg_type);
	};

	template<typename T>
	struct property_helper<const T, smart_type::none> {
		using type = typename remove_reference<typename remove_const<T>::type>::type;
		using ret_type = type;
		using ptr_type = typename __to_pointer_helper<type>::ptr_type;
		using arg_type = type&&;
		using property_class = base_property<type>;
		using getter_type = ret_type(*)(property_class const*);
		using setter_type = void(*)(property_class*, arg_type);
	};

	template<typename T, typename property_helper<T>::getter_type getter, typename property_helper<T>::setter_type setter = null>
	struct property : property_helper<T>::property_class {
		using self = property<T, getter, setter>;

		template<typename U>
		inline typename property_helper<T>::ret_type operator = (U&& value) {
			static_assert(!is_const<T>::value, "can't asign value to a read only property");
			setter(prop(), forward<typename property_helper<T>::type>(value));
			return getter(prop());
		}
		inline operator typename property_helper<T>::ret_type()const {
			return getter(prop());
		}

		inline typename property_helper<T>::ptr_type operator ->()const {
			static auto val = getter(prop());//hack: saving value to static variable
			return to_pointer<decltype(getter(prop()))>(val);
		}
		template<typename U>
		operator U()const {
			return U(getter(prop()));
		}

		template<typename I> 
		auto operator[](I&& i)
		{
			return getter(prop())[i];
		}
		template<typename I>
		auto operator[](I&& i)const
		{
			return getter(prop())[i];
		}

		template<typename U> friend auto operator + (const self& a, const U& b) { return getter(a.prop()) + b; }
		template<typename U> friend auto operator + (const U& a, const self& b) { return a + getter(b.prop()); }
		template<typename U> friend auto operator - (const self& a, const U& b) { return getter(a.prop()) - b; }
		template<typename U> friend auto operator - (const U& a, const self& b) { return a - getter(b.prop()); }
		template<typename U> friend auto operator * (const self& a, const U& b) { return getter(a.prop()) * b; }
		template<typename U> friend auto operator * (const U& a, const self& b) { return a * getter(b.prop()); }
		template<typename U> friend auto operator / (const self& a, const U& b) { return getter(a.prop()) / b; }
		template<typename U> friend auto operator / (const U& a, const self& b) { return a / getter(b.prop()); }
		template<typename U> friend auto operator % (const self& a, const U& b) { return getter(a.prop()) % b; }
		template<typename U> friend auto operator % (const U& a, const self& b) { return a % getter(b.prop()); }
		

		template<typename U> friend auto operator & (const self& a, const U& b) { return getter(a.prop()) & b; }
		template<typename U> friend auto operator & (const U& a, const self& b) { return a & getter(b.prop()); }
		template<typename U> friend auto operator | (const self& a, const U& b) { return getter(a.prop()) | b; }
		template<typename U> friend auto operator | (const U& a, const self& b) { return a | getter(b.prop()); }
		template<typename U> friend auto operator ^ (const self& a, const U& b) { return getter(a.prop()) ^ b; }
		template<typename U> friend auto operator ^ (const U& a, const self& b) { return a ^ getter(b.prop()); }
		
		template<typename U> friend auto operator == (const self& a, const U& b) { return getter(a.prop()) == b; }
		template<typename U> friend auto operator == (const U& a, const self& b) { return a == getter(b.prop()); }
		template<typename U> friend auto operator != (const self& a, const U& b) { return getter(a.prop()) != b; }
		template<typename U> friend auto operator != (const U& a, const self& b) { return a != getter(b.prop()); }
		template<typename U> friend auto operator >= (const self& a, const U& b) { return getter(a.prop()) >= b; }
		template<typename U> friend auto operator >= (const U& a, const self& b) { return a >= getter(b.prop()); }
		template<typename U> friend auto operator <= (const self& a, const U& b) { return getter(a.prop()) <= b; }
		template<typename U> friend auto operator <= (const U& a, const self& b) { return a <= getter(b.prop()); }
		template<typename U> friend auto operator > (const self& a, const U& b) { return getter(a.prop()) > b; }
		template<typename U> friend auto operator > (const U& a, const self& b) { return a > getter(b.prop()); }
		template<typename U> friend auto operator < (const self& a, const U& b) { return getter(a.prop()) < b; }
		template<typename U> friend auto operator < (const U& a, const self& b) { return a < getter(b.prop()); }
		template<typename U> friend auto operator && (const U& a, const self& b) { return a && getter(b.prop()); }
		template<typename U> friend auto operator || (const U& a, const self& b) { return a || getter(b.prop()); }

	private:
		typename property_helper<T>::property_class * prop() { return (typename property_helper<T>::property_class *)this; }
		typename property_helper<T>::property_class const* prop()const { return (typename property_helper<T>::property_class const*)this; }
	};


	template<typename T>
	struct property_helper<object_wrapper<T>, smart_type::none> {
		using type = object_wrapper<T>;
		using ret_type = type;
		using ptr_type = T * ;
		using arg_type = type const&;
		using property_class = base_property<type>;
		using getter_type = ptr_type(*)(property_class const*);
		using setter_type = void(*)(property_class*, arg_type);
	};

	template<typename T>
	struct property_helper<const object_wrapper<T>, smart_type::none> {
		using type = object_wrapper<T>;
		using ret_type = type;
		using ptr_type = T* ;
		using arg_type = type const&;
		using property_class = base_property<type>;
		using getter_type = ptr_type(*)(property_class const*);
		using setter_type = void(*)(property_class*, arg_type);
	};

	template<typename T>
	struct property_helper<intf_wrapper<T>, smart_type::none> {
		using type = intf_wrapper<T>;
		using ret_type = type;
		using ptr_type = T * ;
		using arg_type = type const&;
		using property_class = base_property<type>;
		using getter_type = ptr_type(*)(property_class const*);
		using setter_type = void(*)(property_class*, arg_type);
	};

	template<typename T>
	struct property_helper<const intf_wrapper<T>, smart_type::none> {
		using type = intf_wrapper<T>;
		using ret_type = type;
		using ptr_type = T * ;
		using arg_type = type const&;
		using property_class = base_property<type>;
		using getter_type = ptr_type(*)(property_class const*);
		using setter_type = void(*)(property_class*, arg_type);
	};


	template<typename T>
	struct property_helper<T, smart_type::smart_object>
		: property_helper<object_wrapper<T>> {
	};

	template<typename T>
	struct property_helper<const T, smart_type::smart_object>
		: property_helper<const object_wrapper<T>> {
	};

	template<typename T>
	struct property_helper<T, smart_type::smart_interface>
		: property_helper<intf_wrapper<T>> {
	};

	template<typename T>
	struct property_helper<const T, smart_type::smart_interface>
		: property_helper<const intf_wrapper<T>> {
	};
}

#endif//__COFFE_BASE_PROPERTY_H__
