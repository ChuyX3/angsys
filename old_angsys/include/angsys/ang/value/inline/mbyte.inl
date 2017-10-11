#ifndef __ANG_MBYTE_H__
#error ...
#elif !defined __ANG_MBYTE_HPP__
#define __ANG_MBYTE_HPP__

inline ang::mbyte::mbyte()
{
	convert((char_t)0);
}

inline ang::mbyte::mbyte(wchar n)
{
	convert(n);
}

inline ang::mbyte::mbyte(char n)
{
	convert(n);
}

inline ang::mbyte::mbyte(ang::mbyte&& b)
{
	value = ang::move(b.value);
}

inline ang::mbyte::mbyte(const ang::mbyte& b)
{
	value = b.value;
}


inline void ang::mbyte::convert(char const& _c)
{
	value = 0;
	byte c = _c;
	if (c <= 0X7F)
		bytes[0] = c;
	else
	{
		bytes[1] = 0X80 | (0X3F & c);
		bytes[0] = 0XC0 | ((c >> 6) & 0X1F);
	}
}

inline void ang::mbyte::convert(wchar const& c)
{
	value = 0;
	if (c <= 0X7F)
		bytes[0] = (byte)c;
	else if (c <= 0X7FF)
	{
		bytes[1] = 0X80 | (0X3F & c);
		bytes[0] = 0XC0 | ((c >> 6) & 0X1F);
	}
	else
	{
		bytes[2] = 0X80 | (0X3F & c);
		bytes[1] = 0X80 | ((c >> 6) & 0X3F);
		bytes[0] = 0XE0 | ((c >> 12) & 0X0F);
	}
}

inline wchar ang::mbyte::convert()const
{
	wchar ret = 0;
	switch (bytes[0] & 0XF0)
	{
	case 0XC0:
		if ((bytes[0] == 0XC0) || (bytes[0] == 0XC1)) //Not Allowed
		{
			if (value == inv_null)//0XC0 0X80 (Null Character)
				ret = 0;
			else
				ret = repl;// U+FFFD (Replcement Character);
			break;
		}
		ret = (bytes[0] & 0X1F) << 6;
		ret |= (bytes[1] & 0X3F);
		break;
	case 0XE0:
		ret = (bytes[0] & 0X0F) << 12;
		ret |= (bytes[1] & 0X3F) << 6;
		ret |= (bytes[2] & 0X3F);
		break;
	default:
		if ((bytes[0] | 0X7F) == 0X7F)//0X7F = 01111111
			ret = bytes[0];
		else //Not Allowed
			ret = repl;// U+FFFD (Replcement Character);
	}
	return ret;
}

inline int ang::mbyte::size()const
{
	if ((bytes[0] | 0X7F) == 0X7F)//0X7F = 01111111
		return 1;
	else if ((bytes[0] | 0X1F) == 0XDF)//0X7F = 11011111
		return 2;
	else if ((bytes[0] | 0X0F) == 0XEF)//0XEF = 11101111
		return 3;
	else//if((bytes[0]|0X07)==0XF7)
		return 4;
}

inline ang::mbyte& ang::mbyte::operator = (ang::mbyte&& byte)
{
	value = ang::move(byte.value);
	return *this;
}

inline ang::mbyte& ang::mbyte::operator = (const ang::mbyte& byte)
{
	value = byte.value;
	return *this;
}

inline ang::mbyte& ang::mbyte::operator = (char val)
{
	convert(val);
	return *this;
}

inline ang::mbyte& ang::mbyte::operator = (wchar val)
{
	convert(val);
	return *this;
}




#endif//__ANG_MBYTE_HPP__
