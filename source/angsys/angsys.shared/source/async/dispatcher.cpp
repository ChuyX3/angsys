#include "pch.h"
#include "ang/core/async.hpp"
#include "thread.hpp"

using namespace ang;
using namespace ang::core;
using namespace ang::core::async;


ANG_IMPLEMENT_INTERFACE(ang::core::async, idispatcher);


//dispatcher_thread::dispatcher_thread() {}
//dispatcher_thread::~dispatcher_thread() {}
//
//
//ANG_IMPLEMENT_CLASSNAME(ang::core::async::dispatcher_thread);
//ANG_IMPLEMENT_OBJECTNAME(ang::core::async::dispatcher_thread);
//
//bool dispatcher_thread::is_child_of(type_name_t name)
//{
//	return name == class_name() ||
//		thread::is_child_of(name) ||
//		idispatcher::is_child_of(name);
//}
//
//bool dispatcher_thread::is_kind_of(type_name_t name)const
//{
//	return name == class_name() ||
//		thread::is_kind_of(name) ||
//		idispatcher::is_kind_of(name);
//}
//
//bool dispatcher_thread::query_object(type_name_t name, unknown_ptr_t out)
//{
//	if (out == null) return false;
//	if (name == type_of(*this)) {
//
//		*out = static_cast<dispatcher_thread*>(this);
//		return true;
//	}
//	else if (thread::query_object(name, out)) {
//		return true;
//	}
//	else if (idispatcher::query_object(name, out)) {
//		return true;
//	}
//	return false;
//}
