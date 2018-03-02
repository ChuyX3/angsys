#ifndef __STRING_H__
#error ...
#else

namespace ang
{
	/*ANG_BEGIN_OBJECT_WRAPPER(MY_LINKAGE, strings::basic_string_buffer<MY_ENCODING COMA MY_ALLOCATOR>)
		visible scall constexpr text::encoding ENCODING = MY_ENCODING;
		template<typename T> object_wrapper(safe_str<T> const& str);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper(object_wrapper<strings::string_buffer<OTHER_ENCODING>> const& str);
		template<typename T, wsize N> object_wrapper(const T(&ar)[N]);

		template<typename T> object_wrapper& operator = (safe_str<T> const& str);
		template<typename T, wsize N> object_wrapper& operator = (const T(&ar)[N]);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper& operator = (object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);
		template<typename T> object_wrapper& operator += (safe_str<T> const& str);
		template<typename T, wsize N> object_wrapper& operator += (const T(&ar)[N]);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper& operator += (object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);

		operator str_view<typename text::char_type_by_encoding<ENCODING>::char_t>(void);
		operator cstr_view<typename text::char_type_by_encoding<ENCODING>::char_t>(void)const;
		explicit operator raw_str_t (void)const;
		typename text::char_type_by_encoding<ENCODING>::char_t& operator [] (int);
		typename text::char_type_by_encoding<ENCODING>::char_t operator [] (int)const;
		template<text::encoding_enum OTHER_ENCODING>

		friend object_wrapper<strings::string_buffer<ENCODING>> operator +
			(object_wrapper<strings::string_buffer<ENCODING>> const&, object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);

		template<typename T> friend object_wrapper<strings::string_buffer<ENCODING>> operator +
			(object_wrapper<strings::string_buffer<ENCODING>> const&, safe_str<T> const&);

		template<typename T> friend object_wrapper<strings::string_buffer<ENCODING>> operator +
			(safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING>> const&);

		template<text::encoding_enum OTHER_ENCODING> friend object_wrapper<strings::string_buffer<ENCODING>>& operator <<
			(object_wrapper<strings::string_buffer<ENCODING>>&, object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);

		template<typename T> friend object_wrapper<strings::string_buffer<ENCODING>>& operator <<
			(object_wrapper<strings::string_buffer<ENCODING>>&, safe_str<T> const&);
	ANG_END_OBJECT_WRAPPER();*/

	namespace strings
	{
		template<> class MY_LINKAGE basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>
			: public basic_string_buffer_base
		{
		public:
			static const text::encoding ENCODING = MY_ENCODING;
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t, type;
			typedef typename char_type_by_encoding<ENCODING>::str_t unsafe_str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t unsafe_cstr_t;
			typedef MY_ALLOCATOR<type> allocator_t;
			typedef text::encoder<MY_ENCODING> encoder;

			static constexpr wsize CAPACITY = basic_string_buffer_base::RAW_CAPACITY / sizeof(char_t);
			typedef str_view<char_t> str_t;
			typedef str_view<char_t const> cstr_t;
			typedef basic_string_buffer<MY_ENCODING, MY_ALLOCATOR> string;

		private:
			allocator_t alloc;

		public:
			basic_string_buffer();

		public: //overides
			ANG_DECLARE_INTERFACE();

		public:
			virtual void clean()override;
			virtual bool realloc(wsize new_size, bool save = true)override;

		public:
			void copy(raw_cstr_t);
			void concat(raw_cstr_t);

			int compare(raw_cstr_t)const;
			windex compare_until(raw_cstr_t)const;
			windex find(raw_cstr_t, windex start = 0, windex end = -1)const;
			windex find_revert(raw_cstr_t, windex start = -1, windex end = 0)const;
			wsize sub_string(raw_str_t, windex start, windex end)const;

		private:
			virtual~basic_string_buffer();
		};
	}
}

#endif//__INTF_WRAPPER_HPP__