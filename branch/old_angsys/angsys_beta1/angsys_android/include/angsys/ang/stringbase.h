/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/ 

#ifndef __ANG_BASE_H__
#error ...
#elif !defined  __ANG_STRINGBASE_H__
#define __ANG_STRINGBASE_H__

namespace ang
{
	namespace text
	{
		class encoding_t;
	}

	namespace strings
	{
		struct LINK algorithms
		{
			static uint string_length(ang_cstr_t cstr, uint max = -1);
			static uint string_length(ang_cwstr_t cstr, uint max = -1);
			static uint string_length(ang_cmstr_t cstr, uint max = -1);

			static uint string_size(ang_cstr_t cstr, text::encoding_t);
			static uint string_size(ang_cwstr_t cstr, text::encoding_t);
			static uint string_size(ang_cmstr_t cstr, text::encoding_t);

			static uint string_copy(ang_str_t str, ang_cstr_t cstr, uint max = -1);
			static uint string_copy(ang_str_t str, ang_cwstr_t cstr, uint max = -1);
			static uint string_copy(ang_str_t str, ang_cmstr_t cstr, uint max = -1);

			static uint string_copy(ang_wstr_t str, ang_cwstr_t cstr, uint max = -1);
			static uint string_copy(ang_wstr_t str, ang_cstr_t cstr, uint max = -1);
			static uint string_copy(ang_wstr_t str, ang_cmstr_t cstr, uint max = -1);

			static uint string_copy(ang_mstr_t str, ang_cmstr_t cstr, uint max = -1);
			static uint string_copy(ang_mstr_t str, ang_cstr_t cstr, uint max = -1);
			static uint string_copy(ang_mstr_t str, ang_cwstr_t cstr, uint max = -1);

			static uint string_expand(ang_str_t str, uint len, index beg, index end, uint max = -1);
			static uint string_expand(ang_mstr_t str, uint len, index beg, index end, uint max = -1);
			static uint string_expand(ang_wstr_t str, uint len, index beg, index end, uint max = -1);

			static uint string_contract(ang_str_t str, uint len, index beg, index end);
			static uint string_contract(ang_mstr_t str, uint len, index beg, index end);
			static uint string_contract(ang_wstr_t str, uint len, index beg, index end);

			static int string_compare(ang_cstr_t first, ang_cstr_t second);
			static int string_compare(ang_cstr_t first, ang_cwstr_t second);
			static int string_compare(ang_cstr_t first, ang_cmstr_t second);

			static int string_compare(ang_cwstr_t first, ang_cwstr_t second);
			static int string_compare(ang_cwstr_t first, ang_cstr_t second);
			static int string_compare(ang_cwstr_t first, ang_cmstr_t second);

			static int string_compare(ang_cmstr_t first, ang_cmstr_t second);
			static int string_compare(ang_cmstr_t first, ang_cstr_t second);
			static int string_compare(ang_cmstr_t first, ang_cwstr_t second);

			static index string_compare_until(ang_cstr_t first, ang_cstr_t second);
			static index string_compare_until(ang_cstr_t first, ang_cwstr_t second);
			static index string_compare_until(ang_cstr_t first, ang_cmstr_t second);

			static index string_compare_until(ang_cwstr_t first, ang_cwstr_t second);
			static index string_compare_until(ang_cwstr_t first, ang_cstr_t second);
			static index string_compare_until(ang_cwstr_t first, ang_cmstr_t second);

			static index string_compare_until(ang_cmstr_t first, ang_cmstr_t second);
			static index string_compare_until(ang_cmstr_t first, ang_cstr_t second);
			static index string_compare_until(ang_cmstr_t first, ang_cwstr_t second);

		};
	}

	namespace text
	{
		class text_format;
		struct itext_buffer;
		template<wsize SIZE = sizeof(wchar)>struct native_encoding_selection;

		/******************************************************************/
		/* enum ang::text::encoding :                                     */
		/*  -> represents the text encoding                               */
		/******************************************************************/
		ANG_BEGIN_ENUM(LINK, encoding, uint)
			unknown = 0,
			auto_detect = 0,
			ascii,//native char
			unicode, //native wchar_t
			utf_8,
			utf_16,
			utf_16_le,
			utf_16_be,
			utf_32,
			utf_32_le,
			utf_32_be,
		ANG_END_ENUM(encoding);

		template<uint ENCODING>
		struct char_type_by_encoding;

