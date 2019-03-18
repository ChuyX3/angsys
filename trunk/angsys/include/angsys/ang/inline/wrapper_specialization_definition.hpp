
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
	public: \
		inline intf_wrapper& operator = (type*); \
		inline intf_wrapper& operator = (ang::nullptr_t const&); \
		inline intf_wrapper& operator = (intf_wrapper &&); \
		inline intf_wrapper& operator = (intf_wrapper const&); \
		inline inline intf_wrapper_ptr<type> operator & (void); \
		inline operator ang::intfptr()const; \
		inline type* operator -> (void); \
		inline type const* operator -> (void)const; \
		inline operator type* (void) { return get();} \
		inline operator type const* (void)const { return get();}

#define ANG_END_INTF_WRAPPER() }
#endif //__ANGSYS_HPP__