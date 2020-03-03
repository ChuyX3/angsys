#ifndef __ANGSYS_H__
#elif !defined __CORE_FUNCTION_H__
#define __CORE_FUNCTION_H__

namespace ang
{
	namespace core
	{
		namespace delegates
		{
			template<typename T, typename... Ts>
			ang_begin_interface_inline(ifunction<T(Ts...)>)
				visible vcall T invoke(Ts...)const pure
				visible vcall ifunction* clone()const pure
			ang_end_interface();

			template<typename... Ts>
			ang_begin_interface_inline(ifunction<void(Ts...)>)
				visible vcall void invoke(Ts...)const pure
				visible vcall ifunction* clone()const pure
			ang_end_interface();

			template<typename T, typename... Ts>
			class function_object<T(Ts...)>
				: public smart<function_object<T(Ts...)>, ifunction<T(Ts...)>>
			{
			public:
				function_object() {}
				ANG_DECLARE_INTERFACE();

			protected:
				virtual~function_object() {}
			};

			template<typename... Ts>
			class function_object<void(Ts...)>
				: public smart<function_object<void(Ts...)>, ifunction<void(Ts...)>>
			{
			public:
				function_object() {}
				ANG_DECLARE_INTERFACE();

			protected:
				virtual~function_object() {}
			};

			template<typename F, typename T, typename... Ts>
			class static_function final
				: public function_object<T(Ts...)>
			{
			public:
				typedef F function_type;

			private:
				function_type _function;

			public:
				inline static_function(function_type f)
					: _function(f) {
				}

				inline T invoke(Ts... args)const override {
					return _function(ang::forward<Ts>(args)...);
				}

				inline ifunction<T(Ts...)>* clone()const override {
					return new static_function(_function);
				}
		
			private:
				inline void dispose()override {
					//_function = null;
				}
				inline virtual~static_function() {
				}
			};

			template<typename O, bool IS_INTERFACE, typename T, typename... Ts>
			class member_function final
				: public function_object<T(Ts...)>
			{
			public:
				typedef T(O::*function_type)(Ts...);

			private:
				O* _obj;
				function_type _function;

			public:
				//inline member_function(O* o, function_type const& f) : _obj(o), _function(f) {}
				template<typename D> inline member_function(D* o, function_type const& f) : _obj(o), _function(f) {}

				inline T invoke(Ts... args)const override {
					return (_obj->*_function)(ang::forward<Ts>(args)...);
				}
				inline ifunction<T(Ts...)>* clone()const override {
					return new member_function(_obj, _function);
				}

			private:
				inline void dispose()override {
					_obj = null;
					_function = null;
				}
				inline virtual~member_function() {}
			};

			template<typename O, typename T, typename... Ts>
			class member_function<O, true, T, Ts...> final
				: public function_object<T(Ts...)>
			{
			public:
				typedef T(O::*function_type)(Ts...);

			private:
				mutable weak_ptr<O> _obj;
				function_type _function;

			public:
				/*inline member_function(O* o, function_type const& f)
					: _obj(o), _function(f) {
				}*/
				template<typename D> inline member_function(D* o, function_type const& f)
					: _obj(o), _function(f) {
				}

				inline T invoke(Ts... args)const override {
					object_wrapper<O> lock = _obj.lock();
					return lock.is_empty() ? T() : (lock->*_function)(ang::forward<Ts>(args)...);
				}
				inline ifunction<T(Ts...)>* clone()const override {
					object_wrapper<O> lock = _obj.lock();
					return new member_function(lock.get(), _function);
				}

			private:
				inline void dispose()override {
					_obj = null;
					_function = null;
				}
				inline virtual~member_function() {}
			};

			template<typename O, bool IS_INTERFACE, typename T, typename... Ts>
			class pseudo_member_function final
				: public function_object<T(Ts...)>
			{
			public:
				typedef T(*function_type)(O*, Ts...);

			private:
				O* _obj;
				function_type _function;

			public:
				inline pseudo_member_function(O* o, function_type const& f) : _obj(o), _function(f) {}

				inline T invoke(Ts... args)const override {
					return _function(_obj, ang::forward<Ts>(args)...);
				}
				inline ifunction<T(Ts...)>* clone()const override {
					return new pseudo_member_function(_obj, _function);
				}

			private:
				inline void dispose()override {
					_obj = null;
					_function = null;
				}
				inline virtual~pseudo_member_function() {}
			};

			template<typename O, typename T, typename... Ts>
			class pseudo_member_function<O, true, T, Ts...> final
				: public function_object<T(Ts...)>
			{
			public:
				typedef T(*function_type)(O*, Ts...);

			private:
				mutable weak_ptr<O> _obj;
				function_type _function;

			public:
				inline pseudo_member_function(O* o, function_type const& f) : _obj(o), _function(f) {}

				inline T invoke(Ts... args)const override {
					return _function(_obj.lock(), ang::forward<Ts>(args)...);
				}
				inline ifunction<T(Ts...)>* clone()const override {
					return new pseudo_member_function(_obj.lock(), _function);
				}

			private:
				inline void dispose()override {
					_obj = null;
					_function = null;
				}
				inline virtual~pseudo_member_function() {}
			};
		}

	}

