#ifndef __ANGSYS_H__
#elif !defined __EXCEPTION_H__
#define __EXCEPTION_H__

namespace ang
{
	safe_enum(LINK, except_code, dword)
	{
		unknown = 0,
		unsupported,
		invalid_memory,
		invalid_access,
		expired_object,
		array_overflow,
		invalid_param,
		two_singleton,
		already_created,
		bad_allocation,
		operation_canceled,
		custom,
	};

	class LINK exception
		: public smart<exception>
	{
		dword code;
		string message;

	public:
		exception()throw();
		exception(except_code_t type)throw();
		exception(dword errorCode, string msg)throw();
		exception(string msg)throw();
		exception(const exception&)throw();

		virtual string what()const throw();
		dword result()const throw();

	public: //Overrides
		static ang::rtti_t const& class_info() throw();
		virtual ang::rtti_t const& runtime_info()const throw();
		virtual bool query_interface(ang::rtti_t const&, unknown_ptr_t) throw();

	private:
		virtual~exception()throw();
	};

	template<>
	class LINK object_wrapper<exception>
	{
	public:
		typedef exception type;

	private:
		type* m_ptr;

	public:
		object_wrapper();
		object_wrapper(except_code_t type);
		object_wrapper(dword errorCode, string msg);
		object_wrapper(string msg);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();
	private:
		void clean();
		type* get(void)const;
		void set(exception*);

	public:
		bool is_empty()const;

	public:
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		exception * operator -> (void);
		exception const* operator -> (void)const;
	};

	inline void throw_exception(except_code_t type) { throw exception_t(type); }
	inline void throw_exception(string msg) { throw exception_t(ang::move(msg)); }
	inline void throw_exception(dword errorCode, string msg) { throw exception_t(errorCode, ang::move(msg)); }

}

#endif//__EXCEPTION_H__