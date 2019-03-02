#ifndef __ANGSYS_H__
#elif !defined __RAW_STR_H__
#define __RAW_STR_H__

namespace ang
{
	namespace text
	{
		struct LINK encoding_t : public ang::value<encoding>
		{
			static encoding_t parse(raw_cstr_t);
			static rtti_t const& class_info();
			encoding_t() : value(default_value<type>::value) {}
			encoding_t(type const& v) : value(v) {}
			encoding_t(encoding_t const& v) : value(v) {}
			encoding_t(type && v) : value(ang::forward<type>(v)) { }
			encoding_t(encoding_t && v) : value(ang::forward<value>(v)) { }
			cstr_t to_string()const;
			encoding_t& operator = (type const& v) { get() = v; return*this; }
			encoding_t& operator = (encoding_t const& v) { get() = v.get(); return*this; }
			encoding_t& operator = (type && v) { get() = ang::move(v); v = default_value<type>::value; return*this; }
			encoding_t& operator = (encoding_t && v) { get() = ang::move(v.get()); v.set(default_value<type>::value); return*this; }
			friend inline bool operator == (encoding_t const& a1, encoding_t const& a2) { return a1.get() == a2.get(); }
			friend inline bool operator != (encoding_t const& a1, encoding_t const& a2) { return a1.get() != a2.get(); }
			friend inline bool operator >= (encoding_t const& a1, encoding_t const& a2) { return a1.get() >= a2.get(); }
			friend inline bool operator <= (encoding_t const& a1, encoding_t const& a2) { return a1.get() <= a2.get(); }
			friend inline bool operator > (encoding_t const& a1, encoding_t const& a2) { return a1.get() > a2.get(); }
			friend inline bool operator < (encoding_t const& a1, encoding_t const& a2) { return a1.get() < a2.get(); }
			friend inline bool operator == (encoding_t const& a1, encoding a2) { return a1.get() == a2; }
			friend inline bool operator == (encoding a1, encoding_t const& a2) { return a1 == a2.get(); }
			friend inline bool operator != (encoding_t const& a1, encoding a2) { return a1.get() != a2; }
			friend inline bool operator != (encoding a1, encoding_t const& a2) { return a1 != a2.get(); }
			friend inline bool operator >= (encoding_t const& a1, encoding a2) { return a1.get() >= a2; }
			friend inline bool operator >= (encoding a1, encoding_t const& a2) { return a1 >= a2.get(); }
			friend inline bool operator <= (encoding_t const& a1, encoding a2) { return a1.get() <= a2; }
			friend inline bool operator <= (encoding a1, encoding_t const& a2) { return a1 <= a2.get(); }
			friend inline bool operator > (encoding_t const& a1, encoding a2) { return a1.get() > a2; }
			friend inline bool operator > (encoding a1, encoding_t const& a2) { return a1 > a2.get(); }
			friend inline bool operator < (encoding_t const& a1, encoding a2) { return a1.get() < a2; }
			friend inline bool operator < (encoding a1, encoding_t const& a2) { return a1 < a2.get(); }
		};

		struct LINK raw_str {
			raw_str();
			raw_str(void* v, wsize s, text::encoding e);
			raw_str(raw_str const& str);
			template<typename T, text::encoding E> inline raw_str(str_view<T, E> str)
				: raw_str(str.str(), str.size() * sizeof(typename text::char_type_by_encoding<E>::char_t), E) {
			}

			void* ptr()const;
			wsize size()const;
			wsize count()const;
			wsize char_size()const;
			text::encoding_t encoding()const;

