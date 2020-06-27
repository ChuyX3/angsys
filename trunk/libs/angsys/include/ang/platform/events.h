/*********************************************************************************************************************/
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*********************************************************************************************************************/

#ifndef __ANG_PLATFORM_H__
#error ...
#elif !defined __ANG_PLATFORM_EVENTS_H__
#define __ANG_PLATFORM_EVENTS_H__

namespace ang
{
	namespace platform
	{
		namespace events
		{
			using ievent_function = delegates::ifunction<void(objptr, imsg_event_args_t)>;
			using ievent_function_t = delegates::method<void(objptr, imsg_event_args_t)>;

			template<typename F, typename A>
			class static_event_function
				: public implement<static_event_function<F, A>
				, iid("ang::platform::events::static_event_function")
				, ievent_function>
			{
			public:
				typedef F function_type;

			private:
				function_type m_function;

			public:
				inline static_event_function(function_type f) : m_function(f) {}
				inline virtual~static_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override;
				inline ievent_function* clone()const override {
					return new static_event_function(m_function);
				}
			};

			template<typename F, typename A>
			class static_event_function<F, intf_wrapper<A>>
				: public implement<static_event_function<F, intf_wrapper<A>>
				, iid("ang::platform::events::static_event_function")
				, ievent_function>
			{
			public:
				typedef F function_type;

			private:
				function_type m_function;

			public:
				inline static_event_function(function_type f) :m_function(f) {}
				inline virtual~static_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override;
				inline ievent_function* clone()const override {
					return new static_event_function(m_function);
				}
			};

			template<typename T, bool IS_OBJECT, typename A>
			class member_event_function 
				: public implement<member_event_function<T, IS_OBJECT, A>
				, iid("ang::platform::events::member_event_function")
				, ievent_function>
			{
			public:
				typedef void(T::*function_type)(objptr, intf_wrapper<A>);

			private:
				T * m_obj;
				function_type m_function;

			public:
				inline member_event_function(T * o, function_type f) : m_obj(o), m_function(f) {}
				inline virtual~member_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override;
				inline ievent_function* clone()const override {
					return new member_event_function(m_obj, m_function);
				}
			};

			template<typename T, typename A>
			class member_event_function<T, true, A>
				: public implement<member_event_function<T, true, A>
				, iid("ang::platform::events::member_event_function")
				, ievent_function>
			{
			public:
				typedef void(T::*function_type)(objptr, intf_wrapper<A>);

			private:
				mutable weak_ptr<T> m_obj;
				function_type m_function;

			public:
				inline member_event_function(T* o, function_type f) : m_obj(o), m_function(f) {}
				inline member_event_function(typename smart_ptr_type<T>::smart_ptr_t o, function_type f) : m_obj(o), m_function(f) {}
				inline virtual~member_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override;
				inline ievent_function* clone()const override {
					return new member_event_function(m_obj.lock(), m_function);
				}
			};

		}
	}

	namespace delegates
	{
		template<>
		struct nvt LINK ifunction<void(object*, platform::events::imsg_event_args*)>
			: intf<ifunction<void(object*, platform::events::imsg_event_args*)>
			, iid("ang::delegates::ifunction")> {
			virtual void invoke(objptr, platform::events::imsg_event_args_t)const = 0;
			virtual ifunction* clone()const = 0;
		};

		template<> class LINK function_base <void(object*, platform::events::imsg_event_args*)>
			: public implement<function_base <void(object*, platform::events::imsg_event_args*)>
				, iid("ang::delegates::function_base")
				, ifunction<void(object*, platform::events::imsg_event_args*)>>
		{
		protected:
			platform::events::core_msg_t m_msg;
			platform::events::ievent_function_t m_function;

			function_base(platform::events::core_msg_t msg);
			
			template<typename A, typename F>
			inline void set(F const& f) {
				m_function = new platform::events::static_event_function<F, A>(f);
			}

			template<typename A, typename T>
			inline void set(T* obj, void(T::*f)(objptr, intf_wrapper<A>)) {
				m_function = new platform::events::member_event_function<T, is_base_of<iintf, T>::value, A>(obj, f);
			}

			template<typename A, typename T>
			inline void set(typename smart_ptr_type<T>::smart_ptr_t obj, void(T::*f)(objptr, intf_wrapper<A>)) {
				m_function = new platform::events::member_event_function<T, is_base_of<iintf, T>::value, A>(obj, f);
			}

		public: // Overrides
			platform::events::core_msg_t msg_type()const;
			virtual ifunction* clone()const override;

		protected:
			virtual void invoke(objptr, platform::events::imsg_event_args_t)const override;
			virtual~function_base();

			friend listener<void(object*, platform::events::imsg_event_args*)>;
			friend object_wrapper<function_base<void(object*, platform::events::imsg_event_args*)>>;
		};
	}

