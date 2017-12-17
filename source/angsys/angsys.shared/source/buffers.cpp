/*********************************************************************************************************************/
/*   File Name: buffers.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.hpp"
#include "ang/streams.hpp"

using namespace ang;

ANG_IMPLEMENT_INTERFACE(ang, ibuffer_view);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::ibuffer, ang::ibuffer_view);
//ANG_IMPLEMENT_BASIC_INTERFACE(ang::text::itext_buffer, ibuffer);

ANG_EXTERN ang_uint64_t ang_create_hash_index_cstr(ang_cstr_t key, ang_uint64_t TS);

wsize ibuffer::serialize(ibuffer_t buffer, streams::ibinary_output_stream_t stream)
{
	ang_uint64_t hash = ang_create_hash_index_cstr(buffer->object_name(), 0X7FFFFFFFFFFFFFFF);
	if (text::itext_buffer_t buffer = interface_cast<text::itext_buffer>(buffer.get()))
	{
		auto str = buffer->text_buffer();

		struct
		{
			qword hash;
			dword encoding;
			dword length;
		}info;
		info.hash = hash;
		info.encoding = buffer->encoding();
		info.length = str._size;

		auto sz = stream->write(&info, sizeof(info));
		sz += stream->write(str._value, str._size);
		return sz;
	}
	else
	{
		struct
		{
			qword hash;
			dword encoding;
			dword length;
		}info;
		info.hash = hash;
		info.encoding = buffer->encoding();
		info.length = buffer->buffer_size();

		auto sz = stream->write(&info, sizeof(info));
		sz += stream->write(buffer->buffer_ptr(), buffer->buffer_size());
		return sz;
	}
}

wsize ibuffer::serialize(ibuffer_t buffer, streams::itext_output_stream_t stream)
{
	if (text::itext_buffer_t buffer = interface_cast<text::itext_buffer>(buffer.get()))
	{
		auto sz = stream->write("ang::string<"_s);
		sz += stream->write(buffer->encoding().to_string());
		sz += stream->write(">{"_s);
		sz += stream->write(buffer->text_buffer());
		sz += stream->write("}"_s);
		return sz;
	}
	else
	{
		return stream->write(buffer->object_name());
	}
}

typedef struct _buffer_handler
{
	wsize size;
	inline pointer get_buffer() {
		return pointer(wsize(this) + sizeof(_buffer_handler));
	}
}*buffer_handler;

#define BUFFER_HANDLER(ptr)			buffer_handler(((pointer)(ptr)))

pointer buffer::operator new (wsize size, ulong64 buffsz)throw()
{
	wsize total = wsize(size + buffsz + sizeof(wsize));
	pointer ptr = object::operator new(total);
	if (ptr == null)
		return null;
	BUFFER_HANDLER(wsize(ptr) + size)->size = (wsize)buffsz;
	memset(BUFFER_HANDLER(wsize(ptr) + size)->get_buffer(), 0, (wsize)buffsz);
	return ptr;
}

void buffer::operator delete(pointer ptr, ulong64 buffsz)throw()
{
	object::operator delete(ptr);
}

void buffer::operator delete(pointer ptr)throw()
{
	object::operator delete(ptr);
}


buffer::buffer()
{

}

buffer::~buffer()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::buffer);
ANG_IMPLEMENT_OBJECTNAME(ang::buffer);

bool buffer::is_inherited_of(type_name_t type)
{
	return type == class_name()
		|| type == object::class_name()
		|| type == ibuffer::class_name();
}

bool buffer::is_kind_of(type_name_t type)const
{
	return type == class_name()
		|| type == object::class_name()
		|| type == ibuffer::class_name();
}

bool buffer::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<buffer>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<ibuffer>())
	{
		*out = static_cast<ibuffer*>(this);
		return true;
	}
	return false;
}

text::encoding_t buffer::encoding()const {
	return text::encoding::binary;
}

pointer buffer::buffer_ptr()const
{
	return BUFFER_HANDLER(wsize(this) + sizeof(buffer))->get_buffer();
}

wsize buffer::buffer_size()const
{
	return BUFFER_HANDLER(wsize(this) + sizeof(buffer))->size;
}

wsize buffer::mem_copy(wsize size, pointer ptr, text::encoding_t)
{
	wsize sz = min(size, buffer_size());
	memcpy(BUFFER_HANDLER(wsize(this) + sizeof(buffer))->get_buffer(), ptr, sz);
	return sz;
}

ibuffer_view_t buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > buffer_size())
		return null;
	return new buffer_view(this, start, size);
}

bool buffer::unmap_buffer(ibuffer_view_t& view, wsize)
{
	buffer_view_t buff = interface_cast<buffer_view_t>(view.get());
	if (buff == null && buff->parent().get() != this)
		return false;
	view = null;
	return true;
}


bool buffer::can_realloc_buffer()const { return false; };
bool buffer::realloc_buffer(wsize) { return false; };

//////////////////////////////////////////////////////////////////////////////////////////


pointer aligned_buffer::operator new (wsize size, ulong64 buffsz, word ALIGMENT)throw()
{
	wsize total = wsize(size + buffsz + sizeof(wsize));
	pointer ptr = object::operator new(total, ALIGMENT);
	if (ptr == null)
		return null;
	BUFFER_HANDLER(wsize(ptr) + size)->size = (wsize)buffsz;
	memset(BUFFER_HANDLER(wsize(ptr) + size)->get_buffer(), 0, (wsize)buffsz);
	return ptr;
}

void aligned_buffer::operator delete(pointer ptr, ulong64 buffsz, word ALIGMENT)throw()
{
	object::operator delete(ptr, ALIGMENT);
}

void aligned_buffer::operator delete(pointer ptr)throw()
{
	object::operator delete(ptr, 16);
}


aligned_buffer::aligned_buffer()
{

}

aligned_buffer::~aligned_buffer()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::aligned_buffer);
ANG_IMPLEMENT_OBJECTNAME(ang::aligned_buffer);

bool aligned_buffer::is_inherited_of(type_name_t type)
{
	return type == class_name()
		|| type == object::class_name()
		|| type == ibuffer::class_name();
}

bool aligned_buffer::is_kind_of(type_name_t type)const
{
	return type == class_name()
		|| type == object::class_name()
		|| type == ibuffer::class_name();
}

bool aligned_buffer::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<aligned_buffer>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_of<ibuffer>())
	{
		*out = static_cast<ibuffer*>(this);
		return true;
	}
	return false;
}

bool aligned_buffer::auto_release()
{
	return object::auto_release(16U);
}

text::encoding_t aligned_buffer::encoding()const {
	return text::encoding::binary;
}

pointer aligned_buffer::buffer_ptr()const
{
	return BUFFER_HANDLER(wsize(this) + sizeof(aligned_buffer))->get_buffer();
}

wsize aligned_buffer::buffer_size()const
{
	return BUFFER_HANDLER(wsize(this) + sizeof(aligned_buffer))->size;
}

wsize aligned_buffer::mem_copy(wsize size, pointer ptr, text::encoding_t)
{
	wsize sz = min(size, buffer_size());
	memcpy(BUFFER_HANDLER(wsize(this) + sizeof(aligned_buffer))->get_buffer(), ptr, sz);
	return sz;
}

ibuffer_view_t aligned_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > buffer_size())
		return null;
	return new buffer_view(this, start, size);
}

bool aligned_buffer::unmap_buffer(ibuffer_view_t& view, wsize)
{
	buffer_view_t buff = interface_cast<buffer_view_t>(view.get());
	if (buff == null && buff->parent().get() != this)
		return false;
	view = null;
	return true;
}


bool aligned_buffer::can_realloc_buffer()const { return false; };
bool aligned_buffer::realloc_buffer(wsize) { return false; };

//////////////////////////////////////////////////////////////////////////////////////////

buffer_view::buffer_view()
	: _start(0)
	, _size(0)
	, _buffer(null)
{

}

buffer_view::buffer_view(ibuffer_t buff, windex start, wsize size)
	: _start(start)
	, _size(size)
	, _buffer(buff)
{

}

buffer_view::~buffer_view()
{
	_start = 0;
	_size = 0;
	_buffer = null;
}

ANG_IMPLEMENT_CLASSNAME(ang::buffer_view);
ANG_IMPLEMENT_OBJECTNAME(ang::buffer_view);

bool buffer_view::is_inherited_of(type_name_t type)
{
	return type == class_name()
		|| type == object::class_name()
		|| type == ibuffer_view::class_name();
}

bool buffer_view::is_kind_of(type_name_t type)const
{
	return type == class_name()
		|| type == object::class_name()
		|| type == ibuffer_view::class_name();
}

bool buffer_view::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_of<buffer_view>())
	{
		*out = this;
		return true;
	}
	//else if (object::query_object(type, out))
	//{
	//	return true;
	//}
	else if (type == type_of<ibuffer_view>())
	{
		*out = static_cast<ibuffer_view*>(this);
		return true;
	}
	return false;
}

void buffer_view::set(ibuffer_t buff, windex start, wsize size)
{
	_start = start;
	_size = size;
	_buffer = buff;
}

ibuffer_t buffer_view::parent()const
{
	return _buffer;
}


pointer buffer_view::buffer_ptr()const
{
	return _buffer.is_empty() ? null: pointer(wsize(_buffer->buffer_ptr()) + _start);
}

wsize buffer_view::buffer_size()const
{
	return _size;
}

/////////////////////////////////////////////////////////////////////////////



ang::intf_wrapper<ibuffer>::intf_wrapper() : _ptr(null) {

}

ang::intf_wrapper<ibuffer>::intf_wrapper(ibuffer* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<ibuffer>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ibuffer * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<ibuffer>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::intf_wrapper<ibuffer>::intf_wrapper(std::nullptr_t const&)
	: _ptr(null)
{
}

ang::intf_wrapper<ibuffer>::~intf_wrapper() {
	clean();
}

void ang::intf_wrapper<ibuffer>::clean()
{
	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool ang::intf_wrapper<ibuffer>::is_empty()const
{
	return _ptr == null;
}

ibuffer* ang::intf_wrapper<ibuffer>::get(void)const
{
	return _ptr;
}

void ang::intf_wrapper<ibuffer>::set(ibuffer* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

ang::intf_wrapper<ibuffer>& ang::intf_wrapper<ibuffer>::operator = (ibuffer* ptr)
{
	set(ptr);
	return*this;
}

ang::intf_wrapper<ibuffer>& ang::intf_wrapper<ibuffer>::operator = (ang::intf_wrapper<ibuffer> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::intf_wrapper<ibuffer>& ang::intf_wrapper<ibuffer>::operator = (ang::intf_wrapper<ibuffer> const& other)
{
	set(other._ptr);
	return*this;
}

ibuffer ** ang::intf_wrapper<ibuffer>::addres_of(void)
{
	return &_ptr;
}

ang::intf_wrapper_ptr<ibuffer> ang::intf_wrapper<ibuffer>::operator & (void)
{
	return this;
}

ibuffer * ang::intf_wrapper<ibuffer>::operator -> (void)
{
	return get();
}

ibuffer const* ang::intf_wrapper<ibuffer>::operator -> (void)const
{
	return get();
}

ang::intf_wrapper<ibuffer>::operator ibuffer * (void)
{
	return get();
}

ang::intf_wrapper<ibuffer>::operator ibuffer const* (void)const
{
	return get();
}

//////////////////////////////////////////////////////////////////////////////////


ang_void_ptr_t dummy_buffer::operator new(wsize size)
{
	if(size) return ang::memory::object_allocator<byte>::alloc(size);
	else return null;
}

void dummy_buffer::operator delete(ang_void_ptr_t ptr)
{
	if(ptr) ang::memory::object_allocator<byte>::free((byte*)ptr);
}


dummy_buffer::dummy_buffer(pointer ptr, wsize sz)
	: _ptr(ptr)
	, _size(sz)
{
}

dummy_buffer::~dummy_buffer()
{
}

ANG_IMPLEMENT_BASIC_INTERFACE(ang::dummy_buffer, ibuffer);

text::encoding_t dummy_buffer::encoding()const { return text::encoding::binary; }

pointer dummy_buffer::buffer_ptr()const { return _ptr; }

wsize dummy_buffer::buffer_size()const { return _size; }

wsize dummy_buffer::mem_copy(wsize size, pointer ptr, text::encoding_t) {
	wsize sz = min(size, buffer_size());
	memcpy(BUFFER_HANDLER(wsize(this) + sizeof(aligned_buffer))->get_buffer(), ptr, sz);
	return sz;
}

ibuffer_view_t dummy_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > buffer_size())
		return null;
	return new buffer_view(this, start, size);
}

bool dummy_buffer::unmap_buffer(ibuffer_view_t& view, wsize)
{
	buffer_view_t buff = interface_cast<buffer_view_t>(view.get());
	if (buff == null && buff->parent().get() != this)
		return false;
	view = null;
	return true;
}

bool dummy_buffer::can_realloc_buffer()const { return false; }

bool dummy_buffer::realloc_buffer(wsize) { return false; }
