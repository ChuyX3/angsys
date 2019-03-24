#ifndef __ANGSYS_H__
#elif !defined __ISTRING_H__
#define __ISTRING_H__

namespace ang
{
	namespace text
	{
		typedef void* unknown_str_t;
		typedef  void const* unknown_cstr_t;

		//ang_interface(itext_buffer);
		ang_interface(iencoder);
		ang_interface(iparser);
		ang_interface(istring_factory);

		ang_begin_interface(LINK istring_view)
			visible vcall encoding_t encoding()const pure
			visible vcall wsize length()const pure
			visible vcall unknown_str_t data()const pure
			visible vcall rtti_t const& char_type()const pure
			visible vcall raw_str_t str(int = 0)pure
			visible vcall raw_cstr_t cstr(int = 0)const pure
			visible vcall char32_t at(windex i, wsize* = null)const pure
			visible vcall int compare(raw_cstr_t)const pure
			visible vcall windex compare_until(raw_cstr_t)const pure
			visible vcall windex find(raw_cstr_t, windex start = 0, windex end = -1)const pure
			visible vcall windex find_reverse(raw_cstr_t, windex start = -1, windex end = 0)const pure
			visible vcall istring_t sub_string(istring_ptr_t, windex start, windex end)const pure
			visible vcall collections::ienum_ptr<istring_t> split(raw_cstr_t)const pure
		ang_end_interface();

		ang_begin_interface(LINK istring, istring_view)
			visible vcall void clear()pure
			visible vcall void copy(raw_cstr_t)pure
			visible vcall void concat(raw_cstr_t)pure
			visible vcall void insert(windex, raw_cstr_t)pure
			visible vcall void format(raw_cstr_t, var_args_t)pure
			visible vcall void concat_format(raw_cstr_t, var_args_t)pure
			visible vcall void insert_format(windex, raw_cstr_t, var_args_t)pure
		ang_end_interface();

		ang_begin_interface(LINK istring_factory)
			visible scall istring_factory_t get_factory(encoding_t);
			visible vcall encoding_t encoding()const pure
			visible vcall istring_t create_string(raw_cstr_t = raw_cstr_t())const pure
			visible vcall istring_t create_wrapper(ibuffer_t)const pure //ibuffer must be reallocable otherwise it fail
			visible vcall istring_view_t create_wrapper(ibuffer_view_t)const pure
		ang_end_interface();

		ang_begin_interface(LINK iencoder)
			visible scall iencoder_t get_encoder(encoding_t);
			visible vcall encoding_t format()const pure
			visible vcall rtti_t const& char_type()const pure
			visible vcall char32 to_char32(unknown_cstr_t str, windex& i, bool increment = true)const pure
			visible vcall void set_eos(unknown_str_t str, windex at)const pure
			visible vcall wsize lenght(unknown_cstr_t)const pure
			visible vcall wsize size(unknown_cstr_t, encoding_t, windex = 0, windex = -1)const pure
			visible vcall int compare(unknown_cstr_t, unknown_cstr_t, encoding_t)const pure
			visible vcall wsize compare_until(unknown_cstr_t, unknown_cstr_t, encoding_t)const pure
			visible vcall wsize find(raw_cstr_t, raw_cstr_t, windex)const pure
			visible vcall wsize find_any(raw_cstr_t, wsize start, array_view<const char32>)const pure
			visible vcall wsize find_reverse(raw_cstr_t, raw_cstr_t, windex)const pure
			visible vcall raw_str_t convert(raw_str_t, raw_cstr_t, bool set_eos = true)const pure
			visible vcall raw_str_t convert(raw_str_t, wsize& i, raw_cstr_t, wsize& j, bool set_eos = true)const pure
		ang_end_interface();

