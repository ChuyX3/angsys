/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error 
#elif !defined __ANG_PROPERTIES_H__
#define __ANG_PROPERTIES_H__

namespace ang
{
	template<typename T, class P> class property
	{
	private:
		T _value;

	public:
		friend P;
		typedef property<T, P> self_t;
		typedef T type;

		property(property&& value) : _value(ang::move(value._value)) {}
		property(T&& value) : _value(ang::move(value)) {}
		property(const property& value) : _value(value._value) {}
		property(const T& value) : _value(value) {}
		property() : _value() {}

		operator const T&()const {
			return get();
		}

		operator T&() {
			return get();
		}

		T* operator ->()const {
			return const_cast<T*>(&_value);
		}

		property& operator = (property&& value) {
			set(ang::move(value._value));
			return*this;
		}

		property& operator = (T&& value) {
			set(ang::move(value));
			return*this;
		}

		property& operator = (const property& value) {
			set(value._value);
			return*this;
		}

		property& operator = (const T& value) {
			set(value);
			return*this;
		}

		const T* operator & ()const {
			return &get();
		}

		T* operator & () {
			return &get();
		}

		void set(T value) {
			_value = ang::move(value);
		}

		T& get() {
			return _value;
		}

		const T& get()const {
			return _value;
		}

		////////////////////////////////////////////
		bool operator == (const self_t& value)const {
			return _value == value._value;
		}
		bool operator != (const self_t& value)const {
			return _value != value._value;
		}
		bool operator >= (const self_t& value)const {
			return _value >= value._value;
		}
		bool operator <= (const self_t& value)const {
			return _value <= value._value;
		}
		bool operator > (const self_t& value)const {
			return _value > value._value;
		}
		bool operator < (const self_t& value)const {
			return _value < value._value;
		}

		bool operator == (const T& value)const {
			return _value == value;
		}
		bool operator != (const T& value)const {
			return _value != value;
		}
		bool operator >= (const T& value)const {
			return _value >= value;
		}
		bool operator <= (const T& value)const {
			return _value <= value;
		}
		bool operator > (const T& value)const {
			return _value > value;
		}
		bool operator < (const T& value)const {
			return _value < value;
		}
	};

	template<typename T, class P>
	class property<const T, P>
	{
	private:
		T _value;

	public:
		friend P;
		typedef property<const T, P> self_t;
		typedef T type;

		property(property&& value) : _value(ang::move(value._value)) {}
		property(const property& value) : _value(value._value) {}
		property(T&& value) : _value(ang::move(value)) {}
		property(const T& value) : _value(value) {}
		property() : _value() {}

		operator const T&()const {
			return get();
		}

		const T* operator ->()const {
			return &_value;
		}

		const T& get()const {
			return _value;
		}

		const T* operator & ()const {
			return &get();
		}

		bool operator == (const self_t& value)const {
			return _value == value._value;
		}
		bool operator != (const self_t& value)const {
			return _value != value._value;
		}
		bool operator >= (const self_t& value)const {
			return _value >= value._value;
		}
		bool operator <= (const self_t& value)const {
			return _value <= value._value;
		}
		bool operator > (const self_t& value)const {
			return _value > value._value;
		}
		bool operator < (const self_t& value)const {
			return _value < value._value;
		}

		bool operator == (const T& value)const {
			return _value == value;
		}
		bool operator != (const T& value)const {
			return _value != value;
		}
		bool operator >= (const T& value)const {
			return _value >= value;
		}
		bool operator <= (const T& value)const {
			return _value <= value;
		}
		bool operator > (const T& value)const {
			return _value > value;
		}
		bool operator < (const T& value)const {
			return _value < value;
		}

	private:

		property& operator = (property&& value) {
			set(ang::move(value._value));
			return*this;
		}

		property& operator = (T&& value) {
			set(ang::move(value));
			return*this;
		}

		property& operator = (const property& value) {
			set(value._value);
			return*this;
		}

		property& operator = (const T& value) {
			set(value);
			return*this;
		}

		void set(T value) {
			_value = ang::move(value);
		}
	};

	template<typename T, class P>
	class property<T*, P>
	{
	private:
		T* _value;

