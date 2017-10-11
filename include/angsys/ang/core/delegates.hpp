/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_DELEGATES_HPP__
#define __ANG_DELEGATES_HPP__

#include <angsys.h>
#include<ang/collections/vector.hpp>

#ifdef  LINK
#undef  LINK
#endif//LINK

#if defined WINDOWS_PLATFORM
#if defined ANGSYS_DYNAMIC_LIBRARY

#ifdef ANGSYS_EXPORTS
#define LINK __declspec(dllexport)
#else
#define LINK __declspec(dllimport)
#endif//ANGSYS_EXPORTS
#else//#elif defined ANGSYS_STATIC_LIBRARY
#define LINK
#endif//ANGSYS_DYNAMIC_LIBRARY
#elif defined LINUX_PLATFORM || defined ANDROID_PLATFORM
#define LINK
#endif

#define DEFINE_ARG_TYPE(_LINK, _NAME_, ...) typedef ang::object_wrapper<class _NAME_> _NAME_##_t; class _LINK _NAME_ : public ang::object { public: _NAME_(); ANG_DECLARE_INTERFACE(); DECLARE_VARSN(__VA_ARGS__); private: virtual~_NAME_(); }
#define DEFINE_CORE_EVENT(_LINK, _RET_TYPE, _NAME, ...)  DEFINE_ARG_TYPE(_LINK, _NAME##_event_args, __VA_ARGS__); \
		typedef ang::core::delegates::listener<_RET_TYPE(ang::objptr, _NAME##_event_args##_t)> _NAME##_event_handler, _NAME##_event_handler_t; \
		typedef ang::core::delegates::function_data<_RET_TYPE(ang::objptr, _NAME##_event_args##_t)> _NAME##_event; \
		typedef ang::core::delegates::function<_RET_TYPE(ang::objptr, _NAME##_event_args##_t)> _NAME##_event_t 

namespace ang
{
	namespace core
	{
		namespace delegates
		{
			template<typename T> class listener;
			template<typename T> class function_data;
			template<typename T> using function = object_wrapper<function_data<T>>;
			template<typename T> struct ifunction;
			template<typename T> using method = intf_wrapper<ifunction<T>>;

			using var_args_t = collections::vector<objptr>;
			using var_args = collections::vector_buffer<objptr>;
			
			template<typename return_t, typename... args_t>
			ANG_BEGIN_INLINE_INTERFACE(ifunction<return_t(args_t...)>)
				visible vcall return_t invoke(args_t...)const pure;
				visible vcall ifunction* clone()const pure;
			ANG_END_INTERFACE();

			template<typename... args_t>
			ANG_BEGIN_INLINE_INTERFACE(ifunction<void(args_t...)>)
				visible vcall void invoke(args_t...)const pure;
				visible vcall ifunction* clone()const pure;
			ANG_END_INTERFACE();

			template<typename return_t, typename... args_t> 
			using function_cast = return_t(*)(args_t...);

			template<typename return_t, typename... args_t>
			using function_type = return_t(*)(args_t...);
			
		}
	}

	using ang::core::delegates::var_args;
	using ang::core::delegates::var_args_t;
	using ang::core::delegates::function_cast;
	using ang::core::delegates::function_type;
}

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

		operator object_t()const {
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

		template<typename calleable_t>
		object_wrapper(calleable_t const& func) : object_wrapper() {
			set(new core::delegates::static_function<calleable_t, void, args_t...>(func));
		}

		template<typename obj_t>
		object_wrapper(obj_t* obj, void(obj_t::*f)(args_t...)) : object_wrapper() {
			set(new core::delegates::member_function<obj_t, void, args_t...>(obj, f));
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

		operator object_t()const {
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
			set(new core::delegates::member_function<obj_t, void>(obj, f));
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

		operator object_t()const {
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

//ANG_DECLARE_OBJECT_VECTOR_SPECIALIZATION(LINK, ang::core::delegates::function_data<dword(pointer)>);
//ANG_DECLARE_OBJECT_VECTOR_SPECIALIZATION(LINK, ang::core::delegates::function_data<void(ang::objptr, pointer)>);

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