/*********************************************************************************************************************/
/*   File Name: ang/inline/object_wrapper.hpp                                                                        */
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
#elif !defined __OBJECT_WRAPPER_HPP__
#define __OBJECT_WRAPPER_HPP__

namespace ang
{
	/******************************************************************/
	/* template class ang::object_wrapper :                           */
	/*  -> implements handling of smart pointers                      */
	/******************************************************************/
	template<typename T>
	class object_wrapper
	{
	public:
		typedef T type;
		typedef T* type_ptr;
		typedef T& type_ref;
		typedef T const* ctype_ptr;
		typedef T const& ctype_ref;

	public:
		object_wrapper()
			: m_ptr(null) {
			static_assert(is_object<type>::value, "ERROR: T is not a object type...");
		}

		object_wrapper(type* ptr)
			: object_wrapper() {
			set(ptr);
		}

		object_wrapper(object_wrapper && ptr)
			: object_wrapper() {
			T * temp = ptr.m_ptr;
			ptr.m_ptr = null;
			m_ptr = temp;
		}

		object_wrapper(object_wrapper const& ptr)
			: object_wrapper() {
			set(ptr.get());
		}

		object_wrapper(ang::nullptr_t const&)
			: object_wrapper() {
		}

		~object_wrapper() {
			reset();
		}

	public: //properties

		void reset() {
			if (m_ptr)m_ptr->release();
			m_ptr = null;
		}

		void reset_unsafe() {
			m_ptr = null;
		}

		bool is_empty()const {
			return m_ptr == null;
		}

		type* get(void)const {
			return m_ptr;
		}

		void set(type* ptr) {
			T * temp = m_ptr;
			if (ptr == m_ptr)
				return;
			m_ptr = ptr;
			if (m_ptr)m_ptr->add_ref();
			if (temp)temp->release();
		}

		void move(object_wrapper& ptr) {
			if (this == &ptr)
				return;
			reset();
			m_ptr = ptr.m_ptr;
			ptr.m_ptr = null;
		}

		type ** addres_of(void) {
			return&m_ptr;
		}

		type ** addres_for_init(void) {
			reset();
			return&m_ptr;
		}

		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() {
			return  this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
		}

		template<typename U> bool as(U*& out) {
			out = this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
			return out != null;
		}

	public: //operators

		object_wrapper& operator = (type* ptr) {
			set(ptr);
			return*this;
		}

		object_wrapper& operator = (ang::nullptr_t const&) {
			reset();
			return*this;
		}
		object_wrapper& operator = (object_wrapper && ptr) {
			move(ptr);
			return*this;
		}

		object_wrapper& operator = (object_wrapper const& ptr) {
			set(ptr.get());
			return*this;
		}

		object_wrapper_ptr<T> operator & (void);

		type* operator -> (void)const {
			return get();
		}

		operator type* (void) {
			return get();
		}

		operator type const* (void)const {
			return get();
		}

	private:
		type* m_ptr;
	};

	/******************************************************************/
	/* template class ang::object_wrapper_ptr :                       */
	/*  -> reprecents a object_wrapper pointer                        */
	/******************************************************************/
	template<typename T>
	class object_wrapper_ptr
	{
	public:
		object_wrapper_ptr(ang::nullptr_t const&)
			: m_ptr(null) {
		}

		object_wrapper_ptr(object_wrapper<T>*ptr)
			: m_ptr(ptr) {
		}

		object_wrapper_ptr(object_wrapper_ptr && ptr)
			: m_ptr(ptr.m_ptr) {
			ptr.m_ptr = null;
		}

		object_wrapper_ptr(object_wrapper_ptr const& ptr)
			: m_ptr(ptr.m_ptr) {
		}

		~object_wrapper_ptr() {
			m_ptr = null;
		}

		bool is_empty()const {
			return m_ptr == null;
		}

		object_wrapper_ptr& operator = (object_wrapper<T>*ptr)
		{
			m_ptr = ptr;
			return *this;
		}

		object_wrapper_ptr& operator = (nullptr_t const&)
		{
			m_ptr = null;
			return *this;
		}

