#ifndef __FORMATPARSER_H__
#define __FORMATPARSER_H__

#include <angsys.h>

#define FLAG_INVALID_FORMAT 0

namespace ang
{
	namespace text
	{
		template<encoding E> struct format_parser;

		template<encoding E>
		class parser_interface
			: public object
			, public iparser
		{
		public:
			typedef typename char_type_by_encoding<E>::char_t char_t;
			typedef typename char_type_by_encoding<E>::str_t str_t;
			typedef typename char_type_by_encoding<E>::cstr_t cstr_t;

			parser_interface();

			ANG_DECLARE_INTERFACE();

			virtual encoding_t format()const override;
			virtual text_format_t default_format(text_format_target_t)const override;
			virtual long64 to_signed(unknown_cstr_t str, wsize sz, windex& i, bool increment, int base)const override;
			virtual ulong64 to_unsigned(unknown_cstr_t str, wsize sz, windex& i, bool increment, int base)const override;
			virtual double to_floating(unknown_cstr_t str, wsize sz, windex& i, bool increment, bool ex)const override;
			virtual bool format(unknown_cstr_t format, wsize sz, args_t args, encoding_t e, istring_ptr_t out)const override;
			virtual bool format(unknown_cstr_t format, wsize sz, var_args_t args, encoding_t e, istring_ptr_t out)const override;
			virtual text_format_t parse(unknown_cstr_t format, wsize sz)const override;
			virtual text_format_t parse(unknown_cstr_t format, wsize sz, wsize& beg, int& arg)const override;

		private:
			virtual~parser_interface();
		};


		template<> struct format_parser<encoding::auto_detect>
		{
			static text_format_flags_t& bool_format();
			static text_format_flags_t& char_format();
			static text_format_flags_t& string_format();
			static text_format_flags_t& signed_format();
			static text_format_flags_t& unsigned_format();
			static text_format_flags_t& floating_format();
		};

		template<encoding E>
		struct format_parser
		{
			typedef typename char_type_by_encoding<E>::char_t char_t;
			typedef typename char_type_by_encoding<E>::str_t str_t;
			typedef typename char_type_by_encoding<E>::cstr_t cstr_t;

