#ifndef __ANG_BASE_TEXT_H__
#error ...
#elif !defined __ANG_BASE_TEXT_INL__
#define __ANG_BASE_TEXT_INL__

namespace ang
{

	namespace text
	{
		template<typename T> inline dword char_to_dword(T value) {
			static_assert(is_char_type<T>::value, "T is not a char type");
			return (dword)(typename integer_type_by_size<sizeof(T)>::uint_t)value;
		}

		template <bool SWAP, typename T> inline constexpr T swap_endian(T value) {
			static_assert(is_char_type<T>::value, "T is not a char type"); return value;
		}

		template <> inline constexpr char16_t swap_endian<true, char16_t>(char16_t value) {
			return char16_t((word(value) & 0XFF00) >> 8) | ((word(value) & 0X00FF) << 8);
		}

		template <> inline constexpr char32_t swap_endian<true, char32_t>(char32_t value) {
			return char32_t((dword(value) & 0XFF000000) >> 24) | ((dword(value) & 0X00FF0000) >> 8) | ((dword(value) & 0X0000FF00) << 8) | ((dword(value) & 0X000000FF) << 24);
		}

		template<typename To, typename From = To>
		struct converter {
			template<bool SWAP2> static inline windex size(const From* value, windex& idx) { idx++; return 1u; }
			template<bool SWAP1, bool SWAP2> static inline void convert(To* dest, windex& i, From const* src, windex& j) {
				dest[i++] = swap_endian<SWAP1>((To)char_to_dword(swap_endian<SWAP2>(src[j++])));
			}
		};

		template<typename To>
		struct converter<To, mchar> {
			template<bool SWAP2> static inline windex size(const mchar* src, windex& j) {
				switch (char_to_dword(src[j]) & 0XF0) {
				case 0XD0:
				case 0XC0: j += 2; break;
				case 0XE0: j += 3; break;
				case 0XF0: j += 4; break;
				default: j++; break;
				}
				return 1u;
			}
			template<bool SWAP1, bool> static inline void convert(To* dest, windex& i, const mchar* src, windex& j) {
				dword out;
				switch (char_to_dword(src[j]) & 0XF0) {
				case 0XD0:
				case 0XC0:
					out = (char_to_dword(src[j++]) & 0X1F) << 6;
					out |= char_to_dword(src[j++]) & 0X3F;
					break;
				case 0XE0:
					out = (char_to_dword(src[j++]) & 0X0F) << 12;
					out |= (char_to_dword(src[j++]) & 0X3F) << 6;
					out |= char_to_dword(src[j++]) & 0X3F;
					break;
				case 0XF0:
					out = (char_to_dword(src[j++]) & 0X0F) << 18;
					out |= (char_to_dword(src[j++]) & 0X3F) << 12;
					out |= (char_to_dword(src[j++]) & 0X3F) << 6;
					out |= char_to_dword(src[j++]) & 0X3F;
					break;
				default: out = char_to_dword(src[j++]); break;
				}
				dest[i++] = swap_endian<SWAP1>((To)out);
			}
		};

		template<typename To>
		struct converter<To, char16_t> {
			template<bool SWAP2> static inline windex size(const char16_t* words, windex& j) {
				if ((swap_endian<SWAP2>(words[j]) & 0XFC00) == 0XDC00) j += 2;
				else j++;
				return 1u;
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(To* dest, windex& i, const char16_t* src, windex& j) {
				dword out;
				char16_t c = swap_endian<SWAP2>(src[j]);
				if ((c & 0XFC00) == 0XDC00) {
					out = char_to_dword(swap_endian<SWAP2>(src[j++])) & 0X03FF;
					out |= (char_to_dword(swap_endian<SWAP2>(src[j++])) & 0X03FF) << 10;
					out += 0X10000;
				}
				else out = char_to_dword(swap_endian<SWAP2>(src[j++]));
				dest[i++] = (To)char_to_dword(swap_endian<SWAP1>((To)out));
			}
		};