		ang_begin_interface(LINK iparser)
			visible scall iparser_t get_parser(encoding_t);
			visible vcall encoding_t format()const pure
			visible vcall text_format_t default_format(text_format_target_t)const pure
			visible vcall raw_cstr_t seek(raw_cstr_t, windex& i, raw_cstr_t)const pure
			visible vcall long64 to_signed(raw_cstr_t, windex& i, bool increment = true, int base = 10)const pure
			visible vcall ulong64 to_unsigned(raw_cstr_t, windex& i, bool increment = true, int base = 10)const pure
			visible vcall double to_floating(raw_cstr_t, windex& i, bool increment = true, bool ex = false)const pure
			visible vcall bool format(raw_cstr_t, args_t args, encoding_t e, istring_ptr_t out)const pure
			visible vcall bool format(raw_cstr_t, var_args_t args, encoding_t e, istring_ptr_t out)const pure
			visible vcall text_format_t parse(raw_cstr_t)const pure
			visible vcall text_format_t parse(raw_cstr_t, wsize& beg, int& arg)const pure
			template<typename T> T to_value(raw_cstr_t, windex& i)const;
		ang_end_interface();

		template<typename T, bool = ang::is_unsigned_value<T>::value, bool = ang::is_floating_value<T>::value>
		struct parse_helper {
			static T parse(iparser const* parser, raw_cstr_t cstr, windex& i) {
				return (T)parser->to_signed(cstr, i);
			}
		};

		template<typename T>
		struct parse_helper<T, true, false> {
			static T parse(iparser const* parser, raw_cstr_t cstr, windex& i) {
				return (T)parser->to_unsigned(cstr, i);
			}
		};

		template<typename T>
		struct parse_helper<T, false, true> {
			static T parse(iparser const* parser, raw_cstr_t cstr, windex& i) {
				return (T)parser->to_floating(cstr, i);
			}
		};

		template<typename T> inline T iparser::to_value(raw_cstr_t cstr, windex& i)const {
			return parse_helper<T>::parse(this, cstr, i);
		}
	}

	ANG_INTF_WRAPPER_DECLARATION(LINK, text::iencoder);
	ANG_INTF_WRAPPER_DECLARATION(LINK, text::iparser);
	ANG_INTF_WRAPPER_DECLARATION(LINK, text::istring_factory);

	ANG_BEGIN_INTF_WRAPPER(LINK, text::istring_view)
		operator cstr_t()const;
		char32_t operator [](windex)const;
		inline bool operator == (text::istring_view_t cstr)const { return (cstr_t)*this == (cstr_t)cstr; }
		inline bool operator != (text::istring_view_t cstr)const { return (cstr_t)*this != (cstr_t)cstr; }
		inline bool operator >= (text::istring_view_t cstr)const { return (cstr_t)*this >= (cstr_t)cstr; }
		inline bool operator <= (text::istring_view_t cstr)const { return (cstr_t)*this <= (cstr_t)cstr; }
		inline bool operator > (text::istring_view_t cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
		inline bool operator < (text::istring_view_t cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
	ANG_END_INTF_WRAPPER();

	ANG_BEGIN_INTF_WRAPPER(LINK, text::istring)
		intf_wrapper(cstr_t cstr);
		operator cstr_t ()const;
		operator text::istring_view_t()const;
		char32_t operator [](windex)const;
		inline bool operator == (text::istring_t cstr)const { return (cstr_t)*this == (cstr_t)cstr; }
		inline bool operator != (text::istring_t cstr)const { return (cstr_t)*this != (cstr_t)cstr; }
		inline bool operator >= (text::istring_t cstr)const { return (cstr_t)*this >= (cstr_t)cstr; }
		inline bool operator <= (text::istring_t cstr)const { return (cstr_t)*this <= (cstr_t)cstr; }
		inline bool operator > (text::istring_t cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
		inline bool operator < (text::istring_t cstr)const { return (cstr_t)*this > (cstr_t)cstr; }
	ANG_END_INTF_WRAPPER();

	using text::istring_t;
}


#endif//__ISTRING_H__