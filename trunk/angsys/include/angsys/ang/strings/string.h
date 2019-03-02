#ifndef __ANGSYS_H__
#elif !defined __STRING_H__
#define __STRING_H__

namespace ang
{
	namespace text
	{
		class LINK basic_string_buffer_base
			: public object
			, public istring
			, public ibuffer
			, public ivariant
		{
		public:
			static const wsize RAW_CAPACITY = 64u; //local storage capacity

			enum storage_type_t : wint {
				storage_type_stack = 0,
				storage_type_allocated = wint(-1),
				storage_type_string_pool = wint(-2),
			};

		protected:
			
			struct str_data {
				union {
					storage_type_t m_storage_type;
					struct {
						wsize _unused;
						wsize m_allocated_length;
						wsize m_allocated_capacity;
						pointer m_allocated_buffer;
					};
					struct {
						wsize m_stack_length;
						byte m_stack_buffer[RAW_CAPACITY];
					};
					struct {
						wsize __unused;
						basic_const_string_buffer_base* m_const_string;
						pointer m_const_string_view;
					};
				};
			} m_data;
			wsize m_map_index, m_map_size;
			iencoder_t m_encoder;
			iparser_t m_parser;

		protected:
			basic_string_buffer_base();

		public: //utils
			bool is_empty()const;
			storage_type_t storage_type()const;
			void length(wsize len);
			wsize capacity() const;
			void set(basic_const_string_buffer_base*);
			

		protected: //utils
			void copy_at(raw_str_t, windex at);

		public: //overides
			ANG_DECLARE_INTERFACE();
			virtual comparision_result_t compare(object const* obj)const override;
			virtual bool realloc(wsize new_size, bool save = true) = 0;

		public: //istring_view overrides
			virtual text::encoding_t encoding()const override;
			virtual wsize length() const override;
			virtual unknown_str_t data()const override;
			virtual rtti_t const& char_type()const override;
			virtual raw_str_t str(int = 0)override;
			virtual raw_cstr_t cstr(int = 0)const override;
			virtual char32_t at(windex i, wsize* sz = null)const;
			virtual int compare(raw_cstr_t)const override;
			virtual windex compare_until(raw_cstr_t)const override;
			virtual windex find(raw_cstr_t, windex start = 0, windex end = -1)const override;
			virtual windex find_reverse(raw_cstr_t, windex start = -1, windex end = 0)const override;
			virtual istring_t sub_string(istring_ptr_t, windex start, windex end)const override;

		public: //istring overrides
			virtual void clear() = 0;
			virtual void copy(raw_cstr_t)override;
			virtual void concat(raw_cstr_t)override;
			virtual void insert(windex, raw_cstr_t)override;
			virtual void format(raw_cstr_t, var_args_t)override;
			virtual void concat_format(raw_cstr_t, var_args_t)override;
			virtual void insert_format(windex, raw_cstr_t, var_args_t)override;

		public: //ivariant override
			virtual wstring to_string()const override;
			virtual wstring to_string(text::text_format_t)const override;

		private: //ivariant override
			virtual rtti_t const& value_type()const override;
			virtual bool set_value(rtti_t const&, unknown_t) override;
			virtual bool get_value(rtti_t const&, unknown_t)const override;

		private: //ibuffer override
			virtual pointer buffer_ptr() override;
			virtual const_pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer, text::encoding_t) override;
			virtual ibuffer_view_t map_buffer(windex, wsize) override;
			virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;

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
			typedef allocator<char_t> allocator_t;
			typedef text::encoder<ENCODING> encoder;

			static constexpr wsize CAPACITY = RAW_CAPACITY / sizeof(char_t);
			typedef str_view<char_t, ENCODING> str_t;
			typedef str_view<char_t const, ENCODING> cstr_t;
			typedef basic_string_buffer_base string_base, base_t;
			typedef basic_string_buffer<ENCODING, allocator> string, self_t;
			typedef object_wrapper<string> string_t;
		public:
			basic_string_buffer();

