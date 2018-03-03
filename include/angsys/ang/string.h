#ifndef __ANGSYS_H__
#elif !defined __STRING_H__
#define __STRING_H__

namespace ang
{
	typedef struct LINK raw_str {
		raw_str();
		raw_str(void* v, wsize s, text::encoding e);
		raw_str(raw_str const& str);
		template<typename T, text::encoding E> inline raw_str(str_view<T, E> str)
			: raw_str(str.str(), str.size() * sizeof(typename text::char_type_by_encoding<E>::char_t), E) {
		}

		void* ptr()const;
		wsize size()const;
		wsize count()const;
		wsize char_size()const;
		text::encoding encoding()const;

		template<text::encoding E> inline operator str_view<typename text::char_type_by_encoding<E>::char_t, E>() {
			return E == _encoding ? str_view<typename text::char_type_by_encoding<E>::char_t, E>(
				(typename text::char_type_by_encoding<E>::str_t)_value,
				_size / *sizeof(typename text::char_type_by_encoding<E>::char_t))
				: str_view<typename text::char_type_by_encoding<E>::char_t, E>();
		}
		template<text::encoding E> inline operator cstr_view<typename text::char_type_by_encoding<E>::char_t, E>()const {
			return E == _encoding ? cstr_view<typename text::char_type_by_encoding<E>::char_t, E>(
				(typename text::char_type_by_encoding<E>::cstr_t)_value,
				_size / *sizeof(typename text::char_type_by_encoding<E>::char_t))
				: cstr_view<typename text::char_type_by_encoding<E>::char_t, E>();
		}
		template<text::encoding E> inline str_view<typename text::char_type_by_encoding<E>::char_t, E> to_str() {
			return E == _encoding ? str_view<typename text::char_type_by_encoding<E>::char_t, E>(
				(typename text::char_type_by_encoding<E>::str_t)_value,
				_size / *sizeof(typename text::char_type_by_encoding<E>::char_t))
				: str_view<typename text::char_type_by_encoding<E>::char_t, E>();
		}
		template<text::encoding E> inline cstr_view<typename text::char_type_by_encoding<E>::char_t, E> to_cstr()const {
			return E == _encoding ? cstr_view<typename text::char_type_by_encoding<E>::char_t, E>(
				(typename text::char_type_by_encoding<E>::cstr_t)_value,
				_size / *sizeof(typename text::char_type_by_encoding<E>::char_t))
				: cstr_view<typename text::char_type_by_encoding<E>::char_t, E>();
		}

	private:
		void* _value;
		wsize _size;
		text::encoding _encoding;
	}raw_str_t;

	typedef struct LINK raw_cstr {
		raw_cstr();
		raw_cstr(void const* v, wsize s, text::encoding e);
		raw_cstr(raw_cstr const& str);
		raw_cstr(raw_str const& str);
		template<typename T, text::encoding E> inline raw_cstr(str_view<T, E> str)
			: raw_cstr(str.cstr(), str.size() * sizeof(typename text::char_type_by_encoding<E>::char_t), E) {
		}
		template<typename T, text::encoding E> inline raw_cstr(cstr_view<T, E> str)
			: raw_cstr(str.cstr(), str.size() * sizeof(typename text::char_type_by_encoding<E>::char_t), E) {
		}

		void const* ptr()const;
		wsize size()const;
		wsize count()const;
		wsize char_size()const;
		text::encoding encoding()const;

		template<text::encoding E> inline operator cstr_view<typename text::char_type_by_encoding<E>::char_t, E>()const;
		template<text::encoding E> inline cstr_view<typename text::char_type_by_encoding<E>::char_t, E> to_cstr()const;

	private:
		void const* _value;
		wsize _size;
		text::encoding _encoding;
	}raw_cstr_t;

	namespace text
	{
		typedef void* unknown_str_t;
		typedef  void const* unknown_cstr_t;

		ang_interface(iencoder);
		ang_interface(itext_buffer);

		ang_begin_interface(LINK itext_buffer, ibuffer)
			visible vcall raw_str_t text_buffer() pure;
			visible vcall raw_cstr_t text_buffer()const pure;
		ang_end_interface();