		template<typename From>
		struct converter<mchar, From> {
			template<bool SWAP2> static inline windex size(const From* src, windex& j) {
				dword n = char_to_dword(swap_endian<SWAP2>(src[j]));
				if (n <= 0X7F) { j++; return 1u; }
				else if (n <= 0X7FF) { j++; return 2u; }
				else if (n <= 0XFFFF) { j++; return 3u; }
				else { j++; return 4u; }
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(mchar* dest, windex& i, const From* src, windex& j) {
				if (char_to_dword(src[j]) <= 0X7F) {
					dest[i++] = (mchar)char_to_dword(src[j++]);
				}
				else if (char_to_dword(src[j]) <= 0X7FF) {
					dest[i++] = 0XC0 | (0X1F & (char_to_dword(src[j]) >> 6));
					dest[i++] = 0X80 | (0X3F & char_to_dword(src[j++]));
				}
				else if (char_to_dword(src[j]) <= 0XFFFF) {
					dest[i++] = 0XE0 | (0X0F & (char_to_dword(src[j]) >> 12));
					dest[i++] = 0X80 | (0X3F & (char_to_dword(src[j]) >> 6));
					dest[i++] = 0X80 | (0X3F & char_to_dword(src[j++]));
				}
				else {
					dest[i++] = 0XF0 | (0X07 & (char_to_dword(src[j]) >> 18));
					dest[i++] = 0X80 | (0X3F & (char_to_dword(src[j]) >> 12));
					dest[i++] = 0X80 | (0X3F & (char_to_dword(src[j]) >> 6));
					dest[i++] = 0X80 | (0X3F & char_to_dword(src[j++]));
				}
			}
		};

		template<typename From>
		struct converter<char16_t, From> {
			template<bool SWAP2> static inline windex size(const From* src, windex& j) {
				if (char_to_dword(swap_endian<SWAP2>(src[j++])) > 0XFFFF) return 2u;
				else return 1u;
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(char16_t* dest, windex& i, const From* src, windex& j) {
				if (char_to_dword(swap_endian<SWAP2>(src[j])) > 0XFFFF) {
					dest[i++] = swap_endian<SWAP1, char16_t>(0XDC00 | (char_to_dword(swap_endian<SWAP2>(src[j])) & 0X02FF));
					dest[i++] = swap_endian<SWAP1, char16_t>(0XD800 | (((char_to_dword(swap_endian<SWAP2>(src[j++])) - 0X10000) >> 10) & 0X02FF));
				}
				else
					dest[i++] = (char16_t)char_to_dword(swap_endian<xor_expression<SWAP1, SWAP2>::value>(src[j++]));
			}
		};

		template<>
		struct converter<mchar, char16_t> {
			template<bool SWAP> static inline windex size(const char16_t* src, windex& j) {
				if ((char_to_dword(swap_endian<SWAP>(src[j])) & 0XDC00) == 0XDC00) {
					j += 2; return 4;
				}
				else {
					if (char_to_dword(swap_endian<SWAP>(src[j])) <= 0X7F) {
						j++; return 1;
					}
					else if (char_to_dword(swap_endian<SWAP>(src[j])) <= 0X7FF) {
						j++; return 2;
					}
					else {
						j++; return 3;
					}
				}
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(mchar* dest, windex& i, const char16_t* src, windex& j) {
				if ((swap_endian<SWAP2>(src[j]) & 0XFC00) == 0XDC00) {
					mchar temp = ((swap_endian<SWAP2>(src[j]) >> 6) & 0X0F) + 1; //110110ww wwzzzzyy (uuuuu = wwww + 1)
					dest[i++] = 0XF0 | (temp >> 2); //11110uuu
					dest[i++] = 0X80 | (0X30 & (temp << 4)) | (0X0F & (swap_endian<SWAP2>(src[j]) >> 2));
					dest[i++] = 0X80 | (0X30 & (swap_endian<SWAP2>(src[j++]) << 4)) | (0X0F & (swap_endian<SWAP2>(src[j]) >> 6));
					dest[i++] = 0X80 | (0X3F & swap_endian<SWAP2>(src[j++]));
				}
				else {
					if (char_to_dword(swap_endian<SWAP2>(src[j])) <= 0X7F) {
						dest[i++] = mchar(swap_endian<SWAP2>(src[j++]));
					}
					else if (char_to_dword(swap_endian<SWAP2>(src[j])) <= 0X7FF) {
						dest[i++] = 0XC0 | (0X1F & (char_to_dword(swap_endian<SWAP2>(src[j])) >> 6));
						dest[i++] = 0X80 | (0X3F & char_to_dword(swap_endian<SWAP2>(src[j++])));
					}
					else {
						dest[i++] = 0XE0 | (0X0F & (char_to_dword(swap_endian<SWAP2>(src[j])) >> 12));
						dest[i++] = 0X80 | (0X3F & (char_to_dword(swap_endian<SWAP2>(src[j])) >> 6));
						dest[i++] = 0X80 | (0X3F & char_to_dword(swap_endian<SWAP2>(src[j++])));
					}
				}
			}
		};

