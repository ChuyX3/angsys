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
#include "ang/system.h"
#include "ang_memory.h"
#include <assert.h>

using namespace ang;

safe_enum_rrti(ang, comparision_result_t, value<comparision_result_proxy>);

bool interface::default_query_interface(rtti_t const& src_id, unknown_t src, rtti_t const& out_id, unknown_ptr_t out)
{
#ifdef _DEBUG
	assert(src_id.is_type_of(interface::class_info()));
#endif
	return src ? reinterpret_cast<interface*>(src)->query_interface(out_id, out) : null;
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
	clean();
}

void safe_pointer::clean()
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

	clean();

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
	clean();

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
	clean();
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////


pointer object::operator new(wsize sz)
{
	static memory::iraw_allocator* allocator = memory::get_raw_allocator(ang_object_memory);
	smart_ptr_info_ptr_t ptr = new(allocator->memory_alloc(sz + align_up<16, sizeof(smart_ptr_info_t)>())) smart_ptr_info_t();
	ptr->_obj_ref_counter = 0;
	ptr->_mem_ref_counter = 0;
	ptr->allocator = allocator;
	ptr->_object = (interface*)(wsize(ptr) + align_up<16, sizeof(smart_ptr_info_t)>());
	return ptr->_object;
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
	static memory::iraw_allocator* allocator = memory::get_raw_allocator(ang_aligned_memory);
	smart_ptr_info_ptr_t ptr = new(allocator->memory_alloc(sz + align_up<16, sizeof(smart_ptr_info_t)>())) smart_ptr_info_t();
	ptr->_obj_ref_counter = 0;
	ptr->_mem_ref_counter = 0;
	ptr->allocator = allocator;
	ptr->_object = (interface*)(wsize(ptr) + align_up<16, sizeof(smart_ptr_info_t)>());
	return ptr->_object;
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
	static memory::iraw_allocator* allocator = memory::get_raw_allocator(ang_object_memory);
	smart_ptr_info_ptr_t ptr = new(allocator->memory_alloc(sz + align_up<16, sizeof(smart_ptr_info_t)>(), file, line)) smart_ptr_info_t();
	ptr->_obj_ref_counter = 0;
	ptr->_mem_ref_counter = 0;
	ptr->allocator = allocator;
	ptr->_object = (interface*)(wsize(ptr) + align_up<16, sizeof(smart_ptr_info_t)>());
	return ptr->_object;
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

ang::object_wrapper<object>::object_wrapper() : _ptr(null) {

}

ang::object_wrapper<object>::object_wrapper(object* ptr) : _ptr(null) {
	set(ptr);
}

ang::object_wrapper<object>::object_wrapper(object_wrapper && other) : _ptr(null) {
	object * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::object_wrapper<object>::object_wrapper(object_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}

ang::object_wrapper<object>::object_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

ang::object_wrapper<object>::~object_wrapper()
{
	clean();
}

void ang::object_wrapper<object>::clean()
{
	if (_ptr)_ptr->release();
	_ptr = null;
}

void ang::object_wrapper<object>::clean_unsafe()
{
	_ptr = null;
}

bool ang::object_wrapper<object>::is_empty()const
{
	return _ptr == null;
}

object* ang::object_wrapper<object>::get(void)const
{
	return _ptr;
}

void ang::object_wrapper<object>::set(object* ptr)
{
	object * temp = _ptr;
	if (ptr == _ptr) return;
	_ptr = ptr;
	if (_ptr)_ptr->add_ref();
	if (temp)temp->release();
}

ang::object_wrapper<object>& ang::object_wrapper<object>::operator = (object* ptr)
{
	set(ptr);
	return*this;
}

ang::object_wrapper<object>& ang::object_wrapper<object>::operator = (ang::object_wrapper<object> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}

ang::object_wrapper<object>& ang::object_wrapper<object>::operator = (ang::object_wrapper<object> const& other)
{
	set(other._ptr);
	return*this;
}

object ** ang::object_wrapper<object>::addres_of(void)
{
	return &_ptr;
}

ang::object_wrapper_ptr<object> ang::object_wrapper<object>::operator& (void)
{
	return this;
}

object * ang::object_wrapper<object>::operator -> (void)
{
	return get();
}

object const* ang::object_wrapper<object>::operator -> (void)const
{
	return get();
}

ang::object_wrapper<object>::operator object * (void)
{
	return get();
}

ang::object_wrapper<object>::operator object const* (void)const
{
	return get();
}



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

ang::intf_wrapper<ang::interface>::intf_wrapper() : _ptr(null) {
}

ang::intf_wrapper<ang::interface>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

ang::intf_wrapper<ang::interface>::intf_wrapper(ang::interface* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<ang::interface>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ang::interface * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<ang::interface>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}


ang::intf_wrapper<ang::interface>::~intf_wrapper() {
	clean();
}


void ang::intf_wrapper<ang::interface>::clean()
{
	iobject * _obj = dyn_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}


bool ang::intf_wrapper<ang::interface>::is_empty()const
{
	return _ptr == null;
}


ang::interface* ang::intf_wrapper<ang::interface>::get(void)const
{
	return _ptr;
}


void ang::intf_wrapper<ang::interface>::set(ang::interface* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = dyn_cast<iobject>(_ptr);
	iobject * _new = dyn_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}


ang::intf_wrapper<ang::interface>& ang::intf_wrapper<ang::interface>::operator = (ang::interface* ptr)
{
	set(ptr);
	return*this;
}


ang::intf_wrapper<ang::interface>& ang::intf_wrapper<ang::interface>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}


ang::intf_wrapper<ang::interface>& ang::intf_wrapper<ang::interface>::operator = (ang::intf_wrapper<ang::interface> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


ang::intf_wrapper<ang::interface>& ang::intf_wrapper<ang::interface>::operator = (ang::intf_wrapper<ang::interface> const& other)
{
	set(other._ptr);
	return*this;
}


ang::interface ** ang::intf_wrapper<ang::interface>::addres_of(void)
{
	return &_ptr;
}


ang::intf_wrapper_ptr<ang::interface> ang::intf_wrapper<ang::interface>::operator & (void)
{
	return this;
}


ang::interface * ang::intf_wrapper<ang::interface>::operator -> (void)
{
	return get();
}


ang::interface const* ang::intf_wrapper<ang::interface>::operator -> (void)const
{
	return get();
}


ang::intf_wrapper<ang::interface>::operator ang::interface * (void)
{
	return get();
}


ang::intf_wrapper<ang::interface>::operator ang::interface const* (void)const
{
	return get();
}
