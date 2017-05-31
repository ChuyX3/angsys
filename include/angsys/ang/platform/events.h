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

				void* operator new(wsize_t);
				void operator delete(void*);
			};

			template<typename calleable_t, typename args_t>
			class static_event_function
				: public ievent_function
			{
			public:
				typedef calleable_t function_type;

			private:
				function_type _function;

			public:
				inline static_event_function(function_type f) :_function(f) {}
				inline virtual~static_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override {
					_function(caller, static_cast<args_t*>(args.get()));
				}
				inline ievent_function* clone()const override {
					return new static_event_function(_function);
				}
			};

			template<typename obj_t, typename args_t>
			class member_event_function 
				: public ievent_function
			{
			public:
				typedef void(obj_t::*function_type)(objptr, intf_wrapper<args_t>);

			private:
				obj_t * _obj;
				function_type _function;

			public:
				inline member_event_function(obj_t * o, function_type f) : _obj(o), _function(f) {}
				inline virtual~member_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override {
					(_obj->*_function)(caller, static_cast<args_t*>(args.get()));
				}
				inline ievent_function* clone()const override {
					return new member_event_function(_obj, _function);
				}
			};

			template<typename obj_t, typename args_t>
			class object_member_event_function 
				: public ievent_function
			{
			public:
				typedef void(obj_t::*function_type)(objptr, intf_wrapper<args_t>);

			private:
				mutable safe_pointer _obj;
				function_type _function;

			public:
				inline object_member_event_function(object* o, function_type f) : _obj(o), _function(f) {}
				inline virtual~object_member_event_function() {}
				inline void invoke(objptr caller, imsg_event_args_t args)const override {
					objptr _lock = _obj.lock<object>();
					auto lock = interface_cast<obj_t>(_lock.get());
					if (lock != null)(lock->*_function)(caller, static_cast<args_t*>(args.get()));
				}
				inline ievent_function* clone()const override {
					return new object_member_event_function(_obj.lock<object>(), _function);
				}
			};
		}
	}

	namespace core
	{
		namespace delegates
		{
			template<>
			ANG_BEGIN_INTERFACE_FULL_SPECIALIZATION(LINK, ifunction, void(object*, platform::events::imsg_event_args*))
				visible vcall void invoke(objptr, platform::events::imsg_event_args_t)const pure;
				visible vcall ifunction* clone()const pure;
			ANG_END_INTERFACE();

			template<>
			class LINK function_data <void(object*, platform::events::imsg_event_args*)>
				: public object
				, public ifunction<void(object*, platform::events::imsg_event_args*)>
			{
			protected:
				platform::events::core_msg_t _msg;
				platform::events::ievent_function* _function;

				function_data(platform::events::core_msg_t _msg);
			

				template<typename args_t, typename calleable_t>
				inline void set(calleable_t const& f) {
					_function = new platform::events::static_event_function<calleable_t, args_t>(f);
				}

				template<typename args_t, typename obj_t>
				inline void set(obj_t* obj, void(obj_t::*f)(objptr, intf_wrapper<args_t>)) {
					if (is_type_of<obj_t>(type_name<object>()))
						_function = new platform::events::object_member_event_function<obj_t, args_t>(interface_cast<object>(obj), f);
					else
						_function = new platform::events::member_event_function<obj_t, args_t>(obj, f);
				}

			public: // Overrides
				ANG_DECLARE_INTERFACE();
				platform::events::core_msg_t msg_type()const;
				virtual ifunction* clone()const override;

			protected:
				virtual void invoke(objptr, platform::events::imsg_event_args_t)const override;
				virtual~function_data();

				friend listener<void(object*, platform::events::imsg_event_args*)>;
				friend object_wrapper<function_data<void(object*, platform::events::imsg_event_args*)>>;
			};
		}
	}


	template<>
	class LINK object_wrapper<platform::events::event>
	{
	public:
		typedef platform::events::event type;

	protected:
		platform::events::event* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(platform::events::event*);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		platform::events::event* get(void)const;
		void set(platform::events::event*);

		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);
		object_wrapper& operator = (platform::events::event*);
		void operator () (objptr, platform::events::imsg_event_args_t)const;
	};

}

ANG_DECLARE_OBJECT_VECTOR_SPECIALIZATION(LINK, ang::platform::events::event)

namespace ang
{
	namespace core
	{
		namespace delegates
		{
			template<>
			class LINK listener<void(object*, platform::events::imsg_event_args*)>
			{
			public:
				typedef platform::events::event_t function;

			protected:
				mutable safe_pointer _parent;
				function_type<bool, platform::events::core_msg_t> _comp;
				collections::vector<function> _functions;

				listener(listener &&) = delete;
				listener(listener const&) = delete;

				listener& operator = (listener &&) = delete;
				listener& operator = (listener const&) = delete;

			public:
				listener(object* parent, function_type<bool, platform::events::core_msg_t>);
				~listener();

			public:
				bool is_empty()const;
				virtual index append(platform::events::event_t);

			protected:
				void empty();
				virtual int invoke(platform::events::imsg_event_args_t)const;				
				virtual bool remove(platform::events::event_t);
				int operator ()(platform::events::imsg_event_args_t)const;

			public:
				listener& operator += (platform::events::event_t);
				listener& operator -= (platform::events::event_t);

			};
		}
	}

	namespace platform
	{
		namespace events
		{
			template<typename owner_t>
			class event_trigger final
				: public event_listener
			{
				friend owner_t;
				using event_listener::invoke;
				using event_listener::operator();
			};
			template<typename owner_t>using owner_event = event_trigger<owner_t>;
		}
	}
}

#endif//__ANG_PLATFORM_EVENTS_H__