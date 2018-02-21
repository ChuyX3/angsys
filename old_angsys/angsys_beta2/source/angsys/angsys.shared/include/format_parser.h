#ifndef __FORMATPARSER_H__
#define __FORMATPARSER_H__

#define FLAG_INVALID_FORMAT 0

#define SET_FLAG_TARGET(out,target) (out = (out & 0XFFFFFFFFFFFFFF00) | (0X00000000000000FF & ((qword)target)))
#define GET_FLAG_TARGET(flag) ((ang::text::text_format_t::target)(0X00000000000000FF & flag))

#define GET_FLAG_FILL(flag) ((char)((0X0000000000FF0000&(flag))>>16))
#define GET_FLAG_MAX(flag)	((uint)((0X00000000FF000000&(flag))>>24))


#define FLAG_CHAR_LOWERCASE 0X0000000000000100
#define FLAG_CHAR_UPPERCASE 0X0000000000000200

#define FLAG_SIGNED_HEX_U 0X0000000000000100
#define FLAG_SIGNED_HEX_L 0X0000000000000200
#define FLAG_SIGNED_BIN 0X0000000000000300
#define FLAG_SIGNED_SIG 0X0000000000000400
#define FLAG_SIGNED_FILL(ch) ((qword)((wchar)ch)<<16)
#define FLAG_SIGNED_MAX(num) ((qword)((byte)num)<<24)

#define FLAG_UNSIGNED_HEX_U 0X0000000000000100
#define FLAG_UNSIGNED_HEX_L 0X0000000000000200
#define FLAG_UNSIGNED_BIN 0X0000000000000300
#define FLAG_UNSIGNED_FILL(ch) ((qword)((wchar)ch)<<16)
#define FLAG_UNSIGNED_MAX(num) ((qword)((byte)num)<<24)

#define FLAG_FLOATING_HEX_U 0X0000000000000100
#define FLAG_FLOATING_HEX_L 0X0000000000000200
#define FLAG_FLOATING_BIN 0X0000000000000300
#define FLAG_FLOATING_SIG 0X0000000000000400
#define FLAG_FLOATING_EXP 0X0000000000000800
#define FLAG_FLOATING_FILL(ch) ((qword)((wchar)ch)<<16)
#define FLAG_FLOATING_MAX(num) ((qword)((byte)num)<<24)

namespace ang
{
	class format_parser
	{
		static bool parse_char(cstr_t format, wsize& beg, qword& flag);
		static bool parse_string(cstr_t format, wsize& beg, qword& flag);
		static bool parse_signed(cstr_t format, wsize& beg, qword& flag);
		static bool parse_unsigned(cstr_t format, wsize& beg, qword& flag);
		static bool parse_floating(cstr_t format, wsize& beg, qword& flag);

	public:
		static qword parse(cstr_t f);
		static bool format(qword nFlag, string& out);
	};
}

#endif