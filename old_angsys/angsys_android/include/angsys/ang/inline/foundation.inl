#ifndef __ANG_FOUNDATION_H__
#error ...
#elif !defined __ANG_FOUNDATION_HPP__
#define __ANG_FOUNDATION_HPP__

//namespace ang
//{
//	namespace runtime
//	{
//
//		template<typename T>
//		struct ang::runtime::runtime_type_builder<graphics::point<T>>
//		{
//			typedef ang::graphics::point<T> type;
//			static inline ang::type_name_t type_name() {
//				static string _type_name = ang::move(string("ang::graphics::point<") + runtime::type_name<T>() + ">"_s);
//				return (type_name_t)_type_name;
//			}
//			static inline bool is_type_of(ang::type_name_t name) {
//				return name == type_name();
//			}
//			static inline bool dynamic_constructor(ang::unknown_ptr_t out) {
//				if (out.get() == null)
//					return false;
//				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
//				*out = alloc->construct(alloc->object_alloc<type>(1));
//				return true;
//			}
//			static inline bool dynamic_destructor(ang::unknown_ptr_t uknown) {
//				if (uknown.get() == null)
//					return false;
//				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
//				alloc->memory_release(alloc->destruct(reinterpret_cast<type*>(*uknown.get())));
//				return true;
//			}
//			static inline ang::runtime_type_info_t runtime_type() {
//				static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true);
//				return runtime_type_info;
//			}
//			template<typename new_t> static inline new_t* interface_cast(type* _old) {
//				if (is_type_of(runtime::type_name<new_t>()))
//					return (new_t*)_old;
//				return null;
//			}
//		};
//
//		template<typename T>
//		struct ang::runtime::runtime_type_builder<graphics::size<T>>
//		{
//			typedef ang::graphics::size<T> type;
//			static inline ang::type_name_t type_name() {
//				static string _type_name = ang::move(string("ang::graphics::size<") + runtime::type_name<T>() + ">"_s);
//				return (type_name_t)_type_name;
//			}
//			static inline bool is_type_of(ang::type_name_t name) {
//				return name == type_name();
//			}
//			static inline bool dynamic_constructor(ang::unknown_ptr_t out) {
//				if (out.get() == null)
//					return false;
//				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
//				*out = alloc->construct(alloc->object_alloc<type>(1));
//				return true;
//			}
//			static inline bool dynamic_destructor(ang::unknown_ptr_t uknown) {
//				if (uknown.get() == null)
//					return false;
//				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
//				alloc->memory_release(alloc->destruct(reinterpret_cast<type*>(*uknown.get())));
//				return true;
//			}
//			static inline ang::runtime_type_info_t runtime_type() {
//				static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true);
//				return runtime_type_info;
//			}
//			template<typename new_t> static inline new_t* interface_cast(type* _old) {
//				if (is_type_of(runtime::type_name<new_t>()))
//					return (new_t*)_old;
//				return null;
//			}
//		};
//
//		template<typename T>
//		struct ang::runtime::runtime_type_builder<graphics::rect<T>>
//		{
//			typedef ang::graphics::rect<T> type;
//			static inline ang::type_name_t type_name() {
//				static string _type_name = ang::move(string("ang::graphics::rect<") + runtime::type_name<T>() + ">"_s);
//				return (type_name_t)_type_name;
//			}
//			static inline bool is_type_of(ang::type_name_t name) {
//				return name == type_name();
//			}
//			static inline bool dynamic_constructor(ang::unknown_ptr_t out) {
//				if (out.get() == null)
//					return false;
//				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
//				*out = alloc->construct(alloc->object_alloc<type>(1));
//				return true;
//			}
//			static inline bool dynamic_destructor(ang::unknown_ptr_t uknown) {
//				if (uknown.get() == null)
//					return false;
//				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
//				alloc->memory_release(alloc->destruct(reinterpret_cast<type*>(*uknown.get())));
//				return true;
//			}
//			static inline ang::runtime_type_info_t runtime_type() {
//				static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true);
//				return runtime_type_info;
//			}
//			template<typename new_t> static inline new_t* interface_cast(type* _old) {
//				if (is_type_of(runtime::type_name<new_t>()))
//					return (new_t*)_old;
//				return null;
//			}
//		};
//
//	}
//}


