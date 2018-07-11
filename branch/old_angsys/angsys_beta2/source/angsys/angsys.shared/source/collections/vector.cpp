#include "pch.h"
#include <ang/collections/vector.hpp>

#define MY_ALLOCATOR ang::memory::default_allocator


#define MY_ALLOCATOR ang::memory::default_allocator

#define MY_TYPE char
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE mchar
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE wchar
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE char16_t
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE char32_t
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE short
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ushort
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE int
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE uint
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE long
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ulong
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE long64
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ulong64
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE float
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE double
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE


#define MY_TYPE ang::strings::string_base<ang::text::encoding::ascii>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::unicode>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf8>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf16>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf16_se>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf16_le>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf16_be>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE


#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf32>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf32_se>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf32_le>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::strings::string_base<ang::text::encoding::utf32_be>
#include <ang/collections/inline/vector_object_specialization.inl>
#undef MY_TYPE

#undef MY_ALLOCATOR

////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////

#define MY_ALLOCATOR ang::memory::aligned16_allocator

#define MY_TYPE char
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE mchar
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE wchar
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE char16_t
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE char32_t
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE short
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ushort
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE int
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE uint
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE long
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ulong
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE long64
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ulong64
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE float
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE double
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#undef MY_ALLOCATOR
