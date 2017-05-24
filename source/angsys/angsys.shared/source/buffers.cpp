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
#include "angsys.h"

using namespace ang;

ANG_IMPLEMENT_INTERFACE(ang, ibuffer);
ANG_IMPLEMENT_BASIC_INTERFACE(ang::text::itext_buffer, ibuffer);

typedef struct _buffer_handler
{
	uint size;
	inline pointer get_buffer() {
		return pointer(wsize(this) + sizeof(_buffer_handler));
	}
}*buffer_handler;

#define BUFFER_HANDLER(ptr)			buffer_handler(((pointer)(ptr)))

pointer buffer::operator new (wsize size, long64 buffsz)throw()
{
	wsize total = wsize(size + buffsz + sizeof(wsize));
	pointer ptr = object::operator new(total);
	if (ptr == null)
		return null;
	BUFFER_HANDLER(wsize(ptr) + size)->size = (wsize)buffsz;
	memset(BUFFER_HANDLER(wsize(ptr) + size)->get_buffer(), 0, (wsize)buffsz);
	return ptr;
}

void buffer::operator delete(pointer ptr, long64 buffsz)throw()
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

bool buffer::is_child_of(type_name_t type)
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
	if (type == type_name<buffer>())
	{
		*out = this;
		return true;
	}
	else if (object::query_object(type, out))
	{
		return true;
	}
	else if (type == type_name<ibuffer>())
	{
		*out = static_cast<ibuffer*>(this);
		return true;
	}
	return false;
}


pointer buffer::buffer_ptr()const
{
	return BUFFER_HANDLER(wsize(this) + sizeof(buffer))->get_buffer();
}

wsize buffer::buffer_size()const
{
	return BUFFER_HANDLER(wsize(this) + sizeof(buffer))->size;
}

wsize buffer::mem_copy(wsize size, pointer ptr)
{
	wsize sz = min(size, buffer_size());
	memcpy(BUFFER_HANDLER(wsize(this) + sizeof(buffer))->get_buffer(), ptr, sz);
	return sz;
}

pointer buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > buffer_size())
		return null;
	return (byte*)buffer_ptr() + start;
}

void buffer::unmap_buffer(pointer ptr, wsize used)
{
}


bool buffer::can_realloc_buffer()const { return false; };
bool buffer::realloc_buffer(wsize) { return false; };

//////////////////////////////////////////////////////////////////////////////////////////

void* static_buffer::operator new(size_t)
{
	return null;
}

void static_buffer::operator delete(pointer)
{

}

static_buffer::static_buffer()
	: _size(0)
	, _memory(null)
{

}

static_buffer::static_buffer(pointer mem, wsize size)
	: _size(size)
	, _memory(mem)
{

}

static_buffer::~static_buffer()
{
	_memory = null;
	_size = 0U;
}

ANG_IMPLEMENT_CLASSNAME(ang::static_buffer);
ANG_IMPLEMENT_OBJECTNAME(ang::static_buffer);



bool static_buffer::is_child_of(type_name_t type)
{
	return type == class_name()
		|| type == object::class_name()
		|| type == ibuffer::class_name();
}

bool static_buffer::is_kind_of(type_name_t type)const
{
	return type == class_name()
		|| type == object::class_name()
		|| type == ibuffer::class_name();
}

bool static_buffer::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<static_buffer>())
	{
		*out = this;
		return true;
	}
	//else if (object::query_object(type, out))
	//{
	//	return true;
	//}
	else if (type == type_name<ibuffer>())
	{
		*out = static_cast<ibuffer*>(this);
		return true;
	}
	return false;
}

void static_buffer::set(pointer ptr, wsize sz)
{
	_memory = ptr;
	_size = sz;
}

pointer static_buffer::buffer_ptr()const
{
	return _memory;
}

wsize static_buffer::buffer_size()const
{
	return _size;
}

wsize static_buffer::mem_copy(wsize size, pointer ptr)
{
	wsize sz = min(size, _size);
	memcpy(_memory, ptr, sz);
	return sz;
}

pointer static_buffer::map_buffer(windex start, wsize size)
{
	if ((start + size) > buffer_size())
		return null;
	return (byte*)buffer_ptr() + start;
}

void static_buffer::unmap_buffer(pointer ptr, wsize used)
{
}

bool static_buffer::can_realloc_buffer()const { return false; };
bool static_buffer::realloc_buffer(wsize) { return false; };

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

