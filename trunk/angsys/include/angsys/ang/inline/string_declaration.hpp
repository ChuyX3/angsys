#ifndef __STRING_H__
#error ...
#else

namespace ang
{
	namespace strings
	{
		template<> class MY_LINKAGE basic_string_buffer<MY_ENCODING , MY_ALLOCATOR>
			: public basic_string_buffer_base
		{
		public:
			static constexpr text::encoding ENCODING = MY_ENCODING;
			typedef typename char_type_by_encoding<MY_ENCODING>::char_t char_t, type;
			typedef typename char_type_by_encoding<MY_ENCODING>::str_t unsafe_str_t;
			typedef typename char_type_by_encoding<MY_ENCODING>::cstr_t unsafe_cstr_t;
			typedef MY_ALLOCATOR<char_t> allocator_t;
			typedef text::encoder<ENCODING> encoder;

			static const wsize CAPACITY = RAW_CAPACITY / sizeof(char_t);
			typedef str_view<char_t, MY_ENCODING> str_t;
			typedef cstr_view<char_t, MY_ENCODING> cstr_t;
			typedef basic_string_buffer_base string_base, base_t;
			typedef basic_string_buffer<ENCODING, memory::buffer_allocator> string, self_t;
			typedef object_wrapper<string> string_t;

		public:
			basic_string_buffer();
			basic_string_buffer(wsize reserv);
			basic_string_buffer(raw_cstr_t const& str);
			basic_string_buffer(basic_string_buffer const* str);
			basic_string_buffer(basic_const_string_buffer<MY_ENCODING>* str);

			template<typename T> basic_string_buffer(str_view<T> const& str)
				: basic_string_buffer(raw_cstr(str)) {
			}
		public: //overides
			ANG_DECLARE_INTERFACE();

			virtual void clear()override;
			virtual bool realloc(wsize new_size, bool save = true)override;

		private: //ivariant override
			virtual variant clone()const override;

		public:		
			str_t str();
			cstr_t cstr()const;

		private:	
			virtual bool is_readonly(void) const override;
			raw_str_t str(int)override;
			raw_cstr_t cstr(int)const override;

		public:
			using string_base::sub_string;
			template<text::encoding E, template<typename>class _alloc> basic_string<E>& sub_string(basic_string<E, _alloc>& str, windex start = 0, windex end = -1)const {
				if (str.is_empty())str = new basic_string_buffer<E, _alloc>();
				string_base::sub_string(str.get(), start, end);
				return str;
			}
			string_t sub_string(windex start = 0, windex end = -1)const;	

			template<typename...Ts>
			void format(raw_cstr_t f, Ts... args) {
				string_base::format(f, var_args_t({ ang::forward<Ts>(args)... }));
			}

		private:
			virtual~basic_string_buffer();
		};
	}