	public:
		friend P;
		typedef property<T*, P> self_t;
		typedef T type;
		typedef T* ptr_t;

		property(property&& value) : _value(value._value) { value._value = null; }
		property(const property& value) : _value(value._value) {}
		property(ptr_t value = null) : _value(value) {}

		bool is_empty()const { return _value == null; }

		operator ptr_t()const {
			return get();
		}

		property& operator = (property&& value) {
			set(value._value);
			value._value = null;
			return*this;
		}

		property& operator = (const property& value) {
			set(value._value);
			return*this;
		}

		ptr_t operator ->()const {
			return get();
		}

		property& operator = (ptr_t value) {
			set(value);
			return*this;
		}

		const ptr_t* operator & ()const {
			return &get();
		}

		ptr_t* operator & () {
			return &get();
		}

		void set(ptr_t value) {
			_value = value;
		}

		ptr_t get()const {
			return _value;
		}

		////////////////////////////////////////////
		bool operator == (const self_t& value)const {
			return _value == value._value;
		}
		bool operator != (const self_t& value)const {
			return _value != value._value;
		}
		bool operator >= (const self_t& value)const {
			return _value >= value._value;
		}
		bool operator <= (const self_t& value)const {
			return _value <= value._value;
		}
		bool operator > (const self_t& value)const {
			return _value > value._value;
		}
		bool operator < (const self_t& value)const {
			return _value < value._value;
		}

		bool operator == (const T& value)const {
			return _value == value;
		}
		bool operator != (const T& value)const {
			return _value != value;
		}
		bool operator >= (const T& value)const {
			return _value >= value;
		}
		bool operator <= (const T& value)const {
			return _value <= value;
		}
		bool operator > (const T& value)const {
			return _value > value;
		}
		bool operator < (const T& value)const {
			return _value < value;
		}

		bool operator == (const ang::nullptr_t&)const {
			return _value == null;
		}
		bool operator != (const ang::nullptr_t&)const {
			return _value != null;
		}
	};

	template<typename T, class P>
	class property<T const *, P>
	{
	private:
		T* _value;

	public:
		friend P;
		typedef property<T const*, P> self_t;
		typedef T type;
		typedef T* ptr_t;
		typedef T* cptr_t;

		property(property&& value) : _value(value._value) { value._value = null; }
		property(property const& value) : _value(value._value) {}
		property(ptr_t value = null) : _value(value) {}

		bool is_empty()const { return _value == null; }

		operator cptr_t()const {
			return get();
		}

		cptr_t operator ->()const {
			return get();
		}

		cptr_t get()const {
			return _value;
		}

		////////////////////////////////////////////
		bool operator == (const self_t& value)const {
			return _value == value._value;
		}
		bool operator != (const self_t& value)const {
			return _value != value._value;
		}
		bool operator >= (const self_t& value)const {
			return _value >= value._value;
		}
		bool operator <= (const self_t& value)const {
			return _value <= value._value;
		}
		bool operator > (const self_t& value)const {
			return _value > value._value;
		}
		bool operator < (const self_t& value)const {
			return _value < value._value;
		}

		bool operator == (const T& value)const {
			return _value == value;
		}
		bool operator != (const T& value)const {
			return _value != value;
		}
		bool operator >= (const T& value)const {
			return _value >= value;
		}
		bool operator <= (const T& value)const {
			return _value <= value;
		}
		bool operator > (const T& value)const {
			return _value > value;
		}
		bool operator < (const T& value)const {
			return _value < value;
		}

		bool operator == (const ang::nullptr_t&)const {
			return _value == null;
		}
		bool operator != (const ang::nullptr_t&)const {
			return _value != null;
		}

	protected:
		property& operator = (property&& value) {
			set(value._value);
			value = null;
			return*this;
		}

		property& operator = (const property& value) {
			set(value._value);
			return*this;
		}

		property& operator = (ptr_t value) {
			set(value);
			return*this;
		}

		void set(ptr_t value) {
			_value = value;
		}
	};

	template<typename T, class P>
	class property<const object_wrapper<T>, P>
	{
	private:
		object_wrapper<T> _value;

