#ifndef __ANG_DEFINE_ENUM_H__
#define __ANG_DEFINE_ENUM_H__

#include "ang_inlines.h"


#define FUNCX_STRING_VALUE_PAIR(A0) { FUNCX_TOSTRING(A0), A0 }
#define FUNCX_VALUE_STRING_PAIR(A0) { A0, FUNCX_TOSTRING(A0) }

#define ANG_EXPAND_ENUM_N(M, ...) M(__VA_ARGS__)
#define ANG_EXPAND_ENUM_STRING_VALUE(...) { APPLY_FUNCX_N(FUNCX_STRING_VALUE_PAIR, __VA_ARGS__) }
#define ANG_EXPAND_ENUM_VALUE_STRING(...) { APPLY_FUNCX_N(FUNCX_VALUE_STRING_PAIR, __VA_ARGS__) }

#define ANG_IMPLEMENT_ENUM_TOSTRING(_NAME, _DEFVAL, ...) \
ang::cstr_t _NAME##_t::to_string()const { \
	static ang_pair<_NAME, cstr_t> _map[] = ANG_EXPAND_ENUM_VALUE_STRING(__VA_ARGS__); \
	auto idx = ang_binary_search<_NAME>(_value, _map); \
	return (idx >= array_size(_map))? _DEFVAL :  _map[idx].value; \
}

#define ANG_IMPLEMENT_ENUM_PARSE(_NAME, _DEFVAL, ...) \
_NAME##_t _NAME##_t::parse(ang::cstr_t text) { \
	static ang_pair<cstr_t, _NAME> _map[] = ANG_EXPAND_ENUM_STRING_VALUE(__VA_ARGS__); \
	auto idx = ang_binary_search<ang::cstr_t>(text, _map); \
	return (idx >= array_size(_map))? _DEFVAL :  _map[idx].value; \
} \
_NAME##_t _NAME##_t::parse(ang::cwstr_t text) { \
	static ang_pair<cstr_t, _NAME> _map[] = ANG_EXPAND_ENUM_STRING_VALUE(__VA_ARGS__); \
	auto idx = ang_binary_search<ang::cwstr_t>(text, _map); \
	return (idx >= array_size(_map))? _DEFVAL :  _map[idx].value; \
}

#endif//__ANG_DEFINE_ENUM_H__