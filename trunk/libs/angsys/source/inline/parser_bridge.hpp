
#include "angsys.h"

namespace ang
{
	namespace text {

		template<typename EP = typename value_pack_from_range<encoding, encoding::ascii, encoding::utf32_se>::type> struct parser_bridge;
	
		template<encoding E, typename EP = typename value_pack_from_range<encoding, encoding::ascii, encoding::utf32_se>::type> struct parser_bridge2;

		template<encoding E, encoding...ES>
		struct parser_bridge2<E, value_pack<encoding, ES...>>
		{
			using char_type = typename char_type_by_encoding<E>::char_type;
			using cstr_type = str_view<const char_type, E>;
			using seek_type = cstr_t(*)(cstr_type const&, windex&, cstr_t const&);
			using format_type = cstr_t(*)(cstr_type const&, var_args_t&&, istring_ptr_t);

			using pack_type = value_pack<encoding, ES...>;
			static constexpr encoding FIRST = pack_type::get<0>();
			static constexpr wsize PACK_SIZE = value_pack_size<pack_type>::value;

			static cstr_t seek(cstr_type source, windex& i, cstr_t value)
			{
				wsize idx = get_index(value.encoding());
				return idx >= PACK_SIZE ? cstr_t() : s_seek_array[idx](ang::forward<cstr_type>(source), i, ang::forward<cstr_t>(value));
			}
			static cstr_t format(cstr_type const& cstr, var_args_t&& args, encoding_t e, istring_ptr_t out) {
				wsize idx = get_index(e);
				return idx >= PACK_SIZE ? cstr_t() : s_format_array[idx](cstr, ang::forward<var_args_t>(args), out);
			}
		private:
			static wsize get_index(encoding E) { return wsize(E) - wsize(FIRST); }
			template<encoding E2>
			static cstr_t seek(cstr_type const& source, windex& i, cstr_t const& value)
			{
				cstr_type src = source + i;
				src = text_parser<E>::seek(src, value.cstr<E2>());
				i = (wsize(src.cstr()) - wsize(source.cstr())) / size_of<char_type>();
				return src;
			}
			template<encoding E2>
			static cstr_t format(cstr_type const& cstr, var_args_t&& args, istring_ptr_t out)
			{
				int a;
				basic_string<E2> str = new basic_string_buffer<E2>();
				fast_string<E2>& stream = *str;
				text::text_format_flags_t f;
				wsize i = 0, t = 0, n = 0, l = 0;
				while (char32_t c = text::to_char32<false, false>(cstr, i))
				{
					if (c == U'{') {
						f.value = text_parser<E>::parse(cstr, n, a);
						if (a > -1 && a < (long64)args.size()) {
							stream.append(cstr_type(&cstr[l], t - l));
							stream.append(f.value ? args[a].to_string(f) : args[a].to_string());
							i = l = n;
						}
					}
					t = n = i;
				}
				stream.append(cstr_type(&cstr[l], i - l));
				if (!out.is_empty())
					*out = str.get();
				return str->cstr();
			}
	
			static constexpr seek_type s_seek_array[PACK_SIZE] = { &seek<ES>... };
			static constexpr format_type s_format_array[PACK_SIZE] = { &format<ES>... };
		};

		template<encoding...ES>
		struct parser_bridge<value_pack<encoding, ES...>>
		{
			using seek_type = cstr_t(*)(cstr_t, windex&, cstr_t);
			using to_signed_type = long64(*)(cstr_t, windex&, bool, int);
			using to_unsigned_type = ulong64(*)(cstr_t, windex&, bool, int);
			using to_floating_type = double (*)(cstr_t, windex&, bool, bool);
			using format_type = cstr_t(*)(cstr_t, var_args_t, encoding_t, istring_ptr_t);
			using parse1_type = text_format_t(*)(cstr_t);
			using parse2_type = text_format_t(*)(cstr_t, wsize&, int&);

			using pack_type = value_pack<encoding, ES...>;
			static constexpr encoding FIRST = pack_type::get<0>();
			static constexpr wsize PACK_SIZE = value_pack_size<pack_type>::value;

