#pragma once

namespace ang
{
	namespace text
	{
		template<encoding E>
		class encoder_interface
			: public smart<encoder_interface<E>, iencoder>
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
			virtual wsize find(raw_cstr_t cstr1, raw_cstr_t cstr2, windex start)const override;
			virtual wsize find_any(raw_cstr_t cstr1, wsize start, array_view<const char32> tokens )const override;
			virtual wsize find_reverse(raw_cstr_t cstr1, raw_cstr_t cstr2, windex start)const override;
			virtual raw_str_t convert(raw_str_t dest, raw_cstr_t src, bool set_eos = true)const override;
			virtual raw_str_t convert(raw_str_t dest, wsize& i, raw_cstr_t src, wsize& j, bool set_eos = true)const override;

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
			raw_cstr_t raw(cstr, -1, e);
			switch (e.get())
			{
			case encoding::ascii: return encoder<E>::size(raw.to_cstr<encoding::ascii>(), start, end);
			case encoding::unicode:return encoder<E>::size(raw.to_cstr<encoding::unicode>(), start, end);
			case encoding::utf8: return encoder<E>::size(raw.to_cstr<encoding::utf8>(), start, end);
			case encoding::utf16: return encoder<E>::size(raw.to_cstr<encoding::utf16>(), start, end);
			case encoding::utf16_se: return encoder<E>::size(raw.to_cstr<encoding::utf16_se>(), start, end);
			case encoding::utf16_le: return encoder<E>::size(raw.to_cstr<encoding::utf16_le>(), start, end);
			case encoding::utf16_be: return encoder<E>::size(raw.to_cstr<encoding::utf16_be>(), start, end);
			case encoding::utf32: return encoder<E>::size(raw.to_cstr<encoding::utf32>(), start, end);
			case encoding::utf32_se: return encoder<E>::size(raw.to_cstr<encoding::utf32_se>(), start, end);
			case encoding::utf32_le: return encoder<E>::size(raw.to_cstr<encoding::utf32_le>(), start, end);
			case encoding::utf32_be: return encoder<E>::size(raw.to_cstr<encoding::utf32_be>(), start, end);
			default: return 0;
			}
		}