	template<typename T, typename... Ts>
	class object_wrapper <core::delegates::function_object<T(Ts...)>>
	{
	public:
		typedef core::delegates::function_object<T(Ts...)> type;

	protected:
		type* m_ptr;

	public:
		object_wrapper();
		object_wrapper(core::delegates::function_object<T(Ts...)>*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();

		template<typename F>
		object_wrapper(F const& func) : object_wrapper() {
			static_assert(is_calleable<F>::value, "F is not a calleable object");
			set(new core::delegates::static_function<F, T, Ts...>(func));
		}

		/*template<typename O>
		object_wrapper(O* obj, T(O::*f)(Ts...)) : object_wrapper() {
			set(new core::delegates::member_function<O, is_base_of<intf, O>::value, T, Ts...>(obj, f));
		}*/

		template<typename O, typename D>
		object_wrapper(D* obj, T(O::*f)(Ts...)) : object_wrapper() {
			set(new core::delegates::member_function<O, is_base_of<intf, O>::value, T, Ts...>(obj, f));
		}

		template<typename O>
		object_wrapper(O* obj, T(*f)(O*, Ts...)) : object_wrapper() {
			set(new core::delegates::pseudo_member_function<O, is_base_of<intf, O>::value, T, Ts...>(obj, f));
		}

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

		T invoke(Ts ... args)const {
			if (is_empty())
				return T();
			return get()->invoke(ang::forward<Ts>(args)...);
		}

		template<typename F>
		void se(F const& func) {
			static_assert(is_calleable<F>::value, "F is not a calleable object");
			set((type*)new core::delegates::static_function<F, T, Ts...>(func));
		}

		template<typename O>
		void set(O* obj, T(O::*f)(Ts...)) {
			set((type*)new core::delegates::member_function<O, is_base_of<intf, O>::value, T, Ts...>(obj, f));
		}

		template<typename O>
		void set(O* obj, T(*f)(O*, Ts...)) {
			set((type*)new core::delegates::pseudo_member_function<O, is_base_of<intf, O>::value, T, Ts...>(obj, f));
		}

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (ang::nullptr_t const&) {
			reset();
			return*this;
		}

		operator objptr()const {
			return m_ptr;
		}

		template<typename F>
		object_wrapper& operator = (F const& func)
		{
			static_assert(is_calleable<F>::value, "F is not a calleable object");
			set(new core::delegates::static_function<F, T, Ts...>(func));
			return*this;
		}

		object_wrapper_ptr<type> operator & (void);

		T operator()(Ts ... args)const {
			return invoke(ang::forward<Ts>(args)...);
		}

	};

	template<typename... Ts>
	class object_wrapper <core::delegates::function_object<void(Ts...)>>
	{
	public:
		typedef core::delegates::function_object<void(Ts...)> type;

	protected:
		type* m_ptr;

	public:
		object_wrapper();
		object_wrapper(core::delegates::function_object<void(Ts...)> * func);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(ang::nullptr_t const&);
		~object_wrapper();


		template<typename F>
		object_wrapper(F const& func) : object_wrapper() {
			set(new core::delegates::static_function<F, void, Ts...>(func));
		}

		template<typename O>
		object_wrapper(O* obj, void(O::*f)(Ts...)) : object_wrapper() {
			set(new core::delegates::member_function<O, is_base_of<object, O>::value, void, Ts...>(obj, f));
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

		operator objptr()const {
			return m_ptr;
		}

		template<typename F>
		object_wrapper& operator = (F func)
		{
			set(new core::delegates::static_function<F, void, Ts...>(func));
			return*this;
		}

		object_wrapper_ptr<type> operator & (void);

		void operator()(Ts ... args)const {
			invoke(ang::forward<Ts>(args)...);
		}

		friend safe_pointer;
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


	template<typename T> struct delegate_helper;

	template<typename T, typename...Ts>
	struct delegate_helper<T(Ts...)> {
		using args_type = collections::tuple<Ts...>;
		using ret_type = T;
		using base_class = base_property<T(Ts...)>;
		using function_type = T(*)(base_class const*, args_type&&);
	};

	template<typename T, typename...Ts>
	struct delegate_helper<T(*)(Ts...)> {
		using args_type = collections::tuple<Ts...>;
		using ret_type = T;
		using base_class = base_property<T(Ts...)>;
		using function_type = T(*)(base_class const*, args_type&&);
	};

	template<typename T>
	struct delegate_helper<T(void)> {
		using args_type = collections::tuple<void>;
		using ret_type = T;
		using base_class = base_property<T(void)>;
		using function_type = T(*)(base_class const*, args_type &&);
	};

	template<typename T>
	struct delegate_helper<T(*)(void)> {
		using args_type = collections::tuple<void>;
		using ret_type = T;
		using base_class = base_property<T(void)>;
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

#endif//__CORE_FUNCTION_H__