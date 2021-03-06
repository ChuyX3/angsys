/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_DELEGATES_HPP__
#error...

#else

#define MY_RTYPE int 
#define MY_ATYPES int, float

#define ANG_DECLARE_IFUNCTION_SPECIALIZATION(_LINK, RTYPE, ...)  ANG_BEGIN_IFUNCTION_SPECIALIZATION
namespace ang { namespace core { namespace delegates { \
template<> ANG_BEGIN_INTERFACE(_LINK, ifunction<RTYPE(__VA_ARGS__)>) \
	visible vcall RTYPE invoke(__VA_ARGS__)const pure; \
		visible vcall ifunction* clone()const pure; \
ANG_END_INTERFACE(); } } }


#define ANG_BEGIN_IFUNCTION_SPECIALIZATION(_LINK, RTYPE, ...) \
namespace ang ANG_BEGIN() namespace core ANG_BEGIN() namespace delegates ANG_BEGIN() \
template<> ANG_BEGIN_INTERFACE(_LINK, ifunction<RTYPE(__VA_ARGS__)>) \
	visible vcall RTYPE invoke(__VA_ARGS__)const pure; \
		visible vcall ifunction* clone()const pure;

#define ANG_END_IFUNCTION_SPECIALIZATION() ANG_END_INTERFACE() ANG_END() ANG_END() ANG_END()

namespace ang
{
	namespace core
	{
		namespace delegates
		{
			template<typename return_t, typename... args_t>
			class function_data<return_t(args_t...)>
				: public object
				, public ifunction<return_t(args_t...)>
			{
			public:
				function_data() {}
				ANG_DECLARE_INTERFACE();

			protected:
				virtual~function_data(){}
			};

			template<typename... args_t>
			class function_data<void(args_t...)>
				: public object
				, public ifunction<void(args_t...)>
			{
			public:
				function_data() {}
				ANG_DECLARE_INTERFACE();

			protected:
				virtual~function_data() {}
			};

			template<typename calleable_t, typename return_t, typename... args_t>
			class static_function final
				: public function_data<return_t(args_t...)>
			{
			public:
				typedef calleable_t function_type;

			private:
				function_type _function;

			public:
				inline static_function(function_type f) : _function(f) {
				}
				
				inline return_t invoke(args_t... args)const override {
					return _function(ang::move(args)...);
				}

				inline ifunction<return_t(args_t...)>* clone()const override {
					return new static_function(_function);
				}

			private:
				inline virtual~static_function() {}
			};

			template<typename obj_t, bool IS_OBJECT, typename return_t, typename... args_t>
			class member_function final
				: public function_data<return_t(args_t...)>
			{
			public:
				typedef return_t(obj_t::*function_type)(args_t...);

			private:
				obj_t* _obj;
				function_type _function;

			public:
				inline member_function(obj_t* o, function_type const& f) : _obj(o), _function(f) {}

				inline return_t invoke(args_t... args)const override {
					return (_obj->*_function)(ang::move(args)...);
				}
				inline ifunction<return_t(args_t...)>* clone()const override {
					return new member_function(_obj, _function);
				}

			private:
				inline virtual~member_function() {}
			};

			template<typename obj_t, typename return_t, typename... args_t>
			class member_function<obj_t, true, return_t, args_t...> final
				: public function_data<return_t(args_t...)>
			{
			public:
				typedef return_t(obj_t::*function_type)(args_t...);

			private:
				mutable weak_ptr<obj_t> _obj;
				function_type _function;

			public:
				inline member_function(obj_t* o, function_type const& f)
					: _obj(o), _function(f) {
				}

				inline return_t invoke(args_t... args)const override {
					object_wrapper<obj_t> lock = _obj.lock();
					return lock.is_empty()? return_t() : (lock->*_function)(ang::move(args)...);
				}
				inline ifunction<return_t(args_t...)>* clone()const override {
					return new member_function(_obj.lock(), _function);
				}

			private:
				inline virtual~member_function() {}
			};

			template<typename obj_t, typename return_t, typename... args_t>
			class pseudo_member_function final
				: public function_data<return_t(args_t...)>
			{
			public:
				typedef return_t(*function_type)(obj_t*, args_t...);

			private:
				obj_t* _obj;
				function_type _function;

			public:
				inline pseudo_member_function(obj_t* o, function_type const& f) : _obj(o), _function(f) {}

				inline return_t invoke(args_t... args)const override {
					return _function(_obj, ang::move(args)...);
				}
				inline ifunction<return_t(args_t...)>* clone()const override {
					return new pseudo_member_function(_obj, _function);
				}

			private:
				inline virtual~pseudo_member_function() {}
			};

		}
	}

