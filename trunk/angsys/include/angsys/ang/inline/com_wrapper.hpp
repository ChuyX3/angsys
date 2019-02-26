#ifndef __COM_WRAPPER_HPP__
#define __COM_WRAPPER_HPP__

#include <angsys.h>

namespace ang
{

	template<typename T> struct is_com_object : integer_constant<bool, is_same_type<T, ::IUnknown>::value || is_inherited_from<T, ::IUnknown>::value> {};


	/******************************************************************/
	/* template class ang::com_wrapper :                              */
	/*  -> implements handling of com pointers                        */
	/******************************************************************/
	template<typename T>
	class com_wrapper 
	{
	public:
		typedef T type;
		typedef T* type_ptr;
		typedef T& type_ref;
		typedef T const* ctype_ptr;
		typedef T const& ctype_ref;

	public:
		com_wrapper() 
			: m_ptr(null) {
			static_assert(is_com_object<type>::value, "ERROR: T is not a object type...");
		}

		com_wrapper(type* ptr) 
			: com_wrapper() {
			set(ptr);
		}

		com_wrapper(com_wrapper && ptr) 
			: com_wrapper() {
			T * temp = ptr.m_ptr;
			ptr.m_ptr = null;
			m_ptr = temp;
		}

		com_wrapper(com_wrapper const& ptr)
			: com_wrapper() {
			set(ptr.get()); 
		}

		com_wrapper(ang::nullptr_t const&)
			: com_wrapper() {
		}

		~com_wrapper() {
			reset();
		}

	public: //properties

		void reset() {
			if (m_ptr)m_ptr->Release();
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
			if (m_ptr)m_ptr->AddRef();
			if (temp)temp->Release();
		}

		void move(com_wrapper& ptr) {
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

	public: //operators

		com_wrapper& operator = (type* ptr) { 
			set(ptr); 
			return*this;
		}

		com_wrapper& operator = (ang::nullptr_t const&) { 
			reset(); 
			return*this;
		}
		com_wrapper& operator = (com_wrapper && ptr) {
			move(ptr);
			return*this;
		}

		com_wrapper& operator = (com_wrapper const& ptr) {
			set(ptr.get());
			return*this;
		}

		com_wrapper_ptr<T> operator & (void);

		type* operator -> (void) { 
			return get();
		}

		type const* operator -> (void)const {
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
	/* template class ang::com_wrapper_ptr :                       */
	/*  -> reprecents a com_wrapper pointer                        */
	/******************************************************************/
	template<typename T>
	class com_wrapper_ptr
	{
	public:
		com_wrapper_ptr(ang::nullptr_t const&)
			: m_ptr(null) {
		}

		com_wrapper_ptr(com_wrapper<T>*ptr)
			: m_ptr(ptr) {
		}

		com_wrapper_ptr(com_wrapper_ptr && ptr) 
			: m_ptr(ptr.m_ptr) { 
			ptr.m_ptr = null;
		}

		com_wrapper_ptr(com_wrapper_ptr const& ptr) 
			: m_ptr(ptr.m_ptr) {
		}

		~com_wrapper_ptr() {
			m_ptr = null;
		}

		bool is_empty()const {
			return m_ptr == null;
		}

		com_wrapper<T>& operator *()const {
			return *m_ptr;
		}

		com_wrapper<T>* operator ->()const {
			return m_ptr; 
		}

		operator com_wrapper<T>*()const {
			return m_ptr;
		}

		operator unknown_ptr_t()const {
			return m_ptr->addres_of();
		}

		operator T**()const { 
			return m_ptr->addres_of();
		}

	private:
		com_wrapper<T>* m_ptr;
	};

}


#endif//__OBJECT_WRAPPER_HPP__