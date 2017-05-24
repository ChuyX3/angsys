/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_DELEGATES_H__
#define __ANG_DELEGATES_H__

#include <angsys.h>
#include<ang/collections/vector.h>

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

namespace ang
{
	namespace core
	{
		namespace delegates
		{
			template<typename T> class listener;
			template<typename T> class function_data;
			template<typename T> using function = object_wrapper<function_data<T>>;

			using var_args_t = collections::vector<objptr>;
			using var_args = collections::vector_buffer<objptr>;
			
			template<typename return_t, typename... args_t>
			ANG_BEGIN_INLINE_INTERFACE(ifunction)
				visible vcall return_t invoke(args_t...)const pure;
				visible vcall ifunction* clone()const pure;
			ANG_END_INTERFACE();

			template<typename... args_t>
			ANG_BEGIN_INTERFACE_SPECIALIZATION(ifunction, void, args_t...)
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
				, public ifunction<return_t, args_t...>
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
				, public ifunction<void, args_t...>
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

				inline ifunction<return_t, args_t...>* clone()const override {
					return new static_function(_function);
				}

			private:
				inline virtual~static_function() {}
			};

			template<typename obj_t, typename return_t, typename... args_t>
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
				inline ifunction<return_t, args_t...>* clone()const override {
					return new member_function(_obj, _function);
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
				inline ifunction<return_t, args_t...>* clone()const override {
					return new pseudo_member_function(_obj, _function);
				}

			private:
				inline virtual~pseudo_member_function() {}
			};


			template<typename obj_t, typename return_t, typename... args_t>
			class object_member_function final
				: public function_data<return_t(args_t...)>
			{
			public:
				typedef return_t(obj_t::*function_type)(args_t...);

			private:
				mutable safe_pointer _obj;
				function_type _function;

			public:
				inline object_member_function(object_t o, function_type const& f)
					: _obj(o.get()), _function(f) {
				}

				inline return_t invoke(args_t... args)const override {
					object_wrapper<obj_t> lock = _obj.lock<obj_t>();
					return (lock->*_function)(ang::move(args)...);
				}
				inline ifunction<return_t, args_t...>* clone()const override {
					return new object_member_function(_obj.lock<object>(), _function);
				}

			private:
				inline virtual~object_member_function() {}
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
			set(new core::delegates::member_function<obj_t, return_t, args_t...>(obj, f));
		}	

		template<typename obj_t>
		object_wrapper(obj_t* obj, return_t(*f)(obj_t*,args_t...)) : object_wrapper() {
			set(new core::delegates::pseudo_member_function<obj_t, return_t, args_t...>(obj, f));
		}

	public:
		void clean();
		void clean_unsafe();
		bool_t is_empty()const;
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
		bool_t is_empty()const;
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
		bool_t is_empty()const;
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
				inline bool_t is_empty()const;
				inline index_t append(function<return_t(args_t...)> func);

			public:
				inline listener& operator = (listener &&);
				inline listener& operator = (listener const&);
				inline listener& operator += (function<return_t(args_t...)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					append(function<return_t(args_t...)>(func));
					return*this;
				}
				inline array<return_t> invoke(args_t... args)const;
				inline array<return_t> operator()(args_t... args)const;
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
				inline bool_t is_empty()const;
				inline index_t append(function<void(args_t...)> func);

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


}


#ifdef  LINK
#undef  LINK
#endif//LINK

#include<ang/core/inlines/delegates.hpp>

#endif//__ANG_DELEGATES_H__