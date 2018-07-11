
namespace ang
{
	template<>
	class LINK object_wrapper<streams::text_buffer_input_stream<CURRENT_ENCODING>>
	{
	public:
		typedef streams::text_buffer_input_stream<CURRENT_ENCODING>	type;

	protected:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(type*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		~object_wrapper();
	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;
	};

	template<>
	class LINK object_wrapper<streams::text_buffer_output_stream<CURRENT_ENCODING>>
	{
	public:
		typedef streams::text_buffer_output_stream<CURRENT_ENCODING> type;

	protected:
		type* _ptr;

	public:
		object_wrapper();
		object_wrapper(type*);
		object_wrapper(object_wrapper &&);
		object_wrapper(object_wrapper const&);
		~object_wrapper();

	public:
		void clean();
		bool is_empty()const;
		type* get(void)const;
		void set(type*);
		type ** addres_of(void);

	public:
		object_wrapper& operator = (type*);
		object_wrapper& operator = (object_wrapper &&);
		object_wrapper& operator = (object_wrapper const&);

		object_wrapper_ptr<type> operator & (void);
		type * operator -> (void);
		type const* operator -> (void)const;
		operator type * (void);
		operator type const* (void)const;
	};

	namespace streams
	{

		template<> class LINK text_buffer_input_stream<CURRENT_ENCODING> final
			: public object
			, public itext_input_stream<CURRENT_ENCODING>
		{
		private:
			text::itext_buffer_t<CURRENT_ENCODING> _buffer;
			stream_index_t _cursor;

		public:
			text_buffer_input_stream();
			text_buffer_input_stream(text_buffer_input_stream*);
			text_buffer_input_stream(text::itext_buffer<CURRENT_ENCODING>*);
			text_buffer_input_stream(ibuffer_t);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			virtual wsize read(pointer, wsize)override;
			virtual wsize read(char&)override;
			virtual wsize read(byte&)override;
			virtual wsize read(short&)override;
			virtual wsize read(ushort&)override;
			virtual wsize read(int&)override;
			virtual wsize read(uint&)override;
			virtual wsize read(long&)override;
			virtual wsize read(ulong&)override;
			virtual wsize read(long64&)override;
			virtual wsize read(ulong64&)override;
			virtual wsize read(float&)override;
			virtual wsize read(double&)override;
			virtual wsize seek(safe_str<text::char_type_by_encoding<CURRENT_ENCODING>::char_t const> const&)override; //read a specific sequence of characters
			virtual wsize read(strings::string_base<CURRENT_ENCODING>&, wsize max)override;
			virtual wsize read_line(strings::string_base<CURRENT_ENCODING>&, wsize max, array_view<text::char_type_by_encoding<CURRENT_ENCODING>::char_t> stop)override;

			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool end_of_stream()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			bool is_valid()const;
			ibuffer* buffer()const;
			bool attach(text::itext_buffer<CURRENT_ENCODING>*);
			bool attach(ibuffer* buff);
			bool can_move_to(stream_index_t size, stream_reference_t ref);
			pointer pointer_at(stream_index_t);

		private:

			bool forward(stream_index_t size);
			bool backward(stream_index_t size);
			bool can_forward(stream_index_t size);
			bool can_backward(stream_index_t size);

		private:
			virtual~text_buffer_input_stream();
		};


		template<> class LINK text_buffer_output_stream<CURRENT_ENCODING>
			: public object
			, public itext_output_stream<CURRENT_ENCODING>
		{
		private:
			ibuffer_t _buffer;
			stream_index_t _cursor;
			text::encoding_t _format;
			bool _is_formating_enabled;
			text::text_format_t char_format;
			text::text_format_t string_format;
			text::text_format_t signed_format;
			text::text_format_t unsigned_format;
			text::text_format_t floating_format;

		public:
			text_buffer_output_stream();
			text_buffer_output_stream(text_buffer_output_stream*);
			text_buffer_output_stream(text::itext_buffer<CURRENT_ENCODING>*);
			text_buffer_output_stream(ibuffer_t);

		public: //overrides
			ANG_DECLARE_INTERFACE();

			virtual wsize write(pointer, wsize)override;
			virtual wsize write(char)override;
			virtual wsize write(byte)override;
			virtual wsize write(short)override;
			virtual wsize write(ushort)override;
			virtual wsize write(int)override;
			virtual wsize write(uint)override;
			virtual wsize write(long)override;
			virtual wsize write(ulong)override;
			virtual wsize write(long64)override;
			virtual wsize write(ulong64)override;
			virtual wsize write(float)override;
			virtual wsize write(double)override;
			virtual wsize write(safe_str<text::char_type_by_encoding<CURRENT_ENCODING>::char_t const> const&)override;
			virtual wsize write_line(safe_str<text::char_type_by_encoding<CURRENT_ENCODING>::char_t const> const&)override;

			virtual bool command(special_command_t) override;
			virtual stream_index_t position()const override;
			virtual stream_size_t stream_size()const override;
			virtual bool move_to(stream_index_t size, stream_reference_t ref)override;

			virtual void enable_text_format(bool)override;
			virtual bool is_text_format_enabled()const override;
			virtual void text_format(cstr_t)override;
			virtual void text_format(text::text_format_t)override;

			bool is_valid()const;
			ibuffer* buffer()const;
			bool attach(text::itext_buffer*);
			bool attach(ibuffer* buff, text::encoding_t);
			bool can_move_to(stream_index_t size, stream_reference_t ref);

		private:
			pointer pointer_at(stream_index_t);
			bool forward(pointer, stream_index_t size);
			bool backward(pointer, stream_index_t size);
			bool can_forward(stream_index_t size);
			bool can_backward(stream_index_t size);

		private:
			virtual~text_buffer_output_stream();
		};

	}
}
