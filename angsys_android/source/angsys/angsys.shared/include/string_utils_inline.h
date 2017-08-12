#pragma once
namespace ang
{
	template<class T1, class T2>
	inline index string_find(const T1* cstr1, uint lenght1, const T2* cstr2, uint lenght2, index start, bool rev, uint* = null) {

		if (cstr1 == null || cstr2 == null)
			return invalid_index;

		if (lenght2 == 0 || start >= lenght1)
			return invalid_index;

		if (rev)
		{
			if (lenght2 > start)
				return invalid_index;
			T1 const* beg = cstr1;
			T1 const* end = cstr1 + start - lenght2 + 1;
			index i = start - lenght2 + 1, j = 0;
			do {
				j = 0;
				while (j < lenght2 && (ushort)cstr2[j] == (ushort)end[j]) ++j;
				if (j == lenght2) return i;
				i--;
			} while (beg <= --end);
		}
		else
		{
			if (lenght2 >(lenght1 - start))
				return invalid_index;
			T1 const* beg = cstr1 + start;
			T1 const* end = cstr1 + lenght1 - lenght2 + 1;
			index i = start, j = 0;
			do {
				j = 0;
				while (j < lenght2 && (ushort)cstr2[j] == (ushort)beg[j]) ++j;
				if (j == lenght2) return i;
				i++;
			} while (++beg < end);
		}
		return invalid_index;
	}

	template<class T1, class T2>
	inline index string_find_any_of(T1 const* cstr, uint lenght, ang::static_array<T2> arr, index start) {

		if (cstr == null || arr.size() == 0)
			return invalid_index;

		if (start >= lenght)
			return invalid_index;

		T1 const* beg = cstr + start;
		T1 const* end = cstr + lenght;
		T2 const* chars = arr.get();
		index i = start, j = 0;
		do {
			j = 0;
			while (j < arr.size() && chars[j] != *beg) ++j;
			if (j != arr.size()) return i;
			i++;
		} while (++beg < end);
		return invalid_index;
	}


	template<class T1, class T2>
	inline uint string_substr(T1 const* src, uint length, T2* out, index start, index count)
	{
		if (start >= length)
			return 0;
		if (count > (length - start))
			count = length - start;
		count = strings::algorithms::string_copy(out, &src[start], count);
		if (count == 0)
			return 0;
		out[count] = 0;
		return count;
	}




	template<class T>
	inline index string_find(const mchar* cstr1, uint lenght1, const T* cstr2, uint lenght2, index start, bool rev, uint* pout_lenght) {

		if (cstr1 == null || cstr2 == null)
			return invalid_index;

		if (lenght2 == 0 || start >= lenght1)
			return invalid_index;

		if (rev)
		{
			if (lenght2 > start)
				return invalid_index;
			uint out_lenght = 0;
			mchar const* beg = cstr1;
			mchar const* end = cstr1 + start - lenght2 + 1;
			index i = start - lenght2 + 1, j = 0;
			do {
				j = 0;
				out_lenght = 0;
				while (j < lenght2)
				{
					mbyte _char = cstr2[j];
					if ((_char.bytes[0] | 0X7F) == 0X7F)//0X7F = 01111111
					{
						if (beg[j] != _char.bytes[0])
							break;
						out_lenght++;
					}
					else if ((_char.bytes[0] | 0X1F) == 0XDF)//0X7F = 11011111
					{
						if (beg[j] != _char.bytes[0]
							|| beg[j + 1] != _char.bytes[1])
							break;
						out_lenght += 2;
					}
					else if ((_char.bytes[0] | 0X0F) == 0XEF)//0XEF = 11101111
					{
						if (beg[j] != _char.bytes[0]
							|| beg[j + 1] != _char.bytes[1]
							|| beg[j + 2] != _char.bytes[2])
							break;
						out_lenght += 3;
					}
					else//if((bytes[0]|0X07)==0XF7)
					{
						if (beg[j] != _char.bytes[0]
							|| beg[j + 1] != _char.bytes[1]
							|| beg[j + 2] != _char.bytes[2]
							|| beg[j + 3] != _char.bytes[3])
							break;
						out_lenght += 4;
					}
					j++;
				}
				if (j == lenght2) {
					if (pout_lenght)
						*pout_lenght = out_lenght;
					return i;
				}
				i--;
			} while (beg <= --end);
		}
		else
		{
			if (lenght2 >(lenght1 - start))
				return invalid_index;
			uint out_lenght = 0;
			mchar const* beg = cstr1 + start;
			mchar const* end = cstr1 + lenght1 - lenght2 + 1;
			index i = start, j = 0;
			do {
				j = 0;
				out_lenght = 0;
				while (j < lenght2)
				{
					mbyte _char = cstr2[j];
					if ((_char.bytes[0] | 0X7F) == 0X7F)//0X7F = 01111111
					{
						if (beg[j] != _char.bytes[0])
							break;
						out_lenght++;
						
					}
					else if ((_char.bytes[0] | 0X1F) == 0XDF)//0X7F = 11011111
					{
						if (beg[j] != _char.bytes[0]
							|| beg[j + 1] != _char.bytes[1])
							break;
						out_lenght += 2;
					}
					else if ((_char.bytes[0] | 0X0F) == 0XEF)//0XEF = 11101111
					{
						if (beg[j] != _char.bytes[0]
							|| beg[j + 1] != _char.bytes[1]
							|| beg[j + 2] != _char.bytes[2])
							break;
						out_lenght += 3;
					}
					else//if((bytes[0]|0X07)==0XF7)
					{
						if (beg[j] != _char.bytes[0]
							|| beg[j + 1] != _char.bytes[1]
							|| beg[j + 2] != _char.bytes[2]
							|| beg[j + 3] != _char.bytes[3])
							break;
						out_lenght += 4;
					}
					j++;
				}
				if (j == lenght2) {
					if (pout_lenght)
						*pout_lenght = out_lenght;
					return i;
				}
				i++;
			} while (++beg < end);
		}
		return invalid_index;
	}


	template<>
	inline index string_find(const mchar* cstr1, uint lenght1, const mchar* cstr2, uint lenght2, index start, bool rev, uint*) {

		if (cstr1 == null || cstr2 == null)
			return invalid_index;

		if (lenght2 == 0 || start >= lenght1)
			return invalid_index;

		if (rev)
		{
			if (lenght2 > start)
				return invalid_index;
			mchar const* beg = cstr1;
			mchar const* end = cstr1 + start - lenght2 + 1;
			index i = start - lenght2 + 1, j = 0;
			do {
				j = 0;
				while (j < lenght2 && (ushort)cstr2[j] == (ushort)end[j]) ++j;
				if (j == lenght2) return i;
				i--;
			} while (beg <= --end);
		}
		else
		{
			if (lenght2 >(lenght1 - start))
				return invalid_index;
			mchar const* beg = cstr1 + start;
			mchar const* end = cstr1 + lenght1 - lenght2 + 1;
			index i = start, j = 0;
			do {
				j = 0;
				while (j < lenght2 && (ushort)cstr2[j] == (ushort)beg[j]) ++j;
				if (j == lenght2) return i;
				i++;
			} while (++beg < end);
		}
		return invalid_index;
	}


}