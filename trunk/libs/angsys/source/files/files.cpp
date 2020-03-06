/*********************************************************************************************************************/
/*   File Name: files.cpp                                                                                            */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <ang/core/files.h>
#include "file_system.h"

using namespace ang;
using namespace ang::core::files;

ANG_ENUM_IMPLEMENT(ang::core::files, file_system_priority);
ANG_FLAGS_IMPLEMENT(ang::core::files, open_flags);
ANG_FLAGS_IMPLEMENT(ang::core::files, path_access_type);


static collections::pair<file_system_priority, castr_t> s_file_system_priority_to_string_map[] =
{
	{file_system_priority::lowest, "lowest"},
	{file_system_priority::highest, "highest"}
};

static collections::pair<castr_t, file_system_priority> s_file_system_priority_parsing_map[] =
{
	{"highest", file_system_priority::highest},
	{"lowest", file_system_priority::lowest}
};

ANG_ENUM_PARSE_IMPLEMENT(file_system_priority, s_file_system_priority_parsing_map, file_system_priority::lowest);
ANG_ENUM_TO_STRING_IMPLEMENT(file_system_priority, s_file_system_priority_to_string_map, "unknown"_sv)

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
	case text::encoding::utf16_le: return get_encoding<text::encoding::utf16_le>();
	case text::encoding::utf16_be: return get_encoding<text::encoding::utf16_be>();
	case text::encoding::utf32_le: return get_encoding<text::encoding::utf32_le>();
	case text::encoding::utf32_be: return get_encoding<text::encoding::utf32_be>();
	case text::encoding::unicode: return get_encoding<text::encoding::unicode>();
	case text::encoding::utf16: return get_encoding<text::encoding::utf16>();
	case text::encoding::utf32: return get_encoding<text::encoding::utf32>();
	case text::encoding::utf8: return get_encoding<text::encoding::utf8>();
	default:  return open_flags::format_utf8; //utf8 for default
	}
}

text::encoding_t ang::core::files::get_format(open_flags_t encoding)
{
	if (!(encoding & open_flags::format_text))
		return text::encoding::binary;

	switch ((encoding & (open_flags)0X1F).get())
	{
	case open_flags::format_ascii: return text::encoding::ascii;
	case open_flags::format_utf16_le: return text::encoding::utf16_le;
	case open_flags::format_utf16_be: return text::encoding::utf16_be;
	case open_flags::format_utf32_le: return text::encoding::utf32_le;
	case open_flags::format_utf32_be: return text::encoding::utf32_be;
	//case open_flags::format_utf8: return text::encoding::utf8;
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

bool file::create(cstr_t path, open_flags_t flags)
{
	auto fs = ifile_system::instance();
	if (fs == null)
		return false;
	if (!fs->create_handle(path , flags, &m_hfile))
		return false;

	return true;
}

bool file::attach(ifile* f)
{
	dispose();
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

void file::dispose()
{
	m_hfile = null;
}