		template<encoding E> inline
		int encoder_interface<E>::compare(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const {
			cstr_t ptr = (cstr_t)cstr1;
			raw_cstr_t raw(cstr2, -1, e);
			switch (e.get())
			{
			case encoding::ascii: return encoder<E>::compare(ptr, raw.to_cstr<encoding::ascii>());
			case encoding::unicode:return encoder<E>::compare(ptr, raw.to_cstr<encoding::unicode>());
			case encoding::utf8: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf8>());
			case encoding::utf16: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf16>());
			case encoding::utf16_se: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf16_se>());
			case encoding::utf16_le: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf16_le>());
			case encoding::utf16_be: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf16_be>());
			case encoding::utf32: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf32>());
			case encoding::utf32_se: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf32_se>());
			case encoding::utf32_le: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf32_le>());
			case encoding::utf32_be: return encoder<E>::compare(ptr, raw.to_cstr<encoding::utf32_be>());
			default: return -10;
			}
		}

		template<encoding E> inline
		wsize encoder_interface<E>::compare_until(unknown_cstr_t cstr1, unknown_cstr_t cstr2, encoding_t e)const {
			cstr_t ptr = (cstr_t)cstr1;
			raw_cstr_t raw(cstr2, -1, e);
			switch (e.get())
			{
			case encoding::ascii: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::ascii>());
			case encoding::unicode:return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::unicode>());
			case encoding::utf8: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf8>());
			case encoding::utf16: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf16>());
			case encoding::utf16_se: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf16_se>());
			case encoding::utf16_le: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf16_le>());
			case encoding::utf16_be: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf16_be>());
			case encoding::utf32: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf32>());
			case encoding::utf32_se: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf32_se>());
			case encoding::utf32_le: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf32_le>());
			case encoding::utf32_be: return encoder<E>::compare_until(ptr, raw.to_cstr<encoding::utf32_be>());
			default: return 0;
			}
		}

		template<encoding E> inline
		wsize encoder_interface<E>::find(raw_cstr_t cstr1, raw_cstr_t cstr2, windex start)const {
			cstr_t ptr = (cstr_t)cstr1.ptr();
			wsize s1 = cstr1.size() / size_of<char_t>();
			switch (cstr2.encoding().get())
			{
			case encoding::ascii: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::ascii>(), cstr2.size() / size_of<char>(), start);
			case encoding::unicode:return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::unicode>(), cstr2.size() / size_of<wchar>(), start);
			case encoding::utf8: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf8>(), cstr2.size() / size_of<mchar>(), start);
			case encoding::utf16: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf16>(), cstr2.size() / size_of<char16>(), start);
			case encoding::utf16_se: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf16_se>(), cstr2.size() / size_of<char16>(), start);
			case encoding::utf16_le: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf16_le>(), cstr2.size() / size_of<char16>(), start);
			case encoding::utf16_be: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf16_be>(), cstr2.size() / size_of<char16>(), start);
			case encoding::utf32: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf32>(), cstr2.size() / size_of<char32>(), start);
			case encoding::utf32_se: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf32_se>(), cstr2.size() / size_of<char32>(), start);
			case encoding::utf32_le: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf32_le>(), cstr2.size() / size_of<char32>(), start);
			case encoding::utf32_be: return encoder<E>::find(ptr, s1, cstr2.to_cstr<encoding::utf32_be>(), cstr2.size() / size_of<char32>(), start);
			default: return 0;
			}
		}

		template<encoding E> inline
			wsize encoder_interface<E>::find_any(raw_cstr_t cstr1, wsize start, array_view<const char32> tokens)const {
			return encoder<E>::find_any((cstr_t)cstr1.ptr(), cstr1.size() / size_of<char_t>(), start, tokens);
		}

		template<encoding E> inline
		wsize encoder_interface<E>::find_reverse(raw_cstr_t cstr1, raw_cstr_t cstr2, windex start)const {
			cstr_t ptr = (cstr_t)cstr1.ptr();
			wsize s1 = cstr1.size() / size_of<char_t>();
			switch (cstr2.encoding().get())
			{
			case encoding::ascii: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::ascii>(), cstr2.size() / size_of<char>(), start);
			case encoding::unicode:return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::unicode>(), cstr2.size() / size_of<wchar>(), start);
			case encoding::utf8: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf8>(), cstr2.size() / size_of<mchar>(), start);
			case encoding::utf16: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf16>(), cstr2.size() / size_of<char16>(), start);
			case encoding::utf16_se: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf16_se>(), cstr2.size() / size_of<char16>(), start);
			case encoding::utf16_le: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf16_le>(), cstr2.size() / size_of<char16>(), start);
			case encoding::utf16_be: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf16_be>(), cstr2.size() / size_of<char16>(), start);
			case encoding::utf32: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf32>(), cstr2.size() / size_of<char32>(), start);
			case encoding::utf32_se: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf32_se>(), cstr2.size() / size_of<char32>(), start);
			case encoding::utf32_le: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf32_le>(), cstr2.size() / size_of<char32>(), start);
			case encoding::utf32_be: return encoder<E>::find_reverse(ptr, s1, cstr2.to_cstr<encoding::utf32_be>(), cstr2.size() / size_of<char32>(), start);
			default: return 0;
			}
		}


		template<encoding E> inline
		raw_str_t encoder_interface<E>::convert(raw_str_t dest, raw_cstr_t src, bool set_eos)const {
			wsize i = 0, j = 0;
			str_t ptr = (str_t)dest.ptr();
			wsize max_out_size = dest.size() / size_of<char_t>();
			switch (src.encoding().get())
			{
			case encoding::ascii: encoder<E>::convert(ptr, i, src.to_cstr<encoding::ascii>(), j, set_eos, max_out_size, src.size() / size_of<char>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::unicode: encoder<E>::convert(ptr, i, src.to_cstr<encoding::unicode>(), j, set_eos, max_out_size, src.size() / size_of<wchar>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf8: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf8>(), j, set_eos, max_out_size, src.size() / size_of<mchar>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf16: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf16>(), j, set_eos, max_out_size, src.size() / size_of<char16>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf16_se: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf16_se>(), j, set_eos, max_out_size, src.size() / size_of<char16>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf16_le: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf16_le>(), j, set_eos, max_out_size, src.size() / size_of<char16>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf16_be: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf16_be>(), j, set_eos, max_out_size, src.size() / size_of<char16>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf32: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf32>(), j, set_eos, max_out_size, src.size() / size_of<char32>()); return raw_str(ptr, i, E);
			case encoding::utf32_se: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf32_se>(), j, set_eos, max_out_size, src.size() / size_of<char32>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf32_le: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf32_le>(), j, set_eos, max_out_size, src.size() / size_of<char32>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf32_be: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf32_be>(), j, set_eos, max_out_size, src.size() / size_of<char32>()); return raw_str(ptr, i * size_of<char_t>(), E);
			default: return raw_str();
			}
		}

		template<encoding E> inline
		raw_str_t encoder_interface<E>::convert(raw_str_t dest, wsize& i, raw_cstr_t src, wsize& j, bool set_eos)const {
			str_t ptr = (str_t)dest.ptr();
			wsize max_out_size = dest.size() / size_of<char_t>();
			switch (src.encoding().get())
			{
			case encoding::ascii: encoder<E>::convert(ptr, i, src.to_cstr<encoding::ascii>(), j, set_eos, max_out_size, src.size() / size_of<char>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::unicode: encoder<E>::convert(ptr, i, src.to_cstr<encoding::unicode>(), j, set_eos, max_out_size, src.size() / size_of<wchar>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf8: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf8>(), j, set_eos, max_out_size, src.size() / size_of<mchar>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf16: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf16>(), j, set_eos, max_out_size, src.size() / size_of<char16>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf16_se: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf16_se>(), j, set_eos, max_out_size, src.size() / size_of<char16>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf16_le: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf16_le>(), j, set_eos, max_out_size, src.size() / size_of<char16>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf16_be: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf16_be>(), j, set_eos, max_out_size, src.size() / size_of<char16>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf32: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf32>(), j, set_eos, max_out_size, src.size() / size_of<char32>()); return raw_str(ptr, i, E);
			case encoding::utf32_se: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf32_se>(), j, set_eos, max_out_size, src.size() / size_of<char32>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf32_le: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf32_le>(), j, set_eos, max_out_size, src.size() / size_of<char32>()); return raw_str(ptr, i * size_of<char_t>(), E);
			case encoding::utf32_be: encoder<E>::convert(ptr, i, src.to_cstr<encoding::utf32_be>(), j, set_eos, max_out_size, src.size() / size_of<char32>()); return raw_str(ptr, i * size_of<char_t>(), E);
			default: return raw_str();
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}