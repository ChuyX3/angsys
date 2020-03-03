/*********************************************************************************************************************/
/*   File Name: ang/inline/intf_wrapper.hpp                                                                          */
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
#elif !defined __INTF_WRAPPER_HPP__
#define __INTF_WRAPPER_HPP__

namespace ang
{

	/******************************************************************/
	/* template class ang::intf_wrapper :                             */
	/*  -> implements handling of smart pointers for intercafes       */
	/******************************************************************/
	template<class T>
	class intf_wrapper
	{
	public:
		typedef T type;
		typedef T* type_ptr;
		typedef T& type_ref;
		typedef T const* ctype_ptr;
		typedef T const& ctype_ref;

		intf_wrapper()
			: m_ptr(null) { 
			static_assert(is_interface<type>::value && !is_object<type>::value, "ERROR: T is not a iintf type...");
		}

		intf_wrapper(type* ptr)
			: intf_wrapper() { 
			set(ptr);
		}

		intf_wrapper(ang::nullptr_t const&) 
			: intf_wrapper() {
		}

		intf_wrapper(intf_wrapper && ptr)
			: intf_wrapper() {
			T * temp = ptr.m_ptr;
			ptr.m_ptr = null;
			m_ptr = temp;
		}

		intf_wrapper(intf_wrapper const& ptr)
			: intf_wrapper() { 
			set(ptr.get());
		}

		~intf_wrapper() { 
			reset(); 
		}

	public:
		void reset() {
			iobject * _obj = interface_cast<iobject>(m_ptr);
			if (_obj)_obj->release();
			m_ptr = null;
		}

		void reset_unsafe() {
		//	iobject * _obj = interface_cast<iobject>(m_ptr);
		//	if (_obj)_obj->release();
			m_ptr = null;
		}

		bool is_empty()const { 
			return m_ptr == null;
		}

		type* get(void)const {
			return m_ptr;
		}

		void set(type* ptr) {
			if (ptr == m_ptr)
				return;
			iobject * _old = interface_cast<iobject>(m_ptr);
			iobject * _new = interface_cast<iobject>(ptr);
			m_ptr = ptr;
			if (_new)_new->add_ref();
			if (_old)_old->release();
		}

		void move(intf_wrapper& ptr) {
			if (this == &ptr)
				return;
			reset();
			m_ptr = ptr.m_ptr;
			ptr.m_ptr = null;
		}

		type ** addres_of(void) { 
			return &m_ptr;
		}

		type ** addres_for_init(void) {
			reset(); 
			return &m_ptr;
		}

		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() {
			return  this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
		}

		template<typename U> bool as(U*& out) {
			out = this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
			return out != null;
		}


	public: //operator
		intf_wrapper& operator = (type* ptr) { 
			set(ptr); 
			return*this;
		}

		intf_wrapper& operator = (ang::nullptr_t const&) { 
			reset(); 
			return*this;
		}

		intf_wrapper& operator = (intf_wrapper && ptr) {
			move(ptr); 
			return*this;
		}

		intf_wrapper& operator = (intf_wrapper const& ptr) { 
			set(ptr.get());
			return*this;
		}

		intf_wrapper_ptr<type> operator & (void);

		type* operator -> (void) {
			return get();
		}

		type const* operator -> (void) const {
			return get();
		}

		operator type* (void) {
			return get(); 
		}

		operator type const* (void)const {
			return get();
		}

	protected:
		type* m_ptr;

	};

	/******************************************************************/
	/* template class ang::intf_wrapper_ptr :                         */
	/*  -> reprecents a intf_wrapper pointer                          */
	/******************************************************************/
	template<typename T>
	class intf_wrapper_ptr
	{
	public:
		intf_wrapper_ptr(nullptr_t const&)
			: m_ptr(null) {
		}

		intf_wrapper_ptr(intf_wrapper<T>*ptr)
			: m_ptr(ptr) {
		}

		intf_wrapper_ptr(intf_wrapper_ptr && ptr)
			: m_ptr(ptr.m_ptr) { 
			ptr.m_ptr = null;
		}

