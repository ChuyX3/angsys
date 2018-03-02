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
			visible vcall wsize size(unknown_cstr_t, encoding_t)const pure;
			visible vcall int compare(unknown_cstr_t, unknown_cstr_t, encoding_t)const pure;
			visible vcall wsize compare_until(unknown_cstr_t, unknown_cstr_t, encoding_t)const pure;
			visible vcall wsize find(unknown_cstr_t, wsize, unknown_cstr_t, wsize, encoding_t, windex)const pure;
			visible vcall wsize find_reverse(unknown_cstr_t, wsize, unknown_cstr_t, wsize, encoding_t, windex)const pure;
			visible vcall raw_str_t convert(unknown_str_t dest, unknown_cstr_t src, encoding_t e, bool set_eos = true, wsize max_size = - 1)const pure;
			visible vcall raw_str_t convert(unknown_str_t dest, wsize& i, unknown_cstr_t src, wsize& j, encoding_t e, bool set_eos = true, wsize max_size = - 1)const pure;
		ang_end_interface();

	}

	ANG_INTF_WRAPPER_DECLARATION(LINK, text::itext_buffer);
	ANG_INTF_WRAPPER_DECLARATION(LINK, text::iencoder);

	namespace strings
	{
		class LINK basic_string_buffer_base
			: public object
			, public itext_buffer
		{

		protected:
			struct _str_data {
				static constexpr wsize RAW_CAPACITY = 128u; //local storage capacity

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
			windex find_revert(raw_cstr_t, windex start = -1, windex end = 0)const;
			wsize sub_string(raw_str_t, windex start, windex end)const;

		protected:
			void copy_at(raw_str_t, windex at);

		public:
			virtual comparision_result_t compare(object const* obj)const override;
			virtual pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer, text::encoding_t) override;
			virtual ibuffer_view_t map_buffer(windex, wsize) override;
			virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;
			virtual raw_str_t text_buffer()override;
			virtual raw_cstr_t text_buffer()const override;
			virtual bool realloc(wsize new_size, bool save = true) = 0;
			virtual void clean() = 0;

		protected:
			virtual~basic_string_buffer_base();
		};
	}

}

#define MY_ALLOCATOR memory::buffer_allocator
#define MY_LINKAGE LINK

//#define	MY_ENCODING text::encoding::ascii
//#include <ang/inline/string_declaration.hpp>
//#undef MY_ENCODING

#undef MY_LINKAGE
#undef MY_ALLOCATOR
#endif//__STRING_H__