		ang_begin_interface(LINK iencoder)
			visible scall iencoder_t get_encoder(encoding_t);
			visible vcall encoding_t format()const pure;
			visible vcall rtti_t const& char_type()const pure;
			visible vcall void set_eos(unknown_str_t str, windex at)const pure;
			visible vcall wsize lenght(unknown_cstr_t)const pure;
			visible vcall wsize size(unknown_cstr_t, encoding_t, windex = 0, windex = -1)const pure;
			visible vcall int compare(unknown_cstr_t, unknown_cstr_t, encoding_t)const pure;
			visible vcall wsize compare_until(unknown_cstr_t, unknown_cstr_t, encoding_t)const pure;
			visible vcall wsize find(unknown_cstr_t, wsize, unknown_cstr_t, wsize, encoding_t, windex)const pure;
			visible vcall wsize find_reverse(unknown_cstr_t, wsize, unknown_cstr_t, wsize, encoding_t, windex)const pure;
			visible vcall raw_str_t convert(unknown_str_t dest, unknown_cstr_t src, encoding_t e, bool set_eos = true, wsize max_out_size = -1, wsize max_in_size = -1)const pure;
			visible vcall raw_str_t convert(unknown_str_t dest, wsize& i, unknown_cstr_t src, wsize& j, encoding_t e, bool set_eos = true, wsize max_out_size = -1, wsize max_in_size = -1)const pure;
		ang_end_interface();

	}

	ANG_INTF_WRAPPER_DECLARATION(LINK, text::itext_buffer);
	ANG_INTF_WRAPPER_DECLARATION(LINK, text::iencoder);

}

namespace ang
{

	namespace strings
	{
		class LINK const_string_buffer_base
			: public object
			, public text::itext_buffer
		{
		protected:
			pointer operator new(wsize, text::encoding_t, raw_cstr_t);
			void operator delete(pointer, text::encoding_t, raw_cstr_t);

			const_string_buffer_base();
			virtual~const_string_buffer_base();

			const_string_buffer_base(const_string_buffer_base &&) = delete;
			const_string_buffer_base(const_string_buffer_base const&) = delete;
			const_string_buffer_base& operator = (const_string_buffer_base &&) = delete;
			const_string_buffer_base& operator = (const_string_buffer_base const&) = delete;

		public:	
			virtual bool is_constant()const override;
			virtual bool can_realloc_buffer()const override;
	
		private:
			virtual pointer buffer_ptr() override;
			virtual wsize mem_copy(wsize, pointer, text::encoding_t) override;
			virtual ibuffer_view_t map_buffer(windex, wsize) override;
			virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
			virtual bool realloc_buffer(wsize) override;
		};
	}
}


#define MY_LINKAGE LINK

#define	MY_ENCODING text::encoding::ascii
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#undef MY_LINKAGE


namespace ang
{
	namespace strings
	{
		class LINK basic_string_buffer_base
			: public object
			, public itext_buffer
		{
		public:
			static constexpr wsize RAW_CAPACITY = 128u; //local storage capacity

		protected:
			struct _str_data {
				union {
					wsize _storage_type;
					struct {
						wsize _unused;
						wsize _allocated_length;
						wsize _allocated_capacity;
						pointer _allocated_buffer;
					};
					struct {
						wsize _stack_length;
						byte _stack_buffer[RAW_CAPACITY];
					};
				};
			} _data;
			wsize _map_index, _map_size;
			iencoder_t _encoder;

		protected:
			basic_string_buffer_base();

		public: //overides
			ANG_DECLARE_INTERFACE();

		public:
			bool is_empty()const;
			bool is_local_data()const;
			void length(wsize len);
			wsize length() const;
			wsize capacity() const;

		public:
			void copy(raw_cstr_t);
			void concat(raw_cstr_t);

			int compare(raw_cstr_t)const;
			windex compare_until(raw_cstr_t)const;
			windex find(raw_cstr_t, windex start = 0, windex end = -1)const;
			windex find_reverse(raw_cstr_t, windex start = -1, windex end = 0)const;
			raw_str_t sub_string(raw_str_t, windex start, windex end)const;

		protected:
			void copy_at(raw_str_t, windex at);

		public:
			virtual comparision_result_t compare(object const* obj)const override;
			virtual pointer buffer_ptr() override;
			virtual const_pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer, text::encoding_t) override;
			virtual ibuffer_view_t map_buffer(windex, wsize) override;
			virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;
			virtual raw_str_t text_buffer()override;
			virtual raw_cstr_t text_buffer()const override;

			virtual bool realloc(wsize new_size, bool save = true) pure;
			virtual void clean() pure;

		protected:
			virtual~basic_string_buffer_base();
		};


		template<encoding MY_ENCODING, template<typename> class allocator>
		class basic_string_buffer : public basic_string_buffer_base
		{
		public:
			static constexpr text::encoding ENCODING = MY_ENCODING;
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t, type;
			typedef typename char_type_by_encoding<ENCODING>::str_t unsafe_str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t unsafe_cstr_t;
			typedef allocator<type> allocator_t;
			typedef text::encoder<ENCODING> encoder;