			template<text::encoding E> inline operator str_view<typename text::char_type_by_encoding<E>::char_t, E>() {
				return E == m_encoding ? str_view<typename text::char_type_by_encoding<E>::char_t, E>(
					(typename text::char_type_by_encoding<E>::str_t)m_value,
					m_size / sizeof(typename text::char_type_by_encoding<E>::char_t))
					: str_view<typename text::char_type_by_encoding<E>::char_t, E>();
			}
			template<text::encoding E> inline operator cstr_view<typename text::char_type_by_encoding<E>::char_t, E>()const {
				return E == m_encoding ? cstr_view<typename text::char_type_by_encoding<E>::char_t, E>(
					(typename text::char_type_by_encoding<E>::cstr_t)m_value,
					m_size / sizeof(typename text::char_type_by_encoding<E>::char_t))
					: cstr_view<typename text::char_type_by_encoding<E>::char_t, E>();
			}
			template<text::encoding E> inline str_view<typename text::char_type_by_encoding<E>::char_t, E> to_str() {
				return E == m_encoding ? str_view<typename text::char_type_by_encoding<E>::char_t, E>(
					(typename text::char_type_by_encoding<E>::str_t)m_value,
					m_size / sizeof(typename text::char_type_by_encoding<E>::char_t))
					: str_view<typename text::char_type_by_encoding<E>::char_t, E>();
			}
			template<text::encoding E> inline cstr_view<typename text::char_type_by_encoding<E>::char_t, E> to_cstr()const {
				return E == m_encoding ? cstr_view<typename text::char_type_by_encoding<E>::char_t, E>(
					(typename text::char_type_by_encoding<E>::cstr_t)m_value,
					m_size / sizeof(typename text::char_type_by_encoding<E>::char_t))
					: cstr_view<typename text::char_type_by_encoding<E>::char_t, E>();
			}

