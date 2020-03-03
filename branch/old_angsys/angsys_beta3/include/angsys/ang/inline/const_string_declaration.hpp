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

			pointer operator new(wsize sz, raw_cstr_t cstr) {
				return basic_const_string_buffer_base::operator new(sz, MY_ENCODING, cstr);
			}
#ifdef WINDOWS_PLATFORM
			void operator delete(pointer ptr, raw_cstr_t cstr) {
				basic_const_string_buffer_base::operator delete(ptr, MY_ENCODING, cstr);
			}
#elif defined ANDROID_PLATFORM
			void operator delete(pointer ptr) {
				basic_const_string_buffer_base::operator delete(ptr, MY_ENCODING, raw_cstr_t());
			}
#endif // WINDOWS_PLATFORM

		public:
			basic_const_string_buffer();

			ANG_DECLARE_INTERFACE();

			virtual comparision_result_t compare(object const* obj)const override;
			virtual text::encoding_t encoding()const override;
			virtual const_pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual raw_str_t str(int);
			virtual raw_cstr_t cstr(int)const;

			inline cstr_t cstr()const { return cstr(0).cstr<MY_ENCODING>(); }

		private:
			virtual~basic_const_string_buffer();
		};
	}
}

#endif//__INTF_WRAPPER_HPP__