	public:
		friend P;
		typedef property<const object_wrapper<T>, P> self_t;
		typedef T type;
		typedef T* ptr_t;
		typedef T const* cptr_t;

		property(property&& value) : _value(value._value) { value._value = null; }
		property(property const& value) : _value(value._value) {}
		property(ptr_t value = null) : _value(value) {}

		bool is_empty()const { return _value.is_empty(); }

		operator const object_wrapper<T>& ()const {
			return get();
		}

		cptr_t operator ->()const {
			return get();
		}

		const object_wrapper<T>& get()const {
			return _value;
		}

		////////////////////////////////////////////
		bool operator == (const self_t& value)const {
			return _value.get() == value._value.get();
		}
		bool operator != (const self_t& value)const {
			return _value.get() != value._value.get();
		}
		bool operator >= (const self_t& value)const {
			return _value.get() >= value._value.get();
		}
		bool operator <= (const self_t& value)const {
			return _value.get() <= value._value.get();
		}
		bool operator > (const self_t& value)const {
			return _value.get() > value._value.get();
		}
		bool operator < (const self_t& value)const {
			return _value.get() < value._value.get();
		}

		bool operator == (const T& value)const {
			return _value.get() == value.get();
		}
		bool operator != (const T& value)const {
			return _value.get() != value.get();
		}
		bool operator >= (const T& value)const {
			return _value.get() >= value.get();
		}
		bool operator <= (const T& value)const {
			return _value.get() <= value.get();
		}
		bool operator > (const T& value)const {
			return _value.get() > value.get();
		}
		bool operator < (const T& value)const {
			return _value.get() < value.get();
		}

		bool operator == (const ang::nullptr_t&)const {
			return _value.get() == null;
		}
		bool operator != (const ang::nullptr_t&)const {
			return _value.get() != null;
		}

	protected:
		property& operator = (property&& value) {
			set(value._value);
			value = null;
			return*this;
		}

		property& operator = (const property& value) {
			set(value._value);
			return*this;
		}

		property& operator = (ptr_t value) {
			set(value);
			return*this;
		}

		void set(ptr_t value) {
			_value = value;
		}
	};


	template<typename T, class P>
	class property<const intf_wrapper<T>, P>
	{
	private:
		intf_wrapper<T> _value;

	public:
		friend P;
		typedef property<const intf_wrapper<T>, P> self_t;
		typedef T type;
		typedef T* ptr_t;
		typedef T const* cptr_t;

		property(property&& value) : _value(value._value) { value._value = null; }
		property(property const& value) : _value(value._value) {}
		property(ptr_t value = null) : _value(value) {}

		bool is_empty()const { return _value.is_empty(); }

		operator const intf_wrapper<T>& ()const {
			return get();
		}

		cptr_t operator ->()const {
			return get();
		}

		const intf_wrapper<T>& get()const {
			return _value;
		}

		////////////////////////////////////////////
		bool operator == (const self_t& value)const {
			return _value.get() == value._value.get();
		}
		bool operator != (const self_t& value)const {
			return _value.get() != value._value.get();
		}
		bool operator >= (const self_t& value)const {
			return _value.get() >= value._value.get();
		}
		bool operator <= (const self_t& value)const {
			return _value.get() <= value._value.get();
		}
		bool operator > (const self_t& value)const {
			return _value.get() > value._value.get();
		}
		bool operator < (const self_t& value)const {
			return _value.get() < value._value.get();
		}

		bool operator == (const T& value)const {
			return _value.get() == value.get();
		}
		bool operator != (const T& value)const {
			return _value.get() != value.get();
		}
		bool operator >= (const T& value)const {
			return _value.get() >= value.get();
		}
		bool operator <= (const T& value)const {
			return _value.get() <= value.get();
		}
		bool operator > (const T& value)const {
			return _value.get() > value.get();
		}
		bool operator < (const T& value)const {
			return _value.get() < value.get();
		}

		bool operator == (const ang::nullptr_t&)const {
			return _value.get() == null;
		}
		bool operator != (const ang::nullptr_t&)const {
			return _value.get() != null;
		}

	protected:
		property& operator = (property&& value) {
			set(value._value);
			value = null;
			return*this;
		}

		property& operator = (const property& value) {
			set(value._value);
			return*this;
		}

