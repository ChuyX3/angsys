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
			virtual wsize lenght(unknown_cstr_t cstr)const override;
			virtual wsize size(unknown_cstr_t cstr, encoding_t e)const override;
			virtual int compare(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const override;
			virtual wsize compare_until(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const override;
			virtual raw_str_t convert(unknown_str_t dest, unknown_cstr_t src, encoding_t e, bool set_eos, wsize max_size)const override;
			virtual raw_str_t convert(unknown_str_t dest, wsize& i, unknown_cstr_t src, wsize& j, encoding_t e, bool set_eos, wsize max_size)const override;

		private:
			virtual~encoder_interface();
		};



		template<encoding ENCODING> inline
		encoder_interface<ENCODING>::encoder_interface() {
		}

		template<encoding ENCODING> inline
		encoder_interface<ENCODING>::~encoder_interface() {
		}


		template<encoding ENCODING> inline
		encoding_t encoder_interface<ENCODING>::format()const {
			return ENCODING;
		}

		template<encoding ENCODING> inline
		wsize encoder_interface<ENCODING>::lenght(unknown_cstr_t cstr)const {
			return encoder<ENCODING>::lenght((cstr_t)cstr);
		}

		template<encoding ENCODING> inline
		wsize encoder_interface<ENCODING>::size(unknown_cstr_t cstr, encoding_t e)const {
			switch (e.get())
			{
			case encoding::ascii: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::ascii>::cstr_t)cstr);
			case encoding::unicode:return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::unicode>::cstr_t)cstr);
			case encoding::utf8: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf8>::cstr_t)cstr);
			case encoding::utf16: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf16>::cstr_t)cstr);
			case encoding::utf16_se: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf16_se>::cstr_t)cstr);
			case encoding::utf16_le: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf16_le>::cstr_t)cstr);
			case encoding::utf16_be: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf16_be>::cstr_t)cstr);
			case encoding::utf32: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf32>::cstr_t)cstr);
			case encoding::utf32_se: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf32_se>::cstr_t)cstr);
			case encoding::utf32_le: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf32_le>::cstr_t)cstr);
			case encoding::utf32_be: return encoder<ENCODING>::size((typename char_type_by_encoding<encoding::utf32_be>::cstr_t)cstr);
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
		raw_str_t encoder_interface<ENCODING>::convert(unknown_str_t dest, unknown_cstr_t src, encoding_t e, bool set_eos, wsize max_size)const {
			wsize i = 0, j = 0;
			switch (e.get())
			{
			case encoding::ascii: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::ascii>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::unicode:return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::unicode>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf8: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf8>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf16: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf16_se: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf16_le: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf16_be: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf32: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf32_se: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf32_le: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf32_be: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			default: return raw_str();
			}
		}

		template<encoding ENCODING> inline
		raw_str_t encoder_interface<ENCODING>::convert(unknown_str_t dest, wsize& i, unknown_cstr_t src, wsize& j, encoding_t e, bool set_eos, wsize max_size)const {
			switch (e.get())
			{
			case encoding::ascii: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::ascii>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::unicode:return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::unicode>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf8: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf8>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf16: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf16_se: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_se>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf16_le: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_le>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf16_be: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf16_be>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf32: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf32_se: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_se>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf32_le: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_le>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			case encoding::utf32_be: return raw_str(encoder<ENCODING>::convert((str_t)dest, i, (typename char_type_by_encoding<encoding::utf32_be>::cstr_t)src, j, set_eos, max_size), i, ENCODING);
			default: return raw_str();
			}
		}


	}
}