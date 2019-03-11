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
			struct LINK ievent_function
			{
				visible vcall~ievent_function();
				visible vcall void invoke(objptr, imsg_event_args_t)const pure;
				visible vcall ievent_function* clone()const pure;

				pointer operator new(wsize);
				void operator delete(pointer);
			};

			template<typename F, typename A>
			class static_event_function
				: public ievent_function
			{
			public:
				typedef F function_type;

			private:
				function_type _function;

			public:
				inline static_event_function(function_type f) :_function(f) {}
				inline virtual~static_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override;
				inline ievent_function* clone()const override {
					return new static_event_function(_function);
				}
			};

			template<typename F, typename A>
			class static_event_function<F, intf_wrapper<A>>
				: public ievent_function
			{
			public:
				typedef F function_type;

			private:
				function_type _function;

			public:
				inline static_event_function(function_type f) :_function(f) {}
				inline virtual~static_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override;
				inline ievent_function* clone()const override {
					return new static_event_function(_function);
				}
			};

			template<typename T, bool IS_OBJECT, typename A>
			class member_event_function 
				: public ievent_function
			{
			public:
				typedef void(T::*function_type)(objptr, intf_wrapper<A>);

			private:
				T * _obj;
				function_type _function;

			public:
				inline member_event_function(T * o, function_type f) : _obj(o), _function(f) {}
				inline virtual~member_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override;
				inline ievent_function* clone()const override {
					return new member_event_function(_obj, _function);
				}
			};

			template<typename T, typename A>
			class member_event_function<T, true, A>
				: public ievent_function
			{
			public:
				typedef void(T::*function_type)(objptr, intf_wrapper<A>);

			private:
				mutable weak_ptr<T> _obj;
				function_type _function;

			public:
				inline member_event_function(T* o, function_type f) : _obj(o), _function(f) {}
				inline member_event_function(typename smart_ptr_type<T>::smart_ptr_t o, function_type f) : _obj(o), _function(f) {}
				inline virtual~member_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override;
				inline ievent_function* clone()const override {
					return new member_event_function(_obj.lock(), _function);
				}
			};

		}
	}

	namespace core
	{
		namespace delegates
		{
			template<>
			ang_begin_interface(LINK ifunction<void(object* COMA platform::events::imsg_event_args*)>)
				visible vcall void invoke(objptr, platform::events::imsg_event_args_t)const pure;
				visible vcall ifunction* clone()const pure;
			ang_end_interface();

			template<> class LINK function_object <void(object*, platform::events::imsg_event_args*)>
				: public smart<function_object <void(object*, platform::events::imsg_event_args*)>, ifunction<void(object*, platform::events::imsg_event_args*)>>
			{
			protected:
				platform::events::core_msg_t m_msg;
				platform::events::ievent_function* m_function;

				function_object(platform::events::core_msg_t msg);
			

				template<typename A, typename F>
				inline void set(F const& f) {
					m_function = new platform::events::static_event_function<F, A>(f);
				}

				template<typename A, typename T>
				inline void set(T* obj, void(T::*f)(objptr, intf_wrapper<A>)) {
					m_function = new platform::events::member_event_function<T, is_base_of<interface, T>::value, A>(obj, f);
				}

				template<typename A, typename T>
				inline void set(typename smart_ptr_type<T>::smart_ptr_t obj, void(T::*f)(objptr, intf_wrapper<A>)) {
					m_function = new platform::events::member_event_function<T, is_base_of<interface, T>::value, A>(obj, f);
				}

			public: // Overrides
				ANG_DECLARE_INTERFACE();
				platform::events::core_msg_t msg_type()const;
				virtual ifunction* clone()const override;

			protected:
				virtual void invoke(objptr, platform::events::imsg_event_args_t)const override;
				virtual~function_object();

				friend listener<void(object*, platform::events::imsg_event_args*)>;
				friend object_wrapper<function_object<void(object*, platform::events::imsg_event_args*)>>;
			};
		}
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
	};

}