		template<>
		struct converter<char16_t, mchar> {
			template<bool SWAP> static inline windex size(const mchar* src, windex& j) {
				switch (char_to_dword(src[j]) & 0XF0)
				{
				default: j++; return 1;
				case 0XD0:
				case 0XC0: j += 2; return 1;
				case 0XE0: j += 3; return 1;
				case 0XF0: j += 4; return 2;
				}
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(char16_t* dest, windex& i, const mchar* src, windex& j) {
				char16_t temp;
				switch (char_to_dword(src[j]) & 0XF0)
				{
				case 0XD0://0X110x -> 0X1101
				case 0XC0://0X110x -> 0X1100
					dest[i] = swap_endian<SWAP1, char16_t>(((char_to_dword(src[j++]) & 0X1F) << 6));
					dest[i++] |= swap_endian<SWAP1, char16_t>((char16_t)(char_to_dword(src[j++]) & 0X3F));
					break;
				case 0XE0:
					dest[i] = swap_endian<SWAP1, char16_t>((char16_t)((char_to_dword(src[j++]) & 0X0F) << 12));
					dest[i] |= swap_endian<SWAP1, char16_t>((char16_t)((char_to_dword(src[j++]) & 0X3F) << 6));
					dest[i++] |= swap_endian<SWAP1, char16_t>((char16_t)(char_to_dword(src[j++]) & 0X3F));
					break;
				case 0XF0:
					temp = (char16_t)((char_to_dword(src[j++]) & 0X07) << 2);
					temp |= (char16_t)((char_to_dword(src[j]) & 0X30) >> 4);
					temp |= (temp - 1) << 6;
					temp |= (char16_t)((char_to_dword(src[j++]) & 0X0F) << 2);
					temp |= 0XD800 | (char16_t)((char_to_dword(src[j]) & 0X30) >> 4);
					temp |= (char16_t)((char_to_dword(src[j++]) & 0X0F) << 6);
					dest[i++] = swap_endian<SWAP1, char16_t>(0XDC00 | (char16_t)(char_to_dword(src[j++]) & 0X3F));
					dest[i++] = swap_endian<SWAP1, char16_t>(temp);
					break;
				default:
					dest[i++] = swap_endian<SWAP1, char16_t>((char16_t)char_to_dword(src[j++]));
					break;
				}

			}
		};

		template<>
		struct converter<mchar, mchar> {
			template<bool> static inline windex size(const mchar* src, windex& j) {
				switch (char_to_dword(src[j]) & 0XF0)
				{
				default: j++; return 1;
				case 0XD0:
				case 0XC0: j += 2; return 2;
				case 0XE0: j += 3; return 3;
				case 0XF0: j += 4; return 4;
				}
			}
			template<bool, bool> static inline void convert(mchar* dest, windex& i, const mchar* src, windex& j) {
				dest[i++] = src[j++];
			}
		};

		template<>
		struct converter<char16_t, char16_t> {
			template<bool SWAP> static inline windex size(const char16_t* src, windex& j) {
				if ((swap_endian<SWAP>(src[j]) & 0XFC00) == 0XDC00) {
					j += 2; return 2;
				}
				else {
					j++; return 1;
				}
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(char16_t* dest, windex& i, const char16_t* src, windex& j) {
				dest[i++] = swap_endian<xor_expression<SWAP1, SWAP2>::value>(src[j++]);
			}
		};

		template<typename T> inline bool is_end_of_string(const T* cstr) { return cstr[0] == 0; }
		template<> inline bool is_end_of_string<mchar>(const mchar* cstr) { return cstr[0] == 0 || (cstr[0] == 0XC0 && cstr[1] == 0X80); }
		template<typename T> inline bool is_end_of_word(const T* cstr) {
			return *cstr == U' ' || *cstr == U'\t' || *cstr == U'\n' || *cstr == U'\r';
		}
		template<typename T> inline bool is_end_of_line(const T* cstr) {
			return *cstr == U'\n' || *cstr == U'\r';
		}

