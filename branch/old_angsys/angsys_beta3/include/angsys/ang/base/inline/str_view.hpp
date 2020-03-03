/*********************************************************************************************************************/
/*   File Name: ang/base/inline/str_view.hpp                                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#else

namespace ang //constants
{
	template<> 
	struct MY_LINKAGE str_view<MY_CHAR_TYPE, MY_ENCODING>
	{
		static constexpr text::encoding ENCODING = MY_ENCODING;
		typedef str_view<MY_CHAR_TYPE, ENCODING> sefl_t;
		typedef MY_CHAR_TYPE char_t;
		typedef MY_CHAR_TYPE* str_t;
		typedef MY_CHAR_TYPE const* cstr_t;

		str_view();
		str_view(ang::nullptr_t const&);
		str_view(str_t str, wsize sz = -1);
		template<wsize SIZE>
		str_view(char_t(&str)[SIZE])
			: str_view(str, SIZE - 1){
		}
		wsize size()const;
		str_t str();
		cstr_t cstr()const;
		void set(str_t str, wsize sz = -1);
		char_t* begin()const;
		char_t* end()const;
		sefl_t& operator = (sefl_t const& str);
		sefl_t& operator = (str_t str);
		template<wsize SIZE>
		sefl_t& operator = (char_t(&str)[SIZE]) {
			set(str, SIZE - 1);
			return*this;
		}
		char_t& operator*();
		char_t const& operator*()const;
		char_t** operator & ();
		operator str_t ();
		operator cstr_t ();
		template<typename index_t> char_t& operator[](index_t const& i) {
			return str()[i];
		}
		template<typename index_t> char_t const& operator[](index_t const& i)const {
			return cstr()[i];
		}
		template<typename index_t> sefl_t& operator += (index_t i) {
			set(str() + (wsize)i, size() - (wsize)i);
			return this;
		}
		sefl_t& operator ++ ();
		sefl_t operator ++ (int);
		friend sefl_t operator + (sefl_t const& str, wsize i);

	private:
		str_t m_view;
		wsize m_size;
	};

	template<> 
	struct MY_LINKAGE str_view<const MY_CHAR_TYPE, MY_ENCODING>
	{
		static constexpr text::encoding ENCODING = MY_ENCODING;
		typedef str_view<const MY_CHAR_TYPE, ENCODING> sefl_t;
		typedef typename text::char_type_by_encoding<ENCODING>::char_t char_t;
		typedef typename text::char_type_by_encoding<ENCODING>::cstr_t cstr_t;

		str_view();
		str_view(ang::nullptr_t const&);
		str_view(cstr_t str, wsize sz = -1);
		template<wsize SIZE> str_view(const char_t(&str)[SIZE]) 
			: str_view(str, SIZE - 1)
		{
		}
		wsize size()const;
		cstr_t cstr()const;
		char_t const* begin()const;
		char_t const* end()const;
		void set(cstr_t str, wsize sz = -1);
		sefl_t& operator = (sefl_t const& str);
		sefl_t& operator = (cstr_t str);
		template<wsize SIZE> sefl_t& operator = (const char_t(&str)[SIZE]) {
			set(str, SIZE - 1);
			return*this;
		}
		char_t const& operator*()const;
		char_t const** operator & ();
		operator cstr_t ();
		template<typename index_t> char_t const& operator[](index_t const& i)const {
			return cstr()[i];
		}
		template<typename index_t> sefl_t& operator += (index_t i) {
			set(cstr() + (wsize)i, size() - (wsize)i);
			return *this;
		}
		sefl_t& operator ++ ();
		sefl_t operator ++ (int);
		friend sefl_t operator + (sefl_t const& str, wsize i);

	private:
		cstr_t m_view;
		wsize m_size;
	};


}


#endif//__ANG_BASE_H__
