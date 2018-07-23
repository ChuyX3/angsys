
#ifndef __SAMRT_PTR_H__
#error ...
#elif !defined  __ANG_WRAPPER_SPECIALIZATION_HPP__
#define __ANG_WRAPPER_SPECIALIZATION_HPP__

#define ANG_OBJECT_WRAPPER_DECLARATION(_LINK, MY_TYPE) ANG_OBJECT_WRAPPER_DECLARATION_(_LINK, MY_TYPE); ANG_CONST_OBJECT_WRAPPER_DECLARATION_(_LINK, MY_TYPE)
#define ANG_OBJECT_WRAPPER_DECLARATION_(_LINK, MY_TYPE) ANG_BEGIN_OBJECT_WRAPPER(_LINK, MY_TYPE) ANG_END_OBJECT_WRAPPER()
#define ANG_CONST_OBJECT_WRAPPER_DECLARATION_(_LINK, MY_TYPE) ANG_BEGIN_CONST_OBJECT_WRAPPER(_LINK, MY_TYPE) ANG_END_CONST_OBJECT_WRAPPER()

#define ANG_BEGIN_OBJECT_WRAPPER(_LINK, MY_TYPE) \
	template<> class _LINK object_wrapper<MY_TYPE> { \
	public: \
		typedef MY_TYPE type; \
		typedef MY_TYPE* type_ptr; \
		typedef MY_TYPE& type_ref; \
		typedef MY_TYPE const* ctype_ptr; \
		typedef MY_TYPE const& ctype_ref; \
	private: \
		type* _ptr; \
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
		type* operator -> (void)const; \
		operator type* (void)const;

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
		type* _ptr; \
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

#endif //__ANGSYS_HPP__