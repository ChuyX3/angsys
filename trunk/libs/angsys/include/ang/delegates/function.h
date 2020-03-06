/*********************************************************************************************************************/
/*   File Name: ang/function.h                                                                                 */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, 2019.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                              */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANGSYS_H__
#error ang.h is not included
#elif !defined __ANG_FUNCTION_H__
#define __ANG_FUNCTION_H__

namespace ang
{
	template<typename T, typename...Ts>
	struct smart_ptr_type<T(Ts...), false, false> {
		static constexpr ang::smart_type smart_type = ang::smart_type::none;
		using smart_ptr_t = function<T(Ts...)>;
		using type = delegates::function_base<T(Ts...)>;
	};

	namespace delegates
	{
		template<typename T, typename... Ts>
		struct nvt ifunction<T(Ts...)>
			: intf<ifunction<T(Ts...)>
			, iid("ang::delegates::ifunction")>
		{
			virtual T invoke(Ts...)const = 0;
			virtual ifunction* clone()const = 0;
		};

		template<typename T, typename... Ts>
		class function_base<T(Ts...)>
			: public implement<function_base<T(Ts...)>
			, iid("ang::delegates::function")
			, ifunction<T(Ts...)>>
		{
		public:
			function_base() {}

		protected:
			virtual~function_base() {}
		};

		template<typename F, typename T, typename... Ts>
		class static_function final
			: public function_base<T(Ts...)>
		{
		public:
			typedef F function_type;

		private:
			function_type m_functor;

		public:
			template<typename F2>
			static_function(F2&& f)
				: m_functor(ang::forward<F2>(f)) {
			}
			T invoke(Ts... args)const override {
				return m_functor(ang::forward<Ts>(args)...);
			}
			ifunction<T(Ts...)>* clone()const override {
				return new static_function(m_functor);
			}

		private:
			void dispose()override {
				//m_functor = null;
			}
			virtual~static_function() {
			}
		};

		template<typename O, bool IS_INTERFACE, typename T, typename... Ts>
		class member_function final
			: public function_base<T(Ts...)>
		{
		public:
			typedef O *object_type;
			typedef T(O::*function_type)(Ts...);

		private:
			object_type m_object;
			function_type m_member;

		public:
			template<typename D>
			member_function(D* o, function_type const& f) 
				: m_object(o)
				, m_member(f) {
			}
			T invoke(Ts... args)const override {
				return (m_object->*m_member)(ang::forward<Ts>(args)...);
			}
			ifunction<T(Ts...)>* clone()const override {
				return new member_function(m_object, m_member);
			}

		private:
			void dispose()override {
				m_object = null;
				m_member = null;
			}
			virtual~member_function() {
			}
		};

		template<typename O, typename T, typename... Ts>
		class member_function<O, true, T, Ts...> final
			: public function_base<T(Ts...)>
		{
		public:
			typedef O *object_type;
			typedef T(O::*function_type)(Ts...);

		private:
			mutable weak_ptr<O> m_object;
			function_type m_member;

		public:
			template<typename D> 
			member_function(D* o, function_type const& f)
				: m_object(o)
				, m_member(f) {
			}

			T invoke(Ts... args)const override {
				object_wrapper<O> lock = m_object.lock();
				return lock.is_empty() ? T() : (lock->*m_member)(ang::forward<Ts>(args)...);
			}
			ifunction<T(Ts...)>* clone()const override {
				object_wrapper<O> lock = m_object.lock();
				return new member_function(lock.get(), m_member);
			}

		private:
			void dispose()override {
				m_object = null;
				m_member = null;
			}
			virtual~member_function() {
			}
		};

		template<typename O, bool IS_INTERFACE, typename T, typename... Ts>
		class pseudo_member_function final
			: public function_base<T(Ts...)>
		{
		public:
			typedef O *object_type;
			typedef T(*function_type)(O*, Ts...);

		private:
			O* m_object;
			function_type m_member;

		public:
			pseudo_member_function(O* o, function_type const& f)
				: m_object(o)
				, m_member(f) {
			}

			T invoke(Ts... args)const override {
				return m_member(m_object, ang::forward<Ts>(args)...);
			}
			ifunction<T(Ts...)>* clone()const override {
				return new pseudo_member_function(m_object, m_member);
			}

		private:
			inline void dispose()override {
				m_object = null;
				m_member = null;
			}
			inline virtual~pseudo_member_function() {
			}
		};

		template<typename O, typename T, typename... Ts>
		class pseudo_member_function<O, true, T, Ts...> final
			: public function_base<T(Ts...)>
		{
		public:
			typedef O *object_type;
			typedef T(*function_type)(O*, Ts...);

		private:
			mutable weak_ptr<O> m_object;
			function_type m_member;

		public:
			pseudo_member_function(O* o, function_type const& f)
				: m_object(o)
				, m_member(f) {
			}

			T invoke(Ts... args)const override {
				return m_member(m_object.lock(), ang::forward<Ts>(args)...);
			}
			ifunction<T(Ts...)>* clone()const override {
				return new pseudo_member_function(m_object.lock(), m_member);
			}

		private:
			void dispose()override {
				m_object = null;
				m_member = null;
			}
			virtual~pseudo_member_function() {
			}
		};
	}

	template<typename T, typename... Ts>
	class object_wrapper <delegates::function_base<T(Ts...)>>
	{
	public:
		typedef delegates::function_base<T(Ts...)> type;

	protected:
		type* m_ptr;

