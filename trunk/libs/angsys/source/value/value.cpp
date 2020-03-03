#include "pch.h"
#include "angsys.h"

using namespace ang;

#define MY_TYPE char
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE uchar
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE short
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE ushort
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE int
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE uint
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE long
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE ulong
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE long64
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE ulong64
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE float
#include "inline/value_implement.inl"
#undef MY_TYPE

#define MY_TYPE double
#include "inline/value_implement.inl"
#undef MY_TYPE