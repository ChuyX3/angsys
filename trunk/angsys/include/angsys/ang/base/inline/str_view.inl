/*********************************************************************************************************************/
/*   File Name: ang/base/inline/str_view.inl                                                                         */
/*   Author: Ing. Jesus Rocha <chuyangel.rm@gmail.com>, July 2016.                                                   */
/*                                                                                                                   */
/*   Copyright (C) angsys, Jesus Angel Rocha Morales                                                                 */
/*   You may opt to use, copy, modify, merge, publish and/or distribute copies of the Software, and permit persons   */
/*   to whom the Software is furnished to do so.                                                                     */
/*   This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.      */
/*                                                                                                                   */
/*********************************************************************************************************************/

#ifndef __ANG_BASE_H__
#error ang/base/base.h is not included
#else


ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::str_view()
	: m_view(nullptr)
	, m_size(0)
{

}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::str_view(ang::nullptr_t const&)
	: m_view(nullptr)
	, m_size(0)
{

}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::str_view(MY_STR_TYPE str, wsize sz)
	: m_view(nullptr)
	, m_size(0)
{
	set(str, sz);
}

wsize ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::size()const

{
	return m_size;
}

MY_STR_TYPE ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::str()
{
	return m_view;
}

MY_CSTR_TYPE ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::cstr()const
{
	return m_view;
}

void ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::set(MY_STR_TYPE str, wsize sz)
{
	if (str) {
		m_view = str;
		m_size = (sz == wsize(-1)) ? m_size = text::encoder<ENCODING>::lenght(str) : sz;
	}
	else {
		m_view = nullptr;
		m_size = 0;
	}
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING> ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::begin()const
{
	return sefl_t(m_view, m_size);
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING> ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::end()const
{
	return sefl_t(m_view + m_size, 0);
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING>& ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator = (ang::str_view<MY_CHAR_TYPE, MY_ENCODING> const& str)
{
	set(str.m_view, str.m_size);
	return*this;
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING>& ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator = (MY_STR_TYPE str)
{
	set(str);
	return*this;
}

MY_CHAR_TYPE& ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator*()
{
	return *str();
}

MY_CHAR_TYPE const& ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator*()const
{
	return *cstr();
}

MY_CHAR_TYPE** ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator & ()
{
	return &m_view;
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator MY_STR_TYPE ()
{
	return str();
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator MY_CSTR_TYPE ()
{
	return cstr();
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING>& ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator ++ ()
{
	set(str() + 1, size() - 1);
	return *this;
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING> ang::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator ++ (int)
{
	sefl_t ret = *this;
	set(str() + 1, size() - 1);
	return ang::move(ret);
}

ang::str_view<MY_CHAR_TYPE, MY_ENCODING> ang::operator + (ang::str_view<MY_CHAR_TYPE, MY_ENCODING> const& str, wsize i)
{
	return ang::str_view<MY_CHAR_TYPE, MY_ENCODING>(str.m_view + i, str.size() - i);
}



ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::str_view()
	: m_view(nullptr)
	, m_size(0)
{

}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::str_view(ang::nullptr_t const&)
	: m_view(nullptr)
	, m_size(0)
{

}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::str_view(MY_CSTR_TYPE str, wsize sz)
	: m_view(nullptr)
	, m_size(0)
{
	set(str, sz);
}

wsize ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::size()const
{
	return m_size;
}

MY_CSTR_TYPE ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::cstr()const
{
	return m_view;
}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING> ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::begin()const
{
	return sefl_t(m_view, m_size);
}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING> ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::end()const
{
	return sefl_t(m_view + m_size, 0);
}

void ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::set(MY_CSTR_TYPE str, wsize sz)
{
	if (str) {
		m_view = str;
		m_size = (sz == wsize(-1)) ? m_size = text::encoder<ENCODING>::lenght(str) : sz;
	}
	else {
		m_view = nullptr;
		m_size = 0;
	}
}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>& ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator = (ang::str_view<const MY_CHAR_TYPE, MY_ENCODING> const& str)
{
	set(str.m_view, str.m_size);
	return*this;
}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>& ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator = (MY_CSTR_TYPE str)
{
	set(str);
	return*this;
}

MY_CHAR_TYPE const& ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator*()const
{
	return *cstr();
}

MY_CHAR_TYPE const** ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator & ()
{
	return &m_view;
}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator MY_CSTR_TYPE ()
{
	return cstr();
}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>& ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator ++ ()
{
	set(cstr() + 1, size() - 1);
	return *this;
}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING> ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator ++ (int)
{
	sefl_t ret = *this;
	set(cstr() + 1, size() - 1);
	return ang::move(ret);
}

ang::str_view<const MY_CHAR_TYPE, MY_ENCODING> ang::operator + (ang::str_view<const MY_CHAR_TYPE, MY_ENCODING> const& str, wsize i)
{
	return ang::str_view<const MY_CHAR_TYPE, MY_ENCODING>(str.cstr() + (wsize)i, str.size() - (wsize)i);
}



#endif//__ANG_BASE_H__
