#ifndef __ANG_DISPLAY_H__
#error ...
#elif !defined __ANG_DISPLAY_INL__
#define __ANG_DISPLAY_INL__


template<typename T> 
inline ang::platform::point<T>::point()
	: x(0)
	, y(0)
{
}

template<typename T> 
inline ang::platform::point<T>::point(point const& p)
	: x(p.x)
	, y(p.y)
{
}

template<typename T> 
inline ang::platform::point<T>::point(T x, T y)
	: x(x)
	, y(y)
{
}

template<typename T> 
inline ang::platform::point<T>& ang::platform::point<T>::operator = (const ang::platform::point<T>& p)
{
	x = p.x;
	y = p.y;
	return*this;
}

template<typename T> 
inline ang::platform::point<T>& ang::platform::point<T>::operator += (const ang::platform::point<T>& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

template<typename T> 
inline ang::platform::point<T>& ang::platform::point<T>::operator -= (const ang::platform::point<T>& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

template<typename T> 
inline ang::platform::point<T>& ang::platform::point<T>::operator *= (T k)
{
	x *= k;
	y *= k;
	return *this;
}

template<typename T> 
inline ang::platform::point<T>& ang::platform::point<T>::operator /= (T k)
{
	x /= k;
	y /= k;
	return *this;
}


template<typename T> 
inline ang::platform::size<T>::size()
	: width(0)
	, height(0)
{

}

template<typename T> 
inline ang::platform::size<T>::size(size<T> const& b)
	: width(b.width)
	, height(b.height)
{

}

template<typename T> 
inline ang::platform::size<T>::size(T width, T height)
	: width(width)
	, height(height)
{

}

template<typename T> 
inline ang::platform::size<T>& ang::platform::size<T>::operator = (const ang::platform::size<T>& b)
{
	width = b.width;
	height = b.height;
	return*this;
}

template<typename T> 
inline ang::platform::size<T>& ang::platform::size<T>::operator += (const ang::platform::size<T>& b)
{
	width += b.width;
	height += b.height;
	return *this;
}

template<typename T> 
inline ang::platform::size<T>& ang::platform::size<T>::operator -= (const ang::platform::size<T>& b)
{
	width -= b.width;
	height -= b.height;
	return *this;
}

template<typename T> 
inline ang::platform::size<T>& ang::platform::size<T>::operator *= (T k)
{
	width *= k;
	height *= k;
	return *this;
}

template<typename T> 
inline ang::platform::size<T>& ang::platform::size<T>::operator /= (T k)
{
	width /= k;
	height /= k;
	return *this;
}



template<typename T>
inline ang::platform::rect<T>::rect()
	: left(0)
	, top(0)
	, right(0)
	, bottom(0)
{

}

template<typename T>
inline ang::platform::rect<T>::rect(ang::platform::rect<T> const& rc)
	: left(rc.left)
	, top(rc.top)
	, right(rc.right)
	, bottom(rc.bottom)
{

}

template<typename T>
inline ang::platform::rect<T>::rect(T l, T t, T r, T b)
	: left(l)
	, top(t)
	, right(r < l ? l : r)
	, bottom(b < t ? t : b)
{

}

template<typename T>
inline ang::platform::rect<T>::rect(ang::platform::point<T> lt, ang::platform::point<T> rb)
	: left(lt.x)
	, top(lt.y)
	, right(rb.x)
	, bottom(rb.y)
{

}

template<typename T>
inline ang::platform::rect<T>::rect(ang::platform::point<T> lt, ang::platform::size<T> sz)
	: left(lt.x)
	, top(lt.y)
	, right(lt.x + sz.width)
	, bottom(lt.y + sz.height)
{

}

template<typename T>
inline T ang::platform::rect<T>::width()const { return right - left; }

template<typename T>
inline T ang::platform::rect<T>::height()const { return bottom - top; }

template<typename T>
inline ang::platform::point<T> ang::platform::rect<T>::left_top()const { return{ left, top }; }

template<typename T>
inline ang::platform::point<T> ang::platform::rect<T>::right_top()const { return{ right, top }; }

template<typename T>
inline ang::platform::point<T> ang::platform::rect<T>::left_bottom()const { return{ left, bottom }; }

template<typename T>
inline ang::platform::point<T> ang::platform::rect<T>::right_bottom()const { return{ right, bottom }; }

template<typename T> 
inline void ang::platform::rect<T>::move_to(ang::platform::point<T> p)
{
	right = right - left + p.x;
	bottom = bottom - top + p.y;
	left = p.x;
	top = p.y;
}

template<typename T>
inline void ang::platform::rect<T>::move_to(T x, T y)
{
	right = right - left + x;
	bottom = bottom - top + y;
	left = x;
	top = y;
}

template<typename T>
inline void ang::platform::rect<T>::resize(ang::platform::size<T> sz)
{
	right = left + sz.width;
	bottom = top + sz.height;
}

template<typename T>
inline void ang::platform::rect<T>::resize(T cx, T cy)
{
	right = left + cx;
	bottom = top + cy;
}

template<typename T>
inline void ang::platform::rect<T>::deflate(T l, T t, T r, T b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

template<typename T>
inline void ang::platform::rect<T>::inflate(T l, T t, T r, T b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

template<typename T>
inline ang::platform::rect<T>& ang::platform::rect<T>::operator = (const ang::platform::rect<T>& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
	return *this;
}

template<typename T>
inline ang::platform::rect<T>& ang::platform::rect<T>::operator += (const ang::platform::rect<T>& rc)
{
	left += rc.left;
	top += rc.top;
	right += rc.right;
	bottom += rc.bottom;
	return *this;
}

template<typename T>
inline ang::platform::rect<T>& ang::platform::rect<T>::operator -= (const ang::platform::rect<T>& rc)
{
	left -= rc.left;
	top -= rc.top;
	right -= rc.right;
	bottom -= rc.bottom;
	return *this;
}

template<typename T>
inline ang::platform::rect<T>& ang::platform::rect<T>::operator += (const ang::platform::point<T>& pt)
{
	left += pt.x;
	top += pt.y;
	right += pt.x;
	bottom += pt.y;
	return *this;
}

template<typename T>
inline ang::platform::rect<T>& ang::platform::rect<T>::operator -= (const ang::platform::point<T>& pt)
{
	left -= pt.x;
	top -= pt.y;
	right -= pt.x;
	bottom -= pt.y;
	return *this;
}

template<typename T>
inline ang::platform::rect<T>& ang::platform::rect<T>::operator += (const ang::platform::size<T>& sz)
{
	right += sz.width;
	bottom += sz.height;
	return *this;
}

template<typename T> 
inline ang::platform::rect<T>& ang::platform::rect<T>::operator -= (const ang::platform::size<T>& sz)
{
	right -= sz.width;
	bottom -= sz.height;
	if (left > right) right = left;
	if (top > bottom) bottom = top;
	return *this;
}

template<typename T> 
inline ang::platform::rect<T>& ang::platform::rect<T>::operator *= (T k)
{
	left *= k;
	top *= k;
	right *= k;
	bottom *= k;
	return *this;
}

template<typename T> 
inline ang::platform::rect<T>& ang::platform::rect<T>::operator /= (T k)
{
	left /= k;
	top /= k;
	right /= k;
	bottom /= k;
	return *this;
}


template<typename T> inline ang::platform::point<T> ang::platform::operator + (const ang::platform::point<T>& a1, const ang::platform::point<T>& a2) { return{ a1.x + a2.x, a1.y + a2.x }; }
template<typename T> inline ang::platform::point<T> ang::platform::operator - (const ang::platform::point<T>& a1, const ang::platform::point<T>& a2) { return{ a1.x - a2.x, a1.y - a2.x }; }
template<typename T> inline ang::platform::point<T> ang::platform::operator * (const ang::platform::point<T>& a1, T k) { return{ a1.x * k, a1.y * k }; }
template<typename T> inline ang::platform::point<T> ang::platform::operator * (T k, const ang::platform::point<T>& a2) { return{ a2.x * k, a2.y * k }; }
template<typename T> inline ang::platform::point<T> ang::platform::operator / (const ang::platform::point<T>& a1, T k) { return{ a1.x / k, a1.y / k }; }

template<typename T> inline ang::platform::size<T> ang::platform::operator + (const ang::platform::size<T>& a1, const ang::platform::size<T>& a2) { return{ a1.width + a2.width, a1.height + a2.height }; }
template<typename T> inline ang::platform::size<T> ang::platform::operator - (const ang::platform::size<T>& a1, const ang::platform::size<T>& a2) { return{ a1.width - a2.width, a1.height - a2.height }; }
template<typename T> inline ang::platform::size<T> ang::platform::operator * (const ang::platform::size<T>& a1, T k) { return{ a1.width * k, a1.height * k }; }
template<typename T> inline ang::platform::size<T> ang::platform::operator * (T k, const ang::platform::size<T>& a2) { return{ a2.width * k, a2.height * k }; }
template<typename T> inline ang::platform::size<T> ang::platform::operator / (const ang::platform::size<T>& a1, T k) { return{ a1.width / k, a1.height / k }; }

template<typename T> inline ang::platform::rect<T> ang::platform::operator + (const ang::platform::point<T>& a1, const ang::platform::size<T>& a2) { return{ a1,a2 }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator + (const ang::platform::size<T>& a1, const ang::platform::point<T>& a2) { return{ a2,a1 }; }

template<typename T> inline ang::platform::rect<T> ang::platform::operator + (const ang::platform::rect<T>& a1, const ang::platform::rect<T>& a2) { return{ a1.left + a2.left, a1.top + a2.top, a1.right + a2.right, a1.bottom + a2.bottom }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator - (const ang::platform::rect<T>& a1, const ang::platform::rect<T>& a2) { return{ a1.left - a2.left, a1.top - a2.top, a1.right - a2.right, a1.bottom - a2.bottom }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator + (const ang::platform::rect<T>& a1, const ang::platform::point<T>& a2) { return{ a1.left + a2.x, a1.top + a2.y, a1.right + a2.x, a1.bottom + a2.y }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator - (const ang::platform::rect<T>& a1, const ang::platform::point<T>& a2) { return{ a1.left - a2.x, a1.top - a2.y, a1.right - a2.x, a1.bottom - a2.y }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator + (const ang::platform::point<T>& a1, const ang::platform::rect<T>& a2) { return{ a2.left + a1.x, a2.top + a1.y, a2.right + a1.x, a2.bottom + a1.y }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator + (const ang::platform::rect<T>& a1, const ang::platform::size<T>& a2) { return{ a1.left, a1.top, a1.right + a2.width, a1.bottom + a2.height }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator - (const ang::platform::rect<T>& a1, const ang::platform::size<T>& a2) { return{ a1.left, a1.top, a1.right - a2.width, a1.bottom - a2.height }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator + (const ang::platform::size<T>& a1, const ang::platform::rect<T>& a2) { return{ a2.left, a2.top, a2.right + a1.width, a2.bottom + a1.height }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator * (const ang::platform::rect<T>& a1, T k) { return{ a1.left * k, a1.top * k, a1.right * k, a1.bottom * k }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator * (T k, const ang::platform::rect<T>& a2) { return{ a2.left * k, a2.top * k, a2.right * k, a2.bottom * k }; }
template<typename T> inline ang::platform::rect<T> ang::platform::operator / (const ang::platform::rect<T>& a1, T k) { return{ a1.left / k, a1.top / k, a1.right / k, a1.bottom / k }; }

#endif//__ANG_DISPLAY_INL__