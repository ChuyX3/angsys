/*********************************************************************************************************************/
/*   File Name: ang/strings/istring.h                                                                                 */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#elif !defined __ANG_ISTRING_H__
#define __ANG_ISTRING_H__

namespace ang
{
	template<typename T, text::encoding E>
	struct smart_ptr_type<str_view<T,E>, false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		using smart_ptr_t = string;
		using type = text::istring;
	};
	namespace text
	{
		using parser = text_parser<encoding::auto_detect>;

		struct nvt LINK istring
			: intf<istring
			, iid("ang::text::istring")>
		{
			static string create_string(cstr_t);
			virtual encoding_t encoding()const = 0;
			virtual wsize length()const = 0;
			virtual pointer data()const = 0;
			virtual rtti_t const& char_type()const = 0;
			virtual str_t str(int = 0) = 0;
			virtual cstr_t cstr(int = 0)const = 0;
			virtual char32_t at(windex i, wsize* = null)const = 0;
			virtual int compare(cstr_t)const = 0;
			virtual int compare(string)const = 0;
			virtual windex compare_until(cstr_t)const = 0;
			virtual windex compare_until(string)const = 0;
			virtual windex find(cstr_t, windex start = 0, windex end = -1)const = 0;
			virtual windex find(string, windex start = 0, windex end = -1)const = 0;
			virtual windex find_reverse(cstr_t, windex start = -1, windex end = 0)const = 0;
			virtual windex find_reverse(string, windex start = -1, windex end = 0)const = 0;
			virtual string sub_string(windex start, windex end)const = 0;
			virtual vector<string> split(cstr_t)const = 0;
			virtual vector<string> split(string)const = 0;
			virtual void clear() = 0;
			virtual wsize copy(cstr_t) = 0;
			virtual wsize copy(string) = 0;
			virtual wsize append(cstr_t) = 0;
			virtual wsize append(string) = 0;
			virtual wsize insert(cstr_t, windex = -1) = 0;
			virtual wsize insert(string, windex = -1) = 0;
			virtual void format(cstr_t, var_args_t) = 0;
			virtual void format(string, var_args_t) = 0;
			virtual void append_format(cstr_t, var_args_t) = 0;
			virtual void append_format(string, var_args_t) = 0;
			//virtual void insert_format(windex, cstr_t, var_args_t)= 0;
		};

		template<> struct LINK text_parser<encoding::auto_detect>
		{	
			static text_format_t default_format(text_format_target_t);
			static cstr_t seek(cstr_t, windex& i, cstr_t);
			static long64 to_signed(cstr_t, windex& i, bool increment = true, int base = 10);
			static ulong64 to_unsigned(cstr_t, windex& i, bool increment = true, int base = 10);
			static double to_floating(cstr_t, windex& i, bool increment = true, bool ex = false);
			static cstr_t format(cstr_t, var_args_t args, encoding_t e, istring_ptr_t out);
			static text_format_t parse(cstr_t);
			static text_format_t parse(cstr_t, wsize& beg, int& arg);
			template<typename T>static T to_value(cstr_t const&, windex& i);
		};

		template<typename T, bool = ang::is_unsigned_value<T>::value, bool = ang::is_floating_value<T>::value>
		struct parse_helper {
			static T parse(cstr_t cstr, windex& i) {
				return (T)parser::to_signed(cstr, i);
			}
		};

		template<typename T>
		struct parse_helper<T, true, false> {
			static T parse(cstr_t cstr, windex& i) {
				return (T)parser::to_unsigned(cstr, i);
			}
		};

		template<typename T>
		struct parse_helper<T, false, true> {
			static T parse(cstr_t cstr, windex& i) {
				return (T)parser::to_floating(cstr, i);
			}
		};

		template<typename T> inline T text_parser<encoding::auto_detect>::to_value(cstr_t const& cstr, windex& i) {
			return parse_helper<T>::parse(cstr, i);
		}
	}


	ANG_BEGIN_INTF_WRAPPER(LINK, text::istring)
		inline static string create_new(cstr_t cstr) { return text::istring::create_string(cstr); }
		intf_wrapper(cstr_t cstr);
		//template<typename T, text::encoding E> inline intf_wrapper(str_view<T, E> const& cstr);
		//template<text::encoding E, template<typename> class A> inline intf_wrapper(text::fast_string<E, A>&& str);
		template<typename T, wsize N> inline intf_wrapper(const T(&cstr)[N])
			: intf_wrapper(str_view<const T>(cstr, N - 1)) {
		}

		operator cstr_t ()const;
		char32_t operator [](windex)const;
		inline bool operator == (text::string cstr)const { return (cstr_t)*this == (cstr_t)cstr; }
		inline bool operator != (text::string cstr)const { return (cstr_t)*this != (cstr_t)cstr; }
		inline bool operator >= (text::string cstr)const { return (cstr_t)*this >= (cstr_t)cstr; }
		inline bool operator <= (text::string cstr)const { return (cstr_t)*this <= (cstr_t)cstr; }
		inline bool operator > (text::string cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
		inline bool operator < (text::string cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
		inline bool operator == (ang::nullptr_t const&)const { return m_ptr == null; }
		inline bool operator != (ang::nullptr_t const&)const { return m_ptr != null; }

		inline friend string& operator << (string&, string const&);
		inline friend string& operator << (string&, cstr_t const&);
		template<typename T> inline friend string& operator << (string&, T const&);
		template<typename T, text::encoding E> inline friend string& operator << (string&, str_view<T, E> const&);
	ANG_END_INTF_WRAPPER();

	namespace text
	{
		template<typename T, text::encoding E>
		struct str_view_compare_helper<str_view<T, E>, string>
		{
			static int compare(const str_view<T, E>& value1, string value2) {
				return str_view_compare_helper<cstr_t, cstr_t>::compare(value1, value2);
			}
		};

		template<typename T, text::encoding E>
		struct str_view_compare_helper<string, str_view<T, E>>
		{
			static int compare(string value1, const str_view<T, E>& value2) {
				return str_view_compare_helper<cstr_t, cstr_t>::compare(value1, value2);
			}
		};
	}
}

#endif//__ANG_ISTRING_H__