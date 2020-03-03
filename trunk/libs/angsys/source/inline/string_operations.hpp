
#include "angsys.h"

namespace ang
{
	namespace text {

		template<encoding E, typename EP = typename value_pack_from_range<encoding, encoding::ascii, encoding::utf32_se>::type> struct string_operations;
	
		template<encoding E, encoding...ES>
		struct string_operations<E, value_pack<encoding, ES...>>
		{
			using type = fast_string<E>;
			using char_type = typename char_type_by_encoding<E>::char_type;
			using ptr_type = typename char_type_by_encoding<E>::str_type;
			using cptr_type = typename char_type_by_encoding<E>::cstr_type;
			using str_type = str_view<char_type, E>;
			using cstr_type = str_view<const char_type, E>;
			using compare_type = int(*)(type const&, cstr_t const&);
			using compare_until_type = wsize(*)(type const&, cstr_t const&);
			using find_type = wsize(*)(type const&, cstr_t const&, windex, windex);
			using copy_type = wsize(*)(type&, cstr_t const&);
			using insert_type = wsize(*)(type&, cstr_t const&, wsize);
			using split_type = vector<string>(*)(type const&, cstr_t const&);
			using format_type = wsize(*)(basic_string_buffer<E>*, cstr_t const&, var_args_t&&);
			using create_string_type = string(*)(cstr_t const&);
			using pack_type = value_pack<encoding, ES...>;
			static constexpr encoding FIRST = pack_type::get<0>();
			static constexpr wsize PACK_SIZE = value_pack_size<pack_type>::value;

