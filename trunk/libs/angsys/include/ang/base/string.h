/*********************************************************************************************************************/
/*   File Name: ang/base/string.h                                                                                 */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, 2019.                                                   */
/*                                                                                                                   */
/*   Copyright (C) ang sys, Jesus Angel Rocha Morales                                                              */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_STRING_H__
#define __ANG_BASE_STRING_H__

namespace ang
{
	namespace text
	{
		
		template<encoding E, template<typename> class A>
		class fast_string
		{
		public:
			static constexpr wsize CAPACITY = 32u;
			static constexpr text::encoding ENCODING = E;
			typedef text_encoder<ENCODING> encoder;
			typedef typename char_type_by_encoding<ENCODING>::char_type type;
			typedef typename char_type_by_encoding<ENCODING>::str_type ptr_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_type cptr_t;
			typedef str_view<type, ENCODING> view_t;
			typedef str_view<const type, ENCODING> cview_t;

		private: /*fields*/
			typedef enum : wint {
				storage_type_stack = 0,
				storage_type_allocated = wint(-1)
			}storage_type_t;

			union str_data_t {
				storage_type_t m_storage_type;
				struct {
					wsize _unused;
					wsize m_allocated_length;
					wsize m_allocated_capacity;
					ptr_t m_allocated_buffer;
				};
				struct {
					wsize m_stack_length;
					type m_stack_buffer[CAPACITY];
				};
			};

			str_data_t m_data;

		public: /*constructors and destrucot*/
			fast_string();
			fast_string(fast_string && value);
			fast_string(fast_string const& value);
			template<encoding E2, template<typename> class A2>
			fast_string(fast_string<E2, A2> const& value);
			fast_string(ang::nullptr_t const&);
			template<typename T2, encoding E2>
			fast_string(str_view<const T2, E2> const& value);
			template<typename T2, wsize N>
			fast_string(T2 const(&value)[N]);
			~fast_string();

		public: /*operators*/
			fast_string& operator = (fast_string && value);
			fast_string& operator = (fast_string const& value);
			fast_string& operator = (ang::nullptr_t const&);
			template<typename T2, encoding E2>
			fast_string& operator = (str_view<const T2, E2> const& value);
			template<typename T2, wsize N>
			fast_string& operator = (T2 const(&value)[N]);
			template<encoding E2, template<typename> class A2 >
			fast_string& operator += (fast_string<E2, A2> const& value);
			template<typename T2, encoding E2>
			fast_string& operator += (str_view<const T2, E2> const& value);

			operator view_t();
			operator cview_t()const;
			operator raw_str_t();
			operator raw_cstr_t()const;

		public: /*properties*/
			bool is_empty()const;
			void clear();
			wsize length()const;
			void length(wsize len);
			wsize capacity() const;
			view_t str();
			cview_t cstr()const;
			cview_t to_string()const;
			type& at(windex i);
			type const& at(windex i)const;

		public: /*iterators*/
			type* begin() { return str().begin(); }
			type const* begin()const { return cstr().begin(); }
			type* end() { return str().end(); }
			type const* end()const { return cstr().end(); }

		public: /*utilities*/
			void move(fast_string& value);
			void realloc(wsize new_size, bool save = true);
			fast_string sub_string(windex start, windex end)const;
			fast_string& sub_string(fast_string& out, windex start, windex end)const;
			template<typename T2, encoding E2>
			int compare(str_view<const T2, E2> const& value)const;
			template<encoding E2, template<typename>class A2>
			int compare(fast_string<E2, A2> const& value)const;
			template<typename T2, encoding E2>
			windex compare_until(str_view<const T2, E2> const& value)const;
			template<encoding E2, template<typename>class A2>
			windex compare_until(fast_string<E2, A2> const& value)const;
			template<typename T2, encoding E2>
			windex find(str_view<const T2, E2> const& value, windex start = 0, windex end = -1)const;
			template<encoding E2, template<typename>class A2>
			windex find(fast_string<E2, A2> const& value, windex start = 0, windex end = -1)const;
			template<typename T2, encoding E2> 
			windex find_reverse(str_view<const T2, E2> const& value, windex start = -1, windex end = 0)const;
			template<encoding E2, template<typename>class A2>
			windex find_reverse(fast_string<E2, A2> const& value, windex start = -1, windex end = 0)const;
			template<typename T2, encoding E2>
			vector<fast_string> split(str_view<const T2, E2> const& value)const;
			template<encoding E2, template<typename>class A2>
			vector<fast_string> split(fast_string<E2, A2> const& value)const;
			template<typename T2, encoding E2>
			wsize copy(str_view<const T2, E2> const& value);
			template<encoding E2, template<typename>class A2>
			wsize copy(fast_string<E2, A2> const& value);
			template<typename T2, encoding E2>
			wsize append(str_view<const T2, E2> const& value);
			template<encoding E2, template<typename>class A2> 
			wsize append(fast_string<E2, A2> const& value);
			template<typename T2, encoding E2>
			wsize insert(str_view<const T2, E2> const& value, windex i = -1);
			template<encoding E2, template<typename>class A2>
			wsize insert(fast_string<E2, A2> const& value, windex i = -1);
			template<typename T2, encoding E2>
			wsize replace(str_view<const T2, E2> const& value, windex beg, wsize end);
			template<encoding E2, template<typename>class A2>
			wsize replace(fast_string<E2, A2> const& value, windex beg, wsize end);

