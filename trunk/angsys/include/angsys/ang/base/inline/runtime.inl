#ifndef __ANG_BASE_RUNTIME_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_RUNTIME_INL__
#define __ANG_BASE_RUNTIME_INL__

namespace ang
{
	namespace runtime
	{
		template<typename... Ts>
		inline string& args_list_type_name(string& out);

		template<typename T, typename... Ts>
		inline string& args_list_type_name_builder(string& out) {
			out += rtti::type_of<T>().type_name();
			return args_list_type_name<Ts...>(out);
		}

		template<typename... Ts>
		inline string& args_list_type_name(string& out) {
			out += ","_s;
			return args_list_type_name_builder<Ts...>(out);
		}

		template<>
		inline string& args_list_type_name<>(string& out) { 
			return out; 
		}

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct __type_info_builder_genre_value_helper;

		template<typename T>
		struct __type_info_builder_genre_value_helper {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist("value<'unknown'>", ang::genre::value_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_value_helper<const T> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string("const "_s) += rtti::type_of<type>().type_name())
					, ang::genre::value_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_value_helper<T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::value_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_value_helper<T&&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&&"_s)
					, ang::genre::value_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_value_helper<const T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string((string("const "_s) += rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::value_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_value_helper<T*> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<T>().type_name()) += "*"_s)
					, ang::genre::value_type, sizeof(type*), alignof(type*));
				return info;
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct __type_info_builder_genre_union_helper;

		template<typename T>
		struct __type_info_builder_genre_union_helper {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist("union<'unknown'>", ang::genre::union_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_union_helper<const T> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string("const "_s) += rtti::type_of<type>().type_name())
					, ang::genre::union_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_union_helper<T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::union_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_union_helper<T&&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&&"_s)
					, ang::genre::union_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_union_helper<const T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string((string("const "_s) += rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::union_type, sizeof(type), alignof(type));
				return info;
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct __type_info_builder_genre_enum_helper;

		template<typename T>
		struct __type_info_builder_genre_enum_helper {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist("union<'unknown'>", ang::genre::enum_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_enum_helper<const T> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string("const "_s) += rtti::type_of<type>().type_name())
					, ang::genre::enum_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_enum_helper<T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::enum_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_enum_helper<T&&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&&"_s)
					, ang::genre::enum_type, sizeof(type), alignof(type));
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_enum_helper<const T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string((string("const "_s) += rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::enum_type, sizeof(type), alignof(type));
				return info;
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T, bool IS_INTERFACE = has_class_info_function<T>::value>
		struct __type_info_builder_genre_class_helper {
			static rtti_t const& type_of() { return T::class_info(); }
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<const T, true> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string("const "_s) += rtti::type_of<type>().type_name())
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<T&, true> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<T&&, true> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&&"_s)
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<const T&, true> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string((string("const "_s) += rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};


		template<typename T>
		struct __type_info_builder_genre_class_helper<T, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist("class<'unknown'>"
					, genre::class_type
					, size_of<type>()
					, align_of<type>()
				);
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<const T, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string("const "_s) += rtti::type_of<type>().type_name())
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<T&, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<T&&, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "&&"_s)
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<const T&, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string((string("const "_s) += rtti::type_of<type>().type_name()) += "&"_s)
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<intf_wrapper<T>, false> {
			static rtti_t const& type_of() { return type_of<T>(); }
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<object_wrapper<T>, false> {
			static rtti_t const& type_of() { return type_of<T>(); }
		};

		template<typename T>
		struct __type_info_builder_genre_class_helper<array_view<T>, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_constant<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(string(rtti::type_of<type>().type_name()) += "[]"_s)
					, ang::genre::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct __type_info_builder_genre_function_helper;

		template<typename T, typename... Ts>
		struct __type_info_builder_genre_function_helper<T(Ts...)> { 
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(((string(rtti::type_of<T>().type_name()) += "("_s) += runtime::args_list_type_name<Ts...>()) += ")"_s)
					, genre::function_type, sizeof(wsize), alignof(wsize));
				return info;
			} 
		};

		template<typename T, typename... Ts>
		struct __type_info_builder_genre_function_helper<T(*)(Ts...)> {
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(((string(rtti::type_of<T>().type_name()) += "(*)("_s) += runtime::args_list_type_name<Ts...>()) += ")"_s)
					, genre::function_type, sizeof(wsize), alignof(wsize));
				return info;
			}
		};

#if defined WINDOWS_PLATFORM && defined _X86_
		template<typename T, typename... Ts>
		struct __type_info_builder_genre_function_helper<T(__stdcall*)(Ts...)> {
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(((string(rtti::type_of<T>().type_name()) += "(__stdcall*)("_s) += runtime::args_list_type_name<Ts...>()) += ")"_s)
					, genre::function_type, sizeof(wsize), alignof(wsize));
				return info;
			}
		};

#endif


		template<typename T, typename O, typename... Ts>
		struct __type_info_builder_genre_function_helper<T(O::*)(Ts...)> {
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					text::string_pool::instance()->save_string(((((string(rtti::type_of<T>().type_name()) +="("_s ) += rtti::type_of<O>().type_name()) += "::*)("_s) += runtime::args_list_type_name<Ts...>()) += ")"_s)
					, genre::function_type, sizeof(wsize), alignof(wsize));
				return info;
			}
		};

	}
}

template<typename... Ts>
inline ang::type_name_t ang::runtime::args_list_type_name()
{
	static string out;
	static type_name_t name = text::string_pool::instance()->save_string(
		args_list_type_name_builder<Ts...>(out)
	);
	return name;
}

template<>
inline ang::type_name_t ang::runtime::args_list_type_name()
{
	return rtti::type_of<void>().type_name();
}

template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::genre::value_type>::type_of() {
	return __type_info_builder_genre_value_helper<T>::type_of();
}

template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::genre::union_type>::type_of() {
	return __type_info_builder_genre_union_helper<T>::type_of();
}

template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::genre::enum_type>::type_of() {
	return __type_info_builder_genre_enum_helper<T>::type_of();
}

template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::genre::class_type>::type_of() {
	return __type_info_builder_genre_class_helper<T>::type_of();
}


template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::genre::function_type>::type_of() {
	return __type_info_builder_genre_function_helper<T>::type_of();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif//__ANG_BASE_RUNTIME_INL__