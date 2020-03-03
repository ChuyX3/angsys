/*********************************************************************************************************************/
/*   File Name: ang/inline/wrapper_specialization_definition.hpp                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   File description: this file is exposes many native types and wrappers for them as well as useful macros.        */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __SAMRT_PTR_H__
#error ...
#elif !defined  __ANG_WRAPPER_SPECIALIZATION_HPP__
#define __ANG_WRAPPER_SPECIALIZATION_HPP__

#define ANG_OBJECT_WRAPPER_DECLARATION(_LINK, MY_TYPE) ANG_OBJECT_WRAPPER_DECLARATION_(_LINK, MY_TYPE)
#define ANG_OBJECT_WRAPPER_DECLARATION_(_LINK, MY_TYPE) ANG_BEGIN_OBJECT_WRAPPER(_LINK, MY_TYPE) ANG_END_OBJECT_WRAPPER()

#define ANG_BEGIN_OBJECT_WRAPPER(_LINK, MY_TYPE) \
	template<> class _LINK object_wrapper<MY_TYPE> { \
	public: \
		typedef MY_TYPE type; \
		typedef MY_TYPE* type_ptr; \
		typedef MY_TYPE& type_ref; \
		typedef MY_TYPE const* ctype_ptr; \
		typedef MY_TYPE const& ctype_ref; \
	private: \
		type* m_ptr; \
	public: \
		object_wrapper(); \
		object_wrapper(type*); \
		object_wrapper(object_wrapper &&); \
		object_wrapper(object_wrapper const&); \
		object_wrapper(ang::nullptr_t const&); \
		~object_wrapper(); \
	public: \
		void reset(); \
		void reset_unsafe(); \
		bool is_empty()const; \
		type* get(void)const; \
		void set(type*); \
		type ** addres_of(void); \
	public: \
		object_wrapper& operator = (type*); \
		object_wrapper& operator = (object_wrapper &&); \
		object_wrapper& operator = (object_wrapper const&); \
		object_wrapper& operator = (ang::nullptr_t const&); \
		object_wrapper_ptr<type> operator & (void); \
		inline operator ang::objptr()const{ return (object*)get(); } \
		type* operator -> (void); \
		type const* operator -> (void)const; \
		operator type* (void); \
		operator type const* (void)const;

#define ANG_END_OBJECT_WRAPPER() }


#define ANG_INTF_WRAPPER_DECLARATION(_LINK, MY_TYPE) ANG_INTF_WRAPPER_DECLARATION_(_LINK, MY_TYPE);
#define ANG_INTF_WRAPPER_DECLARATION_(_LINK, MY_TYPE) ANG_BEGIN_INTF_WRAPPER(_LINK, MY_TYPE) ANG_END_INTF_WRAPPER()

#define ANG_BEGIN_INTF_WRAPPER(_LINK, MY_TYPE) \
	template<> class _LINK intf_wrapper<MY_TYPE> { \
	public: \
		typedef MY_TYPE type; \
		typedef MY_TYPE* type_ptr; \
		typedef MY_TYPE& type_ref; \
		typedef MY_TYPE const* ctype_ptr; \
		typedef MY_TYPE const& ctype_ref; \
	protected: \
		type* m_ptr; \
	public: \
		intf_wrapper(); \
		intf_wrapper(type*); \
		intf_wrapper(ang::nullptr_t const&); \
		intf_wrapper(intf_wrapper &&); \
		intf_wrapper(intf_wrapper const&); \
		~intf_wrapper(); \
	public: \
		void reset(); \
		void reset_unsafe(); \
		bool is_empty()const; \
		type* get(void)const; \
		void set(type*); \
		type ** addres_of(void); \
		type** addres_for_init(void); \
		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() { \
			return  this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null; \
		} \
		template<typename U> bool as(U*& out) { \
			out = this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null; \
			return out != null; \
		} \
	public: \
		intf_wrapper& operator = (type*); \
		intf_wrapper& operator = (ang::nullptr_t const&); \
		intf_wrapper& operator = (intf_wrapper &&); \
		intf_wrapper& operator = (intf_wrapper const&); \
		intf_wrapper_ptr<type> operator & (void); \
		operator ang::intfptr()const; \
		type* operator -> (void); \
		type const* operator -> (void)const; \
		operator type* (void); \
		operator type const* (void)const;

#define ANG_END_INTF_WRAPPER() }


#define ANG_BEGIN_INTF_WRAPPER_TEMPLATE(MY_TYPE, ...) \
	ANG_EXPAND(TEMPLATE(__VA_ARGS__)) class intf_wrapper<ANG_EXPAND(MY_TYPE<__VA_ARGS__>)> { \
	public: \
		typedef ANG_EXPAND(MY_TYPE<__VA_ARGS__>) type; \
		typedef ANG_EXPAND(MY_TYPE<__VA_ARGS__>)* type_ptr; \
		typedef ANG_EXPAND(MY_TYPE<__VA_ARGS__>)& type_ref; \
		typedef ANG_EXPAND(MY_TYPE<__VA_ARGS__>) const* ctype_ptr; \
		typedef ANG_EXPAND(MY_TYPE<__VA_ARGS__>) const& ctype_ref; \
	protected: \
		type* m_ptr; \
	public: \
		inline intf_wrapper(); \
		inline intf_wrapper(type*); \
		inline intf_wrapper(ang::nullptr_t const&); \
		inline intf_wrapper(intf_wrapper &&); \
		inline intf_wrapper(intf_wrapper const&); \
		inline ~intf_wrapper(); \
	public: \
		inline void reset(); \
		inline void reset_unsafe(); \
		inline bool is_empty()const; \
		inline type* get(void)const; \
		inline void set(type*); \
		inline type ** addres_of(void); \
		inline type** addres_for_init(void); \
		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() { \
			return  this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null; \
		} \
		template<typename U> bool as(U*& out) { \
			out = this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null; \
			return out != null; \
		} \
	public: \
		inline intf_wrapper& operator = (type*); \
		inline intf_wrapper& operator = (ang::nullptr_t const&); \
		inline intf_wrapper& operator = (intf_wrapper &&); \
		inline intf_wrapper& operator = (intf_wrapper const&); \
		inline intf_wrapper_ptr<type> operator & (void); \
		inline operator ang::intfptr()const; \
		inline type* operator -> (void); \
		inline type const* operator -> (void)const; \
		inline operator type* (void) { return get();} \
		inline operator type const* (void)const { return get();}

#define ANG_END_INTF_WRAPPER() }

#endif //__ANG_WRAPPER_SPECIALIZATION_HPP__
