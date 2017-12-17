/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ...
#elif !defined __ANG_STRINGS_HPP__
#define __ANG_STRINGS_HPP__

namespace ang
{
	namespace text
	{

		ANG_INTERFACE(itext_buffer); //template<encoding_enum ENCODING> struct itext_buffer;
		//template<encoding_enum ENCODING> using itext_buffer_t = intf_wrapper<itext_buffer<ENCODING>>;
		//template<encoding_enum ENCODING> using itext_buffer_ptr_t = intf_wrapper_ptr<itext_buffer<ENCODING>>;

		ANG_BEGIN_INTERFACE_WITH_BASE(LINK, itext_buffer, public ibuffer)
			visible vcall raw_str_t text_buffer()const pure;
		ANG_END_INTERFACE();


		template<text::encoding_enum ENCODING>
		class text_buffer_wrapper final
			: public object
			, public itext_buffer
		{
		private:
			ibuffer_t _buffer;

		public:
			inline text_buffer_wrapper(ibuffer_t buffer) : _buffer(buffer) {}

			//ANG_DECLARE_INLINE_INTERFACE();

			pointer buffer_ptr()const override {
				return _buffer.is_empty() ? nullptr : _buffer->buffer_ptr();
			}
			wsize buffer_size()const override {
				return _buffer.is_empty() ? 0 : _buffer->buffer_size();
			}
			wsize mem_copy(wsize sz, pointer ptr, text::encoding_t e = text::encoding::binary) override {
				return _buffer.is_empty() ? 0 : _buffer->mem_copy(sz, ptr, e);
			}
			ibuffer_view_t map_buffer(windex idx, wsize sz) override {
				return _buffer.is_empty() ? nullptr : _buffer->map_buffer(idx, sz);
			}
			bool unmap_buffer(ibuffer_view_t& buff, wsize used) override {
				return _buffer.is_empty() ? false : _buffer->unmap_buffer(buff, used);
			}
			bool can_realloc_buffer()const override {
				return _buffer.is_empty() ? false : _buffer->can_realloc_buffer();
			}
			bool realloc_buffer(wsize sz) override {
				return _buffer.is_empty() ? false : _buffer->realloc_buffer(sz);
			}
			inline encoding_t encoding()const override { return ENCODING; }
			raw_str_t text_buffer()const override {
				return _buffer.is_empty() ? raw_str_t{ null, 0, encoding::none }
				: raw_str_t{ _buffer->buffer_ptr(), _buffer->buffer_size(), ENCODING };
			}
		private:
			inline~text_buffer_wrapper() {}
		};
	}

	namespace strings
	{
		template<encoding_enum _ENCODING>
		struct str_data
		{
			typedef typename char_type_by_encoding<_ENCODING>::char_t char_t;
			typedef typename char_type_by_encoding<_ENCODING>::str_t  str_t;
			typedef typename char_type_by_encoding<_ENCODING>::cstr_t cstr_t;

			static constexpr encoding_enum ENCODING = _ENCODING;
			static constexpr wsize CAPACITY = 128u / sizeof(char_t);

			union {
				wsize _storage_type;
				struct {
					wsize _unused;
					wsize _allocated_length;
					wsize _allocated_capacity;
					str_t _allocated_buffer;
				};
				struct {
					wsize _stack_length;
					char_t _stack_buffer[CAPACITY];
				};
			};
		};
	}

	namespace strings
	{
		class LINK string_base_buffer
			: public object
				, public text::itext_buffer
		{
		protected:
			struct _str_data
			{
				static constexpr wsize RAW_CAPACITY = 128u;

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
			};

		protected:
			_str_data _data;
			wsize _map_index, _map_size;

		public:
			string_base_buffer();

		protected:
			virtual~string_base_buffer();

		public:
			ANG_DECLARE_INTERFACE();
			bool is_local_data()const;
			bool realloc(wsize new_size, bool save = true);
			void length(wsize len);
			wsize length() const;

		public:
			void clean();
			bool is_empty()const;
			wsize capacity() const;

		public:
			void copy(raw_str_t);
			void concat(raw_str_t);

			int compare(raw_str_t)const;
			windex compare_until(raw_str_t)const;	
			windex find(raw_str_t, windex start = 0, windex end = -1)const;
			windex find_revert(raw_str_t, windex start = -1, windex end = 0)const;
			wsize sub_string(raw_str_t, windex start, windex end)const;

		protected:
			void copy_at(raw_str_t, windex at);

		public:
			virtual comparision_result_t compare(object const& obj)const override;
			virtual wsize serialize(streams::ibinary_output_stream_t stream)const override;
			virtual wsize serialize(streams::itext_output_stream_t stream)const override;
			virtual pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer, text::encoding_t) override;
			virtual ibuffer_view_t map_buffer(windex, wsize) override;
			virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;		
			virtual raw_str_t text_buffer()const override;

			virtual wsize char_size()const pure;
			
		protected:
			virtual text::encoder_interface& encoder()const pure;
			//virtual text::encoding_t encoding()const pure;
		};
	}
}

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::ascii
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::unicode
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf8
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf16
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf16_se
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf16_le
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf16_be
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf32
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf32_se
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf32_le
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
#define CURRENT_ENCODING ang::text::encoding::utf32_be
#include <ang/inline/string_definition.inl>
#undef CURRENT_ENCODING
/////////////////////////////////////////////////////////////////////////////////

namespace ang
{
	namespace text
	{
		template<encoding_enum ENCODING>
		inline windex string_util::sub_string(strings::string_base<ENCODING>& str, windex start, windex end)const {
			if (str.is_empty())
				str = new strings::string_buffer<ENCODING>();
			str->realloc(min(end, _data.size() / _char_size) - start, false);
			auto ptr = str->str();
			str->length(get_encoder<ENCODING>().convert(str->str(), min(_data.size() / _char_size, end) - start, (byte*)_data.ptr() + start * _char_size, _data.encoding(), true));
			return str->length();
		}
	}

	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator == (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator != (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator >= (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator <= (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator > (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	//template<text::encoding_enum ENCODING1, text::encoding_enum ENCODING2>
	//bool operator < (object_wrapper<strings::string_buffer<ENCODING1>> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);

	template<text::encoding_enum ENCODING1, typename T>
	bool operator == (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator != (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator >= (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator <= (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator > (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);
	template<text::encoding_enum ENCODING1, typename T>
	bool operator < (object_wrapper<strings::string_buffer<ENCODING1>> const&, safe_str<T> const&);

	template<typename T, text::encoding_enum ENCODING2>
	bool operator == (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator != (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator >= (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator <= (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator > (safe_str<T>const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);
	template<typename T, text::encoding_enum ENCODING2>
	bool operator < (safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING2>> const&);


	inline string operator "" _o(const char* str, wsize sz) { return new strings::string_buffer < text::encoding::ascii>(cstr_t(str, sz)); }
	inline wstring operator "" _o(const wchar_t* str, wsize sz) { return new strings::string_buffer < text::encoding::unicode>(cwstr_t(str, sz)); }
}
#endif//__ANG_STRINGS_HPP__