		public: //overides
			ANG_DECLARE_INTERFACE();
			virtual void clear()override {
				allocator_t alloc;
				if (storage_type_allocated == storage_type())
					alloc.deallocate((str_t)m_data.m_allocated_buffer);
				else if (storage_type_string_pool == storage_type())
					m_data.m_const_string->release();
				memset(&m_data, 0, sizeof(m_data));
			}
			virtual bool realloc(wsize new_size, bool save = true)override {
				allocator_t alloc;

				if (m_map_index != invalid_index || m_map_size != invalid_index)
					return false;

				if (capacity() > new_size)
					return true;

				wsize cs = sizeof(char_t), size = 32U, i = 0;
				while (size <= new_size)
					size *= 2U;
				auto new_buffer = alloc.allocate(size);
				if (new_buffer == null)
					return false;
				wsize len = 0U, j = 0;
				new_buffer[0] = 0;
				auto data = cstr();
				if (save)
					text::encoder<ENCODING>::convert(new_buffer, len, data.cstr(), j, true, size - 1, data.size());

				clear();
				m_data.m_allocated_length = len;
				m_data.m_allocated_capacity = size - 1;
				m_data.m_allocated_buffer = new_buffer;
				m_data.m_storage_type = invalid_index;
				return true;
			}

		private: //ivariant override
			variant clone()const override {
				return static_cast<ivariant*>(new self_t(cstr()));
			}
			bool is_readonly(void) const override {
				return false;
			}

		public:
			inline str_t str(void) { return this ? istring::str().template to_str<ENCODING>() : str_t(); }
			inline cstr_t cstr(void)const { return this ? istring::cstr().template to_cstr<ENCODING>() : cstr_t();}

		public:
			using string_base::sub_string;
			template<text::encoding E, template<typename>class _alloc> basic_string<E>& sub_string(basic_string<E, _alloc>& str, windex start = 0, windex end = -1)const {
				if (str.is_empty())str = new basic_string_buffer<E, _alloc>();
				string_base::sub_string(str.get(), start, end);
				return str;
			}
			string_t sub_string(windex start = 0, windex end = -1)const {
				string_t str = new string();
				string_base::sub_string(static_cast<istring*>(str.get()), start, end);
				return str.get();
			}

		private:
			virtual~basic_string_buffer() { clear(); }
		};

		class LINK string_pool
			: public singleton<string_pool, memory::unmanaged_allocator>
		{
		private:
			//friend  singleton<string_pool, memory::unmanaged_allocator>;
			friend typename singleton<string_pool, memory::unmanaged_allocator>::allocator_t;
			pointer pool;
			string_pool();
			virtual~string_pool();

		public:
			cstr_t save_string(cstr_t);
			cstr_t save_string(string const&);
			cstr_t save_string(const_string);
			cstr_t save_string(const_string_buffer*);
			const_string find_string(cstr_t);
		};

	}
}

#define MY_ALLOCATOR memory::buffer_allocator
#define MY_LINKAGE LINK

#define	MY_ENCODING text::encoding::ascii
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::unicode
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf8
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf16
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf16_se
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf16_le
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf16_be
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf32
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf32_se
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf32_le
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf32_be
#include <ang/inline/string_declaration.hpp>
#undef MY_ENCODING


#undef MY_LINKAGE
#undef MY_ALLOCATOR


template<ang::text::encoding MY_ENCODING, template<typename> class allocator>
inline ang::rtti_t const& ang::text::basic_string_buffer<MY_ENCODING, allocator>::class_info() {
	static const cstr_view<char> name = string_pool::instance()->save_string((string("ang::string<"_s) += ang::to_string<ang::text::encoding, MY_ENCODING>::value) += ">"_s);
	static rtti_t const* parents[] = { &runtime::type_of<string_base>() };
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(basic_string_buffer<MY_ENCODING, allocator>), alignof(basic_string_buffer<MY_ENCODING, allocator>), parents, &default_query_interface);
	return info;
}