template<typename T> inline ang::graphics::point<T> ang::graphics::operator + (const ang::graphics::point<T>& a1, const ang::graphics::point<T>& a2) { return{ a1.x + a2.x, a1.y + a2.x }; }
template<typename T> inline ang::graphics::point<T> ang::graphics::operator - (const ang::graphics::point<T>& a1, const ang::graphics::point<T>& a2) { return{ a1.x - a2.x, a1.y - a2.x }; }
template<typename T> inline ang::graphics::point<T> ang::graphics::operator * (const ang::graphics::point<T>& a1, T k) { return{ a1.x * k, a1.y * k }; }
template<typename T> inline ang::graphics::point<T> ang::graphics::operator * (T k, const ang::graphics::point<T>& a2) { return{ a2.x * k, a2.y * k }; }
template<typename T> inline ang::graphics::point<T> ang::graphics::operator / (const ang::graphics::point<T>& a1, T k) { return{ a1.x / k, a1.y / k }; }

template<typename T> inline ang::graphics::size<T> ang::graphics::operator + (const ang::graphics::size<T>& a1, const ang::graphics::size<T>& a2) { return{ a1.width + a2.width, a1.height + a2.height }; }
template<typename T> inline ang::graphics::size<T> ang::graphics::operator - (const ang::graphics::size<T>& a1, const ang::graphics::size<T>& a2) { return{ a1.width - a2.width, a1.height - a2.height }; }
template<typename T> inline ang::graphics::size<T> ang::graphics::operator * (const ang::graphics::size<T>& a1, T k) { return{ a1.width * k, a1.height * k }; }
template<typename T> inline ang::graphics::size<T> ang::graphics::operator * (T k, const ang::graphics::size<T>& a2) { return{ a2.width * k, a2.height * k }; }
template<typename T> inline ang::graphics::size<T> ang::graphics::operator / (const ang::graphics::size<T>& a1, T k) { return{ a1.width / k, a1.height / k }; }

template<typename T> inline ang::graphics::rect<T> ang::graphics::operator + (const ang::graphics::point<T>& a1, const ang::graphics::size<T>& a2) { return{ a1,a2 }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator + (const ang::graphics::size<T>& a1, const ang::graphics::point<T>& a2) { return{ a2,a1 }; }

template<typename T> inline ang::graphics::rect<T> ang::graphics::operator + (const ang::graphics::rect<T>& a1, const ang::graphics::rect<T>& a2) { return{ a1.left + a2.left, a1.top + a2.top, a1.right + a2.right, a1.bottom + a2.bottom }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator - (const ang::graphics::rect<T>& a1, const ang::graphics::rect<T>& a2) { return{ a1.left - a2.left, a1.top - a2.top, a1.right - a2.right, a1.bottom - a2.bottom }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator + (const ang::graphics::rect<T>& a1, const ang::graphics::point<T>& a2) { return{ a1.left + a2.x, a1.top + a2.y, a1.right + a2.x, a1.bottom + a2.y }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator - (const ang::graphics::rect<T>& a1, const ang::graphics::point<T>& a2) { return{ a1.left - a2.x, a1.top - a2.y, a1.right - a2.x, a1.bottom - a2.y }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator + (const ang::graphics::point<T>& a1, const ang::graphics::rect<T>& a2) { return{ a2.left + a1.x, a2.top + a1.y, a2.right + a1.x, a2.bottom + a1.y }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator + (const ang::graphics::rect<T>& a1, const ang::graphics::size<T>& a2) { return{ a1.left, a1.top, a1.right + a2.width, a1.bottom + a2.height }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator - (const ang::graphics::rect<T>& a1, const ang::graphics::size<T>& a2) { return{ a1.left, a1.top, a1.right - a2.width, a1.bottom - a2.height }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator + (const ang::graphics::size<T>& a1, const ang::graphics::rect<T>& a2) { return{ a2.left, a2.top, a2.right + a1.width, a2.bottom + a1.height }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator * (const ang::graphics::rect<T>& a1, T k) { return{ a1.left * k, a1.top * k, a1.right * k, a1.bottom * k }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator * (T k, const ang::graphics::rect<T>& a2) { return{ a2.left * k, a2.top * k, a2.right * k, a2.bottom * k }; }
template<typename T> inline ang::graphics::rect<T> ang::graphics::operator / (const ang::graphics::rect<T>& a1, T k) { return{ a1.left / k, a1.top / k, a1.right / k, a1.bottom / k }; }

#endif//__ANG_FOUNDATION_HPP__