	template<typename return_t, typename... args_t>
	class object_wrapper <core::delegates::function_data<return_t(args_t...)>>
	{
	public:
		typedef core::delegates::function_data<return_t(args_t...)> type;

	protected:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

		template<typename calleable_t>
		object_wrapper(calleable_t const& func) : object_wrapper() {
			set(new core::delegates::static_function<calleable_t, return_t, args_t...>(func));
		}

		template<typename obj_t>
		object_wrapper(obj_t* obj, return_t(obj_t::*f)(args_t...)) : object_wrapper() {
			set(new core::delegates::member_function<obj_t, is_base_of<object, obj_t>::value, return_t, args_t...>(obj, f));
		}	

		template<typename obj_t>
		object_wrapper(obj_t* obj, return_t(*f)(obj_t*,args_t...)) : object_wrapper() {
			set(new core::delegates::pseudo_member_function<obj_t, return_t, args_t...>(obj, f));
		}

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		return_t invoke(args_t ... args)const {
			if (is_empty())
				return return_t();
			return get()->invoke(ang::move(args)...);
		}

	public:
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (std::nullptr_t const&) {
			clean();
			return*this;
		}

		operator objptr()const {
			return _ptr;
		}

		template<typename calleable_t>
		object_wrapper& operator = (calleable_t func)
		{
			set(new core::delegates::static_function<calleable_t, return_t, args_t...>(func));
			return*this;
		}

		object_wrapper_ptr<type> operator & (void);

		return_t operator()(args_t ... args)const {
			return invoke(ang::move(args)...);
		}

		friend safe_pointer;
	};

	template<typename... args_t>
	class object_wrapper <core::delegates::function_data<void(args_t...)>>
	{
	public:
		typedef core::delegates::function_data<void(args_t...)> type;

	protected:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

		object_wrapper(core::delegates::function_data<void(args_t...)> * func) : object_wrapper() {
			set(func);
		}


		template<typename calleable_t>
		object_wrapper(calleable_t const& func) : object_wrapper() {
			set(new core::delegates::static_function<calleable_t, void, args_t...>(func));
		}

		template<typename obj_t>
		object_wrapper(obj_t* obj, void(obj_t::*f)(args_t...)) : object_wrapper() {
			set(new core::delegates::member_function<obj_t, is_base_of<object, obj_t>::value, void, args_t...>(obj, f));
		}

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		void invoke(args_t ... args)const {
			if (is_empty())
				return;
			return get()->invoke(ang::move(args)...);
		}

	public:
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (std::nullptr_t const&) {
			clean();
			return*this;
		}

		operator objptr()const {
			return _ptr;
		}

		template<typename calleable_t>
		object_wrapper& operator = (calleable_t func)
		{
			set(new core::delegates::static_function<calleable_t, void, args_t...>(func));
			return*this;
		}

		object_wrapper_ptr<type> operator & (void);

		void operator()(args_t ... args)const {
			invoke(ang::move(args)...);
		}

		friend safe_pointer;
	};

	template<> class LINK object_wrapper<core::delegates::function_data<void(void)>>
	{
	public:
		typedef core::delegates::function_data<void(void)> type;

	protected:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(std::nullptr_t const&);
		~object_wrapper();

		template<typename calleable_t>
		object_wrapper(calleable_t const& func) : object_wrapper() {
			set(new core::delegates::static_function<calleable_t, void>(func));
		}

		template<typename obj_t>
		object_wrapper(obj_t* obj, void(obj_t::*f)(void)) : object_wrapper() {
			set(new core::delegates::member_function<obj_t, is_base_of<object, obj_t>::value, void>(obj, f));
		}

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		void invoke(void)const {
			if (is_empty())
				return;
			return get()->invoke();
		}

	public:
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (std::nullptr_t const&) {
			clean();
			return*this;
		}

		operator objptr()const {
			return _ptr;
		}

		template<typename calleable_t>
		object_wrapper& operator = (calleable_t func) {
			set(new core::delegates::static_function<calleable_t, void, void>(func));
			return*this;
		}

		object_wrapper_ptr<type> operator & (void);

		void operator()(void)const {
			invoke();
		}

		friend safe_pointer;
	};

	template<typename O, typename T, typename ...Ts>
	inline core::delegates::function<T(Ts...)> bind(O*o, T(O::*func)(Ts...)) {
		return ang::move(core::delegates::function<T(Ts...)>(o, func));
	}