namespace ang
{
	template<text::encoding E1, template<typename>class A1, text::encoding E2, template<typename>class A2> bool operator == (text::basic_string<E1, A1> const& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1->cstr(), str2->cstr()) == 0; }
	template<text::encoding E1, template<typename>class A1, text::encoding E2, template<typename>class A2> bool operator != (text::basic_string<E1, A1> const& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1->cstr(), str2->cstr()) != 0; }
	template<text::encoding E1, template<typename>class A1, text::encoding E2, template<typename>class A2> bool operator <= (text::basic_string<E1, A1> const& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1->cstr(), str2->cstr()) <= 0; }
	template<text::encoding E1, template<typename>class A1, text::encoding E2, template<typename>class A2> bool operator >= (text::basic_string<E1, A1> const& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1->cstr(), str2->cstr()) >= 0; }
	template<text::encoding E1, template<typename>class A1, text::encoding E2, template<typename>class A2> bool operator < (text::basic_string<E1, A1> const& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1->cstr(), str2->cstr()) < 0; }
	template<text::encoding E1, template<typename>class A1, text::encoding E2, template<typename>class A2> bool operator > (text::basic_string<E1, A1> const& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1->cstr(), str2->cstr()) > 0; }

	template<text::encoding E1, template<typename>class A1, typename T2, text::encoding E2>	bool operator == (text::basic_string<E1, A1> const& str1, const str_view<T2, E2>& str2) { return text::encoder<E1>::template compare<T2 const*>(str1->cstr(), str2.cstr()) == 0; }
	template<text::encoding E1, template<typename>class A1, typename T2, text::encoding E2>	bool operator != (text::basic_string<E1, A1> const& str1, const str_view<T2, E2>& str2) { return text::encoder<E1>::template compare<T2 const*>(str1->cstr(), str2.cstr()) != 0; }
	template<text::encoding E1, template<typename>class A1, typename T2, text::encoding E2>	bool operator >= (text::basic_string<E1, A1> const& str1, const str_view<T2, E2>& str2) { return text::encoder<E1>::template compare<T2 const*>(str1->cstr(), str2.cstr()) >= 0; }
	template<text::encoding E1, template<typename>class A1, typename T2, text::encoding E2>	bool operator <= (text::basic_string<E1, A1> const& str1, const str_view<T2, E2>& str2) { return text::encoder<E1>::template compare<T2 const*>(str1->cstr(), str2.cstr()) <= 0; }
	template<text::encoding E1, template<typename>class A1, typename T2, text::encoding E2>	bool operator > (text::basic_string<E1, A1> const& str1, const str_view<T2, E2>& str2) { return text::encoder<E1>::template compare<T2 const*>(str1->cstr(), str2.cstr()) > 0; }
	template<text::encoding E1, template<typename>class A1, typename T2, text::encoding E2>	bool operator < (text::basic_string<E1, A1> const& str1, const str_view<T2, E2>& str2) { return text::encoder<E1>::template compare<T2 const*>(str1->cstr(), str2.cstr()) < 0; }

	template<typename T1, text::encoding E1, text::encoding E2, template<typename>class A2> bool operator == (const str_view<T1, E1>& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1.cstr(), str2->cstr()) == 0; }
	template<typename T1, text::encoding E1, text::encoding E2, template<typename>class A2> bool operator != (const str_view<T1, E1>& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1.cstr(), str2->cstr()) != 0; }
	template<typename T1, text::encoding E1, text::encoding E2, template<typename>class A2> bool operator >= (const str_view<T1, E1>& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1.cstr(), str2->cstr()) >= 0; }
	template<typename T1, text::encoding E1, text::encoding E2, template<typename>class A2> bool operator <= (const str_view<T1, E1>& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1.cstr(), str2->cstr()) <= 0; }
	template<typename T1, text::encoding E1, text::encoding E2, template<typename>class A2> bool operator > (const str_view<T1, E1>& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1.cstr(), str2->cstr()) > 0; }
	template<typename T1, text::encoding E1, text::encoding E2, template<typename>class A2> bool operator < (const str_view<T1, E1>& str1, text::basic_string<E1, A2> const& str2) { return text::encoder<E1>::template compare<typename text::char_type_by_encoding<E2>::cstr_t>(str1.cstr(), str2->cstr()) < 0; }
}

#endif//__STRING_H__