#pragma once
namespace ang
{
	namespace text
	{
		template<wsize SIZE> struct unsigned_type_by_size {
			typedef dword type;
		};

		template<> struct unsigned_type_by_size<1> {
			typedef byte type;
		};

		template<> struct unsigned_type_by_size<2> {
			typedef word type;
		};

		template<typename T>
		struct unsigned_type_by_type {
			typedef typename unsigned_type_by_size<sizeof(T)>::type  type;
		};

		template<typename T> dword dword_cast(T const& value){
			return (dword)(typename unsigned_type_by_type<T>::type)value;
		}

		template<encoding_enum ENCODING>
		struct ang_text_encoder
		{
			static constexpr bool is_endian_swapped() { return false; }
			static constexpr bool is_multibyte() { return false; }
			static encoding_t format() { return ENCODING; }
			static wsize length(typename char_type_by_encoding<ENCODING>::cstr_t cstr);

			static wsize size(pointer raw, encoding_t format);
			static wsize convert(typename char_type_by_encoding<ENCODING>::str_t str, wsize sz, pointer raw, encoding_t format, bool eos);
			static int compare(typename char_type_by_encoding<ENCODING>::cstr_t, pointer, encoding_t);
			static wsize compare_until(typename char_type_by_encoding<ENCODING>::cstr_t, pointer, encoding_t);
			static windex find(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t first, wsize s1, pointer second, wsize s2, encoding_t format, windex start);
			static windex find_revert(typename ang::text::char_type_by_encoding<ENCODING>::cstr_t first, wsize s1, pointer second, wsize s2, encoding_t format, windex start);
		};

		template<> constexpr bool ang_text_encoder<encoding::utf16_se>::is_endian_swapped() { return true; }
		template<> constexpr bool ang_text_encoder<encoding::utf32_se>::is_endian_swapped() { return true; }

		template<> constexpr bool ang_text_encoder<encoding::utf8>::is_multibyte() { return true; }
		template<> constexpr bool ang_text_encoder<encoding::utf16>::is_multibyte() { return true; }
		template<> constexpr bool ang_text_encoder<encoding::utf16_se>::is_multibyte() { return true; }


		template<typename T1, typename T2>
		struct converter {

			template<bool SWAP> static inline windex size(const T2* value, windex& idx) {
				idx++; return 1u;
			}
			template<bool SWAP> static inline T1 convert(const T2* value, windex& idx) {
				return  (T1)dword_cast(swap_endian<SWAP>(value[idx++]));
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(T1* dest, windex& i, T2 const* src, windex& j) {
				dest[i++] = swap_endian<SWAP1>(convert<SWAP2>(src, j));
			}
		};

		template<typename T>
		struct converter<T, byte> {
			template<bool SWAP> static inline windex size(const byte* bytes, windex& idx) {
				switch (dword_cast(bytes[idx]) & 0XF0) {
				case 0XD0:
				case 0XC0:
					idx += 2; break;
				case 0XE0:
					idx += 3; break;
				case 0XF0:
					idx += 4; break;
				default: idx ++; break;
				}
				return 1;
			}
			template<bool SWAP> static inline T convert(const byte* bytes, windex& idx) {
				dword out;
				switch (dword_cast(bytes[idx]) & 0XF0) {
				case 0XD0:
				case 0XC0:
					out = (dword_cast(bytes[idx++]) & 0X1F) << 6;
					out |= dword_cast(bytes[idx++]) & 0X3F;
					break;
				case 0XE0:
					out = (dword_cast(bytes[idx++]) & 0X0F) << 12;
					out |= (dword_cast(bytes[idx++]) & 0X3F) << 6;
					out |= dword_cast(bytes[idx++]) & 0X3F;
					break;
				case 0XF0:
					out = (dword_cast(bytes[idx++]) & 0X0F) << 18;
					out |=(dword_cast(bytes[idx++]) & 0X3F) << 12;
					out |= (dword_cast(bytes[idx++]) & 0X3F) << 6;
					out |= dword_cast(bytes[idx++]) & 0X3F;
					break;
				default: out = dword_cast(bytes[idx++]); break;
				}
				return (T)out;
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(T* dest, windex& i, const byte* src, windex& j) {
				dest[i++] = swap_endian<SWAP1>(convert<false>(src, j));
			}
		};