		object_wrapper_ptr& operator = (object_wrapper_ptr && ptr)
		{
			if (this != &ptr) {
				m_ptr = ptr.m_ptr;
				ptr.m_ptr = null;
			}
			return *this;
		}

		object_wrapper_ptr& operator = (object_wrapper_ptr const& ptr)
		{
			if (this != &ptr) {
				m_ptr = ptr.m_ptr;
			}
			return *this;
		}

		object_wrapper<T>& operator *() {
			return *m_ptr;
		}

		object_wrapper<T> const& operator *()const {
			return *m_ptr;
		}

		object_wrapper<T>* operator ->()const {
			return m_ptr;
		}

		operator object_wrapper<T>*() {
			return m_ptr;
		}

		operator object_wrapper<T> const*()const {
			return m_ptr;
		}

		operator void**() {
			return m_ptr->addres_of();
		}

		operator void*const*()const {
			return m_ptr->addres_of();
		}

		operator T**() {
			return m_ptr->addres_of();
		}

		operator T*const*()const {
			return m_ptr->addres_of();
		}

		bool operator == (object_wrapper_ptr<T> const& ptr) {
			return m_ptr == ptr.m_ptr;
		}

		bool operator != (object_wrapper_ptr<T> const& ptr) {
			return m_ptr != ptr.m_ptr;
		}

		bool operator == (nullptr_t const&) {
			return is_empty();
		}

		bool operator != (nullptr_t const&) {
			return !is_empty();
		}
	private:
		object_wrapper<T>* m_ptr;
	};

	/******************************************************************/
	/* template class ang::object_wrapper<object> :                   */
	/*  -> specialization of object_wrapper<object>                   */
	/******************************************************************/
	template<>
	class LINK object_wrapper<object>
	{
	public:
		typedef object type;
		typedef object* type_ptr;
		typedef object& type_ref;
		typedef object const* ctype_ptr;
		typedef object const& ctype_ref;

	public:
		object_wrapper();
		object_wrapper(object*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

		template<wsize N> object_wrapper(const char(&ar)[N]);
		template<wsize N> object_wrapper(const wchar(&ar)[N]);
		template<wsize N> object_wrapper(const mchar(&ar)[N]);
		template<wsize N> object_wrapper(const char16(&ar)[N]);
		template<wsize N> object_wrapper(const char32(&ar)[N]);
		template<typename T, wsize N> object_wrapper(T(&ar)[N]);
		template<typename T, text::encoding E> object_wrapper(str_view<T, E>);

		inline object_wrapper(bool); //int convertible

		inline object_wrapper(int); //int convertible
		inline object_wrapper(uint); //uint convertible

		inline object_wrapper(long); //long convertible
		inline object_wrapper(ulong); //ulong convertible

		inline object_wrapper(long64); //long convertible
		inline object_wrapper(ulong64); //ulong convertible

		inline object_wrapper(float); //float convertible
		inline object_wrapper(double); //double convertible

		template<typename T>//object convertible
		inline object_wrapper(object_wrapper<T>);

		template<typename T>//array convertible
		inline object_wrapper(initializer_list<T>);

		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() {
			return  this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
		}

		template<typename U> bool as(U*& out) {
			out = this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
			return out != null;
		}
	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		object* get(void)const;
		void set(object*);
		template<typename T> inline void move(object_wrapper<T>& ptr) {
			if (this == (object_wrapper<T>)&ptr) return;
			reset();
			m_ptr = ptr.get();
			ptr.reset_unsafe();
		}
		object ** addres_of(void);
		object ** addres_for_init(void);

	public:
		object_wrapper& operator = (object*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<object> operator & (void);
		object* operator -> (void)const;
		operator object* (void);
		operator object const* (void)const;

		template<typename T> explicit operator T& () {
			return (T&)as<T>();
		}

		template<typename T> explicit operator T const& ()const {
			return (T const&)const_cast<objptr*>(this)->as<T>();
		}

	private:
		object* m_ptr;
	};
}

template<typename T>
inline ang::object_wrapper_ptr<T> ang::object_wrapper<T>::operator & (void) {
	return this;
}


#endif//__OBJECT_WRAPPER_HPP__