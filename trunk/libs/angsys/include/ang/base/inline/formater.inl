/*********************************************************************************************************************/
/*   File Name: ang/base/inline/formater.inl                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_TEXT_H__
#error ...
#elif !defined __ANG_BASE_FORMATER_INL__
#define __ANG_BASE_FORMATER_INL__

namespace ang
{
	namespace text
	{		
		template<encoding E>
		inline bool text_parser<E>::parse_bool(str_view<const typename text_parser<E>::char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
			flag.value = 0;
			flag.target = text::text_format::target::bool_;

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
				else if (_c == U'T' || _c == U't')
				{
					flag.type = _c >= '0' && _c <= '2' ? byte(_c - '0') : 0;
					continue;
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
		
		template<encoding E>
		inline bool text_parser<E>::parse_char(str_view<const typename text_parser<E>::char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
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
		
		template<encoding E>
		inline bool text_parser<E>::parse_string(str_view<const typename text_parser<E>::char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
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
		
		template<encoding E>
		inline bool text_parser<E>::parse_signed(str_view<const typename text_parser<E>::char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
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
				char32_t _c = text::to_char32<false, false>(format, beg);

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
					flag.thousand = (char)(byte)text::to_char32<false, false>(format, beg);
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
					flag.fill = (char)(byte)text::to_char32<false, false>(format, beg);
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
		
		template<encoding E>
		inline bool text_parser<E>::parse_unsigned(str_view<const typename text_parser<E>::char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
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
				char32_t _c = text::to_char32<false, false>(format, beg);

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
					flag.thousand = (char)(byte)text::to_char32<false, false>(format, beg);
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
					flag.fill = (char)(byte)text::to_char32<false, false>(format, beg);
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
		
		template<encoding E>
		inline bool text_parser<E>::parse_floating(str_view<const typename text_parser<E>::char_t, E> format, wsize& beg, text::text_format_flags_t& flag) {
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
				char32_t _c = text::to_char32<false, false>(format, beg);

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
					if (text::to_char32<false, false>(format, i) != U'.')
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
					flag.thousand = (char)(byte)text::to_char32<false, false>(format, beg);
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
					flag.fill = (char)(byte)text::to_char32<false, false>(format, beg);
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

		template<encoding E>
		inline qword text_parser<E>::parse(str_view<const typename text_parser<E>::char_t, E> f)
		{
			wsize beg = 0; int n;
			return parse(f, beg, n);
		}

		template<encoding E>
		inline qword text_parser<E>::parse(str_view<const typename text_parser<E>::char_t, E> format, wsize& beg, int& arg) 
		{
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
							flag.value = default_bool_format;
						else if (U'c' == c || U'C' == c)
							flag.value = default_char_format;
						else if (U's' == c || U'S' == c)
							flag.value = default_string_format;
						else if (U'i' == c || U'I' == c)
							flag.value = default_signed_format;
						else if (U'u' == c || U'U' == c)
							flag.value = default_unsigned_format;
						else if (U'f' == c || U'F' == c)
							flag.value = default_floating_format;
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
							if (!parse_bool(format, beg, flag))
								state = state_error;
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
		
		template<encoding E> template<typename T>
		inline T text_parser<E>::str_to_value(str_view<const char_t, E> format, wsize& beg)
		{
			if constexpr (is_floating_value<T>::value)
			{
				return (T)str_to_floating(format, beg);
			}
			else if constexpr (is_unsigned_value<T>::value)
			{
				return (T)str_to_unsigned(format, beg);
			}
			else if constexpr (is_integer_value<T>::value)
			{
				return (T)str_to_signed(format, beg);
			}
			else if constexpr (is_same_type<typename remove_reference<typename remove_const<T>::type>::type, bool >::value)
			{
				return str_to_bool(format, beg);
			}
			else
			{
				static_assert(is_integer_value<T>::value , "is not a basic value type");
				return T();
			}
		}

		template<encoding E>
		inline bool text_parser<E>::str_to_bool(str_view<const typename text_parser<E>::char_t, E> str, windex& i) {
			static const collections::pair<ang::cstr_t, bool> s_array[] = {
				{"FALSE"_sv, false}, {"False"_sv, false},
				{"HIGH"_sv, true}, {"High"_sv, true},
				{"LOW"_sv, false}, {"Low"_sv, false},
				{"NO"_sv, false}, {"No"_sv, false},
				{"TRUE"_sv, true}, {"True"_sv, true},
				{"YES"_sv, true}, {"Yes"_sv, true},
				{"false"_sv, false}, {"high"_sv, true},
				{"no"_sv, false}, {"low"_sv, false},
				{"true"_sv, true}, {"yes"_sv, true},
			};
			wsize sz = algorithms::binary_search(str, to_array(s_array));
			if (sz == invalid_handle)
				return s_array[sz];
			else
				return false;
		}

		template<encoding E>
		inline long64 text_parser<E>::str_to_signed(str_view<const typename text_parser<E>::char_t, E> str, windex& i, int base) {
			static constexpr bool SWAP = text::is_endian_swapped<E>::value;

			long64 res = 0; int sig = 1; char32_t space;

			switch (base)
			{
			case 2:
				for (space = text::swap_endian<SWAP>(str[i]);
					space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
					space = text::swap_endian<SWAP>(str[++i]));
				if (text::swap_endian<SWAP>(str[i]) == U'-') {
					sig = -1; i++;
				}
				else if (text::swap_endian<SWAP>(str[i]) == U'+') {
					i++;
				}
				while (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'1') {
					res = (res << 1) | (text::swap_endian<SWAP>(str[i++]) - U'0');
				}
				if (text::swap_endian<SWAP>(str[i]) == U'b')i++;
				break;
			case 8:
				for (space = text::swap_endian<SWAP>(str[i]);
					space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
					space = text::swap_endian<SWAP>(str[++i]));
				if (text::swap_endian<SWAP>(str[i]) == U'-') {
					sig = -1; i++;
				}
				else if (text::swap_endian<SWAP>(str[i]) == U'+') {
					i++;
				}
				while (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'7') {
					res = (res << 3) | (text::swap_endian<SWAP>(str[i++]) - U'0');
				}
				if (text::swap_endian<SWAP>(str[i]) == U'o')i++;
				break;

			default: //case 10:
				for (space = text::swap_endian<SWAP>(str[i]);
					space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
					space = text::swap_endian<SWAP>(str[++i]));
				if (text::swap_endian<SWAP>(str[i]) == U'-') {
					sig = -1; i++;
				}
				else if (text::swap_endian<SWAP>(str[i]) == U'+') {
					i++;
				}
				while (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'9') {
					res = (res * 10) + (text::swap_endian<SWAP>(str[i++]) - U'0');
				}
				if (text::swap_endian<SWAP>(str[i]) == U'd')i++;
				break;
			case 16:
				for (space = text::swap_endian<SWAP>(str[i]);
					space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
					space = text::swap_endian<SWAP>(str[++i]));
				if (text::swap_endian<SWAP>(str[i]) == U'-') {
					sig = -1; i++;
				}
				else if (text::swap_endian<SWAP>(str[i]) == U'+') {
					i++;
				}
			LOOP:
				if (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'7')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - U'0');
				else if (text::swap_endian<SWAP>(str[i]) >= U'a' && text::swap_endian<SWAP>(str[i]) <= U'f')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - U'a' + 10);
				else if (text::swap_endian<SWAP>(str[i]) >= U'A' && text::swap_endian<SWAP>(str[i]) <= U'F')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - U'A' + 10);
				else goto END;
				goto LOOP;
			END:
				if (text::swap_endian<SWAP>(str[i]) == U'h')i++;
				break;
			}
			return res * sig;
		}

		template<encoding E>
		inline ulong64 text_parser<E>::str_to_unsigned(str_view<const typename text_parser<E>::char_t, E> str, windex& i, int base) {
			static constexpr bool SWAP = text::is_endian_swapped<E>::value;

			ulong64 res = 0; char32_t space;

			switch (base)
			{
			case 2:
				for (space = text::swap_endian<SWAP>(str[i]);
					space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
					space = text::swap_endian<SWAP>(str[++i]));

				while (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'1') {
					res = (res << 1) | (text::swap_endian<SWAP>(str[i++]) - U'0');
				}
				if (text::swap_endian<SWAP>(str[i]) == U'b')i++;
				break;
			case 8:
				for (space = text::swap_endian<SWAP>(str[i]);
					space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
					space = text::swap_endian<SWAP>(str[++i]));

				while (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'7') {
					res = (res << 3) | (text::swap_endian<SWAP>(str[i++]) - U'0');
				}
				if (text::swap_endian<SWAP>(str[i]) == U'o')i++;
				break;
			default://case 10:
				for (space = text::swap_endian<SWAP>(str[i]);
					space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
					space = text::swap_endian<SWAP>(str[++i]));
				while (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'9') {
					res = (res * 10) + (text::swap_endian<SWAP>(str[i++]) - U'0');
				}
				if (text::swap_endian<SWAP>(str[i]) == U'd')i++;
				break;
			case 16:
				for (space = text::swap_endian<SWAP>(str[i]);
					space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
					space = text::swap_endian<SWAP>(str[++i]));
			LOOP:
				if (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'7')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - U'0');
				else if (text::swap_endian<SWAP>(str[i]) >= U'a' && text::swap_endian<SWAP>(str[i]) <= U'f')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - U'a' + 10);
				else if (text::swap_endian<SWAP>(str[i]) >= U'A' && text::swap_endian<SWAP>(str[i]) <= U'F')
					res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - U'A' + 10);
				else goto END;
				goto LOOP;
			END:
				if (text::swap_endian<SWAP>(str[i]) == U'h')i++;
				break;
			}
			return res;
		}

		template<encoding E>
		inline double text_parser<E>::str_to_floating(str_view<const typename text_parser<E>::char_t, E> str, windex& i, bool ex) {
			static constexpr bool SWAP = text::is_endian_swapped<E>::value;

			double res = 0, dec = 0.1; int sig = 1; char32_t space;

			for (space = text::swap_endian<SWAP>(str[i]);
				space == U' ' || space == U'\t' || space == U'\n' || space == U'\r';
				space = text::swap_endian<SWAP>(str[++i]));

			if (text::swap_endian<SWAP>(str[i]) == U'-') {
				sig = -1; i++;
			}
			else if (text::swap_endian<SWAP>(str[i]) == U'+') {
				i++;
			}

			while (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'9') {
				res = (res * 10) + (text::swap_endian<SWAP>(str[i]) - U'0');
				i++;
			}

			if (text::swap_endian<SWAP>(str[i]) != U'.') {
				return res * sig;
			}
			i++;

			while (text::swap_endian<SWAP>(str[i]) >= U'0' && text::swap_endian<SWAP>(str[i]) <= U'9') {
				res = res + (text::swap_endian<SWAP>(str[i]) - U'0') * dec;
				dec /= 10.0;
				i++;
			}
			res = res * sig;

			if (text::swap_endian<SWAP>(str[i]) == U'e' || text::swap_endian<SWAP>(str[i]) == U'E') {
				i++;
				int exp = (int)str_to_signed(str, i, 10);
				res = res * pow(10, exp);
			}
			return res;
		}

		template<encoding E> template<typename T, template<typename>class A>
		inline text::fast_string<E, A>& text_parser<E>::value_to_string(text::fast_string<E, A>& stream, T const& value, text_format_t const& f)
		{
			if constexpr (is_floating_value<T>::value)
			{
				floating_to_string(stream, value, f);
			}
			else if constexpr (is_unsigned_value<T>::value)
			{
				unsigned_to_string(stream, value, f);
			}
			else if constexpr (is_integer_value<T>::value)
			{
				signed_to_string(stream, value, f);
			}
			else if constexpr (is_same_type<typename remove_reference<typename remove_const<T>::type>::type,bool>::value)
			{
				bool_to_string(stream, value, f);
			}
			return stream;
		}

		template<encoding E> template<template<typename>class A>
		inline text::fast_string<E, A>& text_parser<E>::bool_to_string(text::fast_string<E, A>& stream, bool value, text_format_t const& f)
		{	
			static const ang::cstr_t s_array[3][3][2] = {
				{{"false"_sv, "true"_sv},
				{"FALSE"_sv, "TRUE"_sv},
				{"False"_sv, "True"_sv}},
				{{"no"_sv, "yes"_sv},
				{"NO"_sv, "YES"_sv},
				{"No"_sv, "yes"_sv}},
				{{"low"_sv, "high"_sv},
				{"LOW"_sv, "HIGH"_sv},
				{"Low"_sv, "High"_sv}}
			};
			text_format_flags_t flags;
			flags.value = f.format_flags();

			stream << s_array[flags.type][flags.case_][value ? 0 : 1];
			return stream;
		}

		template<encoding E> template<template<typename>class A>
		inline text::fast_string<E, A>& text_parser<E>::signed_to_string(text::fast_string<E, A>& stream, long64 value, text_format_t const& f)
		{
			static constexpr bool IS_SWAPPED = is_endian_swapped<E>::value;
			static constexpr byte max_char = 120;
			text::text_format_flags_t format;
			format.value = f.format_flags();

			switch (format.target)
			{
			case text::text_format::text_: format.value = text::default_text_format<int>::format().format_flags(); break;
			case text::text_format::signed_: break;
			case text::text_format::unsigned_: return unsigned_to_string(stream, (ulong64)value, f);
			case text::text_format::float_: return floating_to_string(stream, (double)value, f);
			default: return stream << "invalid format"_sv;
			}
			int th = -1;
			char_t out[max_char] = { 0 };
			char_t fill = char_to_char<char_t, IS_SWAPPED>(format.fill);
			int max = format.max;
			bool neg = false;
			uint c = max_char - 2;

			if (max == 0 || max > max_char)
				max = max_char - 1;

			if (fill == 0)
			{
				max = 0;
				fill = char_to_char<char_t, IS_SWAPPED>('0');
			}

			if (format.thousand != 0)
				th = 0;

			if (format.base == 3)
			{
				if (value == 0)
				{
					out[c--] = char_to_char<char_t, IS_SWAPPED>('0');
				}
				else do
				{
					if (th >= 0) {
						if (th == 4) {
							out[c--] = char_to_char<char_t, IS_SWAPPED>(format.thousand);
							th = 0;
						}
						th++;
					}
					if (value != 0)
						out[c--] = char_to_char<char_t, IS_SWAPPED>((0X0001 & value) ? '1' : '0');
					else
						out[c--] = fill;
					value = value >> 1;
					max--;
				} while (value != 0 || max > 0);
			}
			else
			{
				if (value < 0)
				{
					neg = true;
					value *= -1;
				}
				if (format.base == 2)
				{
					if (value == 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>('0');
					}
					else do
					{
						if (th >= 0) {
							if (th == 4) {
								out[c--] = char_to_char<char_t, IS_SWAPPED>(format.thousand);
								th = 0;
							}
							th++;
						}
						if (value != 0)
						{
							out[c--] = char_to_char<char_t, IS_SWAPPED>(hexu[(byte)0X000F & value]);
							value = value >> 4;
						}
						else
							out[c--] = fill;
						max--;
					} while (value > 0 || max > 0);
				}
				else if (format.base == 1)
				{
					if (value == 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>('0');
					}
					else do
					{
						if (th >= 0) {
							if (th == 4) {
								out[c--] = char_to_char<char_t, IS_SWAPPED>(format.thousand);
								th = 0;
							}
							th++;
						}
						if (value != 0)
						{
							out[c--] = char_to_char<char_t, IS_SWAPPED>(hexl[(byte)0X000F & value]);
							value = value >> 4;
						}
						else
							out[c--] = fill;
						max--;
					} while (value > 0 || max > 0);
				}
				else
				{
					if (value == 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>('0');
					}
					else do
					{
						if (th >= 0) {
							if (th == 3) {
								out[c--] = char_to_char<char_t, IS_SWAPPED>(format.thousand);
								th = 0;
							}
							th++;
						}

						if (value != 0)
						{
							out[c--] = char_to_char<char_t, IS_SWAPPED>('0' + value % 10);
							value /= 10;
						}
						else
							out[c--] = fill;
						max--;
					} while (value > 0 || max > 0);
				}
				if (neg)
					out[c--] = char_to_char<char_t, IS_SWAPPED>('-');
				else if (format.sign)
					out[c--] = char_to_char<char_t, IS_SWAPPED>('+');
			}
			stream << str_view<const char_t, E>(&out[c + 1u], max_char - c);
			return stream;
		}

		template<encoding E> template<template<typename>class A>
		inline text::fast_string<E, A>& text_parser<E>::unsigned_to_string(text::fast_string<E, A>& stream, ulong64 value, text_format_t const& f)
		{
			static constexpr bool IS_SWAPPED = is_endian_swapped<E>::value;
			static constexpr byte max_char = 120;
			text::text_format_flags_t format;
			format.value = f.format_flags();

			switch (format.target)
			{
			case text::text_format::text_: format.value = text::default_text_format<uint>::format().format_flags(); break;
			case text::text_format::unsigned_: break;
			case text::text_format::signed_: return signed_to_string(stream, (long64)value, f);
			case text::text_format::float_: return floating_to_string(stream, (double)value, f);
			default: return stream << "invalid format"_sv;
			}

			char_t out[max_char] = { 0 };
			char_t fill = char_to_char<char_t, IS_SWAPPED>(format.fill);
			int th = -1;

			int max = format.max;
			bool neg = false;
			uint c = max_char - 2;

			if (max == 0 || max > max_char)
				max = max_char - 1;

			if (fill == 0)
			{
				max = 0;
				fill = char_to_char<char_t, IS_SWAPPED>('0');
			}

			if (format.thousand != 0)
				th = 0;

			if (format.base == 3)
			{
				if (value == 0)
				{
					out[c--] = char_to_char<char_t, IS_SWAPPED>('0');
				}
				else do
				{
					if (th >= 0) {
						if (th == 4) {
							out[c--] = char_to_char<char_t, IS_SWAPPED>(format.thousand);
							th = 0;
						}
						th++;
					}
					if (value != 0)
						out[c--] = char_to_char<char_t, IS_SWAPPED>((0X0001 & value) ? '1' : '0');
					else
						out[c--] = fill;
					value = value >> 1;
					max--;
				} while (value != 0 || max > 0);
			}
			else
			{
				if (value < 0)
				{
					neg = true;
					value *= -1;
				}
				if (format.base == 2)
				{
					if (value == 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>('0');
					}
					else do
					{
						if (th >= 0) {
							if (th == 4) {
								out[c--] = char_to_char<char_t, IS_SWAPPED>(format.thousand);
								th = 0;
							}
							th++;
						}
						if (value != 0)
						{
							out[c--] = char_to_char<char_t, IS_SWAPPED>(hexu[(byte)0X000F & value]);
							value = value >> 4;
						}
						else
							out[c--] = fill;
						max--;
					} while (value > 0 || max > 0);
				}
				else if (format.base == 1)
				{
					if (value == 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>('0');
					}
					else do
					{
						if (th >= 0) {
							if (th == 4) {
								out[c--] = char_to_char<char_t, IS_SWAPPED>(format.thousand);
								th = 0;
							}
							th++;
						}
						if (value != 0)
						{
							out[c--] = char_to_char<char_t, IS_SWAPPED>(hexl[(byte)0X000F & value]);
							value = value >> 4;
						}
						else
							out[c--] = fill;
						max--;
					} while (value > 0 || max > 0);
				}
				else
				{
					if (value == 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>('0');
					}
					else do
					{
						if (th >= 0) {
							if (th == 3) {
								out[c--] = char_to_char<char_t, IS_SWAPPED>(format.thousand);
								th = 0;
							}
							th++;
						}

						if (value != 0)
						{
							out[c--] = char_to_char<char_t, IS_SWAPPED>('0' + value % 10);
							value /= 10;
						}
						else
							out[c--] = fill;
						max--;
					} while (value > 0 || max > 0);
				}

			}
			stream << str_view<const char_t, E>(&out[c + 1u], max_char - c);
			return stream;
		}

		template<encoding E> template<template<typename>class A>
		inline text::fast_string<E, A>& text_parser<E>::floating_to_string(text::fast_string<E, A>& stream, double  value, text_format_t const& f)
		{
			static constexpr bool IS_SWAPPED = is_endian_swapped<E>::value;
			static constexpr byte max_char = 120;
			text::text_format_flags_t format;
			format.value = f.format_flags();

			switch (format.target)
			{
			case text::text_format::text_: format.value = text::default_text_format<double>::format().format_flags(); break;
			case text::text_format::float_: break;
			case text::text_format::signed_: return signed_to_string(stream, (long64)value, f);
			case text::text_format::unsigned_: return unsigned_to_string(stream, (ulong64)value, f);
			default: return stream << "invalid format"_sv;
			}

			char_t out[max_char] = { 0 };
			char_t fill = char_to_char<char_t, IS_SWAPPED>(format.fill);
			int max = format.max;
			int pres = format.pres;
			bool exp = format.exponent;
			word c = max_char - 2;

			if (max == 0 || max > max_char)
				max = max_char - 1;

			if (fill == 0)
			{
				max = 0;
				fill = char_to_char<char_t, IS_SWAPPED>('0');
			}

			union
			{
				ulong64 int_value;
				double float_value;
			}union_value;
			union_value.float_value = value;
			if (format.base == 3)
			{
				do
				{
					if (union_value.int_value != 0)
						out[c--] = char_to_char<char_t, IS_SWAPPED>((0X0001 & union_value.int_value) ? '1' : '0');
					else
						out[c--] = fill;
					union_value.int_value = union_value.int_value >> 1;
					max--;
				} while (union_value.int_value != 0 || max > 0);
			}
			if (format.base == 2)
			{
				do
				{
					if (union_value.int_value != 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>(hexu[(byte)(0X000F & union_value.int_value)]);
						union_value.int_value = union_value.int_value >> 4;
					}
					else
						out[c--] = fill;
					max--;
				} while (union_value.int_value > 0 || max > 0);
			}
			else if (format.base == 1)
			{
				do
				{
					if (union_value.int_value != 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>(hexl[(byte)(0X000F & union_value.int_value)]);
						union_value.int_value = union_value.int_value >> 4;
					}
					else
						out[c--] = fill;
					max--;
				} while (union_value.int_value > 0 || max > 0);
			}
			else
			{
				bool neg = false;
				long64 int_value;
				int iexp = 0;
				if (value < 0.0f)
				{
					neg = true;
					value *= -1.0f;
				}
				if (exp)
				{
					bool bexp = false;
					if (value >= 10.0f)
					{
						bexp = false;
						do {
							iexp++;
							value /= 10.0f;
						} while (value >= 10.0f);
					}
					else if (value < 1.0f)
					{
						bexp = true;
						do {
							iexp++;
							value *= 10.0f;
						} while (value < 1.0f);
					}
					do
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>('0' + iexp % 10);
						iexp /= 10;
					} while (iexp > 0);
					if (bexp)out[c--] = char_to_char<char_t, IS_SWAPPED>('-');
					out[c--] = char_to_char<char_t, IS_SWAPPED>('e');
				}

				int_value = (long64)value;
				value = value - double(int_value);

				if (pres > 0)
				{
					c -= pres;
					for (word i = 0; i < pres; i++)
					{
						out[i + c + 1] = char_to_char<char_t, IS_SWAPPED>('0' + byte(value * 10.0f));
						value *= 10.0f;
						value -= double(int(value));
					}
					out[c--] = char_to_char<char_t, IS_SWAPPED>('.');
				}

				do
				{
					if (int_value != 0)
					{
						out[c--] = char_to_char<char_t, IS_SWAPPED>('0' + int_value % 10);
						int_value /= 10;
					}
					else
						out[c--] = fill;
					max--;
				} while (int_value > 0 || max > 0);

				if (neg)
					out[c--] = char_to_char<char_t, IS_SWAPPED>('-');
				else if (format.sign)
					out[c--] = char_to_char<char_t, IS_SWAPPED>('+');
			}

			return stream << str_view<const char_t, E>(&out[c + 1], max_char - c);
		}

		template<encoding E> template<typename T2, encoding E2>
		inline str_view<const typename text_parser<E>::char_t, E> text_parser<E>::seek(str_view<const typename text_parser<E>::char_t, E>const& source, str_view<T2, E2> const& value) {
			static constexpr bool SWAP1 = is_endian_swapped<E>::value;
			static constexpr bool SWAP2 = is_endian_swapped<E2>::value;
			
			int a = 0;
			wsize i = 0, j = 0, temp;
			wsize size = value.size();
			
			while (i < source.size() && j < size)
			{
				char32 ch = to_char32<false, SWAP2>(value, j);

				if (ch == U' ' || ch == U'\t') //character '\t' or ' ' means "read all spaces"
				{
					temp = j;
					while (ch = to_char32<false, SWAP2>(value, temp) == U' ' || ch == U'\t') j = temp; //read all spaces
					while (source[i] == U' ' || source[i] == U'\t') i++; //read all spaces	
					continue;
				}

				if (ch == U'\n' || ch == U'\r')  //character '\n' or '\r' means "read all endlines"
				{
					temp = j;
					while (ch = to_char32<false, SWAP2>(value, j) == U'\n' || ch == U'\r') j = temp; //read all spaces
					while (source[i] == U'\n' || source[i] == U'r') i++; //read all spaces	
					continue;
				}
				temp = i;
				if (to_char32<false, SWAP1>(source, temp) != ch) //diferent text
					break;
				i = temp;
			}
			return str_view<const char_t, E>(&source[i], source.size() - i);
		}

		template<encoding E> template<template<typename>class A, typename T2, encoding E2, typename...Ts>
		inline bool text_parser<E>::format(text::fast_string<E, A>& stream, str_view<T2, E2> f, Ts&&...args) {
			return format(stream, f, pack_args(ang::forward<Ts>(args)...));
		}

		template<encoding E> template<template<typename>class A, typename T2, encoding E2, typename...Ts>
		inline bool text_parser<E>::format(text::fast_string<E, A>& stream, str_view<T2, E2> format, vector<anyone<Ts...>>&& args) {
			int a;
			text::text_format_flags_t f;
			wsize i = 0, t = 0, n = 0, l = 0;
			while (char32_t c = text::to_char32<false, false>(format, i))
			{
				if (c == U'{') {
					f.value = text_parser<E2>::parse(format, n, a);
					if (a > -1 && a < (long64)args.size()) {
						stream.append(str_view<T2, E2>(&format[l], t - l));
						f.value ? args[a].to_string(stream, f) : args[a].to_string(stream);
						i = l = n;
					}
				}
				t = n = i;
			}
			stream.append(str_view<T2, E2>(&format[l], i - l));
			return false;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename T, bool = is_integer_value<T>::value, bool = is_unsigned_value<T>::value , bool = is_floating_value<T>::value >
		struct value_to_string {
			template<encoding E = encoding::ascii, template<typename>class A = memory::buffer_allocator>
			static fast_string<E, A>& to_string(T const&) {
				return type_of<T>().type_name();
			}
			template<encoding E = encoding::ascii, template<typename>class A = memory::buffer_allocator>
			static fast_string<E, A>& to_string(T const&, text::text_format_t const&) {
				return type_of<T>().type_name();
			}
			template<encoding E = encoding::ascii, template<typename>class A = memory::buffer_allocator>
			static fast_string<E, A>& to_string(fast_string<E, A>& stream, T const& value, text::text_format_t const& f) {
				return stream << type_of<T>().type_name();
			}
		};

		template<typename T, gender_t gender = gender_of<T>()>
		struct __to_string_helper {
			static castr_t to_string(const T&) {
				return type_of<T>().type_name();
			}

			static castr_t to_string(const T&, text::text_format_t) {
				return type_of<T>().type_name();
			}
		};

		template<typename T>
		struct __to_string_helper<T, gender::value_type> {
			static fast_astring_t to_string(const T& value, const text::text_format_t& f = text::default_text_format<T>::format()) {
				fast_astring_t stream;
				text_parser<text::encoding::ascii>::value_to_string(stream, value, f);
				return ang::move(stream);
			}
			
			template<encoding E, template<typename>class A = memory::buffer_allocator>
			static fast_string<E, A> to_string(const T& value, const text::text_format_t& f = text::default_text_format<T>::format()) {
				fast_string<E, A> stream;
				text_parser<E>::value_to_string(stream, value, f);
				return ang::move(stream);
			}

			template<encoding E, template<typename>class A>
			static fast_string<E, A>& to_string(fast_string<E, A>& stream, const T& value, const text::text_format_t& f = text::default_text_format<T>::format()) {
				return text_parser<E>::value_to_string(stream, value, f);
			}
		};


		template<typename T>
		struct __to_string_helper<enum_class<T>, gender::enum_type> {
			static fast_astring_t to_string(const enum_class<T>& value, const text::text_format_t& f = text::default_text_format<typename ang::value<T>::base>::format()) {
				fast_astring_t stream;
				stream << value.to_string();
				return ang::move(stream);
			}

			template<encoding E, template<typename>class A = memory::buffer_allocator>
			static fast_string<E, A> to_string(const enum_class<T>& value, const text::text_format_t& f = text::default_text_format<typename ang::value<T>::base>::format()) {
				fast_string<E, A> stream;
				stream << value.to_string();
				return ang::move(stream);
			}

			template<encoding E, template<typename>class A>
			static fast_string<E, A>& to_string(fast_string<E, A>& stream, const enum_class<T>& value, const text::text_format_t& f = text::default_text_format<typename ang::value<T>::base>::format()) {
				stream << value.to_string();
				return ang::move(stream);
			}
		};

		template<typename T, typename = void>
		struct __to_string_helper_class_1
			: false_type {
			static castr_t to_string(const T&) {
				return "unknown";// type_of<T>().type_name();
			}
		};

		template<typename T, typename = void>
		struct __to_string_helper_class_2
			: false_type {
			static auto to_string(const T& val, text::text_format_t const&) {
				return __to_string_helper_class_1<T>::to_string(val);
			}
		};

		template<typename T>
		struct __to_string_helper_class_1<T, void_t<decltype(declval<T>().to_string())>>
			: true_type {
			static auto to_string(const T& val) {
				return val.to_string();
			}
		};

		template<typename T>
		struct __to_string_helper_class_2<T, void_t<decltype(declval<T>().to_string(declval<text_format_t>()))>>
			: true_type {
			static auto to_string(const T& val, text_format_t const& f) {
				return val.to_string(f);
			}
		};

		template<typename T>
		struct __to_string_helper<T, gender::class_type> {
			static auto to_string(const T& value) {
				return __to_string_helper_class_1<T>::to_string(value);
			}

			static auto to_string(const T& value, const text::text_format_t& f) {
				return __to_string_helper_class_2<T>::to_string(value, f);
			}

			template<text::encoding E, template<typename>class A>
			static fast_string<E, A>& to_string(fast_string<E, A>& out , const T& value, const text::text_format_t& f) {
				return out << __to_string_helper_class_2<T>::to_string(value, f);
			}
		};

		template<typename T>
		auto to_string(const T& val) {
			return __to_string_helper<T>::to_string(val);
		}

		template<typename T>
		auto to_string(const T& val, text_format_t const& f) {
			return __to_string_helper<T>::to_string(val, f);
		}

		template<typename T, text::encoding E, template<typename>class A>
		fast_string<E, A>& to_string(fast_string<E, A>& out, T const& val, text_format_t const& f = text::default_text_format<T>::format()) {
			return __to_string_helper<T>::to_string(out, val, f);
		}

		template<typename T>
		auto to_string(T* const& val) {
			return to_string(*val);
		}

		template<typename T>
		auto to_string(T* const& val, text_format_t const& f) {
			return to_string(*val, f);
		}

		

		///////////////////////////////////////////////////////////////////////////////////////////////////

	}
	
}

#endif//__ANG_BASE_TEXT_INL__