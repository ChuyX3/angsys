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
	const uint PROPERTY_TYPE_DEFAULT = 0u;
	const uint PROPERTY_TYPE_ALERTABLE = 1u;
	const uint PROPERTY_TYPE_NOSTORAGE = 2u;

	template<typename T, typename P, typename D = P, uint TYPE = PROPERTY_TYPE_DEFAULT> class property;

	template<typename T, typename P, typename D> class property<T, P, D, PROPERTY_TYPE_DEFAULT>
	{
	private:
		T _value;

	public:
		friend P;
		typedef property<T, P, D, PROPERTY_TYPE_DEFAULT> self_t;
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

	template<typename T, typename P, typename D>
	class property<const T, P, D, PROPERTY_TYPE_DEFAULT>
	{
	private:
		T _value;

	public:
		friend P;
		typedef  property<const T, P, D, PROPERTY_TYPE_DEFAULT> self_t;
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

	template<typename T, typename P, typename D>
	class property<T*, P, D, PROPERTY_TYPE_DEFAULT>
	{
	private:
		T* _value;

	public:
		friend P;
		typedef property<T*, P, D, PROPERTY_TYPE_DEFAULT> self_t;
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

	template<typename T, typename P, typename D>
	class property<T const *, P, D, PROPERTY_TYPE_DEFAULT>
	{
	private:
		T* _value;

	public:
		friend P;
		typedef property<T const *, P, D, PROPERTY_TYPE_DEFAULT> self_t;
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

	template<typename T, typename P, typename D>
	class property<const object_wrapper<T>, P, D, PROPERTY_TYPE_DEFAULT>
	{
	private:
		object_wrapper<T> _value;

	public:
		friend P;
		typedef property<const object_wrapper<T>, P, D, PROPERTY_TYPE_DEFAULT> self_t;
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


	template<typename T, typename P, typename D>
	class property<const intf_wrapper<T>, P, D, PROPERTY_TYPE_DEFAULT>
	{
	private:
		intf_wrapper<T> _value;

	public:
		friend P;
		typedef property<const intf_wrapper<T>, P, D, PROPERTY_TYPE_DEFAULT> self_t;
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



	template<typename T, class P, class D> 
	class property<T, P, D, PROPERTY_TYPE_NOSTORAGE>
	{
	public:
		friend P;
		typedef property<T, P, D, PROPERTY_TYPE_NOSTORAGE> self_t;
		typedef T type;

	protected:
		property(T&& value) { set(value); }
		property(const property& value) { set(value.get()); }
		property(const T& value) { set(value); }
		property() {}

		operator T ()const { return get(); }
		property& operator = (T&& value) { set(value); return*this; }
		property& operator = (const property& value) {set(value.get()); return*this; }
		property& operator = (const T& value) {	set(value); return*this; }

		void set(T value) { D::set_property_value(this, value); }
		T get()const { return D::get_property_value(const_cast<self_t*>(this)); }

		////////////////////////////////////////////
		bool operator == (const self_t& value)const { return  get() == value. get(); }
		bool operator != (const self_t& value)const { return  get() != value. get(); }
		bool operator >= (const self_t& value)const { return  get() >= value. get(); }
		bool operator <= (const self_t& value)const { return  get() <= value. get(); }
		bool operator > (const self_t& value)const { return  get() > value. get(); }
		bool operator < (const self_t& value)const { return  get() < value. get(); }
		bool operator == (const T& value)const { return  get() == value; }
		bool operator != (const T& value)const { return  get() != value; }
		bool operator >= (const T& value)const { return  get() >= value; }
		bool operator <= (const T& value)const { return  get() <= value; }
		bool operator > (const T& value)const { return  get() > value; }
		bool operator < (const T& value)const { return  get() < value; }
	};

	template<typename T, class P, class D> 
	class property<T&, P, D, PROPERTY_TYPE_NOSTORAGE>
	{
	public:
		friend P;
		friend D;
		typedef property<T&, P, D, PROPERTY_TYPE_NOSTORAGE> self_t;
		typedef T type;

	protected:
		property(T&& value) { set(value); }
		property(const property& value) { set(value.get()); }
		property(const T& value) { set(value); }
		property() {}

		operator T const& ()const { return get(); }
		operator T& () { return get(); }
		T const* operator -> () const { return&get(); }
		T* operator -> () { return&get(); }

		property& operator = (T&& value) { set(ang::move(value)); return*this; }
		property& operator = (const property& value) { set(value.get()); return*this; }
		property& operator = (const T& value) { set(value); return*this; }

		void set(T value) { D::set_property_value(this, value); }
		T const& get()const { return D::get_property_value(const_cast<self_t*>(this)); }
		T& get() { return D::get_property_value(this); }

		////////////////////////////////////////////
		bool operator == (const self_t& value)const { return  get() == value.get(); }
		bool operator != (const self_t& value)const { return  get() != value.get(); }
		bool operator >= (const self_t& value)const { return  get() >= value.get(); }
		bool operator <= (const self_t& value)const { return  get() <= value.get(); }
		bool operator > (const self_t& value)const { return  get() > value.get(); }
		bool operator < (const self_t& value)const { return  get() < value.get(); }
		bool operator == (const T& value)const { return  get() == value; }
		bool operator != (const T& value)const { return  get() != value; }
		bool operator >= (const T& value)const { return  get() >= value; }
		bool operator <= (const T& value)const { return  get() <= value; }
		bool operator > (const T& value)const { return  get() > value; }
		bool operator < (const T& value)const { return  get() < value; }
	};

	template<typename T, typename P, typename D>
	class property<const T, P, D, PROPERTY_TYPE_NOSTORAGE>
	{
	public:
		friend P;
		typedef property<const T, P, D, PROPERTY_TYPE_NOSTORAGE> self_t;
		typedef T type;

	public:
		property(T&& value) { set(value); }
		property(const property& value) { set(value.get()); }
		property(const T& value) { set(value); }
		property() {}

		operator T ()const { return get(); }
		T get()const { return D::get_property_value(const_cast<self_t*>(this)); }

		bool operator == (const self_t& value)const { return  get() == value. get(); }
		bool operator != (const self_t& value)const { return  get() != value. get(); }
		bool operator >= (const self_t& value)const { return  get() >= value. get(); }
		bool operator <= (const self_t& value)const { return  get() <= value. get(); }
		bool operator > (const self_t& value)const { return  get() > value. get(); }
		bool operator < (const self_t& value)const { return  get() < value. get(); }
		bool operator == (const T& value)const { return  get() == value; }
		bool operator != (const T& value)const { return  get() != value; }
		bool operator >= (const T& value)const { return  get() >= value; }
		bool operator <= (const T& value)const { return  get() <= value; }
		bool operator > (const T& value)const { return  get() > value; }
		bool operator < (const T& value)const { return  get() < value; }

	private:
		property& operator = (T&& value) { set(ang::move(value)); return*this; }
		property& operator = (const property& value) { set(value.get()); return*this; }
		property& operator = (const T& value) { set(value); return*this; }
		void set(T value) { D::set_property_value(this, value); }
	};

	template<typename T, typename P, typename D>
	class property<const T&, P, D, PROPERTY_TYPE_NOSTORAGE>
	{
	public:
		friend P;
		typedef property<const T&, P, D, PROPERTY_TYPE_NOSTORAGE> self_t;
		typedef T type;

	public:
		property(T&& value) { set(value); }
		property(const property& value) { set(value.get()); }
		property(const T& value) { set(value); }
		property() {}

		operator T const& ()const { return get(); }
		const T& get()const { return D::get_property_value(const_cast<self_t*>(this)); }
		T const* operator -> () const { return&get(); }

		bool operator == (const self_t& value)const { return  get() == value.get(); }
		bool operator != (const self_t& value)const { return  get() != value.get(); }
		bool operator >= (const self_t& value)const { return  get() >= value.get(); }
		bool operator <= (const self_t& value)const { return  get() <= value.get(); }
		bool operator > (const self_t& value)const { return  get() > value.get(); }
		bool operator < (const self_t& value)const { return  get() < value.get(); }
		bool operator == (const T& value)const { return  get() == value; }
		bool operator != (const T& value)const { return  get() != value; }
		bool operator >= (const T& value)const { return  get() >= value; }
		bool operator <= (const T& value)const { return  get() <= value; }
		bool operator > (const T& value)const { return  get() > value; }
		bool operator < (const T& value)const { return  get() < value; }

	private:
		property& operator = (T&& value) { set(ang::move(value)); return*this; }
		property& operator = (const property& value) { set(value.get()); return*this; }
		property& operator = (const T& value) { set(value); return*this; }
		void set(T value) { D::set_property_value(this, value); }
	};

	template<typename T, typename P, typename D>
	class property<T*, P, D, PROPERTY_TYPE_NOSTORAGE>
	{
	public:
		friend P;
		typedef property<T*, P, D, PROPERTY_TYPE_NOSTORAGE> self_t;
		typedef T type;
		typedef T* ptr_t;

	public:
		property(const property& value) { set(value.get()); }
		property(ptr_t value) { set(value); }
		property() {}

		bool is_empty()const { return get() == null; }
		operator ptr_t()const { return get(); }
		ptr_t operator ->()const { return get(); }
		property& operator = (const property& value) { set(value._value); return*this; }
		property& operator = (ptr_t value) { set(value); return*this; }

		void set(ptr_t value) { D::set_property_value(this, value); }
		ptr_t get()const { return D::get_property_value(const_cast<self_t*>(this)); }

		////////////////////////////////////////////
		bool operator == (const self_t& value)const { return  get() == value.get(); }
		bool operator != (const self_t& value)const { return  get() != value.get(); }
		bool operator >= (const self_t& value)const { return  get() >= value.get(); }
		bool operator <= (const self_t& value)const { return  get() <= value.get(); }
		bool operator > (const self_t& value)const { return  get() > value.get(); }
		bool operator < (const self_t& value)const { return  get() < value.get(); }
		bool operator == (ptr_t value)const { return  get() == value; }
		bool operator != (ptr_t value)const { return  get() != value; }
		bool operator >= (ptr_t value)const { return  get() >= value; }
		bool operator <= (ptr_t value)const { return  get() <= value; }
		bool operator > (ptr_t value)const { return  get() > value; }
		bool operator < (ptr_t value)const { return  get() < value; }
		bool operator == (const ang::nullptr_t&)const { return get() == null; }
		bool operator != (const ang::nullptr_t&)const { return get() != null; }
	};

	template<typename T, typename P, typename D>
	class property<T const *, P, D, PROPERTY_TYPE_NOSTORAGE>
	{
	public:
		friend P;
		typedef property<T const *, P, D, PROPERTY_TYPE_NOSTORAGE> self_t;
		typedef T type;
		typedef T* ptr_t;
		typedef T* cptr_t;

	public:
		property(const property& value) { set(value.get()); }
		property(ptr_t value) { set(value); }
		property() {}

		bool is_empty()const { return get() == null; }
		operator cptr_t()const { return get(); }
		cptr_t operator ->()const { return get(); }
		cptr_t get()const { return D::get_property_value(const_cast<self_t*>(this)); }

		////////////////////////////////////////////
		bool operator == (const self_t& value)const { return  get() == value.get(); }
		bool operator != (const self_t& value)const { return  get() != value.get(); }
		bool operator >= (const self_t& value)const { return  get() >= value.get(); }
		bool operator <= (const self_t& value)const { return  get() <= value.get(); }
		bool operator > (const self_t& value)const { return  get() > value.get(); }
		bool operator < (const self_t& value)const { return  get() < value.get(); }
		bool operator == (ptr_t value)const { return  get() == value; }
		bool operator != (ptr_t value)const { return  get() != value; }
		bool operator >= (ptr_t value)const { return  get() >= value; }
		bool operator <= (ptr_t value)const { return  get() <= value; }
		bool operator > (ptr_t value)const { return  get() > value; }
		bool operator < (ptr_t value)const { return  get() < value; }
		bool operator == (const ang::nullptr_t&)const { return get() == null; }
		bool operator != (const ang::nullptr_t&)const { return get() != null; }

	protected:
		property& operator = (const property& value) { set(value.get()); return*this; }
		property& operator = (ptr_t value) { set(value); return*this; }
		void set(ptr_t value) { set(value); }
	};



#define ANG_PROPERTY(...) property<__VA_ARGS__>

#define DECLARE_PROPERTY(_LINK, _PARENT, _NAME, _TYPE, _PROP_TYPE) \
private : class _LINK _NAME##_property_t : public ANG_PROPERTY(_TYPE, _PARENT, _NAME##_property_t, _PROP_TYPE) {  \
	friend _PARENT; \
	typedef typename remove_constant<_TYPE>::type type; \
	friend ANG_PROPERTY(_TYPE, _PARENT, _NAME##_property_t, _PROP_TYPE); \
	static type get_property_value(_PARENT*); \
	static type set_property_value(_PARENT*, type); \
	static inline type get_property_value(ANG_PROPERTY(_TYPE, _PARENT, _NAME##_property_t, _PROP_TYPE)* _base){ return get_property_value(static_cast<_NAME##_property_t*>(_base)->get_parent()); } \
	static inline type set_property_value(ANG_PROPERTY(_TYPE, _PARENT, _NAME##_property_t, _PROP_TYPE)* _base, type value){ return set_property_value(static_cast<_NAME##_property_t*>(_base)->get_parent(), value); } \
	_PARENT* get_parent()const{ return (_PARENT*)wsize(wsize(this) - offsetof(_PARENT,_NAME));  } \
}; \
public: _NAME##_property_t _NAME;

#define ang_property(_LINK, _PARENT, _TYPE, _NAME, ...) DECLARE_PROPERTY(_LINK, _PARENT, _NAME, _TYPE, __VA_ARGS__)

}

#endif//__PROPERTIES_H__