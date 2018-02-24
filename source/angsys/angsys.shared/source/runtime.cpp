#include "pch.h"
#include "ang/base/base.h"

#include <list>

using namespace ang;

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(char, "ang::char_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(byte, "ang::byte_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(wchar, "ang::wchar_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(short, "ang::short_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ushort, "ang::ushort_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(int, "ang::int_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(uint, "ang::uint_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(long, "ang::long_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ulong, "ang::ulong_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(long64, "ang::long64_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ulong64, "ang::ulong64_t");

inline auto find_rtti(rtti_t* info, std::list<rtti_t*>& list)-> decltype(list.begin()) {
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (info->genre() == (*it)->genre() &&
			info->size() == (*it)->size() &&
			info->aligment() == (*it)->aligment() &&
			text::ascii::compare(info->type_name().cstr(), (*it)->type_name().cstr()) == 0)
			return it;
	}
	return list.end();
}

rtti_t const& rtti::regist(type_name_t name, genre_t g, wsize sz, wsize a)
{
	static memory::unmanaged_allocator<rtti_t> alloc;
	static std::list<rtti_t*> list;
	rtti_t* info = alloc.allocate(1);
	info = new(info)rtti(name, g, sz, a);
	auto it = find_rtti(info, list);
	if (it != list.end()) {
		info->~__type_info();
		alloc.deallocate(info);
		info = *it;
	}
	else {
		list.push_back(info);
	}

	return*info;
}