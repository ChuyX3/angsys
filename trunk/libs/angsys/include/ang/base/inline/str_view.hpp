/*********************************************************************************************************************/
/*   File Name: ang/base/inline/str_view.hpp                                                                                     */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C)  angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#else

namespace ang
{
	namespace text
	{
		template<>
		struct MY_LINKAGE str_view<MY_CHAR_TYPE, MY_ENCODING>
		{
			static constexpr encoding ENCODING = MY_ENCODING;
			using self = str_view<MY_CHAR_TYPE, MY_ENCODING>;
			using char_t = MY_CHAR_TYPE;
			using str_t = char_t * ;
			using cstr_t = char_t const*;

			str_view();
			str_view(ang::nullptr_t const&);
			str_view(str_t str, wsize sz);
			template<wsize SIZE>
			str_view(char_t(&str)[SIZE])
				: str_view(str, SIZE - 1) {
			}
			bool is_empty()const;
			wsize size()const;
			str_t str();
			cstr_t cstr()const;
			void set(str_t str, wsize sz = -1);
			char_t* begin()const;
			char_t* end()const;
			self& operator = (self const& str);
			self& operator = (str_t str);
			template<wsize SIZE>
			self& operator = (char_t(&str)[SIZE]) {
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
			template<typename index_t> self& operator += (index_t i) {
				set(str() + (wsize)i, size() - (wsize)i);
				return *this;
			}
			self& operator ++ ();
			self operator ++ (int);
			friend self operator + (self const& str, wsize i);

		private:
			str_t m_view;
			wsize m_size;
		};

		template<>
		struct MY_LINKAGE str_view<const MY_CHAR_TYPE, MY_ENCODING>
		{
			static constexpr encoding ENCODING = MY_ENCODING;
			using self = str_view<const MY_CHAR_TYPE, MY_ENCODING>;
			using char_t = MY_CHAR_TYPE;
			using cstr_t = char_t const*;

			str_view();
			str_view(ang::nullptr_t const&);
			str_view(cstr_t str, wsize sz);
			template<wsize SIZE> str_view(const char_t(&str)[SIZE])
				: str_view(str, SIZE - 1)
			{
			}
			bool is_empty()const;
			wsize size()const;
			cstr_t cstr()const;
			char_t const* begin()const;
			char_t const* end()const;
			void set(cstr_t str, wsize sz = -1);
			self& operator = (self const& str);
			self& operator = (cstr_t str);
			template<wsize SIZE> self& operator = (const char_t(&str)[SIZE]) {
				set(str, SIZE - 1);
				return*this;
			}
			char_t const& operator*()const;
			char_t const** operator & ();
			operator cstr_t ();
			template<typename index_t> char_t const& operator[](index_t const& i)const {
				return cstr()[i];
			}
			template<typename index_t> self& operator += (index_t i) {
				set(cstr() + (wsize)i, size() - (wsize)i);
				return *this;
			}
			self& operator ++ ();
			self operator ++ (int);
			friend self operator + (self const& str, wsize i);

		private:
			cstr_t m_view;
			wsize m_size;
		};

	}
}


#endif//__ANG_BASE_H__
