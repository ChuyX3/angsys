#pragma once

namespace ang
{
	namespace text
	{
		template<encoding E>
		class string_factory
			: public object
			, public istring_factory
		{
		public:
			string_factory();
			
			ANG_DECLARE_INTERFACE();

			virtual encoding_t encoding()const override;
			virtual istring_t create_string(raw_cstr_t = raw_cstr_t())const override;
			virtual istring_t create_wrapper(ibuffer_t)const override; //ibuffer must be reallocable otherwise it fail
			virtual istring_view_t create_wrapper(ibuffer_view_t)const override;

		private:
			virtual~string_factory();
		};

		template<encoding E>
		class string_wrapper
			: public object
			, public istring
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
			virtual raw_str_t str(int = 0)override;
			virtual raw_cstr_t cstr(int = 0)const override;
			virtual int compare(raw_cstr_t)const override;
			virtual windex compare_until(raw_cstr_t)const override;
			virtual windex find(raw_cstr_t, windex start = 0, windex end = -1)const override;
			virtual windex find_reverse(raw_cstr_t, windex start = -1, windex end = 0)const override;
			virtual istring_t sub_string(istring_t, windex start, windex end)const override;

			virtual void clear()override;
			virtual void copy(raw_cstr_t)override;
			virtual void concat(raw_cstr_t)override;
			virtual void insert(windex, raw_cstr_t)override;
			virtual void format(raw_cstr_t, var_args_t)override;
			virtual void concat_format(raw_cstr_t, var_args_t)override;
			virtual void insert_format(windex, raw_cstr_t, var_args_t)override;

		private:
			virtual~string_wrapper();
		};

		template<encoding E>
		class string_view
			: public object
			, public istring_view
		{
		public:
			typedef typename char_type_by_encoding<E>::char_t char_t;
			typedef typename char_type_by_encoding<E>::str_t str_t;
			typedef typename char_type_by_encoding<E>::cstr_t cstr_t;

		private:
			wsize m_size;
			cstr_t m_view;
			ibuffer_view_t m_buffer;
			iencoder_t m_encoder;

		public:
			string_view(ibuffer_view_t);

			ANG_DECLARE_INTERFACE();

			virtual encoding_t encoding()const override;
			virtual wsize length()const override;
			virtual unknown_str_t data()const override;
			virtual rtti_t const& char_type()const override;
			virtual raw_str_t str(int = 0)override;
			virtual raw_cstr_t cstr(int = 0)const override;
			virtual char32_t at(windex i, wsize* = null)const override;
			virtual int compare(raw_cstr_t)const override;
			virtual windex compare_until(raw_cstr_t)const override;
			virtual windex find(raw_cstr_t, windex start = 0, windex end = -1)const override;
			virtual windex find_reverse(raw_cstr_t, windex start = -1, windex end = 0)const override;
			virtual istring_t sub_string(istring_ptr_t, windex start, windex end)const override;

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
		istring_t string_factory<E>::create_string(raw_cstr_t cstr)const {
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
			, m_encoder(iencoder::get_encoder(E))
		{
			m_size = m_buffer->buffer_size() / size_of<char_t>();
			m_view = (cstr_t)m_buffer->buffer_ptr();
		}

		template<encoding E>
		string_view<E>::~string_view()
		{
			m_buffer = null;
			m_encoder = null;
		}

		template<encoding E>
		ang::rtti_t const& string_view<E>::class_info()
		{
			static const cstr_view<char> name = text::string_pool::instance()->save_string((string("ang::text::string_view<"_s) += encoding_t(E).to_string()) += ">"_s);
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
			return m_encoder->lenght(data());
		}

		template<encoding E>
		unknown_str_t string_view<E>::data()const
		{
			return (pointer)m_buffer->buffer_ptr();
		}

		template<encoding E>
		rtti_t const& string_view<E>::char_type()const
		{
			return m_encoder->char_type();
		}

		template<encoding E>
		raw_str_t string_view<E>::str(int)
		{
			return str_view<char_t, E>((str_t)data(), length());
		}

		template<encoding E>
		raw_cstr_t string_view<E>::cstr(int)const
		{
			return str_view<char_t const, E>((cstr_t)data(), length());
		}

		template<encoding E>
		char32_t string_view<E>::at(windex i, wsize* sz)const
		{
			windex idx = i;
			char32_t c = m_encoder->to_char32(data(), idx, true);
			if (sz) *sz = idx - i;
			return c;
		}

		template<encoding E>
		int string_view<E>::compare(raw_cstr_t cstr)const
		{
			return m_encoder->compare(data(), cstr.ptr(), cstr.encoding());
		}

		template<encoding E>
		windex string_view<E>::compare_until(raw_cstr_t cstr)const
		{
			return m_encoder->compare_until(data(), cstr.ptr(), cstr.encoding());
		}

		template<encoding E>
		windex string_view<E>::find(raw_cstr_t cstr, windex start, windex end)const
		{
			return m_encoder->find(data(), min(end, m_buffer->buffer_size() / size_of<char_t>()), cstr.ptr(), cstr.count(), cstr.encoding(), start);
		}

		template<encoding E>
		windex string_view<E>::find_reverse(raw_cstr_t cstr, windex start, windex end)const
		{
			return m_encoder->find_reverse(data(), min(end, m_buffer->buffer_size() / size_of<char_t>()), cstr.ptr(), cstr.count(), cstr.encoding(), start);
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
		
		/////////////////////////////////////////////////////////////////////////////////////////////////
	}
}