			static cstr_t seek(cstr_t cstr1, windex& i, cstr_t cstr2) {
				wsize idx = get_index(cstr1.encoding());
				return idx < PACK_SIZE ? s_seek_array[idx](cstr1, i, cstr2) : cstr_t();
			}
			static long64 to_signed(cstr_t cstr, windex& i, bool increment, int base) {
				wsize idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_to_signed_array[idx](cstr, i, increment, base) : 0;
			}
			static ulong64 to_unsigned(cstr_t cstr, windex& i, bool increment, int base) {
				wsize idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_to_unsigned_array[idx](cstr, i, increment, base) : 0;
			}
			static double to_floating(cstr_t cstr, windex& i, bool increment, bool ex) {
				wsize idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_to_floating_array[idx](cstr, i, increment, ex) : 0;
			}
			static text_format_t parse(cstr_t cstr) {
				wsize idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_parse1_array[idx](cstr) : text_format_t();
			}
			static text_format_t parse(cstr_t cstr, wsize& beg, int& arg) {
				wsize idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_parse2_array[idx](cstr, beg, arg) : text_format_t();
			}
			static cstr_t format(cstr_t cstr, var_args_t args, encoding_t e, istring_ptr_t out) {
				wsize idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_format_array[idx](cstr, ang::forward<var_args_t>(args), e, out) : cstr_t();
			}
		private:
			static wsize get_index(encoding E) { return wsize(E) - wsize(FIRST); }
			template<encoding E> static cstr_t seek(cstr_t cstr1, windex& i, cstr_t cstr2) {
				return parser_bridge2<E>::seek(cstr1.cstr<E>(), i, cstr2);
			}
			template<encoding E> static long64 to_signed(cstr_t cstr, windex& i, bool increment, int base) {
				wsize _i = i;
				if(increment)
					return text_parser<E>::str_to_signed(cstr.cstr<E>(), i, base);
				else
					return text_parser<E>::str_to_signed(cstr.cstr<E>(), _i, base);

			}
			template<encoding E> static ulong64 to_unsigned(cstr_t cstr, windex& i, bool increment, int base) {
				wsize _i = i;
				if (increment)
					return text_parser<E>::str_to_unsigned(cstr.cstr<E>(), i, base);
				else
					return text_parser<E>::str_to_unsigned(cstr.cstr<E>(), _i, base);
			}
			template<encoding E> static double to_floating(cstr_t cstr, windex& i, bool increment, bool ex) {
				wsize _i = i;
				if (increment)
					return text_parser<E>::str_to_floating(cstr.cstr<E>(), i, ex);
				else
					return text_parser<E>::str_to_floating(cstr.cstr<E>(), _i, ex);
			}
			template<encoding E> static cstr_t format(cstr_t cstr, var_args_t args, encoding_t e, istring_ptr_t out) {
				return parser_bridge2<E>::format(cstr.cstr<E>(), ang::forward<var_args_t>(args), e, out);
			}
			template<encoding E> static text_format_t parse1(cstr_t cstr) {
				return text_parser<E>::parse(cstr.cstr<E>());
			}
			template<encoding E> static text_format_t parse2(cstr_t cstr, wsize& beg, int& arg) {
				return text_parser<E>::parse(cstr.cstr<E>(), beg, arg);
			}

			static constexpr seek_type s_seek_array[PACK_SIZE] = { &seek<ES>... };
			static constexpr to_signed_type s_to_signed_array[PACK_SIZE] = { &to_signed<ES>... };
			static constexpr to_unsigned_type s_to_unsigned_array[PACK_SIZE] = { &to_unsigned<ES>... };
			static constexpr to_floating_type s_to_floating_array[PACK_SIZE] = { &to_floating<ES>... };
			static constexpr format_type s_format_array[PACK_SIZE] = { &format<ES>... };
			static constexpr parse1_type s_parse1_array[PACK_SIZE] = { &parse1<ES>... };
			static constexpr parse2_type s_parse2_array[PACK_SIZE] = { &parse2<ES>... };

		};


	}
}