			static int compare(type const& str, cstr_t const& cstr) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_compare_array[i](str, cstr) : -10;
			}
			static wsize compare_until(type const& str, cstr_t const& cstr) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_compare_until_array[i](str, cstr) : 0;
			}
			static wsize find(type const& str, cstr_t const& cstr, windex first, windex end) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_find_array[i](str, cstr, first, end) : wsize(-1);
			}
			static wsize find_reverse(type const& str, cstr_t const& cstr, windex first, windex end) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_find_reverse_array[i](str, cstr, first, end) : wsize(-1);
			}
			static wsize copy(type& str, cstr_t const& cstr) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_copy_array[i](str, cstr) : 0;
			}
			static wsize append(type& str, cstr_t const& cstr) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_append_array[i](str, cstr) : 0;
			}
			static wsize insert(type& str, cstr_t const& cstr, windex at) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_insert_array[i](str, cstr, at) : 0;
			}
			static vector<string> split(type const& str, cstr_t const& cstr) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_split_array[i](str, cstr) : null;
			}
			static wsize format(basic_string_buffer<E>* str, cstr_t const& cstr, var_args_t&& args) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_format_array[i](str, cstr, ang::forward<var_args_t>(args)) : 0;
			}
			static wsize append_format(basic_string_buffer<E>* str, cstr_t const& cstr, var_args_t&& args) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_append_format_array[i](str, cstr, ang::forward<var_args_t>(args)) : 0;
			}
			static string create_string(cstr_t const& cstr) {
				wsize i = get_index(cstr.encoding());
				return i < PACK_SIZE ? s_create_string_array[i](cstr) : null;
			}
		private:
			static wsize get_index(encoding E) { return wsize(E) - wsize(FIRST); }
			template<encoding E2> static int compare(type const& str, cstr_t const& cstr) { return str.compare(cstr.cstr<E2>()); }
			template<encoding E2> static wsize compare_until(type const& str, cstr_t const& cstr) { return str.compare_until(cstr.cstr<E2>()); }
			template<encoding E2> static wsize find(type const& str, cstr_t const& cstr, windex first, windex end) { return str.find(cstr.cstr<E2>(), first, end); }
			template<encoding E2> static wsize find_reverse(type const& str, cstr_t const& cstr, windex first, windex end) { return str.find_reverse(cstr.cstr<E2>(), first, end); }
			template<encoding E2> static wsize copy(type& str, cstr_t const& cstr) { return str.copy(cstr.cstr<E2>()); }
			template<encoding E2> static wsize append(type& str, cstr_t const& cstr) { return str.append(cstr.cstr<E2>()); }
			template<encoding E2> static wsize insert(type& str, cstr_t const& cstr, wsize i) { return str.insert(cstr.cstr<E2>(), i); }
			template<encoding E2> static vector<string> split(type const& source, cstr_t const& cstr) {		
				vector<string> list;
				auto val = cstr.cstr<E2>();

				windex beg = 0, end = 0;
				basic_string<E> _word = new basic_string_buffer<E>();

				auto data = source.cstr();
				wsize l = data.size(), c = val.size();
				end = text::text_encoder<E>::find(data, l, val, val.size(), 0);
				if (end == invalid_index) {
					list.push_back(new basic_string_buffer<E>(data));
					return ang::move(list);
				}
				do {
					source.sub_string(*_word, beg, end);
					if (!_word->is_empty()) {
						list.push_back(_word.get());
						_word = new basic_string_buffer<E>();
					}

					beg = end + val.size();
					end = text::text_encoder<E>::find(data, l, val, val.size(), beg);
				} while (end != invalid_index);

				source.sub_string(*_word, beg, end);
				if (!_word->is_empty())
					list.push_back(_word.get());
				return ang::move(list);
			}
			template<encoding E2> static wsize format(basic_string_buffer<E>* str_, cstr_t const& cstr, var_args_t && args) {
				using T2 = typename char_type_by_encoding<E2>::char_type const;
				int a;
				auto format = cstr.cstr<E2>();
				
				fast_string<E>* str = str_;

				str->clear();
				text::text_format_flags_t f;
				wsize i = 0, t = 0, n = 0, l = 0;
				while (char32_t c = text::to_char32<false, false>(format, i))
				{
					if (c == U'{') {
						f.value = text_parser<E2>::parse(format, n, a);
						if (a > -1 && a < (long64)args.size()) {
							static_cast<type&>(*str).append(str_view<T2, E2>(&format[l], t - l));
							str->append(f.value ? args[a].to_string(f) : args[a].to_string());
							i = l = n;
						}
					}
					t = n = i;
				}
				static_cast<type&>(*str).append(str_view<T2, E2>(&format[l], i - l));
				return static_cast<type&>(*str).length();
			}
			template<encoding E2> static wsize append_format(basic_string_buffer<E>* str_, cstr_t const& cstr, var_args_t && args) {
				using T2 = typename char_type_by_encoding<E2>::char_type const;
				int a;
				auto format = cstr.cstr<E2>();
				fast_string<E>* str = str_;
				wsize len = str->length();
				
				str->clear();
				text::text_format_flags_t f;
				wsize i = 0, t = 0, n = 0, l = 0;
				while (char32_t c = text::to_char32<false, false>(format, i))
				{
					if (c == U'{') {
						f.value = text_parser<E2>::parse(format, n, a);
						if (a > -1 && a < (long64)args.size()) {
							static_cast<type&>(*str).append(str_view<T2, E2>(&format[l], t - l));
							str->append(f.value ? args[a].to_string(f) : args[a].to_string());
							i = l = n;
						}
					}
					t = n = i;
				}
				static_cast<type&>(*str).append(str_view<T2, E2>(&format[l], i - l));
				return static_cast<type&>(*str).length() - len;
			}
			template<encoding E2> static string create_string(cstr_t const& cstr) {
				return new text::basic_string_buffer<E2>(cstr.cstr<E2>());
			}

			static constexpr compare_type s_compare_array[] = { &compare<ES>... };
			static constexpr compare_until_type s_compare_until_array[] = { &compare_until<ES>... };
			static constexpr find_type s_find_array[] = { &find<ES>... };
			static constexpr find_type s_find_reverse_array[] = { &find_reverse<ES>... };
			static constexpr copy_type s_copy_array[] = { &copy<ES>... };
			static constexpr copy_type s_append_array[] = { &append<ES>... };
			static constexpr insert_type s_insert_array[] = { &insert<ES>... };
			static constexpr split_type s_split_array[] = { &split<ES>... };
			static constexpr format_type s_format_array[] = { format<ES>... };
			static constexpr format_type s_append_format_array[] = { append_format<ES>... };
			static constexpr create_string_type s_create_string_array[] = { &create_string<ES>... };
		};


	}
}
