#include "pch.h"
#include "angsys.h"

using namespace ang;

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::ibuffer_view, intf);
ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::ibuffer, ibuffer_view);

#define MY_TYPE ang::ibuffer_view
#include "ang/inline/intf_wrapper_specialization.inl"
#undef MY_TYPE

#define MY_TYPE ang::ibuffer
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
	: m_start(0)
	, m_size(0)
	, m_buffer(null)
{

}

buffer_view::buffer_view(ibuffer_t buff, windex start, wsize size)
	: m_start(start)
	, m_size(size)
	, m_buffer(buff)
{

}

buffer_view::~buffer_view()
{
	m_start = 0;
	m_size = 0;
	m_buffer = null;
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::buffer_view);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::buffer_view, object, ibuffer_view);
ANG_IMPLEMENT_OBJECT_QUERY_INTERFACE(ang::buffer_view, object, ibuffer_view);

void buffer_view::set(ibuffer_t buff, windex start, wsize size)
{
	m_start = start;
	m_size = size;
	m_buffer = buff;
}

ibuffer_t buffer_view::parent()const
{
	return m_buffer;
}

text::encoding_t buffer_view::encoding()const {
	return m_buffer->encoding();
}

bool buffer_view::is_readonly()const { return false; }

pointer buffer_view::buffer_ptr()
{
	return m_buffer.is_empty() ? null : pointer(wsize(m_buffer->buffer_ptr()) + m_start);
}

const_pointer buffer_view::buffer_ptr()const
{
	return m_buffer.is_empty() ? null : pointer(wsize(m_buffer->buffer_ptr()) + m_start);
}

wsize buffer_view::buffer_size()const
{
	return m_size;
}

//////////////////////////////////////////////////////////////////////////////////


ang_void_ptr_t buffer_wrapper::operator new(wsize size)
{
	ang::memory::object_allocator<byte> allocator;
	if (size) return allocator.allocate(size);
	else return null;
}

void buffer_wrapper::operator delete(ang_void_ptr_t ptr)
{
	ang::memory::object_allocator<byte> allocator;
	if (ptr) allocator.deallocate((byte*)ptr);
}

buffer_wrapper::buffer_wrapper(pointer ptr, wsize sz)
	: m_ptr(ptr)
	, m_size(sz)
{
}

buffer_wrapper::~buffer_wrapper()
{
}

ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(ang::buffer_wrapper);
ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::buffer_wrapper, ibuffer, ibuffer_view);
ANG_IMPLEMENT_INTERFACE_QUERY_INTERFACE(ang::buffer_wrapper, ibuffer, ibuffer_view);

text::encoding_t buffer_wrapper::encoding()const { return text::encoding::binary; }

bool buffer_wrapper::is_readonly()const { return false; }

pointer buffer_wrapper::buffer_ptr() { return m_ptr; }

const_pointer buffer_wrapper::buffer_ptr()const { return m_ptr; }

wsize buffer_wrapper::buffer_size()const { return m_size; }

wsize buffer_wrapper::mem_copy(wsize size, pointer ptr, text::encoding_t) {
	wsize sz = min(size, buffer_size());
	memcpy(BUFFER_HANDLER(wsize(this) + sizeof(aligned_buffer))->get_buffer(), ptr, sz);
	return sz;
}

ibuffer_view_t buffer_wrapper::map_buffer(windex start, wsize size)
{
	if ((start + size) > buffer_size())
		return null;
	return new buffer_view(this, start, size);
}

bool buffer_wrapper::unmap_buffer(ibuffer_view_t& view, wsize)
{
	buffer_view_t buff = interface_cast<buffer_view>(view.get());
	if (buff == null && buff->parent().get() != this)
		return false;
	view = null;
	return true;
}

bool buffer_wrapper::can_realloc_buffer()const { return false; }

bool buffer_wrapper::realloc_buffer(wsize) { return false; }
