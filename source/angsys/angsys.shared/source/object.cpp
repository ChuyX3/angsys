/*********************************************************************************************************************/
/*   File Name: object.cpp                                                                                           */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#include "pch.h"
#include "angsys.h"

//#include <ang/collections/list.h>

using namespace ang;

ANG_IMPLEMENT_ENUM(ang, comparision_result, int, comparision_result::diferent);

ANG_IMPLEMENT_INTERFACE(ang, iobject);

typedef struct smart_ptr_info //16bytes for correct 16 byte alignment
{
	dword _shared_counter; //4bytes
	dword _ref_counter; //4bytes
	union 
	{
		object* _object; //4-8bytes
		ulong64 _unused;//8bytes
	};

}smart_ptr_info_t, *smart_ptr_info_ptr_t;

#define GET_SMART_PTR_INFO(_ptr) smart_ptr_info_ptr_t(wsize(_ptr) - sizeof(smart_ptr_info_t))

safe_pointer::safe_pointer()
	: _info(null)
{
}

safe_pointer::safe_pointer(safe_pointer&& other)
	: _info(other._info)
{
	other._info = null;
}

safe_pointer::safe_pointer(safe_pointer const& other)
	: _info(null)
{
	set(const_cast<safe_pointer&>(other).lock<object>());
}


safe_pointer::safe_pointer(ang::nullptr_t const&)
	: _info(null)
{
}

safe_pointer::safe_pointer(object* obj)
	: _info(null)
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
#ifdef WINDOWS_PLATFORM
		dword r = InterlockedDecrement(&smart_ptr_info_ptr_t(_info)->_shared_counter);
#elif defined ANDROID_PLATFORM
		dword r = --smart_ptr_info_ptr_t(_info)->_shared_counter;
#endif
		if (r <= 0U && smart_ptr_info_ptr_t(_info)->_object == null)
		{
			ang::memory::allocator_manager::get_allocator(memory::allocator_manager::object_allocator)->memory_release(_info);
		}
	}
	_info = null;
}

void safe_pointer::set(object* obj)
{
	if (_info && smart_ptr_info_ptr_t(_info)->_object == obj)
		return;
	clean();

	if (obj != null)
	{
		auto info = GET_SMART_PTR_INFO(obj);
		_info = info;
#ifdef WINDOWS_PLATFORM
		InterlockedIncrement(&info->_shared_counter);
#elif defined ANDROID_PLATFORM
		++smart_ptr_info_ptr_t(_info)->_shared_counter;
#endif
	}
}

bool safe_pointer::is_valid()const
{
	auto info = smart_ptr_info_ptr_t(_info);
	return ((info) ? (info->_object != null) : false);
}

template<>
object_t safe_pointer::lock<object>()
{
	return is_valid() ? (object*)smart_ptr_info_ptr_t(_info)->_object : null;
}

safe_pointer& safe_pointer::operator = (object* obj)
{
	set(obj);
	return *this;
}

safe_pointer& safe_pointer::operator = (object_t obj)
{
	set(obj.get());
	return *this;
}

safe_pointer& safe_pointer::operator = (safe_pointer&& other)
{
	_info = other._info;
	other._info = null;
	return *this;
}

safe_pointer& safe_pointer::operator = (safe_pointer const& ptr)
{
	objptr obj = const_cast<safe_pointer&>(ptr).lock<object>();
	set(obj.get());
	return *this;
}

safe_pointer& safe_pointer::operator = (ang::nullptr_t const&)
{
	clean();
	return *this;
}

ang_void_ptr_t object::operator new(wsize size)
{
	smart_ptr_info_ptr_t ptr = (smart_ptr_info_ptr_t) ang::memory::allocator_manager::get_allocator(memory::allocator_manager::object_allocator)->memory_alloc(size + sizeof(smart_ptr_info_t));
	ptr->_shared_counter = 0;
	ptr->_ref_counter = 0;
	ptr->_object = (object*)ang_void_ptr_t(wsize(ptr) + sizeof(smart_ptr_info_t));
	return ptr->_object;
}

ang_void_ptr_t object::operator new(wsize size, ushort)
{
	smart_ptr_info_ptr_t ptr = (smart_ptr_info_ptr_t)ang::memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator)->memory_alloc(size + sizeof(smart_ptr_info_t));
	ptr->_shared_counter = 0;
	ptr->_ref_counter = 0;
	ptr->_object = (object*)ang_void_ptr_t(wsize(ptr) + sizeof(smart_ptr_info_t));
	return ptr->_object;
}

ang_void_ptr_t object::operator new[](wsize size)
{
	//throw(exception_t(exceptions::except_code::invalid_access));
	return null;
}

void object::operator delete(ang_void_ptr_t ptr)
{
	smart_ptr_info_t* info = (smart_ptr_info_t*)((wsize)ptr - sizeof(smart_ptr_info_t));
	info->_object = null;
	if(info->_shared_counter == 0)
		ang::memory::allocator_manager::get_allocator(memory::allocator_manager::object_allocator)->memory_release(info);
}