		template<typename T>
		struct converter<T, char16_t> {
			template<bool SWAP> static inline windex size(const char16_t* words, windex& idx) {
				if ((dword_cast(swap_endian<SWAP>(words[idx])) & 0XDC00) == 0XDC00)
					idx += 2;
				else
					idx++;
				return 1;
			}
			template<bool SWAP> static inline T convert(const char16_t* words, windex& idx) {
				dword out;
				if ((dword_cast(swap_endian<SWAP>(words[idx])) & 0XDC00) == 0XDC00) {
					out = dword_cast(swap_endian<SWAP>(words[idx++])) & 0X03FF;
					out |= (dword_cast(swap_endian<SWAP>(words[idx++])) & 0X03FF) << 10;
					out += 0X10000;
				}
				else {
					out = dword_cast(swap_endian<SWAP>(words[idx++]));
				}
				return (T)out;
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(T* dest, windex& i, const char16_t* src, windex& j) {
				dest[i++] = (T)dword_cast(swap_endian<SWAP1>(convert<SWAP2>(src, j)));
			}
		};


		template<typename T>
		struct converter<byte, T> {
			template<bool SWAP> static inline windex size(const T* src, windex& idx) {
				dword n = dword_cast(swap_endian<SWAP>(src[idx]));
				if (n <= 0X7F) {
					idx++; return 1;
				}
				else if (n <= 0X7FF) {
					idx++; return 2;
				}
				else if (n <= 0XFFFF) {
					idx++; return 3;
				}
				else {
					idx++; return 4;
				}
			}

			template<bool SWAP1, bool SWAP2> static inline void convert(byte* dest, windex& i, const T* src, windex& j) {
				if (dword_cast(src[j]) <= 0X7F) {
					dest[i++] = (byte)dword_cast(src[j++]);
				}
				else if (dword_cast(src[j]) <= 0X7FF) {
					dest[i++] = 0XC0 | (0X1F & (dword_cast(src[j]) >> 6));
					dest[i++] = 0X80 | (0X3F & dword_cast(src[j++]));
				}
				else if (dword_cast(src[j]) <= 0XFFFF) {
					dest[i++] = 0XE0 | (0X0F & (dword_cast(src[j]) >> 12));
					dest[i++] = 0X80 | (0X3F & (dword_cast(src[j]) >> 6));
					dest[i++] = 0X80 | (0X3F & dword_cast(src[j++]));
				}
				else {
					dest[i++] = 0XF0 | (0X07 & (dword_cast(src[j]) >> 18));
					dest[i++] = 0X80 | (0X3F & (dword_cast(src[j]) >> 12));
					dest[i++] = 0X80 | (0X3F & (dword_cast(src[j]) >> 6));
					dest[i++] = 0X80 | (0X3F & dword_cast(src[j++]));
				}
			}
		};

		template<typename T>
		struct converter<char16_t, T> {
			template<bool SWAP> static inline windex size(const T* src, windex& idx) {
				if (dword_cast(swap_endian<SWAP>(src[idx++])) > 0XFFFF)
					return 2;
				else
					return 1;
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(char16_t* dest, windex& i, const T* src, windex& j) {
				if (dword_cast(swap_endian<SWAP2>(src[j])) > 0XFFFF) {
					dest[i++] = swap_endian<SWAP1, char16_t>( 0XDC00 | (dword_cast(swap_endian<SWAP2>(src[j])) & 0X02FF));
					dest[i++] = swap_endian<SWAP1, char16_t>( 0XD800 | (((dword_cast(swap_endian<SWAP2>(src[j++])) - 0X10000) >> 10) & 0X02FF));
				}
				else
					dest[i++] = (char16_t)dword_cast(swap_endian<xor_operator<SWAP1, SWAP2>()>(src[j++]));
			}
		};

		template<>
		struct converter<byte, char16_t> {
			template<bool SWAP> static inline windex size(const char16_t* src, windex& idx) {
				if ((dword_cast(swap_endian<SWAP>(src[idx])) & 0XDC00) == 0XDC00) {
					idx += 2; return 4;
				}
				else {
					if (dword_cast(swap_endian<SWAP>(src[idx])) <= 0X7F) {
						idx++; return 1;
					}
					else if (dword_cast(swap_endian<SWAP>(src[idx])) <= 0X7FF) {
						idx++; return 2;
					}
					else {
						idx++; return 3;
					}
				}
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(byte* dest, windex& i, const char16_t* src, windex& j) {
				if ((dword_cast(swap_endian<SWAP2>(src[j])) & 0XDC00) == 0XDC00) {
					byte temp = ((swap_endian<SWAP2>(src[j]) >> 6) & 0X0F) + 1; //110110ww wwzzzzyy (uuuuu = wwww + 1)
					dest[i++] = 0XF0 | (temp >> 2); //11110uuu
					dest[i++] = 0X80 | (0X30 & (temp << 4)) | (0X0F & (swap_endian<SWAP2>(src[j]) >> 2));
					dest[i++] = 0X80 | (0X30 & (swap_endian<SWAP2>(src[j++]) << 4)) | (0X0F & (swap_endian<SWAP2>(src[j]) >> 6));
					dest[i++] = 0X80 | (0X3F & swap_endian<SWAP2>(src[j++]));
				}
				else {
					if (dword_cast(swap_endian<SWAP2>(src[j])) <= 0X7F) {
						dest[i++] = byte(swap_endian<SWAP2>(src[j++]));
					}
					else if (dword_cast(swap_endian<SWAP2>(src[j])) <= 0X7FF) {
						dest[i++] = 0XC0 | (0X1F & (dword_cast(swap_endian<SWAP2>(src[j])) >> 6));
						dest[i++] = 0X80 | (0X3F & dword_cast(swap_endian<SWAP2>(src[j++])));
					}
					else {
						dest[i++] = 0XE0 | (0X0F & (dword_cast(swap_endian<SWAP2>(src[j])) >> 12));
						dest[i++] = 0X80 | (0X3F & (dword_cast(swap_endian<SWAP2>(src[j])) >> 6));
						dest[i++] = 0X80 | (0X3F & dword_cast(swap_endian<SWAP2>(src[j++])));
					}
				}
			}
		};

