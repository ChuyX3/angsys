#ifndef __COFFE_FOUNDATIONS_H__
#error ...
#elif !defined __COFFE_FOUNDATIONS_INL__
#define __COFFE_FOUNDATIONS_INL__

template<typename T>
inline coffe::graphics::point<T> coffe::graphics::point<T>::parse(coffe::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0 };
	text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
	windex i = 0;
	T temp[2] = { 0,0 };
	parser->seek(cstr, i, " [ "_s);
	temp[0] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[1] = parser->to_value<T>(cstr, i);
	//parser->seek(cstr, i, " ] "_s);
	return{ temp[0], temp[1] };
}

template<typename T> 
inline coffe::graphics::point<T>::point()
	: x(0)
	, y(0)
{
}

template<typename T> 
inline coffe::graphics::point<T>::point(point const& p)
	: x(p.x)
	, y(p.y)
{
}

template<typename T> 
inline coffe::graphics::point<T>::point(T x, T y)
	: x(x)
	, y(y)
{
}

template<typename T> 
inline coffe::graphics::point<T>& coffe::graphics::point<T>::operator = (const coffe::graphics::point<T>& p)
{
	x = p.x;
	y = p.y;
	return*this;
}

template<typename T> 
inline coffe::graphics::point<T>& coffe::graphics::point<T>::operator += (const coffe::graphics::point<T>& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

template<typename T> 
inline coffe::graphics::point<T>& coffe::graphics::point<T>::operator -= (const coffe::graphics::point<T>& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

template<typename T> 
inline coffe::graphics::point<T>& coffe::graphics::point<T>::operator *= (T k)
{
	x *= k;
	y *= k;
	return *this;
}

template<typename T> 
inline coffe::graphics::point<T>& coffe::graphics::point<T>::operator /= (T k)
{
	x /= k;
	y /= k;
	return *this;
}



template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::point3d<T>::parse(coffe::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0,0 };
	text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
	windex i = 0;
	T temp[3] = { 0,0,0 };
	parser->seek(cstr, i, " [ "_s);
	temp[0] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[1] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[2] = parser->to_value<T>(cstr, i);
	//parser->seek(cstr, i, " ] "_s);
	return{ temp[0], temp[1] ,temp[2] };
}

template<typename T>
inline coffe::graphics::point3d<T>::point3d()
	: x(0)
	, y(0)
	, z(0)
{
}

template<typename T>
inline coffe::graphics::point3d<T>::point3d(point3d const& p)
	: x(p.x)
	, y(p.y)
	, z(p.z)
{
}

template<typename T>
inline coffe::graphics::point3d<T>::point3d(T x, T y, T z)
	: x(x)
	, y(y)
	, z(z)
{
}

template<typename T>
inline coffe::graphics::point3d<T>& coffe::graphics::point3d<T>::operator = (const coffe::graphics::point3d<T>& p)
{
	x = p.x;
	y = p.y;
	z = p.z;
	return*this;
}

template<typename T>
inline coffe::graphics::point3d<T>& coffe::graphics::point3d<T>::operator += (const coffe::graphics::point3d<T>& p)
{
	x += p.x;
	y += p.y;
	z += p.z;
	return *this;
}

template<typename T>
inline coffe::graphics::point3d<T>& coffe::graphics::point3d<T>::operator -= (const coffe::graphics::point3d<T>& p)
{
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}

template<typename T>
inline coffe::graphics::point3d<T>& coffe::graphics::point3d<T>::operator *= (T k)
{
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

template<typename T>
inline coffe::graphics::point3d<T>& coffe::graphics::point3d<T>::operator /= (T k)
{
	x /= k;
	y /= k;
	z /= k;
	return *this;
}


template<typename T>
inline coffe::graphics::size<T> coffe::graphics::size<T>::parse(coffe::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0 };
	text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
	windex i = 0;
	T temp[2] = { 0,0 };
	parser->seek(cstr, i, " [ "_s);
	temp[0] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[1] = parser->to_value<T>(cstr, i);
	return{ temp[0], temp[1] };
}

template<typename T> 
inline coffe::graphics::size<T>::size()
	: width(0)
	, height(0)
{

}

template<typename T> 
inline coffe::graphics::size<T>::size(size<T> const& b)
	: width(b.width)
	, height(b.height)
{

}

template<typename T> 
inline coffe::graphics::size<T>::size(T width, T height)
	: width(width)
	, height(height)
{

}

template<typename T> 
inline coffe::graphics::size<T>& coffe::graphics::size<T>::operator = (const coffe::graphics::size<T>& b)
{
	width = b.width;
	height = b.height;
	return*this;
}

template<typename T> 
inline coffe::graphics::size<T>& coffe::graphics::size<T>::operator += (const coffe::graphics::size<T>& b)
{
	width += b.width;
	height += b.height;
	return *this;
}

template<typename T> 
inline coffe::graphics::size<T>& coffe::graphics::size<T>::operator -= (const coffe::graphics::size<T>& b)
{
	width -= b.width;
	height -= b.height;
	return *this;
}

template<typename T> 
inline coffe::graphics::size<T>& coffe::graphics::size<T>::operator *= (T k)
{
	width *= k;
	height *= k;
	return *this;
}

template<typename T> 
inline coffe::graphics::size<T>& coffe::graphics::size<T>::operator /= (T k)
{
	width /= k;
	height /= k;
	return *this;
}



template<typename T>
inline coffe::graphics::size3d<T> coffe::graphics::size3d<T>::parse(coffe::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0,0 };
	text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
	windex i = 0;
	T temp[3] = { 0,0,0 };
	parser->seek(cstr, i, " [ "_s);
	temp[0] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[1] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[2] = parser->to_value<T>(cstr, i);
	return{ temp[0], temp[1], temp[2] };
}