void object::operator delete(ang_void_ptr_t ptr, ushort)
{
	smart_ptr_info_t* info = (smart_ptr_info_t*)((wsize)ptr - sizeof(smart_ptr_info_t));
	info->_object = null;
	if (info->_shared_counter == 0)
		ang::memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator)->memory_release(info);
}

void object::operator delete[](ang_void_ptr_t ptr)
{
	//throw(exception_t(exceptions::except_code::invalid_access));
	//ang::memory::allocator_manager::instance()->free(ptr, 1);
}

#ifdef _DEBUG
ang_void_ptr_t object::operator new(wsize size, char const* file, int line)
{
	smart_ptr_info_ptr_t ptr = (smart_ptr_info_ptr_t)ang::memory::allocator_manager::get_allocator(memory::allocator_manager::object_allocator)->memory_alloc(size + sizeof(smart_ptr_info_t), file, line);
	ptr->_shared_counter = 0;
	ptr->_ref_counter = 0;
	ptr->_object = (object*)ang_void_ptr_t(wsize(ptr) + sizeof(smart_ptr_info_t));
	return ptr->_object;
}

void object::operator delete(ang_void_ptr_t ptr, char const* file, int line)
{
	smart_ptr_info_ptr_t info = (smart_ptr_info_ptr_t)((wsize)ptr - sizeof(smart_ptr_info_t));
	info->_object = null;
	if (info->_shared_counter == 0)
		ang::memory::allocator_manager::get_allocator(memory::allocator_manager::object_allocator)->memory_release(info);
}
#endif

object::object(bool inc_ref)
	: ref_count(GET_SMART_PTR_INFO(this)->_ref_counter)
{
	ref_count = inc_ref ? 1 : 0;
}

object::~object()
{

}

ANG_IMPLEMENT_CLASSNAME(ang::object);
ANG_IMPLEMENT_ISCHILDOF_BASE(ang::object, ang::iobject);
ANG_IMPLEMENT_OBJECTNAME(ang::object);
ANG_IMPLEMENT_ISKINDOF_BASE(ang::object, ang::iobject);

bool object::query_object(type_name_t type, unknown_ptr_t out)
{
	if (out == null)
		return false;
	if (type == type_name<object>())
	{
		*out = this;
		return true;
	}
	else if (type == type_name<iobject>())
	{
		*out = static_cast<iobject*>(this);
		return true;
	}
	return false;
}

dword object::add_ref()
{
#if defined WINAPI_FAMILY
	if (ref_count < 0U)
		return ref_count;
	return InterlockedIncrement(&ref_count);
#else
	if (ref_count < 0U)
		return false;
	return ++ref_count;
#endif
}

dword object::release()
{
	if (ref_count < 0U)
		return -1;
#if defined WINAPI_FAMILY
	dword r = InterlockedDecrement(&ref_count);
	if (r <= 0U)
#else
	ref_count--;
	if (ref_count <= 0U)
#endif	
	{
		auto_release();
		return 0;
	}
	return ref_count;
}

bool object::auto_release()
{
#ifdef WINDOWS_PLATFORM
	InterlockedIncrement(&GET_SMART_PTR_INFO(this)->_shared_counter);
#elif defined ANDROID_PLATFORM
	++GET_SMART_PTR_INFO(this)->_shared_counter;
#endif
	delete this;
	
#ifdef WINDOWS_PLATFORM
	dword r = InterlockedDecrement(&GET_SMART_PTR_INFO(this)->_shared_counter);
#elif defined ANDROID_PLATFORM
	dword r = --GET_SMART_PTR_INFO(this)->_shared_counter;
#endif
	if (r <= 0U)
	{
		ang::memory::allocator_manager::get_allocator(memory::allocator_manager::object_allocator)->memory_release(GET_SMART_PTR_INFO(this));
	}
	return true;
}

bool object::auto_release(ushort)
{
#ifdef WINDOWS_PLATFORM
	InterlockedIncrement(&GET_SMART_PTR_INFO(this)->_shared_counter);
#elif defined ANDROID_PLATFORM
	++GET_SMART_PTR_INFO(this)->_shared_counter;
#endif
	delete this;
#ifdef WINDOWS_PLATFORM
	dword r = InterlockedDecrement(&GET_SMART_PTR_INFO(this)->_shared_counter);
#elif defined ANDROID_PLATFORM
	dword r = --GET_SMART_PTR_INFO(this)->_shared_counter;
#endif
	if (r <= 0U)
	{
		ang::memory::allocator_manager::get_allocator(memory::allocator_manager::aligned_allocator)->memory_release(GET_SMART_PTR_INFO(this));
	}
	return true;
}

comparision_result_t object::compare(object const& obj)const
{
	if (&obj == this)
		return comparision_result::same;
	return comparision_result::diferent;
}

string object::to_string()const
{
	return object_name();
}

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
