/*********************************************************************************************************************/
/*   File Name: ang/base/inline/runtime.inl                                                                          */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/


#ifndef __ANG_BASE_RUNTIME_H__
#error ang/base/base.h is not included
#elif !defined __ANG_BASE_RUNTIME_INL__
#define __ANG_BASE_RUNTIME_INL__

namespace ang
{
	namespace runtime
	{
		template<typename... Ts>
		struct __args_list_type_name_builder1;

		template<typename... Ts>
		struct __args_list_type_name_builder2;

		template<typename... Ts>
		inline fast_astring_t& args_list_type_name(fast_astring_t& out);

		template<typename T, typename... Ts>
		struct __args_list_type_name_builder1<T, Ts...> {
			inline static fast_astring_t& build(fast_astring_t& out) {
				out << rtti::type_of<T>().type_name();
				return __args_list_type_name_builder2<Ts...>::build(out);
			}
		};

		template<>
		struct __args_list_type_name_builder1<> {
			inline static fast_astring_t& build(fast_astring_t& out) {
				//out << "void"_sv;
				return out;
			}
		};

		template<typename... Ts>
		struct __args_list_type_name_builder2 {
			inline static fast_astring_t& build(fast_astring_t& out) {
				out << ","_sv;
				return __args_list_type_name_builder1<Ts...>::build(out);
			}
		};

		template<>
		struct __args_list_type_name_builder2<> {
			inline static fast_astring_t& build(fast_astring_t& out) {
				return out;
			}
		};

	
		template<typename... Ts>
		inline fast_astring_t& args_list_type_name(fast_astring_t& out) {
			return __args_list_type_name_builder1<Ts...>::build(out);
		}

		template<typename... Ts>
		inline fast_astring_t& operator << (fast_astring_t& stream, type_pack<Ts...>const&) {
			return __args_list_type_name_builder1<Ts...>::build(stream);
		}

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct __type_info_builder_gender_value_helper;

		template<typename T>
		struct __type_info_builder_gender_value_helper {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist("value<'unknown'>"_sv, ang::gender::value_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_value_helper<const T> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name();
						return ang::move(out);
					}(),
					ang::gender::value_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_value_helper<T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::value_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_value_helper<T&&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&&"_sv;
						return ang::move(out);
					}(),
					ang::gender::value_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_value_helper<const T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::value_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_value_helper<T*> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "*"_sv;
						return ang::move(out);
					}(),
					ang::gender::value_type, sizeof(type*), alignof(type*));
				return info;
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct __type_info_builder_gender_union_helper;

		template<typename T>
		struct __type_info_builder_gender_union_helper {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist("union<'unknown'>"_sv, ang::gender::union_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_union_helper<const T> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name();
						return ang::move(out);
					}(),
					ang::gender::union_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_union_helper<T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::union_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_union_helper<T&&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&&"_sv;
						return ang::move(out);
					}(),
					ang::gender::union_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_union_helper<const T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::union_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct __type_info_builder_gender_enum_helper;

		template<typename T>
		struct __type_info_builder_gender_enum_helper {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist("enum<'unknown'>"_sv, ang::gender::enum_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_enum_helper<const T> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name();
						return ang::move(out);
					}(),
					ang::gender::enum_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_enum_helper<T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::enum_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_enum_helper<T&&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&&"_sv;
						return ang::move(out);
					}(),
					ang::gender::enum_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_enum_helper<const T&> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::enum_type, size_of<type>(), align_of<type>());
				return info;
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T, bool IS_INTERFACE = has_class_info_function<T>::value>
		struct __type_info_builder_gender_class_helper {
			static rtti_t const& type_of() { return T::class_info(); }
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<const T, true> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name();
						return ang::move(out);
					}(),
					ang::gender::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<T&, true> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<T&&, true> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&&"_sv;
						return ang::move(out);
					}(),
					ang::gender::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<const T&, true> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};


		template<typename T>
		struct __type_info_builder_gender_class_helper<T, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist("class<'unknown'>"_sv
					, gender::class_type
					, size_of<type>()
					, align_of<type>()
				);
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<const T, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name();
						return ang::move(out);
					}(),
					ang::gender::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<T&, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<T&&, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "&&"_sv;
						return ang::move(out);
					}(),
					ang::gender::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<const T&, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "const "_sv << rtti::type_of<type>().type_name() << "&"_sv;
						return ang::move(out);
					}(),
					ang::gender::class_type, rtti::type_of<type>().size(), rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<intf_wrapper<T>, false> {
			static rtti_t const& type_of() { return ang::type_of<T>(); }
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<object_wrapper<T>, false> {
			static rtti_t const& type_of() { return ang::type_of<T>(); }
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<value<T>, false> {
			static rtti_t const& type_of() { return value<T>::class_info(); }
		};

		template<typename T>
		struct __type_info_builder_gender_class_helper<array_view<T>, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<type>().type_name() << "[]"_sv;
						return ang::move(out);
					}()
					, ang::gender::class_type
					, rtti::type_of<type>().size()
					, rtti::type_of<type>().aligment());
				return info;
			}
		};

		template<typename T, text::encoding E>
		struct __type_info_builder_gender_class_helper<str_view<T, E>, false> {
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << "str<"_sv << rtti::type_of<T>().type_name() << text::encoding_t(E).to_string() << ">"_sv;
						return ang::move(out);
					}()
					, ang::gender::class_type
					, size_of<str_view<T, E>>()
					, align_of<str_view<T, E>>());
				return info;
			}
		};

		template<typename K, typename T>
		struct __type_info_builder_gender_class_helper<collections::pair<K,T>, false> {
			static rtti_t const& type_of() {
				typedef typename remove_reference<typename remove_const<T>::type>::type type;
				rtti_t const& info = rtti::regist(
						[]()->fast_astring_t {
						fast_astring_t out;
						out << "ang::collections::pair<"_sv << type_pack<K,T>() << ">"_sv;
						return ang::move(out);
					}()
					, ang::gender::class_type
					, size_of<collections::pair<K,T>>()
					, align_of<collections::pair<K, T>>());
				return info;
			}
		};

		template<typename...Ts>
		struct __type_info_builder_gender_class_helper<collections::tuple<Ts...>, false> {
			static rtti_t const& type_of() {
				typedef collections::tuple<Ts...> type;
				rtti_t const& info = rtti::regist(
						[]()->fast_astring_t {
						fast_astring_t out;
						out << "ang::collections::tuple<"_sv << type_pack<Ts...>() << ">"_sv;
						return ang::move(out);
					}()
					, ang::gender::class_type
					, size_of<collections::tuple<Ts...>>()
					, align_of<collections::tuple<Ts...>>());
				return info;
			}
		};

		//////////////////////////////////////////////////////////////////////////////////////

		template<typename T> struct __type_info_builder_gender_function_helper;

		template<typename T, typename... Ts>
		struct __type_info_builder_gender_function_helper<T(Ts...)> { 
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<T>().type_name() << "("_sv
							<< type_pack<Ts...>() << ")"_sv;
						return ang::move(out);
					}(),
					gender::function_type, sizeof(wsize), alignof(wsize));
				return info;
			} 
		};

		template<typename T, typename... Ts>
		struct __type_info_builder_gender_function_helper<T(*)(Ts...)> {
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<T>().type_name() << "(*)("_sv
							<< type_pack<Ts...>() << ")"_sv;
						return ang::move(out);
					}(),
					gender::function_type, sizeof(wsize), alignof(wsize));
				return info;
			}
		};