		template<bool SWAP1, bool SWAP2, typename T> char32_t to_char32(T const* val, wsize& j) {
			wsize i = 0; char32_t n;
			converter<char32_t, T>::template convert<SWAP1, SWAP2>(&n, i, val, j);
			return n;
		}

		template<encoding ENCODING> inline wsize encoder<ENCODING>::lenght(raw_cstr_t cstr) {
			wsize i = 0;
			if (cstr == nullptr)
				goto END;
		LOOP:
			if (is_end_of_string(&cstr[i])) goto END;
			converter<char_t, char_t>::template size<is_endian_swapped<ENCODING>::value>(cstr, i);
			goto LOOP;
		END:
			return i;
		}

		template<encoding ENCODING> inline wsize encoder<ENCODING>::count(raw_cstr_t cstr) {
			return encoder<encoding::utf32>::size(cstr);
		}

		template<encoding ENCODING> template <typename cstr2_t> inline wsize encoder<ENCODING>::size(cstr2_t cstr) {
			typedef typename char_type_by_type<cstr2_t>::char_t other_char_t;
			typedef typename char_type_by_type<cstr2_t>::cstr_t other_cstr_t;

			wsize i = 0, j = 0;
			if (cstr == nullptr) goto END;
		LOOP:
			if (is_end_of_string(&other_cstr_t(cstr)[j])) goto END;
			i += converter<char_t, other_char_t>::template size<is_endian_swapped<encoding_by_char_type<cstr2_t>::value>::value>((other_cstr_t)cstr, j);
			goto LOOP;
		END:
			return i;
		}

		template<encoding ENCODING> template <typename cstr2_t> inline int encoder<ENCODING>::compare(raw_cstr_t first, cstr2_t second)
		{
			typedef typename char_type_by_type<cstr2_t>::char_t other_char_t;
			typedef typename char_type_by_type<cstr2_t>::cstr_t other_cstr_t;

			if ((pointer)first == (pointer)(other_cstr_t)second) return 0;
			if (first == nullptr) return 1;
			else if ((other_cstr_t)second == nullptr) return -1;

			int r;
			windex i = 0, j = 0;
			char32_t c1, c2;
		LOOP:
			c1 = to_char32<false, is_endian_swapped<ENCODING>::value>(first, i);
			c2 = to_char32<false, is_endian_swapped<encoding_by_char_type<cstr2_t>::value>::value>((other_cstr_t)second, j);
			r = int(c1) - int(c2);
			if (r != 0) return r > 0 ? 1 : -1;
			else if (c1 == 0) return 0;
			goto LOOP;
		}

		template<encoding ENCODING> template <typename cstr2_t> inline windex encoder<ENCODING>::compare_until(raw_cstr_t first, cstr2_t second)
		{
			typedef typename char_type_by_type<cstr2_t>::char_t other_char_t;
			typedef typename char_type_by_type<cstr2_t>::cstr_t other_cstr_t;

			if (first == nullptr || (other_cstr_t)second == nullptr)
				return 0;
			if ((pointer)first == (pointer)(other_cstr_t)second)
				return lenght(first);

			windex i = 0, j = 0, k = 0;
			char32_t c1, c2;
		LOOP:
			c1 = to_char32<false, is_endian_swapped<ENCODING>::value>(first, i);
			c2 = to_char32<false, is_endian_swapped<encoding_by_char_type<cstr2_t>::value>::value>((other_cstr_t)second, j);
			if (c1 != c2 || c1 == 0) return k;
			k = i;
			goto LOOP;
		}

		template<encoding ENCODING> template <typename cstr2_t> inline windex encoder<ENCODING>::find(raw_cstr_t first, wsize s1, cstr2_t second, wsize s2, windex start)
		{
			typedef char_t char1_t;
			typedef typename char_type_by_type<cstr2_t>::char_t char2_t;		
			constexpr bool SWAP1 = is_endian_swapped<ENCODING>::value;
			constexpr bool SWAP2 = is_endian_swapped<encoding_by_char_type<char2_t>::value>::value;

			if (first == null || second == null) return invalid_index;
			if (s2 == 0 || start >= s1) return invalid_index;
			if (s2 > (s1 - start)) return invalid_index;

			char1_t const* beg = first + start;
			char1_t const* end = first + s1 - s2 + 1;

			char32_t c1, c2;
			windex i = start, j, k, l, t;
			do {
				j = 0;
				k = 0;
				l = 0;
				while (k < s2 && to_char32<false, SWAP1>(beg, j) == to_char32<false, SWAP2>(second, k)) 
					l = k;
				if (l == s2) return i;
				t = 0;
				to_char32<false,SWAP1>(beg, t);
				i += t;
				beg += t;
			} while (beg < end);
			return ang::invalid_index;
		}