			static inline bool parse_char(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
				flag.value = 0;
				flag.target = text::text_format::target::char_;

				wsize b = beg, temp, end = format.size();

				while (beg < end)
				{
					temp = beg;
					char32_t _c = text::to_char32<false, false>(format, beg);
					if (_c == U'\t' || _c == U' ')
					{
						continue;
					}
					else if (_c == U'N' || _c == U'n')
					{
						break;
					}
					else if (_c == U'L' || _c == U'l')
					{
						flag.lower_case = true;
						break;
					}
					else if (_c == U'U' || _c == U'u')
					{
						flag.upper_case = true;
						break;
					}
					else
					{
						beg = b;
						return false;
					}
				}
				while (beg < end)
				{
					temp = beg;
					char32_t _c = text::to_char32<false, false>(format, beg);
					if (_c == U'\t' || _c == U' ')
					{
						continue;
					}
					else if (_c == U'}')
					{
						beg = temp;
						return true;
					}
				}
				beg = b;
				return false;
			}
			static inline bool parse_string(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
				flag.value = 0;
				flag.target = text::text_format::target::text_;

				wsize b = beg, temp, end = format.size();

				while (beg < end)
				{
					temp = beg;
					char32_t _c = text::to_char32<false, false>(format, beg);
					if (_c == U'\t' || _c == U' ')
					{
						continue;
					}
					else if (_c == U'N' || _c == U'n')
					{
						break;
					}
					else if (_c == U'L' || _c == U'l')
					{
						flag.case_ = 1;
						break;
					}
					else if (_c == U'U' || _c == U'u')
					{
						flag.case_ = 2;
						break;
					}
					else if (_c == U'C' || _c == U'c')
					{
						flag.case_ = 3;
						break;
					}
					else
					{
						beg = b;
						return false;
					}
				}
				while (beg < end)
				{
					temp = beg;
					char32_t _c = text::to_char32<false, false>(format, beg);
					if (_c == U'\t' || _c == U' ')
					{
						continue;
					}
					else if (_c == U'}')
					{
						beg = temp;
						return true;
					}
				}
				beg = b;
				return false;
			}
			static inline bool parse_signed(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
				flag.value = 0;
				flag.target = text::text_format::target::signed_;

				wsize b = beg, temp, end = format.size();

				bool waitForEnd = false;
				bool notation = false;
				bool fill = false;
				bool thousand = false;
				bool max = false;
				bool sign = false;

				while (beg < end)
				{
					temp = beg;
					char32_t _c = text::to_char32<false,false>(format, beg);

					if (waitForEnd)
					{
						waitForEnd = false;
						do
						{				
							if (_c == U'\t' || _c == U' ') {
								//continue;
							}
							else if (_c == U'}') {
								beg = temp;
								return true;
							}
							else if (_c == U',') {
								break;
							}
							else
							{
								goto PARSE_ERROR;
							}
							temp = beg;
							_c = text::to_char32<false, false>(format, beg);
						} while (beg <= end); 
						continue;
					}
					else if (_c == U'\t' || _c == U' ')
					{
						continue;
					}
					else if (_c == U'}')
					{
						beg = temp;
						return true;
					}
					else if (_c == U'N' || _c == U'n')
					{
						if (max)
							goto PARSE_ERROR;
						max = true;
						flag.max = str_to_unsigned(format, beg, 10);
						waitForEnd = true;
						continue;
					}
					else if (_c == U'T' || _c == U't')
					{
						if (thousand)
							goto PARSE_ERROR;
						thousand = true;
						if ((beg + 1) >= end)
							goto PARSE_ERROR;
						flag.thousand = (char)(byte)text::to_char32<false,false>(format, beg);
						waitForEnd = true;
						continue;
					}
					else if (_c == U'F' || _c == U'f')
					{
						if (fill)
							goto PARSE_ERROR;
						fill = true;
						if ((beg + 1) >= end)
							goto PARSE_ERROR;
						flag.fill = (char)(byte)text::to_char32<false,false>(format, beg);
						waitForEnd = true;
						continue;
					}
					else if (_c == U'X')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 2;
						waitForEnd = true;
						continue;
					}
					else if (_c == U'x')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 1;
						waitForEnd = true;
						continue;
					}
					else if (_c == U'B' || _c == U'b')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 3;
						waitForEnd = true;
						continue;
					}
					else if (_c == U'S' || _c == U's')
					{
						if (sign)
							goto PARSE_ERROR;
						sign = true;
						flag.sign = true;
						waitForEnd = true;
						continue;
					}
					else
						goto PARSE_ERROR;
				}
			PARSE_ERROR:
				beg = b;
				return false;
			}
			static inline bool parse_unsigned(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
				flag.value = 0;
				flag.target = text::text_format::target::unsigned_;

				wsize b = beg, temp, end = format.size();

				bool waitForEnd = false;
				bool notation = false;
				bool fill = false;
				bool thousand = false;
				bool max = false;
				bool sign = false;

				while (beg < end)
				{
					temp = beg;
					char32_t _c = text::to_char32<false,false>(format, beg);

					if (waitForEnd)
					{
						waitForEnd = false;
					
						do {
						
							if (_c == U'\t' || _c == U' ') {
								continue;
							}
							else if (_c == U'}') {
								beg = temp;
								return true;
							}
							else if (_c == U',') {
								break;
							}
							else
							{
								goto PARSE_ERROR;
							}
							_c = text::to_char32<false, false>(format, beg);
						} while (beg < end);
						continue;
					}
					else if (_c == U'\t' || _c == U' ')
					{
						continue;
					}
					else if (_c == U'}')
					{
						beg = temp;
						return true;
					}
					else if (_c == U'N' || _c == U'n')
					{
						if (max)
							goto PARSE_ERROR;
						max = true;
						flag.max = str_to_unsigned(format, beg, 10);
						waitForEnd = true;
						continue;
					}
					else if (_c == U'T' || _c == U't')
					{
						if (thousand)
							goto PARSE_ERROR;
						thousand = true;
						if ((beg + 1) >= end)
							goto PARSE_ERROR;
						flag.thousand = (char)(byte)text::to_char32<false,false>(format, beg);
						waitForEnd = true;
						continue;
					}
					else if (_c == U'F' || _c == U'f')
					{
						if (fill)
							goto PARSE_ERROR;
						fill = true;
						if ((beg + 1) >= end)
							goto PARSE_ERROR;
						flag.fill = (char)(byte)text::to_char32<false,false>(format, beg);
						waitForEnd = true;
						continue;
					}
					else if (_c == U'X')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 2;
						waitForEnd = true;
						continue;
					}
					else if (_c == U'x')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 1;
						waitForEnd = true;
						continue;
					}
					else if (_c == U'B' || _c == U'b')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 3;
						waitForEnd = true;
						continue;
					}
					else
						goto PARSE_ERROR;
				}
			PARSE_ERROR:
				beg = b;
				return false;
			}
			static inline bool parse_floating(str_view<const char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
				flag.value = 0;
				flag.target = text::text_format::target::float_;

				wsize b = beg, temp, end = format.size();

				bool waitForEnd = false;
				bool notation = false;
				bool fill = false;
				bool thousand = false;
				bool max = false;
				bool sign = false;
				bool exp = false;

				while (beg < end)
				{
					temp = beg;
					char32_t _c = text::to_char32<false,false>(format, beg);

					if (waitForEnd)
					{
						waitForEnd = false;
						do {
						
							if (_c == U'\t' || _c == U' ')
							{
								continue;
							}
							else if (_c == U'}')
							{
								beg = temp;
								return true;
							}		
							else if (_c == U',')
							{
								break;
							}
							else
								goto PARSE_ERROR;
							temp = beg;
							_c = text::to_char32<false, false>(format, beg);
						} while (beg < end);
						continue;
					}
					else if (_c == U'\t' || _c == U' ')
					{
						continue;
					}
					else if (_c == U'}')
					{
						beg = temp;
						return true;
					}
					else if (_c == U'N' || _c == U'n')
					{
						if (max)
							goto PARSE_ERROR;
						max = true;
						bool dec = false;
					
						wsize i;
						flag.max = min(str_to_unsigned(format, beg), 0X0F) << 4;
						i = beg;
						if (text::to_char32<false,false>(format, i) != U'.')
							goto PARSE_ERROR;
						beg = i;
						flag.pres = min(str_to_unsigned(format, beg), 0X0F);

						waitForEnd = true;
						continue;
					}
					else if (_c == U'T' || _c == U't')
					{
						if (thousand)
							goto PARSE_ERROR;
						thousand = true;
						if ((beg + 1) >= end)
							goto PARSE_ERROR;
						flag.thousand = (char)(byte)text::to_char32<false,false>(format, beg);
						waitForEnd = true;
						continue;
					}
					else if (_c == 'F' || _c == 'f')
					{
						if (fill)
							goto PARSE_ERROR;
						fill = true;
						if ((beg + 1) >= end)
							goto PARSE_ERROR;
						flag.fill = (char)(byte)text::to_char32<false,false>(format, beg);
						waitForEnd = true;
						continue;
					}
					else if (_c == 'X')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 2;
						waitForEnd = true;
						continue;
					}
					else if (_c == 'x')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 1;
						waitForEnd = true;
						continue;
					}
					else if (_c == 'B' || _c == 'b')
					{
						if (notation)
							goto PARSE_ERROR;
						notation = true;
						flag.base = 3;
						waitForEnd = true;
						continue;
					}
					else if (_c == 'S' || _c == 's')
					{
						if (sign)
							goto PARSE_ERROR;
						sign = true;
						flag.sign = true;
						waitForEnd = true;
						continue;
					}
					else if (_c == 'E' || _c == 'e')
					{
						if (exp)
							goto PARSE_ERROR;
						exp = true;
						flag.exponent = true;
						waitForEnd = true;
						continue;
					}
					else
						goto PARSE_ERROR;
				}

			PARSE_ERROR:
				beg = b;
				return false;
			}
			static qword parse(str_view<const char_t, E> f) {
				wsize beg = 0; int n;
				return parse(f, beg, n);
			}

			static qword parse(str_view<const char_t, E> format, wsize& beg, int& arg) {
				text::text_format_flags_t flag;
				flag.value = 0;
				arg = -1;
				if (!format.cstr())
					return 0;

				wsize b = beg, temp, pos = 0, pos2 = 0, end = format.size();
				char32_t c;
				bool has_arg = false;

				enum {
					state_start,
					state_args,
					state_type,
					state_info,
					state_end,
					state_error
				}state;

				state = state_start;

				while (beg < end)
				{
					switch (state)
					{
					case state_start: {
						//removing spaces	
						c = text::to_char32<false, false>(format, beg);
						if (c == U' ' || c == U'\t')
							continue;
						else if (c != U'{')
							state = state_error;
						else state = state_args;
					} break;
					case state_args:
					{
						do {
							temp = beg;
							c = text::to_char32<false, false>(format, beg);
						} while (c == ' ' || c == '\t');

						beg = temp;
						arg = (int)str_to_signed(format, beg);
						state = state_type;
						if (temp == beg)
							arg = -1;
					} break;
					case state_type:
					{
						temp = beg;
						c = text::to_char32<false, false>(format, beg);
						//removing spaces
						if (c == U' ' || c == U'\t')
						{
							if (arg != -1)
								state = state_end;
						}
						else if (c == U'}')
						{
							beg = temp;
							state = state_end;
						}
						else
						{
							state = state_info;
							if (U'b' == c || U'B' == c)
								flag.value = format_parser<encoding::auto_detect>::bool_format().value;
							else if (U'c' == c || U'C' == c)
								flag.value = format_parser<encoding::auto_detect>::char_format().value;
							else if (U's' == c || U'S' == c)
								flag.value = format_parser<encoding::auto_detect>::string_format().value;
							else if (U'i' == c || U'I' == c)
								flag.value = format_parser<encoding::auto_detect>::signed_format().value;
							else if (U'u' == c || U'U' == c)
								flag.value = format_parser<encoding::auto_detect>::unsigned_format().value;
							else if (U'f' == c || U'F' == c)
								flag.value = format_parser<encoding::auto_detect>::floating_format().value;
							else
								state = state_error;
						}
					} break;
					case state_info:
					{
						temp = beg;
						c = text::to_char32<false, false>(format, beg);
						if (c == U' ' || c == U'\t')
							break;
						else if (c == U':')
						{
							switch (flag.target)
							{
							case text::text_format::bool_:
								//	if (!parse_bool(format, beg, flag))
								//		goto PARSE_ERROR;
								break;
							case text::text_format::char_:
								if (!parse_char(format, beg, flag))
									state = state_error;
								break;
							case text::text_format::text_:
								if (!parse_string(format, beg, flag))
									state = state_error;
								break;
							case text::text_format::signed_:
								if (!parse_signed(format, beg, flag))
									state = state_error;
								break;
							case text::text_format::unsigned_:
								if (!parse_unsigned(format, beg, flag))
									state = state_error;
								break;
							case text::text_format::float_:
								if (!parse_floating(format, beg, flag))
									state = state_error;
								break;
							default: state = state_error;
								break;
							}
							state = state_end;
						}
						else if (c == U'}')
						{
							beg = temp;
							state = state_end;
						}
						else
							state = state_error;
					}break;
					case state_end:
					{
						temp = beg;
						c = text::to_char32<false, false>(format, beg);
						if (c == U' ' || c == U'\t')
							break;
						else if (c == U'}') {
							//beg = temp;
							return flag.value;
						}
						else
							state = state_error;
					} break;
					//case state_error:
					default:
					{
						arg = -1;
						beg = b;
						return 0;
					} break;
					}
				}//while

				arg = -1;
				beg = b;
				return 0;
			}

			template<template<typename>class Allocator>
			static inline bool format(text::basic_string_buffer<E, Allocator>* out, str_view<const char_t, E> format, args_t args) {
				if (out == null) return false;
				out->clear();
				int a;
				text::text_format_flags_t f;
				wsize i = 0, t = 0, n = 0, l = 0;
				while (char32_t c = text::to_char32<false, false>(format, i))
				{
					if (c == U'{') {
						f.value = parse(format, n, a);
						if (a > -1 && a < (long64)args->size()) {
							out->concat(str_view<const char_t, E>(&format[l], t - l));
							out->concat(f.value ? args[a]->to_string(f) : args[a]->to_string());
							i = l = n;
						}
					}
					t = n = i;
				}
				out->concat(str_view<const char_t, E>(&format[l], i - l));
				return false;
			}

			template<template<typename>class Allocator>
			static inline bool format(text::basic_string<E, Allocator>& out, str_view<const char_t, E> format, args_t args) {
				out = "";
				int a;
				text::text_format_flags_t f;
				wsize i = 0, n = 0, l = 0;
				while (char32_t c = text::to_char32<false,false>(format, i))
				{
					if (c == U' ' || c == U'\t')
						continue;
					else if (c == U'{') {
						n = i;
						f.value = parse(format, n, a);
						if (a > -1 && text::to_char32<false, false>(format, n) == U'}') {
							out += str_view<const char_t, E>(&format[l], i - l);
							out += f.value ? args[a]->to_string(f) : args[a]->to_string();
							i = l = n;
						}			
					}
				}
				out += str_view<const char_t, E>(&format[l], i - l);
				return false;
			}


			template<template<typename>class Allocator>
			static inline bool format(text::basic_string_buffer<E, Allocator>* out, str_view<const char_t, E> format, var_args_t args) {
				if (out == null) return false;
				out->clear();
				int a;
				text::text_format_flags_t f;
				wsize i = 0, t = 0, n = 0, l = 0;
				while (char32_t c = text::to_char32<false, false>(format, i))
				{
					if (c == U'{') {
						f.value = parse(format, n, a);
						if (a > -1 && a < (long64)args->size()) {
							out->concat(str_view<const char_t, E>(&format[l], t - l));
							out->concat(f.value ? args[a]->to_string(f) : args[a]->to_string());
							i = l = n;
						}
					}
					t = n = i;
				}
				out->concat(str_view<const char_t, E>(&format[l], i - l));
				return false;
			}

			template<template<typename>class Allocator>
			static inline bool format(text::basic_string<E, Allocator>& out, str_view<const char_t, E> format, var_args_t args) {
				out = "";
				int a;
				text::text_format_flags_t f;
				wsize i = 0, n = 0, l = 0;
				while (char32_t c = text::to_char32<false, false>(format, i))
				{
					if (c == U' ' || c == U'\t')
						continue;
					else if (c == U'{') {
						n = i;
						f.value = parse(format, n, a);
						if (a > -1 && text::to_char32<false, false>(format, n) == U'}') {
							out += str_view<const char_t, E>(&format[l], i - l);
							out += f.value ? args[a]->to_string(f) : args[a]->to_string();
							i = l = n;
						}
					}
				}
				out += str_view<const char_t, E>(&format[l], i - l);
				return false;
			}
		
		};


