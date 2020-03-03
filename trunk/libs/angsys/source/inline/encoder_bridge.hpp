
#include "angsys.h"

namespace ang
{
	namespace text {

		template<encoding E, typename EP = typename value_pack_from_range<encoding, encoding::ascii, encoding::utf32_be>::type>
		struct encoder_bridge1;

		template<typename EP> struct encoder_bridge2;
		using encoder_bridge = encoder_bridge2<typename value_pack_from_range<encoding, encoding::ascii, encoding::utf32_be>::type>;

		template<encoding E, encoding...ES>
		struct encoder_bridge1<E, value_pack<encoding, ES...>>
		{
			using char_type = typename char_type_by_encoding<E>::char_type;
			using str_type = str_view<char_type, E>;
			using cstr_type = str_view<const char_type, E>;
			
			using size_type = wsize(*)(cstr_t const&, windex, windex);
			using compare_type = int (*)(cstr_type const&, cstr_t const&);
			using compare_until_type = windex(*)(cstr_type const&, cstr_t const&);
			using find_type = windex(*)(cstr_type const&, cstr_t const&, windex);
			using convert_type = cstr_t(*)(str_type, wsize&, cstr_t const&, wsize&, bool, wsize, wsize);

			using pack_type = value_pack<encoding, ES...>;
			static constexpr encoding FIRST = pack_type::get<0>();
			static constexpr wsize PACK_SIZE = value_pack_size<pack_type>::value;

			inline static wsize size(cstr_t const& from, windex start, windex end) {
				windex idx = get_index(from.encoding());
				return idx < PACK_SIZE ? s_size_array[idx](from, start, end) : 0;
			}
			inline static int compare(cstr_type const& first, cstr_t const& second) {
				windex idx = get_index(second.encoding());
				return idx < PACK_SIZE ? s_compare_array[idx](first, second) : -10;
			}
			inline static windex compare_until(cstr_type const& first, cstr_t const& second) {
				windex idx = get_index(second.encoding());
				return idx < PACK_SIZE ? s_compare_until_array[idx](first, second) : 0;
			}
			inline static windex find(cstr_type const& first, cstr_t const& second, windex start) {
				windex idx = get_index(second.encoding());
				return idx < PACK_SIZE ? s_find_array[idx](first, second, start) : (wsize)invalid_index;
			}
			inline static windex find_reverse(cstr_type const& first, cstr_t const& second, windex start) {
				windex idx = get_index(second.encoding());
				return idx < PACK_SIZE ? s_find_reverse_array[idx](first, second, start) : (wsize)invalid_index;
			}
			inline static cstr_t convert(str_type str, wsize& i, cstr_t const& cstr, wsize& j, bool eos, wsize max_out, wsize max_in) {
				windex idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_convert_array[idx](str, i, cstr, j, eos, max_out, max_in) : cstr_t();
			}
		private:
			static wsize get_index(encoding E) { return wsize(E) - wsize(FIRST); }
			template<encoding E2> inline static wsize size(cstr_t const& from, windex start, windex end) {
				return text_encoder<E>::size(from.cstr<E2>(), start, end);
			}
			template<encoding E2> inline static int compare(cstr_type const& first, cstr_t const& second) {
				return text_encoder<E>::compare(first.cstr(), second.cstr<E2>());
			}
			template<encoding E2> inline static windex compare_until(cstr_type const& first, cstr_t const& second) {
				return text_encoder<E>::compare_until(first.cstr(), second.cstr<E2>());
			}
			template<encoding E2> inline static windex find(cstr_type const& first, cstr_t const& second_, windex start) {
				auto second = second_.cstr<E2>();
				return text_encoder<E>::find(first.cstr(), first.size(), second, second.size(), start);
			}
			template<encoding E2> inline static windex find_reverse(cstr_type const& first, cstr_t const& second_, windex start) {
				auto second = second_.cstr<E2>();
				return text_encoder<E>::find_reverse(first.cstr(), first.size(), second, second.size(), start);
			}
			template<encoding E2> inline static cstr_t convert(str_type str, wsize& i, cstr_t const& cstr, wsize& j, bool eos, wsize max_out, wsize max_in) {
				auto data = text_encoder<E>::convert(str.str(), i, cstr.cstr<E2>(), j, eos, max_out, max_in);
				return cstr_t(data, i * size_of<char_type>(), E);
			}
		
			static constexpr size_type s_size_array[] = { &size<ES>... };
			static constexpr compare_type s_compare_array[] = { &compare<ES>... };
			static constexpr compare_until_type s_compare_until_array[] = { &compare_until<ES>... };
			static constexpr find_type s_find_array[] = { &find<ES>... };
			static constexpr find_type s_find_reverse_array[] = { &find_reverse<ES>... };
			static constexpr convert_type s_convert_array[] = { &convert<ES>... };
		};

		template<encoding...ES>
		struct encoder_bridge2<value_pack<encoding, ES...>>
		{
			using pack_type = value_pack<encoding, ES...>;
			static constexpr encoding FIRST = pack_type::get<0>();
			static constexpr wsize PACK_SIZE = value_pack_size<pack_type>::value;

			using count_type = wsize(*)(cstr_t const&);
			using size_type = wsize(*)(cstr_t const&, windex, windex);
			using compare_type = int(*)(cstr_t const&, cstr_t const&);
			using compare_until_type = windex(*)(cstr_t const&, cstr_t const&);
			using find_type = windex(*)(cstr_t const&, cstr_t const&, windex);
			using find_any_type = windex(*)(cstr_t const&, windex, array_view<const char32>);
			using to_chart32_type = char32_t(*)(cstr_t const&, windex&);
			using set_eos_type = void(*)(str_t, wsize);
			using convert_type = cstr_t(*)(str_t, wsize&, cstr_t const&, wsize&, bool, wsize, wsize);

