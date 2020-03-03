

runtime::rtti_t const& value<MY_TYPE, gender_of<MY_TYPE>()>::class_info() {
	return ang::type_of<MY_TYPE>();
}

value<MY_TYPE, gender_of<MY_TYPE>()>::value()
	: m_value(default_value<MY_TYPE>::value)
{
}

value<MY_TYPE, gender_of<MY_TYPE>()>::value(MY_TYPE const& v)
	: m_value(v)
{
}

value<MY_TYPE, gender_of<MY_TYPE>()>::value(value const& v)
	: m_value(v.m_value)
{

}

value<MY_TYPE, gender_of<MY_TYPE>()>::value(MY_TYPE && v)
	: m_value(ang::move(v))
{
	v = default_value<MY_TYPE>::value;
}

value<MY_TYPE, gender_of<MY_TYPE>()>::value(value && v)
	: m_value(ang::move(v.m_value))
{
	v.m_value = default_value<MY_TYPE>::value;
}

value<MY_TYPE, gender_of<MY_TYPE>()>::operator MY_TYPE& ()
{ 
	return get();
}

value<MY_TYPE, gender_of<MY_TYPE>()>::operator MY_TYPE const& ()const 
{
	return get();
}

MY_TYPE* value<MY_TYPE, gender_of<MY_TYPE>()>::operator& ()
{
	return &get();
}

MY_TYPE const* value<MY_TYPE, gender_of<MY_TYPE>()>::operator& ()const
{
	return &get();
}

value<MY_TYPE, gender_of<MY_TYPE>()>& value<MY_TYPE, gender_of<MY_TYPE>()>::operator = (MY_TYPE const& v) 
{
	m_value = v;
	return*this;
}

value<MY_TYPE, gender_of<MY_TYPE>()>& value<MY_TYPE, gender_of<MY_TYPE>()>::operator = (value const& v)
{
	m_value = v.m_value;
	return*this;
}

value<MY_TYPE, gender_of<MY_TYPE>()>& value<MY_TYPE, gender_of<MY_TYPE>()>::operator = (MY_TYPE && v)
{
	m_value = ang::move(v);
	v = default_value<MY_TYPE>::value;
	return*this;
}

value<MY_TYPE, gender_of<MY_TYPE>()>& value<MY_TYPE, gender_of<MY_TYPE>()>::operator = (value && v)
{
	m_value = ang::move(v.m_value);
	v.m_value = default_value<MY_TYPE>::value;
	return*this;
}

MY_TYPE& value<MY_TYPE, gender_of<MY_TYPE>()>::get()
{
	return m_value;
}

MY_TYPE const& value<MY_TYPE, gender_of<MY_TYPE>()>::get()const
{
	return m_value;
}

void value<MY_TYPE, gender_of<MY_TYPE>()>::set(MY_TYPE const& v) 
{ 
	m_value = v; 
}

void value<MY_TYPE, gender_of<MY_TYPE>()>::move(MY_TYPE && v) 
{
	m_value = ang::forward<MY_TYPE>(v);
	v = default_value<MY_TYPE>::value;
}
