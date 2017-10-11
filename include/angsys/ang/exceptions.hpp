/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_EXCEPTIONS_H__
#define __ANG_EXCEPTIONS_H__

namespace ang
{
	ANG_BEGIN_ENUM(LINK, except_code, dword)
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
	ANG_END_ENUM(except_code);

	class LINK exception
		: public object
	{
		except_code_t code;
		string message;

	public:
		exception()throw();
		exception(except_code_t type)throw();
		exception(uint errorCode, string msg)throw();
		exception(string msg)throw();
		exception(const exception&)throw();

		virtual string what()const throw();
		dword result()const throw();

	public: //Overrides
		static type_name_t class_name()throw();
		static bool is_child_of(type_name_t) throw();
		virtual type_name_t object_name()const throw();
		virtual bool is_kind_of(type_name_t)const throw();
		virtual bool query_object(type_name_t, unknown_ptr_t) throw();

	private:
		virtual~exception()throw();
	};

	template<>
	class LINK object_wrapper<exception>
	{
	public:
		typedef exception type;

	private:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(except_code_t type);
		object_wrapper(dword errorCode, string msg);
		object_wrapper(string msg);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);

	private:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(exception*);

	public:
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		exception * operator -> (void);
		exception const* operator -> (void)const;
	};

	inline void throw_exception(except_code_t type) { throw exception_t(type); }
	inline void throw_exception(string msg) { throw exception_t(ang::move(msg)); }
	inline void throw_exception(uint errorCode, string msg) { throw exception_t(errorCode, ang::move(msg)); }

}


#endif