//ANG_DECLARE_OBJECT_VECTOR_SPECIALIZATION(LINK, ang::platform::events::base_event_handler)

namespace ang
{
	namespace core
	{
		namespace delegates
		{
			template<> class LINK listener<void(object*, platform::events::imsg_event_args*)>
			{
			public:
				typedef platform::events::event_t function;

			protected:
				mutable safe_pointer m_parent;
				function_type<bool(platform::events::core_msg_t)> m_comp;
				collections::list<function> m_functions;

				listener(listener &&) = delete;
				listener(listener const&) = delete;

				listener& operator = (listener &&) = delete;
				listener& operator = (listener const&) = delete;

			public:
				listener(object* parent, function_type<bool(platform::events::core_msg_t)>);
				~listener();

			public:
				bool is_empty()const;
				virtual platform::events::event_token_t push(platform::events::event_t);
				virtual bool pop(platform::events::event_token_t);

			public:
				void empty();
				virtual int invoke(platform::events::imsg_event_args_t)const;				
				virtual bool remove(platform::events::event_t);
				int operator ()(platform::events::imsg_event_args_t)const;

			public:
				platform::events::event_token_t operator += (platform::events::event_t);
				bool operator -= (platform::events::event_token_t);

			};

			template<> struct event_helper<void(object*, platform::events::imsg_event_args*)> {
				using type = void(object*, platform::events::imsg_event_args*);
				using listen_token = listen_token<type>;
				using function_type = function_object<type>;
				using add_event_handler = listen_token(*)(base_event*, object_wrapper<function_type>);
				using remove_event_handler = bool(*)(base_event*, listen_token);
			};
		}
	}

	namespace platform
	{
		namespace events
		{

			template<typename T, core_msg MSG >
			class event_handler final : public smart<event_handler<T, MSG>, base_event_handler>
			{
			public:
				template<class O>
				event_handler(O* o, void(O::*f)(objptr, intf_wrapper<T>))
					: base((core_msg_t)MSG) {
					set<T>(o, f);
				}
				template<class O>
				event_handler(typename smart_ptr_type<O>::smart_ptr_t o, void(O::*f)(objptr, intf_wrapper<T>))
					: base((core_msg_t)MSG) {
					set<T>(o, f);
				}
				template<class F>
				event_handler(F f)
					: base((core_msg_t)MSG) {
					set<T>(f);
				}
			};

			using core::delegates::base_event;
		
			template<typename T>
			struct event_helper
			{
				using type = typename remove_reference<typename remove_constant<T>::type>::type;
				using type_ptr = type*;
				using arg_type = intf_wrapper<type>;
				using add_event_type = event_token_t(*)(base_event*, event_t);
				using remove_event_type = bool(*)(base_event*, event_token_t);
			};

			template<typename T, typename event_helper<T>::add_event_type add_event, typename event_helper<T>::remove_event_type remove_event>
			struct event final : public base_event {
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

#define ang_platform_event(_TYPE, _NAME) \
private: \
	ang::platform::events::event_listener m_##_NAME; \
	static ang::platform::events::event_token_t add_##_NAME##_event_handler(ang::platform::events::base_event*, ang::platform::events::event_t); \
	static bool remove_##_NAME##_event_handler(ang::platform::events::base_event*, ang::platform::events::event_token_t); \
public: \
	ang::platform::events::event<_TYPE, add_##_NAME##_event_handler, remove_##_NAME##_event_handler> _NAME;

#define ang_platform_implement_event_handler(_CLASS, _NAME) \
ang::platform::events::event_token_t _CLASS::add_##_NAME##_event_handler(ang::platform::events::base_event* prop, ang::platform::events::event_t e) { \
	return field_to_parent(&_CLASS::_NAME, prop)->m_##_NAME += e;	\
} \
bool _CLASS::remove_##_NAME##_event_handler(ang::platform::events::base_event* prop, ang::platform::events::event_token_t token) { \
	return field_to_parent(&_CLASS::_NAME, prop)->m_##_NAME -= token;	\
}

#endif//__ANG_PLATFORM_EVENTS_H__