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


ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::str_view()
	: m_view(nullptr)
	, m_size(0)
{

}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::str_view(ang::nullptr_t const&)
	: m_view(nullptr)
	, m_size(0)
{

}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::str_view(MY_STR_TYPE str, wsize sz)
	: m_view(nullptr)
	, m_size(0)
{
	set(str, sz);
}

bool ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::is_empty()const
{
	return m_size == 0;
}

wsize ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::size()const
{
	return m_size;
}

MY_STR_TYPE ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::str()
{
	return m_view;
}

MY_CSTR_TYPE ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::cstr()const
{
	return m_view;
}

void ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::set(MY_STR_TYPE str, wsize sz)
{
	if (str) {
		m_view = str;
		m_size = (sz == wsize(-1)) ? m_size = text::text_encoder<ENCODING>::lenght(str) : sz;
	}
	else {
		m_view = nullptr;
		m_size = 0;
	}
}

MY_CHAR_TYPE* ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::begin()const
{
	return m_view;
}

MY_CHAR_TYPE* ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::end()const
{
	return m_view + m_size;
}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>& ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator = (ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING> const& str)
{
	set(str.m_view, str.m_size);
	return*this;
}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>& ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator = (MY_STR_TYPE str)
{
	set(str);
	return*this;
}

MY_CHAR_TYPE& ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator*()
{
	return *str();
}

MY_CHAR_TYPE const& ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator*()const
{
	return *cstr();
}

MY_CHAR_TYPE** ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator & ()
{
	return &m_view;
}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator MY_STR_TYPE ()
{
	return str();
}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator MY_CSTR_TYPE ()
{
	return cstr();
}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>& ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator ++ ()
{
	set(str() + 1, size() - 1);
	return *this;
}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING> ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>::operator ++ (int)
{
	self ret = *this;
	set(str() + 1, size() - 1);
	return ang::move(ret);
}

ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING> ang::text::operator + (ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING> const& str, wsize i)
{
	return ang::text::str_view<MY_CHAR_TYPE, MY_ENCODING>(str.m_view + i, str.size() - i);
}



ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::str_view()
	: m_view(nullptr)
	, m_size(0)
{

}

ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::str_view(ang::nullptr_t const&)
	: m_view(nullptr)
	, m_size(0)
{

}

ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::str_view(MY_CSTR_TYPE str, wsize sz)
	: m_view(nullptr)
	, m_size(0)
{
	set(str, sz);
}

bool ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::is_empty()const
{
	return m_size == 0;
}

wsize ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::size()const
{
	return m_size;
}

MY_CSTR_TYPE ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::cstr()const
{
	return m_view;
}

MY_CHAR_TYPE const* ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::begin()const
{
	return m_view;
}

MY_CHAR_TYPE const* ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::end()const
{
	return m_view + m_size;
}

void ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::set(MY_CSTR_TYPE str, wsize sz)
{
	if (str) {
		m_view = str;
		m_size = (sz == wsize(-1)) ? m_size = text::text_encoder<ENCODING>::lenght(str) : sz;
	}
	else {
		m_view = nullptr;
		m_size = 0;
	}
}

ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>& ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator = (ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING> const& str)
{
	set(str.m_view, str.m_size);
	return*this;
}

ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>& ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator = (MY_CSTR_TYPE str)
{
	set(str);
	return*this;
}

MY_CHAR_TYPE const& ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator*()const
{
	return *cstr();
}

MY_CHAR_TYPE const** ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator & ()
{
	return &m_view;
}

ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator MY_CSTR_TYPE ()
{
	return cstr();
}

ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>& ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator ++ ()
{
	set(cstr() + 1, size() - 1);
	return *this;
}

ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING> ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>::operator ++ (int)
{
	self ret = *this;
	set(cstr() + 1, size() - 1);
	return ang::move(ret);
}

ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING> ang::text::operator + (ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING> const& str, wsize i)
{
	return ang::text::str_view<const MY_CHAR_TYPE, MY_ENCODING>(str.cstr() + (wsize)i, str.size() - (wsize)i);
}



#endif//__ANG_BASE_H__