		template<encoding ENCODING> template <typename cstr2_t> inline windex encoder<ENCODING>::find_reverse(raw_cstr_t first, wsize s1, cstr2_t second, wsize s2, windex start)
		{
			typedef char_t char1_t;
			typedef typename char_type_by_type<cstr2_t>::char_t char2_t;
			constexpr bool SWAP1 = is_endian_swapped<ENCODING>::value;
			constexpr bool SWAP2 = is_endian_swapped<encoding_by_char_type<char2_t>::value>::value;

			if (first == null || second == null) return ang::invalid_index;
			if (s2 == 0 || start > s1) return ang::invalid_index;
			if (s2 > start)	return ang::invalid_index;

			char1_t const* beg = first;
			char1_t const* end = first + (start - s2);
			windex i = start - s2, j, k, l;
			do {
				j = 0;
				k = 0;
				l = 0;
				while (k < s2 && to_char32<false, SWAP1>(beg, j) == to_char32<false, SWAP2>(second, k))
					l = k;
				if (l == s2) return i;
				i--;
			} while (beg <= --end);
			return ang::invalid_index;
		}

		template<encoding ENCODING> template <typename cstr2_t> inline typename encoder<ENCODING>::raw_str_t encoder<ENCODING>::convert(raw_str_t dest, cstr2_t src, bool eos, wsize max)
		{
			wsize i = 0, j = 0;
			return convert(dest, i, src, j, eos, max);
		}

		template<encoding ENCODING> template <typename cstr2_t> inline typename encoder<ENCODING>::raw_str_t encoder<ENCODING>::convert(raw_str_t dest, wsize& i, cstr2_t src, wsize& j, bool eos, wsize max)
		{
			typedef typename char_type_by_type<cstr2_t>::char_t other_char_t;
			typedef typename char_type_by_type<cstr2_t>::cstr_t other_cstr_t;

			if ((other_cstr_t)src == nullptr) goto END;
		LOOP:
			if (is_end_of_string(&other_cstr_t(src)[j]) || i >= max) goto END;
			converter<char_t, other_char_t>::template convert<is_endian_swapped<ENCODING>::value, is_endian_swapped<encoding_by_char_type<cstr2_t>::value>::value>(dest, i, (other_cstr_t)src, j);
			goto LOOP;
		END:
			if (eos)dest[i] = 0;
			return dest;
		}
	}

	inline str_view<const char> operator "" _s(const char* str, wsize sz) { return str_view<const char>(str, sz); }
	inline str_view<const mchar> operator "" _sm(const char* str, wsize sz) { return str_view<const mchar>((mchar const*)str, sz); }
	inline str_view<const wchar_t> operator "" _s(const wchar_t* str, wsize sz) { return str_view<const wchar_t>(str, sz); }
	inline str_view<const char16_t> operator "" _s(const char16_t* str, wsize sz) { return str_view<const char16_t>(str, sz); }
	inline str_view<const char32_t> operator "" _s(const char32_t* str, wsize sz) { return str_view<const char32_t>(str, sz); }

	typedef str_view<char> str_t;
	typedef str_view<mchar> mstr_t;
	typedef str_view<wchar> wstr_t;
	typedef str_view<char16_t> str16_t;
	typedef str_view<char32_t> str32_t;

	typedef str_view<const char> cstr_t;
	typedef str_view<const mchar> cmstr_t;
	typedef str_view<const wchar> cwstr_t;
	typedef str_view<const char16_t> cstr16_t;
	typedef str_view<const char32_t> cstr32_t;


