#include "pch.h"
#include "angsys.h"
#include "format_parser.h"

using namespace ang;
using namespace ang::strings;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::text::istring, interface);

#define MY_TYPE ang::text::istring
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE


basic_string_buffer_base::basic_string_buffer_base()
{
	memset(&m_data, 0, sizeof(m_data));
	m_map_index = (wsize)invalid_index;
	m_map_size = (wsize)invalid_index;
}

basic_string_buffer_base::~basic_string_buffer_base()
{
	m_map_index = (wsize)invalid_index;
	m_map_size = (wsize)invalid_index;
	//clear();
}

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::strings::basic_string_buffer_base, object, istring, ibuffer);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::strings::basic_string_buffer_base);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::strings::basic_string_buffer_base, object, istring, ibuffer, ibuffer_view);

/////////////////////////////////////////////////////////////////////////

wstring basic_string_buffer_base::to_string()const
{
	return cstr();
}

wstring basic_string_buffer_base::to_string(text::text_format_t format)const
{
	//TODO:
	return cstr(); 
}

rtti_t const& basic_string_buffer_base::value_type()const
{
	return runtime_info();
}

bool basic_string_buffer_base::set_value(rtti_t const& id, unknown_t ptr)
{
	if (id.is_type_of(class_info()))
	{
		basic_string_buffer_base* str = interface_cast<basic_string_buffer_base>((interface*)ptr);
		if (!str)
			return false;
		copy(str->cstr());
		return true;
	}
	else if (id.is_type_of(type_of<char*>()) || id.is_type_of(type_of<char const*>()))
	{
		char const* str = reinterpret_cast<char const*>(ptr);
		copy(cstr_t(str));
		return true;
	}
	else if (id.is_type_of(type_of<mchar*>()) || id.is_type_of(type_of<mchar const*>()))
	{
		mchar const* str = reinterpret_cast<mchar const*>(ptr);
		copy(cmstr_t(str));
		return true;
	}
	else if (id.is_type_of(type_of<wchar*>()) || id.is_type_of(type_of<wchar const*>()))
	{
		wchar const* str = reinterpret_cast<wchar const*>(ptr);
		copy(cwstr_t(str));
		return true;
	}
	else if (id.is_type_of(type_of<char16_t*>()) || id.is_type_of(type_of<char16_t const*>()))
	{
		char16_t const* str = reinterpret_cast<char16_t const*>(ptr);
		copy(cstr16_t(str));
		return true;
	}
	else if (id.is_type_of(type_of<char32_t*>()) || id.is_type_of(type_of<char32_t const*>()))
	{
		char32_t const* str = reinterpret_cast<char32_t const*>(ptr);
		copy(cstr32_t(str));
		return true;
	}
	else if (id.is_type_of(type_of<cstr_t>()))
	{
		cstr_t& str = *reinterpret_cast<cstr_t*>(ptr);
		copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cwstr_t>()))
	{
		cwstr_t& str = *reinterpret_cast<cwstr_t*>(ptr);
		copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cmstr_t>()))
	{
		cmstr_t& str = *reinterpret_cast<cmstr_t*>(ptr);
		copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cstr16_t>()))
	{
		cstr16_t& str = *reinterpret_cast<cstr16_t*>(ptr);
		copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cstr32_t>()))
	{
		cstr32_t& str = *reinterpret_cast<cstr32_t*>(ptr);
		copy(str);
		return true;
	}

	return false;
}

