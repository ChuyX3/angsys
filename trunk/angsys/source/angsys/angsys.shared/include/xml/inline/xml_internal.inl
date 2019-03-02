#pragma once

template<typename T, ang::dom::xml::xml_encoding E>
inline int ang::dom::xml::xml_utils<T,E>::xml_skip_space(ang::str_view<T, E> const& code, windex begin) {
	windex c = begin;
	char32_t at;
	while (((at = text::to_char32<false, text::is_endian_swapped<E>::value>(code, c)) == U' ')
		|| (at == U'\n')
		|| (at == U'\t')
		|| (at == U'\r'));
	return int(c - begin);
}

template<typename T, ang::dom::xml::xml_encoding E>
inline int ang::dom::xml::xml_utils<T, E>::xml_get_element_name(ang::str_view<T, E> const& code, windex begin) {
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

template<typename T, ang::dom::xml::xml_encoding E>
inline int ang::dom::xml::xml_utils<T, E>::xml_get_attibute_name(ang::str_view<T, E> const& code, windex begin) {
	windex c = begin;
	char32_t at;
	while (((at = text::to_char32<false, text::is_endian_swapped<E>::value>(code, c)) != U' ')
		&& (at != U'\n')
		&& (at != U'\t')
		&& (at != U'\r')
		&& (at != U'='));
	return int(c - begin);
}



inline int ang::dom::xml::xml_utils<void, ang::dom::xml::xml_encoding::auto_detect>::xml_skip_space(text::istring_view_t const& code, windex begin) {
	windex i = begin;
	wsize c = 0;
	char32_t at;
	while (((at = code->at(i, &c)) == U' ')
		|| (at == U'\n')
		|| (at == U'\t')
		|| (at == U'\r')) {
		i += c;
	}
	return int(i - begin);
}

inline int ang::dom::xml::xml_utils<void, ang::dom::xml::xml_encoding::auto_detect>::xml_get_element_name(text::istring_view_t const& code, windex begin) {
	windex i = begin;
	wsize c = 0;
	char32_t at;
	while (((at = code->at(i, &c)) != U' ')
		&& (at != U'\n')
		&& (at != U':')
		&& (at != U'\t')
		&& (at != U'\r')
		&& (at != U'/')
		&& (at != U'>')) {
		i += c;
	}
	return int(i - begin);
}

inline int ang::dom::xml::xml_utils<void, ang::dom::xml::xml_encoding::auto_detect>::xml_get_attibute_name(text::istring_view_t const& code, windex begin) {
	windex i = begin;
	wsize c = 0;
	char32_t at;
	while (((at = code->at(i, &c)) != U' ')
		&& (at != U'\n')
		&& (at != U'\t')
		&& (at != U'\r')
		&& (at != U'=')) {
		i += c;
	}
	return int(i - begin);
}