			template<typename T2, encoding E2, typename...Ts>
			wsize format(str_view<const T2, E2> const& value, Ts&&...);
			template<encoding E2, template<typename>class A2, typename...Ts>
			wsize format(fast_string<E2, A2> const& value, Ts&&...);

			template<typename T2, encoding E2, typename...Ts>
			wsize append_format(str_view<const T2, E2> const& value, Ts&&...);
			template<encoding E2, template<typename>class A2, typename...Ts>
			wsize append_format(fast_string<E2, A2> const& value, Ts&&...);

		public: /*friends*/

			template<encoding E2, template<typename> class A2 > friend bool operator == (const fast_string& first, const fast_string<E2, A2>& second) { return first.compare(second.cstr()) == 0; }
			template<encoding E2, template<typename> class A2 > friend bool operator != (const fast_string& first, const fast_string<E2, A2>& second) { return first.compare(second.cstr()) != 0; }
			template<encoding E2, template<typename> class A2 > friend bool operator >= (const fast_string& first, const fast_string<E2, A2>& second) { return first.compare(second.cstr()) >= 0; }
			template<encoding E2, template<typename> class A2 > friend bool operator <= (const fast_string& first, const fast_string<E2, A2>& second) { return first.compare(second.cstr()) <= 0; }
			template<encoding E2, template<typename> class A2 > friend bool operator > (const fast_string& first, const fast_string<E2, A2>& second) { return first.compare(second.cstr()) > 0; }
			template<encoding E2, template<typename> class A2 > friend bool operator < (const fast_string& first, const fast_string<E2, A2>& second) { return first.compare(second.cstr()) < 0; }

			template<typename T2, encoding E2>
			friend fast_string& operator << (fast_string& stream, str_view<const T2, E2> const& value) {
				stream.append(value);
				return stream;
			}

			template<encoding E2, template<typename>class A2>
			friend fast_string& operator << (fast_string& stream, fast_string<E2, A2> const& value) {
				stream.append(value);
				return stream;
			}
		};


		template<typename T1, text::encoding E1, text::encoding E2, template<typename>class A2>
		struct str_view_compare_helper<str_view<T1, E1>, fast_string<E2, A2>>
		{
			using T2 = typename char_type_by_encoding<E2>::char_type;
			static int compare(const str_view<T1, E1>& value1, const fast_string<E2, A2>& value2) {
				return str_view_compare_helper<str_view<T1, E1>, str_view<const T2, E2>>::compare(value1, value2.cstr());
			}
		};

		template<text::encoding E1, template<typename>class A1, typename T2, text::encoding E2>
		struct str_view_compare_helper<fast_string<E1, A1>, str_view<T2, E2>>
		{
			using T1 = typename char_type_by_encoding<E1>::char_type;
			static int compare(const fast_string<E1, A1>& value1, const str_view<T2, E2>& value2) {
				return str_view_compare_helper<str_view<const T1, E1>, str_view<T2, E2>>::compare(value1.cstr(), value2);
			}
		};

	}

	namespace algorithms
	{
		template<text::encoding E, template<typename>class A>
		struct hash<text::fast_string<E, A>> {
			static ulong64 make(text::fast_string<E, A> const& value) {
				return hash<decltype(value.cstr())>::make(value.cstr());
			}
			ulong64 operator()(text::fast_string<E, A> const& value)const {
				return make(value);
			}
		};
	}

	inline text::fast_string<text::encoding::ascii> operator "" _fs(const char* str, wsize sz) { return str_view<const char>(str, sz); }
	inline text::fast_string<text::encoding::utf8> operator "" _fsm(const char* str, wsize sz) { return str_view<const mchar>((mchar const*)str, sz); }
	inline text::fast_string<text::encoding::unicode> operator "" _fs(const wchar_t* str, wsize sz) { return str_view<const wchar_t>(str, sz); }
	inline text::fast_string<text::encoding::utf16> operator "" _fs(const char16_t* str, wsize sz) { return str_view<const char16_t>(str, sz); }
	inline text::fast_string<text::encoding::utf32> operator "" _fs(const char32_t* str, wsize sz) { return str_view<const char32_t>(str, sz); }

}

#endif//__ANG_BASE_STRING_H__