	public:
		object_wrapper();
		object_wrapper(delegates::function_base<T(Ts...)>*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

		template<typename F>
		object_wrapper(F const& func) : object_wrapper() {
			static_assert(is_calleable<F>::value, "F is not a calleable object");
			set(new delegates::static_function<F, T, Ts...>(func));
		}

		template<typename O, typename D>
		object_wrapper(D* obj, T(O::*f)(Ts...)) : object_wrapper() {
			set(new delegates::member_function<O, is_base_of<iintf, O>::value, T, Ts...>(obj, f));
		}

		template<typename O>
		object_wrapper(O* obj, T(*f)(O*, Ts...)) : object_wrapper() {
			set(new delegates::pseudo_member_function<O, is_base_of<iintf, O>::value, T, Ts...>(obj, f));
		}

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		T invoke(Ts&&... args)const {
			if (is_empty())
				return T();
			return get()->invoke(ang::forward<Ts>(args)...);
		}

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (ang::nullptr_t const&) {
			reset();
			return*this;
		}

		operator object_t()const {
			return m_ptr;
		}

		template<typename F>
		object_wrapper& operator = (F const& func)
		{
			static_assert(is_calleable<F>::value, "F is not a calleable object");
			set(new delegates::static_function<F, T, Ts...>(func));
			return*this;
		}

		object_wrapper_ptr<type> operator & (void);

		T operator()(Ts ... args)const {
			return invoke(ang::forward<Ts>(args)...);
		}

	};

	template<typename... Ts>
	class object_wrapper <delegates::function_base<void(Ts...)>>
	{
	public:
		typedef delegates::function_base<void(Ts...)> type;

	protected:
		type* m_ptr;

	public:
		object_wrapper();
		object_wrapper(delegates::function_base<void(Ts...)> * func);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();


		template<typename F>
		object_wrapper(F const& func) : object_wrapper() {
			set(new delegates::static_function<F, void, Ts...>(func));
		}

		template<typename O>
		object_wrapper(O* obj, void(O::*f)(Ts...)) : object_wrapper() {
			set(new delegates::member_function<O, is_base_of<object, O>::value, void, Ts...>(obj, f));
		}

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		void invoke(Ts ... args)const {
			if (is_empty())
				return;
			return get()->invoke(ang::forward<Ts>(args)...);
		}

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper& operator = (ang::nullptr_t const&) {
			reset();
			return*this;
		}

		operator object_t()const {
			return m_ptr;
		}

		template<typename F>
		object_wrapper& operator = (F func)
		{
			set(new delegates::static_function<F, void, Ts...>(func));
			return*this;
		}

		object_wrapper_ptr<type> operator & (void);

		void operator()(Ts ... args)const {
			invoke(ang::forward<Ts>(args)...);
		}

		friend weak_ptr_base;
	};


	template<typename O, typename D, typename T, typename...Ts>
	inline function<T(Ts...)> bind(D* o, T(O::*f)(Ts...)) {
		return function<T(Ts...)>(o, f);
	}

	template<typename O, typename D, typename T, typename...Ts>
	inline function<T(Ts...)> bind(object_wrapper<D> o, T(O::*f)(Ts...)) {
		return function<T(Ts...)>(o.get(), f);
	}

	template<typename O, typename D, typename T, typename...Ts>
	inline function<T(Ts...)> bind(intf_wrapper<D> o, T(O::*f)(Ts...)) {
		return function<T(Ts...)>(o.get(), f);
	}


	namespace delegates
	{

		template<typename T> struct delegate_helper;

		template<typename T, typename...Ts>
		struct delegate_helper<T(Ts...)> {
			using args_type = collections::tuple<Ts...>;
			using ret_type = T;
			using base_class = base_property;
			using function_type = T(*)(base_class const*, args_type&&);
		};

		template<typename T, typename...Ts>
		struct delegate_helper<T(*)(Ts...)> {
			using args_type = collections::tuple<Ts...>;
			using ret_type = T;
			using base_class = base_property;
			using function_type = T(*)(base_class const*, args_type&&);
		};

		template<typename T>
		struct delegate_helper<T(void)> {
			using args_type = collections::tuple<void>;
			using ret_type = T;
			using base_class = base_property;
			using function_type = T(*)(base_class const*, args_type &&);
		};

		template<typename T>
		struct delegate_helper<T(*)(void)> {
			using args_type = collections::tuple<void>;
			using ret_type = T;
			using base_class = base_property;
			using function_type = T(*)(base_class const*, args_type &&);
		};

		template<typename T, typename delegate_helper<T>::function_type callback> struct delegate;

		template<typename T, typename...Ts, typename delegate_helper<T(Ts...)>::function_type callback>
		struct delegate<T(Ts...), callback> : delegate_helper<T(Ts...)>::base_class {
			auto operator()(Ts... args)const {
				return callback(this, ang::forward<typename delegate_helper<T(Ts...)>::args_type>(pack_args(ang::forward<Ts>(args)...)));
			}
		};

		template<typename T, typename...Ts, typename delegate_helper<T(*)(Ts...)>::function_type callback>
		struct delegate<T(*)(Ts...), callback> : delegate_helper<T(Ts...)>::base_class {
			auto operator()(Ts... args)const {
				return callback(this, ang::forward<typename delegate_helper<T(Ts...)>::args_type>(pack_args(ang::forward<Ts>(args)...)));
			}
		};

		template<typename T, typename delegate_helper<T(void)>::function_type callback>
		struct delegate<T(void), callback> : delegate_helper<T(void)>::base_class {
			auto operator()(void)const {
				return callback(this, collections::tuple<void>());
			}
		};

		template<typename T, typename delegate_helper<T(*)(void)>::function_type callback>
		struct delegate<T(*)(void), callback> : delegate_helper<T(void)>::base_class {
			auto operator()(void)const {
				return callback(this, collections::tuple<void>());
			}
		};
	}

	using delegates::delegate;
}


#endif//__ANG_BASE_STRING_H__
