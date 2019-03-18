#include "pch.h"
#include <angsys.h>
#include <ang/collections/list.h>
#include <ang/collections/hash_map.h>



#define MY_ALLOC ang::memory::default_allocator

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

#define MY_TYPE ang::intfptr  
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::objptr    
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#define MY_TYPE ang::string  
#include <ang/collections/inline/list_specialization.inl>
#undef MY_TYPE

#undef MY_ALLOC

