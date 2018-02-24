#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_RUNTIME_H__
#define __ANG_BASE_RUNTIME_H__

namespace ang //constants
{
	namespace runtime
	{
		typedef void* unknown_t;
		typedef void** unknown_ptr_t;
		typedef wsize type_id_t;
		typedef cstr_view<char> type_name_t;
		typedef struct __type_info type_info_t, rtti, rtti_t;
		template<typename T, genre_t TYPE = genre_of<T>()> struct type_info_builder;

		typedef bool(*dynamic_cast_proc)(const rtti_t&, unknown_t, const rtti_t&, unknown_ptr_t);

		template<typename T, genre_t TYPE>
		struct type_info_builder {
			typedef T type;
			static rtti_t const& type_of();
			static bool is_type_of(rtti_t const&);
			template<typename N> static bool is_type_of();
			template<typename N> static bool is_type_of(N const&);
			template<typename N> static N* dyn_cast(T*);
			template<typename N> static N const* dyn_cast(T const*);
		};

		template<typename T> inline rtti_t const& type_of() { return type_info_builder<T>::type_of(); }
		template<typename T> inline rtti_t const& type_of(const T&) { return type_info_builder<T>::type_of(); }
		template<typename T> inline type_id_t type_id() { return type_info_builder<T>::type_of().type_id(); }
		template<typename T> inline type_id_t type_id(const T&) { return type_info_builder<T>::type_of().type_id(); }
		template<typename T> inline type_name_t type_name() { return type_info_builder<T>::type_of().type_name(); }
		template<typename T> inline type_name_t type_name(const T&) { type_info_builder<T>::type_of().type_name(); }
		template<typename T> inline bool is_type_name(rtti_t const& id) { type_info_builder<T>::is_type_of(id); }
		template<typename T, typename U> inline bool is_type_name() { type_info_builder<T>::template is_type_of<U>(); }
		template<typename T, typename U> inline bool is_type_name(const U&) { type_info_builder<T>::template is_type_of<U>(); }
		template<typename To, typename From> inline To* dyn_cast(From* old) { type_info_builder<From>::template dyn_cast<To>(old); }

		struct LINK __type_info {
			__type_info(rtti_t&&) = delete;
			__type_info(const rtti_t&) = delete;
			rtti_t& operator = (rtti_t&&) = delete;
			rtti_t& operator = (const rtti_t&) = delete;

			inline type_name_t const& type_name()const { return _type_name; }
			inline type_id_t type_id()const { return (type_id_t)_type_name.cstr(); }
			inline genre_t genre()const { return _genre; }
			inline wsize size()const { return _size; }
			inline wsize aligment()const { return _aligment; }
			inline array_view<rtti_t const*> const& parents()const { return _parents; }
			inline bool is_type_of(rtti_t const& id)const {
				if (type_id() == id.type_id())
					return true;
				for (rtti_t const* type : _parents) {
					if (type->is_type_of(id))
						return true;
				}
				return false;
			}
			inline bool dyn_cast(unknown_t from, rtti_t const& id, unknown_ptr_t out)const { return _dyn_cast(*this, from, id, out); }

			template<typename T>
			static rtti_t const& type_of() {
				return type_info_builder<T>::type_of();
			}

			template<typename T>
			static rtti_t const& type_of(const T&) {
				return type_info_builder<T>::type_of();
			}

			template<typename T>
			bool is_type_of()const {
				return is_type_of(type_of<T>());
			}

			template<typename T>
			bool is_type_of(const T&)const {
				return is_type_of(type_of<T>());
			}

			template<typename To>
			inline bool dyn_cast(unknown_t from, To** out)const {
				return dyn_cast(from, type_of<To>(), (unknown_ptr_t)out);
			}

			template<typename From, typename To>
			static inline bool dyn_cast(From* from, To** out) { 
				return type_of<From>().dyn_cast((unknown_t)from, type_of<To>(), (unknown_ptr_t)out);
			}

			static rtti_t const& regist(type_name_t name, genre_t g, wsize sz, wsize a);
			static rtti_t const& regist(type_name_t name, genre_t g, wsize sz, wsize a, array_view<rtti_t const *> parent, dynamic_cast_proc cast = nullptr);

			inline ~__type_info() {}
		private:
			static bool default_dyn_cast(const rtti_t&, unknown_t, const rtti_t&, unknown_ptr_t);

			inline __type_info(type_name_t name, genre_t g, wsize sz, wsize a) {
				_type_name = name;
				_genre = g;
				_size = sz;
				_aligment = a;
				_parents = null;
				_dyn_cast = &default_dyn_cast;
			}
			inline __type_info(type_name_t name, genre_t g, wsize sz, wsize a, array_view<rtti_t const *> parents, dynamic_cast_proc cast = nullptr) {
				_type_name = name;
				_genre = g;
				_size = sz;
				_aligment = a;
				_parents = ang::move(parents);
				_dyn_cast = cast ? cast : &default_dyn_cast;
			}

			type_name_t _type_name;
			genre_t _genre;
			wsize _size;
			wsize _aligment;
			dynamic_cast_proc _dyn_cast;
			array_view<rtti_t const*> _parents;
		};


		template<typename T, genre_t TYPE>
		struct type_info_builder<const T, TYPE> : type_info_builder<T, TYPE> {};

		template<typename T, genre_t TYPE>
		struct type_info_builder<T&, TYPE> : type_info_builder<T, TYPE> {};

		template<typename T, genre_t TYPE>
		struct type_info_builder<T const&, TYPE> : type_info_builder<T, TYPE> {};

