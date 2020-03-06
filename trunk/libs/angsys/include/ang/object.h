/*********************************************************************************************************************/
/*   File Name: ang/object.h                                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/


#ifndef __ANGSYS_H__
#elif !defined __OBJECT_H__
#define __OBJECT_H__

namespace ang
{
	/******************************************************************/
	/* class ang::object :                                            */
	/*  -> implements the base class for all library's objects        */
	/******************************************************************/
	class LINK object
		: public intf<object
		, iid("ang::object")
		, iobject>
	{
	public:
		object();

	protected:
		virtual~object();

		object(object &&) = delete;
		object(object const&) = delete;
		object& operator = (object &&) = delete;
		object& operator = (object const&) = delete;

	public:
		virtual string to_string()const;
		virtual string to_string(text::text_format_t)const;

	protected:
		virtual void dispose() override;
		virtual bool auto_release();

	public:
		static rtti_t const& class_info();
		virtual rtti_t const& runtime_info()const override;
		virtual bool query_interface(rtti_t const& id, unknown_ptr_t out)override;

	public:
		virtual dword add_ref() override;
		virtual dword release() override;

	public:
		pointer operator new(wsize)noexcept;
		void operator delete(pointer)noexcept;

		pointer operator new(wsize, pointer)noexcept;
		void operator delete(pointer, pointer)noexcept;

#ifdef _DEBUG
		pointer operator new(wsize, const char*, int);
		pointer operator new(wsize, word, const char*, int);
#endif//_DEBUG

	protected: //for aligned objects
		pointer operator new(wsize, ushort alignment);
		void operator delete(pointer, ushort alignment);

	protected:
		pointer operator new[](wsize)noexcept;
		void operator delete[](pointer)noexcept;

	public:

		template<typename T> typename smart_ptr_type<T>::smart_ptr_t as() {
			return  this ? interface_cast<typename smart_ptr_type<T>::type>(this) : null;
		}
		template<typename T> bool as(T*& out) {
			out = this ? interface_cast<typename smart_ptr_type<T>::type>(this) : null;
			return out != null;
		}

	};

	template<typename T1, typename T2>
	bool operator == (object_wrapper<T1> const& obj1, object_wrapper<T2> const& obj2) {
		if ((object*)obj1.get() == (object*)obj2.get())
			return true;
		return false;
	}

	template<typename T1, typename T2>
	bool operator != (object_wrapper<T1> const& obj1, object_wrapper<T2> const& obj2) {
		return operator == (obj1, obj2);
	}



	template<typename T, typename...Ts> struct implement;
	

	template<typename...Ts> struct __object_inherit_helper : false_type {};
	template<typename T, typename U, typename... Ts> struct __object_inherit_helper<T, U, Ts...> : is_object<U> {};

	template<typename T, typename...Ts> using object_inherit
		= typename type_selector<__object_inherit_helper<T, Ts...>::value, unique_inherit<T, Ts...>, unique_inherit<T, object, Ts...>>::type;

	template<typename T, typename... Ts>
	struct __query_interface;

	template<typename T, typename TP>
	struct __query_interface_pack;

	template<typename T, typename U, typename... Ts>
	struct __query_interface<T, U, Ts...> {
		static bool query_interface(T* ptr, rtti_t const& id, unknown_ptr_t out) {
			rtti_t const& id2 = type_of<U>();
			if (id.type_id() == id2.type_id()) {
				if (out == null)
					return false;
				*out = static_cast<U*>(ptr);
				return true;
			}
			else {
				return __query_interface<T, Ts...>::query_interface(ptr, id, out);
			}
		}
	};

	template<typename T>
	struct __query_interface<T> {
		static bool query_interface(T*, rtti_t const&, unknown_ptr_t) {
			return false;
		}
	};


	template<typename T, typename... Ts>
	struct __query_interface_pack<T, type_pack<Ts...>> {
		static bool query_interface(T* ptr, rtti_t const& id, unknown_ptr_t out) {
			return __query_interface<T, Ts...>::query_interface(ptr, id, out);
		}
	};

	template<typename T, typename...Ts>
	struct implement
		: object_inherit<T, Ts...>
	{
	protected:
		static_assert(are_interface<Ts...>::value);
		using self = T;
		using base = implement<T, Ts...>;

		template<typename...Arsg>implement(Arsg&&... args)
			: object_inherit<T, Ts...>(ang::forward<Arsg>(args)...) {
		}

	public:
		using iid_type = iid_t<0>;
		using base_pack = typename object_inherit<T, Ts...>::base_pack;
		using object_base = typename base_pack::first;
		using intf_pack = typename base_pack::rest;

		static rtti_t const& class_info();
		virtual rtti_t const& runtime_info()const override;
		virtual bool query_interface(rtti_t const& id, unknown_ptr_t out)override;
	};

	template<typename T, typename CP, typename...Ts>
	struct implement<T, ang::iid_type<CP>, Ts...>
		: object_inherit<T, Ts...>
	{
	protected:
		static_assert(are_interface<Ts...>::value);
		using self = T;
		using base = implement<T, ang::iid_type<CP>, Ts...>;
		
		template<typename...Arsg>implement(Arsg&&... args)
			: object_inherit<T, Ts...>(ang::forward<Arsg>(args)...) { 
		}

	public:
		using iid_type = ang::iid_type<CP>;
		using base_pack = typename object_inherit<T, Ts...>::base_pack;
		using object_base = typename base_pack::first_type;
		using intf_pack = typename base_pack::rest_types;

		static rtti_t const& class_info();
		virtual rtti_t const& runtime_info()const override;
		virtual bool query_interface(rtti_t const& id, unknown_ptr_t out)override;
	};
}

#endif//__SAMRT_PTR_H__