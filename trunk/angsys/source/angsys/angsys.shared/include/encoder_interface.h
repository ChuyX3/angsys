#pragma once

namespace ang
{
	namespace text
	{
		template<encoding E>
		class encoder_interface
			: public object
			, public iencoder
		{
		public:
			const encoding ENCODING = E;
			typedef typename char_type_by_encoding<E>::char_t char_t;
			typedef typename char_type_by_encoding<E>::str_t str_t;
			typedef typename char_type_by_encoding<E>::cstr_t cstr_t;

			encoder_interface();

			ANG_DECLARE_INTERFACE();

			virtual encoding_t format()const override;
			virtual rtti_t const& char_type()const override;
			virtual char32 to_char32(unknown_cstr_t str, windex& i, bool increment)const override;
			virtual void set_eos(unknown_str_t str, windex at)const override;
			virtual wsize lenght(unknown_cstr_t cstr)const override;
			virtual wsize size(unknown_cstr_t cstr, encoding_t e, windex start, windex end)const override;
			virtual int compare(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const override;
			virtual wsize compare_until(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const override;
			virtual wsize find(unknown_cstr_t cstr1, wsize s1, unknown_cstr_t cstr2, wsize s2, encoding_t e, windex start)const override;
			virtual wsize find_any(unknown_cstr_t cstr, wsize sz, wsize start, array_view<const char32>)const override;
			virtual wsize find_reverse(unknown_cstr_t cstr1, wsize s1, unknown_cstr_t cstr2, wsize s2, encoding_t e, windex start)const override;
			virtual raw_str_t convert(unknown_str_t dest, unknown_cstr_t src, encoding_t e, bool set_eos, wsize max_out_size, wsize max_in_size)const override;
			virtual raw_str_t convert(unknown_str_t dest, wsize& i, unknown_cstr_t src, wsize& j, encoding_t e, bool set_eos, wsize max_out_size, wsize max_in_size)const override;

		private:
			virtual~encoder_interface();
		};


		template<encoding E> inline
		encoder_interface<E>::encoder_interface() {
		}

		template<encoding E> inline
		encoder_interface<E>::~encoder_interface() {
		}


		template<encoding E> 
		inline encoding_t encoder_interface<E>::format()const {
			return E;
		}

		template<encoding E> 
		inline rtti_t const& encoder_interface<E>::char_type()const {
			return rtti::type_of<char_t>();
		}

		template<encoding E> 
		inline wsize encoder_interface<E>::lenght(unknown_cstr_t cstr)const {
			return encoder<E>::lenght((cstr_t)cstr);
		}

		template<encoding E>
		inline char32 encoder_interface<E>::to_char32(unknown_cstr_t str, windex& i, bool increment)const {
			windex j = i;
			return text::to_char32<false, text::is_endian_swapped<E>::value>((cstr_t)str, increment ? i : j);
		}


		template<encoding E> inline
		void encoder_interface<E>::set_eos(unknown_str_t dest, windex at)const {
			((str_t)dest)[at] = 0;
		}

		template<encoding E> inline
		wsize encoder_interface<E>::size(unknown_cstr_t cstr, encoding_t e, windex start, windex end)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<E>::size((typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr, start, end);
			case encoding::unicode:return encoder<E>::size((typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr, start, end);
			case encoding::utf8: return encoder<E>::size((typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr, start, end);
			case encoding::utf16: return encoder<E>::size((typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr, start, end);
			case encoding::utf16_se: return encoder<E>::size((typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr, start, end);
			case encoding::utf16_le: return encoder<E>::size((typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr, start, end);
			case encoding::utf16_be: return encoder<E>::size((typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr, start, end);
			case encoding::utf32: return encoder<E>::size((typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr, start, end);
			case encoding::utf32_se: return encoder<E>::size((typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr, start, end);
			case encoding::utf32_le: return encoder<E>::size((typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr, start, end);
			case encoding::utf32_be: return encoder<E>::size((typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr, start, end);
			default: return 0;
			}
		}

		template<encoding E> inline
		int encoder_interface<E>::compare(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr2);
			case encoding::unicode:return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr2);
			case encoding::utf8: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr2);
			case encoding::utf16: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr2);
			case encoding::utf16_se: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr2);
			case encoding::utf16_le: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr2);
			case encoding::utf16_be: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr2);
			case encoding::utf32: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr2);
			case encoding::utf32_se: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr2);
			case encoding::utf32_le: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr2);
			case encoding::utf32_be: return encoder<E>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr2);
			default: return -10;
			}
		}

