/*********************************************************************************************************************/
/*   File Name: ang/base/value.h                                                                                   */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                              */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_FORMATER_H__
#define __ANG_BASE_FORMATER_H__

namespace ang //constants
{
	template<typename...Ts> struct anyone;
	template<typename...Ts> using pack_args_t = vector<anyone<Ts...>>;
	
	template<typename...Ts>
	auto pack_args(Ts&&... args) {
		using types = typename unique_type_pack<Ts...>::type;
		pack_args_t<typename remove_reference<typename remove_const<Ts>::type>::type ...> pack
			= ang::initializer_list<anyone<typename remove_reference<typename remove_const<Ts>::type>::type ...>>{
			ang::forward<Ts>(args)...
		};
		return ang::move(pack);
	}

	namespace text
	{
		////text_format class////
		// {b:[u|U]|[l|L]|[c|C],[t|T]x}
		//	u = uppercase (TRUE/FALSE)
		//	l = lowercase (true/false)
		//	c = camelcase (True/False)
		//  t = (type) 0 == [true/false](default), 1 == [yes/no], 2 == [hight/low]
		// {c:[u|U]|[l|L][N|n]}
		//	u = uppercase
		//	l = lowercase
		//  n = default, optional
		// {s:[u|U]|[l|L]|[c|C]}
		//	u = uppercase (ABCD)
		//	l = lowercase (abcd)
		//	c = camelcase (Abcd)
		//  n = default, optional
		// {i:[n|N]xx,[t|T]c,(F|f)c,[[x|X]|[b|B]],[S|s]}
		//	N = Max Number of digits (xx = number)
		//	T = Thousand separator character (c = character)
		//	F = Fill character (c)
		//	X = Uppercase Hexadecimal format
		//	x = Lowercase Hexadecimal format
		//	B = Binary
		//	S = +|-
		// {u:[n|N]xx,[t|T]c,(F|f)c,[[x|X]|[b|B]]}
		//	N = Max Number of digits (xx = number)
		//	T = Thousand separator character (c = character)
		//	X = Hexadecimal format
		//	F = Fill character (c)
		//	B = Binary
		// {f:[n|N]xx,[t|T]c,(F|f)c,[[x|X]|[b|B]],[E],[S|s]}
		//	N = Max Number of decimals (xx = number)
		//	T = Thousand separator character (c = character)
		//	X = Hexadecimal format
		//	F = Fill character (c)
		//	B = Binary
		//	S = +|-
		//	E = Cientific Notation
		class LINK text_format
		{
		public:
			enum target : byte
			{
				none = 0,
				bool_,
				char_,
				text_,
				signed_,
				unsigned_,
				float_,
			};

			struct LINK text_format_flags {
				union {
					qword value;
					struct {
						ang::text::text_format::target target;
						union {
							byte flags;
							struct {
								bool lower_case : 1;
								bool upper_case : 1;
								byte type : 2; //(boolean) : 0 - [true/false], 1 - [yes/no], 2 - [hight/low]
							};
							struct {
								byte case_ : 2; // 1 - lower case, 2 - upper case, 3 - camel case
								byte base : 2; //(number) : 1-x, 2-X, 3-b
								bool sign : 1;
								bool exponent : 1;
							};
						};
						char fill;
						union {
							byte max_pres;
							struct {
								byte max : 4;
								byte pres : 4;
							};
						};
						char thousand;
					};
				};
				text_format_flags();
				text_format_flags(qword val);
				text_format_flags(ang::text::text_format::target t);
			};

		public:
			text_format();//default format-> bad format
			text_format(castr_t format);
			text_format(qword format);
			text_format(const text_format&);
			text_format(const text_format_flags&);
			virtual~text_format();

		public:
			target format_target()const;
			void format(castr_t format);
			//string format()const;
			qword format_flags()const;
			text_format& operator = (const text_format&);

		protected:
			text_format_flags flags;

		};

		typedef text_format text_format_t;
		typedef text_format::target text_format_target, text_format_target_t;
		typedef text_format::text_format_flags text_format_flags, text_format_flags_t;

		template<typename T>
		struct default_text_format {
			static text_format format() {
				return text_format();
			}
		};

		template<typename T>
		struct default_text_format<const T> : default_text_format<T> { };

		template<typename T>
		struct default_text_format<T&> : default_text_format<T> { };

		template<typename T>
		struct default_text_format<const T&> : default_text_format<T> { };

		template<> struct default_text_format<bool> {
			static text_format format() { static text_format _format = castr_t("{b:}"); return _format; }
		};

		template<> struct default_text_format<char> {
			static text_format format() { static text_format _format = castr_t("{c:}"); return _format; }
		};

		template<> struct default_text_format<mchar> : public default_text_format<char> {};
		template<> struct default_text_format<wchar> : public default_text_format<char> {};
		template<> struct default_text_format<char16_t> : public default_text_format<char> {};
		template<> struct default_text_format<char32_t> : public default_text_format<char> {};

		template<> struct default_text_format<int> {
			static text_format format() { static text_format _format = castr_t("{s:}"); return _format; }
		};

