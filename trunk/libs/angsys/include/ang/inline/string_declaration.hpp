/*********************************************************************************************************************/
/*   File Name: ang/inline/string_declaration.hpp                                                                    */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_STRING_H__
#error ...
#else

namespace ang
{
	namespace text
	{
		template<>
		class MY_LINKAGE basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>
			: public implement<basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>
			, iid("ang::text::string")
			, ivariable
			, istring
			, ibuffer
			, ibuffer_view>
			, public fast_string<MY_ENCODING, MY_ALLOCATOR>
		{
		public:
			static constexpr text::encoding ENCODING = MY_ENCODING;
			typedef text::text_encoder<ENCODING> encoder;
			typedef fast_string<MY_ENCODING, MY_ALLOCATOR> fast_string_t;
			typedef typename char_type_by_encoding<ENCODING>::char_type type;
			typedef typename char_type_by_encoding<ENCODING>::str_type ptr_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_type cptr_t;
			typedef str_view<type, ENCODING> view_t;
			typedef str_view<const type, ENCODING> cview_t;

		public: /*constructors*/
			basic_string_buffer();
			basic_string_buffer(cstr_t);
			basic_string_buffer(basic_string_buffer &&);
			basic_string_buffer(fast_string_t &&);

			template<typename T, text::encoding E>
			basic_string_buffer(str_view<T, E> const& cstr) {
				fast_string::copy(cstr);
			}

			template<text::encoding E, template<typename>class A>
			basic_string_buffer(basic_string_buffer<E, A> const& cstr) {
				fast_string::copy(cstr.cstr());
			}

			template<text::encoding E, template<typename>class A>
			basic_string_buffer(fast_string<E, A> const& cstr) {
				fast_string::copy(cstr.cstr());
			}

		public: /*overrides*/
			virtual void dispose()override;
			virtual string to_string()const override;
			virtual string to_string(text::text_format_t)const override;

		private: /*ivariable*/
			virtual rtti_t const& value_type()const override;
			virtual bool set_value(rtti_t const&, unknown_t)override;
			virtual bool get_value(rtti_t const&, unknown_t)const override;
			virtual variant clone()const override;
			//virtual string to_string()const override;
			//virtual string to_string(text::text_format_t)const override;

		public: /*istring overrides*/
			virtual encoding_t encoding()const override;
			virtual wsize length()const override;
			virtual pointer data()const override;
			virtual rtti_t const& char_type()const override;
			virtual str_t str(int = 0)override;
			virtual cstr_t cstr(int = 0)const override;
			virtual char32_t at(windex i, wsize* = null)const override;
			virtual int compare(cstr_t)const override;
			virtual int compare(string)const override;
			virtual windex compare_until(cstr_t)const override;
			virtual windex compare_until(string)const override;
			virtual windex find(cstr_t, windex start = 0, windex end = -1)const override;
			virtual windex find(string, windex start = 0, windex end = -1)const override;
			virtual windex find_reverse(cstr_t, windex start = -1, windex end = 0)const override;
			virtual windex find_reverse(string, windex start = -1, windex end = 0)const override;
			virtual string sub_string(windex start, windex end)const override;
			virtual vector<string> split(cstr_t)const override;
			virtual vector<string> split(string)const override;
			virtual void clear()override;
			virtual wsize copy(cstr_t)override;
			virtual wsize copy(string)override;
			virtual wsize append(cstr_t)override;
			virtual wsize append(string)override;
			virtual wsize insert(cstr_t, windex = -1)override;
			virtual wsize insert(string, windex = -1)override;
			virtual void format(cstr_t, var_args_t)override;
			virtual void format(string, var_args_t)override;
			virtual void append_format(cstr_t, var_args_t)override;
			virtual void append_format(string, var_args_t)override;
			//virtual void insert_format(windex, cstr_t, var_args_t)override;
		
		public: /*ibuffer overrides*/
			virtual bool is_readonly()const override;
			virtual pointer buffer_ptr() override;
			virtual const_pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer, text::encoding_t = text::encoding::utf8) override;
			virtual ibuffer_view_t map_buffer(windex, wsize) override;
			virtual bool unmap_buffer(ibuffer_view_t&, wsize used) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;

		/*private:
			using fast_string::append;
			using fast_string::append_format;*/

		private: /*destructor*/
			virtual ~basic_string_buffer();

		};
	}

	ANG_DECLARE_CLASS_INFO_OVERRIDE(MY_LINKAGE, ang::text::basic_string_buffer<MY_ENCODING COMA MY_ALLOCATOR>)
}

#endif//__INTF_WRAPPER_HPP__