template<typename T>
inline coffe::graphics::size3d<T>::size3d()
	: width(0)
	, height(0)
	, depth(0)
{

}

template<typename T>
inline coffe::graphics::size3d<T>::size3d(size3d<T> const& b)
	: width(b.width)
	, height(b.height)
	, depth(b.depth)
{

}

template<typename T>
inline coffe::graphics::size3d<T>::size3d(T width, T height, T depth)
	: width(width)
	, height(height)
	, depth(depth)
{

}

template<typename T>
inline coffe::graphics::size3d<T>& coffe::graphics::size3d<T>::operator = (const coffe::graphics::size3d<T>& b)
{
	width = b.width;
	height = b.height;
	depth = b.depth;
	return*this;
}

template<typename T>
inline coffe::graphics::size3d<T>& coffe::graphics::size3d<T>::operator += (const coffe::graphics::size3d<T>& b)
{
	width += b.width;
	height += b.height;
	depth += b.depth;
	return *this;
}

template<typename T>
inline coffe::graphics::size3d<T>& coffe::graphics::size3d<T>::operator -= (const coffe::graphics::size3d<T>& b)
{
	width -= b.width;
	height -= b.height;
	depth -= b.depth;
	return *this;
}

template<typename T>
inline coffe::graphics::size3d<T>& coffe::graphics::size3d<T>::operator *= (T k)
{
	width *= k;
	height *= k;
	depth *= k;
	return *this;
}

template<typename T>
inline coffe::graphics::size3d<T>& coffe::graphics::size3d<T>::operator /= (T k)
{
	width /= k;
	height /= k;
	depth /= k;
	return *this;
}



template<typename T>
inline coffe::graphics::rect<T> coffe::graphics::rect<T>::parse(coffe::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0,0,0 };
	text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
	windex i = 0;
	T temp[4] = { 0,0,0,0 };
	parser->seek(cstr, i, " [ "_s);
	temp[0] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[1] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[2] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[3] = parser->to_value<T>(cstr, i);
	return{ temp[0], temp[1], temp[2], temp[3] };
}

