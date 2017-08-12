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
			xml_value_t	code;
			xml_value_t	value;
		}xml_entity_values_t;
		extern xml_entity_values_t xml_entity_values[5];

		template<class _char_t>
		inline int xml_skip_space(const _char_t* code, index begin)
		{
			int c = 0;
			_char_t at;
			while (((at = code[begin + c]) == ' ')
				|| (at == '\n')
				|| (at == '\t')
				|| (at == '\r')
				)c++;
			return c;
		}

		template<class _char_t>
		inline int xml_get_element_name(const _char_t* code, index begin)
		{
			int c = 0;
			_char_t at;
			while (((at = code[begin + c]) != ' ')
				&& (at != '\n')
				&& (at != '\t')
				&& (at != '\r')
				&& (at != '/')
				&& (at != '>'))
				c++;
			return c;
		}

		template<class _char_t>
		inline int xml_get_attibute_name(const _char_t* code, index begin)
		{
			int c = 0;
			_char_t at;
			while (((at = code[begin + c]) != ' ')
				&& (at != '\n')
				&& (at != '\t')
				&& (at != '\r')
				&& (at != '='))
				c++;
			return c;
		}


#if defined _DEBUG
		template<class _char>
		class xml_string_iterator
		{
		private:
			_char const* _at;
			_char const* _data;
			collections::iterator<_char> &_current;

		public:
			xml_string_iterator(const xml_string_iterator& iter) : _current(iter._current) {
				_data = iter._data;
				_at = &_data[_current.offset()];
			}
			xml_string_iterator(collections::iterator<_char>& beg) :_current(beg) {
				_data = (_char const*)beg.current();
				_at = &_data[beg.offset()];
			}

			xml_string_iterator& operator = (const collections::iterator<_char>& curr) {
				_data = (_char const*)curr.current();
				_current = curr;
				_at = &_data[_current.offset()];
				return *this;
			}

			xml_string_iterator& operator += (const collections::iterator<_char>& curr) {
				_current += curr;
				_at = &_data[_current.offset()];
				return *this;
			}

			xml_string_iterator& operator -= (const collections::iterator<_char>& curr) {
				_current -= curr;
				_at = &_data[_current.offset()];
				return *this;
			}

			xml_string_iterator& operator = (const int& curr) {
				_current = curr;
				_at = &_data[_current.offset()];
				return *this;
			}

			xml_string_iterator& operator += (const int& curr) {
				_current += curr;
				_at = &_data[_current.offset()];
				return *this;
			}

			xml_string_iterator& operator -= (const int& curr) {
				_current -= curr;
				_at = &_data[_current.offset()];
				return *this;
			}

			xml_string_iterator& operator ++ () {
				_current++;
				_at = &_data[_current.offset()];
				return *this;
			}

			xml_string_iterator& operator -- () {
				_current--;
				_at = &_data[_current.offset()];
				return *this;
			}

			collections::iterator<_char> operator + (int value)
			{
				return _current + value;
			}
			collections::iterator<_char> operator - (int value)
			{
				return _current - value;
			}

			operator collections::iterator<_char>& () {
				return _current;
			}

			operator const collections::iterator<_char>& ()const {
				return _current;
			}

			collections::ienum<_char>* parent()const { return _current.partent(); }
			void parent(collections::ienum<_char>* p) { _current.partent(p); }
			collections::position_t current()const { return _current; }
			void current(collections::position_t p) { _current.current(p); }
			wsize offset()const { return _current.offset(); }
			void offset(wsize o) { _current.offset(o); }
			bool is_valid()const { return _current.is_valid(); }
		};

		template<class _char>
		class xml_cstr_iterator
		{
		private:
			_char const* _at;
			_char const* _data;
			index &_current;

		public:
			xml_cstr_iterator(xml_cstr_iterator& iter) : _current(iter._current) {
				_data = iter._data;
				_at = &_data[_current];
			}
			xml_cstr_iterator(index &beg, _char const* data) :_current(beg) {
				_data = data;
				_at = &data[beg];
			}

			xml_cstr_iterator& operator = (uint curr) {
				_current = curr;
				_at = &_data[_current];
				return *this;
			}

			xml_cstr_iterator& operator += (uint curr) {
				_current += curr;
				_at = &_data[_current];
				return *this;
			}

			xml_cstr_iterator& operator -= (uint curr) {
				_current -= curr;
				_at = &_data[_current];
				return *this;
			}

			xml_cstr_iterator& operator = (int curr) {
				_current = curr;
				_at = &_data[_current];
				return *this;
			}

			xml_cstr_iterator& operator += (int curr) {
				_current += curr;
				_at = &_data[_current];
				return *this;
			}

			xml_cstr_iterator& operator -= (int curr) {
				_current -= curr;
				_at = &_data[_current];
				return *this;
			}

			xml_cstr_iterator& operator ++ () {
				_current++;
				_at = &_data[_current];
				return *this;
			}


			xml_cstr_iterator& operator -- () {
				_current--;
				_at = &_data[_current];
				return *this;
			}

			index operator + (int value)
			{
				return _current + value;
			}
			index operator - (int value)
			{
				return _current - value;
			}

			index operator + (uint value)
			{
				return _current + value;
			}
			index operator - (uint value)
			{
				return _current - value;
			}

			operator index& () {
				return _current;
			}

			operator const index& ()const {
				return _current;
			}
		};

#endif


	}
}