	template<typename T, text::encoding ENCODING>
	inline long64 str_to_integer(str_view<T, ENCODING> str, windex& i, int base = 10) {
		static constexpr bool SWAP = text::is_endian_swapped<ENCODING>::value;

		long64 res = 0; int sig = 1; i = 0; char32_t space = 0;
		space = text::swap_endian<SWAP>(str[i]);
		while (space == ' ' || space == '\t' || space == '\n' || space == '\r')
			space = text::swap_endian<SWAP>(str[++i]);

		if (text::swap_endian<SWAP>(str[i]) == '-') {
			sig = -1; i++;
		}
		else if (text::swap_endian<SWAP>(str[i]) == '+') {
			i++;
		}

		switch (base)
		{
		case 2:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '1')
				res = (res << 1) | (text::swap_endian<SWAP>(str[i++]) - '0');
			if (text::swap_endian<SWAP>(str[i]) == 'b')i++;
			break;
		case 8:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '7')
				res = (res << 3) | (text::swap_endian<SWAP>(str[i++]) - '0');
			if (text::swap_endian<SWAP>(str[i]) == 'o')i++;
			break;
		case 10:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '9')
				res = (res * 10) + (text::swap_endian<SWAP>(str[i++]) - '0');
			if (text::swap_endian<SWAP>(str[i]) == 'd')i++;
			break;
		case 16:
		LOOP:
			if (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '7')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - '0');
			else if (text::swap_endian<SWAP>(str[i]) >= 'a' && text::swap_endian<SWAP>(str[i]) <= 'f')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - 'a' + 10);
			else if (text::swap_endian<SWAP>(str[i]) >= 'A' && text::swap_endian<SWAP>(str[i]) <= 'F')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - 'A' + 10);
			else goto END;
			goto LOOP;
		END:
			if (text::swap_endian<SWAP>(str[i]) == 'h')i++;
			break;
		default:return 0;
		}

		return res*sig;
	}

	template<typename T, text::encoding ENCODING>
	inline ulong64 str_to_uinteger(str_view<T, ENCODING> str, windex& i, int base = 10) {
		static constexpr bool SWAP = text::is_endian_swapped<ENCODING>::value;

		ulong64 res = 0; char32_t space = 0;
		i = 0;

		space = text::swap_endian<SWAP>(str[i]);
		while (space == ' ' || space == '\t' || space == '\n' || space == '\r')
			space = text::swap_endian<SWAP>(str[++i]);

		switch (base)
		{
		case 2:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '1') {
				res = (res << 1) | (text::swap_endian<SWAP>(str[i++]) - '0');
			}break;
		case 8:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '7') {
				res = (res << 3) | (text::swap_endian<SWAP>(str[i++]) - '0');
			}break;
		case 10:
			while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '9') {
				res = (res * 10) + (text::swap_endian<SWAP>(str[i++]) - '0');
			}break;
		case 16:
		LOOP:
			if (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '7')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - '0');
			else if (text::swap_endian<SWAP>(str[i]) >= 'a' && text::swap_endian<SWAP>(str[i]) <= 'f')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - 'a' + 10);
			else if (text::swap_endian<SWAP>(str[i]) >= 'A' && text::swap_endian<SWAP>(str[i]) <= 'F')
				res = (res << 4) + (text::swap_endian<SWAP>(str[i++]) - 'A' + 10);
			else goto END;
			goto LOOP;
		END:
			break;
		default:return 0;
		}

		return res;
	}

	template<typename T, text::encoding ENCODING>
	inline double str_to_floating(str_view<T, ENCODING> str, windex& i, bool ex = false) {
		static constexpr bool SWAP = text::is_endian_swapped<ENCODING>::value;

		double res = 0, dec = 0.1; int sig = 1; i = 0; char32_t space;

		space = text::swap_endian<SWAP>(str[i]);
		while (space == ' ' || space == '\t' || space == '\n' || space == '\r')
			space = text::swap_endian<SWAP>(str[++i]);

		if (text::swap_endian<SWAP>(str[i]) == '-') {
			sig = -1; i++;
		}
		else if (text::swap_endian<SWAP>(str[i]) == '+') {
			i++;
		}

		while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '9') {
			res = (res * 10) + (text::swap_endian<SWAP>(str[i]) - '0');
			i++;
		}

		if (text::swap_endian<SWAP>(str[i]) != '.')
			return res*sig;
		i++;

		while (text::swap_endian<SWAP>(str[i]) >= '0' && text::swap_endian<SWAP>(str[i]) <= '9') {
			res = res + (text::swap_endian<SWAP>(str[i]) - '0') * dec;
			dec /= 10.0;
			i++;
		}

		return res*sig;
	}
}

#endif//__ANG_BASE_TEXT_INL__