			friend LINK bool operator == (raw_str_t const& str1, raw_str_t const& str2);
			friend LINK bool operator == (raw_str_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator != (raw_str_t const& str1, raw_str_t const& str2);
			friend LINK bool operator != (raw_str_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator >= (raw_str_t const& str1, raw_str_t const& str2);
			friend LINK bool operator >= (raw_str_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator <= (raw_str_t const& str1, raw_str_t const& str2);
			friend LINK bool operator <= (raw_str_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator > (raw_str_t const& str1, raw_str_t const& str2);
			friend LINK bool operator > (raw_str_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator < (raw_str_t const& str1, raw_str_t const& str2);
			friend LINK bool operator < (raw_str_t const& str1, raw_cstr_t const& str2);

			template<typename T, text::encoding E> friend inline bool operator == (raw_str_t const& str1, str_view<T, E> const& str2) { return operator == (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator == (str_view<T, E> const& str1, raw_str_t const& str2) { return operator == (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator != (raw_str_t const& str1, str_view<T, E> const& str2) { return operator != (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator != (str_view<T, E> const& str1, raw_str_t const& str2) { return operator != (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator >= (raw_str_t const& str1, str_view<T, E> const& str2) { return operator >= (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator >= (str_view<T, E> const& str1, raw_str_t const& str2) { return operator >= (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator <= (raw_str_t const& str1, str_view<T, E> const& str2) { return operator <= (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator <= (str_view<T, E> const& str1, raw_str_t const& str2) { return operator <= (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator > (raw_str_t const& str1, str_view<T, E> const& str2) { return operator > (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator > (str_view<T, E> const& str1, raw_str_t const& str2) { return operator > (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator < (raw_str_t const& str1, str_view<T, E> const& str2) { return operator < (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator < (str_view<T, E> const& str1, raw_str_t const& str2) { return operator < (raw_cstr(str1), str2); }

		private:
			void* m_value;
			wsize m_size;
			text::encoding_t m_encoding;
		};

		struct LINK raw_cstr {
			raw_cstr();
			raw_cstr(void const* v, wsize s, text::encoding e);
			raw_cstr(raw_cstr const& str);
			raw_cstr(raw_str const& str);
			template<typename T, text::encoding E> inline raw_cstr(str_view<T, E> str)
				: raw_cstr(str.cstr(), str.size() * sizeof(typename text::char_type_by_encoding<E>::char_t), E) {
			}
			template<typename T, text::encoding E> inline raw_cstr(cstr_view<T, E> str)
				: raw_cstr(str.cstr(), str.size() * sizeof(typename text::char_type_by_encoding<E>::char_t), E) {
			}
			template<typename T, wsize N> inline raw_cstr(const T(&str)[N])
				: raw_cstr(str, (N - 1) * sizeof(T), text::encoding_by_char_type<T>::value) {
			}

			void const* ptr()const;
			wsize size()const;
			wsize count()const;
			wsize char_size()const;
			text::encoding_t encoding()const;

			template<text::encoding E> inline operator cstr_view<typename text::char_type_by_encoding<E>::char_t, E>()const {
				return E == m_encoding ? cstr_view<typename text::char_type_by_encoding<E>::char_t, E>(
					(typename text::char_type_by_encoding<E>::cstr_t)m_value,
					m_size / sizeof(typename text::char_type_by_encoding<E>::char_t))
					: cstr_view<typename text::char_type_by_encoding<E>::char_t, E>();
			}

			template<text::encoding E> inline cstr_view<typename text::char_type_by_encoding<E>::char_t, E> to_cstr()const {
				return E == m_encoding ? cstr_view<typename text::char_type_by_encoding<E>::char_t, E>(
					(typename text::char_type_by_encoding<E>::cstr_t)m_value,
					m_size / sizeof(typename text::char_type_by_encoding<E>::char_t))
					: cstr_view<typename text::char_type_by_encoding<E>::char_t, E>();
			}

			friend LINK bool operator == (raw_cstr_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator == (raw_cstr_t const& str1, raw_str_t const& str2);
			friend LINK bool operator != (raw_cstr_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator != (raw_cstr_t const& str1, raw_str_t const& str2);
			friend LINK bool operator >= (raw_cstr_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator >= (raw_cstr_t const& str1, raw_str_t const& str2);
			friend LINK bool operator <= (raw_cstr_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator <= (raw_cstr_t const& str1, raw_str_t const& str2);
			friend LINK bool operator > (raw_cstr_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator > (raw_cstr_t const& str1, raw_str_t const& str2);
			friend LINK bool operator < (raw_cstr_t const& str1, raw_cstr_t const& str2);
			friend LINK bool operator < (raw_cstr_t const& str1, raw_str_t const& str2);

			template<typename T, text::encoding E> friend inline bool operator == (raw_cstr_t const& str1, str_view<T, E> const& str2) { return operator == (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator == (str_view<T, E> const& str1, raw_cstr_t const& str2) { return operator == (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator != (raw_cstr_t const& str1, str_view<T, E> const& str2) { return operator != (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator != (str_view<T, E> const& str1, raw_cstr_t const& str2) { return operator != (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator >= (raw_cstr_t const& str1, str_view<T, E> const& str2) { return operator >= (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator >= (str_view<T, E> const& str1, raw_cstr_t const& str2) { return operator >= (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator <= (raw_cstr_t const& str1, str_view<T, E> const& str2) { return operator <= (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator <= (str_view<T, E> const& str1, raw_cstr_t const& str2) { return operator <= (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator > (raw_cstr_t const& str1, str_view<T, E> const& str2) { return operator > (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator > (str_view<T, E> const& str1, raw_cstr_t const& str2) { return operator > (raw_cstr(str1), str2); }
			template<typename T, text::encoding E> friend inline bool operator < (raw_cstr_t const& str1, str_view<T, E> const& str2) { return operator < (str1, raw_cstr(str2)); }
			template<typename T, text::encoding E> friend inline bool operator < (str_view<T, E> const& str1, raw_cstr_t const& str2) { return operator < (raw_cstr(str1), str2); }

		private:
			void const* m_value;
			wsize m_size;
			text::encoding_t m_encoding;
		};
	}

}

#endif//__RAW_STR_H__