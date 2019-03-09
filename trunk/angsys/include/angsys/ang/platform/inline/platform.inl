#ifndef __ANG_PLATFORM_H__
#error ...
#elif !defined __ANG_PLATFORM_GRAPHICS_INL__
#define __ANG_PLATFORM_GRAPHICS_INL__


template<typename T> 
inline ang::graphics::point<T>::point()
	: x(0)
	, y(0)
{
}

template<typename T> 
inline ang::graphics::point<T>::point(point const& p)
	: x(p.x)
	, y(p.y)
{
}

template<typename T> 
inline ang::graphics::point<T>::point(T x, T y)
	: x(x)
	, y(y)
{
}

template<typename T> 
inline ang::graphics::point<T>& ang::graphics::point<T>::operator = (const ang::graphics::point<T>& p)
{
	x = p.x;
	y = p.y;
	return*this;
}

template<typename T> 
inline ang::graphics::point<T>& ang::graphics::point<T>::operator += (const ang::graphics::point<T>& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

template<typename T> 
inline ang::graphics::point<T>& ang::graphics::point<T>::operator -= (const ang::graphics::point<T>& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

template<typename T> 
inline ang::graphics::point<T>& ang::graphics::point<T>::operator *= (T k)
{
	x *= k;
	y *= k;
	return *this;
}

template<typename T> 
inline ang::graphics::point<T>& ang::graphics::point<T>::operator /= (T k)
{
	x /= k;
	y /= k;
	return *this;
}


template<typename T> 
inline ang::graphics::size<T>::size()
	: width(0)
	, height(0)
{

}

template<typename T> 
inline ang::graphics::size<T>::size(size<T> const& b)
	: width(b.width)
	, height(b.height)
{

}

template<typename T> 
inline ang::graphics::size<T>::size(T width, T height)
	: width(width)
	, height(height)
{

}

template<typename T> 
inline ang::graphics::size<T>& ang::graphics::size<T>::operator = (const ang::graphics::size<T>& b)
{
	width = b.width;
	height = b.height;
	return*this;
}

template<typename T> 
inline ang::graphics::size<T>& ang::graphics::size<T>::operator += (const ang::graphics::size<T>& b)
{
	width += b.width;
	height += b.height;
	return *this;
}

template<typename T> 
inline ang::graphics::size<T>& ang::graphics::size<T>::operator -= (const ang::graphics::size<T>& b)
{
	width -= b.width;
	height -= b.height;
	return *this;
}

template<typename T> 
inline ang::graphics::size<T>& ang::graphics::size<T>::operator *= (T k)
{
	width *= k;
	height *= k;
	return *this;
}

template<typename T> 
inline ang::graphics::size<T>& ang::graphics::size<T>::operator /= (T k)
{
	width /= k;
	height /= k;
	return *this;
}



template<typename T>
inline ang::graphics::rect<T>::rect()
	: left(0)
	, top(0)
	, right(0)
	, bottom(0)
{

}

template<typename T>
inline ang::graphics::rect<T>::rect(ang::graphics::rect<T> const& rc)
	: left(rc.left)
	, top(rc.top)
	, right(rc.right)
	, bottom(rc.bottom)
{

}

template<typename T>
inline ang::graphics::rect<T>::rect(T l, T t, T r, T b)
	: left(l)
	, top(t)
	, right(r < l ? l : r)
	, bottom(b < t ? t : b)
{

}

template<typename T>
inline ang::graphics::rect<T>::rect(ang::graphics::point<T> lt, ang::graphics::point<T> rb)
	: left(lt.x)
	, top(lt.y)
	, right(rb.x)
	, bottom(rb.y)
{

}

template<typename T>
inline ang::graphics::rect<T>::rect(ang::graphics::point<T> lt, ang::graphics::size<T> sz)
	: left(lt.x)
	, top(lt.y)
	, right(lt.x + sz.width)
	, bottom(lt.y + sz.height)
{

}

template<typename T> 
inline T ang::graphics::rect<T>::get_width_property(ang::base_property<T>const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::width, prop);
	return proxy->right - proxy->left;
}
template<typename T> 
inline void ang::graphics::rect<T>::set_width_property(ang::base_property<T>*prop, T&& value) {
	rect<T>* proxy = field_to_parent(&rect<T>::width, prop);
	proxy->right = proxy->left + forward<T>(value);
}
template<typename T>
inline T ang::graphics::rect<T>::get_height_property(ang::base_property<T>const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::height, prop);
	return proxy->bottom - proxy->top;
}
template<typename T> 
inline void ang::graphics::rect<T>::set_height_property(ang::base_property<T>*prop, T&& value) {
	rect<T>* proxy = field_to_parent(&rect<T>::height, prop);
	proxy->bottom = proxy->top + forward<T>(value);
}
template<typename T>
inline ang::graphics::point<T> ang::graphics::rect<T>::get_left_top_property(ang::base_property<point<T>>const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::left_top, prop);
	return point<T>(proxy->left, proxy->top);
}
template<typename T> 
inline ang::graphics::point<T> ang::graphics::rect<T>::get_right_top_property(ang::base_property<point<T>>const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::right_top, prop);
	return point<T>(proxy->right, proxy->top);
}
template<typename T>
inline ang::graphics::point<T> ang::graphics::rect<T>::get_left_bottom_property(ang::base_property<point<T>>const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::left_bottom, prop);
	return point<T>(proxy->left, proxy->bottom);
}
template<typename T>
inline ang::graphics::point<T> ang::graphics::rect<T>::get_right_bottom_property(ang::base_property<point<T>>const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::right_bottom, prop);
	return point<T>(proxy->right, proxy->bottom);
}

