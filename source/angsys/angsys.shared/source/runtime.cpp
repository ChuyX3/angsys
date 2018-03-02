#include "pch.h"
#include "ang/system.h"
#include "runtime_manager.h"

using namespace ang;

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(char, "ang::char_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(byte, "ang::byte_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(wchar, "ang::wchar_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(char16_t, "ang::char16_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(char32_t, "ang::char32_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(short, "ang::short_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ushort, "ang::ushort_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(int, "ang::int_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(uint, "ang::uint_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(long, "ang::long_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ulong, "ang::ulong_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(long64, "ang::long64_t");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ulong64, "ang::ulong64_t");


bool ang_runtime_rtti_default_dyn_cast(const rtti_t& src_id, unknown_t src, const rtti_t& out_id, unknown_ptr_t out)
{
	if (out && src_id.type_id() == out_id.type_id()) {
		*out = src;
		return true;
	}
	return false;
}

rtti_t const& rtti::regist(type_name_t name, genre_t g, wsize sz, wsize a)
{
	runtime_type_manager* instance = runtime_type_manager::instance();

	rtti_t* info = instance->allocate();
	info = new(info)rtti(name, g, sz, a, null, &ang_runtime_rtti_default_dyn_cast);
	auto it = instance->find_info(info);
	if (it != null) {
		info->~__type_info();
		instance->deallocate(info);
		info = it;
	}
	else {
		instance->push(info);
	}
	return*info;
}

rtti_t const& rtti::regist(type_name_t name, genre_t g, wsize sz, wsize a, array_view<rtti_t const*> parents, dynamic_cast_proc cast)
{
	runtime_type_manager* instance = runtime_type_manager::instance();

	rtti_t* info = instance->allocate();
	info = new(info)rtti(name, g, sz, a, parents, cast ? cast : &ang_runtime_rtti_default_dyn_cast);
	auto it = instance->find_info(info);
	if (it != null) {
		info->~__type_info();
		instance->deallocate(info);
		info = it;
	}
	else {
		instance->push(info);
	}
	return*info;
}

safe_enum_rrti(ang::text, encoding_t, value<encoding_proxy>);