	template<> class LINK object_wrapper<platform::events::base_event_handler>
	{
	public:
		typedef platform::events::base_event_handler type;

	protected:
		platform::events::base_event_handler* m_ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(platform::events::base_event_handler*);
		~object_wrapper();

	public:
		void reset();
		void reset_unsafe();
		bool is_empty()const;
		platform::events::base_event_handler* get(void)const;
		void set(platform::events::base_event_handler*);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (platform::events::base_event_handler*);
		void operator () (objptr, platform::events::imsg_event_args_t)const;
		type* operator ->()const { return m_ptr; }
	};

}

//ANG_DECLARE_OBJECT_VECTOR_SPECIALIZATION(LINK, ang::platform::events::base_event_handler)

namespace ang
{
	
	namespace delegates
	{
		template<> class LINK listener<void(object*, platform::events::imsg_event_args*)>
		{
		public:
			typedef platform::events::event_t function;

		protected:
			mutable weak_ptr_base m_parent;
			collections::list<function> m_functions;

			listener(listener &&) = delete;
			listener(listener const&) = delete;

			listener& operator = (listener &&) = delete;
			listener& operator = (listener const&) = delete;

		public:
			listener(object* parent);
			~listener();

		public:
			bool is_empty()const;
			virtual platform::events::event_token_t push(platform::events::event_t);
			virtual bool pop(platform::events::event_token_t);

		public:
			void empty();
			virtual int invoke(platform::events::imsg_event_args_t)const;				
			int operator ()(platform::events::imsg_event_args_t)const;

		public:
			platform::events::event_token_t operator += (platform::events::event_t);
			bool operator -= (platform::events::event_token_t);

		};

		template<> struct event_helper<void(object*, platform::events::imsg_event_args*)> {
			using type = void(object*, platform::events::imsg_event_args*);
			using listen_token = listen_token<type>;
			using function_type = function_base<type>;
			using add_event_handler = listen_token(*)(base_event*, object_wrapper<function_type>);
			using remove_event_handler = bool(*)(base_event*, listen_token);
		};
	}
	

	namespace platform
	{
		namespace events
		{
			template<typename T, core_msg MSG >
			class event_handler final
				: public implement<event_handler<T, MSG>
				, base_event_handler>
			{
			public:
				template<class O>
				event_handler(O* o, void(O::*f)(objptr, intf_wrapper<T>))
					: implement<event_handler<T, MSG>, base_event_handler>((core_msg_t)MSG) {
					this->template set<T>(o, f);
				}
				template<class O>
				event_handler(typename smart_ptr_type<O>::smart_ptr_t o, void(O::*f)(objptr, intf_wrapper<T>))
					: implement<event_handler<T, MSG>, base_event_handler>((core_msg_t)MSG) {
					this->template set<T>(o, f);
				}
				template<class F>
				event_handler(F f)
					: implement<event_handler<T, MSG>, base_event_handler>((core_msg_t)MSG) {
					this->template set<T>(f);
				}
			};

			using delegates::base_event;

			template<typename T>
			struct event_helper
			{
				using type = typename remove_reference<typename remove_const<T>::type>::type;
				using type_ptr = type*;
				using arg_type = intf_wrapper<type>;
				using add_event_type = event_token_t(*)(base_event*, event_t);
				using remove_event_type = bool(*)(base_event*, event_token_t);
			};

			template<typename T, typename event_helper<T>::add_event_type add_event, typename event_helper<T>::remove_event_type remove_event>
			struct event final
				: public base_event
			{
				template<core_msg MSG> inline auto operator += (event_handler<T, MSG>* e) {
					return add_event(this, e);
				}
				template<core_msg MSG> inline auto operator += (object_wrapper<event_handler<T, MSG>> e) {
					return add_event(this, e.get());
				}
				inline auto operator -= (event_token_t token) {
					return remove_event(this, token);
				}
			};
		}
	}
}

#endif//__ANG_PLATFORM_EVENTS_H__