		template<typename T, genre_t TYPE>
		struct type_info_builder<T&&, TYPE> : type_info_builder<T, TYPE> {};


		template<typename T, genre_t TYPE> rtti_t const& type_info_builder<T, TYPE>::type_of() {
			rtti_t const& info = rtti::regist("unknown", genre_of<T>(), sizeof(T), alignof(T));
			return info;
		}

		template<typename T, genre_t TYPE> inline bool type_info_builder<T, TYPE>::is_type_of(rtti_t const& id) { return type_of().is_type_of(id); }
		template<typename T, genre_t TYPE> template<typename N> inline bool type_info_builder<T, TYPE>::is_type_of() { return type_of().is_type_of(runtime::type_of<N>()); }
		template<typename T, genre_t TYPE> template<typename N> inline bool type_info_builder<T, TYPE>::is_type_of(const N&) { return type_of().is_type_of(runtime::type_of<N>()); }
		template<typename T, genre_t TYPE> template<typename N> inline N* type_info_builder<T, TYPE>::dyn_cast(T* old) {
			N* _new = null; 
			type_of().dyn_cast(old, runtime::type_of<N>(), &_new);
			return _new;
		}
		template<typename T, genre_t TYPE> template<typename N> inline N const* type_info_builder<T, TYPE>::dyn_cast(T const* old) {
			N* _new = null;
			type_of().dyn_cast(const_cast<T*>(old), runtime::type_of<N>(), &_new);
			return _new;
		}
	}
	
	using namespace runtime;
}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _TYPE) \
namespace ang { namespace runtime{ \
template<> struct _LINK type_info_builder<_TYPE> { \
	static rtti_t const& type_of(); \
	static bool is_type_of(rtti_t const&); \
	template<typename T> inline static bool is_type_of() { return type_of().is_type_of(runtime::type_of<T>()); } \
	template<typename T> inline static bool is_type_of(T const&) { return type_of().is_type_of(runtime::type_of<T>()); } \
	template<typename T> inline static T* dyn_cast(_TYPE* old) { T* _new = null; type_of().dyn_cast(old, runtime::type_of<T>(), &_new); return _new; } \
	template<typename T> inline static T const* dyn_cast(_TYPE const*) { T* _new = null; type_of().dyn_cast(const_cast<_TYPE*>(old), runtime::type_of<T>(), &_new); return _new; } \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(_TYPE) \
namespace ang { namespace runtime{ \
template<> struct _LINK type_info_builder<_TYPE> { \
	static rtti_t const& type_of() { static rtti_t const& info = rtti::regist(#_TYPE, genre_of<_TYPE>(), sizeof(_TYPE), alignof(_TYPE)); return info; } \
	static bool is_type_of(rtti_t const& id) { return type_of().is_type_of(id); } \
	template<typename T> inline static bool is_type_of() { return type_of().is_type_of(runtime::type_of<T>()); } \
	template<typename T> inline static bool is_type_of(T const&) { return type_of().is_type_of(runtime::type_of<T>()); } \
	template<typename T> inline static T* dyn_cast(_TYPE* old) { T* _new = null; type_of().dyn_cast(old, runtime::type_of<T>(), &_new); return _new; } \
	template<typename T> inline static T const* dyn_cast(_TYPE const*) { T* _new = null; type_of().dyn_cast(const_cast<_TYPE*>(old), runtime::type_of<T>(), &_new); return _new; } \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE_OVERRIDE(_TYPE, _NAME) \
namespace ang { namespace runtime{ \
template<> struct _LINK type_info_builder<_TYPE> { \
	static rtti_t const& type_of() { static rtti_t const& info = rtti::regist(_NAME, genre_of<_TYPE>(), sizeof(_TYPE), alignof(_TYPE)); return info; } \
	static bool is_type_of(rtti_t const& id) { return type_of().is_type_of(id); } \
	template<typename T> inline static bool is_type_of() { return type_of().is_type_of(runtime::type_of<T>()); } \
	template<typename T> inline static bool is_type_of(T const&) { return type_of().is_type_of(runtime::type_of<T>()); } \
	template<typename T> inline static T* dyn_cast(_TYPE* old) { T* _new = null; type_of().dyn_cast(old, runtime::type_of<T>(), &_new); return _new; } \
	template<typename T> inline static T const* dyn_cast(_TYPE const*) { T* _new = null; type_of().dyn_cast(const_cast<_TYPE*>(old), runtime::type_of<T>(), &_new); return _new; } \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(_TYPE) \
namespace ang { namespace runtime{ \
rtti_t const& type_info_builder<_TYPE>::type_of() { static rtti_t const& info = rtti::regist(#_TYPE, genre_of<_TYPE>(), sizeof(_TYPE), alignof(_TYPE)); return info; } \
bool type_info_builder<_TYPE>::is_type_of(rtti_t const& id) { return type_of().is_type_of(id); } \
}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(_TYPE, _NAME) \
namespace ang { namespace runtime{ \
rtti_t const& type_info_builder<_TYPE>::type_of() { static rtti_t const& info = rtti::regist(_NAME, genre_of<_TYPE>(), sizeof(_TYPE), alignof(_TYPE)); return info; } \
bool type_info_builder<_TYPE>::is_type_of(rtti_t const& id) { return type_of().is_type_of(id); } \
}}


ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, char);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, byte);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, wchar);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, short);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ushort);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, int);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, uint);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, long);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ulong);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, long64);
ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(LINK, ulong64);

#endif//__ANG_BASE_RUNTIME_H__