template<typename T>
inline coffe::graphics::rect<T>::rect()
	: left(0)
	, top(0)
	, right(0)
	, bottom(0)
{

}

template<typename T>
inline coffe::graphics::rect<T>::rect(coffe::graphics::rect<T> const& rc)
	: left(rc.left)
	, top(rc.top)
	, right(rc.right)
	, bottom(rc.bottom)
{

}

template<typename T>
inline coffe::graphics::rect<T>::rect(T l, T t, T r, T b)
	: left(l)
	, top(t)
	, right(r < l ? l : r)
	, bottom(b < t ? t : b)
{

}

template<typename T>
inline coffe::graphics::rect<T>::rect(coffe::graphics::point<T> lt, coffe::graphics::point<T> rb)
	: left(lt.x)
	, top(lt.y)
	, right(rb.x)
	, bottom(rb.y)
{

}

template<typename T>
inline coffe::graphics::rect<T>::rect(coffe::graphics::point<T> lt, coffe::graphics::size<T> sz)
	: left(lt.x)
	, top(lt.y)
	, right(lt.x + sz.width)
	, bottom(lt.y + sz.height)
{

}

template<typename T> 
inline T coffe::graphics::rect<T>::get_width_property(coffe::base_property const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::width, prop);
	return proxy->right - proxy->left;
}
template<typename T> 
inline void coffe::graphics::rect<T>::set_width_property(coffe::base_property*prop, T&& value) {
	rect<T>* proxy = field_to_parent(&rect<T>::width, prop);
	proxy->right = proxy->left + forward<T>(value);
}
template<typename T>
inline T coffe::graphics::rect<T>::get_height_property(coffe::base_property const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::height, prop);
	return proxy->bottom - proxy->top;
}
template<typename T> 
inline void coffe::graphics::rect<T>::set_height_property(coffe::base_property*prop, T&& value) {
	rect<T>* proxy = field_to_parent(&rect<T>::height, prop);
	proxy->bottom = proxy->top + forward<T>(value);
}
template<typename T>
inline coffe::graphics::point<T> coffe::graphics::rect<T>::get_left_top_property(coffe::base_property const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::left_top, prop);
	return point<T>(proxy->left, proxy->top);
}
template<typename T> 
inline coffe::graphics::point<T> coffe::graphics::rect<T>::get_right_top_property(coffe::base_property const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::right_top, prop);
	return point<T>(proxy->right, proxy->top);
}
template<typename T>
inline coffe::graphics::point<T> coffe::graphics::rect<T>::get_left_bottom_property(coffe::base_property const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::left_bottom, prop);
	return point<T>(proxy->left, proxy->bottom);
}
template<typename T>
inline coffe::graphics::point<T> coffe::graphics::rect<T>::get_right_bottom_property(coffe::base_property const*prop) {
	rect<T>* proxy = field_to_parent(&rect<T>::right_bottom, prop);
	return point<T>(proxy->right, proxy->bottom);
}

template<typename T> 
inline void coffe::graphics::rect<T>::move_to(coffe::graphics::point<T> p)
{
	right = right - left + p.x;
	bottom = bottom - top + p.y;
	left = p.x;
	top = p.y;
}

template<typename T>
inline void coffe::graphics::rect<T>::move_to(T x, T y)
{
	right = right - left + x;
	bottom = bottom - top + y;
	left = x;
	top = y;
}

template<typename T>
inline void coffe::graphics::rect<T>::resize(coffe::graphics::size<T> sz)
{
	right = left + sz.width;
	bottom = top + sz.height;
}

template<typename T>
inline void coffe::graphics::rect<T>::resize(T cx, T cy)
{
	right = left + cx;
	bottom = top + cy;
}

