#ifndef __ANGSYS_H__
#elif !defined __CORE_LISTENER_H__
#define __CORE_LISTENER_H__

namespace ang
{
	namespace core
	{
		namespace delegates 
		{
			struct base_event {};

			template<typename T> using listen_token = collections::base_iterator<function<T>>;

			template<typename T, typename... Ts>
			class listener <T(Ts...)>
			{
			public:
				typedef function<T(Ts...)> type;

			protected:
				collections::list<function<T(Ts...)>> functions;

			public:
				inline listener();
				inline listener(listener &&);
				inline listener(listener const&);
				inline ~listener();

			public:
				inline void empty();
				inline bool is_empty()const;
				inline listen_token<T(Ts...)> push(function<T(Ts...)> func);
				inline bool pop(listen_token<T(Ts...)> token);

			public:
				inline listener& operator = (listener &&);
				inline listener& operator = (listener const&);
				inline listen_token<T(Ts...)> operator += (function<T(Ts...)>);
				inline bool operator -= (listen_token<T(Ts...)>);

				template<typename calleable_t>
				inline listen_token<T(Ts...)> operator += (calleable_t const& func) {
					return push(function<T(Ts...)>(func));
				}
				inline collections::ienum_t<T> invoke(Ts... args)const;
				inline collections::ienum_t<T> operator()(Ts... args)const;
			};

			template<typename... Ts>
			class listener <void(Ts...)>
			{
			public:
				typedef function<void(Ts...)> type;

			protected:
				collections::list<function<void(Ts...)>> functions;

			public:
				inline listener();
				inline listener(listener &&);
				inline listener(listener const&);
				inline ~listener();

			public:
				inline void empty();
				inline bool is_empty()const;
				inline listen_token<void(Ts...)> push(function<void(Ts...)> func);
				inline bool pop(listen_token<void(Ts...)> token);

			public:
				inline listener& operator = (listener &&);
				inline listener& operator = (listener const&);
				inline listen_token<void(Ts...)> operator += (function<void(Ts...)>);
				inline bool operator -= (listen_token<void(Ts...)>);

				template<typename calleable_t>
				inline listen_token<void(Ts...)> operator += (calleable_t const& func) {
					return push(function<void(Ts...)>(func));
				}

				inline void invoke(Ts... args)const;
				inline void operator()(Ts ... args)const;
			};


			template<> class LINK listener <dword(pointer)>
			{
			public:
				typedef function<dword(pointer)> type;

			protected:
				collections::list<function<dword(pointer)>> functions;

			public:
				listener();
				listener(listener &&);
				listener(listener const&);
				~listener();

			public:
				void empty();
				bool is_empty()const;
				listen_token<dword(pointer)> push(function<dword(pointer)> func);
				bool pop(listen_token<dword(pointer)> token);

			public:
				listener& operator = (listener &&);
				listener& operator = (listener const&);
				listen_token<dword(pointer)> operator += (function<dword(pointer)>);
				bool operator -= (listen_token<dword(pointer)>);

				template<typename calleable_t>
				inline listen_token<dword(pointer)> operator += (calleable_t const& func) {
					return push(function<dword(pointer)>(func));
				}

				array<dword> invoke(pointer)const;
				array<dword> operator()(pointer)const;
			};

			template<> class LINK listener <void(objptr, pointer)>
			{
			public:
				typedef function<void(objptr, pointer)> type;

			protected:
				collections::list<function<void(objptr, pointer)>> functions;

			public:
				listener();
				listener(listener &&);
				listener(listener const&);
				~listener();

			public:
				void empty();
				bool is_empty()const;
				listen_token<void(objptr, pointer)> push(function<void(objptr, pointer)> func);
				bool pop(listen_token<void(objptr, pointer)> token);

			public:
				listener& operator = (listener &&);
				listener& operator = (listener const&);
				listen_token<void(objptr, pointer)> operator += (function<void(objptr, pointer)>);
				bool operator -= (listen_token<void(objptr, pointer)>);
				template<typename calleable_t>
				inline listen_token<void(objptr, pointer)> operator += (calleable_t const& func) {
					return push(function<void(objptr, pointer)>(func));
				}

				void invoke(objptr, pointer)const;
				void operator()(objptr, pointer)const;
			};

			template<> class LINK listener <void(objptr, var_args_t)>
			{
			public:
				typedef function<void(objptr, var_args_t)> type;

			protected:
				collections::list<function<void(objptr, var_args_t)>> functions;

			public:
				listener();
				listener(listener &&);
				listener(listener const&);
				~listener();

			public:
				void empty();
				bool is_empty()const;
				listen_token<void(objptr, var_args_t)> push(function<void(objptr, var_args_t)> func);
				bool pop(listen_token<void(objptr, var_args_t)> token);

			public:
				listener& operator = (listener &&);
				listener& operator = (listener const&);
				listen_token<void(objptr, var_args_t)> operator += (function<void(objptr, var_args_t)>);
				bool operator -= (listen_token<void(objptr, var_args_t)>);

				template<typename calleable_t>
				inline listen_token<void(objptr, var_args_t)> operator += (calleable_t const& func) {
					return push(function<void(objptr, var_args_t)>(func));
				}

				void invoke(objptr, var_args_t)const;
				void operator()(objptr, var_args_t)const;
			};


			template<typename T> struct event_helper;

			template<typename T, typename...Ts> 
			struct event_helper<T(Ts...)> {
				using type = T(Ts...);
				using listen_token = listen_token<type>;
				using function_type = function_object<type>;
				using add_event_handler = listen_token(*)(base_event*, object_wrapper<function_type>);
				using remove_event_handler = bool(*)(base_event*, listen_token);
			};

			template<typename T, typename event_helper<T>::add_event_handler add_event, typename event_helper<T>::remove_event_handler remove_event>
			struct event : public base_event
			{
				inline auto operator += (typename event_helper<T>::function_type* func) {
					return add_event(this, func);
				}
				inline auto operator += (object_wrapper<typename event_helper<T>::function_type> func) {
					return add_event(this, func.get());
				}
				inline auto operator -= (typename event_helper<T>::listen_token token) {
					return remove_event(this, ang::forward<typename event_helper<T>::listen_token>(token));
				}
			};
			
		}
	}

	//using core::delegates::event;
}


#endif//__CORE_LISTENER_H__