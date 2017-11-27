namespace ang
{
	template<> class LINK object_wrapper<strings::string_buffer<CURRENT_ENCODING>>
	{

	public:
		static const text::encoding_enum ENCODING = CURRENT_ENCODING;
		typedef strings::string_buffer<ENCODING> type;
	private:
		strings::string_buffer<ENCODING>* _ptr;

	public:
		object_wrapper();
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		object_wrapper(strings::string_buffer<ENCODING>*);
		object_wrapper(ang::nullptr_t const&);
		//template<typename T> object_wrapper(T const* str);
		template<typename T> object_wrapper(safe_str<T> const& str);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper(object_wrapper<strings::string_buffer<OTHER_ENCODING>> const& str);
		template<typename T, wsize N> object_wrapper(const T(&ar)[N]);
		~object_wrapper();

	public:
		void clean();
		void clean_unsafe();
		bool is_empty()const;
		strings::string_buffer<ENCODING>* get(void)const;
		void set(strings::string_buffer<ENCODING>*);
		strings::string_buffer<ENCODING> ** addres_of(void);

	public:
		object_wrapper& operator = (ang::nullptr_t const&);
		object_wrapper& operator = (strings::string_buffer<ENCODING>*);
		object_wrapper& operator = (object_wrapper<strings::string_buffer<ENCODING>> &&);
		object_wrapper& operator = (object_wrapper<strings::string_buffer<ENCODING>> const&);
		template<typename T> object_wrapper& operator = (safe_str<T> const& str);
		template<typename T, wsize N> object_wrapper& operator = (const T(&ar)[N]);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper& operator = (object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);
		template<typename T> object_wrapper& operator += (safe_str<T> const& str);
		template<typename T, wsize N> object_wrapper& operator += (const T(&ar)[N]);
		template<text::encoding_enum OTHER_ENCODING> object_wrapper& operator += (object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);
		inline operator objptr()const;
		object_wrapper_ptr<strings::string_buffer<ENCODING>> operator & (void);
		strings::string_buffer<ENCODING> * operator -> (void);
		strings::string_buffer<ENCODING> const* operator -> (void)const;
		explicit operator strings::string_buffer<ENCODING> * (void);
		explicit operator strings::string_buffer<ENCODING> const* (void)const;
		operator safe_str<typename text::char_type_by_encoding<ENCODING>::char_t>(void);
		operator safe_str<typename text::char_type_by_encoding<ENCODING>::char_t const>(void)const;
		typename text::char_type_by_encoding<ENCODING>::char_t& operator [] (int);
		typename text::char_type_by_encoding<ENCODING>::char_t operator [] (int)const;
		template<text::encoding_enum OTHER_ENCODING>

		friend object_wrapper<strings::string_buffer<ENCODING>> operator +
			(object_wrapper<strings::string_buffer<ENCODING>> const&, object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);

		template<typename T> friend object_wrapper<strings::string_buffer<ENCODING>> operator +
			(object_wrapper<strings::string_buffer<ENCODING>> const&, safe_str<T> const&);

		template<typename T> friend object_wrapper<strings::string_buffer<ENCODING>> operator +
			(safe_str<T> const&, object_wrapper<strings::string_buffer<ENCODING>> const&);

		template<text::encoding_enum OTHER_ENCODING> friend object_wrapper<strings::string_buffer<ENCODING>>& operator <<
			(object_wrapper<strings::string_buffer<ENCODING>>&, object_wrapper<strings::string_buffer<OTHER_ENCODING>> const&);

		template<typename T> friend object_wrapper<strings::string_buffer<ENCODING>>& operator <<
			(object_wrapper<strings::string_buffer<ENCODING>>&, safe_str<T> const&);
	};

	namespace strings
	{


		template<> class LINK string_buffer<CURRENT_ENCODING>
			: public object
			, public text::itext_buffer
		{
		public:
			static const text::encoding_enum ENCODING = CURRENT_ENCODING;
			static const wsize LOCAL_CAPACITY = str_data<ENCODING>::CAPACITY;
			typedef typename char_type_by_encoding<ENCODING>::char_t char_t, type;
			typedef typename char_type_by_encoding<ENCODING>::str_t unsafe_str_t;
			typedef typename char_type_by_encoding<ENCODING>::cstr_t unsafe_cstr_t;
			typedef safe_str<char_t> str_t;
			typedef safe_str<char_t const> cstr_t;

		protected:
			str_data<ENCODING> _data;
			wsize _map_index, _map_size;

		public:
			string_buffer();
			string_buffer(wsize reserv);
			template<typename T>
			string_buffer(safe_str<T> const& str) : string_buffer() {
				copy(str);
			}
			template<encoding_enum OTHER_ENCODING>
			string_buffer(string_buffer<OTHER_ENCODING> const* str) : string_buffer() {
				if (str)copy(str->cstr());
			}
		protected:
			virtual~string_buffer();

		public:
			ANG_DECLARE_INTERFACE();
			bool is_local_data()const;
			bool realloc(wsize new_size, bool save = true);
			void length(wsize len);
			wsize length() const;

		public:
			void clean();
			bool is_empty()const;
			str_t str();
			cstr_t cstr() const;
			wsize capacity() const;
			void move(string_base<CURRENT_ENCODING>&);
			void move(string_buffer<CURRENT_ENCODING>*);

		public:
			template<typename cstr_t>
			inline int compare(cstr_t cstr)const {
				return get_encoder<ENCODING>().compare(this->cstr().cstr(), (typename char_type_by_type<cstr_t>::cstr_t)cstr);
			}
			template<typename cstr_t>
			inline windex compare_until(cstr_t cstr)const {
				return get_encoder<ENCODING>().compare_until(this->cstr().cstr(), (typename char_type_by_type<cstr_t>::cstr_t)cstr);
			}
			template<typename cstr_t>
			inline void copy(cstr_t cstr) {
				copy((pointer)(typename char_type_by_type<cstr_t>::cstr_t)cstr, algorithms::string_length(cstr), encoding_by_type<typename char_type_by_type<cstr_t>::char_t>::encoding());
			}
			template<typename cstr_t>
			inline void concat(cstr_t cstr) {
				copy_at(length() ,(pointer)(typename char_type_by_type<cstr_t>::cstr_t)cstr, algorithms::string_length(cstr), encoding_by_type<typename char_type_by_type<cstr_t>::char_t>::encoding());
			}

			template<typename cstr_t>
			inline windex find(cstr_t cstr, windex start = 0)const { 
				return get_encoder<ENCODING>().find(this->cstr(), this->length(), (typename text::char_type_by_type<cstr_t>::cstr_t)cstr, algorithms::string_length(cstr), start);
			}

			template<typename cstr_t>
			inline windex find_revert(cstr_t cstr, windex start = -1)const {
				return get_encoder<ENCODING>().find_revert(this->cstr(), this->length(), (typename text::char_type_by_type<cstr_t>::cstr_t)cstr, algorithms::string_length(cstr), start);
			}

			template<typename T>
			inline wsize sub_string(wsize sz, T* ptr, windex start = 0, windex end = -1) {
				return sub_string((pointer)ptr, start, start + min(sz, end - start), encoding_by_type<T>::encoding());
			}

			template<typename T>
			inline wsize sub_string(safe_str<T> ptr, windex start = 0, windex end = -1) {
				return sub_string((pointer)ptr.get(), start, start + min(ptr.size(), end - start), encoding_by_type<T>::encoding());
			}

			template<encoding_enum OTHER_ENCODING>
			inline wsize sub_string(string_base<OTHER_ENCODING>& str, windex start = 0, windex end = -1) {
				if (str.is_empty())
					str = new string_buffer<OTHER_ENCODING>();
				str->realloc(min(end, length()) - start, false);
				auto ptr = str->str();
				str->length(sub_string((pointer)ptr.get(), start, min(end, length()), OTHER_ENCODING));
				return str->length();
			}

			void copy(pointer raw, wsize sz, encoding_t);

		protected:
			void copy_at(windex, pointer raw, wsize sz, encoding_t);
			wsize sub_string(pointer raw, windex start, windex end, encoding_t)const;

		public:
			virtual comparision_result_t compare(object const& obj)const override;
			virtual wsize serialize(streams::ibinary_output_stream_t stream)const override;
			virtual wsize serialize(streams::itext_output_stream_t stream)const override;
			virtual pointer buffer_ptr()const override;
			virtual wsize buffer_size()const override;
			virtual wsize mem_copy(wsize, pointer, text::encoding_t = ENCODING) override;
			virtual ibuffer_view_t map_buffer(windex, wsize) override;
			virtual bool unmap_buffer(ibuffer_view_t&, wsize) override;
			virtual bool can_realloc_buffer()const override;
			virtual bool realloc_buffer(wsize) override;
			virtual text::encoding_t encoding()const override;
			virtual raw_str_t text_buffer() override;

		public:
			char_t& at(windex it);
			str_t begin();
			str_t end();
			char_t const& at(windex it)const;
			cstr_t begin()const;
			cstr_t end()const;
			void format(cstr_t format, ...);
			void concat_format(cstr_t format, ...);
			void invert();
			void invert(windex beg, windex end);
			void uppercase();
			void lowercase();

		};

		template<> inline int string_buffer<CURRENT_ENCODING>::compare(raw_str_t cstr)const {
			return get_encoder<ENCODING>().compare(this->cstr().cstr(), cstr._value, cstr._encoding);
		}
	}

	template<typename T>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper(safe_str<T> const& str)
		: object_wrapper() {
		set(new strings::string_buffer<ENCODING>(str));
	}

	//template<typename T>
	//inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper(T const* str)
	//	: object_wrapper() {
	//	set(new strings::string_buffer<ENCODING>(safe_str<const T>(str, -1)));
	//}

	template<text::encoding_enum OTHER_ENCODING>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper(object_wrapper<strings::string_buffer<OTHER_ENCODING>> const& str)
		: object_wrapper() {
		if (!str.is_empty())
			set(new strings::string_buffer<ENCODING>(str->cstr()));
	}

	template<typename T, wsize N>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::object_wrapper(const T(&ar)[N])
		: object_wrapper() {
		set(new strings::string_buffer<ENCODING>(safe_str<const T>(ar, N - 1)));
	}

	template<typename T>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator = (safe_str<T> const& str) {
		if (is_empty()) set(new strings::string_buffer<ENCODING>(str));
		else get()->copy(str);
		return*this;
	}

	template<typename T, wsize N>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator = (const T(&ar)[N]) {
		if (is_empty()) set(new strings::string_buffer<ENCODING>(safe_str<const T>(ar, N)));
		else get()->copy(safe_str<const T>(ar, N));
		return*this;
	}

	template<text::encoding_enum OTHER_ENCODING>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator = (object_wrapper<strings::string_buffer<OTHER_ENCODING>> const& str) {
		if (is_empty()) set(new strings::string_buffer<ENCODING>(str.get()));
		else if (!str.is_empty()) get()->copy(str->cstr());
		return*this;
	}

	template<typename T>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator += (safe_str<T> const& str) {
		if (is_empty()) set(new strings::string_buffer<ENCODING>(str));
		else get()->concat(str);
		return*this;
	}

	template<typename T, wsize N>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator += (const T(&ar)[N]) {
		if (is_empty()) set(new strings::string_buffer<ENCODING>(safe_str<const T>(ar, N)));
		else get()->concat(safe_str<const T>(ar, N));
		return*this;
	}

	template<text::encoding_enum OTHER_ENCODING>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& object_wrapper<strings::string_buffer<CURRENT_ENCODING>>::operator += (object_wrapper<strings::string_buffer<OTHER_ENCODING>> const& str) {
		if (is_empty()) set(new strings::string_buffer<ENCODING>(str.get()));
		else if (!str.is_empty()) get()->concat(str->cstr());
		return*this;
	}

	template<typename T>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>> operator + (
		object_wrapper<strings::string_buffer<CURRENT_ENCODING>> const& str1, safe_str<T> const& str2) {
		if (str1.is_empty())
		{
			return new strings::string_buffer<CURRENT_ENCODING>(str2);
		}
		else
		{
			object_wrapper<strings::string_buffer<CURRENT_ENCODING>> output;
			output = new strings::string_buffer<CURRENT_ENCODING>(str1->length() + str2.size());
			output->copy(str1->cstr());
			output->concat(str2);
			return ang::move(output);
		}
	}

	template<typename T>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>> operator + (
		safe_str<T> const& str1, object_wrapper<strings::string_buffer<CURRENT_ENCODING>> const& str2) {
		if (str2.is_empty())
		{
			return new strings::string_buffer<CURRENT_ENCODING>(str1);
		}
		else
		{
			object_wrapper<strings::string_buffer<CURRENT_ENCODING>> output;
			output = new strings::string_buffer<CURRENT_ENCODING>(str1.size() + str2->length());
			output->copy(str1);
			output->concat(str2->cstr());
			return ang::move(output);
		}
	}


	template<text::encoding_enum OTHER_ENCODING>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& operator << (object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& stream, object_wrapper<strings::string_buffer<OTHER_ENCODING>> const& value) {
		stream += value;
		return stream;
	}

	template<typename T>
	inline object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& operator << (object_wrapper<strings::string_buffer<CURRENT_ENCODING>>& stream, safe_str<T> const& value) {
		stream += value;
		return stream;
	}
}