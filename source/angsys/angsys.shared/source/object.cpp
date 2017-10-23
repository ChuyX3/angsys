#include "pch.h"
#include <angsys.h>

using namespace ang;

ANG_IMPLEMENT_INTERFACE(ang, iobject);
ANG_IMPLEMENT_ENUM(ang, comparision_result, int, comparision_result::diferent);

ang::type_name_t interface_t::class_name() { return "interface"; }
bool interface_t::is_child_of(type_name_t name) { return name == class_name(); }


#define ANG_MEMORY_MANAGER

#ifdef ANG_MEMORY_MANAGER
#define ANG_ALLOCATOR_ALLOC(_SIZE) ang::memory::object_allocator<byte>::alloc(_SIZE)
#define ANG_ALLOCATOR_ALLOC_DEBUG(_SIZE, _FILE, _LINE) ang::memory::object_allocator<byte>::alloc(_SIZE, _FILE, _LINE)
#define ANG_ALLOCATOR_RELEASE(_PTR) ang::memory::object_allocator<byte>::free(_PTR)
#define ANG_ALIGNED_ALLOCATOR_ALLOC(_SIZE, _ALIGMENT) ang::memory::aligned_allocator<byte, _ALIGMENT>::alloc(_SIZE)
#define ANG_ALIGNED_ALLOCATOR_RELEASE(_PTR, _ALIGMENT) ang::memory::aligned_allocator<byte, _ALIGMENT>::free(_PTR)
#else
#define ANG_ALLOCATOR_ALLOC(_SIZE) malloc(_SIZE)
#define ANG_ALLOCATOR_ALLOC_DEBUG(_SIZE, _FILE, _LINE) malloc(_SIZE)
#define ANG_ALLOCATOR_RELEASE(_PTR) free(_PTR)
#define ANG_ALIGNED_ALLOCATOR_ALLOC(_SIZE, _ALIGMENT) _mm_malloc(_SIZE, _ALIGMENT)
#define ANG_ALIGNED_ALLOCATOR_RELEASE(_PTR, _ALIGMENT) _mm_free(_PTR)
#endif

typedef struct smart_ptr_info //16bytes for correct 16 byte alignment
{
	dword _shared_counter; //4bytes
	dword _ref_counter; //4bytes
	union
	{
		interface_t* _object; //4-8bytes
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
	set(const_cast<safe_pointer&>(other).lock<intfptr>());
}


safe_pointer::safe_pointer(ang::nullptr_t const&)
	: _info(null)
{
}

safe_pointer::safe_pointer(interface_t* obj)
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
			ANG_ALLOCATOR_RELEASE((byte*)_info);
		}
	}
	_info = null;
}

void safe_pointer::set(interface_t* obj)
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
intfptr safe_pointer::lock<intfptr>()
{
	return is_valid() ? smart_ptr_info_ptr_t(_info)->_object : null;
}

safe_pointer& safe_pointer::operator = (interface_t* obj)
{
	set(obj);
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
	intfptr obj = const_cast<safe_pointer&>(ptr).lock<intfptr>();
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
	smart_ptr_info_ptr_t ptr = (smart_ptr_info_ptr_t)ANG_ALLOCATOR_ALLOC(size + sizeof(smart_ptr_info_t));
	ptr->_shared_counter = 0;
	ptr->_ref_counter = 0;
	ptr->_object = (interface_t*)ang_void_ptr_t(wsize(ptr) + sizeof(smart_ptr_info_t));
	return ptr->_object;
}

ang_void_ptr_t object::operator new(wsize size, ushort ALIGMENT)
{
	smart_ptr_info_ptr_t ptr = (smart_ptr_info_ptr_t)ANG_ALIGNED_ALLOCATOR_ALLOC(size + sizeof(smart_ptr_info_t), 16U);
	ptr->_shared_counter = 0;
	ptr->_ref_counter = 0;
	ptr->_object = (interface_t*)ang_void_ptr_t(wsize(ptr) + sizeof(smart_ptr_info_t));
	return ptr->_object;
}

ang_void_ptr_t object::operator new[](wsize size)throw()
{
	//throw exception_t(except_code::invalid_access);
	return null;
}

void object::operator delete(ang_void_ptr_t ptr)
{
	smart_ptr_info_t* info = (smart_ptr_info_t*)((wsize)ptr - sizeof(smart_ptr_info_t));
	info->_object = null;
	if (info->_shared_counter == 0)
		ANG_ALLOCATOR_RELEASE((byte*)info);
}

void object::operator delete(ang_void_ptr_t ptr, ushort)
{
	smart_ptr_info_t* info = (smart_ptr_info_t*)((wsize)ptr - sizeof(smart_ptr_info_t));
	info->_object = null;
	if (info->_shared_counter == 0)
		ANG_ALIGNED_ALLOCATOR_RELEASE((byte*)info, 16u);
}

void object::operator delete[](ang_void_ptr_t ptr)throw()
{
	//throw exception_t(except_code::invalid_access);
}