			static constexpr wsize CAPACITY = RAW_CAPACITY / sizeof(char_t);
			typedef str_view<char_t> str_t;
			typedef str_view<char_t const> cstr_t;
			typedef basic_string_buffer_base string_base, base_t;
			typedef basic_string_buffer<ENCODING, allocator> string, self_t;

		private:
			allocator_t alloc;

		public:
			basic_string_buffer();

		public: //overides
			static ang::rtti_t const& class_info();
			virtual ang::rtti_t const& runtime_info()const override { return class_info(); }
			virtual bool query_interface(ang::rtti_t const&, ang::unknown_ptr_t) override {

			}

		public:
			virtual void clean()override;
			virtual bool realloc(wsize new_size, bool save = true)override;
			inline str_t str() { return text_buffer().to_str<ENCODING>(); }
			inline cstr_t cstr()const { return text_buffer().to_cstr<ENCODING>(); }

		public:
			using string_base::copy;
			using string_base::concat;
			using string_base::compare;
			using string_base::compare_until;
			using string_base::find;
			using string_base::find_reverse;
			using string_base::sub_string;

			template<typename T, text::encoding E> void copy(str_view<T, E> const& str) { string_base::copy(str); }
			template<text::encoding E, template<typename>class _alloc> void copy(basic_string<E, _alloc> const& str) { string_base::copy((cstr_view<typename char_type_by_encoding<E>::char_t, E>)str); }

			template<typename T, text::encoding E> void concat(str_view<T, E> const& str) { string_base::concat(str); }
			template<text::encoding E, template<typename>class _alloc> void concat(basic_string<E, _alloc> const& str) { string_base::concat((cstr_view<typename char_type_by_encoding<E>::char_t, E>)str); }

			template<typename T, text::encoding E> int compare(str_view<T, E> const& str)const { string_base::compare(str); }
			template<text::encoding E, template<typename>class _alloc> int compare(basic_string<E, _alloc> const& str)const { return string_base::compare((cstr_view<typename char_type_by_encoding<E>::char_t, E>)str);	}

			template<typename T, text::encoding E> wsize compare_until(str_view<T, E> const& str)const { string_base::compare_until(str); }
			template<text::encoding E, template<typename>class _alloc> wsize compare_until(basic_string<E, _alloc> const& str)const { return string_base::compare_until((cstr_view<typename char_type_by_encoding<E>::char_t, E>)str); }

			template<typename T, text::encoding E> windex find(str_view<T, E> const& str, windex start = 0, windex end = -1)const { string_base::find(str, start, end); }
			template<text::encoding E, template<typename>class _alloc> windex find(basic_string<E, _alloc> const& str, windex start = 0, windex end = -1)const { string_base::find((cstr_view<typename char_type_by_encoding<E>::char_t, E>)str, start, end); }

			template<typename T, text::encoding E> windex find_reverse(str_view<T, E> const& str, windex start = 0, windex end = -1)const { string_base::find_reverse(str, start, end); }
			template<text::encoding E, template<typename>class _alloc> windex find_reverse(basic_string<E, _alloc> const& str, windex start = 0, windex end = -1)const { string_base::find_reverse((cstr_view<typename char_type_by_encoding<E>::char_t, E>)str, start, end); }

			template<typename T, text::encoding E> str_view<T, E> sub_string(str_view<T, E>& str, windex start, windex end)const {
				string_base::sub_string(str, start, end).to_str<E>();
			}
			template<text::encoding E, template<typename>class _alloc> basic_string<E>& sub_string(basic_string<E, _alloc>& str, windex start = 0, windex end = -1)const {
				if (str.is_empty())str = new basic_string_buffer<E, _alloc>();
				cstr_t my_str = cstr();
				my_str.set(my_str.cstr() + start, min(my_str.size(), end) - start);
				str->copy(my_str);
				return str;
			}

		private:
			virtual~basic_string_buffer() { }
		};

	}

}

#define MY_ALLOCATOR memory::buffer_allocator
#define MY_LINKAGE LINK

#define	MY_ENCODING text::encoding::ascii
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#undef MY_LINKAGE
#undef MY_ALLOCATOR


template<ang::text::encoding MY_ENCODING, template<typename> class allocator>
inline ang::rtti_t const& ang::strings::basic_string_buffer<MY_ENCODING, allocator>::class_info() {
	static const char name[] = "ang::strings::basic_string_buffer"_s; //TODO:
	static rtti_t const* parents[] = { &runtime::type_of<string_base>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(_CLASS), alignof(_CLASS), parents, &default_query_interface);
	return info;
}
#endif//__STRING_H__