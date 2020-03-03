/*********************************************************************************************************************/
/*   File Name: input_binary_file.cpp                                                                                  */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include <ang/core/files.h>
#include "format_parser.h"

using namespace ang;
using namespace ang::streams;
using namespace ang::core::files;

input_binary_file::input_binary_file()
	: base()
{
}

input_binary_file::input_binary_file(cstr_t path)
	: base()
{
	open(path);
}

input_binary_file::~input_binary_file()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::core::files::input_binary_file)
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::core::files::input_binary_file, file, streams::ibinary_input_stream);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::core::files::input_binary_file, file, streams::ibinary_input_stream);

bool input_binary_file::open(cstr_t path)
{
	if (is_valid())
		return false;
	if (!create(path, open_flags::access_in + open_flags::open_exist + open_flags::format_text))
		return false;
	return true;
}

bool input_binary_file::is_eos()const
{
	return m_hfile.is_empty() ? true : m_hfile->is_eof();
}

text::encoding_t input_binary_file::format()const
{
	return m_hfile.is_empty() ? text::encoding::none : m_hfile->format().get();
}

file_offset_t input_binary_file::cursor()const
{
	return m_hfile.is_empty() ? 0 : m_hfile->cursor();
}

bool input_binary_file::cursor(file_offset_t offset, stream_reference_t ref)
{
	return m_hfile.is_empty() ? false : m_hfile->cursor(offset, ref);
}

file_offset_t input_binary_file::size()const
{
	return m_hfile.is_empty() ? 0 : m_hfile->size();
}

stream_mode_t input_binary_file::mode()const
{
	return stream_mode::out;
}

bool input_binary_file::map(function<bool(ibuffer_view_t)> func, wsize sz, file_offset_t offset)
{
	ibuffer_t buff = file::map(min((wsize)size(), sz), min(size(), offset));
	if (buff.is_empty())
		return false;
	func(buff.get());
	unmap(buff, min((wsize)size(), sz));
	return true;
}

//bool input_binary_file::map(function<bool(text::istring_view_t)> func, wsize sz, file_offset_t offset)
//{
//	ibuffer_t buff = file::map(min(size(), sz), min(size(), offset));
//	if (buff.is_empty())
//		return false;
//	func(text::istring_factory::get_factory(format())->create_wrapper((ibuffer_view*)buff.get()));
//	unmap(buff, min(size(), sz));
//	return true;
//}

bool input_binary_file::read(function<bool(streams::ibinary_input_stream_t)> func)
{
	return func(this);
}

wsize input_binary_file::read(pointer ptr, wsize sz)
{
	if (m_hfile.is_empty() || ptr == null || sz == 0)
		return 0U;
	auto current = m_hfile->cursor();
	return m_hfile->read(sz, ptr) - current;
}

wsize input_binary_file::read(ibuffer_t buff)
{
	if (m_hfile.is_empty() || buff.is_empty())
		return 0U;
	auto current = m_hfile->cursor();
	return m_hfile->read(buff->buffer_size(), buff->buffer_ptr()) - current;
}

#define FUNCX_BINARY_READ(A0) if(type.type_id() == type_of<A0>().type_id()) { return read(ptr, size_of<A0>()); }
#define BINARY_READ_SWITCH(...) { ANG_EXPAND(APPLY_FUNCX_N(FUNCX_BINARY_READ, ELSE_SEPARATOR,##__VA_ARGS__)) }
 
wsize input_binary_file::read(pointer ptr, const rtti_t& type)
{
	BINARY_READ_SWITCH(
		char,
		byte,
		wchar_t,
		char16_t,
		char32_t,
		short,
		ushort,
		int,
		uint,
		long,
		ulong,
		long64,
		ulong64,
		float,
		double
	);
	return 0;
}