		property& operator = (ptr_t value) {
			set(value);
			return*this;
		}

		void set(ptr_t value) {
			_value = value;
		}
	};


	const uint PROPERTY_TYPE_DEFAULT = 0u;
	const uint PROPERTY_TYPE_ALERTABLE = 1u;
	const uint PROPERTY_TYPE_NOSTORAGE = 2u;

	template<typename D, typename T, uint TYPE = PROPERTY_TYPE_DEFAULT> struct base_property;

	template<typename D, typename T>
	struct base_property<D, T, PROPERTY_TYPE_DEFAULT>
	{
		typedef T type;
		typedef base_property<D, T, PROPERTY_TYPE_DEFAULT> base;
		typedef D self;

	protected:
		type _value;
		type get_value()const { return _value; }
		type set_value(type& value) { _value = ang::move(value); return _value; }
	};

	template<typename D, typename T>
	struct base_property<D, T, PROPERTY_TYPE_NOSTORAGE>
	{
		typedef T type;
		typedef base_property<D, T, PROPERTY_TYPE_NOSTORAGE> base;
		typedef D self;

	protected:
		inline type get_value()const { return D::get_property_value(this); }
		inline type set_value(type& value) { return D::set_property_value(this, value); }
	};

	template<typename D, typename T>
	struct base_property<D, T, PROPERTY_TYPE_ALERTABLE>
	{
		typedef T type;
		typedef base_property<D, T, PROPERTY_TYPE_ALERTABLE> base;
		typedef D self;

	protected:
		type _value;
		inline type get_value()const { return _value; }
		inline type set_value(type& value) { return D::set_property_value(this, value); }
	};

#define DECLARE_PROPERTY(_LINK, _PARENT, _NAME, _TYPE, ...) \
private : class _LINK _NAME##_property_t : public base_property<_NAME##_property_t, __VA_ARGS__> {  \
	friend _PARENT; \
	friend base; \
	static _TYPE get_property_value(_PARENT*); \
	static _TYPE set_property_value(_PARENT*, _TYPE&); \
	static inline _TYPE get_property_value(base* _base){ return ang::move(get_property_value(static_cast<self*>(_base)->get_parent())); } \
	static inline _TYPE set_property_value(base* _base, _TYPE& value){ return ang::move(set_property_value(static_cast<self*>(_base)->get_parent(), value)); } \
	_PARENT* get_parent()const{ return (_PARENT*)wsize(wsize(this) - offsetof(_PARENT,_NAME));  } \
	public : operator _TYPE ()const{ return ang::move(get_value()); } \
	public : _TYPE operator = (_TYPE value){ return ang::move(set_value(value)); } \
}; \
public: _NAME##_property_t _NAME;

#define DECLARE_READONLY_PROPERTY(_LINK, _PARENT, _NAME, _TYPE, ...) \
private : class _LINK _NAME##_property_t : public base_property<_NAME##_property_t, __VA_ARGS__> {  \
	friend _PARENT; \
	friend base; \
	static _TYPE get_property_value(_PARENT*); \
	static _TYPE set_property_value(_PARENT*, _TYPE&); \
	static inline _TYPE get_property_value(base* _base){ return ang::move(get_property_value(static_cast<self*>(_base)->get_parent())); } \
	static inline _TYPE set_property_value(base* _base, _TYPE& value){ return ang::move(set_property_value(static_cast<self*>(_base)->get_parent(), value)); } \
	_PARENT* get_parent()const{ return (_PARENT*)wsize(wsize(this) - offsetof(_PARENT,_NAME));  } \
	public : operator _TYPE ()const{ return ang::move(get_value()); } \
}; \
public: _NAME##_property_t _NAME;

#define ang_property(_LINK, _PARENT, _TYPE, _NAME, ...) DECLARE_PROPERTY(_LINK, _PARENT, _NAME, _TYPE, _TYPE, __VA_ARGS__)
#define ang_readonly_property(_LINK, _PARENT, _TYPE, _NAME, ...) DECLARE_READONLY_PROPERTY(_LINK, _PARENT, _NAME, _TYPE, _TYPE, __VA_ARGS__)

}

#endif//__PROPERTIES_H__