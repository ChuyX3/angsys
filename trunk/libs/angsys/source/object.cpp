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
#include "runtime_manager.h"
#include <assert.h>

using namespace ang;

bool intf<void, iid("ang::intf")>::default_query_interface(rtti_t const& src_id, unknown_t src, rtti_t const& out_id, unknown_ptr_t out)
{
#ifdef _DEBUG
	assert(src_id.is_type_of(iintf::class_info()));
#endif
	return src ? reinterpret_cast<iintf*>(src)->query_interface(out_id, out) : false;
}

rtti_t const& iintf::class_info() {
	static type_name_t name = "ang::iintf";
	static rtti_t const& info = rtti::regist(name, gender::class_type, sizeof(iintf), alignof(wsize), null, &default_query_interface);
	return info;
	align_of<iintf>();
}

//COFFE_IMPLEMENT_INTERFACE_CLASS_INFO(ang::iobject);

typedef struct smart_ptr_info
{
	ang_atomic_dword_t _mem_ref_counter; //4bytes
	ang_atomic_dword_t _obj_ref_counter; //4bytes
	ang_memory_hint_t _alloc;
	uint _object;

	inline ang::iintf* iintf() {
		return (ang::iintf*)(wsize(this) + _object);
	}
	inline dword add_mem_ref() {
		return ang_atomic_increment(&_mem_ref_counter);
	}
	inline dword add_obj_ref() {
		return ang_atomic_increment(&_obj_ref_counter);
	}
	inline dword remove_mem_ref() {
		return ang_atomic_decrement(&_mem_ref_counter);
	}
	inline dword remove_obj_ref() {
		return ang_atomic_decrement(&_obj_ref_counter);
	}
	static smart_ptr_info* from_intf(ang::iintf* ptr) {
		return smart_ptr_info_ptr_t(wsize(ptr) - align_up<16, sizeof(smart_ptr_info_t)>());
	}
}smart_ptr_info_t, *smart_ptr_info_ptr_t;


//////////////////////////////////////////////////////////////////////////////////////

weak_ptr_base::weak_ptr_base()
	: _info(nullptr)
	, _offset(0)
{

}

weak_ptr_base::weak_ptr_base(weak_ptr_base&& other)
	: _info(other._info)
	, _offset(other._offset)
{
	other._info = nullptr;
}

weak_ptr_base::weak_ptr_base(weak_ptr_base const& other)
	: _info(nullptr)
	, _offset(0)
{
	set(const_cast<weak_ptr_base&>(other).lock<iintf>());
}


weak_ptr_base::weak_ptr_base(std::nullptr_t const&)
	: _info(nullptr)
	, _offset(0)
{
}

weak_ptr_base::weak_ptr_base(iintf* obj)
	: _info(nullptr)
	, _offset(0)
{
	set(obj);
}

weak_ptr_base::~weak_ptr_base()
{
	reset();
}

void weak_ptr_base::reset()
{
	if (_info)
	{
		dword r = smart_ptr_info_ptr_t(_info)->remove_mem_ref();
		if (r <= 0U && smart_ptr_info_ptr_t(_info)->_object == 0)
		{
			ang_free_managed_memory(_info, smart_ptr_info_ptr_t(_info)->_alloc);
		}
	}
	_info = nullptr;
	_offset = 0;
}

void weak_ptr_base::set(iintf* _obj)
{
	iobject* obj = interface_cast<iobject>(_obj);

	if (_info && smart_ptr_info_ptr_t(_info)->iintf() == obj)
		return;

	reset();

	if (obj != nullptr)
	{
		_offset = wsize(_obj) - wsize(obj);
		auto info = smart_ptr_info::from_intf(obj);
		_info = info;
		smart_ptr_info_ptr_t(_info)->add_mem_ref();
	}
}

bool weak_ptr_base::is_valid()const
{
	auto info = smart_ptr_info_ptr_t(_info);
	return ((info) ? (info->_object != 0) : false);
}

template<>
intfptr weak_ptr_base::lock<iintf>()
{
	return is_valid() ? (iintf*)(wsize(smart_ptr_info_ptr_t(_info)->iintf()) + _offset) : nullptr;
}

weak_ptr_base& weak_ptr_base::operator = (iintf* obj)
{
	set(obj);
	return *this;
}

weak_ptr_base& weak_ptr_base::operator = (intfptr obj)
{
	set(obj.get());
	return *this;
}

weak_ptr_base& weak_ptr_base::operator = (weak_ptr_base&& other)
{
	if (this != &other)
		return*this;
	reset();

	_info = other._info;
	_offset = other._offset;
	other._info = nullptr;
	other._offset = 0;
	return *this;
}

weak_ptr_base& weak_ptr_base::operator = (weak_ptr_base const& ptr)
{
	intfptr obj = const_cast<weak_ptr_base&>(ptr).lock<iintf>();
	set(obj.get());
	return *this;
}

