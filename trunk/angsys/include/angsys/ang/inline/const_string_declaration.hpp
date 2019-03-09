#ifndef __CONST_STRING_H__
#error ...
#else

namespace ang
{
	namespace text
	{
		template<> class MY_LINKAGE basic_const_string_buffer<MY_ENCODING>
			: public smart<basic_const_string_buffer<MY_ENCODING>, basic_const_string_buffer_base>
		{
		public:
			static const text::encoding ENCODING = MY_ENCODING;
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t, type;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t unsafe_cstr_t;
			typedef str_view<char_t const, MY_ENCODING> cstr_t;

		public:
			template<typename T, wsize N> pointer operator new(wsize sz, const T(&ar)[N]) { return basic_const_string_buffer_base::operator new(sz, ENCODING, str_view<const T>(ar)); }
			template<typename T, text::encoding E> pointer operator new(wsize sz, str_view<T, E> const& str) { return basic_const_string_buffer_base::operator new(sz, ENCODING, (raw_cstr)str); }
#ifdef WINDOWS_PLATFORM
			template<typename T, text::encoding E> void operator delete(pointer ptr, raw_cstr_t str) { return basic_const_string_buffer_base::operator delete(ptr, ENCODING, (raw_cstr)str); }
#elif defined ANDROID_PLATFORM
			inline void operator delete(pointer ptr) { basic_const_string_buffer_base::operator delete(ptr); }
#endif

			basic_const_string_buffer();

			ANG_DECLARE_INTERFACE();

			virtual comparision_result_t compare(object const* obj)const override;
			virtual text::encoding_t encoding()const override;
			virtual const_pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual raw_str_t str(int);
			virtual raw_cstr_t cstr(int)const;

			inline cstr_t cstr()const { return cstr(0).to_cstr<MY_ENCODING>(); }

		private:
			virtual~basic_const_string_buffer();
		};
	}
}

#endif//__INTF_WRAPPER_HPP__