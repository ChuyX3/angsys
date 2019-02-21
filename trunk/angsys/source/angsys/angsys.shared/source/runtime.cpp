#include "pch.h"
#include "angsys.h"
#include "runtime_manager.h"

using namespace ang;


rtti_t const& type_info_builder<void, genre::value_type>::type_of() { 
	static rtti_t const& info = rtti::regist("void"_s, genre::value_type, sizeof(wsize), alignof(wsize)); 
	return info;
}
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(pointer, "pointer");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(char, "char");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(byte, "byte");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(wchar, "wchar");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(char16_t, "char16");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(char32_t, "char32");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(short, "short");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ushort, "ushort");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(int, "int");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(uint, "uint");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(long, "long");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ulong, "ulong");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(long64, "long64");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ulong64, "ulong64");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(float, "float");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(double, "double");

ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<char>, "char[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<byte>, "byte[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<wchar>, "wchar[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<char16_t>, "char16[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<char32_t>, "char32[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<short>, "short[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<ushort>, "ushort[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<int>, "int[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<uint>, "uint[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<long>, "long[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<ulong>, "ulong[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<long64>, "long64[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<ulong64>, "ulong64[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<float>, "float[]");
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(ang::array_view<double>, "double[]");


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

safe_enum_rrti2(ang::text, encoding);