		intf_wrapper_ptr(intf_wrapper_ptr const& ptr)
			: m_ptr(ptr.m_ptr) {
		}

		~intf_wrapper_ptr() { 
			m_ptr = null;
		}

		bool is_empty()const {
			return m_ptr == null;
		}

		intf_wrapper_ptr& operator = (intf_wrapper<T>*ptr)
		{
			m_ptr = ptr;
			return *this;
		}

		intf_wrapper_ptr& operator = (nullptr_t const&)
		{
			m_ptr = null;
			return *this;
		}

		intf_wrapper_ptr& operator = (intf_wrapper_ptr && ptr)
		{
			if (this != &ptr) {
				m_ptr = ptr.m_ptr;
				ptr.m_ptr = null;
			}
			return *this;
		}

		intf_wrapper_ptr& operator = (intf_wrapper_ptr const& ptr)
		{
			if (this != &ptr) {
				m_ptr = ptr.m_ptr;
			}
			return *this;
		}

		intf_wrapper<T>* operator ->() {
			return m_ptr;
		}

		intf_wrapper<T> const* operator ->()const {
			return m_ptr;
		}

		operator intf_wrapper<T>*() {
			return m_ptr;
		}

		operator intf_wrapper<T> const*()const {
			return m_ptr;
		}

		operator T**() { 
			return m_ptr->addres_of();
		}

		operator T* const*()const {
			return m_ptr->addres_of();
		}

		operator void**() {
			return m_ptr->addres_of();
		}

		operator void*const*()const {
			return m_ptr->addres_of();
		}

		intf_wrapper<T>& operator *() {
			return *m_ptr;
		}

		intf_wrapper<T> const& operator *()const {
			return *m_ptr;
		}

		bool operator == (intf_wrapper_ptr<T> const& ptr) {
			return m_ptr == ptr.m_ptr;
		}

		bool operator != (intf_wrapper_ptr<T> const& ptr) {
			return m_ptr != ptr.m_ptr;
		}

		bool operator == (nullptr_t const&) {
			return is_empty();
		}

		bool operator != (nullptr_t const&) {
			return !is_empty();
		}

	private:
		intf_wrapper<T>* m_ptr;

	};

	/******************************************************************/
	/* template class ang::intf_wrapper<iintf> :                      */
	/*  -> specialization of intf_wrapper<iintf> -> intfptr           */
	/******************************************************************/
	template<> 
	class LINK intf_wrapper<iintf>
	{
	public:
		typedef iintf type;
		typedef iintf* type_ptr;
		typedef iintf& type_ref;
		typedef iintf const* ctype_ptr;
		typedef iintf const& ctype_ref;

	public:
		intf_wrapper();
		intf_wrapper(type_ptr);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);

		template<typename T> intf_wrapper(T* ptr) : intf_wrapper() {
			static_assert(is_inherited_from<T, iintf>::value, "T is not valid interface type");
			set(ptr);
		}
		template<typename T> intf_wrapper(intf_wrapper<T> const& ptr) : intf_wrapper() {
			static_assert(is_inherited_from<T, iintf>::value, "T is not valid interface type");
			set(ptr.get());
		}

		~intf_wrapper();

	public:
		type_ptr get(void)const;
		void set(type_ptr);
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type ** addres_of(void);
		type ** addres_for_init(void);

		template<typename U> typename smart_ptr_type<U>::smart_ptr_t as() {
			return  this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
		}

