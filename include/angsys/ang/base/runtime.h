#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_RUNTIME_H__
#define __ANG_BASE_RUNTIME_H__

namespace ang //constants
{
	namespace runtime
	{
		typedef cstr_view<char> type_name_t;
		typedef struct __type_info type_info_t, rtti, rtti_t;
		template<typename T, genre_t TYPE = genre_of<T>()> struct type_info_builder;


		template<typename T, genre_t TYPE> 
		struct type_info_builder
		{
			static rtti_t const& type_of();
		};

		struct LINK __type_info {
			__type_info(rtti_t&&) = delete;
			__type_info(const rtti_t&) = delete;
			rtti_t& operator = (rtti_t&&) = delete;
			rtti_t& operator = (const rtti_t&) = delete;

			inline type_name_t const& type_name()const { return _type_name; }
			inline wsize type_id()const { return (wsize)_type_name.cstr(); }
			inline genre_t genre()const { return _genre; }
			inline wsize size()const { return _size; }
			inline wsize aligment()const { return _aligment; }

			template<typename T>
			static rtti_t const& type_of() {
				return type_info_builder<T>::type_of();
			}

			template<typename T>
			static rtti_t const& type_of(const T&) {
				return type_info_builder<T>::type_of();
			}

			static rtti_t const& regist(type_name_t name, genre_t g, wsize sz, wsize a);

		private:
			inline __type_info(type_name_t name, genre_t g, wsize sz, wsize a) {
				_type_name = name;
				_genre = g;
				_size = sz;
				_aligment = a;
			}
			inline ~__type_info() {

			}
			type_name_t _type_name;
			genre_t _genre;
			wsize _size;
			wsize _aligment;	
		};


		template<typename T, genre_t TYPE>
		rtti_t const& type_info_builder<T, TYPE>::type_of()
		{
			rtti_t const& info = rtti::regist("unknown", genre_of<T>(), sizeof(T), alignof(T));
			return info;
		}
	}
	
	using namespace runtime;
}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO(_LINK, _TYPE) \
namespace ang { namespace runtime{ \
template<> struct _LINK type_info_builder<_TYPE> { \
	static rtti_t const& type_of(); \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE(_TYPE) \
namespace ang { namespace runtime{ \
template<> struct _LINK type_info_builder<_TYPE> { \
	static rtti_t const& type_of() { \
		static rtti_t const& info = rtti::regist(#_TYPE, genre_of<_TYPE>(), sizeof(_TYPE), alignof(_TYPE)); \
		return info; \
	} \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_INLINE_OVERRIDE(_TYPE, _NAME) \
namespace ang { namespace runtime{ \
template<> struct _LINK type_info_builder<_TYPE> { \
	static rtti_t const& type_of() { \
		static rtti_t const& info = rtti::regist(_NAME, genre_of<_TYPE>(), sizeof(_TYPE), alignof(_TYPE)); \
		return info; \
	} \
};}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT(_TYPE) \
namespace ang { namespace runtime{ \
rtti_t const& type_info_builder<_TYPE>::type_of() { \
	static rtti_t const& info = rtti::regist(#_TYPE, genre_of<_TYPE>(), sizeof(_TYPE), alignof(_TYPE)); \
	return info; \
}}}

#define ANG_REGIST_RUNTIME_VALUE_TYPE_INFO_IMPLEMENT_OVERRIDE(_TYPE, _NAME) \
namespace ang { namespace runtime{ \
rtti_t const& type_info_builder<_TYPE>::type_of() { \
	static rtti_t const& info = rtti::regist(_NAME, genre_of<_TYPE>(), sizeof(_TYPE), alignof(_TYPE)); \
	return info; \
}}}


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