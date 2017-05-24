#ifndef __ANG_CORE_INLINES_H__
#error ...
#elif !defined __ANG_PAIR_HPP__
#define __ANG_PAIR_HPP__



template<typename _key_t, typename _value_t>
inline bool operator == (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second) {
	return first.get_key() == second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator != (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second) {
	return first.get_key() != second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator >= (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second) {
	return first.get_key() >= second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator <= (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second) {
	return first.get_key() <= second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator > (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second) {
	return first.get_key() > second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator < (ang_pair_t<_key_t, _value_t> const& first, ang_pair_t<_key_t, _value_t> const& second) {
	return first.get_key() < second.get_key();
}

template<typename _key_t, typename _value_t>
inline bool operator == (ang_pair_t<_key_t, _value_t> const& first, _key_t second) {
	return first.get_key() == second;
}
template<typename _key_t, typename _value_t>
inline bool operator != (ang_pair_t<_key_t, _value_t> const& first, _key_t second) {
	return first.get_key() != second;
}
template<typename _key_t, typename _value_t>
inline bool operator >= (ang_pair_t<_key_t, _value_t> const& first, _key_t second) {
	return first.get_key() >= second;
}
template<typename _key_t, typename _value_t>
inline bool operator <= (ang_pair_t<_key_t, _value_t> const& first, _key_t second) {
	return first.get_key() <= second;
}
template<typename _key_t, typename _value_t>
inline bool operator > (ang_pair_t<_key_t, _value_t> const& first, _key_t second) {
	return first.get_key() > second;
}
template<typename _key_t, typename _value_t>
inline bool operator < (ang_pair_t<_key_t, _value_t> const& first, _key_t second) {
	return first.get_key() < second;
}

template<typename _key_t, typename _value_t>
inline bool operator == (_key_t first, ang_pair_t<_key_t, _value_t> const& second) {
	return first == second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator != (_key_t first, ang_pair_t<_key_t, _value_t> const& second) {
	return first != second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator >= (_key_t first, ang_pair_t<_key_t, _value_t> const& second) {
	return first >= second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator <= (_key_t first, ang_pair_t<_key_t, _value_t> const& second) {
	return first <= second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator > (_key_t first, ang_pair_t<_key_t, _value_t> const& second) {
	return first > second.get_key();
}
template<typename _key_t, typename _value_t>
inline bool operator < (_key_t first, ang_pair_t<_key_t, _value_t> const& second) {
	return first < second.get_key();
}

///////////////////////////////////////////////////////////////////////////////////////////////

template<typename _key_t, typename _value_t>
ang_pair_t<_key_t, _value_t>::ang_pair_t()
	: _key(), _value() {

}

template<typename _key_t, typename _value_t>
ang_pair_t<_key_t, _value_t>::ang_pair_t(const ang_pair_t<_key_t, _value_t>& other)
	: _key(other._key), _value(other._value) {

}

template<typename _key_t, typename _value_t>
ang_pair_t<_key_t, _value_t>::ang_pair_t(_key_t key, const _value_t& value)
	: _key(key), _value(value) {

}

template<typename _key_t, typename _value_t>
ang_pair_t<_key_t, _value_t>::~ang_pair_t()
{

}


#endif//__ANG_PAIR_HPP__
