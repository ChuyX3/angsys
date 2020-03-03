#pragma once

namespace ang
{
	namespace text
	{
		template<encoding E>
		class string_factory
			: public smart<string_factory<E>, istring_factory>
		{
		public:
			string_factory();
			
			ANG_DECLARE_INTERFACE();

			virtual encoding_t encoding()const override;
			virtual istring_t create_string(ang::cstr_t = ang::cstr_t())const override;
			virtual istring_t create_wrapper(ibuffer_t)const override; //ibuffer must be reallocable otherwise it fail
			virtual istring_view_t create_wrapper(ibuffer_view_t)const override;

		private:
			virtual~string_factory();
		};

		template<encoding E>
		class string_wrapper
			: public smart<string_wrapper<E>, istring>
		{
		private:
			ibuffer_t m_buffer;

		public:
			string_wrapper(ibuffer_t);

			ANG_DECLARE_INTERFACE();

			virtual encoding_t encoding()const override;
			virtual wsize length()const override;
			virtual unknown_str_t data()const override;
			virtual rtti_t const& char_type()const override;
			virtual ang::str_t str(int = 0)override;
			virtual ang::cstr_t cstr(int = 0)const override;
			virtual int compare(ang::cstr_t)const override;
			virtual windex compare_until(ang::cstr_t)const override;
			virtual windex find(ang::cstr_t, windex start = 0, windex end = -1)const override;
			virtual windex find_reverse(ang::cstr_t, windex start = -1, windex end = 0)const override;
			virtual istring_t sub_string(istring_t, windex start, windex end)const override;
			virtual collections::ienum_ptr<istring_t> split(ang::cstr_t)const override;

			virtual void clear()override;
			virtual void copy(ang::cstr_t)override;
			virtual void concat(ang::cstr_t)override;
			virtual void insert(windex, ang::cstr_t)override;
			virtual void format(ang::cstr_t, var_args_t)override;
			virtual void concat_format(ang::cstr_t, var_args_t)override;
			virtual void insert_format(windex, ang::cstr_t, var_args_t)override;

		private:
			virtual~string_wrapper();
		};

		template<encoding E>
		class string_view
			: public smart<string_view<E>, istring_view>
		{
		public:
			typedef typename char_type_by_encoding<E>::char_t char_t;
			typedef typename char_type_by_encoding<E>::str_t str_t;
			typedef typename char_type_by_encoding<E>::cstr_t cstr_t;

		private:
			wsize m_size;
			cstr_t m_view;
			ibuffer_view_t m_buffer;

		public:
			string_view(ibuffer_view_t);

			ANG_DECLARE_INTERFACE();

			virtual encoding_t encoding()const override;
			virtual wsize length()const override;
			virtual unknown_str_t data()const override;
			virtual rtti_t const& char_type()const override;
			virtual ang::str_t str(int = 0)override;
			virtual ang::cstr_t cstr(int = 0)const override;
			virtual char32_t at(windex i, wsize* = null)const override;
			virtual int compare(ang::cstr_t)const override;
			virtual windex compare_until(ang::cstr_t)const override;
			virtual windex find(ang::cstr_t, windex start = 0, windex end = -1)const override;
			virtual windex find_reverse(ang::cstr_t, windex start = -1, windex end = 0)const override;
			virtual istring_t sub_string(istring_ptr_t, windex start, windex end)const override;
			virtual collections::ienum_ptr<istring_t> split(ang::cstr_t)const override;
		private:
			virtual~string_view();
		};



		template<encoding E>
		string_factory<E>::string_factory() {

		}

		template<encoding E>
		string_factory<E>::~string_factory() {

		}

		template<encoding E>
		encoding_t string_factory<E>::encoding()const {
			return E;
		}

		template<encoding E>
		istring_t string_factory<E>::create_string(ang::cstr_t cstr)const {
			return new basic_string_buffer<E>(cstr);
		}

		template<encoding E>
		istring_t string_factory<E>::create_wrapper(ibuffer_t buff)const {
			/*if (buff.is_empty() || !buff->can_realloc_buffer() ||
				(buff->encoding() != E && buff->encoding() != text::encoding::binary))
				return null;
			
			istring_t str = interface_cast<istring>(buff.get());

			return str.is_empty() ? new string_wrapper<E>(buff) : str.get();*/
			return null;
		}

