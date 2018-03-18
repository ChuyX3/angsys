/*********************************************************************************************************************/
/*   File Name: angsys.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"
#include "ang_memory.h"
#include <assert.h>

using namespace ang;

safe_enum_rrti(ang, comparision_result_t, value<comparision_result_proxy>);

bool interface::default_query_interface(rtti_t const& src_id, unknown_t src, rtti_t const& out_id, unknown_ptr_t out)
{
#ifdef _DEBUG
	assert(src_id.is_type_of(interface::class_info()));
#endif
	return src ? reinterpret_cast<interface*>(src)->query_interface(out_id, out) : false;
}

rtti_t const& interface::class_info() {
	static const char name[] = "ang::interface";
	static rtti_t const& info = rtti::regist(name, genre::class_type, sizeof(interface), alignof(wsize), null, &default_query_interface);
	return info;
}

ANG_IMPLEMENT_INTERFACE_CLASS_INFO(ang::iobject, interface);

typedef struct smart_ptr_info
{
	dword _mem_ref_counter; //4bytes
	dword _obj_ref_counter; //4bytes
	memory::iraw_allocator* allocator;
	interface* _object; //4-8bytes
}smart_ptr_info_t, *smart_ptr_info_ptr_t;

#define GET_SMART_PTR_INFO(_ptr) smart_ptr_info_ptr_t(wsize(_ptr) - align_up<16, sizeof(smart_ptr_info_t)>())


//////////////////////////////////////////////////////////////////////////////////////

safe_pointer::safe_pointer()
	: _info(nullptr)
	, _offset(0)
{

}

safe_pointer::safe_pointer(safe_pointer&& other)
	: _info(other._info)
	, _offset(0)
{
	other._info = nullptr;
}

safe_pointer::safe_pointer(safe_pointer const& other)
	: _info(nullptr)
	, _offset(0)
{
	set(const_cast<safe_pointer&>(other).lock<object>());
}


safe_pointer::safe_pointer(std::nullptr_t const&)
	: _info(nullptr)
	, _offset(0)
{
}

safe_pointer::safe_pointer(interface* obj)
	: _info(nullptr)
	, _offset(0)
{
	set(obj);
}

safe_pointer::~safe_pointer()
{
	clear();
}

void safe_pointer::clear()
{
	if (_info)
	{
		dword r = --smart_ptr_info_ptr_t(_info)->_mem_ref_counter;
		if (r <= 0U && smart_ptr_info_ptr_t(_info)->_object == nullptr)
		{
			smart_ptr_info_ptr_t(_info)->allocator->memory_release(_info);
		}
	}
	_info = nullptr;
	_offset = 0;
}

void safe_pointer::set(interface* _obj)
{
	iobject* obj = dyn_cast<iobject>(_obj);

	if (_info && smart_ptr_info_ptr_t(_info)->_object == obj)
		return;

	clear();

	if (obj != nullptr)
	{
		_offset = wsize(_obj) - wsize(obj);
		auto info = GET_SMART_PTR_INFO(obj);
		_info = info;
#ifdef WINDOWS_PLATFORM
		InterlockedIncrement(&smart_ptr_info_ptr_t(_info)->_mem_ref_counter);
#else
		++smart_ptr_info_ptr_t(_info)->_mem_ref_counter;
#endif
	}
}

bool safe_pointer::is_valid()const
{
	auto info = smart_ptr_info_ptr_t(_info);
	return ((info) ? (info->_object != nullptr) : false);
}

template<>
intfptr safe_pointer::lock<interface>()
{
	return is_valid() ? (interface*)(wsize(smart_ptr_info_ptr_t(_info)->_object) + _offset) : nullptr;
}

safe_pointer& safe_pointer::operator = (interface* obj)
{
	set(obj);
	return *this;
}

safe_pointer& safe_pointer::operator = (intfptr obj)
{
	set(obj.get());
	return *this;
}

safe_pointer& safe_pointer::operator = (safe_pointer&& other)
{
	if (this != &other)
		return*this;
	clear();

	_info = other._info;
	_offset = other._offset;
	other._info = nullptr;
	other._offset = 0;
	return *this;
}

safe_pointer& safe_pointer::operator = (safe_pointer const& ptr)
{
	intfptr obj = const_cast<safe_pointer&>(ptr).lock<interface>();
	set(obj.get());
	return *this;
}

safe_pointer& safe_pointer::operator = (std::nullptr_t const&)
{
	clear();
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////

object* ang_alloc_object_memory(ang_size_t sz, ang_memory_hint_t hint)
{
	memory::iraw_allocator* allocator = memory::get_raw_allocator(hint);
	smart_ptr_info_ptr_t ptr = new(allocator->memory_alloc(sz + align_up<16, sizeof(smart_ptr_info_t)>())) smart_ptr_info_t();
	ptr->_obj_ref_counter = 0;
	ptr->_mem_ref_counter = 0;
	ptr->allocator = allocator;
	ptr->_object = (interface*)(wsize(ptr) + align_up<16, sizeof(smart_ptr_info_t)>());
	return (object*)ptr->_object;
}

#ifdef _DEBUG
object* ang_alloc_object_memory(ang_size_t sz, const char* file, int line, ang_memory_hint_t hint)
{
	memory::iraw_allocator* allocator = memory::get_raw_allocator(hint);
	smart_ptr_info_ptr_t ptr = new(allocator->memory_alloc(sz + align_up<16, sizeof(smart_ptr_info_t)>(), file, line)) smart_ptr_info_t();
	ptr->_obj_ref_counter = 0;
	ptr->_mem_ref_counter = 0;
	ptr->allocator = allocator;
	ptr->_object = (interface*)(wsize(ptr) + align_up<16, sizeof(smart_ptr_info_t)>());
	return (object*)ptr->_object;
}
#endif

pointer object::operator new(wsize sz)
{
	return ang_alloc_object_memory(sz, ang_object_memory);
}

void object::operator delete(pointer ptr)
{
	static memory::iraw_allocator* default_allocator = memory::get_raw_allocator(ang_object_memory);
	smart_ptr_info_t* info = (smart_ptr_info_t*)(wsize(ptr) - align_up<16, sizeof(smart_ptr_info_t)>());
	memory::iraw_allocator* allocator = info->allocator;
	info->_object = nullptr;
	if (info->_mem_ref_counter == 0)
	{
		if (allocator)
			allocator->memory_release(info);
		else
			default_allocator->memory_release(info);
	}
}

pointer object::operator new(wsize sz, const word)
{
	return ang_alloc_object_memory(sz, ang_aligned_memory);
}

void object::operator delete(pointer ptr, const word)
{
	static memory::iraw_allocator* default_allocator = memory::get_raw_allocator(ang_aligned_memory);
	smart_ptr_info_t* info = (smart_ptr_info_t*)(wsize(ptr) - align_up<16, sizeof(smart_ptr_info_t)>());
	memory::iraw_allocator* allocator = info->allocator;
	info->_object = nullptr;
	if (info->_mem_ref_counter == 0)
	{
		if (allocator)allocator->memory_release(info);
		else default_allocator->memory_release(info);
	}
}

#ifdef _DEBUG
pointer object::operator new(wsize sz, const char* file, int line)
{
	return ang_alloc_object_memory(sz, file, line, ang_aligned_memory);
}

pointer object::operator new(wsize sz, const word, const char* file, int line)
{
	static memory::iraw_allocator* allocator = memory::get_raw_allocator(ang_aligned_memory);
	smart_ptr_info_ptr_t ptr = new(allocator->memory_alloc(sz + align_up<16, sizeof(smart_ptr_info_t)>(), file, line)) smart_ptr_info_t();
	ptr->_obj_ref_counter = 0;
	ptr->_mem_ref_counter = 0;
	ptr->allocator = allocator;
	ptr->_object = (interface*)(wsize(ptr) + align_up<16, sizeof(smart_ptr_info_t)>());
	return ptr->_object;
}
#endif

pointer object::operator new[](wsize)noexcept { return null; }
void object::operator delete[](pointer)noexcept {}

pointer object::operator new(wsize, pointer ptr)noexcept { return ptr; }
void object::operator delete(pointer, pointer)noexcept { }

object::object(bool inc_ref)
	: _ref_count(GET_SMART_PTR_INFO(this)->_obj_ref_counter)
{
	if (inc_ref)
		add_ref();
}

object::~object()
{

}

ANG_IMPLEMENT_OBJECT_CLASS_INFO(ang::object, ang::iobject);
ANG_IMPLEMENT_OBJECT_RUNTIME_INFO(object);
ANG_IMPLEMENT_INTERFACE_QUERY_INTERFACE(object, iobject, interface);

dword object::add_ref()
{
#ifdef WINDOWS_PLATFORM
	return InterlockedIncrement(&_ref_count);
#else
	return ++_ref_count;
#endif
}

dword object::release()
{
#ifdef WINDOWS_PLATFORM
	auto count = InterlockedDecrement(&_ref_count);
#else
	auto count = --_ref_count;
#endif
	if (count == 0)
		auto_release();
	return count;
}

bool object::auto_release()
{
	smart_ptr_info_t* info = (smart_ptr_info_t*)(wsize(this) - align_up<16, sizeof(smart_ptr_info_t)>());

#ifdef WINDOWS_PLATFORM
	InterlockedIncrement(&info->_mem_ref_counter);
#else
	++info->_mem_ref_counter;
#endif

	this->~object(); //call destructor;
	info->_object = nullptr;
#ifdef WINDOWS_PLATFORM
	auto count = InterlockedDecrement(&info->_mem_ref_counter);
#else
	auto count = --info->_mem_ref_counter;
#endif
	if (count <= 0)
	{
		info->allocator->memory_release(info);
	}
	return true;
}


bool object::auto_release(word ALIGMENT)
{
	smart_ptr_info_t* info = (smart_ptr_info_t*)(wsize(this) - align_up<16, sizeof(smart_ptr_info_t)>());

#ifdef WINDOWS_PLATFORM
	InterlockedIncrement(&info->_mem_ref_counter);
#else
	++info->_mem_ref_counter;
#endif

	this->~object(); //call destructor;
	info->_object = nullptr;
#ifdef WINDOWS_PLATFORM
	auto count = InterlockedDecrement(&info->_mem_ref_counter);
#else
	auto count = --info->_mem_ref_counter;
#endif
	if (count <= 0)
	{
		info->allocator->memory_release(info);
	}
	return true;
}

comparision_result_t object::compare(object const* obj)const
{
	return (obj != null && runtime_info().type_id() == obj->runtime_info().type_id())? comparision_result::same : comparision_result::diferent;
}

/////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////

object_wrapper<object>::object_wrapper() : _ptr(null) {

}

object_wrapper<object>::object_wrapper(object* ptr) : _ptr(null) {
	set(ptr);
}

object_wrapper<object>::object_wrapper(object_wrapper && other) : _ptr(null) {
	object * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

object_wrapper<object>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

object_wrapper<object>::object_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

object_wrapper<object>::~object_wrapper()
{
	clear();
}

void object_wrapper<object>::clear()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void object_wrapper<object>::clear_unsafe()
{
	_ptr = null;
}

bool object_wrapper<object>::is_empty()const
{
	return _ptr == null;
}

object* object_wrapper<object>::get(void)const
{
	return _ptr;
}

void object_wrapper<object>::set(object* ptr)
{
	object * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

object_wrapper<object>& object_wrapper<object>::operator = (object* ptr)
{
	set(ptr);
	return*this;
}

object_wrapper<object>& object_wrapper<object>::operator = (object_wrapper<object> && other)
{
	if (this == &other)
		return *this;
	clear();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

object_wrapper<object>& object_wrapper<object>::operator = (object_wrapper<object> const& other)
{
	set(other._ptr);
	return*this;
}

object ** object_wrapper<object>::addres_of(void)
{
	return &_ptr;
}

object_wrapper_ptr<object> object_wrapper<object>::operator& (void)
{
	return this;
}

object* object_wrapper<object>::operator -> (void)const
{
	return get();
}

object_wrapper<object>::operator object* (void)const
{
	return get();
}

///////////////////////////////////////////////////////////////////////////

object_wrapper<const object>::object_wrapper() : _ptr(null) {

}

object_wrapper<const object>::object_wrapper(object const * ptr) : _ptr(null) {
	set(ptr);
}

object_wrapper<const object>::object_wrapper(object_wrapper && other) : _ptr(null) {
	object const * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

object_wrapper<const object>::object_wrapper(object_wrapper<object> && other) : _ptr(null) {
	move(other);
}

object_wrapper<const object>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other.get());
}

object_wrapper<const object>::object_wrapper(object_wrapper<object> const& other) : _ptr(null) {
	set(other.get());
}

object_wrapper<const object>::object_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

object_wrapper<const object>::~object_wrapper()
{
	clear();
}

void object_wrapper<const object>::clear()
{
	if (_ptr)const_cast<object*>(_ptr)->release();
	_ptr = null;
}

void object_wrapper<const object>::clear_unsafe()
{
	_ptr = null;
}

bool object_wrapper<const object>::is_empty()const
{
	return _ptr == null;
}

object const * object_wrapper<const object>::get(void)const
{
	return _ptr;
}

void object_wrapper<const object>::set(object const * ptr)
{
	object const * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)const_cast<object*>(_ptr)->add_ref();
	if (temp)const_cast<object*>(temp)->release();
}

object_wrapper<const object>& object_wrapper<const object>::operator = (object const * ptr)
{
	set(ptr);
	return*this;
}

object_wrapper<const object>& object_wrapper<const object>::operator = (object_wrapper<const object> && other)
{
	move(other);
	return*this;
}

object_wrapper<const object>& object_wrapper<const object>::operator = (object_wrapper<object> && other)
{
	move(other);
	return*this;
}

object_wrapper<const object>& object_wrapper<const object>::operator = (object_wrapper<const object> const& other)
{
	set(other._ptr);
	return*this;
}

object_wrapper<const object>& object_wrapper<const object>::operator = (object_wrapper<object> const& other)
{
	set(other.get());
	return*this;
}

object const ** object_wrapper<const object>::addres_of(void)
{
	return &_ptr;
}

object_wrapper_ptr<const object> object_wrapper<const object>::operator& (void)
{
	return this;
}

object const * object_wrapper<const object>::operator -> (void)const
{
	return get();
}

object_wrapper<const object>::operator const object* (void)const
{
	return get();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

intf_wrapper<interface>::intf_wrapper() : _ptr(null) {
}

intf_wrapper<interface>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

intf_wrapper<interface>::intf_wrapper(interface* ptr) : _ptr(null) {
	set(ptr);
}

intf_wrapper<interface>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	interface * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

intf_wrapper<interface>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

intf_wrapper<interface>::~intf_wrapper() {
	clear();
}

void intf_wrapper<interface>::clear()
{
	iobject * _obj = dyn_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}

bool intf_wrapper<interface>::is_empty()const
{
	return _ptr == null;
}

interface* intf_wrapper<interface>::get(void)const
{
	return _ptr;
}

void intf_wrapper<interface>::set(interface* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = dyn_cast<iobject>(_ptr);
	iobject * _new = dyn_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

intf_wrapper<interface>& intf_wrapper<interface>::operator = (interface* ptr)
{
	set(ptr);
	return*this;
}

intf_wrapper<interface>& intf_wrapper<interface>::operator = (ang::nullptr_t const&)
{
	clear();
	return*this;
}

intf_wrapper<interface>& intf_wrapper<interface>::operator = (intf_wrapper<interface> && other)
{
	if (this == &other)
		return *this;
	clear();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

intf_wrapper<interface>& intf_wrapper<interface>::operator = (intf_wrapper<interface> const& other)
{
	set(other._ptr);
	return*this;
}

interface ** intf_wrapper<interface>::addres_of(void)
{
	return &_ptr;
}

intf_wrapper_ptr<interface> intf_wrapper<interface>::operator & (void)
{
	return this;
}

interface* intf_wrapper<interface>::operator -> (void)const
{
	return get();
}

intf_wrapper<interface>::operator interface* (void)const
{
	return get();
}

//////////////////////////////////////////////////////////////////////////////////////////

intf_wrapper<const interface>::intf_wrapper() : _ptr(null) {
}

intf_wrapper<const interface>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

intf_wrapper<const interface>::intf_wrapper(interface const * ptr) : _ptr(null) {
	set(ptr);
}

intf_wrapper<const interface>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	interface const * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

intf_wrapper<const interface>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

intf_wrapper<const interface>::~intf_wrapper() {
	clear();
}

void intf_wrapper<const interface>::clear()
{
	iobject const * _obj = dyn_cast<iobject>(_ptr);
	if (_obj)const_cast<iobject*>(_obj)->release();
	_ptr = null;
}

bool intf_wrapper<const interface>::is_empty()const
{
	return _ptr == null;
}

interface const * intf_wrapper<const interface>::get(void)const
{
	return _ptr;
}

void intf_wrapper<const interface>::set(interface const * ptr)
{
	if (ptr == _ptr) return;
	iobject const * _old = dyn_cast<iobject>(_ptr);
	iobject const * _new = dyn_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)const_cast<iobject*>(_new)->add_ref();
	if (_old)const_cast<iobject*>(_old)->release();
}

intf_wrapper<const interface>& intf_wrapper<const interface>::operator = (interface const * ptr)
{
	set(ptr);
	return*this;
}

intf_wrapper<const interface>& intf_wrapper<const interface>::operator = (ang::nullptr_t const&)
{
	clear();
	return*this;
}

intf_wrapper<const interface>& intf_wrapper<const interface>::operator = (intf_wrapper<const interface> && other)
{
	if (this == &other)
		return *this;
	clear();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

intf_wrapper<const interface>& intf_wrapper<const interface>::operator = (intf_wrapper<const interface> const& other)
{
	set(other._ptr);
	return*this;
}

interface const ** intf_wrapper<const interface>::addres_of(void)
{
	return &_ptr;
}

intf_wrapper_ptr<const interface> intf_wrapper<const interface>::operator & (void)
{
	return this;
}

interface const * intf_wrapper<const interface>::operator -> (void)const
{
	return get();
}

intf_wrapper<const interface>::operator interface const * (void)const
{
	return get();
}

