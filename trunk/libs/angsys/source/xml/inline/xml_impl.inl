#pragma once

template<typename T, coffe::dom::xml::xml_encoding E>
inline int coffe::dom::xml::xml_utils<T,E>::xml_skip_space(coffe::str_view<T, E> const& code, windex begin) {
	windex c = begin;
	char32_t at;
	while (((at = text::to_char32<false, text::is_endian_swapped<E>::value>(code, c)) == U' ')
		|| (at == U'\n')
		|| (at == U'\t')
		|| (at == U'\r'));
	return int(c - begin);
}

template<typename T, coffe::dom::xml::xml_encoding E>
inline int coffe::dom::xml::xml_utils<T, E>::xml_get_element_name(coffe::str_view<T, E> const& code, windex begin) {
	windex c = begin;
	char32_t at;
	while (((at = text::to_char32<false, text::is_endian_swapped<E>::value>(code, c)) != U' ')
		&& (at != U'\n')
		&& (at != U':')
		&& (at != U'\t')
		&& (at != U'\r')
		&& (at != U'/')
		&& (at != U'>'));
	return int(c - begin);
}

template<typename T, coffe::dom::xml::xml_encoding E>
inline int coffe::dom::xml::xml_utils<T, E>::xml_get_attibute_name(coffe::str_view<T, E> const& code, windex begin) {
	windex c = begin;
	char32_t at;
	while (((at = text::to_char32<false, text::is_endian_swapped<E>::value>(code, c)) != U' ')
		&& (at != U'\n')
		&& (at != U'\t')
		&& (at != U'\r')
		&& (at != U'='));
	return int(c - begin);
}