template<typename T> 
inline void ang::graphics::rect<T>::move_to(ang::graphics::point<T> p)
{
	right = right - left + p.x;
	bottom = bottom - top + p.y;
	left = p.x;
	top = p.y;
}

template<typename T>
inline void ang::graphics::rect<T>::move_to(T x, T y)
{
	right = right - left + x;
	bottom = bottom - top + y;
	left = x;
	top = y;
}

template<typename T>
inline void ang::graphics::rect<T>::resize(ang::graphics::size<T> sz)
{
	right = left + sz.width;
	bottom = top + sz.height;
}

template<typename T>
inline void ang::graphics::rect<T>::resize(T cx, T cy)
{
	right = left + cx;
	bottom = top + cy;
}

template<typename T>
inline void ang::graphics::rect<T>::deflate(T l, T t, T r, T b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

template<typename T>
inline void ang::graphics::rect<T>::inflate(T l, T t, T r, T b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

template<typename T>
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator = (const ang::graphics::rect<T>& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
	return *this;
}

template<typename T>
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator += (const ang::graphics::rect<T>& rc)
{
	left += rc.left;
	top += rc.top;
	right += rc.right;
	bottom += rc.bottom;
	return *this;
}

template<typename T>
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator -= (const ang::graphics::rect<T>& rc)
{
	left -= rc.left;
	top -= rc.top;
	right -= rc.right;
	bottom -= rc.bottom;
	return *this;
}

template<typename T>
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator += (const ang::graphics::point<T>& pt)
{
	left += pt.x;
	top += pt.y;
	right += pt.x;
	bottom += pt.y;
	return *this;
}

template<typename T>
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator -= (const ang::graphics::point<T>& pt)
{
	left -= pt.x;
	top -= pt.y;
	right -= pt.x;
	bottom -= pt.y;
	return *this;
}

template<typename T>
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator += (const ang::graphics::size<T>& sz)
{
	right += sz.width;
	bottom += sz.height;
	return *this;
}

template<typename T> 
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator -= (const ang::graphics::size<T>& sz)
{
	right -= sz.width;
	bottom -= sz.height;
	if (left > right) right = left;
	if (top > bottom) bottom = top;
	return *this;
}

template<typename T> 
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator *= (T k)
{
	left *= k;
	top *= k;
	right *= k;
	bottom *= k;
	return *this;
}

template<typename T> 
inline ang::graphics::rect<T>& ang::graphics::rect<T>::operator /= (T k)
{
	left /= k;
	top /= k;
	right /= k;
	bottom /= k;
	return *this;
}


template<typename T> inline ang::graphics::point<T> ang::graphics::operator + (const ang::graphics::point<T>& a1, const ang::graphics::point<T>& a2) { return{ a1.x + a2.x, a1.y + a2.y }; }
template<typename T> inline ang::graphics::point<T> ang::graphics::operator - (const ang::graphics::point<T>& a1, const ang::graphics::point<T>& a2) { return{ a1.x - a2.x, a1.y - a2.y }; }
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

////////////////////////////////////////


//ang::events


template<typename F, typename A>
inline void ang::platform::events::static_event_function<F, A>::invoke(ang::objptr caller, ang::platform::events::imsg_event_args_t args)const
{
	_function(caller, static_cast<A*>(args.get()));
}

template<typename F, typename A>
inline void ang::platform::events::static_event_function<F, ang::intf_wrapper<A>>::invoke(ang::objptr caller, ang::platform::events::imsg_event_args_t args)const
{
	_function(caller, static_cast<A*>(args.get()));
}

template<typename T, bool IS_OBJECT, typename A>
inline void ang::platform::events::member_event_function<T, IS_OBJECT, A>::invoke(ang::objptr caller, ang::platform::events::imsg_event_args_t args)const
{
	(_obj->*_function)(caller, static_cast<A*>(args.get()));
}

template<typename T, typename A>
inline void ang::platform::events::member_event_function<T, true, A>::invoke(ang::objptr caller, ang::platform::events::imsg_event_args_t args)const
{
	auto lock = _obj.lock();
	if (lock != null)(lock->*_function)(caller, static_cast<A*>(args.get()));
}

#endif//__ANG_FOUNDATION_HPP__