///////////////////////////////////////////////////////////////////////////////////////////////////////////


		template<encoding E>
		inline parser_interface<E>::parser_interface() {

		}

		template<encoding E>
		inline parser_interface<E>::~parser_interface() {

		}

		template<encoding E>
		inline encoding_t parser_interface<E>::format()const {
			return E;
		}

		template<encoding E>
		inline text_format_t parser_interface<E>::default_format(text_format_target_t target)const {

			switch (target)
			{
			case text_format_target::bool_: return format_parser<encoding::auto_detect>::bool_format().value;
			case text_format_target::char_: return format_parser<encoding::auto_detect>::char_format().value;
			case text_format_target::text_: return format_parser<encoding::auto_detect>::string_format().value;
			case text_format_target::signed_: return format_parser<encoding::auto_detect>::signed_format().value;
			case text_format_target::unsigned_: return format_parser<encoding::auto_detect>::unsigned_format().value;
			case text_format_target::float_: return format_parser<encoding::auto_detect>::floating_format().value;

			}
			return text_format();
		}


		template<encoding E>
		inline long64 parser_interface<E>::to_signed(unknown_cstr_t str, wsize sz, windex& i, bool increment, int base)const {
			windex j = i;
			return str_to_signed<char_t const, E>(str_view<const char_t, E>((cstr_t)str, sz), increment ? i : j, base);
		}

		template<encoding E>
		inline ulong64 parser_interface<E>::to_unsigned(unknown_cstr_t str, wsize sz, windex& i, bool increment, int base)const {
			windex j = i;
			return str_to_unsigned<char_t const, E>(str_view<const char_t, E>((cstr_t)str, sz), increment ? i : j, base);
		}

		template<encoding E>
		inline double parser_interface<E>::to_floating(unknown_cstr_t str, wsize sz, windex& i, bool increment, bool ex)const {
			windex j = i;
			return str_to_floating<char_t const, E>(str_view<const char_t, E>((cstr_t)str, sz), increment ? i : j, ex);
		}

		template<encoding E>
		inline bool parser_interface<E>::format(unknown_cstr_t format, wsize sz, args_t args, encoding_t e, istring_ptr_t out)const {
			if (out.is_empty())
				return false;
			istring_t str = istring_factory::get_factory(e)->create_string();
			*out = str;
			str->clear();
			int a;
			text::text_format_flags_t f;
			wsize i = 0, t = 0, n = 0, l = 0;
			while (char32_t c = text::to_char32<false, is_endian_swapped<E>::value>((cstr_t)format, i))
			{
				if (c == U'{') {
					f.value = parse(format, sz, n, a).format_flags();
					if (a > -1 && a < (long64)args->size()) {
						str->concat(str_view<const char_t, E>(&cstr_t(format)[l], t - l));
						str->concat(f.value ? (raw_cstr)args[a]->to_string(f) : (raw_cstr)args[a]->to_string());
						i = l = n;
					}
				}
				t = n = i;
			}
			str->concat(str_view<const char_t, E>(&cstr_t(format)[l], i - l));
			return false;
		}

		template<encoding E>
		inline bool parser_interface<E>::format(unknown_cstr_t format, wsize sz, var_args_t args, encoding_t e, istring_ptr_t out) const {
			if (out.is_empty())
				return false;
			istring_t str = istring_factory::get_factory(e)->create_string();
			*out = str;
			str->clear();
			int a;
			text::text_format_flags_t f;
			wsize i = 0, t = 0, n = 0, l = 0;
			while (char32_t c = text::to_char32<false, is_endian_swapped<E>::value>((cstr_t)format, i))
			{
				if (c == U'{') {
					f.value = parse(format, sz, n, a).format_flags();
					if (a > -1 && a < (long64)args->size()) {
						str->concat(str_view<const char_t, E>(&cstr_t(format)[l], t - l));
						str->concat(f.value ? (raw_cstr_t)args[a]->to_string(f) : (raw_cstr_t)args[a]->to_string());
						i = l = n;
					}
				}
				t = n = i;
			}
			str->concat(str_view<const char_t, E>(&cstr_t(format)[l], i - l));
			return false;
		}

		template<encoding E>
		inline text_format_t parser_interface<E>::parse(unknown_cstr_t format, wsize sz)const {
			text_format_flags f;
			f.value = format_parser<E>::parse(str_view<const char_t, E>((cstr_t)format, sz));
			return f;
		}

		template<encoding E>
		inline text_format_t parser_interface<E>::parse(unknown_cstr_t format, wsize sz, wsize& beg, int& arg)const {
			text_format_flags f;
			f.value = format_parser<E>::parse(str_view<const char_t, E>((cstr_t)format, sz), beg, arg);
			return f;
		}
	}
}

#endif