weak_ptr_base& weak_ptr_base::operator = (std::nullptr_t const&)
{
	reset();
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////

void* ang_alloc_object_memory(wsize sz, ang_memory_hint_t hint, ushort ALIGMENT)
{
	smart_ptr_info_ptr_t ptr;
	if (hint == ang_aligned_memory) {
		ptr = (smart_ptr_info_ptr_t)ang_alloc_aligned_memory(sz + align_up<16, size_of<smart_ptr_info_t>()>(), ALIGMENT);
	}
	else {
		ptr = (smart_ptr_info_ptr_t)ang_alloc_managed_memory(sz + align_up<16, size_of<smart_ptr_info_t>()>(), hint);
	}
	ptr->_obj_ref_counter = 0;
	ptr->_mem_ref_counter = 0;
	ptr->_alloc = hint;
	ptr->_object = (uint)align_up<16, sizeof(smart_ptr_info_t)>();
	return static_cast<object*>(ptr->iintf());
}

#ifdef _DEBUG
void* ang_alloc_object_memory_debug(wsize sz, const char* file, int line, ang_memory_hint_t hint, ushort ALIGMENT)
{
	smart_ptr_info_ptr_t ptr;
	if (hint == ang_aligned_memory) {
		ptr = (smart_ptr_info_ptr_t)ang_alloc_aligned_memory_debug(sz + align_up<16, size_of<smart_ptr_info_t>()>(), ALIGMENT, file, line);
	}
	else {
		ptr = (smart_ptr_info_ptr_t)ang_alloc_managed_memory_debug(sz + align_up<16, size_of<smart_ptr_info_t>()>(), hint, file, line);
	}
	ptr->_obj_ref_counter = 0;
	ptr->_mem_ref_counter = 0;
	ptr->_alloc = hint;
	ptr->_object = (uint)align_up<16, sizeof(smart_ptr_info_t)>();
	return static_cast<object*>(ptr->iintf());
}
#endif

pointer object::operator new(wsize sz) noexcept
{
	return ang_alloc_object_memory(sz, ang_object_memory, 4);
}

void object::operator delete(pointer ptr)
{
	smart_ptr_info_ptr_t info = smart_ptr_info::from_intf((iintf*)ptr);
	info->_object = 0;
	if (info->_mem_ref_counter == 0)
	{
		ang_free_managed_memory(info, info->_alloc);
	}
}

pointer object::operator new(wsize sz, const ushort ALIGMENT)
{
	return ang_alloc_object_memory(sz, ang_aligned_memory, ALIGMENT);
}

void object::operator delete(pointer ptr, const ushort)
{
	smart_ptr_info_ptr_t info = smart_ptr_info::from_intf((iintf*)ptr);
	info->_object = 0;
	if (info->_mem_ref_counter == 0)
	{
		ang_free_managed_memory(info, info->_alloc);
	}
}

#ifdef _DEBUG
pointer object::operator new(wsize sz, const char* file, int line)
{
	return ang_alloc_object_memory_debug(sz, file, line, ang_aligned_memory, 16);
}

pointer object::operator new(wsize sz, const ushort ALIGMENT, const char* file, int line)
{
	return ang_alloc_object_memory_debug(sz, file, line, ang_aligned_memory, ALIGMENT);
}
#endif

pointer object::operator new[](wsize)noexcept { return null; }
void object::operator delete[](pointer)noexcept {}

pointer object::operator new(wsize, pointer ptr)noexcept { return ptr; }
void object::operator delete(pointer, pointer)noexcept { }

object::object()
{
#if defined _DEBUG || defined _DEVELOPPER
	object_manager::instance()->push(this);
#endif
}

object::~object()
{
#if defined _DEBUG || defined _DEVELOPPER
	object_manager::instance()->pop(this);
#endif
}

ang::rtti_t const& ang::object::class_info() {
	static type_name_t name = "ang::object"_sv;
	static inherit_pack_info_t parents
		= ang::rtti_from_pack<object, type_pack<iobject>>::types();
	static rtti_t const& info = rtti::regist(name, gender::class_type, size_of<object>(), align_of<object>(), parents, &iintf::default_query_interface);
	return info;
}

ang::rtti_t const& ang::object::runtime_info()const {
	return object::class_info();
}

bool ang::object::query_interface(ang::rtti_t const& id, unknown_ptr_t out)
{
	return __query_interface<object, iobject, iintf>::query_interface(this, id, out);
}

dword object::add_ref()
{
	return smart_ptr_info::from_intf(this)->add_obj_ref();
}

dword object::release()
{
	auto count = smart_ptr_info::from_intf(this)->remove_obj_ref();
	if (count == 0)
		auto_release();
	return count;
}

bool object::auto_release()
{
	smart_ptr_info_t* info = smart_ptr_info::from_intf(this);

	info->add_mem_ref();

	this->dispose();
	this->~object(); //call destructor;
	info->_object = 0;

	auto count = info->remove_mem_ref();

	if (count <= 0)
	{
		ang_free_managed_memory(info, info->_alloc);
	}
	return true;
}

void object::dispose()
{
	//clear all data
}

string object::to_string()const
{
	return runtime_info().type_name();
}

string object::to_string(text::text_format_t)const
{
	return runtime_info().type_name();
}

/////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////

object_wrapper<object>::object_wrapper() : m_ptr(null) {

}

object_wrapper<object>::object_wrapper(object* ptr) : m_ptr(null) {
	set(ptr);
}

object_wrapper<object>::object_wrapper(object_wrapper && other) : m_ptr(null) {
	object * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

object_wrapper<object>::object_wrapper(object_wrapper const& other) : m_ptr(null) {
	set(other.m_ptr);
}

object_wrapper<object>::object_wrapper(ang::nullptr_t const&) : m_ptr(null) {
}

object_wrapper<object>::~object_wrapper()
{
	reset();
}

void object_wrapper<object>::reset()
{
	if (m_ptr)m_ptr->release();
	m_ptr = null;
}

void object_wrapper<object>::reset_unsafe()
{
	m_ptr = null;
}

bool object_wrapper<object>::is_empty()const
{
	return m_ptr == null;
}

object* object_wrapper<object>::get(void)const
{
	return m_ptr;
}

void object_wrapper<object>::set(object* ptr)
{
	object * temp = m_ptr;
	if (ptr == m_ptr) return;
	m_ptr = ptr;
	if (m_ptr)m_ptr->add_ref();
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
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

object_wrapper<object>& object_wrapper<object>::operator = (object_wrapper<object> const& other)
{
	set(other.m_ptr);
	return*this;
}

object ** object_wrapper<object>::addres_of(void)
{
	return &m_ptr;
}

object_wrapper_ptr<object> object_wrapper<object>::operator& (void)
{
	return this;
}

object* object_wrapper<object>::operator -> (void)
{
	return get();
}

object const* object_wrapper<object>::operator -> (void)const
{
	return get();
}

object_wrapper<object>::operator object* (void)
{
	return get();
}

object_wrapper<object>::operator object const* (void)const
{
	return get();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

intf_wrapper<iintf>::intf_wrapper() : m_ptr(null) {
}

intf_wrapper<iintf>::intf_wrapper(ang::nullptr_t const&) : m_ptr(null) {
}

intf_wrapper<iintf>::intf_wrapper(iintf* ptr) : m_ptr(null) {
	set(ptr);
}

intf_wrapper<iintf>::intf_wrapper(intf_wrapper && other) : m_ptr(null) {
	iintf * temp = other.m_ptr;
	other.m_ptr = null;
	m_ptr = temp;
}

intf_wrapper<iintf>::intf_wrapper(intf_wrapper const& other) : m_ptr(null) {
	set(other.m_ptr);
}

intf_wrapper<iintf>::~intf_wrapper() {
	reset();
}

void intf_wrapper<iintf>::reset()
{
	iobject * _obj = interface_cast<iobject>(m_ptr);
	if (_obj)_obj->release();
	m_ptr = null;
}

bool intf_wrapper<iintf>::is_empty()const
{
	return m_ptr == null;
}

iintf* intf_wrapper<iintf>::get(void)const
{
	return m_ptr;
}

void intf_wrapper<iintf>::set(iintf* ptr)
{
	if (ptr == m_ptr) return;
	iobject * _old = interface_cast<iobject>(m_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	m_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}

intf_wrapper<iintf>& intf_wrapper<iintf>::operator = (iintf* ptr)
{
	set(ptr);
	return*this;
}

intf_wrapper<iintf>& intf_wrapper<iintf>::operator = (ang::nullptr_t const&)
{
	reset();
	return*this;
}

intf_wrapper<iintf>& intf_wrapper<iintf>::operator = (intf_wrapper<iintf> && other)
{
	if (this == &other)
		return *this;
	reset();
	m_ptr = other.m_ptr;
	other.m_ptr = null;
	return*this;
}

intf_wrapper<iintf>& intf_wrapper<iintf>::operator = (intf_wrapper<iintf> const& other)
{
	set(other.m_ptr);
	return*this;
}

iintf ** intf_wrapper<iintf>::addres_of(void)
{
	return &m_ptr;
}

intf_wrapper_ptr<iintf> intf_wrapper<iintf>::operator & (void)
{
	return this;
}

iintf* intf_wrapper<iintf>::operator -> (void)
{
	return get();
}

iintf const* intf_wrapper<iintf>::operator -> (void)const
{
	return get();
}

intf_wrapper<iintf>::operator iintf* (void)
{
	return get();
}

intf_wrapper<iintf>::operator iintf const* (void)const
{
	return get();
}
