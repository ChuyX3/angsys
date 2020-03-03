/*********************************************************************************************************************/
/*   File Name: ang/base/errors.h                                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/


#ifndef __ANG_BASE_H__
#elif !defined __ERROR_H__
#define __ERROR_H__

namespace ang
{
	declare_enum(LINK, error_code, dword)
	{
		success = 0,
		none_error = success,
		unknown,
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
		item_not_found,
		file_not_found,
		custom = ~0U,
	};

	struct LINK error
	{
		error();
		error(error const&);
		error(error_code_t const& type);
		error(dword info, castr_t msg);
		error(castr_t msg);
		~error();

		castr_t message()const;
		error_code_t code()const;
		dword info()const;

		//error& operator = (error&&);
		error& operator = (error const&);
		error& operator = (error_code_t const&);

	protected:
		dword m_info;
		error_code_t m_code;
		fast_astring_t m_message;
	};

	struct LINK exception : error
	{
	public:
		exception()throw();
		exception(error const&)throw();
		exception(error_code_t const& code)throw();
		exception(dword errorCode, castr_t msg)throw();
		exception(castr_t msg)throw();
		exception(const exception&)throw();
		~exception()throw();

		castr_t what()const throw();
		dword result()const throw();
	};

	inline void throw_exception(error const& error) { throw exception(error); }
	inline void throw_exception(error_code_t error) { throw exception(error); }
	inline void throw_exception(castr_t msg) { throw exception(ang::move(msg)); }
	inline void throw_exception(dword errorCode, castr_t msg) { throw exception(errorCode, ang::move(msg)); }

	inline void throw_exception_if(bool cond, error const& error) { if (cond) throw exception(error); }
	inline void throw_exception_if(bool cond, error_code_t error) { if (cond) throw exception(error); }
	inline void throw_exception_if(bool cond, castr_t msg) { if (cond) throw exception(ang::move(msg)); }
	inline void throw_exception_if(bool cond, dword errorCode, castr_t msg) { if (cond) throw exception(errorCode, ang::move(msg)); }

}

ANG_ENUM_DECLARATION(LINK, ang::error_code);


#endif//__EXCEPTION_H__