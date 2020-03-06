#include  "pch.h"
#include "xml/xml_implement.h"

using namespace coffe;
using namespace coffe::dom;
using namespace coffe::dom::xml;

xml_text::xml_text()
{
}

xml_text::xml_text(cstr_t cstr)
{
	copy(cstr);
}

xml_text::~xml_text()
{

}

rtti_t const& xml_text::class_info() {
	static rtti_t const* parents[] = TYPE_OF_PTR_ARRAY(object, ixml_text);
	rtti_t const& info = rtti::regist(
		"xml_text"_s
		, genre::class_type
		, size_of<xml_text>()
		, align_of<xml_text>());
	return info;
}

rtti_t const& xml_text::runtime_info()const
{
	return class_info();
}

bool xml_text::query_interface(rtti_t const& id, unknown_ptr_t out)
{
	if (id.type_id() == xml_text::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<xml_text*>(this);
		return true;
	}
	else if (object::query_interface(id, out))
	{
		return true;
	}
	else if (id.type_id() == ixml_text::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<ixml_text*>(this);
		return true;
	}
	else if (id.type_id() == ixml_object::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<ixml_object*>(this);
		return true;
	}
	else if (id.type_id() == text::istring::class_info().type_id())
	{
		if (out == null)
			return false;
		*out = static_cast<text::istring*>(this);
		return true;
	}
	return false;
}

void xml_text::dispose()
{
	clear();
}

xml_type_t xml_text::type()const
{
	return xml_type::text;
}

bool xml_text::is_type_of(xml_type_t ty)const
{
	return ty == xml_type::text;
}

text::encoding_t xml_text::encoding()const
{
	return text::encoding::utf8;
}

void xml_text::clear()
{
	fast_string::clear();
}

wsize xml_text::length()const
{
	return fast_string::length();
}

pointer xml_text::data()const
{
	return (pointer)fast_string::cstr().cstr();
}

rtti_t const& xml_text::char_type()const
{
	return type_of<byte>();
}

str_t xml_text::str(int)
{
	return fast_string::str();
}

cstr_t xml_text::cstr(int)const
{
	return fast_string::cstr();
}

char32_t xml_text::at(windex i, wsize*sz)const
{
	windex j = i;
	auto c = text::to_char32<false, false>(fast_string::cstr(), j);
	if (sz)*sz = j - i;
	return c;
}

int xml_text::compare(cstr_t val)const
{
	return string_operations::compare(*this, val);
}

int xml_text::compare(string val)const
{
	return string_operations::compare(*this, val);
}

windex xml_text::compare_until(cstr_t val)const 
{
	return string_operations::compare_until(*this, val);
}

windex xml_text::compare_until(string val)const
{
	return string_operations::compare_until(*this, val);
}

windex xml_text::find(cstr_t val, windex start, windex end)const
{
	return string_operations::find(*this, val, start, end);
}

windex xml_text::find(string val, windex start, windex end)const
{
	return string_operations::find(*this, val, start, end);
}

windex xml_text::find_reverse(cstr_t val, windex start, windex end)const
{
	return string_operations::find_reverse(*this, val, start, end);
}

windex xml_text::find_reverse(string val, windex start, windex end)const
{
	return string_operations::find_reverse(*this, val, start, end);
}

string xml_text::sub_string(windex start, windex end)const
{
	text::basic_string<text::encoding::utf8> out = new text::basic_string_buffer<text::encoding::utf8>();
	fast_string::sub_string(*out, start, end);
	return out.get();
}

fast_vector<string> xml_text::split(cstr_t val)const
{
	return string_operations::split(*this, val);
}

fast_vector<string> xml_text::split(string val)const
{
	return string_operations::split(*this, val);
}

wsize xml_text::copy(cstr_t val)
{
	return string_operations::copy(*this, val);
}

wsize xml_text::copy(string val)
{
	return string_operations::copy(*this, val);
}

wsize xml_text::append(cstr_t)
{ 
	return 0; 
}

wsize xml_text::append(string)
{
	return 0;
}

wsize xml_text::insert(cstr_t, windex)
{ 
	return 0;
}

wsize xml_text::insert(string, windex)
{ 
	return 0;
}

void xml_text::format(cstr_t, var_args_t)
{ 
}

void xml_text::format(string, var_args_t)
{
}

void xml_text::append_format(cstr_t, var_args_t)
{
}

void xml_text::append_format(string, var_args_t)
{
}

		