#pragma once

#include "format_parser.h"

namespace ang
{
	extern char hexl[18];
	extern char hexu[18];
}

template<typename T, bool SWAP>
static ang::safe_str<const T> ang_integer_to_string(long64 value, ang::text::text_format_t f, ang::array_view<T> out)
{
	static const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != ang::text::text_format::signed_integer)
	{
		out[0] = 0;
		return{ out.get(), 0 };
	}

	T fill = ang::text::swap_endian<SWAP, T>(GET_FLAG_FILL(format));
	int max = GET_FLAG_MAX(format);
	bool neg = false;
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = ang::text::swap_endian<SWAP, T>('0');
	}

	if ((FLAG_SIGNED_BIN & format) == FLAG_SIGNED_BIN)
	{
		if (value == 0)
		{
			out[c--] = ang::text::swap_endian<SWAP, T>('0');
		}
		else do
		{
			if (value != 0)
				out[c--] = ang::text::swap_endian<SWAP, T>((0X0001 & value) ? '1' : '0');
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
		if ((FLAG_SIGNED_HEX_U & format) == FLAG_SIGNED_HEX_U)
		{
			if (value == 0)
			{
				out[c--] = ang::text::swap_endian<SWAP, T>('0');
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = ang::text::swap_endian<SWAP, T>(ang::hexu[(byte)0X000F & value]);
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
		{
			if (value == 0)
			{
				out[c--] = ang::text::swap_endian<SWAP, T>('0');
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = ang::text::swap_endian<SWAP, T>(ang::hexl[(byte)0X000F & value]);
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
				out[c--] = ang::text::swap_endian<SWAP, T>('0');
			}
			else do
			{
				if (value != 0)
				{
					out[c--] = ang::text::swap_endian<SWAP, T>('0' + value % 10);
					value /= 10;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0 || max > 0);
		}
		if (neg)
			out[c--] = ang::text::swap_endian<SWAP, T>('-');
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = ang::text::swap_endian<SWAP, T>('+');
	}

	return ang::safe_str<const T>(&out[c + 1u], max_char - c);
}

template<typename T, bool SWAP>
static ang::safe_str<const T> ang_uinteger_to_string(ulong64 value, ang::text::text_format_t f, ang::array_view<T> out)
{
	static const uint max_char = 65;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != ang::text::text_format::signed_integer)
	{
		out[0] = 0;
		return{ out.get(), 0 };
	}

	T fill = ang::text::swap_endian<SWAP, T>(GET_FLAG_FILL(format));
	int max = GET_FLAG_MAX(format);
	uint c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = ang::text::swap_endian<SWAP, T>('0');
	}

	if ((FLAG_UNSIGNED_BIN & format) == FLAG_UNSIGNED_BIN)
	{
		if (value == 0U)
		{
			out[c--] = ang::text::swap_endian<SWAP, T>('0');
		}
		else do
		{
			if (value != 0U)
				out[c--] = ang::text::swap_endian<SWAP, T>((0X0001 & value) ? '1' : '0');
			else
				out[c--] = fill;
			value = value >> 1;
			max--;
		} while (value != 0U || max > 0);
	}
	else
	{
		if ((FLAG_UNSIGNED_HEX_U & format) == FLAG_UNSIGNED_HEX_U)
		{
			if (value == 0U)
			{
				out[c--] = ang::text::swap_endian<SWAP, T>('0');
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = ang::text::swap_endian<SWAP, T>(ang::hexu[(byte)0X000F & value]);
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
		else if ((FLAG_UNSIGNED_HEX_L & format) == FLAG_UNSIGNED_HEX_L)
		{
			if (value == 0U)
			{
				out[c--] = ang::text::swap_endian<SWAP, T>('0');
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = ang::text::swap_endian<SWAP, T>(ang::hexl[(byte)0X000F & value]);
					value = value >> 4;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
		else
		{
			if (value == 0U)
			{
				out[c--] = ang::text::swap_endian<SWAP, T>('0');
			}
			else do
			{
				if (value != 0U)
				{
					out[c--] = ang::text::swap_endian<SWAP, T>('0' + value % 10);
					value /= 10U;
				}
				else
					out[c--] = fill;
				max--;
			} while (value > 0U || max > 0);
		}
	}
	return  ang::safe_str<const T>(&out[c + 1], max_char - c);
}

template<typename T, bool SWAP>
static ang::safe_str<const T> ang_floating_to_string(double value, ang::text::text_format_t f, ang::array_view<T> out)
{
	const byte max_char = 120;

	qword format = f.format_flags();
	if (GET_FLAG_TARGET(format) != ang::text::text_format::floating)
	{
		out[0] = 0;
		return{ out.get(), 0 };
	}

	//char out[max_char] = { 0 };
	T fill = ang::text::swap_endian<SWAP, T>(GET_FLAG_FILL(format));
	int max = GET_FLAG_MAX(format) >> 4;
	int pres = 0X0F & GET_FLAG_MAX(format);
	bool exp = (format & FLAG_FLOATING_EXP) == FLAG_FLOATING_EXP;
	word c = max_char - 2;

	if (max == 0 || max > max_char)
		max = max_char - 1;

	if (fill == 0)
	{
		max = 0;
		fill = ang::text::swap_endian<SWAP, T>('0');
	}

	union
	{
		qword int_value;
		double float_value;
	}union_value;
	union_value.float_value = value;
	if ((FLAG_FLOATING_BIN & format) == FLAG_FLOATING_BIN)
	{
		do
		{
			if (union_value.int_value != 0)
				out[c--] = ang::text::swap_endian<SWAP, T>((0X0001 & union_value.int_value) ? '1' : '0');
			else
				out[c--] = fill;
			union_value.int_value = union_value.int_value >> 1;
			max--;
		} while (union_value.int_value != 0 || max > 0);
	}
	if ((FLAG_FLOATING_HEX_U & format) == FLAG_FLOATING_HEX_U)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = ang::text::swap_endian<SWAP, T>(ang::hexu[(byte)(0X000F & union_value.int_value)]);
				union_value.int_value = union_value.int_value >> 4;
			}
			else
				out[c--] = fill;
			max--;
		} while (union_value.int_value > 0 || max > 0);
	}
	else if ((FLAG_SIGNED_HEX_L & format) == FLAG_SIGNED_HEX_L)
	{
		do
		{
			if (union_value.int_value != 0)
			{
				out[c--] = ang::text::swap_endian<SWAP, T>(ang::hexl[(byte)(0X000F & union_value.int_value)]);
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
			value *= -1.0;
		}
		if (exp)
		{
			bool bexp = false;
			if (value > 10.0)
			{
				bexp = false;
				do {
					iexp++;
					value /= 10.0;
				} while (value > 10.0);
			}
			else if (value < 1.0f)
			{
				bexp = true;
				do {
					iexp++;
					value *= 10.0;
				} while (value < 1.0);
			}
			do
			{
				out[c--] = ang::text::swap_endian<SWAP, T>('0' + iexp % 10);
				iexp /= 10;
			} while (iexp > 0);
			if (bexp)out[c--] = ang::text::swap_endian<SWAP, T>('-');
			out[c--] = ang::text::swap_endian<SWAP, T>('e');
		}

		int_value = (long64)value;
		value = value - double(int_value);

		if (pres > 0)
		{
			c -= pres;
			for (word i = 0; i < pres; i++)
			{
				out[i + c + 1] = ang::text::swap_endian<SWAP, T>('0' + byte(value * 10.0));
				value *= 10.0;
				value -= double(long64(value));
			}
			out[c--] = ang::text::swap_endian<SWAP, T>('.');
		}

		do
		{
			if (int_value != 0)
			{
				out[c--] = ang::text::swap_endian<SWAP, T>('0' + int_value % 10);
				int_value /= 10;
			}
			else
				out[c--] = fill;
			max--;
		} while (int_value > 0 || max > 0);

		if (neg)
			out[c--] = ang::text::swap_endian<SWAP, T>('-');
		else if ((FLAG_SIGNED_SIG & format) == FLAG_SIGNED_SIG)
			out[c--] = ang::text::swap_endian<SWAP, T>('+');
	}

	return ang::safe_str<const T>(&out[c + 1], max_char - c);
}


//
//
//ang::cstr_t value_to_string<char, true>::integer_to_string(long64 value, text::text_format_t f, array_view<char> out) { return ang_integer_to_string<char, true>(value, f, out); }
//ang::cstr_t value_to_string<char, true>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<char> out) { return ang_uinteger_to_string<char, true>(value, f, out); }
//ang::cstr_t value_to_string<char, true>::floating_to_string(double value, text::text_format_t f, array_view<char> out) { return ang_floating_to_string<char, true>(value, f, out); }
//ang::cstr_t value_to_string<char, false>::integer_to_string(long64 value, text::text_format_t f, array_view<char> out) { return ang_integer_to_string<char, false>(value, f, out); }
//ang::cstr_t value_to_string<char, false>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<char> out) { return ang_uinteger_to_string<char, false>(value, f, out); }
//ang::cstr_t value_to_string<char, false>::floating_to_string(double value, text::text_format_t f, array_view<char> out) { return ang_floating_to_string<char, false>(value, f, out); }
//
//ang::cmstr_t value_to_string<mchar, true>::integer_to_string(long64 value, text::text_format_t f, array_view<mchar> out) { return ang_integer_to_string<mchar, true>(value, f, out); }
//cmstr_t value_to_string<mchar, true>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<mchar> out) { return ang_uinteger_to_string<mchar, true>(value, f, out); }
//ang::cmstr_t value_to_string<mchar, true>::floating_to_string(double value, text::text_format_t f, array_view<mchar> out) { return ang_floating_to_string<mchar, true>(value, f, out); }
//cmstr_t value_to_string<mchar, false>::integer_to_string(long64 value, text::text_format_t f, array_view<mchar> out) { return ang_integer_to_string<mchar, false>(value, f, out); }
//cmstr_t value_to_string<mchar, false>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<mchar> out) { return ang_uinteger_to_string<mchar, false>(value, f, out); }
//cmstr_t value_to_string<mchar, false>::floating_to_string(double value, text::text_format_t f, array_view<mchar> out) { return ang_floating_to_string<mchar, false>(value, f, out); }
//
//cwstr_t value_to_string<wchar, true>::integer_to_string(long64 value, text::text_format_t f, array_view<wchar> out) { return ang_integer_to_string<wchar, true>(value, f, out); }
//cwstr_t value_to_string<wchar, true>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<wchar> out) { return ang_uinteger_to_string<wchar, true>(value, f, out); }
//cwstr_t value_to_string<wchar, true>::floating_to_string(double value, text::text_format_t f, array_view<wchar> out) { return ang_floating_to_string<wchar, true>(value, f, out); }
//cwstr_t value_to_string<wchar, false>::integer_to_string(long64 value, text::text_format_t f, array_view<wchar> out) { return ang_integer_to_string<wchar, false>(value, f, out); }
//cwstr_t value_to_string<wchar, false>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<wchar> out) { return ang_uinteger_to_string<wchar, false>(value, f, out); }
//cwstr_t value_to_string<wchar, false>::floating_to_string(double value, text::text_format_t f, array_view<wchar> out) { return ang_floating_to_string<wchar, false>(value, f, out); }
//
//cu16str_t value_to_string<char16_t, true>::integer_to_string(long64 value, text::text_format_t f, array_view<char16_t> out) { return ang_integer_to_string<char16_t, true>(value, f, out); }
//cu16str_t value_to_string<char16_t, true>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<char16_t> out) { return ang_uinteger_to_string<char16_t, true>(value, f, out); }
//cu16str_t value_to_string<char16_t, true>::floating_to_string(double value, text::text_format_t f, array_view<char16_t> out) { return ang_floating_to_string<char16_t, true>(value, f, out); }
//cu16str_t value_to_string<char16_t, false>::integer_to_string(long64 value, text::text_format_t f, array_view<char16_t> out) { return ang_integer_to_string<char16_t, false>(value, f, out); }
//cu16str_t value_to_string<char16_t, false>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<char16_t> out) { return ang_uinteger_to_string<char16_t, false>(value, f, out); }
//cu16str_t value_to_string<char16_t, false>::floating_to_string(double value, text::text_format_t f, array_view<char16_t> out) { return ang_floating_to_string<char16_t, false>(value, f, out); }
//
//cu32str_t value_to_string<char32_t, true>::integer_to_string(long64 value, text::text_format_t f, array_view<char32_t> out) { return ang_integer_to_string<char32_t, true>(value, f, out); }
//cu32str_t value_to_string<char32_t, true>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<char32_t> out) { return ang_uinteger_to_string<char32_t, true>(value, f, out); }
//cu32str_t value_to_string<char32_t, true>::floating_to_string(double value, text::text_format_t f, array_view<char32_t> out) { return ang_floating_to_string<char32_t, true>(value, f, out); }
//cu32str_t value_to_string<char32_t, false>::integer_to_string(long64 value, text::text_format_t f, array_view<char32_t> out) { return ang_integer_to_string<char32_t, false>(value, f, out); }
//cu32str_t value_to_string<char32_t, false>::uinteger_to_string(ulong64 value, text::text_format_t f, array_view<char32_t> out) { return ang_uinteger_to_string<char32_t, false>(value, f, out); }
//cu32str_t value_to_string<char32_t, false>::floating_to_string(double value, text::text_format_t f, array_view<char32_t> out) { return ang_floating_to_string<char32_t, false>(value, f, out); }
//
