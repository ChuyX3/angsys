/*********************************************************************************************************************/
/*   File Name: streams.cpp                                                                                          */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) Angsys, - All Rights Reserved                                                                     */
/*   Confidential Information of Angsys. Not for disclosure or distribution without the author's prior written       */
/*   consent. This file contains code, techniques and know-how which is confidential and proprietary to Jesus Rocha  */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "ang/streams.h"

using namespace ang;
using namespace ang::streams;

//ANG_IMPLEMENT_INTERFACE(ang::streams, iserializable);
ANG_IMPLEMENT_INTERFACE(ang::streams, iinput_stream);
ANG_IMPLEMENT_INTERFACE(ang::streams, ioutput_stream);
ANG_IMPLEMENT_INTERFACE(ang::streams, itext_input_stream);
ANG_IMPLEMENT_INTERFACE(ang::streams, itext_output_stream);
ANG_IMPLEMENT_INTERFACE(ang::streams, ibinary_input_stream);
ANG_IMPLEMENT_INTERFACE(ang::streams, ibinary_output_stream);

stream_fromat_t itext_input_stream::format()const { return stream_fromat::text; }
stream_fromat_t itext_output_stream::format()const { return stream_fromat::text; }
stream_fromat_t ibinary_input_stream::format()const { return stream_fromat::binary; }
stream_fromat_t ibinary_output_stream::format()const { return stream_fromat::binary; }

ANG_IMPLEMENT_ENUM(streams, stream_mode, byte, stream_mode::unknow);
cstr_t stream_mode_t::to_string()const
{
	switch (_value)
	{
	default:
	case ang::streams::stream_mode::unknow:
		return "unknow";
	case ang::streams::stream_mode::in:
		return "in";
	case ang::streams::stream_mode::out:
		return "out";
	}
}

ANG_IMPLEMENT_ENUM(streams, stream_fromat, byte, stream_fromat::binary);
cstr_t stream_fromat_t::to_string()const
{
	switch (_value)
	{
	default:
		return "invalid";
	case ang::streams::stream_fromat::binary:
		return "binary";
	case ang::streams::stream_fromat::text:
		return "text";
	}
}

ANG_IMPLEMENT_ENUM(streams, stream_reference, byte, stream_reference::begin);
cstr_t stream_reference_t::to_string()const
{
	switch (_value)
	{
	default:
		return "invalid";
	case ang::streams::stream_reference::begin:
		return "begin";
	case ang::streams::stream_reference::current:
		return "current";
	case ang::streams::stream_reference::end:
		return "end";
	}
}