	ANG_BEGIN_OBJECT_WRAPPER(MY_LINKAGE, strings::basic_string_buffer<MY_ENCODING COMA MY_ALLOCATOR>)
		visible scall const text::encoding ENCODING = MY_ENCODING;
		template<typename T, typename...Ts> 
		inline static ang::strings::basic_string<MY_ENCODING, MY_ALLOCATOR> format(ang::str_view<T, MY_ENCODING> f, Ts... args) {
			ang::strings::basic_string<MY_ENCODING, MY_ALLOCATOR> str;
			str->format(f, ang::forward<Ts>(args)...);
			return str;
		}
		object_wrapper(strings::basic_const_string_buffer<MY_ENCODING>* str) : object_wrapper() {
			set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str));
		}
		object_wrapper(raw_cstr_t const& str) : object_wrapper() {
			set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str));
		}
		template<typename T, text::encoding E> 
		object_wrapper(str_view<T, E> const& str) : object_wrapper() {
			set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str));
		}
		template<text::encoding E>
		object_wrapper(object_wrapper<strings::basic_string_buffer<E>> const& str) : object_wrapper() {
			if(str) set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str->cstr()));
		}
		template<typename T, wsize N> 
		object_wrapper(const T(&ar)[N]) : object_wrapper() {
			set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(cstr_view<T>(ar)));
		}	

		object_wrapper& operator = (strings::basic_const_string_buffer<MY_ENCODING>* str) {
			if (is_empty()) set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str));
			else get()->set(str);
			return *this;
		}
		template<typename T, text::encoding E>
		object_wrapper& operator = (str_view<T, E> const& str) {
			if(is_empty()) set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str));
			else get()->copy(str);
			return *this;
		}
		template<typename T, wsize N> 
		object_wrapper& operator = (const T(&ar)[N]) {
			if (is_empty()) set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(cstr_view<T>(ar)));
			else get()->copy(cstr_view<T>(ar));
			return *this;
		}
		template<text::encoding E> object_wrapper& operator = (object_wrapper<strings::basic_string_buffer<E>> const& str) {
			if (is_empty()) set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str->cstr()));
			else get()->copy(str->cstr());
			return *this;
		}
		template<typename T, text::encoding E> 
		object_wrapper& operator += (str_view<T, E> const& str) {
			if (is_empty()) set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str));
			else get()->concat(str);
			return *this;
		}
		template<typename T, wsize N>
		object_wrapper& operator += (const T(&ar)[N]) {
			if (is_empty()) set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(cstr_view<T>(ar)));
			else get()->concat(cstr_view<T>(ar));
			return *this;
		}
		template<text::encoding E>
		object_wrapper& operator += (object_wrapper<strings::basic_string_buffer<E>> const& str) {
			if (is_empty()) set(new strings::basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(str->cstr()));
			else get()->concat(str->cstr());
			return *this;
		}

		operator str_view<typename text::char_type_by_encoding<ENCODING>::char_t, ENCODING>(void) { return get()->str(); }
		operator cstr_view<typename text::char_type_by_encoding<ENCODING>::char_t, ENCODING>(void)const { return get()->cstr(); }
		explicit operator raw_str_t (void)const { return get()->str(); }
		explicit operator raw_cstr_t (void)const { return get()->cstr(); }
		template<typename I>typename text::char_type_by_encoding<ENCODING>::char_t& operator [](I idx) { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return get()->str()[idx]; }
		template<typename I>typename text::char_type_by_encoding<ENCODING>::char_t const& operator [](I idx)const { static_assert(is_integer_value<I>::value, "no integer value is no accepted"); return get()->cstr()[idx]; }
		template<text::encoding E> 
		friend object_wrapper<strings::basic_string_buffer<ENCODING>> operator + 
			(object_wrapper<strings::basic_string_buffer<ENCODING>> const& str1, object_wrapper<strings::basic_string_buffer<E>> const& str2) {
			object_wrapper<strings::basic_string_buffer<ENCODING>> out = str1;
			out += str2;
			return ang::move(out);
		}

		template<typename T, text::encoding E> friend object_wrapper<strings::basic_string_buffer<ENCODING>> operator +
			(object_wrapper<strings::basic_string_buffer<ENCODING>> const& str1, str_view<T, E> const& str2) {
			object_wrapper<strings::basic_string_buffer<ENCODING>> out = str1;
			out += str2;
			return ang::move(out);
		}

		template<typename T, text::encoding E> friend object_wrapper<strings::basic_string_buffer<ENCODING>> operator +
			(str_view<T, E> const& str1, object_wrapper<strings::basic_string_buffer<ENCODING>> const& str2) {
			object_wrapper<strings::basic_string_buffer<ENCODING>> out = str1;
			out += str2;
			return ang::move(out);
		}

		template<text::encoding E> friend object_wrapper<strings::basic_string_buffer<ENCODING>>& operator <<
			(object_wrapper<strings::basic_string_buffer<ENCODING>>& str1, object_wrapper<strings::basic_string_buffer<E>> const& str2) {
			return str1 += str2;
		}

		template<typename T, text::encoding E> friend object_wrapper<strings::basic_string_buffer<ENCODING>>& operator <<
			(object_wrapper<strings::basic_string_buffer<ENCODING>>& str1, str_view<T, E> const& str2) {
			return str1 += str2;	
		}
	ANG_END_OBJECT_WRAPPER();

}

#endif//__INTF_WRAPPER_HPP__