		template<> 	struct char_type_by_encoding<encoding::ascii>
		{
			typedef char char_t;
			typedef char* str_t;
			typedef char const* cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::utf_8>
		{
			typedef mchar char_t;
			typedef mchar* str_t;
			typedef mchar const* cstr_t;
		};

		template<> 	struct char_type_by_encoding<encoding::unicode>
		{
			typedef wchar char_t;
			typedef wchar* str_t;
			typedef wchar const* cstr_t;
		};

	}
}


template<> inline bool ang::value_compare_same<ang_cstr_t, ang_cstr_t>(ang_cstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cstr_t, ang_cstr_t>(ang_cstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cstr_t, ang_cstr_t>(ang_cstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cstr_t, ang_cstr_t>(ang_cstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cstr_t, ang_cstr_t>(ang_cstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cstr_t, ang_cstr_t>(ang_cstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_str_t, ang_cstr_t>(ang_str_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_str_t, ang_cstr_t>(ang_str_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_str_t, ang_cstr_t>(ang_str_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_str_t, ang_cstr_t>(ang_str_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_str_t, ang_cstr_t>(ang_str_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_str_t, ang_cstr_t>(ang_str_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cstr_t, ang_str_t>(ang_cstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cstr_t, ang_str_t>(ang_cstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cstr_t, ang_str_t>(ang_cstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cstr_t, ang_str_t>(ang_cstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cstr_t, ang_str_t>(ang_cstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cstr_t, ang_str_t>(ang_cstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cwstr_t, ang_cwstr_t>(ang_cwstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cwstr_t, ang_cwstr_t>(ang_cwstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cwstr_t, ang_cwstr_t>(ang_cwstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cwstr_t, ang_cwstr_t>(ang_cwstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cwstr_t, ang_cwstr_t>(ang_cwstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cwstr_t, ang_cwstr_t>(ang_cwstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_wstr_t, ang_cwstr_t>(ang_wstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_wstr_t, ang_cwstr_t>(ang_wstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_wstr_t, ang_cwstr_t>(ang_wstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_wstr_t, ang_cwstr_t>(ang_wstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_wstr_t, ang_cwstr_t>(ang_wstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_wstr_t, ang_cwstr_t>(ang_wstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cwstr_t, ang_wstr_t>(ang_cwstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cwstr_t, ang_wstr_t>(ang_cwstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cwstr_t, ang_wstr_t>(ang_cwstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cwstr_t, ang_wstr_t>(ang_cwstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cwstr_t, ang_wstr_t>(ang_cwstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cwstr_t, ang_wstr_t>(ang_cwstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cmstr_t, ang_cmstr_t>(ang_cmstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cmstr_t, ang_cmstr_t>(ang_cmstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cmstr_t, ang_cmstr_t>(ang_cmstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cmstr_t, ang_cmstr_t>(ang_cmstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cmstr_t, ang_cmstr_t>(ang_cmstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cmstr_t, ang_cmstr_t>(ang_cmstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_mstr_t, ang_cmstr_t>(ang_mstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_mstr_t, ang_cmstr_t>(ang_mstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_mstr_t, ang_cmstr_t>(ang_mstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_mstr_t, ang_cmstr_t>(ang_mstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_mstr_t, ang_cmstr_t>(ang_mstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_mstr_t, ang_cmstr_t>(ang_mstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cmstr_t, ang_mstr_t>(ang_cmstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cmstr_t, ang_mstr_t>(ang_cmstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cmstr_t, ang_mstr_t>(ang_cmstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cmstr_t, ang_mstr_t>(ang_cmstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cmstr_t, ang_mstr_t>(ang_cmstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cmstr_t, ang_mstr_t>(ang_cmstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cstr_t, ang_cwstr_t>(ang_cstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cstr_t, ang_cwstr_t>(ang_cstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cstr_t, ang_cwstr_t>(ang_cstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cstr_t, ang_cwstr_t>(ang_cstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cstr_t, ang_cwstr_t>(ang_cstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cstr_t, ang_cwstr_t>(ang_cstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cstr_t, ang_cmstr_t>(ang_cstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cstr_t, ang_cmstr_t>(ang_cstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cstr_t, ang_cmstr_t>(ang_cstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cstr_t, ang_cmstr_t>(ang_cstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cstr_t, ang_cmstr_t>(ang_cstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cstr_t, ang_cmstr_t>(ang_cstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_str_t, ang_cwstr_t>(ang_str_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_str_t, ang_cwstr_t>(ang_str_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_str_t, ang_cwstr_t>(ang_str_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_str_t, ang_cwstr_t>(ang_str_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_str_t, ang_cwstr_t>(ang_str_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_str_t, ang_cwstr_t>(ang_str_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_str_t, ang_cmstr_t>(ang_str_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_str_t, ang_cmstr_t>(ang_str_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_str_t, ang_cmstr_t>(ang_str_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_str_t, ang_cmstr_t>(ang_str_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_str_t, ang_cmstr_t>(ang_str_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_str_t, ang_cmstr_t>(ang_str_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cstr_t, ang_wstr_t>(ang_cstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cstr_t, ang_wstr_t>(ang_cstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cstr_t, ang_wstr_t>(ang_cstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cstr_t, ang_wstr_t>(ang_cstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cstr_t, ang_wstr_t>(ang_cstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cstr_t, ang_wstr_t>(ang_cstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cstr_t, ang_mstr_t>(ang_cstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cstr_t, ang_mstr_t>(ang_cstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cstr_t, ang_mstr_t>(ang_cstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cstr_t, ang_mstr_t>(ang_cstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cstr_t, ang_mstr_t>(ang_cstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cstr_t, ang_mstr_t>(ang_cstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cwstr_t, ang_cstr_t>(ang_cwstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cwstr_t, ang_cstr_t>(ang_cwstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cwstr_t, ang_cstr_t>(ang_cwstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cwstr_t, ang_cstr_t>(ang_cwstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cwstr_t, ang_cstr_t>(ang_cwstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cwstr_t, ang_cstr_t>(ang_cwstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cwstr_t, ang_cmstr_t>(ang_cwstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cwstr_t, ang_cmstr_t>(ang_cwstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cwstr_t, ang_cmstr_t>(ang_cwstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cwstr_t, ang_cmstr_t>(ang_cwstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cwstr_t, ang_cmstr_t>(ang_cwstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cwstr_t, ang_cmstr_t>(ang_cwstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_wstr_t, ang_cstr_t>(ang_wstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_wstr_t, ang_cstr_t>(ang_wstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_wstr_t, ang_cstr_t>(ang_wstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_wstr_t, ang_cstr_t>(ang_wstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_wstr_t, ang_cstr_t>(ang_wstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_wstr_t, ang_cstr_t>(ang_wstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_wstr_t, ang_cmstr_t>(ang_wstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_wstr_t, ang_cmstr_t>(ang_wstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_wstr_t, ang_cmstr_t>(ang_wstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_wstr_t, ang_cmstr_t>(ang_wstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_wstr_t, ang_cmstr_t>(ang_wstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_wstr_t, ang_cmstr_t>(ang_wstr_t const& arg1, ang_cmstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cwstr_t, ang_str_t>(ang_cwstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cwstr_t, ang_str_t>(ang_cwstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cwstr_t, ang_str_t>(ang_cwstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cwstr_t, ang_str_t>(ang_cwstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cwstr_t, ang_str_t>(ang_cwstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cwstr_t, ang_str_t>(ang_cwstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cwstr_t, ang_mstr_t>(ang_cwstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cwstr_t, ang_mstr_t>(ang_cwstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cwstr_t, ang_mstr_t>(ang_cwstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cwstr_t, ang_mstr_t>(ang_cwstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cwstr_t, ang_mstr_t>(ang_cwstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cwstr_t, ang_mstr_t>(ang_cwstr_t const& arg1, ang_mstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }



template<> inline bool ang::value_compare_same<ang_cmstr_t, ang_cstr_t>(ang_cmstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cmstr_t, ang_cstr_t>(ang_cmstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cmstr_t, ang_cstr_t>(ang_cmstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cmstr_t, ang_cstr_t>(ang_cmstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cmstr_t, ang_cstr_t>(ang_cmstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cmstr_t, ang_cstr_t>(ang_cmstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cmstr_t, ang_cwstr_t>(ang_cmstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cmstr_t, ang_cwstr_t>(ang_cmstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cmstr_t, ang_cwstr_t>(ang_cmstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cmstr_t, ang_cwstr_t>(ang_cmstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cmstr_t, ang_cwstr_t>(ang_cmstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cmstr_t, ang_cwstr_t>(ang_cmstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_mstr_t, ang_cstr_t>(ang_mstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_mstr_t, ang_cstr_t>(ang_mstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_mstr_t, ang_cstr_t>(ang_mstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_mstr_t, ang_cstr_t>(ang_mstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_mstr_t, ang_cstr_t>(ang_mstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_mstr_t, ang_cstr_t>(ang_mstr_t const& arg1, ang_cstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_mstr_t, ang_cwstr_t>(ang_mstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_mstr_t, ang_cwstr_t>(ang_mstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_mstr_t, ang_cwstr_t>(ang_mstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_mstr_t, ang_cwstr_t>(ang_mstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_mstr_t, ang_cwstr_t>(ang_mstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_mstr_t, ang_cwstr_t>(ang_mstr_t const& arg1, ang_cwstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cmstr_t, ang_str_t>(ang_cmstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cmstr_t, ang_str_t>(ang_cmstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cmstr_t, ang_str_t>(ang_cmstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cmstr_t, ang_str_t>(ang_cmstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cmstr_t, ang_str_t>(ang_cmstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cmstr_t, ang_str_t>(ang_cmstr_t const& arg1, ang_str_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }

template<> inline bool ang::value_compare_same<ang_cmstr_t, ang_wstr_t>(ang_cmstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) == 0; }
template<> inline bool ang::value_compare_dif<ang_cmstr_t, ang_wstr_t>(ang_cmstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) != 0; }
template<> inline bool ang::value_compare_same_ma<ang_cmstr_t, ang_wstr_t>(ang_cmstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) >= 0; }
template<> inline bool ang::value_compare_same_mi<ang_cmstr_t, ang_wstr_t>(ang_cmstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) <= 0; }
template<> inline bool ang::value_compare_ma<ang_cmstr_t, ang_wstr_t>(ang_cmstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) > 0; }
template<> inline bool ang::value_compare_mi<ang_cmstr_t, ang_wstr_t>(ang_cmstr_t const& arg1, ang_wstr_t const& arg2) { return ang::strings::algorithms::string_compare(arg1, arg2) < 0; }






#endif//__ANG_STRINGBASE_H__