template<typename T>
inline void coffe::graphics::rect<T>::deflate(T l, T t, T r, T b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

template<typename T>
inline void coffe::graphics::rect<T>::inflate(T l, T t, T r, T b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

template<typename T>
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator = (const coffe::graphics::rect<T>& rc)
{
	left = rc.left;
	top = rc.top;
	right = rc.right;
	bottom = rc.bottom;
	return *this;
}

template<typename T>
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator += (const coffe::graphics::rect<T>& rc)
{
	left += rc.left;
	top += rc.top;
	right += rc.right;
	bottom += rc.bottom;
	return *this;
}

template<typename T>
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator -= (const coffe::graphics::rect<T>& rc)
{
	left -= rc.left;
	top -= rc.top;
	right -= rc.right;
	bottom -= rc.bottom;
	return *this;
}

template<typename T>
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator += (const coffe::graphics::point<T>& pt)
{
	left += pt.x;
	top += pt.y;
	right += pt.x;
	bottom += pt.y;
	return *this;
}

template<typename T>
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator -= (const coffe::graphics::point<T>& pt)
{
	left -= pt.x;
	top -= pt.y;
	right -= pt.x;
	bottom -= pt.y;
	return *this;
}

template<typename T>
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator += (const coffe::graphics::size<T>& sz)
{
	right += sz.width;
	bottom += sz.height;
	return *this;
}

template<typename T> 
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator -= (const coffe::graphics::size<T>& sz)
{
	right -= min(sz.width, left);
	bottom -= min(sz.height, top) ;
	return *this;
}

template<typename T> 
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator *= (T k)
{
	left *= k;
	top *= k;
	right *= k;
	bottom *= k;
	return *this;
}

template<typename T> 
inline coffe::graphics::rect<T>& coffe::graphics::rect<T>::operator /= (T k)
{
	left /= k;
	top /= k;
	right /= k;
	bottom /= k;
	return *this;
}



template<typename T>
inline coffe::graphics::box<T> coffe::graphics::box<T>::parse(coffe::cstr_t cstr)
{
	if (cstr.ptr() == null)
		return{ 0,0,0,0,0,0 };
	text::iparser_t parser = text::iparser::get_parser(cstr.encoding());
	windex i = 0;
	T temp[6] = { 0,0,0,0,0,0 };
	parser->seek(cstr, i, " [ "_s);
	temp[0] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[1] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[2] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[3] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[4] = parser->to_value<T>(cstr, i);
	parser->seek(cstr, i, " , "_s);
	temp[5] = parser->to_value<T>(cstr, i);
	return{ temp[0], temp[1], temp[2], temp[3], temp[4], temp[5] };
}

template<typename T>
inline coffe::graphics::box<T>::box()
	: left(0)
	, top(0)
	, front(0)
	, right(0)
	, bottom(0)
	, back(0)
{

}

template<typename T>
inline coffe::graphics::box<T>::box(coffe::graphics::box<T> const& bx)
	: left(bx.left)
	, top(bx.top)
	, front(bx.front)
	, right(bx.right)
	, bottom(bx.bottom)
	, back(bx.back)
{

}

template<typename T>
inline coffe::graphics::box<T>::box(T l, T t, T f, T r, T b, T bk)
	: left(l)
	, top(t)
	, front(f)
	, right(r < l ? l : r)
	, bottom(b < t ? t : b)
	, back(bk < f ? f : bk)
{

}

template<typename T>
inline coffe::graphics::box<T>::box(coffe::graphics::point3d<T> ltf, coffe::graphics::point3d<T> rbb)
	: left(ltf.x)
	, top(ltf.y)
	, front(ltf.z)
	, right(rbb.x)
	, bottom(rbb.y)
	, back(rbb.z)
{

}

template<typename T>
inline coffe::graphics::box<T>::box(coffe::graphics::point3d<T> ltf, coffe::graphics::size3d<T> sz)
	: left(ltf.x)
	, top(ltf.y)
	, front(ltf.z)
	, right(ltf.x + sz.width)
	, bottom(ltf.y + sz.height)
	, back(ltf.z + sz.depth)
{

}

template<typename T>
inline T coffe::graphics::box<T>::get_width_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::width, prop);
	return proxy->right - proxy->left;
}
template<typename T>
inline void coffe::graphics::box<T>::set_width_property(coffe::base_property *prop, T&& value) {
	box<T>* proxy = field_to_parent(&box<T>::width, prop);
	proxy->right = proxy->left + forward<T>(value);
}
template<typename T>
inline T coffe::graphics::box<T>::get_height_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::height, prop);
	return proxy->bottom - proxy->top;
}
template<typename T>
inline void coffe::graphics::box<T>::set_height_property(coffe::base_property *prop, T&& value) {
	box<T>* proxy = field_to_parent(&box<T>::height, prop);
	proxy->bottom = proxy->top + forward<T>(value);
}
template<typename T>
inline T coffe::graphics::box<T>::get_depth_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::depth, prop);
	return proxy->back - proxy->front;
}
template<typename T>
inline void coffe::graphics::box<T>::set_depth_property(coffe::base_property *prop, T&& value) {
	box<T>* proxy = field_to_parent(&box<T>::depth, prop);
	proxy->back = proxy->front + forward<T>(value);
}
template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::box<T>::get_left_top_front_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::left_top_front, prop);
	return point3d<T>(proxy->left, proxy->top, proxy->front);
}
template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::box<T>::get_left_top_back_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::left_top_back, prop);
	return point3d<T>(proxy->left, proxy->top, proxy->back);
}
template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::box<T>::get_right_top_front_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::right_top_front, prop);
	return point3d<T>(proxy->right, proxy->top, proxy->front);
}
template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::box<T>::get_right_top_back_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::right_top_back, prop);
	return point3d<T>(proxy->right, proxy->top, proxy->back);
}
template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::box<T>::get_left_bottom_front_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::left_bottom_front, prop);
	return point3d<T>(proxy->left, proxy->bottom, proxy->front);
}
template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::box<T>::get_left_bottom_back_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::left_bottom_back, prop);
	return point3d<T>(proxy->left, proxy->bottom, proxy->back);
}
template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::box<T>::get_right_bottom_front_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::right_bottom_front, prop);
	return point3d<T>(proxy->right, proxy->bottom, proxy->front);
}
template<typename T>
inline coffe::graphics::point3d<T> coffe::graphics::box<T>::get_right_bottom_back_property(coffe::base_property const*prop) {
	box<T>* proxy = field_to_parent(&box<T>::right_bottom_back, prop);
	return point3d<T>(proxy->right, proxy->bottom, proxy->back);
}
template<typename T>
inline void coffe::graphics::box<T>::move_to(coffe::graphics::point3d<T> p)
{
	right = right - left + p.x;
	bottom = bottom - top + p.y;
	back = back - front + p.z;
	left = p.x;
	top = p.y;
	front = p.z;
}