#ifdef _DEBUG
ang_void_ptr_t object::operator new(wsize size, char const* file, int line)
{
	smart_ptr_info_ptr_t ptr = (smart_ptr_info_ptr_t)ANG_ALLOCATOR_ALLOC_DEBUG(size + sizeof(smart_ptr_info_t), file, line);
	ptr->_shared_counter = 0;
	ptr->_ref_counter = 0;
	ptr->_object = (interface_t*)ang_void_ptr_t(wsize(ptr) + sizeof(smart_ptr_info_t));
	return ptr->_object;
}

void object::operator delete(ang_void_ptr_t ptr, char const* file, int line)
{
	smart_ptr_info_ptr_t info = (smart_ptr_info_ptr_t)((wsize)ptr - sizeof(smart_ptr_info_t));
	info->_object = null;
	if (info->_shared_counter == 0)
		ANG_ALLOCATOR_RELEASE((byte*)info);
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
	if (type == type_of<object>())
	{
		*out = this;
		return true;
	}
	else if (type == type_of<iobject>())
	{
		*out = static_cast<iobject*>(this);
		return true;
	}
	return false;
}

dword object::add_ref()
{
#if defined WINAPI_FAMILY
	if (long(ref_count) < 0)
		return ref_count;
	return InterlockedIncrement(&ref_count);
#else
	if (long(ref_count) < 0)
		return false;
	return ++ref_count;
#endif
}

dword object::release()
{
	if (long(ref_count) < 0)
		return -1;
#if defined WINAPI_FAMILY
	dword r = InterlockedDecrement(&ref_count);
	if (r <= 0U)
#else
	ref_count--;
	if (long(ref_count) <= 0)
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
		ANG_ALLOCATOR_RELEASE((byte*)GET_SMART_PTR_INFO(this));
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
		ANG_ALIGNED_ALLOCATOR_RELEASE((byte*)GET_SMART_PTR_INFO(this), 16u);
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



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

ang::intf_wrapper<ang::interface_t>::intf_wrapper() : _ptr(null) {
}

ang::intf_wrapper<ang::interface_t>::intf_wrapper(ang::nullptr_t const&) : _ptr(null) {
}

ang::intf_wrapper<ang::interface_t>::intf_wrapper(ang::interface_t* ptr) : _ptr(null) {
	set(ptr);
}

ang::intf_wrapper<ang::interface_t>::intf_wrapper(intf_wrapper && other) : _ptr(null) {
	ang::interface_t * temp = other._ptr;
	other._ptr = null;
	_ptr = temp;
}

ang::intf_wrapper<ang::interface_t>::intf_wrapper(intf_wrapper const& other) : _ptr(null) {
	set(other._ptr);
}


ang::intf_wrapper<ang::interface_t>::~intf_wrapper() {
	clean();
}


inline void ang::intf_wrapper<ang::interface_t>::clean()
{
	has_runtime_type_info<interface_t>::value;

	iobject * _obj = interface_cast<iobject>(_ptr);
	if (_obj)_obj->release();
	_ptr = null;
}


inline bool ang::intf_wrapper<ang::interface_t>::is_empty()const
{
	return _ptr == null;
}


inline ang::interface_t* ang::intf_wrapper<ang::interface_t>::get(void)const
{
	return _ptr;
}


inline void ang::intf_wrapper<ang::interface_t>::set(ang::interface_t* ptr)
{
	if (ptr == _ptr) return;
	iobject * _old = interface_cast<iobject>(_ptr);
	iobject * _new = interface_cast<iobject>(ptr);
	_ptr = ptr;
	if (_new)_new->add_ref();
	if (_old)_old->release();
}


inline ang::intf_wrapper<ang::interface_t>& ang::intf_wrapper<ang::interface_t>::operator = (ang::interface_t* ptr)
{
	set(ptr);
	return*this;
}


inline ang::intf_wrapper<ang::interface_t>& ang::intf_wrapper<ang::interface_t>::operator = (ang::nullptr_t const&)
{
	clean();
	return*this;
}


inline ang::intf_wrapper<ang::interface_t>& ang::intf_wrapper<ang::interface_t>::operator = (ang::intf_wrapper<ang::interface_t> && other)
{
	if (this == &other)
		return *this;
	clean();
	_ptr = other._ptr;
	other._ptr = null;
	return*this;
}


inline ang::intf_wrapper<ang::interface_t>& ang::intf_wrapper<ang::interface_t>::operator = (ang::intf_wrapper<ang::interface_t> const& other)
{
	set(other._ptr);
	return*this;
}


inline ang::interface_t ** ang::intf_wrapper<ang::interface_t>::addres_of(void)
{
	return &_ptr;
}


inline ang::intf_wrapper_ptr<ang::interface_t> ang::intf_wrapper<ang::interface_t>::operator & (void)
{
	return this;
}


inline ang::interface_t * ang::intf_wrapper<ang::interface_t>::operator -> (void)
{
	return get();
}


inline ang::interface_t const* ang::intf_wrapper<ang::interface_t>::operator -> (void)const
{
	return get();
}


inline ang::intf_wrapper<ang::interface_t>::operator ang::interface_t * (void)
{
	return get();
}


inline ang::intf_wrapper<ang::interface_t>::operator ang::interface_t const* (void)const
{
	return get();
}
