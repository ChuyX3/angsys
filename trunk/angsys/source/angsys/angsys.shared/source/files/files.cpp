/*********************************************************************************************************************/
/*   File Name: files.cpp                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <angsys.h>
#include <ang/core/files.h>
#include "file_system.h"

using namespace ang;
using namespace ang::core::files;

safe_flags_implement(ang::core::files, open_flags);

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::files::ifile, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::core::files::ifile_system, interface);

safe_enum_rrti2(ang::core::files, file_system_priority);

//#define  MY_TYPE ang::collections::pair<ang::core::files::path, ang::core::files::pack_file_info>
//#define MY_ALLOCATOR ang::memory::default_allocator
//#include <ang/collections/inline/array_object_specialization.inl>
//#undef MY_TYPE
//#undef MY_ALLOCATOR

static collections::pair<file_system_priority, cstr_t> s_file_system_priority_to_string_map[] =
{
	{file_system_priority::lowest, "lowest"_s},
	{file_system_priority::highest, "highest"_s}
};

static collections::pair<cstr_t, file_system_priority> s_file_system_priority_parsing_map[] =
{
	{"highest"_s, file_system_priority::highest},
	{"lowest"_s, file_system_priority::lowest}
};


file_system_priority_t file_system_priority_t::parse(text::raw_cstr_t str)
{
	auto idx = algorithms::binary_search(str, collections::to_array(s_file_system_priority_parsing_map));
	if (idx >= algorithms::array_size(s_file_system_priority_parsing_map))
		return file_system_priority::lowest;
	else
		return s_file_system_priority_parsing_map[idx].value;
}

wstring file_system_priority_t::to_string()const
{
	auto idx = algorithms::binary_search(get(), collections::to_array(s_file_system_priority_to_string_map));
	if (idx >= algorithms::array_size(s_file_system_priority_to_string_map))
		return "unknown"_s;
	else
		return s_file_system_priority_to_string_map[idx].value;
}

template<text::encoding> open_flags_t get_encoding();

template<> open_flags_t get_encoding<text::encoding::ascii>() {
	return open_flags::format_ascii;
}

template<> open_flags_t get_encoding<text::encoding::utf8>() {
	return open_flags::format_utf8;
}

template<> open_flags_t get_encoding<text::encoding::utf16_le>() {
	return open_flags::format_utf16_le;
}

template<> open_flags_t get_encoding<text::encoding::utf16_be>() {
	return open_flags::format_utf16_be;
}

template<> open_flags_t get_encoding<text::encoding::utf32_le>() {
	return open_flags::format_utf32_le;
}

template<> open_flags_t get_encoding<text::encoding::utf32_be>() {
	return open_flags::format_utf32_be;
}

template<> open_flags_t get_encoding<text::encoding::utf16>() {
	return get_encoding<text::native_encoding<text::encoding::utf16>::value>();
}

template<> open_flags_t get_encoding<text::encoding::utf16_se>() {
	return get_encoding<text::native_inverse_encoding<text::encoding::utf16>::value>();
}

template<> open_flags_t get_encoding<text::encoding::utf32>() {
	return get_encoding<text::native_encoding<text::encoding::utf32>::value>();
}

template<> open_flags_t get_encoding<text::encoding::utf32_se>() {
	return get_encoding<text::native_inverse_encoding<text::encoding::utf32>::value>();
}

template<> open_flags_t get_encoding<text::encoding::unicode>() {
	return get_encoding<text::native_encoding<text::encoding::unicode>::value>();
}


open_flags_t ang::core::files::get_format(text::encoding_t encoding)
{
	switch (encoding.get())
	{
	case text::encoding::ascii: return get_encoding<text::encoding::ascii>();
	case text::encoding::utf8: return get_encoding<text::encoding::utf8>();
	case text::encoding::utf16_le: return get_encoding<text::encoding::utf16_le>();
	case text::encoding::utf16_be: return get_encoding<text::encoding::utf16_be>();
	case text::encoding::utf32_le: return get_encoding<text::encoding::utf32_le>();
	case text::encoding::utf32_be: return get_encoding<text::encoding::utf32_be>();
	case text::encoding::unicode: return get_encoding<text::encoding::unicode>();
	case text::encoding::utf16: return get_encoding<text::encoding::utf16>();
	case text::encoding::utf32: return get_encoding<text::encoding::utf32>();
	default:  return open_flags::format_utf8; //utf8 for default
	}
}

text::encoding_t ang::core::files::get_format(open_flags_t encoding)
{
	if (!(encoding & open_flags::format_text))
		return text::encoding::binary;

	switch ((encoding.get() & (open_flags)0X1F).get())
	{
	case open_flags::format_ascii: return text::encoding::ascii;
	case open_flags::format_utf8: return text::encoding::utf8;
	case open_flags::format_utf16_le: return text::encoding::utf16_le;
	case open_flags::format_utf16_be: return text::encoding::utf16_be;
	case open_flags::format_utf32_le: return text::encoding::utf32_le;
	case open_flags::format_utf32_be: return text::encoding::utf32_be;
	default:  return text::encoding::utf8; //utf8 for default
	}
}


////////////////////////////////////////////////////////////////////////////////////

file::file()
	: m_hfile(null)

{
}

file::~file()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::files::file);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::files::file, object);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::files::file, object);


bool file::create(path_view_t path, open_flags_t flags)
{
	auto fs = ifile_system::fs_instance();
	if (fs == null)
		return false;
	if (!fs->open_file(path , flags, &m_hfile))
		return false;

	return true;
}

bool file::attach(ifile* f)
{
	clear();
	m_hfile = f;
	return true;
}

ibuffer_t file::map(wsize size, file_offset_t offset)
{
	if (m_hfile.is_empty())
		return null;
	return m_hfile->map(size, offset);
}

bool file::unmap(ibuffer_t buffer, wsize size)
{
	if (buffer.is_empty())
		return false;
	return m_hfile->unmap(buffer, size);
}

bool file::is_valid()const
{
	return !m_hfile.is_empty();
}

//file_size_t file::size()const
//{
//	return is_valid() ? m_hfile->stream_size() : 0;
//}

//bool file::size(file_size_t size)
//{
//	if(m_hfile.is_empty())
//		return false;
//
//	if (m_hfile->stream_size() > size)
//		return m_hfile->stream_size(size);
//	file_size_t pos = m_hfile->position();
//	switch (m_hfile->mode().get())
//	{
//	case streams::stream_mode::out:
//	case streams::stream_mode::inout: 		
//		m_hfile->move_to(size, file_reference::begin);
//		m_hfile->move_to(pos, file_reference::begin);
//		return true;
//	default:
//		return false;
//	}
//}

streams::stream_mode_t file::mode()const
{
	return is_valid() ? m_hfile->mode().get() : streams::stream_mode::unknow;
}

bool file::set_mutex(core::async::mutex_ptr_t mutex)
{
	return is_valid() ? m_hfile->set_mutex(mutex) : false;
}

void file::clear()
{
	m_hfile = null;
}
