#pragma once
namespace ang
{
	namespace xml
	{
		typedef enum xml_entity : uint
		{
			amp,	//	&
			lt,		//	<
			gt,		//	>
			quot,	//	"
			apos,	//	'
		}xml_entity_t;

		typedef	struct xml_entity_values
		{
			xml_entity_t key;
			cstr_t	code;
			cstr_t	value;
		}xml_entity_values_t;
		extern xml_entity_values_t xml_entity_values[5];


		//////////////////////////////////////////////////////////////////////////////////////////////


		inline int xml_skip_space(text::string_util code, windex begin)
		{
			windex c = begin;
			char32_t at;
			while (((at = code[c]) == U' ')
				|| (at == U'\n')
				|| (at == U'\t')
				|| (at == U'\r')) c++;
			return int(c - begin);
		}

		inline int xml_get_element_name(text::string_util code, windex begin)
		{
			windex c = begin;
			char32_t at;
			while (((at = code[c]) != U' ')
				&& (at != U'\n')
				&& (at != U'\t')
				&& (at != U'\r')
				&& (at != U'/')
				&& (at != U'>')) c++;
			return int(c - begin);
		}


		inline int xml_get_attibute_name(text::string_util code, windex begin)
		{
			windex c = begin;
			char32_t at;
			while (((at = code[c]) != U' ')
				&& (at != U'\n')
				&& (at != U'\t')
				&& (at != U'\r')
				&& (at != U'=')) c++;
			return int(c - begin);
		}


#if defined _DEBUG
		class xml_parsing_iterator
		{
		private:
			raw_str_t _at;
			raw_str_t _data;
			windex& _current;

		public:
			xml_parsing_iterator(const xml_parsing_iterator& iter) : _current(iter._current) {
				_data = iter._data;
				_at._value = ((byte*)_data.ptr() + _current * _data.char_size());
				_at._size = _data._size - _current;
				_at._encoding = _data._encoding;
			}
			xml_parsing_iterator(raw_str_t data, windex& current) :_current(current) {
				_data = data;
				_at._value = ((byte*)_data.ptr() + _current * _data.char_size());
				_at._size = _data._size - _current;
				_at._encoding = _data._encoding;
			}

			template <typename T> xml_parsing_iterator& operator = (T const& curr) {
				_current = curr;
				_at._value = ((byte*)_data.ptr() + _current * _data.char_size());
				_at._size = _data._size - _current;
				_at._encoding = _data._encoding;
				return *this;
			}

			template <typename T> xml_parsing_iterator& operator += (T const& curr) {
				_current += curr;
				_at._value = ((byte*)_data.ptr() + _current * _data.char_size());
				_at._size = _data._size - _current;
				_at._encoding = _data._encoding;
				return *this;
			}

			template <typename T> xml_parsing_iterator& operator -= (T const& curr) {
				_current -= curr;
				_at._value = ((byte*)_data.ptr() + _current * _data.char_size());
				_at._size = _data._size - _current;
				_at._encoding = _data._encoding;
				return *this;
			}

			xml_parsing_iterator& operator ++ () {
				_current += 1;
				_at._value = ((byte*)_data.ptr() + _current * _data.char_size());
				_at._size = _data._size - _current;
				_at._encoding = _data._encoding;
				return *this;
			}

			xml_parsing_iterator& operator -- () {
				_current -= 1;
				_at._value = ((byte*)_data.ptr() + _current * _data.char_size());
				_at._size = _data._size - _current;
				_at._encoding = _data._encoding;
				return *this;
			}
			template <typename T> windex operator + (T const& value)
			{
				return _current + value;
			}
			template <typename T> windex operator - (T const& value)
			{
				return _current - value;
			}

			operator windex& () {
				return _current;
			}

			operator const windex& ()const {
				return _current;
			}
		};

#endif


	}
}