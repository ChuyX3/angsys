#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_INTERFACE_H__
#define __ANG_BASE_INTERFACE_H__

#define vcall virtual
#define pure = 0
#define visible public:
#define inherite protected:
#define hidden private:

namespace ang //constants
{
	struct LINK interface {
		visible static rtti_t const& class_info();
		visible vcall rtti_t const& object_info()const pure;
		visible vcall bool query_interface(rtti_t const&, unknown_ptr_t)pure;
		inherite static bool default_query_interface(rtti_t const&, unknown_t, rtti_t const&, unknown_ptr_t);
	};

	template<typename T> struct is_interface : or_expression<is_same_type<T, interface>::value, is_inherited_from<T, interface>::value> {};


	namespace runtime
	{
		template<typename T, bool IS_INTERFACE = is_interface<T>::value>
		struct __type_info_builder_genre_class_helper {
			static rtti_t const& type_of();
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<T, true> {
			static rtti_t const& type_of() { return T::class_info(); }
		};

		template<typename T>
		struct type_info_builder<T, genre::class_type> : __type_info_builder_genre_class_helper<T> {

		};
	}
}

#endif//__ANG_BASE_INTERFACE_H__