ANG_IMPLEMENT_ENUM(streams, special_command, byte, special_command::none);
cstr_t special_command_t::to_string()const
{
	switch (_value)
	{
	default:
		return "none";
	case ang::streams::special_command::end_line:
		return "end_line";
	case ang::streams::special_command::space:
		return "space";
	case ang::streams::special_command::tab:
		return "tab";
	case ang::streams::special_command::back_space:
		return "back_space";
	case ang::streams::special_command::insert:
		return "insert";
	case ang::streams::special_command::del:
		return "del";
	case ang::streams::special_command::home:
		return "home";
	case ang::streams::special_command::end:
		return "end";
	case ang::streams::special_command::log_none:
		return "log_none";
	case ang::streams::special_command::log_info:
		return "log_info";
	case ang::streams::special_command::log_warning:
		return "log_warning";
	case ang::streams::special_command::log_error:
		return "log_error";
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////


#define ANG_IMPLEMENT_INTF_WRRAPER_SPECIALIZATION(_TYPE) \
ang::intf_wrapper<_TYPE>::intf_wrapper() : _ptr(null) { } \
ang::intf_wrapper<_TYPE>::intf_wrapper(_TYPE* ptr) : _ptr(null) { set(ptr); } \
ang::intf_wrapper<_TYPE>::intf_wrapper(intf_wrapper && other) : _ptr(null) { \
	_TYPE * temp = other._ptr; \
	other._ptr = null; \
	_ptr = temp; \
} \
ang::intf_wrapper<_TYPE>::intf_wrapper(intf_wrapper const& other) : _ptr(null) { set(other._ptr); } \
void ang::intf_wrapper<_TYPE>::clean() { \
	iobject* obj = interface_cast<iobject>(_ptr); \
	_ptr = null; \
	if (obj) obj->release(); \
} \
ang::intf_wrapper<_TYPE>::~intf_wrapper() { clean(); } \
ang::bool_t ang::intf_wrapper<_TYPE>::is_empty()const { return _ptr == null; } \
_TYPE* ang::intf_wrapper<_TYPE>::get(void)const { return _ptr; } \
void ang::intf_wrapper<_TYPE>::set(_TYPE* ptr) { \
	if (ptr == _ptr) return; \
	iobject * _old = interface_cast<iobject>(_ptr); \
	iobject * _new = interface_cast<iobject>(ptr); \
	_ptr = ptr; \
	if (_new)_new->add_ref(); \
	if (_old)_old->release(); \
} \
ang::intf_wrapper<_TYPE>& ang::intf_wrapper<_TYPE>::operator = (_TYPE* ptr) { \
	set(ptr); \
	return*this; \
} \
ang::intf_wrapper<_TYPE>& ang::intf_wrapper<_TYPE>::operator = (ang::intf_wrapper<_TYPE> && other) { \
	if (this == &other) return *this; \
	clean(); \
	_ptr = other._ptr; \
	other._ptr = null; \
	return*this; \
} \
ang::intf_wrapper<_TYPE>& ang::intf_wrapper<_TYPE>::operator = (ang::intf_wrapper<_TYPE> const& other) { \
	set(other._ptr); \
	return*this; \
} \
_TYPE ** ang::intf_wrapper<_TYPE>::addres_of(void) { return &_ptr; } \
_TYPE * ang::intf_wrapper<_TYPE>::operator -> (void) { return get(); } \
_TYPE const* ang::intf_wrapper<_TYPE>::operator -> (void)const { return get(); } \
ang::intf_wrapper<_TYPE>::operator _TYPE * (void) { return get(); } \
ang::intf_wrapper<_TYPE>::operator _TYPE const* (void)const { return get(); } \
ang::intf_wrapper_ptr<_TYPE> ang::intf_wrapper<_TYPE>::operator &(void) { return this; }

ANG_IMPLEMENT_INTF_WRRAPER_SPECIALIZATION(itext_input_stream);
ANG_IMPLEMENT_INTF_WRRAPER_SPECIALIZATION(itext_output_stream);
ANG_IMPLEMENT_INTF_WRRAPER_SPECIALIZATION(ibinary_input_stream);
ANG_IMPLEMENT_INTF_WRRAPER_SPECIALIZATION(ibinary_output_stream);

//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, char_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, achar_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, byte_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, short_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, ushort_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, int_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, uint_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, long_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, ulong_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, float_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, double_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, string_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, astring_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, cstr_t value) { if (!stream.is_empty())stream->read(value); return stream; }
//itext_input_stream_t& streams::operator >> (itext_input_stream_t& stream, castr_t value) { if (!stream.is_empty())stream->read(value); return stream; }

//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, special_command_t value) { if (!stream.is_empty())stream->command(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, char_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, achar_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, byte_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, short_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, ushort_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, int_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, uint_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, long_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, ulong_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, float_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, double_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, string_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, astring_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, cstr_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//itext_output_stream_t& streams::operator << (itext_output_stream_t& stream, castr_t value) { if (!stream.is_empty())stream->write(value); return stream; }

//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, char_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, achar_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, byte_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, short_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, ushort_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, int_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, uint_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, long_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, ulong_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, float_t& value) { if (!stream.is_empty())stream->read(value); return stream; }
//ibinary_input_stream_t& streams::operator >> (ibinary_input_stream_t& stream, double_t& value) { if (!stream.is_empty())stream->read(value); return stream; }

//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, char_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, achar_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, byte_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, short_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, ushort_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, int_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, uint_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, long_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, ulong_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, float_t value) { if (!stream.is_empty())stream->write(value); return stream; }
//ibinary_output_stream_t& streams::operator << (ibinary_output_stream_t& stream, double_t value) { if (!stream.is_empty())stream->write(value); return stream; }