#if defined WINDOWS_PLATFORM && defined _X86_
		template<typename T, typename... Ts>
		struct __type_info_builder_gender_function_helper<T(__stdcall*)(Ts...)> {
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<T>().type_name() << "(__stdcall*)("_sv
							<< type_pack<Ts...>() << ")"_sv;
						return ang::move(out);
					}(),
					gender::function_type, sizeof(wsize), alignof(wsize));
				return info;
			}
		};

#endif


		template<typename T, typename O, typename... Ts>
		struct __type_info_builder_gender_function_helper<T(O::*)(Ts...)> {
			static rtti_t const& type_of() {
				rtti_t const& info = rtti::regist(
					[]()->fast_astring_t {
						fast_astring_t out;
						out << rtti::type_of<T>().type_name() << "("_sv
							<< rtti::type_of<O>().type_name() << "::*)("_sv
							<< type_pack<Ts...>() << ")"_sv;
						return ang::move(out);
					}(),
					gender::function_type, sizeof(wsize), alignof(wsize));
				return info;
			}
		};

	}
}


template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::gender::value_type>::type_of() {
	return __type_info_builder_gender_value_helper<T>::type_of();
}

template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::gender::union_type>::type_of() {
	return __type_info_builder_gender_union_helper<T>::type_of();
}

template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::gender::enum_type>::type_of() {
	return __type_info_builder_gender_enum_helper<T>::type_of();
}

template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::gender::class_type>::type_of() {
	return __type_info_builder_gender_class_helper<T>::type_of();
}


template<typename T> ang::rtti_t const& ang::runtime::type_info_builder<T, ang::gender::function_type>::type_of() {
	return __type_info_builder_gender_function_helper<T>::type_of();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename...Ts>
inline ang::inherit_pack_info_t ang::rtti_from_pack<T, ang::type_pack<Ts...>>::types() {
	static inherit_info_t s_array[type_pack_size<wsize, type_pack<Ts...>>::value]
		= { inherit_info_t(&runtime::type_of<Ts>(), offset_of<T, Ts>())... };
	return to_array(s_array);
}


template<typename T>
inline ang::rtti_t const& ang::intf_class_info<T>::class_info()
{
	using _iid_type = typename T::iid_type;
	rtti_t const& info = rtti::regist(
		_iid_type::type_name()
		, gender::class_type
		, size_of<T>()
		, align_of<T>()
		, rtti_from_type<T>::types()
		, &iintf::default_query_interface
	);
	return info;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif//__ANG_BASE_RUNTIME_INL__
