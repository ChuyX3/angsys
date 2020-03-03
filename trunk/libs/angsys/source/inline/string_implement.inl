

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer()
{
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(cstr_t cstr)
{
	copy(cstr);
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(basic_string_buffer && str)
{
	move(ang::move(str));
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::basic_string_buffer(fast_string_t && str)
{
	move(ang::move(str));
}

basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::~basic_string_buffer()
{

}

void basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::dispose()
{
	clear();
}

rtti_t const& basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::value_type()const
{
	return class_info();
}

bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::set_value(rtti_t const& id, unknown_t ptr)
{
	if (id.is_type_of(istring::class_info()))
	{
		string str = interface_cast<istring>((iintf*)ptr);
		if (!str)
			return false;
		copy(str->cstr());
		return true;
	}
	else if (id.is_type_of(type_of<char*>()) || id.is_type_of(type_of<char const*>()))
	{
		char const* str = reinterpret_cast<char const*>(ptr);
		fast_string::copy(castr_t(str, text::text_encoder<text::encoding::ascii>::lenght(str)));
		return true;
	}
	else if (id.is_type_of(type_of<mchar*>()) || id.is_type_of(type_of<mchar const*>()))
	{
		mchar const* str = reinterpret_cast<mchar const*>(ptr);
		fast_string::copy(cmstr_t(str, text::text_encoder<text::encoding::utf8>::lenght(str)));
		return true;
	}
	else if (id.is_type_of(type_of<wchar*>()) || id.is_type_of(type_of<wchar const*>()))
	{
		wchar const* str = reinterpret_cast<wchar const*>(ptr);
		fast_string::copy(cwstr_t(str, text::text_encoder<text::encoding::unicode>::lenght(str)));
		return true;
	}
	else if (id.is_type_of(type_of<char16_t*>()) || id.is_type_of(type_of<char16_t const*>()))
	{
		char16_t const* str = reinterpret_cast<char16_t const*>(ptr);
		fast_string::copy(cstr16_t(str, text::text_encoder<text::encoding::utf16>::lenght(str)));
		return true;
	}
	else if (id.is_type_of(type_of<char32_t*>()) || id.is_type_of(type_of<char32_t const*>()))
	{
		char32_t const* str = reinterpret_cast<char32_t const*>(ptr);
		fast_string::copy(cstr32_t(str, text::text_encoder<text::encoding::utf32>::lenght(str)));
		return true;
	}
	else if (id.is_type_of(type_of<castr_t>()))
	{
		castr_t& str = *reinterpret_cast<castr_t*>(ptr);
		fast_string::copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cwstr_t>()))
	{
		cwstr_t& str = *reinterpret_cast<cwstr_t*>(ptr);
		fast_string::copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cmstr_t>()))
	{
		cmstr_t& str = *reinterpret_cast<cmstr_t*>(ptr);
		fast_string::copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cstr16_t>()))
	{
		cstr16_t& str = *reinterpret_cast<cstr16_t*>(ptr);
		fast_string::copy(str);
		return true;
	}
	else if (id.is_type_of(type_of<cstr32_t>()))
	{
		cstr32_t& str = *reinterpret_cast<cstr32_t*>(ptr);
		fast_string::copy(str);
		return true;
	}

	return false;
}

bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::get_value(rtti_t const& id, unknown_t ptr)const
{
	if (id.is_type_of(istring::class_info()))
	{
		string str = interface_cast<istring>((iintf*)ptr);
		if (!str)
			return false;
		str->copy(cstr());
		return true;
	}
	else if (id.is_type_of<astr_t>())
	{
		astr_t& dest = *reinterpret_cast<astr_t*>(ptr);
		auto src = fast_string::cstr();
		wsize i = 0, j = 0;
		text::text_encoder<encoding::ascii>::convert(dest.str(), i, src, j, true, dest.size(), src.size());
		dest.set(dest.str(), i);
		return true;
	}
	else if (id.is_type_of<wstr_t>())
	{
		wstr_t& dest = *reinterpret_cast<wstr_t*>(ptr);
		auto src = fast_string::cstr();
		wsize i = 0, j = 0;
		text::text_encoder<encoding::unicode>::convert(dest.str(), i, src, j, true, dest.size(), src.size());
		dest.set(dest.str(), i);
		return true;
	}
	else if (id.is_type_of<mstr_t>())
	{
		mstr_t& dest = *reinterpret_cast<mstr_t*>(ptr);
		auto src = fast_string::cstr();
		wsize i = 0, j = 0;
		text::text_encoder<encoding::utf8>::convert(dest.str(), i, src, j, true, dest.size(), src.size());
		dest.set(dest.str(), i);
		return true;
	}
	else if (id.is_type_of<str16_t>())
	{
		str16_t& dest = *reinterpret_cast<str16_t*>(ptr);
		auto src = fast_string::cstr();
		wsize i = 0, j = 0;
		text::text_encoder<encoding::utf16>::convert(dest.str(), i, src, j, true, dest.size(), src.size());
		dest.set(dest.str(), i);
		return true;
	}
	else if (id.is_type_of<str32_t>())
	{
		str32_t& dest = *reinterpret_cast<str32_t*>(ptr);
		auto src = fast_string::cstr();
		wsize i = 0, j = 0;
		text::text_encoder<encoding::utf32>::convert(dest.str(), i, src, j, true, dest.size(), src.size());
		dest.set(dest.str(), i);
		return true;
	}
	else if (id.is_type_of<raw_str_t>())
	{
		raw_str_t& dest = *reinterpret_cast<raw_str_t*>(ptr);
		//auto src = fast_string::cstr();
		//wsize i = 0, j = 0;
		//text::encoder<encoding::utf8>::convert(dest.str(), i, src, j, true, dest.size(), src.size());
		//dest.set(dest.str(), i);
		return true;
	}
	else if (id.is_type_of<short>())
	{
		windex i = 0;
		short& value = *reinterpret_cast<short*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<short>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<ushort>())
	{
		windex i = 0;
		ushort& value = *reinterpret_cast<ushort*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<ushort>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<int>())
	{
		windex i = 0;
		int& value = *reinterpret_cast<int*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<int>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<uint>())
	{
		windex i = 0;
		uint& value = *reinterpret_cast<uint*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<uint>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<long>())
	{
		windex i = 0;
		long& value = *reinterpret_cast<long*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<long>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<ulong>())
	{
		windex i = 0;
		ulong& value = *reinterpret_cast<ulong*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<ulong>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<long64>())
	{
		windex i = 0;
		long64& value = *reinterpret_cast<long64*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<long64>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<ulong64>())
	{
		windex i = 0;
		ulong64& value = *reinterpret_cast<ulong64*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<ulong64>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<float>())
	{
		windex i = 0;
		float& value = *reinterpret_cast<float*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<float>(fast_string::cstr(), i);
		return i > 0;
	}
	else if (id.is_type_of<double>())
	{
		windex i = 0;
		double& value = *reinterpret_cast<double*>(ptr);
		value = text_parser<MY_ENCODING>::str_to_value<double>(fast_string::cstr(), i);
		return i > 0;
	}
	return false;
}

variant basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::clone()const
{
	return (ivariable*)new basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>(cstr());
}

string basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::to_string()const
{
	return const_cast<basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>*>(this);
}

string basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::to_string(text::text_format_t)const
{
	return const_cast<basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>*>(this);
}