		template<typename U> bool as(U*& out) {
			out = this ? interface_cast<typename smart_ptr_type<U>::type>(m_ptr) : null;
			return out != null;
		}

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);
		template<typename T> intf_wrapper& operator = (T*);
		template<typename T> intf_wrapper& operator = (intf_wrapper<T> const&);

		intf_wrapper_ptr<type> operator & (void);

		type* operator -> (void);
		type const* operator -> (void)const;
		operator type* (void);
		operator type const* (void)const;

	private:
		iintf* m_ptr;

	};

	/******************************************************************/
	/* class ang::weak_ptr_base :                                     */
	/*  -> implements a basic weak smart pointer                      */
	/******************************************************************/
	class LINK weak_ptr_base
	{
	public:
		typedef iintf type;

	private:
		pointer _info;
		wsize _offset;

	public:
		weak_ptr_base();
		weak_ptr_base(weak_ptr_base&&);
		weak_ptr_base(weak_ptr_base const&);
		weak_ptr_base(iintf*);
		weak_ptr_base(std::nullptr_t const&);
		template< typename T>
		weak_ptr_base(intf_wrapper<T> obj) : weak_ptr_base((iintf*)obj.get()) {}
		template< typename T>
		weak_ptr_base(object_wrapper<T> obj) : weak_ptr_base((iintf*)obj.get()) {}
		~weak_ptr_base();

	private:
		void set(iintf*);
		void reset();
		void reset_unsafe();

	public: //properties
		bool is_valid()const;
		template< typename T>
		typename smart_ptr_type<T>::smart_ptr_t lock();

		weak_ptr_base& operator = (intfptr);
		weak_ptr_base& operator = (iintf*);
		weak_ptr_base& operator = (weak_ptr_base&&);
		weak_ptr_base& operator = (weak_ptr_base const&);
		template< typename T>
		weak_ptr_base& operator = (intf_wrapper<T> obj) { return operator = ((iintf*)obj.get()); }
		template< typename T>
		weak_ptr_base& operator = (object_wrapper<T> obj) { return operator = ((iintf*)obj.get()); }

		weak_ptr_base& operator = (std::nullptr_t const&);
	};

	template<> LINK smart_ptr_type<iintf>::smart_ptr_t weak_ptr_base::lock<iintf>();

	template< typename T> inline typename smart_ptr_type<T>::smart_ptr_t weak_ptr_base::lock() {
		return is_valid() ? interface_cast<typename smart_ptr_type<T>::type>(lock<iintf>().get()) : nullptr;
	}

	template<typename T>
	class weak_ptr : public weak_ptr_base
	{
	public:
		typedef T type;
		typedef T* type_ptr;
		typedef T& type_ref;
		typedef T const* ctype_ptr;
		typedef T const& ctype_ref;

	public:
		weak_ptr() : weak_ptr_base() {}
		weak_ptr(weak_ptr&& other) : weak_ptr_base((weak_ptr_base&&)other) {}
		weak_ptr(weak_ptr const& other) : weak_ptr_base((weak_ptr_base const&)other) {}
		weak_ptr(ang::nullptr_t const&) : weak_ptr_base(null) {}
		weak_ptr(T* obj) : weak_ptr_base(static_interface_cast(obj)) {}
		weak_ptr(typename smart_ptr_type<T>::smart_ptr_t obj) : weak_ptr_base(static_interface_cast(obj.get())) {}
		~weak_ptr() {}

	public: //properties
		typename smart_ptr_type<T>::smart_ptr_t lock()const {
			auto ptr = const_cast<weak_ptr<T>*>(this)->weak_ptr_base::lock<iintf>();
			return reinterpret_cast<T*>(ptr.get());
		}

		weak_ptr& operator = (typename smart_ptr_type<T>::smart_ptr_t obj) { weak_ptr_base::operator=(static_interface_cast(obj.get())); return *this; }
		weak_ptr& operator = (T* obj) { weak_ptr_base::operator=(static_interface_cast(obj));  return *this; }
		weak_ptr& operator = (weak_ptr&& other) { weak_ptr_base::operator=(other); return *this; }
		weak_ptr& operator = (weak_ptr const& other) { weak_ptr_base::operator=(other);  return *this; }
		weak_ptr& operator = (ang::nullptr_t const&) { weak_ptr_base::operator=(null); return *this; }
	};

}


template<typename T>
inline ang::intf_wrapper_ptr<T> ang::intf_wrapper<T>::operator & (void) {
	return this;
}


#endif//__INTF_WRAPPER_HPP__