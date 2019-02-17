#pragma once

namespace ang
{
	namespace text
	{
		template<encoding ENCODING>
		class encoder_interface
			: public object
			, public iencoder
		{
		public:
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<ENCODING>::str_t str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t cstr_t;

			encoder_interface();

			ANG_DECLARE_INTERFACE();

			virtual encoding_t format()const override;
			virtual rtti_t const& char_type()const override;
			virtual char32 to_char32(unknown_cstr_t str, windex& i)const override;
			virtual long64 to_signed(unknown_cstr_t str, windex& i, int base)const override;
			virtual ulong64 to_unsigned(unknown_cstr_t str, windex& i, int base)const override;
			virtual double to_floating(unknown_cstr_t str, windex& i, bool ex)const override;
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



		template<encoding ENCODING> inline
		encoder_interface<ENCODING>::encoder_interface() {
		}

		template<encoding ENCODING> inline
		encoder_interface<ENCODING>::~encoder_interface() {
		}


		template<encoding ENCODING> 
		inline encoding_t encoder_interface<ENCODING>::format()const {
			return ENCODING;
		}

		template<encoding ENCODING> 
		inline rtti_t const& encoder_interface<ENCODING>::char_type()const {
			return rtti::type_of<char_t>();
		}

		template<encoding ENCODING> 
		inline wsize encoder_interface<ENCODING>::lenght(unknown_cstr_t cstr)const {
			return encoder<ENCODING>::lenght((cstr_t)cstr);
		}

		template<encoding ENCODING>
		inline char32 encoder_interface<ENCODING>::to_char32(unknown_cstr_t str, windex& i)const {
			return text::to_char32<false, text::is_endian_swapped<ENCODING>::value>((cstr_t)str, i);
		}

		template<encoding ENCODING>
		inline long64 encoder_interface<ENCODING>::to_signed(unknown_cstr_t str, windex& i, int base)const {
			return str_to_signed<char_t const, ENCODING>((cstr_t)str, i, base);
		}

		template<encoding ENCODING>
		inline ulong64 encoder_interface<ENCODING>::to_unsigned(unknown_cstr_t str, windex& i, int base)const {
			return str_to_unsigned<char_t const, ENCODING>((cstr_t)str, i, base);
		}

		template<encoding ENCODING>
		inline double encoder_interface<ENCODING>::to_floating(unknown_cstr_t str, windex& i, bool ex)const {
			return str_to_floating<char_t const, ENCODING>((cstr_t)str, i, ex);
		}


		template<encoding ENCODING> inline
		void encoder_interface<ENCODING>::set_eos(unknown_str_t dest, windex at)const {
			((str_t)dest)[at] = 0;
		}

		template<encoding ENCODING> inline
		wsize encoder_interface<ENCODING>::size(unknown_cstr_t cstr, encoding_t e, windex start, windex end)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr, start, end);
			case encoding::unicode:return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr, start, end);
			case encoding::utf8: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr, start, end);
			case encoding::utf16: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr, start, end);
			case encoding::utf16_se: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr, start, end);
			case encoding::utf16_le: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr, start, end);
			case encoding::utf16_be: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr, start, end);
			case encoding::utf32: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr, start, end);
			case encoding::utf32_se: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr, start, end);
			case encoding::utf32_le: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr, start, end);
			case encoding::utf32_be: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr, start, end);
			default: return 0;
			}
		}

		template<encoding ENCODING> inline
		int encoder_interface<ENCODING>::compare(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr2);
			case encoding::unicode:return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr2);
			case encoding::utf8: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr2);
			case encoding::utf16: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr2);
			case encoding::utf16_se: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr2);
			case encoding::utf16_le: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr2);
			case encoding::utf16_be: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr2);
			case encoding::utf32: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr2);
			case encoding::utf32_se: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr2);
			case encoding::utf32_le: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr2);
			case encoding::utf32_be: return encoder<ENCODING>::compare((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr2);
			default: return -10;
			}
		}

		template<encoding ENCODING> inline
		wsize encoder_interface<ENCODING>::compare_until(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr2);
			case encoding::unicode:return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr2);
			case encoding::utf8: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr2);
			case encoding::utf16: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr2);
			case encoding::utf16_se: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr2);
			case encoding::utf16_le: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr2);
			case encoding::utf16_be: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr2);
			case encoding::utf32: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr2);
			case encoding::utf32_se: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr2);
			case encoding::utf32_le: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr2);
			case encoding::utf32_be: return encoder<ENCODING>::compare_until((cstr_t)cstr1, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr2);
			default: return 0;
			}
		}

		template<encoding ENCODING> inline
		wsize encoder_interface<ENCODING>::find(unknown_cstr_t cstr1, wsize s1, unknown_cstr_t cstr2, wsize s2, encoding_t e, windex start)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr2, s2, start);
			case encoding::unicode:return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr2, s2, start);
			case encoding::utf8: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr2, s2, start);
			case encoding::utf16: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr2, s2, start);
			case encoding::utf16_se: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr2, s2, start);
			case encoding::utf16_le: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr2, s2, start);
			case encoding::utf16_be: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr2, s2, start);
			case encoding::utf32: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr2, s2, start);
			case encoding::utf32_se: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr2, s2, start);
			case encoding::utf32_le: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr2, s2, start);
			case encoding::utf32_be: return encoder<ENCODING>::find((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr2, s2, start);
			default: return 0;
			}
		}

		template<encoding ENCODING> inline
			wsize encoder_interface<ENCODING>::find_any(unknown_cstr_t cstr1, wsize s1, wsize start, array_view<const char32> chars)const {
			return encoder<ENCODING>::find_any((cstr_t)cstr1, s1, start, chars);
		}

		template<encoding ENCODING> inline
		wsize encoder_interface<ENCODING>::find_reverse(unknown_cstr_t cstr1, wsize s1, unknown_cstr_t cstr2, wsize s2, encoding_t e, windex start)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr2, s2, start);
			case encoding::unicode:return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr2, s2, start);
			case encoding::utf8: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr2, s2, start);
			case encoding::utf16: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr2, s2, start);
			case encoding::utf16_se: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr2, s2, start);
			case encoding::utf16_le: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr2, s2, start);
			case encoding::utf16_be: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr2, s2, start);
			case encoding::utf32: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr2, s2, start);
			case encoding::utf32_se: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr2, s2, start);
			case encoding::utf32_le: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr2, s2, start);
			case encoding::utf32_be: return encoder<ENCODING>::find_reverse((cstr_t)cstr1, s1, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr2, s2, start);
			default: return 0;
			}
		}


		template<encoding ENCODING> inline
		raw_str_t encoder_interface<ENCODING>::convert(unknown_str_t dest, unknown_cstr_t src, encoding_t e, bool set_eos, wsize max_out_size, wsize max_in_size)const {
			wsize i = 0, j = 0;
			switch (e.get())
			{
			case encoding::ascii: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::ascii>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::unicode: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::unicode>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf8: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf8>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf16: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf16_se: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf16_le: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf16_be: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf32: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf32_se: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf32_le: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf32_be: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			default: return raw_str();
			}
		}

		template<encoding ENCODING> inline
		raw_str_t encoder_interface<ENCODING>::convert(unknown_str_t dest, wsize& i, unknown_cstr_t src, wsize& j, encoding_t e, bool set_eos, wsize max_out_size, wsize max_in_size)const {
			switch (e.get())
			{
			case encoding::ascii: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::ascii>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::unicode: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::unicode>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf8: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf8>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf16: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf16_se: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf16_le: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf16_be: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf32: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf32_se: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf32_le: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			case encoding::utf32_be: encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)src, j, set_eos, max_out_size, max_in_size); return raw_str((str_t)dest, i, ENCODING);
			default: return raw_str();
			}
		}


	}
}