encoding_t basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::encoding()const
{
	return MY_ENCODING;
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::length()const
{
	return fast_string::length();
}

pointer basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::data()const
{
	return (void*)fast_string::cstr().cstr();
}

rtti_t const& basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::char_type()const
{
	return type_of<type>();
}

str_t basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::str(int)
{
	return fast_string::str();
}

cstr_t basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::cstr(int)const
{
	return fast_string::cstr();
}

char32_t basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::at(windex i, wsize* sz)const
{
	windex j = i;
	auto c = to_char32<false, is_endian_swapped<MY_ENCODING>::value>(fast_string::cstr(), j);
	if (sz)*sz = j - i;
	return c;
}

int basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::compare(cstr_t val)const
{
	return string_operations<MY_ENCODING>::compare(*this, val);
}

int basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::compare(string val)const
{
	return string_operations<MY_ENCODING>::compare(*this, val);
}

windex basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::compare_until(cstr_t val)const
{
	return string_operations<MY_ENCODING>::compare_until(*this, val);
}

windex basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::compare_until(string val)const
{
	return string_operations<MY_ENCODING>::compare_until(*this, val);
}

windex basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::find(cstr_t val, windex start, windex end)const
{
	return string_operations<MY_ENCODING>::find(*this, val, start, end);
}

windex basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::find(string val, windex start, windex end)const
{
	return string_operations<MY_ENCODING>::find(*this, val, start, end);
}

windex basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::find_reverse(cstr_t val, windex start, windex end)const
{
	return string_operations<MY_ENCODING>::find_reverse(*this, val, start, end);
}

windex basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::find_reverse(string val, windex start, windex end)const
{
	return string_operations<MY_ENCODING>::find_reverse(*this, val, start, end);
}

string basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::sub_string(windex start, windex end)const
{
	basic_string<MY_ENCODING, MY_ALLOCATOR> out = new basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>();
	fast_string::sub_string(*out,start, end);
	return out.get();
}

vector<string> basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::split(cstr_t val)const
{
	return string_operations<MY_ENCODING>::split(*this, val);
}

vector<string> basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::split(string val)const
{
	return string_operations<MY_ENCODING>::split(*this, val);
}

void basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::clear()
{
	fast_string::clear();
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::copy(cstr_t val)
{
	return string_operations<MY_ENCODING>::copy(*this, val);
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::copy(string val)
{
	return string_operations<MY_ENCODING>::copy(*this, val);
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::append(cstr_t val)
{
	return string_operations<MY_ENCODING>::append(*this, val);
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::append(string val)
{
	return string_operations<MY_ENCODING>::append(*this, val);
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::insert(cstr_t val, windex i)
{
	return string_operations<MY_ENCODING>::insert(*this, val, i);
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::insert(string val, windex i)
{
	return string_operations<MY_ENCODING>::insert(*this, val, i);
}

void basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::format(cstr_t val, var_args_t args)
{
	string_operations<MY_ENCODING>::format(this, val, ang::forward<var_args_t>(args));
}

void basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::format(string val, var_args_t args)
{
	string_operations<MY_ENCODING>::format(this, val, ang::forward<var_args_t>(args));
}

void basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::append_format(cstr_t val, var_args_t args)
{
	string_operations<MY_ENCODING>::append_format(this, val, ang::forward<var_args_t>(args));
}

void basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::append_format(string val, var_args_t args)
{
	string_operations<MY_ENCODING>::append_format(this, val, ang::forward<var_args_t>(args));
}

bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::is_readonly()const
{
	return false;
}

pointer basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::buffer_ptr()
{
	return fast_string::str().str();
}

const_pointer basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::buffer_ptr()const 
{
	return fast_string::cstr().cstr();
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::buffer_size()const
{
	//TODO:when buffer is mapped, returns capacity
	return fast_string::length() * size_of<type>();
}

wsize basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::mem_copy(wsize sz, pointer ptr, text::encoding_t e)
{
	return copy(cstr_t(ptr, sz, e));
}

ibuffer_view_t basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::map_buffer(windex, wsize)
{
	//TODO:
	return null;
}

bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::unmap_buffer(ibuffer_view_t&, wsize used)
{
	//TODO:
	return false;
}

bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::can_realloc_buffer()const
{
	return true;
}

bool basic_string_buffer<MY_ENCODING, MY_ALLOCATOR>::realloc_buffer(wsize sz)
{
	fast_string::realloc(sz, true);
	return true;
}