		template<>
		struct converter<char16_t, byte> {
			template<bool SWAP> static inline windex size(const byte* src, windex& idx) {
				switch (dword_cast(src[idx]) & 0XF0)
				{
				default: idx++; return 1;
				case 0XD0:
				case 0XC0: idx += 2; return 1;
				case 0XE0: idx += 3; return 1;
				case 0XF0: idx += 4; return 2;
				}
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(char16_t* dest, windex& i, const byte* src, windex& j) {
				char16_t temp;
				switch (dword_cast(src[j]) & 0XF0)
				{
				case 0XD0://0X110x -> 0X1101
				case 0XC0://0X110x -> 0X1100
					dest[i] = swap_endian<SWAP1,char16_t>(((dword_cast(src[j++]) & 0X1F) << 6));
					dest[i++] |= swap_endian<SWAP1, char16_t>((char16_t)(dword_cast(src[j++]) & 0X3F));
					break;
				case 0XE0:
					dest[i] = swap_endian<SWAP1, char16_t>((char16_t)((dword_cast(src[j++]) & 0X0F) << 12));
					dest[i] |= swap_endian<SWAP1, char16_t>((char16_t)((dword_cast(src[j++]) & 0X3F) << 6));
					dest[i++] |= swap_endian<SWAP1, char16_t>((char16_t)(dword_cast(src[j++]) & 0X3F));
					break;
				case 0XF0:
					temp = (char16_t)((dword_cast(src[j++]) & 0X07) << 2);
					temp |= (char16_t)((dword_cast(src[j]) & 0X30) >> 4);
					temp |= (temp - 1) << 6;
					temp |= (char16_t)((dword_cast(src[j++]) & 0X0F) << 2);
					temp |= 0XD800 | (char16_t)((dword_cast(src[j]) & 0X30) >> 4);
					temp |= (char16_t)((dword_cast(src[j++]) & 0X0F) << 6);
					dest[i++] = swap_endian<SWAP1, char16_t>(0XDC00 | (char16_t)(dword_cast(src[j++]) & 0X3F));
					dest[i++] = swap_endian<SWAP1, char16_t>(temp);
					break;
				default:
					dest[i++] = swap_endian<SWAP1, char16_t>((char16_t)dword_cast(src[j++]));
					break;
				}

			}
		};

		template<>
		struct converter<byte, byte> {
			template<bool SWAP> static inline windex size(const byte* src, windex& idx) {
				switch (dword_cast(src[idx]) & 0XF0)
				{
				default: idx++; return 1;
				case 0XD0:
				case 0XC0: idx += 2; return 2;
				case 0XE0: idx += 3; return 3;
				case 0XF0: idx += 4; return 4;
				}
			}
			template<bool SWAP> static inline byte convert(const byte* bytes, windex& idx) {
				return bytes[idx++];
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(byte* dest, windex& i, const byte* src, windex& j) {
				dest[i++] = convert<false>(src, j);
			}
		};

		template<>
		struct converter<char16_t, char16_t> {
			template<bool SWAP> static inline windex size(const char16_t* src, windex& idx) {
				if ((dword_cast(swap_endian<SWAP>(src[idx])) & 0XDC00) == 0XDC00) {
					idx += 2; return 2;
				}
				else {
					idx++; return 1;
				}
			}
			template<bool SWAP> static inline char16_t convert(const char16_t* words, windex& idx) {
				return swap_endian<SWAP>(words[idx++]);
			}
			template<bool SWAP1, bool SWAP2> static inline void convert(char16_t* dest, windex& i, const char16_t* src, windex& j) {
				dest[i++] = swap_endian<xor_operator<SWAP1, SWAP2>()>(src[j++]);
			}
		};
	}
}



