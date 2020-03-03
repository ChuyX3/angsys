#include "pch.h"
#include <angsys.h>
#include <ang/collections/list.h>
#include <ang/collections/hash_map.h>

#define DECLARE_ALLOC(_TYPE, _ALLOC) static _ALLOC<_TYPE> ANG_CATABCD(s_, _ALLOC, _ , _TYPE)
#define m_alloc ANG_CATABCD(s_, MY_ALLOC, _, MY_TYPE)

using namespace ang;
using namespace ang::memory;

#define MY_ALLOC buffer_allocator

#define MY_TYPE char
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE uchar
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE wchar
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE char16
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE char32
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

#define MY_TYPE pointer
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE intfptr
#include <ang/collections/inline/vector_specialization.inl>
#undef MY_TYPE

#define MY_TYPE objptr
#include <ang/collections/inline/vector_specialization.inl>
#undef MY_TYPE

#define MY_TYPE string 
#include <ang/collections/inline/vector_specialization.inl>
#undef MY_TYPE

#undef MY_ALLOC
#define MY_ALLOC aligned16_allocator

#define MY_TYPE char
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE uchar
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE wchar
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE char16
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#define MY_TYPE char32
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

#define MY_TYPE pointer
#include <ang/collections/inline/vector_value_specialization.inl>
#undef MY_TYPE

#undef m_alloc
#undef DECLARE_ALLOC
#define DECLARE_ALLOC(_TYPE, _ALLOC) static collections::list_object<_TYPE, _ALLOC>::allocator_t ANG_CATABCD(s_, _ALLOC, _node_ , _TYPE)
#define m_alloc ANG_CATABCD(s_, MY_ALLOC, _node_, MY_TYPE)

#undef MY_ALLOC
#define MY_ALLOC default_allocator

#define MY_TYPE char
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE byte
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE short
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ushort
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE int
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE uint
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE long
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ulong
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE long64
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ulong64
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE float
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE double
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE intfptr
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE objptr
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE string
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#undef MY_ALLOC