template<typename T>
inline void coffe::graphics::box<T>::move_to(T x, T y, T z)
{
	right = right - left + x;
	bottom = bottom - top + y;
	back = back - front + z;
	left = x;
	top = y;
	front = z;
}

template<typename T>
inline void coffe::graphics::box<T>::resize(coffe::graphics::size3d<T> sz)
{
	right = left + sz.width;
	bottom = top + sz.height;
	back = front + sz.depth;
}

template<typename T>
inline void coffe::graphics::box<T>::resize(T cx, T cy, T cz)
{
	right = left + cx;
	bottom = top + cy;
	back = front + cz;
}

template<typename T>
inline void coffe::graphics::box<T>::deflate(T l, T t, T f, T r, T b, T bk)
{
	left += l;
	top += t;
	front += f;
	right -= r;
	bottom -= b;
	back -= bk;
}

template<typename T>
inline void coffe::graphics::box<T>::inflate(T l, T t, T f, T r, T b, T bk)
{
	left -= l;
	top -= t;
	front -= f;
	right += r;
	bottom += b;
	back += bk;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator = (const coffe::graphics::box<T>& bx)
{
	left = bx.left;
	top = bx.top;
	front = bx.front;
	right = bx.right;
	bottom = bx.bottom;
	back = bx.back;
	return *this;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator += (const coffe::graphics::box<T>& bx)
{
	left += bx.left;
	top += bx.top;
	front += bx.front;
	right += bx.right;
	bottom += bx.bottom;
	back += bx.back;
	return *this;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator -= (const coffe::graphics::box<T>& bx)
{
	left -= bx.left;
	top -= bx.top;
	front -= bx.front;
	right -= bx.right;
	bottom -= bx.bottom;
	back -= bx.back;
	return *this;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator += (const coffe::graphics::point3d<T>& pt)
{
	left += pt.x;
	top += pt.y;
	front += pt.z;
	right += pt.x;
	bottom += pt.y;
	back += pt.z;
	return *this;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator -= (const coffe::graphics::point3d<T>& pt)
{
	left -= pt.x;
	top -= pt.y;
	front -= pt.z;
	right -= pt.x;
	bottom -= pt.y;
	back -= pt.z;
	return *this;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator += (const coffe::graphics::size3d<T>& sz)
{
	right += sz.width;
	bottom += sz.height;
	back += sz.depth;
	return *this;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator -= (const coffe::graphics::size3d<T>& sz)
{
	right -= min(sz.width, left);
	bottom -= min(sz.height, top);
	back -= min(sz.depth, front);
	return *this;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator *= (T k)
{
	left *= k;
	top *= k;
	front *= k;
	right *= k;
	bottom *= k;
	back *= k;
	return *this;
}

template<typename T>
inline coffe::graphics::box<T>& coffe::graphics::box<T>::operator /= (T k)
{
	left /= k;
	top /= k;
	front /= k;
	right /= k;
	bottom /= k;
	back /= k;
	return *this;
}


template<typename T> inline coffe::graphics::point<T> coffe::graphics::operator + (const coffe::graphics::point<T>& a1, const coffe::graphics::point<T>& a2) { return{ a1.x + a2.x, a1.y + a2.y }; }
template<typename T> inline coffe::graphics::point<T> coffe::graphics::operator - (const coffe::graphics::point<T>& a1, const coffe::graphics::point<T>& a2) { return{ a1.x - a2.x, a1.y - a2.y }; }
template<typename T> inline coffe::graphics::point<T> coffe::graphics::operator * (const coffe::graphics::point<T>& a1, T k) { return{ a1.x * k, a1.y * k }; }
template<typename T> inline coffe::graphics::point<T> coffe::graphics::operator * (T k, const coffe::graphics::point<T>& a2) { return{ a2.x * k, a2.y * k }; }
template<typename T> inline coffe::graphics::point<T> coffe::graphics::operator / (const coffe::graphics::point<T>& a1, T k) { return{ a1.x / k, a1.y / k }; }

template<typename T> inline coffe::graphics::size<T> coffe::graphics::operator + (const coffe::graphics::size<T>& a1, const coffe::graphics::size<T>& a2) { return{ a1.width + a2.width, a1.height + a2.height }; }
template<typename T> inline coffe::graphics::size<T> coffe::graphics::operator - (const coffe::graphics::size<T>& a1, const coffe::graphics::size<T>& a2) { return{ a1.width - a2.width, a1.height - a2.height }; }
template<typename T> inline coffe::graphics::size<T> coffe::graphics::operator * (const coffe::graphics::size<T>& a1, T k) { return{ a1.width * k, a1.height * k }; }
template<typename T> inline coffe::graphics::size<T> coffe::graphics::operator * (T k, const coffe::graphics::size<T>& a2) { return{ a2.width * k, a2.height * k }; }
template<typename T> inline coffe::graphics::size<T> coffe::graphics::operator / (const coffe::graphics::size<T>& a1, T k) { return{ a1.width / k, a1.height / k }; }

template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator + (const coffe::graphics::point<T>& a1, const coffe::graphics::size<T>& a2) { return{ a1,a2 }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator + (const coffe::graphics::size<T>& a1, const coffe::graphics::point<T>& a2) { return{ a2,a1 }; }

template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator + (const coffe::graphics::rect<T>& a1, const coffe::graphics::rect<T>& a2) { return{ a1.left + a2.left, a1.top + a2.top, a1.right + a2.right, a1.bottom + a2.bottom }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator - (const coffe::graphics::rect<T>& a1, const coffe::graphics::rect<T>& a2) { return{ a1.left - a2.left, a1.top - a2.top, a1.right - a2.right, a1.bottom - a2.bottom }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator + (const coffe::graphics::rect<T>& a1, const coffe::graphics::point<T>& a2) { return{ a1.left + a2.x, a1.top + a2.y, a1.right + a2.x, a1.bottom + a2.y }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator - (const coffe::graphics::rect<T>& a1, const coffe::graphics::point<T>& a2) { return{ a1.left - a2.x, a1.top - a2.y, a1.right - a2.x, a1.bottom - a2.y }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator + (const coffe::graphics::point<T>& a1, const coffe::graphics::rect<T>& a2) { return{ a2.left + a1.x, a2.top + a1.y, a2.right + a1.x, a2.bottom + a1.y }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator + (const coffe::graphics::rect<T>& a1, const coffe::graphics::size<T>& a2) { return{ a1.left, a1.top, a1.right + a2.width, a1.bottom + a2.height }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator - (const coffe::graphics::rect<T>& a1, const coffe::graphics::size<T>& a2) { return{ a1.left, a1.top, a1.right - a2.width, a1.bottom - a2.height }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator + (const coffe::graphics::size<T>& a1, const coffe::graphics::rect<T>& a2) { return{ a2.left, a2.top, a2.right + a1.width, a2.bottom + a1.height }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator * (const coffe::graphics::rect<T>& a1, T k) { return{ a1.left * k, a1.top * k, a1.right * k, a1.bottom * k }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator * (T k, const coffe::graphics::rect<T>& a2) { return{ a2.left * k, a2.top * k, a2.right * k, a2.bottom * k }; }
template<typename T> inline coffe::graphics::rect<T> coffe::graphics::operator / (const coffe::graphics::rect<T>& a1, T k) { return{ a1.left / k, a1.top / k, a1.right / k, a1.bottom / k }; }



template<typename T> inline coffe::graphics::point3d<T> coffe::graphics::operator + (const coffe::graphics::point3d<T>& a1, const coffe::graphics::point3d<T>& a2) { return{ a1.x + a2.x, a1.y + a2.y, a1.z + a2.z }; }
template<typename T> inline coffe::graphics::point3d<T> coffe::graphics::operator - (const coffe::graphics::point3d<T>& a1, const coffe::graphics::point3d<T>& a2) { return{ a1.x - a2.x, a1.y - a2.y, a1.z - a2.z }; }
template<typename T> inline coffe::graphics::point3d<T> coffe::graphics::operator * (const coffe::graphics::point3d<T>& a1, T k) { return{ a1.x * k, a1.y * k, a1.z * k }; }
template<typename T> inline coffe::graphics::point3d<T> coffe::graphics::operator * (T k, const coffe::graphics::point3d<T>& a2) { return{ a2.x * k, a2.y * k, a2.z * k }; }
template<typename T> inline coffe::graphics::point3d<T> coffe::graphics::operator / (const coffe::graphics::point3d<T>& a1, T k) { return{ a1.x / k, a1.y / k, a1.z / k }; }

template<typename T> inline coffe::graphics::size3d<T> coffe::graphics::operator + (const coffe::graphics::size3d<T>& a1, const coffe::graphics::size3d<T>& a2) { return{ a1.width + a2.width, a1.height + a2.height, a1.depth + a2.depth }; }
template<typename T> inline coffe::graphics::size3d<T> coffe::graphics::operator - (const coffe::graphics::size3d<T>& a1, const coffe::graphics::size3d<T>& a2) { return{ a1.width - a2.width, a1.height - a2.height, a1.depth - a2.depth }; }
template<typename T> inline coffe::graphics::size3d<T> coffe::graphics::operator * (const coffe::graphics::size3d<T>& a1, T k) { return{ a1.width * k, a1.height * k, a1.depth * k }; }
template<typename T> inline coffe::graphics::size3d<T> coffe::graphics::operator * (T k, const coffe::graphics::size3d<T>& a2) { return{ a2.width * k, a2.height * k, a2.depth * k }; }
template<typename T> inline coffe::graphics::size3d<T> coffe::graphics::operator / (const coffe::graphics::size3d<T>& a1, T k) { return{ a1.width / k, a1.height / k, a1.depth / k }; }

template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator + (const coffe::graphics::point3d<T>& a1, const coffe::graphics::size3d<T>& a2) { return{ a1,a2 }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator + (const coffe::graphics::size3d<T>& a1, const coffe::graphics::point3d<T>& a2) { return{ a2,a1 }; }

template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator + (const coffe::graphics::box<T>& a1, const coffe::graphics::box<T>& a2) { return{ a1.left + a2.left, a1.top + a2.top, a1.front + a2.front, a1.right + a2.right, a1.bottom + a2.bottom, a1.back + a2.back }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator - (const coffe::graphics::box<T>& a1, const coffe::graphics::box<T>& a2) { return{ a1.left - a2.left, a1.top - a2.top, a1.front - a2.front, a1.right - a2.right, a1.bottom - a2.bottom, a1.back - a2.back }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator + (const coffe::graphics::box<T>& a1, const coffe::graphics::point3d<T>& a2) { return{ a1.left + a2.x, a1.top + a2.y, a1.front + a2.z, a1.right + a2.x, a1.bottom + a2.y, a1.back + a2.z }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator - (const coffe::graphics::box<T>& a1, const coffe::graphics::point3d<T>& a2) { return{ a1.left - a2.x, a1.top - a2.y, a1.front - a2.z, a1.right - a2.x, a1.bottom - a2.y, a1.back - a2.z }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator + (const coffe::graphics::point3d<T>& a1, const coffe::graphics::box<T>& a2) { return{ a2.left + a1.x, a2.top + a1.y, a2.front + a1.z, a2.right + a1.x, a2.bottom + a1.y, a2.back + a1.z }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator + (const coffe::graphics::box<T>& a1, const coffe::graphics::size3d<T>& a2) { return{ a1.left, a1.top, a1.front, a1.right + a2.width, a1.bottom + a2.height, a1.back + a2.depth }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator - (const coffe::graphics::box<T>& a1, const coffe::graphics::size3d<T>& a2) { return{ a1.left, a1.top, a1.front, a1.right - min(a2.width, a1.left), a1.bottom - min(a2.height,a1.top), a1.back - min(a2.depth, a1.front) }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator + (const coffe::graphics::size3d<T>& a1, const coffe::graphics::box<T>& a2) { return{ a2.left, a2.top, a2.front, a2.right + a1.width, a2.bottom + a1.height, a2.back + a1.depth }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator * (const coffe::graphics::box<T>& a1, T k) { return{ a1.left * k, a1.top * k, a1.front * k, a1.right * k, a1.bottom * k, a1.back * k }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator * (T k, const coffe::graphics::box<T>& a2) { return{ a2.left * k, a2.top * k, a2.front * k, a2.right * k, a2.bottom * k, a2.back * k }; }
template<typename T> inline coffe::graphics::box<T> coffe::graphics::operator / (const coffe::graphics::box<T>& a1, T k) { return{ a1.left / k, a1.top / k, a1.front / k, a1.right / k, a1.bottom / k, a1.back / k }; }

#endif//__COFFE_FOUNDATIONS_INL__