			static inline wsize lenght(cstr_t const& cstr) {
				auto idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_lenght_array[idx](cstr) : 0;
			}
			static inline wsize count(cstr_t const& cstr) {
				auto idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_count_array[idx](cstr) : 0;
			}
			static inline wsize size(cstr_t const& from, encoding to, windex start, windex end) {
				auto idx = get_index(to);
				return idx < PACK_SIZE ? s_size_array[idx](from, start, end) : 0;
			}
			static inline int compare(cstr_t const& first, cstr_t const& second) {
				auto idx = get_index(first.encoding());
				return idx < PACK_SIZE ? s_compare_array[idx](first, second) : -10;
			}
			static inline windex compare_until(cstr_t const& first, cstr_t const& second) {
				auto idx = get_index(first.encoding());
				return idx < PACK_SIZE ? s_compare_until_array[idx](first, second) : 0;
			}
			static inline windex find(cstr_t const& first, cstr_t const& second, windex start) {
				auto idx = get_index(first.encoding());
				return idx < PACK_SIZE ? s_find_array[idx](first, second, start) : windex(invalid_index);
			}
			static inline windex find_reverse(cstr_t const& first, cstr_t const& second, windex start) {
				auto idx = get_index(first.encoding());
				return idx < PACK_SIZE ? s_find_reverse_array[idx](first, second, start) : windex(invalid_index);
			}
			static inline windex find_any(cstr_t const& first, windex start, array_view<const char32> chars) {
				auto idx = get_index(first.encoding());
				return idx < PACK_SIZE ? s_find_any_array[idx](first, start, chars) : windex(invalid_index);
			}
			static char32_t to_char32(cstr_t const& cstr, windex& at)
			{
				auto idx = get_index(cstr.encoding());
				return idx < PACK_SIZE ? s_to_char32_array[idx](cstr, at) : char32_t(0);
			}
			static void set_eos(str_t str, windex at) {
				auto idx = get_index(str.encoding());
				idx < PACK_SIZE ? s_set_eos_array[idx](str, at) : void(0);
			}
			static inline cstr_t convert(str_t str, wsize& i, cstr_t const& cstr, wsize& j, bool eos, wsize max_out, wsize max_in) {
				auto idx = get_index(str.encoding());
				return idx < PACK_SIZE ? s_convert_array[idx](str, i, cstr, j, eos, max_out, max_in) : cstr_t();
			}
		private:
			static wsize get_index(encoding E) { return wsize(E) - wsize(FIRST); }
			template<encoding E> static inline wsize lenght(cstr_t const& cstr) {
				return text_encoder<E>::lenght(cstr.cstr<E>());
			}
			template<encoding E> static inline wsize count(cstr_t const& cstr) {
				return text_encoder<E>::count(cstr.cstr<E>());
			}
			template<encoding E> static inline wsize size(cstr_t const& from, windex start, windex end) {
				return encoder_bridge1<E>::size(from, start, end);
			}
			template<encoding E> static inline int compare(cstr_t const& first, cstr_t const& second) {
				return encoder_bridge1<E>::compare(first.cstr<E>(), second);
			}
			template<encoding E> static inline windex compare_until(cstr_t const& first, cstr_t const& second) {
				return encoder_bridge1<E>::compare_until(first.cstr<E>(), second);
			}
			template<encoding E> static inline windex find(cstr_t const& first, cstr_t const& second, windex start) {
				return encoder_bridge1<E>::find(first.cstr<E>(), second, start);
			}
			template<encoding E> static inline windex find_reverse(cstr_t const& first, cstr_t const& second, windex start) {
				return encoder_bridge1<E>::find_reverse(first.cstr<E>(), second, start);
			}
			template<encoding E> static inline windex find_any(cstr_t const& first_, windex start, array_view<const char32> chars) {
				auto first = first_.cstr<E>();
				return text_encoder<E>::find_any(first, first.size(), start, chars);
			}
			template<encoding E> static inline char32_t to_char32(cstr_t const& cstr, windex& at) {
				return text::to_char32<false, is_endian_swapped<E>::value>(cstr.cstr<E>(), at);
			}
			template<encoding E> static inline  void set_eos(str_t str, windex at) {
				wsize j = 0;
				text_encoder<E>::convert(str.str<E>().str(), at, "", j, true, -1, -1);
			}
			template<encoding E> static inline cstr_t convert(str_t str, wsize& i, cstr_t const& cstr, wsize& j, bool eos, wsize max_out, wsize max_in) {
				return encoder_bridge1<E>::convert(str.str<E>(), i, cstr, i, eos, max_out, max_in);
			}

			static constexpr count_type s_lenght_array[] = { &lenght<ES>... };
			static constexpr count_type s_count_array[] = { &count<ES>... };
			static constexpr size_type s_size_array[] = { &size<ES>... };
			static constexpr compare_type s_compare_array[] = { &compare<ES>... };
			static constexpr compare_until_type s_compare_until_array[] = { &compare_until<ES>... };
			static constexpr find_type s_find_array[] = { &find<ES>... };
			static constexpr find_type s_find_reverse_array[] = { &find_reverse<ES>... };
			static constexpr find_any_type s_find_any_array[] = { &find_any<ES>... };
			static constexpr to_chart32_type s_to_char32_array[] = { &to_char32<ES>... };
			static constexpr set_eos_type s_set_eos_array[] = { &set_eos<ES>... };
			static constexpr convert_type s_convert_array[] = { &convert<ES>... };
		};

	}
}
