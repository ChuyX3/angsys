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
#elif !defined __ANG_LISTENER_H__
#define __ANG_LISTENER_H__


namespace ang
{
	namespace delegates
	{
		template<typename T> struct ilistener;
		struct base_event {};
		template<typename T> using listen_token = typename list<function<T>>::base_iterator_t;

		template<typename T, typename...Ts>
		struct ilistener<T(Ts...)> 
			: intf<ilistener<T(Ts...)>
			, iid("ang::delegates::ilistener")>
		{
			virtual array<T> invoke(Ts... args)const = 0;
			virtual listen_token<T(Ts...)> push(function<T(Ts...)> func) = 0;
			virtual bool pop(listen_token<T(Ts...)> token) = 0;
		};

		template<typename T, typename... Ts>
		class listener <T(Ts...)>
		{
		public:
			typedef function<T(Ts...)> type;

		protected:
			list<function<T(Ts...)>> functions;

		public:
			inline listener();
			inline listener(listener &&);
			inline listener(listener const&);
			inline ~listener();

		public:
			inline void clear();
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
			inline array<T> invoke(Ts... args)const;
			inline array<T> operator()(Ts... args)const;
		};

		template<typename... Ts>
		class listener <void(Ts...)>
		{
		public:
			typedef function<void(Ts...)> type;

		protected:
			list<function<void(Ts...)>> functions;

		public:
			inline listener();
			inline listener(listener &&);
			inline listener(listener const&);
			inline ~listener();

		public:
			inline void clear();
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
			list<function<dword(pointer)>> functions;

		public:
			listener();
			listener(listener &&);
			listener(listener const&);
			~listener();

		public:
			void clear();
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
			list<function<void(objptr, pointer)>> functions;

		public:
			listener();
			listener(listener &&);
			listener(listener const&);
			~listener();

		public:
			void clear();
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
			list<function<void(objptr, var_args_t)>> functions;

		public:
			listener();
			listener(listener &&);
			listener(listener const&);
			~listener();

		public:
			void clear();
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
			using function_type = function_base<type>;
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

	using delegates::event;
	using delegates::base_event;
	using delegates::listener;
	using delegates::listen_token;

#define ang_event(NAME, ...) _ang_event(NAME, __VA_ARGS__)
#define _ang_event(NAME, TYPE) \
private: \
	static ang::listen_token<TYPE> add_##NAME(ang::base_event*, ang::function<TYPE>); \
	static bool remove_##NAME(ang::base_event*, ang::listen_token<TYPE>); \
	ang::listener<TYPE> m_##NAME; \
public: \
	ang::event<TYPE, add_##NAME, remove_##NAME> NAME;




}

#endif//__ANG_LISTENER_H__