		template<encoding E> inline
		wsize encoder_interface<E>::compare_until(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr2);
			case encoding::unicode:return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr2);
			case encoding::utf8: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr2);
			case encoding::utf16: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr2);
			case encoding::utf16_se: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr2);
			case encoding::utf16_le: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr2);
			case encoding::utf16_be: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr2);
			case encoding::utf32: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr2);
			case encoding::utf32_se: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr2);
			case encoding::utf32_le: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr2);
			case encoding::utf32_be: return encoder<E>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr2);
			default: return 0;
			}
		}

		template<encoding E> inline
		wsize encoder_interface<E>::find(unknown_cstr_t cstr1, wsize s1, unknown_cstr_t cstr2, wsize s2, encoding_t e, windex start)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr2, s2, start);
			case encoding::unicode:return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr2, s2, start);
			case encoding::utf8: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr2, s2, start);
			case encoding::utf16: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr2, s2, start);
			case encoding::utf16_se: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr2, s2, start);
			case encoding::utf16_le: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr2, s2, start);
			case encoding::utf16_be: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr2, s2, start);
			case encoding::utf32: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr2, s2, start);
			case encoding::utf32_se: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr2, s2, start);
			case encoding::utf32_le: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr2, s2, start);
			case encoding::utf32_be: return encoder<E>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr2, s2, start);
			default: return 0;
			}
		}

		template<encoding E> inline
			wsize encoder_interface<E>::find_any(unknown_cstr_t cstr1, wsize s1, wsize start, array_view<const char32> chars)const {
			return encoder<E>::find_any((cstr_t)cstr1, s1, start, chars);
		}

		template<encoding E> inline
		wsize encoder_interface<E>::find_reverse(unknown_cstr_t cstr1, wsize s1, unknown_cstr_t cstr2, wsize s2, encoding_t e, windex start)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr2, s2, start);
			case encoding::unicode:return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr2, s2, start);
			case encoding::utf8: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr2, s2, start);
			case encoding::utf16: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr2, s2, start);
			case encoding::utf16_se: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr2, s2, start);
			case encoding::utf16_le: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr2, s2, start);
			case encoding::utf16_be: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr2, s2, start);
			case encoding::utf32: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr2, s2, start);
			case encoding::utf32_se: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr2, s2, start);
			case encoding::utf32_le: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr2, s2, start);
			case encoding::utf32_be: return encoder<E>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr2, s2, start);
			default: return 0;
			}
		}


		template<encoding E> inline
		raw_str_t encoder_interface<E>::convert(unknown_str_t dest, unknown_cstr_t src, encoding_t e, bool set_eos, wsize max_out_size, wsize max_in_size)const {
			wsize i = 0, j = 0;
			switch (e.get())
			{
			case encoding::ascii: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::ascii>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::unicode: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::unicode>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf8: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf8>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf16: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf16_se: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf16_le: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf16_be: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf32: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf32_se: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf32_le: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf32_be: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			default: return raw_str();
			}
		}

		template<encoding E> inline
		raw_str_t encoder_interface<E>::convert(unknown_str_t dest, wsize& i, unknown_cstr_t src, wsize& j, encoding_t e, bool set_eos, wsize max_out_size, wsize max_in_size)const {
			switch (e.get())
			{
			case encoding::ascii: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::ascii>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::unicode: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::unicode>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf8: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf8>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf16: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf16_se: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf16_le: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf16_be: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf32: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf32_se: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf32_le: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			case encoding::utf32_be: encoder<E>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, E);
			default: return raw_str();
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}