#ifndef __ANG_FOUNDATION_H__
#error ...
#elif !defined __ANG_FOUNDATION_HPP__
#define __ANG_FOUNDATION_HPP__

namespace ang
{
	namespace runtime
	{

		template<typename T>
		struct runtime_type_builder<foundation::point<T>>
		{
			typedef ang::foundation::point<T> type;
			static inline ang::type_name_t type_name() {
				static string _type_name = ang::move("ang::foundation::point<"_o + runtime::type_name<T>() + ">"_s);
				return (type_name_t)_type_name;
			}
			static inline bool is_type_of(ang::type_name_t name) {
				return name == type_name();
			}
			static inline bool dynamic_constructor(ang::unknown_ptr_t out) {
				if (out.get() == null)
					return false;
				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
				*out = alloc->construct(alloc->object_alloc<type>(1));
				return true;
			}
			static inline bool dynamic_destructor(ang::unknown_ptr_t uknown) {
				if (uknown.get() == null)
					return false;
				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
				alloc->memory_release(alloc->destruct(reinterpret_cast<type*>(*uknown.get())));
				return true;
			}
			static inline ang::runtime_type_info_t runtime_type() {
				static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true);
				return runtime_type_info;
			}
			template<typename new_t> static inline new_t* interface_cast(type* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<typename T>
		struct runtime_type_builder<foundation::size<T>>
		{
			typedef ang::foundation::size<T> type;
			static inline ang::type_name_t type_name() {
				static string _type_name = ang::move("ang::foundation::size<"_o + runtime::type_name<T>() + ">"_s);
				return (type_name_t)_type_name;
			}
			static inline bool is_type_of(ang::type_name_t name) {
				return name == type_name();
			}
			static inline bool dynamic_constructor(ang::unknown_ptr_t out) {
				if (out.get() == null)
					return false;
				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
				*out = alloc->construct(alloc->object_alloc<type>(1));
				return true;
			}
			static inline bool dynamic_destructor(ang::unknown_ptr_t uknown) {
				if (uknown.get() == null)
					return false;
				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
				alloc->memory_release(alloc->destruct(reinterpret_cast<type*>(*uknown.get())));
				return true;
			}
			static inline ang::runtime_type_info_t runtime_type() {
				static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true);
				return runtime_type_info;
			}
			template<typename new_t> static inline new_t* interface_cast(type* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

		template<typename T>
		struct runtime_type_builder<foundation::rect<T>>
		{
			typedef ang::foundation::rect<T> type;
			static inline ang::type_name_t type_name() {
				static string _type_name = ang::move("ang::foundation::rect<"_o + runtime::type_name<T>() + ">"_s);
				return (type_name_t)_type_name;
			}
			static inline bool is_type_of(ang::type_name_t name) {
				return name == type_name();
			}
			static inline bool dynamic_constructor(ang::unknown_ptr_t out) {
				if (out.get() == null)
					return false;
				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
				*out = alloc->construct(alloc->object_alloc<type>(1));
				return true;
			}
			static inline bool dynamic_destructor(ang::unknown_ptr_t uknown) {
				if (uknown.get() == null)
					return false;
				auto alloc = memory::allocator_manager::get_allocator(memory::allocator_manager::default_allocator);
				alloc->memory_release(alloc->destruct(reinterpret_cast<type*>(*uknown.get())));
				return true;
			}
			static inline ang::runtime_type_info_t runtime_type() {
				static runtime_type_info_t runtime_type_info(type_name(), &dynamic_constructor, &dynamic_destructor, true);
				return runtime_type_info;
			}
			template<typename new_t> static inline new_t* interface_cast(type* _old) {
				if (is_type_of(runtime::type_name<new_t>()))
					return (new_t*)_old;
				return null;
			}
		};

	}
}


template<typename T> 
inline ang::foundation::point<T>::point()
	: x(0)
	, y(0)
{
}

template<typename T> 
inline ang::foundation::point<T>::point(point const& p)
	: x(p.x)
	, y(p.y)
{
}

template<typename T> 
inline ang::foundation::point<T>::point(T x, T y)
	: x(x)
	, y(y)
{
}

template<typename T> 
inline ang::foundation::point<T>& ang::foundation::point<T>::operator = (const ang::foundation::point<T>& p)
{
	x = p.x;
	y = p.y;
	return*this;
}

template<typename T> 
inline ang::foundation::point<T>& ang::foundation::point<T>::operator += (const ang::foundation::point<T>& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

template<typename T> 
inline ang::foundation::point<T>& ang::foundation::point<T>::operator -= (const ang::foundation::point<T>& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

template<typename T> 
inline ang::foundation::point<T>& ang::foundation::point<T>::operator *= (T k)
{
	x *= k;
	y *= k;
	return *this;
}

template<typename T> 
inline ang::foundation::point<T>& ang::foundation::point<T>::operator /= (T k)
{
	x /= k;
	y /= k;
	return *this;
}


template<typename T> 
inline ang::foundation::size<T>::size()
	: width(0)
	, height(0)
{

}

template<typename T> 
inline ang::foundation::size<T>::size(size<T> const& b)
	: width(b.width)
	, height(b.height)
{

}

template<typename T> 
inline ang::foundation::size<T>::size(T width, T height)
	: width(width)
	, height(height)
{

}

template<typename T> 
inline ang::foundation::size<T>& ang::foundation::size<T>::operator = (const ang::foundation::size<T>& b)
{
	width = b.width;
	height = b.height;
	return*this;
}

template<typename T> 
inline ang::foundation::size<T>& ang::foundation::size<T>::operator += (const ang::foundation::size<T>& b)
{
	width += b.width;
	height += b.height;
	return *this;
}

template<typename T> 
inline ang::foundation::size<T>& ang::foundation::size<T>::operator -= (const ang::foundation::size<T>& b)
{
	width -= b.width;
	height -= b.height;
	return *this;
}

template<typename T> 
inline ang::foundation::size<T>& ang::foundation::size<T>::operator *= (T k)
{
	width *= k;
	height *= k;
	return *this;
}

template<typename T> 
inline ang::foundation::size<T>& ang::foundation::size<T>::operator /= (T k)
{
	width /= k;
	height /= k;
	return *this;
}



template<typename T>
inline ang::foundation::rect<T>::rect()
	: left(0)
	, top(0)
	, right(0)
	, bottom(0)
{

}

template<typename T>
inline ang::foundation::rect<T>::rect(ang::foundation::rect<T> const& rc)
	: left(rc.left)
	, top(rc.top)
	, right(rc.right)
	, bottom(rc.bottom)
{

}

template<typename T>
inline ang::foundation::rect<T>::rect(T l, T t, T r, T b)
	: left(l)
	, top(t)
	, right(r < l ? l : r)
	, bottom(b < t ? t : b)
{

}

template<typename T>
inline ang::foundation::rect<T>::rect(ang::foundation::point<T> lt, ang::foundation::point<T> rb)
	: left(lt.x)
	, top(lt.y)
	, right(rb.x)
	, bottom(rb.y)
{

}

template<typename T>
inline ang::foundation::rect<T>::rect(ang::foundation::point<T> lt, ang::foundation::size<T> sz)
	: left(lt.x)
	, top(lt.y)
	, right(lt.x + sz.width)
	, bottom(lt.y + sz.height)
{

}

template<typename T>
inline T ang::foundation::rect<T>::width()const { return right - left; }

template<typename T>
inline T ang::foundation::rect<T>::height()const { return bottom - top; }

template<typename T>
inline ang::foundation::point<T> ang::foundation::rect<T>::left_top()const { return{ left, top }; }

template<typename T>
inline ang::foundation::point<T> ang::foundation::rect<T>::right_top()const { return{ right, top }; }

template<typename T>
inline ang::foundation::point<T> ang::foundation::rect<T>::left_bottom()const { return{ left, bottom }; }

template<typename T>
inline ang::foundation::point<T> ang::foundation::rect<T>::right_bottom()const { return{ right, bottom }; }

template<typename T> 
inline void ang::foundation::rect<T>::move_to(ang::foundation::point<T> p)
{
	right = right - left + p.x;
	bottom = bottom - top + p.y;
	left = p.x;
	top = p.y;
}

template<typename T>
inline void ang::foundation::rect<T>::move_to(T x, T y)
{
	right = right - left + x;
	bottom = bottom - top + y;
	left = x;
	top = y;
}

template<typename T>
inline void ang::foundation::rect<T>::resize(ang::foundation::size<T> sz)
{
	right = left + sz.width;
	bottom = top + sz.height;
}

template<typename T>
inline void ang::foundation::rect<T>::resize(T cx, T cy)
{
	right = left + cx;
	bottom = top + cy;
}

template<typename T>
inline void ang::foundation::rect<T>::deflate(T l, T t, T r, T b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

template<typename T>
inline void ang::foundation::rect<T>::inflate(T l, T t, T r, T b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

template<typename T>
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator = (const ang::foundation::rect<T>& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
	return *this;
}

template<typename T>
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator += (const ang::foundation::rect<T>& rc)
{
	left += rc.left;
	top += rc.top;
	right += rc.right;
	bottom += rc.bottom;
	return *this;
}

template<typename T>
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator -= (const ang::foundation::rect<T>& rc)
{
	left -= rc.left;
	top -= rc.top;
	right -= rc.right;
	bottom -= rc.bottom;
	return *this;
}

template<typename T>
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator += (const ang::foundation::point<T>& pt)
{
	left += pt.x;
	top += pt.y;
	right += pt.x;
	bottom += pt.y;
	return *this;
}

template<typename T>
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator -= (const ang::foundation::point<T>& pt)
{
	left -= pt.x;
	top -= pt.y;
	right -= pt.x;
	bottom -= pt.y;
	return *this;
}

template<typename T>
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator += (const ang::foundation::size<T>& sz)
{
	right += sz.width;
	bottom += sz.height;
	return *this;
}

template<typename T> 
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator -= (const ang::foundation::size<T>& sz)
{
	right -= sz.width;
	bottom -= sz.height;
	if (left > right) right = left;
	if (top > bottom) bottom = top;
	return *this;
}

template<typename T> 
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator *= (T k)
{
	left *= k;
	top *= k;
	right *= k;
	bottom *= k;
	return *this;
}

template<typename T> 
inline ang::foundation::rect<T>& ang::foundation::rect<T>::operator /= (T k)
{
	left /= k;
	top /= k;
	right /= k;
	bottom /= k;
	return *this;
}


template<typename T> inline ang::foundation::point<T> ang::foundation::operator + (const ang::foundation::point<T>& a1, const ang::foundation::point<T>& a2) { return{ a1.x + a2.x, a1.y + a2.x }; }
template<typename T> inline ang::foundation::point<T> ang::foundation::operator - (const ang::foundation::point<T>& a1, const ang::foundation::point<T>& a2) { return{ a1.x - a2.x, a1.y - a2.x }; }
template<typename T> inline ang::foundation::point<T> ang::foundation::operator * (const ang::foundation::point<T>& a1, T k) { return{ a1.x * k, a1.y * k }; }
template<typename T> inline ang::foundation::point<T> ang::foundation::operator * (T k, const ang::foundation::point<T>& a2) { return{ a2.x * k, a2.y * k }; }
template<typename T> inline ang::foundation::point<T> ang::foundation::operator / (const ang::foundation::point<T>& a1, T k) { return{ a1.x / k, a1.y / k }; }

template<typename T> inline ang::foundation::size<T> ang::foundation::operator + (const ang::foundation::size<T>& a1, const ang::foundation::size<T>& a2) { return{ a1.width + a2.width, a1.height + a2.height }; }
template<typename T> inline ang::foundation::size<T> ang::foundation::operator - (const ang::foundation::size<T>& a1, const ang::foundation::size<T>& a2) { return{ a1.width - a2.width, a1.height - a2.height }; }
template<typename T> inline ang::foundation::size<T> ang::foundation::operator * (const ang::foundation::size<T>& a1, T k) { return{ a1.width * k, a1.height * k }; }
template<typename T> inline ang::foundation::size<T> ang::foundation::operator * (T k, const ang::foundation::size<T>& a2) { return{ a2.width * k, a2.height * k }; }
template<typename T> inline ang::foundation::size<T> ang::foundation::operator / (const ang::foundation::size<T>& a1, T k) { return{ a1.width / k, a1.height / k }; }

template<typename T> inline ang::foundation::rect<T> ang::foundation::operator + (const ang::foundation::point<T>& a1, const ang::foundation::size<T>& a2) { return{ a1,a2 }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator + (const ang::foundation::size<T>& a1, const ang::foundation::point<T>& a2) { return{ a2,a1 }; }

template<typename T> inline ang::foundation::rect<T> ang::foundation::operator + (const ang::foundation::rect<T>& a1, const ang::foundation::rect<T>& a2) { return{ a1.left + a2.left, a1.top + a2.top, a1.right + a2.right, a1.bottom + a2.bottom }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator - (const ang::foundation::rect<T>& a1, const ang::foundation::rect<T>& a2) { return{ a1.left - a2.left, a1.top - a2.top, a1.right - a2.right, a1.bottom - a2.bottom }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator + (const ang::foundation::rect<T>& a1, const ang::foundation::point<T>& a2) { return{ a1.left + a2.x, a1.top + a2.y, a1.right + a2.x, a1.bottom + a2.y }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator - (const ang::foundation::rect<T>& a1, const ang::foundation::point<T>& a2) { return{ a1.left - a2.x, a1.top - a2.y, a1.right - a2.x, a1.bottom - a2.y }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator + (const ang::foundation::point<T>& a1, const ang::foundation::rect<T>& a2) { return{ a2.left + a1.x, a2.top + a1.y, a2.right + a1.x, a2.bottom + a1.y }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator + (const ang::foundation::rect<T>& a1, const ang::foundation::size<T>& a2) { return{ a1.left, a1.top, a1.right + a2.width, a1.bottom + a2.height }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator - (const ang::foundation::rect<T>& a1, const ang::foundation::size<T>& a2) { return{ a1.left, a1.top, a1.right - a2.width, a1.bottom - a2.height }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator + (const ang::foundation::size<T>& a1, const ang::foundation::rect<T>& a2) { return{ a2.left, a2.top, a2.right + a1.width, a2.bottom + a1.height }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator * (const ang::foundation::rect<T>& a1, T k) { return{ a1.left * k, a1.top * k, a1.right * k, a1.bottom * k }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator * (T k, const ang::foundation::rect<T>& a2) { return{ a2.left * k, a2.top * k, a2.right * k, a2.bottom * k }; }
template<typename T> inline ang::foundation::rect<T> ang::foundation::operator / (const ang::foundation::rect<T>& a1, T k) { return{ a1.left / k, a1.top / k, a1.right / k, a1.bottom / k }; }



#endif//__ANG_FOUNDATION_HPP__