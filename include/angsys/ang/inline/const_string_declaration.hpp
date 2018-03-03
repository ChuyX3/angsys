#ifndef __STRING_H__
#error ...
#else

namespace ang
{
	namespace strings
	{
		template<> class MY_LINKAGE const_string_buffer<MY_ENCODING>
			: public const_string_buffer_base
		{
		public:
			static const text::encoding ENCODING = MY_ENCODING;
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t, type;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t unsafe_cstr_t;
			typedef str_view<char_t const> cstr_t;

		public:
			template<typename T, text::encoding E> pointer operator new(wsize sz, str_view<T, E> const& str) { return operator new(sz, ENCODING, (raw_cstr)str); }
			template<typename T, text::encoding E> void operator delete(pointer ptr, raw_cstr_t str) { return operator delete(ptr, ENCODING, (raw_cstr)str); }

			const_string_buffer();

			ANG_DECLARE_INTERFACE();

			virtual comparision_result_t compare(object const* obj)const override;
			virtual text::encoding_t encoding()const override;
			virtual const_pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual raw_str_t text_buffer()override;
			virtual raw_cstr_t text_buffer()const override;

		private:
			virtual~const_string_buffer();
		};
	}
}

#endif//__INTF_WRAPPER_HPP__