		template<encoding E>
		istring_view_t string_factory<E>::create_wrapper(ibuffer_view_t buff)const {
			if (buff.is_empty()
				//||(buff->encoding() != E && buff->encoding() != text::encoding::binary)
				)
				return null;

			istring_view_t str = interface_cast<istring_view>(buff.get());
			return str.is_empty() ? new string_view<E>(buff) : str.get();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
		
		template<encoding E>
		string_view<E>::string_view(ibuffer_view_t buff)
			: m_size(0)
			, m_view(null)
			, m_buffer(buff)
		{
			m_view = (cstr_t)m_buffer->buffer_ptr();
			m_size = encoder<E>::lenght((cstr_t)data());
		}

		template<encoding E>
		string_view<E>::~string_view()
		{
			m_buffer = null;
		}

		template<encoding E>
		ang::rtti_t const& string_view<E>::class_info()
		{
			static const cstr_view<char> name = text::string_pool::instance()->save_string((astring("ang::text::string_view<"_s) += (castr_t)encoding_t(E).to_string()) += ">"_s);
			static rtti_t const* parents[] = {&runtime::type_of<object>(),  &runtime::type_of<istring_view>() };
			static rtti_t const& info = rtti::regist(name, genre::class_type, size_of<string_view<E>>(), align_of<string_view<E>>(), parents, &default_query_interface);
			return info;
		}

		template<encoding E>
		ang::rtti_t const& string_view<E>::runtime_info()const
		{
			return class_info();
		}

		template<encoding E>
		bool string_view<E>::query_interface(ang::rtti_t const& id, ang::unknown_ptr_t out)
		{
			if (id.type_id() == class_info().type_id())
			{
				if (out == null) return false;
				*out = static_cast<string_view<E>*>(this);
				return true;
			}
			else if (object::query_interface(id, out))
			{
				return true;
			}
			else if (id.type_id() == type_of<istring_view>().type_id()) {
				if (out == null) return false;
				*out = static_cast<istring_view*>(this);
				return true;
			}
			return false;
		}

		template<encoding E>
		encoding_t string_view<E>::encoding()const
		{
			return E;
		}

		template<encoding E>
		wsize string_view<E>::length()const
		{
			return m_size;
		}

		template<encoding E>
		unknown_str_t string_view<E>::data()const
		{
			return (pointer)m_buffer->buffer_ptr();
		}

		template<encoding E>
		rtti_t const& string_view<E>::char_type()const
		{
			return ang::type_of<char_t>();
		}

		template<encoding E>
		ang::str_t string_view<E>::str(int)
		{
			return str_view<char_t, E>((str_t)data(), length());
		}

		template<encoding E>
		ang::cstr_t string_view<E>::cstr(int)const
		{
			return str_view<char_t const, E>((cstr_t)data(), length());
		}

		template<encoding E>
		char32_t string_view<E>::at(windex i, wsize* sz)const
		{	
			windex idx = i;
			char32_t c = to_char32<false, is_endian_swapped<E>::value>((cstr_t)data(), idx);
			if (sz) *sz = idx - i;
			return c;
		}

		template<encoding E>
		int string_view<E>::compare(ang::cstr_t cstr)const
		{
			cstr_t ptr = (cstr_t)data();
			switch (cstr.encoding())
			{
			case encoding::ascii: return encoder<E>::compare(ptr, cstr.cstr<encoding::ascii>());
			case encoding::unicode:return encoder<E>::compare(ptr, cstr.cstr<encoding::unicode>());
			case encoding::utf8: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf8>());
			case encoding::utf16: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf16>());
			case encoding::utf16_se: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf16_se>());
			case encoding::utf16_le: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf16_le>());
			case encoding::utf16_be: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf16_be>());
			case encoding::utf32: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf32>());
			case encoding::utf32_se: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf32_se>());
			case encoding::utf32_le: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf32_le>());
			case encoding::utf32_be: return encoder<E>::compare(ptr, cstr.cstr<encoding::utf32_be>());
			default: return -10;
			}
		}

		template<encoding E>
		windex string_view<E>::compare_until(ang::cstr_t cstr)const
		{
			cstr_t ptr = (cstr_t)data();
			switch (cstr.encoding())
			{
			case encoding::ascii: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::ascii>());
			case encoding::unicode:return encoder<E>::compare_until(ptr, cstr.cstr<encoding::unicode>());
			case encoding::utf8: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf8>());
			case encoding::utf16: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf16>());
			case encoding::utf16_se: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf16_se>());
			case encoding::utf16_le: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf16_le>());
			case encoding::utf16_be: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf16_be>());
			case encoding::utf32: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf32>());
			case encoding::utf32_se: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf32_se>());
			case encoding::utf32_le: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf32_le>());
			case encoding::utf32_be: return encoder<E>::compare_until(ptr, cstr.cstr<encoding::utf32_be>());
			default: return 0;
			}
		}

		template<encoding E>
		windex string_view<E>::find(ang::cstr_t cstr, windex start, windex end)const
		{
			cstr_t ptr = (cstr_t)data();
			end = min(end, length());
			switch (cstr.encoding())
			{
			case encoding::ascii: return encoder<E>::find(ptr, end, cstr.cstr<encoding::ascii>(), cstr.size() / size_of<char>(), start);
			case encoding::unicode:return encoder<E>::find(ptr, end, cstr.cstr<encoding::unicode>(), cstr.size() / size_of<wchar>(), start);
			case encoding::utf8: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf8>(), cstr.size() / size_of<mchar>(), start);
			case encoding::utf16: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf16>(), cstr.size() / size_of<char16>(), start);
			case encoding::utf16_se: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf16_se>(), cstr.size() / size_of<char16>(), start);
			case encoding::utf16_le: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf16_le>(), cstr.size() / size_of<char16>(), start);
			case encoding::utf16_be: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf16_be>(), cstr.size() / size_of<char16>(), start);
			case encoding::utf32: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf32>(), cstr.size() / size_of<char32>(), start);
			case encoding::utf32_se: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf32_se>(), cstr.size() / size_of<char32>(), start);
			case encoding::utf32_le: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf32_le>(), cstr.size() / size_of<char32>(), start);
			case encoding::utf32_be: return encoder<E>::find(ptr, end, cstr.cstr<encoding::utf32_be>(), cstr.size() / size_of<char32>(), start);
			default: return 0;
			}
		}

		template<encoding E>
		windex string_view<E>::find_reverse(ang::cstr_t cstr, windex start, windex end)const
		{
			cstr_t ptr = (cstr_t)data();
			end = min(end, length());
			switch (cstr.encoding())
			{
			case encoding::ascii: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::ascii>(), cstr.size() / size_of<char>(), start);
			case encoding::unicode:return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::unicode>(), cstr.size() / size_of<wchar>(), start);
			case encoding::utf8: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf8>(), cstr.size() / size_of<mchar>(), start);
			case encoding::utf16: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf16>(), cstr.size() / size_of<char16>(), start);
			case encoding::utf16_se: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf16_se>(), cstr.size() / size_of<char16>(), start);
			case encoding::utf16_le: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf16_le>(), cstr.size() / size_of<char16>(), start);
			case encoding::utf16_be: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf16_be>(), cstr.size() / size_of<char16>(), start);
			case encoding::utf32: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf32>(), cstr.size() / size_of<char32>(), start);
			case encoding::utf32_se: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf32_se>(), cstr.size() / size_of<char32>(), start);
			case encoding::utf32_le: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf32_le>(), cstr.size() / size_of<char32>(), start);
			case encoding::utf32_be: return encoder<E>::find_reverse(ptr, end, cstr.cstr<encoding::utf32_be>(), cstr.size() / size_of<char32>(), start);
			default: return 0;
			}
		}

		template<encoding E>
		istring_t string_view<E>::sub_string(istring_ptr_t out, windex start, windex end)const
		{
			if (start >= end || start > length())
				return null;

			istring_t str = istring_factory::get_factory(encoding())->create_string();
			if (!out.is_empty())
				*out = str;

			str->copy(raw_cstr((cstr_t)data() + start, min(end, m_buffer->buffer_size() / size_of<char_t>()) - start, E));
			return str;
		}
		
		template<encoding E>
		collections::ienum_ptr<istring_t> string_view<E>::split(ang::cstr_t val)const
		{
			collections::vector<istring_t> list;

			windex beg = 0, end = 0;
			istring_t _word;
			ang::cstr_t data = cstr();
			wsize l = length(), c = val.count();
			end = find(val);
			if (end == invalid_index)
				return list.get();

			do {

				if (sub_string(&_word, beg, end) > 0)
				{
					list += _word;
					_word = null;
				}
				beg = end + val.size();
				end = find(val, beg);
			} while (end != invalid_index);

			if (sub_string(&_word, beg, l) > 0)
			{
				list += _word;
				_word = null;
			}
			return list.get();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
	}
}