bool basic_string_buffer_base::get_value(rtti_t const& id, unknown_t ptr)const
{
	if (id.is_type_of(class_info()))
	{
		basic_string_buffer_base* str = interface_cast<basic_string_buffer_base>((interface*)ptr);
		if (!str)
			return false;
		str->copy(cstr());
		return true;
	}
	else if (id.is_type_of<str_t>())
	{
		str_t& dest = *reinterpret_cast<str_t*>(ptr);
		auto src = cstr();
		dest.set(dest.str(), iencoder::get_encoder(encoding::ascii)->convert(dest.str(), src.ptr(), src.encoding(), true, dest.size(), src.count()).count());
		return true;
	}
	else if (id.is_type_of<wstr_t>())
	{
		wstr_t& dest = *reinterpret_cast<wstr_t*>(ptr);
		auto src = cstr();
		dest.set(dest.str(), iencoder::get_encoder(encoding::unicode)->convert(dest.str(), src.ptr(), src.encoding(), true, dest.size(), src.count()).count());
		return true;
	}
	else if (id.is_type_of<mstr_t>())
	{
		mstr_t& dest = *reinterpret_cast<mstr_t*>(ptr);
		auto src = cstr();
		dest.set(dest.str(), iencoder::get_encoder(encoding::utf8)->convert(dest.str(), src.ptr(), src.encoding(), true, dest.size(), src.count()).count());
		return true;
	}
	else if (id.is_type_of<str16_t>())
	{
		str16_t& dest = *reinterpret_cast<str16_t*>(ptr);
		auto src = cstr();
		dest.set(dest.str(), iencoder::get_encoder(encoding::utf16)->convert(dest.str(), src.ptr(), src.encoding(), true, dest.size(), src.count()).count());
		return true;
	}
	else if (id.is_type_of<str32_t>())
	{
		str32_t& dest = *reinterpret_cast<str32_t*>(ptr);
		auto src = cstr();
		dest.set(dest.str(), iencoder::get_encoder(encoding::utf32)->convert(dest.str(), src.ptr(), src.encoding(), true, dest.size(), src.count()).count());
		return true;
	}
	else if (id.is_type_of<raw_str_t>())
	{
		raw_str_t& dest = *reinterpret_cast<raw_str_t*>(ptr);
		auto src = cstr();
		dest = iencoder::get_encoder(dest.encoding())->convert(dest.ptr(), src.ptr(), src.encoding(), true, dest.count(), src.count());
		return true;
	}
	else if (id.is_type_of<short>())
	{
		windex i = 0;
		short& value = *reinterpret_cast<short*>(ptr);
		auto str = cstr();
		value = (short)m_parser->to_signed(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<ushort>())
	{
		windex i = 0;
		ushort& value = *reinterpret_cast<ushort*>(ptr);
		auto str = cstr();
		value = (ushort)m_parser->to_unsigned(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<int>())
	{
		windex i = 0;
		int& value = *reinterpret_cast<int*>(ptr);
		auto str = cstr();
		value = (int)m_parser->to_signed(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<uint>())
	{
		windex i = 0;
		uint& value = *reinterpret_cast<uint*>(ptr);
		auto str = cstr();
		value = (uint)m_parser->to_unsigned(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<long>())
	{
		windex i = 0;
		long& value = *reinterpret_cast<long*>(ptr);
		auto str = cstr();
		value = (int)m_parser->to_signed(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<ulong>())
	{
		windex i = 0;
		ulong& value = *reinterpret_cast<ulong*>(ptr);
		auto str = cstr();
		value = (ulong)m_parser->to_unsigned(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<long64>())
	{
		windex i = 0;
		long64& value = *reinterpret_cast<long64*>(ptr);
		auto str = cstr();
		value = m_parser->to_signed(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<ulong64>())
	{
		windex i = 0;
		ulong64& value = *reinterpret_cast<ulong64*>(ptr);
		auto str = cstr();
		value = m_parser->to_unsigned(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<float>())
	{
		windex i = 0;
		float& value = *reinterpret_cast<float*>(ptr);
		auto str = cstr();
		value = (float)m_parser->to_floating(str.ptr(), str.count(), i);
		return i > 0;
	}
	else if (id.is_type_of<double>())
	{
		windex i = 0;
		double& value = *reinterpret_cast<double*>(ptr);
		auto str = cstr();
		value = m_parser->to_floating(str.ptr(), str.count(), i);
		return i > 0;
	}
	return false;
}

pointer basic_string_buffer_base::buffer_ptr()
{
	return str().ptr();
}

const_pointer basic_string_buffer_base::buffer_ptr()const
{
	return cstr().ptr();
}

wsize basic_string_buffer_base::buffer_size()const
{
	return capacity() * m_encoder->char_type().size();
}

wsize basic_string_buffer_base::mem_copy(wsize sz, pointer ptr, text::encoding_t format)
{
	//if (format == encoding::binary
	//	|| format == encoding::auto_detect)
	//	throw exception_t(except_code::unsupported);
	auto char_size = m_encoder->char_type().size();
	if (m_map_index != (wsize)invalid_index || m_map_size != (wsize)invalid_index)
	{
		copy_at(raw_str(ptr, min(m_map_size, sz) / char_size, format), m_map_index / char_size);
		return min(m_map_size, sz);
	}
	else {
		copy(raw_str(ptr, sz / char_size, format));
		return length() * char_size;
	}
}

ibuffer_view_t basic_string_buffer_base::map_buffer(windex start, wsize size)
{
	if (m_map_index != (wsize)invalid_index || m_map_size != (wsize)invalid_index)
		return null;
	if ((start + size) > (capacity() * m_encoder->char_type().size()))
		return null;
	m_map_index = start;
	m_map_size = size;
	return this;
}

bool basic_string_buffer_base::unmap_buffer(ibuffer_view_t& view, wsize used)
{
	if (view.get() != static_cast<ibuffer_view*>(this))
		return false;
	length((m_map_index + used) / m_encoder->char_type().size());
	view = null;
	return true;
}

bool basic_string_buffer_base::can_realloc_buffer()const { return (m_map_index == (wsize)invalid_index && m_map_size == (wsize)invalid_index); };

bool basic_string_buffer_base::realloc_buffer(wsize size) { return realloc(size / m_encoder->char_type().size(), true); };

text::encoding_t basic_string_buffer_base::encoding()const
{
	return m_encoder->format();
}

rtti_t const& basic_string_buffer_base::char_type()const
{
	return m_encoder->char_type();
}

raw_str_t  basic_string_buffer_base::str(int) {
	auto char_size = m_encoder->char_type().size();
	if (m_map_index != (wsize)invalid_index || m_map_size != (wsize)invalid_index)
		return storage_type_stack == storage_type() ? raw_str((pointer)(m_data.m_stack_buffer + m_map_index), m_map_size, encoding()) : raw_str(((byte*)m_data.m_allocated_buffer) + m_map_index, m_map_size, encoding());
	else
		return storage_type_stack == storage_type() ? raw_str((pointer)m_data.m_stack_buffer, m_data.m_stack_length * char_size, encoding()) 
		: storage_type_allocated == storage_type() ? raw_str(m_data.m_allocated_buffer, m_data.m_allocated_length * char_size, encoding())
		: m_data.m_const_string->str();
}

raw_cstr_t  basic_string_buffer_base::cstr(int)const {
	auto char_size = m_encoder->char_type().size();
	if (m_map_index != (wsize)invalid_index || m_map_size != (wsize)invalid_index)
		return storage_type_stack == storage_type() ? raw_cstr((pointer)(m_data.m_stack_buffer + m_map_index), m_map_size, encoding()) : raw_cstr(((byte*)m_data.m_allocated_buffer) + m_map_index, m_map_size, encoding());
	else
		return storage_type_stack == storage_type() ? raw_cstr((pointer)m_data.m_stack_buffer, m_data.m_stack_length * char_size, encoding())
		: storage_type_allocated == storage_type() ? raw_cstr(m_data.m_allocated_buffer, m_data.m_allocated_length * char_size, encoding())
		: ((basic_const_string_buffer_base const*)m_data.m_const_string)->cstr();
}

/////////////////////////////////////////////////////////////////////////

unknown_str_t basic_string_buffer_base::data()const {
	return const_cast<basic_string_buffer_base*>(this)->str(0).ptr();
}

comparision_result_t basic_string_buffer_base::compare(object const* obj)const
{
	istring_t other = interface_cast<istring>(const_cast<object*>(obj));
	if (other.is_empty())
		return comparision_result::diferent;
	return (comparision_result)m_encoder->compare(cstr().ptr(), other->cstr().ptr(), other->encoding());
}

basic_string_buffer_base::storage_type_t basic_string_buffer_base::storage_type()const
{
	return m_data.m_storage_type == storage_type_allocated ? storage_type_allocated 
		: m_data.m_storage_type == storage_type_string_pool ? storage_type_string_pool
		: storage_type_stack;
}

void basic_string_buffer_base::length(wsize len)
{
	dword end = 0; wsize i = 0;
	auto char_size = m_encoder->char_type().size();
	if (storage_type_stack == storage_type()) {
		len = min(len, 128u / char_size - 1);
		m_data.m_stack_length = len;
		m_encoder->set_eos(m_data.m_stack_buffer, len); //set end of string
	}
	else if (storage_type_allocated == storage_type()) {
		len = min(len, m_data.m_allocated_capacity - 1);
		m_data.m_allocated_length = len;
		m_encoder->set_eos(m_data.m_allocated_buffer, len); //set end of string
	}
}

bool basic_string_buffer_base::is_empty()const
{
	return (m_data.m_stack_length == 0 || m_data.m_allocated_length == 0) ? true : false;
}

wsize basic_string_buffer_base::length() const
{
	if (m_map_index != (wsize)invalid_index || m_map_size != (wsize)invalid_index)
		return m_map_size;
	else
		return storage_type_stack == storage_type() ? m_data.m_stack_length 
		: storage_type_allocated == storage_type() ? m_data.m_allocated_length
		: m_data.m_const_string->cstr().count();
}

wsize basic_string_buffer_base::capacity() const
{
	if (m_map_index != (wsize)invalid_index || m_map_size != (wsize)invalid_index)
		return m_map_size;
	else
		return storage_type_stack == storage_type() ? 128u / m_encoder->char_type().size() - 1
		: storage_type_allocated == storage_type() ? m_data.m_allocated_capacity - 1
		: m_data.m_const_string->cstr().count();
}

void basic_string_buffer_base::set(basic_const_string_buffer_base* ptr)
{
	clear();
	if (ptr != null && encoding() == ptr->encoding())
	{
		m_data.m_storage_type = storage_type_string_pool;
		m_data.m_const_string = ptr;
		m_data.m_const_string->add_ref();
		m_data.m_const_string_view = ptr->str().ptr();
	}
}

void basic_string_buffer_base::copy(raw_cstr_t str)
{
	wsize sz = (encoding()==str.encoding()?str.count() : m_encoder->lenght(str.ptr())), j = 0;
	wsize char_size = m_encoder->char_type().size();
	if (sz == 0U)
		return;
	auto lc = 128u / char_size;
	if (sz < lc)
	{
		clear();
		m_encoder->convert(m_data.m_stack_buffer, m_data.m_stack_length, str.ptr(), j, str.encoding(), true, lc - 1, sz);
	}
	else
	{
		realloc(sz, false);
		m_encoder->convert(m_data.m_allocated_buffer, m_data.m_allocated_length, str.ptr(), j, str.encoding(), true, m_data.m_allocated_capacity - 1, sz);
	}
}

void basic_string_buffer_base::concat(raw_cstr_t str)
{
	wsize sz = (encoding() == str.encoding() ? str.count() : m_encoder->lenght(str.ptr())), j = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / m_encoder->char_type().size();
	wsize my_len = length();
	realloc(my_len + sz, true);
	if (storage_type_stack == storage_type())
	{
		m_encoder->convert(m_data.m_stack_buffer, m_data.m_stack_length, str.ptr(), j, str.encoding(), true, lc - my_len - 1, sz);
	}
	else if (storage_type_allocated == storage_type())
	{
		m_encoder->convert(m_data.m_allocated_buffer, m_data.m_allocated_length, str.ptr(), j, str.encoding(), true, m_data.m_allocated_capacity - my_len - 1, sz);
	}
	else
	{	
		basic_const_string_buffer_base* _const_string = m_data.m_const_string;
		auto txt = _const_string->cstr();
		memset(&m_data, 0, sizeof(m_data));
		realloc(txt.count() + str.count());
		copy(txt);
		concat(str);
		_const_string->release();
	}
}

void basic_string_buffer_base::copy_at(raw_str_t str, windex at)
{
	wsize sz = (encoding() == str.encoding() ? str.count() : m_encoder->lenght(str.ptr())), j = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / m_encoder->char_type().size();
	wsize my_len = min(length(), at);
	realloc(my_len + sz, true);
	if (storage_type_stack == storage_type())
	{
		m_data.m_stack_length = my_len;
		m_encoder->convert(m_data.m_stack_buffer, m_data.m_stack_length, str.ptr(), j, str.encoding(), true, lc - my_len - 1, sz);
	}
	else if (storage_type_allocated == storage_type())
	{
		m_data.m_allocated_length = my_len;
		m_encoder->convert(m_data.m_allocated_buffer, m_data.m_allocated_length, str.ptr(), j, str.encoding(), true, m_data.m_allocated_capacity - my_len - 1, sz);
	}
	else
	{
		basic_const_string_buffer_base* _const_string = m_data.m_const_string;
		auto txt = _const_string->cstr();
		memset(&m_data, 0, sizeof(m_data));
		realloc(txt.count() + str.count());
		copy(txt);
		copy_at(str, at);
		_const_string->release();
	}
}

void basic_string_buffer_base::insert(windex at, raw_cstr_t str)
{
	wsize sz = (encoding() == str.encoding() ? str.count() : m_encoder->lenght(str.ptr())), j = 0;
	if (sz == 0U)
		return;
	auto lc = 128u / m_encoder->char_type().size();
	wsize my_len = length();
	realloc(my_len + sz, true);
	if (storage_type_stack == storage_type())
	{
		m_data.m_stack_length = my_len;
		m_encoder->convert(m_data.m_stack_buffer, m_data.m_stack_length, str.ptr(), j, str.encoding(), true, lc - my_len - 1, sz);
	}
	else if (storage_type_allocated == storage_type())
	{
		m_data.m_allocated_length = my_len;
		m_encoder->convert(m_data.m_allocated_buffer, m_data.m_allocated_length, str.ptr(), j, str.encoding(), true, m_data.m_allocated_capacity - my_len - 1, sz);
	}
	else
	{
		basic_const_string_buffer_base* _const_string = m_data.m_const_string;
		auto txt = _const_string->cstr();
		memset(&m_data, 0, sizeof(m_data));
		realloc(txt.count() + str.count());
		copy(txt);
		insert(at, str);
		_const_string->release();
	}
}

void basic_string_buffer_base::format(raw_cstr_t cstr, var_args_t args)
{
	int a;
	text::text_format_flags_t f;
	iencoder_t encoder = iencoder::get_encoder(cstr.encoding());
	iparser_t parser = iparser::get_parser(cstr.encoding());
	wsize i = 0, t = 0, n = 0, l = 0, cs = encoder->char_type().size();
	clear();
	
	while (char32_t c = encoder->to_char32(cstr.ptr(), i))
	{
		if (c == U'{') {
			f.value = parser->parse(cstr.ptr(), cstr.size(), n, a).format_flags();
			if (a > -1 && a < (long64)args->size()) {
				concat(raw_cstr(&((byte*)cstr.ptr())[l * cs], (t - l) *cs, cstr.encoding()));
				concat(f.value ? (raw_cstr_t)args[a]->to_string(f) : (raw_cstr_t)args[a]->to_string());
				i = l = n;
			}
		}
		t = n = i;
	}
	concat(raw_cstr(&((byte*)cstr.ptr())[l * cs], (i - l) * cs, cstr.encoding()));
}

void basic_string_buffer_base::concat_format(raw_cstr_t cstr, var_args_t args)
{
	int a;
	text::text_format_flags_t f;
	iencoder_t encoder = iencoder::get_encoder(cstr.encoding());
	iparser_t parser = iparser::get_parser(cstr.encoding());
	wsize i = 0, t = 0, n = 0, l = 0, cs = encoder->char_type().size();
	//clear();

	while (char32_t c = encoder->to_char32(cstr.ptr(), i))
	{
		if (c == U'{') {
			f.value = parser->parse(cstr.ptr(), cstr.size(), n, a).format_flags();
			if (a > -1 && a < (long64)args->size()) {
				concat(raw_cstr(&((byte*)cstr.ptr())[l * cs], (t - l) *cs, cstr.encoding()));
				concat(f.value ? (raw_cstr_t)args[a]->to_string(f) : (raw_cstr_t)args[a]->to_string());
				i = l = n;
			}
		}
		t = n = i;
	}
	concat(raw_cstr(&((byte*)cstr.ptr())[l * cs], (i - l) * cs, cstr.encoding()));
}

void basic_string_buffer_base::insert_format(windex, raw_cstr_t, var_args_t)
{

}

int basic_string_buffer_base::compare(raw_cstr_t str)const
{
	return m_encoder->compare(cstr().ptr(), str.ptr(), str.encoding());
}

windex basic_string_buffer_base::compare_until(raw_cstr_t str)const
{
	return m_encoder->compare_until(cstr().ptr(), str.ptr(), str.encoding());
}

windex basic_string_buffer_base::find(raw_cstr_t str, windex start, windex end)const
{
	auto my_data = cstr();
	return m_encoder->find(my_data.ptr(), min(my_data.count(), end), str.ptr(), str.count(), str.encoding(), start);
}

windex basic_string_buffer_base::find_reverse(raw_cstr_t str, windex start, windex end)const
{
	auto my_data = cstr();
	return m_encoder->find_reverse(my_data.ptr(), min(my_data.count(), end), str.ptr(), str.count(), str.encoding(), start);
}

istring_t basic_string_buffer_base::sub_string(istring_t str, windex start, windex end)const
{
	if (str.is_empty() || start >= end || start > length())
		return str;

	auto encoder = iencoder::get_encoder(str->encoding());
	auto my_data = cstr();
	wsize cs = encoder->char_type().size();
	raw_cstr_t raw = raw_cstr((byte*)my_data.ptr() + start * cs, cs * (end - start), encoder->format());
	str->copy(raw);
	return str;
}

///////////////////////////////////////////////////////////////////////////////////
#define MY_ALLOCATOR ang::memory::buffer_allocator

#define MY_ENCODING ang::text::encoding::ascii
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::unicode
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf8
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_se
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_le
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf16_be
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_se
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_le
#include "inline/string.inl"
#undef MY_ENCODING

#define MY_ENCODING ang::text::encoding::utf32_be
#include "inline/string.inl"
#undef MY_ENCODING

