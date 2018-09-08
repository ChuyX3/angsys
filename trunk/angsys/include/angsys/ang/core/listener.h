#ifndef __ANGSYS_H__
#elif !defined __CORE_LISTENER_H__
#define __CORE_LISTENER_H__

namespace ang
{
	namespace core
	{
		namespace delegates 
		{
			template<typename T, typename... Ts>
			class listener <T(Ts...)>
			{
			public:
				typedef function<T(Ts...)> type;

			protected:
				collections::vector<function<T(Ts...)>> functions;

			public:
				inline listener();
				inline listener(listener &&);
				inline listener(listener const&);
				inline ~listener();

			public:
				inline void empty();
				inline bool is_empty()const;
				inline windex push(function<T(Ts...)> func);

			public:
				inline listener& operator = (listener &&);
				inline listener& operator = (listener const&);
				inline listener& operator += (function<T(Ts...)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					push(function<T(Ts...)>(func));
					return*this;
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
				collections::vector<function<void(Ts...)>> functions;

			public:
				inline listener();
				inline listener(listener &&);
				inline listener(listener const&);
				inline ~listener();

			public:
				inline void empty();
				inline bool is_empty()const;
				inline windex push(function<void(Ts...)> func);

			public:
				inline listener& operator = (listener &&);
				inline listener& operator = (listener const&);
				inline listener& operator += (function<void(Ts...)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					push(function<void(Ts...)>(func));
					return*this;
				}

				inline void invoke(Ts... args)const;
				inline void operator()(Ts ... args)const;
			};


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
				windex push(function<dword(pointer)> func);

			public:
				listener& operator = (listener &&);
				listener& operator = (listener const&);
				listener& operator += (function<dword(pointer)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					push(function<dword(pointer)>(func));
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
				windex push(function<void(objptr, pointer)> func);

			public:
				listener& operator = (listener &&);
				listener& operator = (listener const&);
				listener& operator += (function<void(objptr, pointer)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					push(function<void(objptr, pointer)>(func));
					return*this;
				}

				void invoke(objptr, pointer)const;
				void operator()(objptr, pointer)const;
			};

			template<> class LINK listener <void(objptr, var_args_t)>
			{
			public:
				typedef function<void(objptr, var_args_t)> type;

			protected:
				collections::vector<function<void(objptr, var_args_t)>> functions;

			public:
				listener();
				listener(listener &&);
				listener(listener const&);
				~listener();

			public:
				void empty();
				bool is_empty()const;
				windex push(function<void(objptr, var_args_t)> func);

			public:
				listener& operator = (listener &&);
				listener& operator = (listener const&);
				listener& operator += (function<void(objptr, var_args_t)>);

				template<typename calleable_t>
				inline listener& operator += (calleable_t const& func) {
					push(function<void(objptr, var_args_t)>(func));
					return*this;
				}

				void invoke(objptr, var_args_t)const;
				void operator()(objptr, var_args_t)const;
			};
		}
	}

	template<typename proxy, typename T>
	struct event : public core::delegates::listener<T>
	{
	private:
		using core::delegates::listener<T>::invoke;
		using core::delegates::listener<T>::operator();
		friend proxy;
	};

	//template<typename proxy, typename T, typename... Ts>
	//struct event<proxy, T(Ts...)> : public core::delegates::listener<T(Ts...)>
	//{
	//private:
	//	using listener<T(Ts...)>::invoke;
	//	using listener<T(Ts...)>::operator();
	//	friend proxy;
	//};
}


#endif//__CORE_LISTENER_H__