#ifndef __ANGSYS_H__
#elif !defined __STRING_H__
#define __STRING_H__

namespace ang
{
	typedef struct LINK raw_str {
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
		text::encoding encoding()const;

		template<text::encoding E> inline operator str_view<typename text::char_type_by_encoding<E>::char_t, E>() {
			return E == _encoding ? str_view<typename text::char_type_by_encoding<E>::char_t, E>(
				(typename text::char_type_by_encoding<E>::str_t)_value,
				_size / *sizeof(typename text::char_type_by_encoding<E>::char_t))
				: str_view<typename text::char_type_by_encoding<E>::char_t, E>();
		}
		template<text::encoding E> inline operator cstr_view<typename text::char_type_by_encoding<E>::char_t, E>()const {
			return E == _encoding ? cstr_view<typename text::char_type_by_encoding<E>::char_t, E>(
				(typename text::char_type_by_encoding<E>::cstr_t)_value,
				_size / *sizeof(typename text::char_type_by_encoding<E>::char_t))
				: cstr_view<typename text::char_type_by_encoding<E>::char_t, E>();
		}
		template<text::encoding E> inline str_view<typename text::char_type_by_encoding<E>::char_t, E> to_str() {
			return E == _encoding ? str_view<typename text::char_type_by_encoding<E>::char_t, E>(
				(typename text::char_type_by_encoding<E>::str_t)_value,
				_size / *sizeof(typename text::char_type_by_encoding<E>::char_t))
				: str_view<typename text::char_type_by_encoding<E>::char_t, E>();
		}
		template<text::encoding E> inline cstr_view<typename text::char_type_by_encoding<E>::char_t, E> to_cstr()const {
			return E == _encoding ? cstr_view<typename text::char_type_by_encoding<E>::char_t, E>(
				(typename text::char_type_by_encoding<E>::cstr_t)_value,
				_size / *sizeof(typename text::char_type_by_encoding<E>::char_t))
				: cstr_view<typename text::char_type_by_encoding<E>::char_t, E>();
		}

	private:
		void* _value;
		wsize _size;
		text::encoding _encoding;
	}raw_str_t;

	typedef struct LINK raw_cstr {
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

		void const* ptr()const;
		wsize size()const;
		wsize count()const;
		wsize char_size()const;
		text::encoding encoding()const;

		template<text::encoding E> inline operator cstr_view<typename text::char_type_by_encoding<E>::char_t, E>()const;
		template<text::encoding E> inline cstr_view<typename text::char_type_by_encoding<E>::char_t, E> to_cstr()const;

	private:
		void const* _value;
		wsize _size;
		text::encoding _encoding;
	}raw_cstr_t;

	namespace text
	{
		typedef void* unknown_str_t;
		typedef  void const* unknown_cstr_t;

		ang_interface(iencoder);
		ang_interface(itext_buffer);

		ang_begin_interface(LINK itext_buffer, ibuffer)
			visible vcall raw_str_t raw_str() pure;
			visible vcall raw_cstr_t raw_cstr()const pure;
		ang_end_interface();

		ang_begin_interface(LINK iencoder)
			visible scall iencoder_t get_encoder(encoding_t);
			visible vcall encoding_t format()const pure;
			visible vcall wsize lenght(unknown_cstr_t)const pure;
			visible vcall wsize size(unknown_cstr_t, encoding_t)const pure;
			visible vcall int compare(unknown_cstr_t, unknown_cstr_t, encoding_t)const pure;
			visible vcall wsize compare_until(unknown_cstr_t, unknown_cstr_t, encoding_t)const pure;
			visible vcall raw_str_t convert(unknown_str_t dest, unknown_cstr_t src, encoding_t e, bool set_eos = true, wsize max_size = - 1)const pure;
			visible vcall raw_str_t convert(unknown_str_t dest, wsize& i, unknown_cstr_t src, wsize& j, encoding_t e, bool set_eos = true, wsize max_size = - 1)const pure;
		ang_end_interface();

	}

	ANG_INTF_WRAPPER_DECLARATION(LINK, text::itext_buffer);
	ANG_INTF_WRAPPER_DECLARATION(LINK, text::iencoder);

	namespace strings
	{


	}
}

#endif//__STRING_H__