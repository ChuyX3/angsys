#ifndef __ANGSYS_H__
#elif !defined __CONST_STRING_H__
#define __CONST_STRING_H__

namespace ang
{
	namespace text
	{
		class LINK basic_const_string_buffer_base
			: public object
			, public ibuffer
		{
		protected:
			pointer operator new(wsize, text::encoding_t, raw_cstr_t);
#ifdef WINDOWS_PLATFORM
			void operator delete(pointer, text::encoding_t, raw_cstr_t);
#elif defined ANDROID_PLATFORM
			void operator delete(pointer);
#endif // WINDOWS_PLATFORM

			basic_const_string_buffer_base();
			virtual~basic_const_string_buffer_base();

			basic_const_string_buffer_base(basic_const_string_buffer_base &&) = delete;
			basic_const_string_buffer_base(basic_const_string_buffer_base const&) = delete;
			basic_const_string_buffer_base& operator = (basic_const_string_buffer_base &&) = delete;
			basic_const_string_buffer_base& operator = (basic_const_string_buffer_base const&) = delete;

		public:	
			virtual bool is_readonly()const override;
			virtual bool can_realloc_buffer()const override;
	
			ANG_DECLARE_INTERFACE();

			virtual raw_str_t str(int = 0) = 0;
			virtual raw_cstr_t cstr(int = 0)const = 0;

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

#define	MY_ENCODING text::encoding::unicode
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf8
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf16
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf16_se
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf16_le
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf16_be
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf32
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf32_se
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf32_le
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING

#define	MY_ENCODING text::encoding::utf32_be
#include <ang/inline/const_string_declaration.hpp>
#undef MY_ENCODING
#undef MY_LINKAGE

#endif//__CONST_STRING_H__