#include "pch.h"
#include "angsys.h"

using namespace ang;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::ibuffer_view, interface);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::ibuffer, ibuffer_view);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::text::itext_buffer, ibuffer);

#define MY_TYPE ang::ibuffer_view
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::ibuffer
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::text::itext_buffer
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

/////////////////////////////////////////////////////////////////////////


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

buffer_t buffer::new_buffer(wsize sz) {
	return new(sz)buffer();
}

buffer::buffer()
{

}

buffer::~buffer()
{

}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::buffer);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::buffer, object, ibuffer);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::buffer, object, ibuffer, ibuffer_view);

text::encoding_t buffer::encoding()const {
	return text::encoding::binary;
}

bool buffer::is_readonly()const { return false; }

pointer buffer::buffer_ptr()
{
	return BUFFER_HANDLER(wsize(this) + sizeof(buffer))->get_buffer();
}

const_pointer buffer::buffer_ptr()const
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
	buffer_view_t buff = interface_cast<buffer_view>(view.get());
	if (buff == null || buff->parent().get() != this)
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

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::aligned_buffer);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::aligned_buffer, object, ibuffer);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::aligned_buffer, object, ibuffer, ibuffer_view);

bool aligned_buffer::auto_release()
{
	return object::auto_release(16U);
}

bool aligned_buffer::is_readonly()const { return false; }

text::encoding_t aligned_buffer::encoding()const {
	return text::encoding::binary;
}

pointer aligned_buffer::buffer_ptr()
{
	return BUFFER_HANDLER(wsize(this) + sizeof(aligned_buffer))->get_buffer();
}

const_pointer aligned_buffer::buffer_ptr()const
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
	buffer_view_t buff = interface_cast<buffer_view>(view.get());
	if (buff == null || buff->parent().get() != this)
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

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::buffer_view);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::buffer_view, object, ibuffer_view);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::buffer_view, object, ibuffer_view);

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

bool buffer_view::is_readonly()const { return false; }

pointer buffer_view::buffer_ptr()
{
	return _buffer.is_empty() ? null : pointer(wsize(_buffer->buffer_ptr()) + _start);
}

const_pointer buffer_view::buffer_ptr()const
{
	return _buffer.is_empty() ? null : pointer(wsize(_buffer->buffer_ptr()) + _start);
}

wsize buffer_view::buffer_size()const
{
	return _size;
}

//////////////////////////////////////////////////////////////////////////////////


ang_void_ptr_t dummy_buffer::operator new(wsize size)
{
	ang::memory::object_allocator<byte> allocator;
	if (size) return allocator.allocate(size);
	else return null;
}

void dummy_buffer::operator delete(ang_void_ptr_t ptr)
{
	ang::memory::object_allocator<byte> allocator;
	if (ptr) allocator.deallocate((byte*)ptr);
}

dummy_buffer::dummy_buffer(pointer ptr, wsize sz)
	: _ptr(ptr)
	, _size(sz)
{
}

dummy_buffer::~dummy_buffer()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::dummy_buffer);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::dummy_buffer, ibuffer, ibuffer_view);
ANG_IMPLEMENT_INTERFACE_QUERY_INTERFACE(ang::dummy_buffer, ibuffer, ibuffer_view);

text::encoding_t dummy_buffer::encoding()const { return text::encoding::binary; }

bool dummy_buffer::is_readonly()const { return false; }

pointer dummy_buffer::buffer_ptr() { return _ptr; }

const_pointer dummy_buffer::buffer_ptr()const { return _ptr; }

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
	buffer_view_t buff = interface_cast<buffer_view>(view.get());
	if (buff == null && buff->parent().get() != this)
		return false;
	view = null;
	return true;
}

bool dummy_buffer::can_realloc_buffer()const { return false; }

bool dummy_buffer::realloc_buffer(wsize) { return false; }