		template<> struct default_text_format<short> : public default_text_format<int> {};
		template<> struct default_text_format<long> : public default_text_format<int> {};
		template<> struct default_text_format<long64> : public default_text_format<int> {};

		template<> struct default_text_format<uint> {
			static text_format format() { static text_format _format = castr_t("{u:}"); return _format; }
		};

		template<> struct default_text_format<ushort> : public default_text_format<uint> {};
		template<> struct default_text_format<ulong> : public default_text_format<uint> {};
		template<> struct default_text_format<ulong64> : public default_text_format<uint> {};

		template<> struct default_text_format<float> {
			static text_format format() { static text_format _format = castr_t("{f:N.4}"); return _format; }
		};

		template<> struct default_text_format<double> :public default_text_format<float> {};

		template<encoding E>
		struct text_parser
		{
			typedef typename char_type_by_encoding<E>::char_type char_t;
			typedef typename char_type_by_encoding<E>::str_type str_t;
			typedef typename char_type_by_encoding<E>::cstr_type cstr_t;

			static constexpr qword default_bool_format = 0x0000000000000301;
			static constexpr qword default_char_format = 0x0000000000000002;
			static constexpr qword default_string_format = 0x0000000000000003;
			static constexpr qword default_signed_format = 0x0000000000000004;
			static constexpr qword default_unsigned_format = 0x0000000000000005;
			static constexpr qword default_floating_format = 0x0000000040000006;

			static constexpr char hexl[18] = { "0123456789abcdefx" };
			static constexpr char hexu[18] = { "0123456789ABCDEFX" };

		private:
			static inline bool parse_bool(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag);
			static inline bool parse_char(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag);
			static inline bool parse_string(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag);
			static inline bool parse_signed(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag);
			static inline bool parse_unsigned(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag);
			static inline bool parse_floating(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag);
			
		public:
			static qword parse(str_view<const char_t, E> f);
			static qword parse(str_view<const char_t, E> format, wsize& beg, int& arg);

			template<typename T>
			static T str_to_value(str_view<const char_t, E> format, wsize& beg);
			static bool str_to_bool(str_view<const char_t, E> str, windex& i);
			static long64 str_to_signed(str_view<const char_t, E> str, windex& i, int base = 10);
			static ulong64 str_to_unsigned(str_view<const char_t, E> str, windex& i, int base = 10);
			static double str_to_floating(str_view<const char_t, E> str, windex& i, bool ex = false);

			template<typename T, template<typename>class A>
			static text::fast_string<E, A>& value_to_string(text::fast_string<E, A>& stream, T const& value, text_format_t const& f = default_text_format<T>::format());

			template<template<typename>class A>
			static text::fast_string<E, A>& bool_to_string(text::fast_string<E, A>& stream, bool value, text_format_t const& f = default_text_format<bool>::format());
			template<template<typename>class A>
			static text::fast_string<E, A>& signed_to_string(text::fast_string<E, A>& stream, long64 value, text_format_t const& f = default_text_format<long64>::format());
			template<template<typename>class A>
			static text::fast_string<E, A>& unsigned_to_string(text::fast_string<E, A>& stream, ulong64 value, text_format_t const& f = default_text_format<ulong64>::format());
			template<template<typename>class A>
			static text::fast_string<E, A>& floating_to_string(text::fast_string<E, A>& stream, double  value, text_format_t const& f = default_text_format<double>::format());

			template<typename T2, encoding E2>
			static str_view<const char_t, E> seek(str_view<const char_t, E>const& source, str_view<T2, E2> const& value);

			template<template<typename>class A, typename T2, encoding E2, typename...Ts>
			static inline bool format(text::fast_string<E, A>& stream, str_view<T2, E2> f, Ts&&...args);

			template<template<typename>class A, typename T2, encoding E2, typename...Ts>
			static inline bool format(text::fast_string<E, A>& stream, str_view<T2, E2> format, vector<anyone<Ts...>>&& args);
		};


		template<typename T> auto to_string(const T&);
		template<typename T> auto to_string(const T& val, text_format_t const& f);
		
		template<typename T, encoding E>
		bool to_bool(str_view<const T, E> const& cstr, windex& beg) {
			return text_parser<E>::str_to_bool(cstr, beg);
		}

		template<typename T, encoding E>
		long64 to_signed(str_view<const T, E> const& cstr, windex& beg, int base = 10) {
			return text_parser<E>::str_to_signed(cstr, beg, base);
		}

		template<typename T, encoding E>
		long64 to_unsigned(str_view<const T, E> const& cstr, windex& beg, int base = 10) {
			return text_parser<E>::str_to_unsigned(cstr, beg, base);
		}

		template<typename T, encoding E>
		long64 to_floating(str_view<const T, E> const& cstr, windex& beg, bool exp = false) {
			return text_parser<E>::str_to_floating(cstr, beg, exp);
		}

	}

	using text::to_string;
	using text::to_signed;
	using text::to_unsigned;
	using text::to_floating;

}


#endif//__ANG_BASE_FORMATER_H__
