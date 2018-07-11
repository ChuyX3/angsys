#ifndef __COM_WRAPPER_HPP__
#define __COM_WRAPPER_HPP__

#include <angsys.hpp>
#include <ang/collections/vector.hpp>
#include <ang/core/async.hpp>
#include <Unknwn.h>

namespace ang
{
	template<typename T> class com_wrapper_ptr;

	template<typename T>
	class com_wrapper : public rule<is_base_of<IUnknown, T>::value>
	{
	public:
		typedef T type;

	private:
		type* _ptr;

	public:
		com_wrapper() : _ptr(null) {}
		com_wrapper(type* ptr) : _ptr(null) { set(ptr); }
		com_wrapper(com_wrapper && other) {
			T * temp = other._ptr;
			other._ptr = null;
			_ptr = temp;
		}
		com_wrapper(com_wrapper const& other) : _ptr(null) {
			set(other._ptr);
		}
		com_wrapper(std::nullptr_t const&) : _ptr(null) {}
		~com_wrapper() { clean(); }

	public:
		void clean() {
			if (_ptr)_ptr->Release();
			_ptr = null;
		}
		bool is_empty()const { return _ptr == null; }
		type* get(void)const { return _ptr; }
		void set(type* ptr) {
			T * temp = _ptr;
			if (ptr == _ptr) return;
			_ptr = ptr;
			if (_ptr)_ptr->AddRef();
			if (temp)temp->Release();
		}
		type ** addres_of(void) { return &_ptr; }

	public:
		com_wrapper& operator = (type* ptr) {
			set(ptr);
			return*this;
		}
		com_wrapper& operator = (com_wrapper && other) {
			T * temp = other._ptr;
			other._ptr = null;
			_ptr = temp;
			return*this;
		}
		com_wrapper& operator = (com_wrapper const& other) {
			set(other._ptr);
			return*this;
		}

		com_wrapper_ptr<T> operator & (void) { return this; }
		type * operator -> (void) { return get(); }
		type const* operator -> (void)const { return get(); }
		operator type * (void) { return get(); }
		operator type const* (void)const { return get(); }
	};

	template<typename T>
	class com_wrapper_ptr
	{
		com_wrapper<T>* _ptr;
	public:
		com_wrapper_ptr(std::nullptr_t const&) : _ptr(null) {}
		com_wrapper_ptr(com_wrapper<T>*ptr) : _ptr(ptr) {}
		com_wrapper_ptr(com_wrapper_ptr && ptr) : _ptr(ptr._ptr) { ptr._ptr = null; }
		com_wrapper_ptr(com_wrapper_ptr const& ptr) : _ptr(ptr._ptr) {}
		~com_wrapper_ptr() { _ptr = null; }
		bool is_empty()const { return _ptr == null; }
		com_wrapper<T>& operator *()const { return *_ptr; }
		com_wrapper<T>* operator ->()const { return _ptr; }
		operator com_wrapper<T>*()const { return _ptr; }
		operator T**()const { return _ptr->addres_of(); }
		explicit operator void**()const { return (void**)_ptr->addres_of(); }
	};

	template<typename T>
	class safe_thread_wrapper
	{
	public:
		typedef T type;

	private:
		type* _ptr;
		core::async::scope_locker<core::async::mutex_t> _locker;

	public:
		safe_thread_wrapper(type* ptr, core::async::mutex_t& mutex) : _ptr(null), _locker(mutex) { set(ptr); }
		~safe_thread_wrapper() { clean(); }

	public:
		void clean() {
			if (_ptr)_ptr->Release();
			_ptr = null;
		}
		void set(type* ptr) {
			T * temp = _ptr;
			if (ptr == _ptr) return;
			_ptr = ptr;
			if (_ptr)_ptr->AddRef();
			if (temp)temp->Release();
		}
		type* get(void)const { return _ptr; }

	public:
		type * operator -> (void) { return get(); }
		type const* operator -> (void)const { return get(); }
		operator type * (void) { return get(); }
		operator type const* (void)const { return get(); }
	};

	namespace runtime
	{
		template<typename T>
		struct runtime_type_builder<com_wrapper<T>> 
			: public runtime_type_builder<typename com_wrapper<T>::type> {
		};
	}

}

#include <ang/inline/com_wrapper.inl>

#endif//__COM_WRAPPER_HPP__