	template<typename O, typename T, typename ...Ts>
	inline core::delegates::function<T(Ts...)> bind(object_wrapper<O> o, T(O::*func)(Ts...)) {
		return ang::move(core::delegates::function<T(Ts...)>(o.get(), func));
	}

	namespace core
	{
		namespace delegates
		{
			template<typename return_t, typename... args_t>
			class listener <return_t(args_t...)>
			{
			public:
				typedef function<return_t(args_t...)> type;

			protected:
				collections::vector<function<return_t(args_t...)>> functions;

			public:
				inline listener();
				inline listener(listener &&);
				inline listener(listener const&);
				inline ~listener();

			public:
				inline void empty();
				inline bool is_empty()const;
				inline index append(function<return_t(args_t...)> func);

			public:
				inline listener& operator = (listener &&);
				inline listener& operator = (listener const&);
				inline listener& operator += (function<return_t(args_t...)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					append(function<return_t(args_t...)>(func));
					return*this;
				}
				inline collections::ienum_t<return_t> invoke(args_t... args)const;
				inline collections::ienum_t<return_t> operator()(args_t... args)const;
			};

			template<typename... args_t>
			class listener <void(args_t...)>
			{
			public:
				typedef function<void(args_t...)> type;

			protected:
				collections::vector<function<void(args_t...)>> functions;

			public:
				inline listener();
				inline listener(listener &&);
				inline listener(listener const&);
				inline ~listener();

			public:
				inline void empty();
				inline bool is_empty()const;
				inline index append(function<void(args_t...)> func);

			public:
				inline listener& operator = (listener &&);
				inline listener& operator = (listener const&);
				inline listener& operator += (function<void(args_t...)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					append(function<void(args_t...)>(func));
					return*this;
				}

				inline void invoke(args_t... args)const;
				inline void operator()(args_t ... args)const;
			};

		}
	}

	template<typename return_t, typename... args_t>
	class intf_wrapper<core::delegates::ifunction<return_t(args_t...)>>
	{
	public:
		typedef core::delegates::ifunction<return_t(args_t...)> type;

	protected:
		type* _ptr;

	public:
		intf_wrapper();
		intf_wrapper(type*);
		intf_wrapper(ang::nullptr_t const&);
		intf_wrapper(intf_wrapper &&);
		intf_wrapper(intf_wrapper const&);
		~intf_wrapper();

	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		return_t invoke(args_t ... args)const {
			if (is_empty())
				return return_t();
			return get()->invoke(ang::move(args)...);
		}

	public:
		intf_wrapper& operator = (type*);
		intf_wrapper& operator = (ang::nullptr_t const&);
		intf_wrapper& operator = (intf_wrapper &&);
		intf_wrapper& operator = (intf_wrapper const&);

		intf_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;

		return_t operator() (args_t ... args)const {
			if (is_empty())
				return return_t();
			return get()->invoke(ang::move(args)...);
		}
	};
}

ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::core::delegates::function<dword(pointer)>, ang::memory::default_allocator);
ANG_VECTOR_VALUE_SPECIALIZATION_DECLARATION(LINK, ang::core::delegates::function<void(ang::objptr, pointer)>, ang::memory::default_allocator);

namespace ang
{
	namespace core
	{
		namespace delegates
		{
			template<> class LINK listener <dword(pointer)>
			{
			public:
				typedef function<dword(pointer)> type;

			protected:
				collections::vector<function<dword(pointer)>> functions;

			public:
				listener();
				listener(listener &&);
				listener(listener const&);
				~listener();

			public:
				void empty();
				bool is_empty()const;
				index append(function<dword(pointer)> func);

			public:
				listener& operator = (listener &&);
				listener& operator = (listener const&);
				listener& operator += (function<dword(pointer)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					append(function<dword(pointer)>(func));
					return*this;
				}

				array<dword> invoke(pointer)const;
				array<dword> operator()(pointer)const;
			};

			template<> class LINK listener <void(objptr, pointer)>
			{
			public:
				typedef function<void(objptr, pointer)> type;

			protected:
				collections::vector<function<void(objptr, pointer)>> functions;

			public:
				listener();
				listener(listener &&);
				listener(listener const&);
				~listener();

			public:
				void empty();
				bool is_empty()const;
				index append(function<void(objptr, pointer)> func);

			public:
				listener& operator = (listener &&);
				listener& operator = (listener const&);
				listener& operator += (function<void(objptr, pointer)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					append(function<void(objptr, pointer)>(func));
					return*this;
				}

				void invoke(objptr, pointer)const;
				void operator()(objptr, pointer)const;
			};
		}
	}
}

#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/core/inlines/delegates.inl>

#endif//__ANG_DELEGATES_HPP__